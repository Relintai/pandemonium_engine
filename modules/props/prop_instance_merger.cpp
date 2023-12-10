#include "prop_instance_merger.h"

#include "../mesh_data_resource/nodes/mesh_data_instance.h"

//#include "../thread_pool/thread_pool.h"

#include "scene/resources/world_3d.h"
#include "core/servers/rendering/rendering_server.h"

#include "core/config/engine.h"

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

#include "core/servers/rendering/rendering_server.h"

#include "modules/modules_enabled.gen.h"

#include "./singleton/prop_cache.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

#include "./props/prop_data_entry.h"
#include "./props/prop_data_light.h"
#include "./props/prop_data_prop.h"
#include "./props/prop_data_scene.h"
#include "./props/prop_data_static_body.h"

#include "jobs/prop_mesher_job_step.h"
#include "lights/prop_light.h"
#include "material_cache/prop_material_cache.h"
#include "scene/3d/camera.h"
#include "scene/main/viewport.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "./singleton/prop_cache.h"
#endif

#include "core/os/thread_pool.h"

#include "./props/prop_data_tiled_wall.h"

#include "tiled_wall/tiled_wall_data.h"

#include "core/scene/resources/shapes/box_shape.h"

const float PropInstanceMerger::LOD_CHECK_INTERVAL = 2;

bool PropInstanceMerger::get_building() {
	return _building;
}
void PropInstanceMerger::set_building(const bool value) {
	_building = value;

	set_physics_process_internal(_building);

	if (!_auto_lod_on) {
		set_process_internal(_building);
	}
}

int PropInstanceMerger::get_lod_level() {
	return _lod_level;
}
void PropInstanceMerger::set_lod_level(const int value) {
	_lod_level = value;

	if (_lod_level < 0) {
		_lod_level = 0;
	}

	apply_lod_level();
}

bool PropInstanceMerger::get_auto_lod() {
	return _auto_lod;
}
void PropInstanceMerger::set_auto_lod(const bool value) {
	_auto_lod = value;

	check_auto_lod();
}

float PropInstanceMerger::get_first_lod_distance_squared() {
	return _first_lod_distance_squared;
}
void PropInstanceMerger::set_first_lod_distance_squared(const float dist) {
	_first_lod_distance_squared = dist;
}

float PropInstanceMerger::get_lod_reduction_distance_squared() {
	return _lod_reduction_distance_squared;
}
void PropInstanceMerger::set_lod_reduction_distance_squared(const float dist) {
	_lod_reduction_distance_squared = dist;
}

Ref<PropInstanceJob> PropInstanceMerger::get_job() {
	return _job;
}
void PropInstanceMerger::set_job(const Ref<PropInstanceJob> &job) {
	_job = job;

	if (_job.is_valid() && is_inside_tree()) {
		_job->prop_instance_enter_tree();
	}
}

//Materials
Ref<Material> PropInstanceMerger::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

void PropInstanceMerger::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

int PropInstanceMerger::material_get_num() const {
	return _materials.size();
}

void PropInstanceMerger::materials_clear() {
	_materials.clear();
}

Vector<Variant> PropInstanceMerger::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void PropInstanceMerger::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Meshes
RID PropInstanceMerger::mesh_get(const int index) {
	ERR_FAIL_INDEX_V(index, _meshes.size(), RID());

	return _meshes[index].mesh;
}

RID PropInstanceMerger::mesh_instance_get(const int index) {
	ERR_FAIL_INDEX_V(index, _meshes.size(), RID());

	return _meshes[index].mesh_instance;
}

void PropInstanceMerger::mesh_add(const RID mesh_instance, const RID mesh) {
	MeshEntry e;
	e.mesh = mesh;
	e.mesh_instance = mesh_instance;

	_meshes.push_back(e);
}

int PropInstanceMerger::mesh_get_num() const {
	return _meshes.size();
}

void PropInstanceMerger::meshes_clear() {
	_meshes.clear();
}

void PropInstanceMerger::meshes_create(const int num) {
	free_meshes();

	for (int i = 0; i < num; ++i) {
		RID mesh_instance_rid = RS::get_singleton()->instance_create();

		if (get_world_3d().is_valid()) {
			RS::get_singleton()->instance_set_scenario(mesh_instance_rid, get_world_3d()->get_scenario());
		}

		RID mesh_rid = RS::get_singleton()->mesh_create();

		RS::get_singleton()->instance_set_base(mesh_instance_rid, mesh_rid);

		RS::get_singleton()->instance_set_transform(mesh_instance_rid, get_transform());

		RS::get_singleton()->instance_set_visible(mesh_instance_rid, false);

		MeshEntry e;
		e.mesh = mesh_rid;
		e.mesh_instance = mesh_instance_rid;

		_meshes.push_back(e);
	}

	apply_lod_level();
}

Vector<Variant> PropInstanceMerger::meshes_get() {
	Vector<Variant> r;
	for (int i = 0; i < _meshes.size(); i++) {
		Array a;

		a.push_back(_meshes[i].mesh);
		a.push_back(_meshes[i].mesh_instance);

		r.push_back(a);
	}
	return r;
}

void PropInstanceMerger::meshes_set(const Vector<Variant> &meshs) {
	_meshes.clear();

	for (int i = 0; i < _meshes.size(); i++) {
		Array arr = Array(meshs[i]);

		ERR_CONTINUE(arr.size() != 2);

		MeshEntry e;
		e.mesh = RID(arr[0]);
		e.mesh_instance = RID(arr[1]);

		_meshes.push_back(e);
	}
}

//Collider

Transform PropInstanceMerger::collider_local_transform_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform());

	return _colliders[index].transform;
}

RID PropInstanceMerger::collider_body_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}

Ref<Shape> PropInstanceMerger::collider_shape_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape>());

	return _colliders[index].shape;
}

RID PropInstanceMerger::collider_shape_rid_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}

int PropInstanceMerger::collider_add(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid, const RID &body, const bool owns_shape) {
	ERR_FAIL_COND_V(!shape.is_valid() && shape_rid == RID(), 0);

	int index = _colliders.size();

	ColliderBody e;
	e.transform = local_transform;
	e.body = body;
	e.shape = shape;
	e.shape_rid = shape_rid;
	e.owns_shape = owns_shape;

	_colliders.push_back(e);

	return index;
}

int PropInstanceMerger::collider_get_num() const {
	return _colliders.size();
}

void PropInstanceMerger::colliders_clear() {
	_colliders.clear();
}

Vector<Variant> PropInstanceMerger::colliders_get() {
	Vector<Variant> r;
	for (int i = 0; i < _colliders.size(); i++) {
		r.push_back(_colliders[i].body);
	}
	return r;
}

void PropInstanceMerger::colliders_set(const Vector<Variant> &colliders) {
	_colliders.clear();

	for (int i = 0; i < colliders.size(); i++) {
		RID collider = (colliders[i]);

		ColliderBody c;
		c.body = collider;

		_colliders.push_back(c);
	}
}

void PropInstanceMerger::check_auto_lod() {
	if (!_auto_lod) {
		_auto_lod_on = false;
		return;
	}

	if (_meshes.size() <= 1) {
		_auto_lod_on = false;

		if (!_building) {
			set_process_internal(false);
		}

		return;
	}

	_auto_lod_on = true;
	set_process_internal(true);
}

void PropInstanceMerger::apply_lod_level() {
	if (_meshes.size() == 0) {
		return;
	}

	RenderingServer *vs = RenderingServer::get_singleton();

	for (int i = 0; i < _meshes.size(); ++i) {
		RID mi = _meshes[i].mesh_instance;

		if (mi == RID()) {
			continue;
		}

		vs->instance_set_visible(mi, false);
	}

	if (!is_inside_tree()) {
		return;
	}

	if (!is_visible_in_tree()) {
		return;
	}

	int indx = _lod_level;

	if (_meshes.size() <= _lod_level) {
		indx = _meshes.size() - 1;
	}

	RID mi = _meshes[indx].mesh_instance;

	if (mi == RID()) {
		return;
	}

	vs->instance_set_visible(mi, true);
}

void PropInstanceMerger::debug_mesh_allocate() {
	if (_debug_mesh_rid == RID()) {
		_debug_mesh_rid = RenderingServer::get_singleton()->mesh_create();
	}

	if (_debug_mesh_instance == RID()) {
		_debug_mesh_instance = RenderingServer::get_singleton()->instance_create();

		if (get_world_3d().is_valid()) {
			RS::get_singleton()->instance_set_scenario(_debug_mesh_instance, get_world_3d()->get_scenario());
		}

		RS::get_singleton()->instance_set_base(_debug_mesh_instance, _debug_mesh_rid);
		RS::get_singleton()->instance_set_transform(_debug_mesh_instance, get_transform());
		RS::get_singleton()->instance_set_visible(_debug_mesh_instance, true);
	}
}
void PropInstanceMerger::debug_mesh_free() {
	if (_debug_mesh_instance != RID()) {
		RenderingServer::get_singleton()->free(_debug_mesh_instance);
	}

	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->free(_debug_mesh_rid);
	}
}
bool PropInstanceMerger::debug_mesh_has() {
	return _debug_mesh_rid != RID();
}
void PropInstanceMerger::debug_mesh_clear() {
	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->mesh_clear(_debug_mesh_rid);
	}
}
void PropInstanceMerger::debug_mesh_array_clear() {
	_debug_mesh_array.resize(0);
}
void PropInstanceMerger::debug_mesh_add_vertices_to(const PoolVector3Array &arr) {
	_debug_mesh_array.append_array(arr);

	if (_debug_mesh_array.size() % 2 == 1) {
		_debug_mesh_array.append(_debug_mesh_array[_debug_mesh_array.size() - 1]);
	}
}
void PropInstanceMerger::debug_mesh_send() {
	debug_mesh_allocate();
	debug_mesh_clear();

	if (_debug_mesh_array.size() == 0) {
		return;
	}

	SceneTree *st = SceneTree::get_singleton();

	Array arr;
	arr.resize(RenderingServer::ARRAY_MAX);
	arr[RenderingServer::ARRAY_VERTEX] = _debug_mesh_array;

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_debug_mesh_rid, RenderingServer::PRIMITIVE_LINES, arr);

	if (st) {
		RenderingServer::get_singleton()->mesh_surface_set_material(_debug_mesh_rid, 0, SceneTree::get_singleton()->get_debug_collision_material()->get_rid());
	}

	debug_mesh_array_clear();
}

void PropInstanceMerger::draw_debug_mdr_colliders() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	for (int i = 0; i < collider_get_num(); ++i) {
		Ref<Shape> shape = collider_shape_get(i);

		if (!shape.is_valid()) {
			continue;
		}

		Transform t = collider_local_transform_get(i);

		shape->add_vertices_to_array(_debug_mesh_array, t);
	}

	debug_mesh_send();
}

void PropInstanceMerger::free_meshes() {
	RID rid;

	for (int i = 0; i < _meshes.size(); ++i) {
		MeshEntry &e = _meshes.write[i];

		if (e.mesh_instance != rid) {
			RS::get_singleton()->free(e.mesh_instance);
		}

		if (e.mesh != rid) {
			RS::get_singleton()->free(e.mesh);
		}

		e.mesh_instance = rid;
		e.mesh = rid;
	}
}

void PropInstanceMerger::free_colliders() {
	for (int i = 0; i < _colliders.size(); ++i) {
		ColliderBody &e = _colliders.write[i];

		PhysicsServer::get_singleton()->free(e.body);

		e.body = RID();

		if (e.owns_shape) {
			e.shape.unref();
			e.shape_rid = RID();
		}
	}
}

void PropInstanceMerger::_init_materials() {
}

void PropInstanceMerger::_build() {
	if (_building) {
		return;
	}

	if (!is_inside_tree() || !get_world_3d().is_valid()) {
		queue_build();
		return;
	}

	set_building(true);
	_build_queued = false;

	if (_job.is_valid()) {
		_job->reset_meshes();
	}

	_job->clear_lights();

	for (int i = 0; i < get_child_count(); ++i) {
		Node *n = get_child(i);

		//this way we won't delete the user's nodes
		if (n->get_owner() == NULL) {
			n->queue_delete();
		}
	}

	if (!_prop_data.is_valid()) {
		set_building(false);
		return;
	}

	if (!is_inside_tree()) {
		set_building(false);
		_build_queued = true;
		return;
	}

	_job->reset();
	_job->set_complete(false);
	_job->set_cancelled(false);

	Ref<PropMaterialCache> cache = PropCache::get_singleton()->material_cache_get(_prop_data);

	if (cache->material_get_num() == 0) {
		//lock it!
		cache->mutex_lock();

		//check again, this thread might have gotten here after an another one already did the initialization!
		//this check might not be needed here
		if (cache->material_get_num() == 0) {
			//this will set up materials, and settings
			//needs to be called from the main thread!
			cache->initial_setup_default();
		}

		cache->mutex_unlock();
	}

	_job->set_material_cache(cache);

	prop_preprocess(Transform(), _prop_data);

	/*

Don't submit here, as it starts in physics process mode

	ThreadPool::get_singleton()->add_job(_job);
*/
}

void PropInstanceMerger::_build_finished() {
	set_building(false);

	apply_lod_level();
	check_auto_lod();

	notification(NOTIFICATION_TRANSFORM_CHANGED);

	if (_build_queued) {
		call_deferred("build");
	}
}

void PropInstanceMerger::_prop_preprocess(Transform transform, const Ref<PropData> &prop) {
	ERR_FAIL_COND(!prop.is_valid());

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<PropDataEntry> e = prop->get_prop(i);

		if (!e.is_valid()) {
			continue;
		}

		Transform t = transform * e->get_transform();

		Ref<PropDataProp> prop_entry_data = e;

		if (prop_entry_data.is_valid()) {
			Ref<PropData> p = prop_entry_data->get_prop();

			if (!p.is_valid()) {
				continue;
			}

			prop_preprocess(t, p);

			continue;
		}

		Ref<PropDataStaticBody> static_body_data = e;

		if (static_body_data.is_valid()) {
			for (int j = 0; j < static_body_data->get_collision_shape_count(); ++j) {
				Ref<Shape> collision_shape = static_body_data->get_collision_shape(j);

				if (collision_shape.is_valid()) {
					Transform et = t * static_body_data->get_collision_shape_transform(j);

					_job->add_collision_shape(collision_shape, et, true);
				}
			}

			continue;
		}

		Ref<PropDataTiledWall> tiled_wall_data = e;

		if (tiled_wall_data.is_valid()) {
			Ref<TiledWallData> twd = tiled_wall_data->get_data();

			if (twd.is_valid()) {
				_job->add_tiled_wall(tiled_wall_data, t);

				if (tiled_wall_data->get_collision()) {
					Ref<BoxShape> tws;
					tws.instance();

					float hew = tiled_wall_data->get_width() / 2.0;
					float heh = tiled_wall_data->get_heigth() / 2.0;

					tws->set_extents(Vector3(hew, heh, 0.01));

					Transform et = e->get_transform() * Transform(Basis(), Vector3(0, 0, twd->get_collider_z_offset()));
					Transform tt = transform * et;
					//tt.origin += Vector3(hew, heh, 0);
					tt.translate_local(hew, heh, 0);

					_job->add_collision_shape(tws, tt, true);
				}
			}

			continue;
		}

		Ref<PropDataScene> scene_data = e;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid()) {
				continue;
			}

			Node *n = sc->instance();
			add_child(n);
			n->set_owner(this);

			Spatial *sp = Object::cast_to<Spatial>(n);

			if (sp) {
				sp->set_transform(t);
			}

			continue;
		}

		//Will create a Terralight node, and prop
		//PropDataLight could use standard pandemonium light nodes
		Ref<PropDataLight> light_data = e;

		if (light_data.is_valid()) {
			Ref<PropLight> light;
			light.instance();

			Vector3 v = t.xform(Vector3());

			light->set_position(v);
			light->set_color(light_data->get_light_color());
			light->set_size(light_data->get_light_size());

			_job->add_light(light);

			continue;
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<PropDataMeshData> mesh_data = e;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid()) {
				continue;
			}

			_job->add_mesh(mesh_data, t);

			for (int j = 0; j < mdr->get_collision_shape_count(); ++j) {
				_job->add_collision_shape(mdr->get_collision_shape(j), t * mdr->get_collision_shape_offset(j));
			}
		}
#endif
	}
}

void PropInstanceMerger::collision_layer_changed() {
	for (int i = 0; i < _colliders.size(); ++i) {
		const ColliderBody &c = _colliders[i];

		if (c.body != RID()) {
			PhysicsServer::get_singleton()->body_set_collision_layer(c.body, _collision_layer);
		}
	}
}
void PropInstanceMerger::collision_mask_changed() {
	for (int i = 0; i < _colliders.size(); ++i) {
		const ColliderBody &c = _colliders[i];

		if (c.body != RID()) {
			PhysicsServer::get_singleton()->body_set_collision_mask(c.body, _collision_mask);
		}
	}
}

void PropInstanceMerger::_create_job() {
	_job = Ref<PropInstancePropJob>(memnew(PropInstancePropJob()));
	_job->set_prop_instace(this);

	Ref<PropMesherJobStep> js;

	js.instance();
	js->set_job_type(PropMesherJobStep::TYPE_NORMAL);
	_job->add_jobs_step(js);

	js.instance();
	js->set_job_type(PropMesherJobStep::TYPE_MERGE_VERTS);
	_job->add_jobs_step(js);

	js.instance();
	js->set_job_type(PropMesherJobStep::TYPE_BAKE_TEXTURE);
	_job->add_jobs_step(js);
}

PropInstanceMerger::PropInstanceMerger() {
	_build_queued = false;
	_auto_lod = true;
	_auto_lod_on = false;
	_lod_level = 0;

	//randomize so even if there is a lot they won't check for this at the same frame
	_lod_check_timer = Math::randf() * LOD_CHECK_INTERVAL;

	set_building(false);

	set_notify_transform(true);

	_first_lod_distance_squared = 1000;
	_lod_reduction_distance_squared = 600;
}

PropInstanceMerger::~PropInstanceMerger() {
	_job.unref();

	_prop_data.unref();

	_materials.clear();

	free_meshes();
	free_colliders();
	meshes_clear();
	colliders_clear();
}

void PropInstanceMerger::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			call("_create_job");
		} break;
		case NOTIFICATION_ENTER_TREE: {
			if (_job.is_valid()) {
				_job->prop_instance_enter_tree();
			}

			if (_prop_data.is_valid()) {
				call_deferred("build");
			}

			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			if (_job.is_valid()) {
				_job->prop_instance_exit_tree();
				_job->set_cancelled(true);
			}

			if (!_building) {
				free_meshes();
				free_colliders();
				meshes_clear();
				colliders_clear();
			}

			break;
		}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			if (_building) {
				if (!_job.is_valid()) {
					return;
				}

				if (_job->get_build_phase_type() == PropInstanceJob::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
					_job->physics_process(get_physics_process_delta_time());

					if (_job->get_build_phase_type() == PropInstanceJob::BUILD_PHASE_TYPE_NORMAL) {
						ThreadPool::get_singleton()->add_job(_job);
					}
				}
			}

			break;
		}
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (_building) {
				if (!_job.is_valid()) {
					return;
				}

				if (_job->get_build_phase_type() == PropInstanceJob::BUILD_PHASE_TYPE_PROCESS) {
					_job->process(get_process_delta_time());

					if (_job->get_build_phase_type() == PropInstanceJob::BUILD_PHASE_TYPE_NORMAL) {
						ThreadPool::get_singleton()->add_job(_job);
					}
				}
			} else {
				if (!_auto_lod_on) {
					return;
				}

				if (_meshes.size() == 0) {
					return;
				}

				_lod_check_timer += get_process_delta_time();

				if (_lod_check_timer > LOD_CHECK_INTERVAL) {
					_lod_check_timer = 0;

					if (!is_visible_in_tree()) {
						return;
					}

					SceneTree *st = get_tree();

					if (st) {
						Viewport *vp = st->get_root();

						if (vp) {
							Camera *cam = vp->get_camera();

							if (cam) {
								Vector3 cam_world_pos = cam->get_global_transform().xform(Vector3());
								Vector3 world_pos = get_global_transform().xform(Vector3());

								Vector3 dstv = cam_world_pos - world_pos;
								float dst = dstv.length_squared();

								if (dst <= _first_lod_distance_squared) {
									set_lod_level(0);
									return;
								}

								dst -= _first_lod_distance_squared;

								dst /= _lod_reduction_distance_squared;

								int dstl = static_cast<int>(dst);
								//the lod update method handles it if it's higher that the max generated lod level
								set_lod_level(dstl + 1);
							}
						}
					}
				}
			}

			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED: {
			Transform new_transform = get_global_transform();

			//Don't do this check, so this can be used to setmesh positions after a build
			//if (new_transform == _last_transform) {
			//	break;
			//}

			_last_transform = new_transform;

			RenderingServer *vs = RenderingServer::get_singleton();

			for (int i = 0; i < _meshes.size(); ++i) {
				RID mir = _meshes[i].mesh_instance;

				if (mir != RID()) {
					vs->instance_set_transform(mir, new_transform);
				}
			}

			if (_debug_mesh_instance != RID()) {
				vs->instance_set_transform(_debug_mesh_instance, new_transform);
			}

			for (int i = 0; i < _colliders.size(); ++i) {
				const ColliderBody &c = _colliders[i];

				if (c.body != RID()) {
					PhysicsServer::get_singleton()->body_set_shape_transform(c.body, 0, new_transform * c.transform);
				}
			}

			break;
		}
		case NOTIFICATION_VISIBILITY_CHANGED: {
			apply_lod_level();
			break;
		}
	}
}

void PropInstanceMerger::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_create_job"));
	ClassDB::bind_method(D_METHOD("_create_job"), &PropInstanceMerger::_create_job);

	ClassDB::bind_method(D_METHOD("get_job"), &PropInstanceMerger::get_job);
	ClassDB::bind_method(D_METHOD("set_job", "value"), &PropInstanceMerger::set_job);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "job", PROPERTY_HINT_RESOURCE_TYPE, "PropInstanceJob", 0), "set_job", "get_job");

	ClassDB::bind_method(D_METHOD("get_lod_level"), &PropInstanceMerger::get_lod_level);
	ClassDB::bind_method(D_METHOD("set_lod_level", "value"), &PropInstanceMerger::set_lod_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_level"), "set_lod_level", "get_lod_level");

	ClassDB::bind_method(D_METHOD("get_auto_lod"), &PropInstanceMerger::get_auto_lod);
	ClassDB::bind_method(D_METHOD("set_auto_lod", "value"), &PropInstanceMerger::set_auto_lod);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_lod"), "set_auto_lod", "get_auto_lod");

	ClassDB::bind_method(D_METHOD("get_first_lod_distance_squared"), &PropInstanceMerger::get_first_lod_distance_squared);
	ClassDB::bind_method(D_METHOD("set_first_lod_distance_squared", "value"), &PropInstanceMerger::set_first_lod_distance_squared);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "first_lod_distance_squared"), "set_first_lod_distance_squared", "get_first_lod_distance_squared");

	ClassDB::bind_method(D_METHOD("get_lod_reduction_distance_squared"), &PropInstanceMerger::get_lod_reduction_distance_squared);
	ClassDB::bind_method(D_METHOD("set_lod_reduction_distance_squared", "value"), &PropInstanceMerger::set_lod_reduction_distance_squared);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lod_reduction_distance_squared"), "set_lod_reduction_distance_squared", "get_lod_reduction_distance_squared");

	///Materials
	ClassDB::bind_method(D_METHOD("material_get", "index"), &PropInstanceMerger::material_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &PropInstanceMerger::material_add);
	ClassDB::bind_method(D_METHOD("material_get_num"), &PropInstanceMerger::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &PropInstanceMerger::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &PropInstanceMerger::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &PropInstanceMerger::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	//Meshes
	ClassDB::bind_method(D_METHOD("mesh_get", "index"), &PropInstanceMerger::mesh_get);
	ClassDB::bind_method(D_METHOD("mesh_instance_get", "index"), &PropInstanceMerger::mesh_instance_get);
	ClassDB::bind_method(D_METHOD("mesh_add", "mesh_instance", "mesh"), &PropInstanceMerger::mesh_add);
	ClassDB::bind_method(D_METHOD("mesh_get_num"), &PropInstanceMerger::mesh_get_num);
	ClassDB::bind_method(D_METHOD("meshes_clear"), &PropInstanceMerger::meshes_clear);

	ClassDB::bind_method(D_METHOD("meshes_get"), &PropInstanceMerger::meshes_get);
	ClassDB::bind_method(D_METHOD("meshes_set"), &PropInstanceMerger::meshes_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "meshes", PROPERTY_HINT_NONE, "", 0), "meshes_set", "meshes_get");

	//Colliders
	ClassDB::bind_method(D_METHOD("collider_local_transform_get", "index"), &PropInstanceMerger::collider_local_transform_get);
	ClassDB::bind_method(D_METHOD("collider_body_get", "index"), &PropInstanceMerger::collider_body_get);
	ClassDB::bind_method(D_METHOD("collider_shape_get", "index"), &PropInstanceMerger::collider_shape_get);
	ClassDB::bind_method(D_METHOD("collider_shape_rid_get", "index"), &PropInstanceMerger::collider_shape_rid_get);
	ClassDB::bind_method(D_METHOD("collider_add", "local_transform", "shape", "shape_rid", "body"), &PropInstanceMerger::collider_add);
	ClassDB::bind_method(D_METHOD("collider_get_num"), &PropInstanceMerger::collider_get_num);
	ClassDB::bind_method(D_METHOD("colliders_clear"), &PropInstanceMerger::colliders_clear);
	ClassDB::bind_method(D_METHOD("meshes_create", "num"), &PropInstanceMerger::meshes_create);

	//Colliders
	ClassDB::bind_method(D_METHOD("debug_mesh_allocate"), &PropInstanceMerger::debug_mesh_allocate);
	ClassDB::bind_method(D_METHOD("debug_mesh_free"), &PropInstanceMerger::debug_mesh_free);
	ClassDB::bind_method(D_METHOD("debug_mesh_has"), &PropInstanceMerger::debug_mesh_has);
	ClassDB::bind_method(D_METHOD("debug_mesh_clear"), &PropInstanceMerger::debug_mesh_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_array_clear"), &PropInstanceMerger::debug_mesh_array_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_add_vertices_to", "arr"), &PropInstanceMerger::debug_mesh_add_vertices_to);
	ClassDB::bind_method(D_METHOD("debug_mesh_send"), &PropInstanceMerger::debug_mesh_send);
	ClassDB::bind_method(D_METHOD("draw_debug_mdr_colliders"), &PropInstanceMerger::draw_debug_mdr_colliders);

	ClassDB::bind_method(D_METHOD("check_auto_lod"), &PropInstanceMerger::check_auto_lod);
	ClassDB::bind_method(D_METHOD("apply_lod_level"), &PropInstanceMerger::apply_lod_level);

	//---
	ClassDB::bind_method(D_METHOD("free_meshes"), &PropInstanceMerger::free_meshes);
	ClassDB::bind_method(D_METHOD("free_colliders"), &PropInstanceMerger::free_colliders);
}
