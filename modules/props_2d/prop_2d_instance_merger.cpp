#include "prop_2d_instance_merger.h"

#include "../mesh_data_resource/nodes/mesh_data_instance.h"

//#include "../thread_pool/thread_pool.h"
#include "scene/resources/world_2d.h"
#include "servers/physics_server.h"

#include "core/config/engine.h"

#include "modules/modules_enabled.gen.h"

#include "./singleton/prop_2d_cache.h"

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

#include "servers/rendering_server.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif

#include "./props/prop_2d_data_entry.h"
#include "./props/prop_2d_data_light.h"
#include "./props/prop_2d_data_prop.h"
#include "./props/prop_2d_data_scene.h"
#include "lights/prop_2d_light.h"
#include "material_cache/prop_2d_material_cache.h"
#include "scene/3d/camera.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "./singleton/prop_2d_cache.h"
#endif

#include "core/os/thread_pool.h"

#include "./props/prop_2d_data_tiled_wall_2d.h"

#include "tiled_wall/tiled_wall_2d_data.h"

#include "scene/resources/box_shape.h"

bool Prop2DInstanceMerger::get_building() {
	return _building;
}
void Prop2DInstanceMerger::set_building(const bool value) {
	_building = value;

	set_physics_process_internal(_building);
	set_process_internal(_building);
}

Ref<Prop2DInstanceJob> Prop2DInstanceMerger::get_job() {
	return _job;
}
void Prop2DInstanceMerger::set_job(const Ref<Prop2DInstanceJob> &job) {
	_job = job;

	if (_job.is_valid() && is_inside_tree()) {
		_job->prop_instance_enter_tree();
	}
}

//Meshes
RID Prop2DInstanceMerger::mesh_get(const int index) {
	ERR_FAIL_INDEX_V(index, _meshes.size(), RID());

	return _meshes[index];
}

void Prop2DInstanceMerger::mesh_add(const RID mesh) {
	_meshes.push_back(mesh);
}

int Prop2DInstanceMerger::mesh_get_num() const {
	return _meshes.size();
}

void Prop2DInstanceMerger::meshes_clear() {
	_meshes.clear();
}

void Prop2DInstanceMerger::meshes_create(const int num) {
	free_meshes();

	for (int i = 0; i < num; ++i) {
		RID mesh_rid = RS::get_singleton()->mesh_create();

		_meshes.push_back(mesh_rid);
	}
}

Vector<Variant> Prop2DInstanceMerger::meshes_get() {
	Vector<Variant> r;
	for (int i = 0; i < _meshes.size(); i++) {
		r.push_back(_meshes[i]);
	}
	return r;
}

void Prop2DInstanceMerger::meshes_set(const Vector<Variant> &meshs) {
	_meshes.clear();

	for (int i = 0; i < _meshes.size(); i++) {
		_meshes.push_back(meshs[i]);
	}
}

//Collider

Transform2D Prop2DInstanceMerger::collider_local_transform_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform2D());

	return _colliders[index].transform;
}

RID Prop2DInstanceMerger::collider_body_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}

Ref<Shape2D> Prop2DInstanceMerger::collider_shape_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape2D>());

	return _colliders[index].shape;
}

RID Prop2DInstanceMerger::collider_shape_rid_get(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}

int Prop2DInstanceMerger::collider_add(const Transform2D &local_transform, const Ref<Shape2D> &shape, const RID &shape_rid, const RID &body, const bool owns_shape) {
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

int Prop2DInstanceMerger::collider_get_num() const {
	return _colliders.size();
}

void Prop2DInstanceMerger::colliders_clear() {
	_colliders.clear();
}

Vector<Variant> Prop2DInstanceMerger::colliders_get() {
	Vector<Variant> r;
	for (int i = 0; i < _colliders.size(); i++) {
		r.push_back(_colliders[i].body);
	}
	return r;
}

void Prop2DInstanceMerger::colliders_set(const Vector<Variant> &colliders) {
	_colliders.clear();

	for (int i = 0; i < colliders.size(); i++) {
		RID collider = (colliders[i]);

		ColliderBody c;
		c.body = collider;

		_colliders.push_back(c);
	}
}

void Prop2DInstanceMerger::debug_mesh_allocate() {
	if (_debug_mesh_rid == RID()) {
		_debug_mesh_rid = RenderingServer::get_singleton()->mesh_create();
	}
}
void Prop2DInstanceMerger::debug_mesh_free() {
	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->free(_debug_mesh_rid);
	}
}
bool Prop2DInstanceMerger::debug_mesh_has() {
	return _debug_mesh_rid != RID();
}
void Prop2DInstanceMerger::debug_mesh_clear() {
	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->mesh_clear(_debug_mesh_rid);
	}
}
void Prop2DInstanceMerger::debug_mesh_array_clear() {
	_debug_mesh_array.resize(0);
}
void Prop2DInstanceMerger::debug_mesh_add_vertices_to(const PoolVector2Array &arr) {
	_debug_mesh_array.append_array(arr);

	if (_debug_mesh_array.size() % 2 == 1) {
		_debug_mesh_array.append(_debug_mesh_array[_debug_mesh_array.size() - 1]);
	}
}
void Prop2DInstanceMerger::debug_mesh_send() {
	debug_mesh_allocate();
	debug_mesh_clear();

	if (_debug_mesh_array.size() == 0)
		return;

	//SceneTree *st = SceneTree::get_singleton();

	Array arr;
	arr.resize(RenderingServer::ARRAY_MAX);
	arr[RenderingServer::ARRAY_VERTEX] = _debug_mesh_array;

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_debug_mesh_rid, RenderingServer::PRIMITIVE_LINES, arr);

	debug_mesh_array_clear();
}

void Prop2DInstanceMerger::draw_debug_mdr_colliders() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	for (int i = 0; i < collider_get_num(); ++i) {
		Ref<Shape2D> shape = collider_shape_get(i);

		if (!shape.is_valid()) {
			continue;
		}

		Transform2D t = collider_local_transform_get(i);

		RenderingServer::get_singleton()->canvas_item_add_set_transform(get_canvas_item(), t);
		shape->draw(get_canvas_item(), Color(1, 1, 1, 1));
		RenderingServer::get_singleton()->canvas_item_add_set_transform(get_canvas_item(), Transform2D());
	}

	debug_mesh_send();
}

void Prop2DInstanceMerger::free_meshes() {
	RID rid;

	for (int i = 0; i < _meshes.size(); ++i) {
		RID &e = _meshes.write[i];

		if (e != rid) {
			RS::get_singleton()->free(e);
		}

		e = rid;
	}
}

void Prop2DInstanceMerger::free_colliders() {
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

void Prop2DInstanceMerger::_init_materials() {
}

void Prop2DInstanceMerger::_build() {
	if (_building) {
		return;
	}

	if (!is_inside_tree() || !get_world_2d().is_valid()) {
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

	Ref<Prop2DMaterialCache> cache = Prop2DCache::get_singleton()->material_cache_get(_prop_data);

	//TODO this needs to be changed
	if (!cache->material_get().is_valid()) {
		//lock it!
		cache->mutex_lock();

		//check again, this thread might have gotten here after an another one already did the initialization!
		//this check might not be needed here
		if (!cache->material_get().is_valid()) {
			//this will set up materials, and settings
			//needs to be called from the main thread!
			cache->initial_setup_default();
		}

		cache->mutex_unlock();
	}

	_job->set_material_cache(cache);

	prop_preprocess(Transform2D(), _prop_data);

	/*

Don't submit here, as it starts in physics process mode

	ThreadPool::get_singleton()->add_job(_job);
*/
}

void Prop2DInstanceMerger::_build_finished() {
	set_building(false);

	notification(NOTIFICATION_TRANSFORM_CHANGED);

	if (_build_queued) {
		call_deferred("build");
	}
}

void Prop2DInstanceMerger::_prop_preprocess(Transform2D transform, const Ref<Prop2DData> &prop) {
	ERR_FAIL_COND(!prop.is_valid());

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<Prop2DDataEntry> e = prop->get_prop(i);

		if (!e.is_valid())
			continue;

		Transform2D t = transform * e->get_transform_2d();

		Ref<Prop2DDataProp2D> prop_entry_data = e;

		if (prop_entry_data.is_valid()) {
			Ref<Prop2DData> p = prop_entry_data->get_prop();

			if (!p.is_valid())
				continue;

			prop_preprocess(t, p);

			continue;
		}

		Ref<Prop2DDataTiledWall2D> tiled_wall_data = e;

		if (tiled_wall_data.is_valid()) {
			_job->add_tiled_wall(tiled_wall_data, t);

			if (tiled_wall_data->get_collision()) {
				Ref<BoxShape> tws;
				tws.instance();

				float hew = tiled_wall_data->get_width() / 2.0;
				float heh = tiled_wall_data->get_heigth() / 2.0;

				tws->set_extents(Vector3(hew, heh, 0.01));

				Transform2D tt = t;
				//tt.origin += Vector3(hew, heh, 0);
				tt.translate_local(hew, heh);

				_job->add_collision_shape(tws, tt, true);
			}

			continue;
		}

		Ref<Prop2DDataScene> scene_data = e;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid())
				continue;

			Node *n = sc->instance();
			add_child(n);
			n->set_owner(this);

			Node2D *sp = Object::cast_to<Node2D>(n);

			if (sp) {
				sp->set_transform(t);
			}

			continue;
		}

		//Will create a Terralight node, and prop
		//Prop2DDataLight could use standard pandemonium light nodes
		Ref<Prop2DDataLight> light_data = e;

		if (light_data.is_valid()) {
			Ref<Prop2DLight> light;
			light.instance();

			Vector2 v = t.xform(Vector2());

			light->set_position(v);
			light->set_color(light_data->get_light_color());
			//light->set_size(light_data->get_light_size());

			_job->add_light(light);

			continue;
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<Prop2DDataMeshData> mesh_data = e;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid())
				continue;

			_job->add_mesh(mesh_data, t);
			/*
						for (int j = 0; j < mdr->get_collision_shape_count(); ++j) {
							_job->add_collision_shape(mdr->get_collision_shape(j), t * mdr->get_collision_shape_offset(j));
						}
						*/
		}
#endif
	}
}

void Prop2DInstanceMerger::collision_layer_changed() {
	for (int i = 0; i < _colliders.size(); ++i) {
		const ColliderBody &c = _colliders[i];

		if (c.body != RID()) {
			PhysicsServer::get_singleton()->body_set_collision_layer(c.body, _collision_layer);
		}
	}
}
void Prop2DInstanceMerger::collision_mask_changed() {
	for (int i = 0; i < _colliders.size(); ++i) {
		const ColliderBody &c = _colliders[i];

		if (c.body != RID()) {
			PhysicsServer::get_singleton()->body_set_collision_mask(c.body, _collision_mask);
		}
	}
}

void Prop2DInstanceMerger::_create_job() {
	_job = Ref<Prop2DInstanceProp2DJob>(memnew(Prop2DInstanceProp2DJob()));
	_job->set_prop_instace(this);
}

Prop2DInstanceMerger::Prop2DInstanceMerger() {
	_build_queued = false;

	set_building(false);

	set_notify_transform(true);
}

Prop2DInstanceMerger::~Prop2DInstanceMerger() {
	_job.unref();

	_prop_data.unref();

	free_meshes();
	free_colliders();
	meshes_clear();
	colliders_clear();
}

void Prop2DInstanceMerger::_notification(int p_what) {
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

				if (_job->get_build_phase_type() == Prop2DInstanceJob::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
					_job->physics_process(get_physics_process_delta_time());

					if (_job->get_build_phase_type() == Prop2DInstanceJob::BUILD_PHASE_TYPE_NORMAL) {
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

				if (_job->get_build_phase_type() == Prop2DInstanceJob::BUILD_PHASE_TYPE_PROCESS) {
					_job->process(get_process_delta_time());

					if (_job->get_build_phase_type() == Prop2DInstanceJob::BUILD_PHASE_TYPE_NORMAL) {
						ThreadPool::get_singleton()->add_job(_job);
					}
				}
			}

			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED: {
			Transform2D new_transform = get_global_transform();

			//Don't do this check, so this can be used to setmesh positions after a build
			//if (new_transform == _last_transform) {
			//	break;
			//}

			_last_transform = new_transform;
			/*
						for (int i = 0; i < _colliders.size(); ++i) {
							const ColliderBody &c = _colliders[i];

							if (c.body != RID()) {
								PhysicsServer::get_singleton()->body_set_shape_transform(c.body, 0, new_transform * c.transform);
							}
						}
			*/
			break;
		}
		case NOTIFICATION_DRAW: {
			break;
		}
	}
}

void Prop2DInstanceMerger::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_create_job"));
	ClassDB::bind_method(D_METHOD("_create_job"), &Prop2DInstanceMerger::_create_job);

	ClassDB::bind_method(D_METHOD("get_job"), &Prop2DInstanceMerger::get_job);
	ClassDB::bind_method(D_METHOD("set_job", "value"), &Prop2DInstanceMerger::set_job);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "job", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DInstanceJob", 0), "set_job", "get_job");

	//Meshes
	ClassDB::bind_method(D_METHOD("mesh_get", "index"), &Prop2DInstanceMerger::mesh_get);
	ClassDB::bind_method(D_METHOD("mesh_add", "mesh"), &Prop2DInstanceMerger::mesh_add);
	ClassDB::bind_method(D_METHOD("mesh_get_num"), &Prop2DInstanceMerger::mesh_get_num);
	ClassDB::bind_method(D_METHOD("meshes_clear"), &Prop2DInstanceMerger::meshes_clear);

	ClassDB::bind_method(D_METHOD("meshes_get"), &Prop2DInstanceMerger::meshes_get);
	ClassDB::bind_method(D_METHOD("meshes_set"), &Prop2DInstanceMerger::meshes_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "meshes", PROPERTY_HINT_NONE, "", 0), "meshes_set", "meshes_get");

	//Colliders
	ClassDB::bind_method(D_METHOD("collider_local_transform_get", "index"), &Prop2DInstanceMerger::collider_local_transform_get);
	ClassDB::bind_method(D_METHOD("collider_body_get", "index"), &Prop2DInstanceMerger::collider_body_get);
	ClassDB::bind_method(D_METHOD("collider_shape_get", "index"), &Prop2DInstanceMerger::collider_shape_get);
	ClassDB::bind_method(D_METHOD("collider_shape_rid_get", "index"), &Prop2DInstanceMerger::collider_shape_rid_get);
	ClassDB::bind_method(D_METHOD("collider_add", "local_transform", "shape", "shape_rid", "body"), &Prop2DInstanceMerger::collider_add);
	ClassDB::bind_method(D_METHOD("collider_get_num"), &Prop2DInstanceMerger::collider_get_num);
	ClassDB::bind_method(D_METHOD("colliders_clear"), &Prop2DInstanceMerger::colliders_clear);
	ClassDB::bind_method(D_METHOD("meshes_create", "num"), &Prop2DInstanceMerger::meshes_create);

	//Colliders
	ClassDB::bind_method(D_METHOD("debug_mesh_allocate"), &Prop2DInstanceMerger::debug_mesh_allocate);
	ClassDB::bind_method(D_METHOD("debug_mesh_free"), &Prop2DInstanceMerger::debug_mesh_free);
	ClassDB::bind_method(D_METHOD("debug_mesh_has"), &Prop2DInstanceMerger::debug_mesh_has);
	ClassDB::bind_method(D_METHOD("debug_mesh_clear"), &Prop2DInstanceMerger::debug_mesh_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_array_clear"), &Prop2DInstanceMerger::debug_mesh_array_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_add_vertices_to", "arr"), &Prop2DInstanceMerger::debug_mesh_add_vertices_to);
	ClassDB::bind_method(D_METHOD("debug_mesh_send"), &Prop2DInstanceMerger::debug_mesh_send);
	ClassDB::bind_method(D_METHOD("draw_debug_mdr_colliders"), &Prop2DInstanceMerger::draw_debug_mdr_colliders);

	//---
	ClassDB::bind_method(D_METHOD("free_meshes"), &Prop2DInstanceMerger::free_meshes);
	ClassDB::bind_method(D_METHOD("free_colliders"), &Prop2DInstanceMerger::free_colliders);
}
