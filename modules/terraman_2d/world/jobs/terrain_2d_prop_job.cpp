/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "terrain_2d_prop_job.h"

#include "../../defines.h"

#include "../../library/terrain_2d_library.h"
#include "../../library/terrain_2d_material_cache.h"
#include "../../library/terrain_2d_surface.h"

#include "../../meshers/default/terrain_2d_mesher_default.h"
#include "../../meshers/terrain_2d_mesher.h"
#include "../default/terrain_2d_chunk_default.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "../../world/default/terrain_2d_world_default.h"

#ifdef MODULE_MESH_UTILS_ENABLED
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<Terrain2DMesher> Terrain2DProp2DJob::get_prop_mesher() const {
	return _prop_mesher;
}
void Terrain2DProp2DJob::set_prop_mesher(const Ref<Terrain2DMesher> &mesher) {
	_prop_mesher = mesher;
}

void Terrain2DProp2DJob::phase_physics_process() {
	ERR_FAIL_COND(!_chunk.is_valid());
	
	/*
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	//TODO this should only update the differences
	for (int i = 0; i < chunk->collider_get_count(); ++i) {
		PhysicsServer::get_singleton()->free(chunk->collider_get_body(i));
	}

	chunk->colliders_clear();
*/
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
//TODO
/*
	for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
		Ref<MeshDataResource> mdr = chunk->mesh_data_resource_get(i);

		for (int j = 0; j < mdr->get_collision_shape_count(); ++j) {
			Ref<Shape> shape = mdr->get_collision_shape(j);
			Transform offset = mdr->get_collision_shape_offset(j);

			if (!shape.is_valid()) {
				continue;
			}

			RID body = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

			Transform transform = chunk->mesh_data_resource_get_transform(i);
			transform *= offset;

			PhysicsServer::get_singleton()->body_add_shape(body, shape->get_rid());

			//TODO store the layer mask somewhere
			PhysicsServer::get_singleton()->body_set_collision_layer(body, 1);
			PhysicsServer::get_singleton()->body_set_collision_mask(body, 1);

			if (chunk->get_voxel_world()->is_inside_tree() && chunk->get_voxel_world()->is_inside_world()) {
				Ref<World3D> world = chunk->get_voxel_world()->get_world_2d();

				if (world.is_valid() && world->get_space() != RID()) {
					PhysicsServer::get_singleton()->body_set_space(body, world->get_space());
				}
			}

			PhysicsServer::get_singleton()->body_set_state(body, PhysicsServer::BODY_STATE_TRANSFORM, chunk->get_transform() * transform);

			chunk->collider_add(transform, shape, shape->get_rid(), body);
		}
	}
	*/
#endif
	/*
	#if TOOLS_ENABLED
		if (SceneTree::get_singleton()->is_debugging_collisions_hint() && chunk->collider_get_count() > 0) {
			chunk->draw_debug_mdr_colliders();
		}
	#endif
	*/
	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	next_phase();
}

void Terrain2DProp2DJob::phase_prop() {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	if (!get_prop_mesher().is_valid()) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		return;
	}

	if (should_do()) {
		if (chunk->mesh_data_resource_get_count() == 0) {
			reset_stages();
			set_complete(true); //So threadpool knows it's done
			next_job();
			return;
		}

		for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
			if (chunk->mesh_data_resource_get_is_inside(i)) {
				get_prop_mesher()->add_mesh_data_resource_transform(chunk->mesh_data_resource_get(i), chunk->mesh_data_resource_get_transform(i), chunk->mesh_data_resource_get_uv_rect(i));
			}
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			get_prop_mesher()->bake_colors(_chunk);
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		//TODO
		/*
		if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			Terrain2DWorldDefault *world = Object::cast_to<Terrain2DWorldDefault>(chunk->get_voxel_world());

			if (world) {
				for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
					if (!chunk->mesh_data_resource_get_is_inside(i)) {
						Ref<MeshDataResource> mdr = chunk->mesh_data_resource_get(i);

						ERR_CONTINUE(!mdr.is_valid());

						Transform trf = chunk->mesh_data_resource_get_transform(i);

						Array arr = mdr->get_array();

						if (arr.size() <= Mesh::ARRAY_VERTEX) {
							continue;
						}

						PoolVector3Array varr = arr[Mesh::ARRAY_VERTEX];

						if (varr.size() == 0) {
							continue;
						}

						PoolColorArray carr = world->get_vertex_colors(trf, varr);

						get_prop_mesher()->add_mesh_data_resource_transform_colored(mdr, trf, carr, chunk->mesh_data_resource_get_uv_rect(i));
					}
				}
			}
		}
		*/
	}

	if (get_prop_mesher()->get_vertex_count() == 0) {
		reset_stages();

		set_complete(true); //So threadpool knows it's done
		next_job();
		return;
	}

#endif

	reset_stages();
	next_phase();
}

void Terrain2DProp2DJob::_physics_process(float delta) {
	if (_phase == 0)
		phase_physics_process();
}

void Terrain2DProp2DJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<Terrain2DLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	Ref<Terrain2DChunkDefault> chunk = _chunk;

	if (!chunk.is_valid()
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
			|| chunk->mesh_data_resource_get_count() == 0
#endif
	) {
		set_complete(true);
		next_job();
		return;
	}

	if (_phase == 1) {
		phase_setup();
	} else if (_phase == 2) {
		phase_prop();
	} else if (_phase == 3) {
		phase_steps();
	} else if (_phase > 3) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("Terrain2DProp2DJob: _phase is too high!");
	}
}

void Terrain2DProp2DJob::_reset() {
	Terrain2DJob::_reset();

	_build_done = false;
	_phase = 0;

	if (get_prop_mesher().is_valid()) {
		get_prop_mesher()->reset();
		get_prop_mesher()->set_library(_chunk->get_library());
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
}

void Terrain2DProp2DJob::phase_setup() {
	Ref<Terrain2DLibrary> library = _chunk->get_library();

	if (!library->supports_caching()) {
		next_phase();
		return;
	}

	if (library->supports_caching()) {
		if (!_chunk->prop_material_cache_key_has()) {
			library->prop_material_cache_get_key(_chunk);

			if (!_chunk->prop_material_cache_key_has()) {
				//chunk does not need a key
				next_phase();
				return;
			}
		}

		Ref<Terrain2DMaterialCache> cache = library->prop_material_cache_get(_chunk->prop_material_cache_key_get());

		//Note: without threadpool and threading none of this can happen, as cache will get initialized the first time a thread requests it!
		while (!cache->get_initialized()) {
			//Means it's currently merging the atlases on a different thread.
			//Let's just wait
			OS::get_singleton()->delay_usec(100);
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		for (int i = 0; i < _chunk->mesh_data_resource_get_count(); ++i) {
			Ref<Texture> tex = _chunk->mesh_data_resource_get_texture(i);

			if (!tex.is_valid())
				continue;

			Rect2 r = cache->additional_texture_get_uv_rect(tex);

			_chunk->mesh_data_resource_set_uv_rect(i, r);
		}
#endif
	}

	next_phase();

	if (should_return()) {
		return;
	}
}

void Terrain2DProp2DJob::phase_steps() {
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!_prop_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if (_prop_mesher->get_vertex_count() == 0) {
		reset_stages();
		//next_phase();
		set_complete(true); //So threadpool knows it's done
		next_job();

		return;
	}

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_PROP, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);

		if (mesh_rid == RID()) {
			//need to allocate the meshes

			//allocate
			chunk->mesh_create(Terrain2DChunkDefault::MESH_INDEX_PROP);

		} else {
			mesh_rid = chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_PROP, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);

			if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
				RS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
		}
	}

	step_type_normal();

	reset_stages();
	//next_phase();

	set_complete(true); //So threadpool knows it's done
	next_job();
}

void Terrain2DProp2DJob::step_type_normal() {
	//TODO add a lighting generation step

	Ref<Terrain2DChunkDefault> chunk = _chunk;

	temp_mesh_arr = _prop_mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_PROP, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);
}

Terrain2DProp2DJob::Terrain2DProp2DJob() {
	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
}

Terrain2DProp2DJob::~Terrain2DProp2DJob() {
}

void Terrain2DProp2DJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_mesher"), &Terrain2DProp2DJob::get_prop_mesher);
	ClassDB::bind_method(D_METHOD("set_prop_mesher", "mesher"), &Terrain2DProp2DJob::set_prop_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_mesher", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DMesher", 0), "set_prop_mesher", "get_prop_mesher");

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &Terrain2DProp2DJob::_physics_process);
}
