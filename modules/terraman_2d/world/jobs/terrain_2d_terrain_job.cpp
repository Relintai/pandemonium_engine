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

#include "terrain_2d_terrain_job.h"

#include "../../library/terrain_2d_library.h"
#include "../../library/terrain_2d_material_cache.h"
#include "../../library/terrain_2d_surface.h"

#include "../../meshers/default/terrain_2d_mesher_default.h"
#include "../../meshers/terrain_2d_mesher.h"

#include "../default/terrain_2d_chunk_default.h"
#include "servers/physics_2d_server.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_UTILS_ENABLED
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<Terrain2DMesher> Terrain2DTerrain2DJob::get_mesher() const {
	return _mesher;
}
void Terrain2DTerrain2DJob::set_mesher(const Ref<Terrain2DMesher> &mesher) {
	_mesher = mesher;
}

Ref<Terrain2DMesher> Terrain2DTerrain2DJob::get_liquid_mesher() const {
	return _liquid_mesher;
}
void Terrain2DTerrain2DJob::set_liquid_mesher(const Ref<Terrain2DMesher> &mesher) {
	_liquid_mesher = mesher;
}

void Terrain2DTerrain2DJob::phase_setup() {
	if (_mesher.is_valid()) {
		_mesher->set_library(_chunk->get_library());
		_mesher->reset();
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->set_library(_chunk->get_library());
		_liquid_mesher->reset();
	}

	next_phase();
}

void Terrain2DTerrain2DJob::phase_library_setup() {
	if (should_return()) {
		return;
	}

	Ref<Terrain2DLibrary> lib = _chunk->get_library();

	if (!lib.is_valid()) {
		next_phase();
		return;
	}

	if (lib->supports_caching()) {
		if (!_chunk->material_cache_key_has()) {
			lib->material_cache_get_key(_chunk);
		}

		Ref<Terrain2DMaterialCache> cache = lib->material_cache_get(_chunk->material_cache_key_get());

		if (!cache.is_valid()) {
			//Try a fallback texture
			Ref<Terrain2DChunkDefault> cd = _chunk;

			if (cd.is_valid()) {
				Ref<Texture> tex = lib->texture_get();

				if (tex.is_valid()) {
					cd->mesh_rid_set(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_TEXTURE_RID, tex->get_rid());
				}
			}

			next_phase();
			return;
		}

		//Note: without threadpool and threading none of this can happen, as cache will get initialized the first time a thread requests it!
		while (!cache->get_initialized()) {
			//Means it's currently merging the atlases on a different thread.
			//Let's just wait
			OS::get_singleton()->delay_usec(100);
		}

		Ref<Terrain2DChunkDefault> cd = _chunk;

		if (cd.is_valid()) {
			Ref<Texture> tex = cache->texture_get_merged();

			if (tex.is_valid()) {
				cd->mesh_rid_set(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_TEXTURE_RID, tex->get_rid());
			}
		}

	} else {
		Ref<Terrain2DChunkDefault> cd = _chunk;

		if (cd.is_valid()) {
			Ref<Texture> tex = lib->texture_get();

			if (tex.is_valid()) {
				cd->mesh_rid_set(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_TEXTURE_RID, tex->get_rid());
			}
		}
	}

	next_phase();

	if (should_return()) {
		return;
	}
}

void Terrain2DTerrain2DJob::phase_terrain_mesh_setup() {
	if (should_return()) {
		return;
	}

	if (_mesher.is_valid()) {
		if (should_do()) {
			_mesher->add_chunk(_chunk);

			if (should_return()) {
				return;
			}
		}
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->add_chunk(_chunk);
	}

	reset_stages();
	next_phase();
}

void Terrain2DTerrain2DJob::phase_collider() {
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_CREATE_COLLIDER) == 0) {
		next_phase();
		return;
	}

	ERR_FAIL_COND(!_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if (should_do()) {
		temp_arr_collider.append_array(_mesher->build_collider());

		if (should_return()) {
			return;
		}
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		if (_liquid_mesher.is_valid()) {
			temp_arr_collider_liquid.append_array(_liquid_mesher->build_collider());
		}
	}

	if (temp_arr_collider.size() == 0 && temp_arr_collider_liquid.size() == 0) {
		reset_stages();
		next_phase();
		next_phase();
		return;
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
	reset_stages();
	next_phase();
}

void Terrain2DTerrain2DJob::phase_physics_process() {
	ERR_FAIL_COND(!_chunk.is_valid());
	
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	if (temp_arr_collider.size() != 0) {
		if (!chunk->meshes_has(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_BODY)) {
			chunk->colliders_create(Terrain2DChunkDefault::MESH_INDEX_TERRAIN);
		}

		Physics2DServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider);

		temp_arr_collider.resize(0);
	}

	if (temp_arr_collider_liquid.size() != 0) {
		if (Engine::get_singleton()->is_editor_hint()) {
			if (!chunk->meshes_has(Terrain2DChunkDefault::MESH_INDEX_LIQUID, Terrain2DChunkDefault::MESH_TYPE_INDEX_BODY)) {
				chunk->colliders_create(Terrain2DChunkDefault::MESH_INDEX_LIQUID);
			}
		}

		Physics2DServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_LIQUID, Terrain2DChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider_liquid);

		temp_arr_collider_liquid.resize(0);
	}

	_mesher->create_tile_colliders(_chunk);

	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	reset_stages();
	next_phase();
}

void Terrain2DTerrain2DJob::phase_terrain_mesh() {
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		if (should_do()) {
			if (_liquid_mesher.is_valid()) {
				_liquid_mesher->bake_colors(_chunk);

				if (should_return()) {
					return;
				}
			}
		}
	}

	if ((_mesher->get_vertex_count() == 0 && _mesher->get_stored_mesh_count() == 0) && (!_liquid_mesher.is_valid() || _liquid_mesher->get_vertex_count() == 0)) {
		reset_stages();
		next_phase();

		return;
	}

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get_index(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (mesh_rid == RID()) {
			//need to allocate the meshes
			//allocate
			chunk->meshes_create(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, 1 + _mesher->get_stored_mesh_count());

			chunk->_mesh_transforms.resize(1 + _mesher->get_stored_mesh_count());

		} else {
			//we have the meshes, just clear
			int count = chunk->mesh_rid_get_count(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);
			chunk->_mesh_transforms.resize(count);

			for (int i = 0; i < count; ++i) {
				mesh_rid = chunk->mesh_rid_get_index(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
					RS::get_singleton()->mesh_remove_surface(mesh_rid, 0);

				chunk->_mesh_transforms.write[i] = Transform2D();
			}
		}
	}

	step_type_normal();

	if (_liquid_mesher.is_valid() && _liquid_mesher->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = _liquid_mesher->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_LIQUID, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);

		if (should_do()) {
			if (mesh_rid == RID()) {
				chunk->mesh_create(Terrain2DChunkDefault::MESH_INDEX_LIQUID);

				mesh_rid = chunk->mesh_rid_get(Terrain2DChunkDefault::MESH_INDEX_LIQUID, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH);
			}

			if (RS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
				RS::get_singleton()->mesh_remove_surface(mesh_rid, 0);

			if (should_return()) {
				return;
			}
		}

		RS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);
	}

	reset_stages();
	next_phase();
}

void Terrain2DTerrain2DJob::phase_finalize() {
	set_complete(true); //So threadpool knows it's done

	next_job();
}

void Terrain2DTerrain2DJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<Terrain2DLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	if (_phase == 0) {
		phase_setup();
	} else if (_phase == 1) {
		phase_library_setup();
	} else if (_phase == 2) {
		phase_terrain_mesh_setup();
	} else if (_phase == 3) {
		phase_collider();
	} else if (_phase == 5) {
		phase_terrain_mesh();
	} else if (_phase == 6) {
		phase_finalize();
	} else if (_phase > 6) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("Terrain2DTerrain2DJob: _phase is too high!");
	}
}

void Terrain2DTerrain2DJob::_reset() {
	Terrain2DJob::_reset();

	_build_done = false;
	_phase = 0;

	ERR_FAIL_COND(!_mesher.is_valid());

	_mesher->set_cell_size_x(_chunk->get_cell_size_x());
	_mesher->set_cell_size_y(_chunk->get_cell_size_y());

	Ref<Terrain2DChunkDefault> chunk = _chunk;
	Ref<Terrain2DMesherDefault> md = _mesher;

	if (chunk.is_valid() && md.is_valid()) {
		md->set_build_flags(chunk->get_build_flags());
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->set_cell_size_x(_chunk->get_cell_size_x());
		_liquid_mesher->set_cell_size_y(_chunk->get_cell_size_y());

		md = _liquid_mesher;

		if (chunk.is_valid() && md.is_valid()) {
			md->set_build_flags(chunk->get_build_flags());
		}
	}
}

void Terrain2DTerrain2DJob::_physics_process(float delta) {
	if (_phase == 4)
		phase_physics_process();
}

void Terrain2DTerrain2DJob::step_type_normal() {
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	temp_mesh_arr = _mesher->build_mesh();
	RID mesh_rid = chunk->mesh_rid_get_index(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH, 0);
	RS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	for (int i = 0; i < _mesher->get_stored_mesh_count(); ++i) {
		Array arr = _mesher->build_stored_mesh(i);
		mesh_rid = chunk->mesh_rid_get_index(Terrain2DChunkDefault::MESH_INDEX_TERRAIN, Terrain2DChunkDefault::MESH_TYPE_INDEX_MESH, i + 1);
		RS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, arr);

		chunk->_mesh_transforms.write[i + 1] = _mesher->get_stored_mesh_transform(i);
		//AABB aabb = _mesher->calculate_stored_mesh_aabb(i);
		//RS::get_singleton()->mesh_set_custom_aabb(mesh_rid, aabb);
	}
}

Terrain2DTerrain2DJob::Terrain2DTerrain2DJob() {
}

Terrain2DTerrain2DJob::~Terrain2DTerrain2DJob() {
	_mesher.unref();
	_liquid_mesher.unref();
}

void Terrain2DTerrain2DJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesher"), &Terrain2DTerrain2DJob::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "mesher"), &Terrain2DTerrain2DJob::set_mesher);

	ClassDB::bind_method(D_METHOD("get_liquid_mesher"), &Terrain2DTerrain2DJob::get_liquid_mesher);
	ClassDB::bind_method(D_METHOD("set_liquid_mesher", "mesher"), &Terrain2DTerrain2DJob::set_liquid_mesher);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &Terrain2DTerrain2DJob::_physics_process);
}
