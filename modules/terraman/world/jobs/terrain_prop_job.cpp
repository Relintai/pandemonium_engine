/*************************************************************************/
/*  terrain_prop_job.cpp                                                 */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "terrain_prop_job.h"

#include "../../defines.h"

#include "../../library/terrain_library.h"
#include "../../library/terrain_material_cache.h"
#include "../../library/terrain_surface.h"

#include "../../meshers/default/terrain_mesher_default.h"
#include "../../meshers/terrain_mesher.h"
#include "../default/terrain_chunk_default.h"
#include "scene/resources/material/shader_material.h"
#include "scene/resources/material/spatial_material.h"
#include "scene/resources/world_3d.h"
#include "servers/physics_server.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "../../world/default/terrain_world_default.h"

#ifdef MODULE_MESH_UTILS_ENABLED
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

#ifdef MODULE_PROPS_ENABLED
#include "../../../props/props/prop_data_static_body.h"
#endif

Ref<TerrainMesher> TerrainPropJob::get_prop_mesher() const {
	return _prop_mesher;
}
void TerrainPropJob::set_prop_mesher(const Ref<TerrainMesher> &mesher) {
	_prop_mesher = mesher;
}

Ref<TerrainMesherJobStep> TerrainPropJob::get_jobs_step(int index) const {
	ERR_FAIL_INDEX_V(index, _job_steps.size(), Ref<TerrainMesherJobStep>());

	return _job_steps.get(index);
}
void TerrainPropJob::set_jobs_step(int index, const Ref<TerrainMesherJobStep> &step) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.set(index, step);
}
void TerrainPropJob::remove_jobs_step(const int index) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.remove(index);
}
void TerrainPropJob::add_jobs_step(const Ref<TerrainMesherJobStep> &step) {
	_job_steps.push_back(step);
}
int TerrainPropJob::get_jobs_step_count() const {
	return _job_steps.size();
}

void TerrainPropJob::phase_physics_process() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<TerrainChunkDefault> chunk = _chunk;

	//TODO this should only update the differences
	for (int i = 0; i < chunk->collider_get_count(); ++i) {
		PhysicsServer::get_singleton()->free(chunk->collider_get_body(i));
	}

	chunk->colliders_clear();

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
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

			if (chunk->get_terrain_world()->is_inside_tree() && chunk->get_terrain_world()->is_inside_world()) {
				Ref<World3D> world = chunk->get_terrain_world()->get_world_3d();

				if (world.is_valid() && world->get_space() != RID()) {
					PhysicsServer::get_singleton()->body_set_space(body, world->get_space());
				}
			}

			PhysicsServer::get_singleton()->body_set_state(body, PhysicsServer::BODY_STATE_TRANSFORM, chunk->get_transform() * transform);

			chunk->collider_add(transform, shape, shape->get_rid(), body);
		}
	}
#endif

#ifdef MODULE_PROPS_ENABLED
	for (int i = 0; i < chunk->prop_get_count(); ++i) {
		Ref<PropData> prop = chunk->prop_get(i);
		Transform prop_transform = chunk->prop_get_transform(i);
		Transform chunk_prop_local_tform = prop_transform;
		chunk_prop_local_tform.origin = chunk->to_local(chunk_prop_local_tform.origin);

		for (int j = 0; j < prop->get_prop_count(); ++j) {
			Ref<PropDataStaticBody> psb = prop->get_prop(j);

			if (!psb.is_valid()) {
				continue;
			}

			if (psb->get_collision_shape_count() == 0) {
				continue;
			}

			for (int k = 0; k < psb->get_collision_shape_count(); ++k) {
				Ref<Shape> shape = psb->get_collision_shape(k);
				Transform offset = psb->get_collision_shape_transform(k);

				if (!shape.is_valid()) {
					continue;
				}

				RID body = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

				Transform transform = chunk_prop_local_tform;
				transform *= offset;

				PhysicsServer::get_singleton()->body_add_shape(body, shape->get_rid());

				//TODO store the layer mask somewhere
				PhysicsServer::get_singleton()->body_set_collision_layer(body, 1);
				PhysicsServer::get_singleton()->body_set_collision_mask(body, 1);

				if (chunk->get_terrain_world()->is_inside_tree() && chunk->get_terrain_world()->is_inside_world()) {
					Ref<World3D> world = chunk->get_terrain_world()->get_world_3d();

					if (world.is_valid() && world->get_space() != RID()) {
						PhysicsServer::get_singleton()->body_set_space(body, world->get_space());
					}
				}

				PhysicsServer::get_singleton()->body_set_state(body, PhysicsServer::BODY_STATE_TRANSFORM, chunk->get_transform() * transform);

				chunk->collider_add(transform, shape, shape->get_rid(), body);
			}
		}
	}
#endif

#if TOOLS_ENABLED
	if (SceneTree::get_singleton()->is_debugging_collisions_hint() && chunk->collider_get_count() > 0) {
		chunk->draw_debug_mdr_colliders();
	}
#endif

	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	next_phase();
}

void TerrainPropJob::phase_prop() {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	Ref<TerrainChunkDefault> chunk = _chunk;

	if (!get_prop_mesher().is_valid()) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		return;
	}

	if (should_do()) {
		if (chunk->mesh_data_resource_get_count() == 0) {
			// clear mesh from previous runs
			int mesh_count = chunk->mesh_rid_get_count(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH);
			if (mesh_count > 0) {
				for (int i = 0; i < mesh_count; ++i) {
					RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, i);

					if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
						RS::get_singleton()->mesh_clear(mesh_rid);
					}
				}
			}

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
		if ((chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			get_prop_mesher()->bake_colors(_chunk);
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			TerrainWorldDefault *world = Object::cast_to<TerrainWorldDefault>(chunk->get_terrain_world());

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
	}

	if (get_prop_mesher()->get_vertex_count() == 0) {
		// clear mesh from previous runs
		int mesh_count = chunk->mesh_rid_get_count(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH);
		if (mesh_count > 0) {
			for (int i = 0; i < mesh_count; ++i) {
				RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
					RS::get_singleton()->mesh_clear(mesh_rid);
				}
			}
		}

		reset_stages();

		set_complete(true); //So threadpool knows it's done
		next_job();
		return;
	}

#endif

	reset_stages();
	next_phase();
}

void TerrainPropJob::_physics_process(float delta) {
	if (_phase == 0) {
		phase_physics_process();
	}
}

void TerrainPropJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<TerrainLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	Ref<TerrainChunkDefault> chunk = _chunk;

	if (!chunk.is_valid()
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
			|| chunk->mesh_data_resource_get_count() == 0
#endif
	) {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		// clear mesh from previous runs
		int mesh_count = chunk->mesh_rid_get_count(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH);
		if (mesh_count > 0) {
			for (int i = 0; i < mesh_count; ++i) {
				RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
					RS::get_singleton()->mesh_clear(mesh_rid);
				}
			}
		}
#endif

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
		ERR_FAIL_MSG("TerrainPropJob: _phase is too high!");
	}
}

void TerrainPropJob::_reset() {
	TerrainJob::_reset();

	_build_done = false;
	_phase = 0;

	_current_job_step = 0;
	_current_mesh = 0;

	if (get_prop_mesher().is_valid()) {
		get_prop_mesher()->reset();
		get_prop_mesher()->set_library(_chunk->get_library());
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
}

void TerrainPropJob::phase_setup() {
	Ref<TerrainLibrary> library = _chunk->get_library();

	if (!library->supports_caching()) {
		next_phase();
		return;
	}

	if (library->supports_caching()) {
		if (!_chunk->prop_material_cache_key_has() || _chunk->prop_material_cache_key_invalid_get()) {
			library->prop_material_cache_get_key(_chunk);

			if (!_chunk->prop_material_cache_key_has()) {
				//chunk does not need a key
				next_phase();
				return;
			}
		}

		Ref<TerrainMaterialCache> cache = library->prop_material_cache_get(_chunk->prop_material_cache_key_get());

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		for (int i = 0; i < _chunk->mesh_data_resource_get_count(); ++i) {
			Ref<Texture> tex = _chunk->mesh_data_resource_get_texture(i);

			if (!tex.is_valid()) {
				continue;
			}

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

void TerrainPropJob::phase_steps() {
	Ref<TerrainChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!_prop_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if (_prop_mesher->get_vertex_count() == 0) {
		// clear mesh from previous runs
		int mesh_count = chunk->mesh_rid_get_count(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH);
		if (mesh_count > 0) {
			for (int i = 0; i < mesh_count; ++i) {
				RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
					RS::get_singleton()->mesh_clear(mesh_rid);
				}
			}
		}

		reset_stages();
		//next_phase();
		set_complete(true); //So threadpool knows it's done
		next_job();

		return;
	}

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (mesh_rid == RID()) {
			//need to allocate the meshes

			//first count how many we need
			int count = 0;
			for (int i = 0; i < _job_steps.size(); ++i) {
				Ref<TerrainMesherJobStep> step = _job_steps[i];

				ERR_FAIL_COND(!step.is_valid());

				switch (step->get_job_type()) {
					case TerrainMesherJobStep::TYPE_NORMAL:
						++count;
						break;
					case TerrainMesherJobStep::TYPE_NORMAL_LOD:
						++count;
						break;
					case TerrainMesherJobStep::TYPE_DROP_UV2:
						++count;
						break;
					case TerrainMesherJobStep::TYPE_MERGE_VERTS:
						++count;
						break;
					case TerrainMesherJobStep::TYPE_BAKE_TEXTURE:
						++count;
						break;
					case TerrainMesherJobStep::TYPE_SIMPLIFY_MESH:
#ifdef MODULE_MESH_UTILS_ENABLED
						count += step->get_simplification_steps();
#endif
						break;
					default:
						break;
				}
			}

			//allocate
			if (count > 0) {
				chunk->meshes_create(TerrainChunkDefault::MESH_INDEX_PROP, count);
			}
		}
	}

	for (; _current_job_step < _job_steps.size();) {
		Ref<TerrainMesherJobStep> step = _job_steps[_current_job_step];

		ERR_FAIL_COND(!step.is_valid());

		switch (step->get_job_type()) {
			case TerrainMesherJobStep::TYPE_NORMAL:
				step_type_normal();
				break;
			case TerrainMesherJobStep::TYPE_NORMAL_LOD:
				step_type_normal_lod();
				break;
			case TerrainMesherJobStep::TYPE_DROP_UV2:
				step_type_drop_uv2();
				break;
			case TerrainMesherJobStep::TYPE_MERGE_VERTS:
				step_type_merge_verts();
				break;
			case TerrainMesherJobStep::TYPE_BAKE_TEXTURE:
				step_type_bake_texture();
				break;
			case TerrainMesherJobStep::TYPE_SIMPLIFY_MESH:
				step_type_simplify_mesh();
				break;
			case TerrainMesherJobStep::TYPE_OTHER:
				//do nothing
				break;
		}

		++_current_job_step;

		if (should_return()) {
			return;
		}
	}

	reset_stages();
	//next_phase();

	set_complete(true); //So threadpool knows it's done
	next_job();
}

void TerrainPropJob::step_type_normal() {
	//TODO add a lighting generation step

	Ref<TerrainChunkDefault> chunk = _chunk;

	temp_mesh_arr = _prop_mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
		RS::get_singleton()->mesh_clear(mesh_rid);
	}

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerrainPropJob::step_type_normal_lod() {
	print_error("Error: step_type_normal_lod doesn't work for TerrainPropJobs!");

	++_current_mesh;
}

void TerrainPropJob::step_type_drop_uv2() {
	Ref<TerrainChunkDefault> chunk = _chunk;

	RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	temp_mesh_arr[RenderingServer::ARRAY_TEX_UV2] = Variant();

	if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
		RS::get_singleton()->mesh_clear(mesh_rid);
	}

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerrainPropJob::step_type_merge_verts() {
	Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
	temp_mesh_arr = temp_mesh_arr2;

	Ref<TerrainChunkDefault> chunk = _chunk;
	RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
		RS::get_singleton()->mesh_clear(mesh_rid);
	}

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerrainPropJob::step_type_bake_texture() {
	Ref<TerrainChunkDefault> chunk = _chunk;

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	Ref<ShaderMaterial> mat = lmat;
	Ref<SpatialMaterial> spmat = lmat;
	Ref<Texture> tex;

	if (mat.is_valid()) {
		tex = mat->get_shader_param("texture_albedo");
	} else if (spmat.is_valid()) {
		tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
	}

	if (tex.is_valid()) {
		temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
		temp_mesh_arr[RenderingServer::ARRAY_TEX_UV] = Variant();

		RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
			RS::get_singleton()->mesh_clear(mesh_rid);
		}

		RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		if (lmat.is_valid()) {
			RenderingServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}
	}

	++_current_mesh;
}

void TerrainPropJob::step_type_simplify_mesh() {
#ifdef MODULE_MESH_UTILS_ENABLED

	Ref<TerrainChunkDefault> chunk = _chunk;
	Ref<TerrainMesherJobStep> step = _job_steps[_current_job_step];
	ERR_FAIL_COND(!step.is_valid());
	Ref<FastQuadraticMeshSimplifier> fqms = step->get_fqms();
	ERR_FAIL_COND(!fqms.is_valid());

	fqms->initialize(temp_mesh_arr);

	for (int i = 0; i < step->get_simplification_steps(); ++i) {
		fqms->simplify_mesh(temp_mesh_arr.size() * step->get_simplification_step_ratio(), step->get_simplification_agressiveness());
		temp_mesh_arr = fqms->get_arrays();

		RID mesh_rid = chunk->mesh_rid_get_index(TerrainChunkDefault::MESH_INDEX_PROP, TerrainChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0) {
			RS::get_singleton()->mesh_clear(mesh_rid);
		}

		RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->prop_material_cache_key_has()) {
			lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			RenderingServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}

		++_current_mesh;
	}

#endif
}

TerrainPropJob::TerrainPropJob() {
	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);

	_current_job_step = 0;
	_current_mesh = 0;
}

TerrainPropJob::~TerrainPropJob() {
}

void TerrainPropJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_mesher"), &TerrainPropJob::get_prop_mesher);
	ClassDB::bind_method(D_METHOD("set_prop_mesher", "mesher"), &TerrainPropJob::set_prop_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_mesher", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMesher", 0), "set_prop_mesher", "get_prop_mesher");

	ClassDB::bind_method(D_METHOD("get_jobs_step", "index"), &TerrainPropJob::get_jobs_step);
	ClassDB::bind_method(D_METHOD("set_jobs_step", "index", "mesher"), &TerrainPropJob::set_jobs_step);
	ClassDB::bind_method(D_METHOD("remove_jobs_step", "index"), &TerrainPropJob::remove_jobs_step);
	ClassDB::bind_method(D_METHOD("add_jobs_step", "mesher"), &TerrainPropJob::add_jobs_step);
	ClassDB::bind_method(D_METHOD("get_jobs_step_count"), &TerrainPropJob::get_jobs_step_count);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &TerrainPropJob::_physics_process);
}
