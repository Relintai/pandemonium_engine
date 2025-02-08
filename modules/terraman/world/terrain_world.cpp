/*************************************************************************/
/*  terrain_world.cpp                                                    */
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

#include "terrain_world.h"

#include "core/containers/hash_set.h"

#include "core/object/message_queue.h"
#include "core/os/main_loop.h"
#include "terrain_chunk.h"
#include "terrain_structure.h"

#include "../defines.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_entry.h"
#include "../../props/props/prop_data_light.h"
#include "../../props/props/prop_data_prop.h"
#include "../../props/props/prop_data_scene.h"
#endif

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

#if TOOLS_ENABLED
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"
#endif

const String TerrainWorld::BINDING_STRING_CHANNEL_TYPE_INFO = "Type,Isolevel,Liquid,Liquid Level";

bool TerrainWorld::get_active() const {
	return _active;
}
void TerrainWorld::set_active(const bool value) {
	_active = value;

	notification(NOTIFICATION_ACTIVE_STATE_CHANGED);
}

bool TerrainWorld::get_editable() const {
	return _editable;
}
void TerrainWorld::set_editable(const bool value) {
	_editable = value;
}

int TerrainWorld::get_chunk_size_x() const {
	return _chunk_size_x;
}
void TerrainWorld::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int TerrainWorld::get_chunk_size_z() const {
	return _chunk_size_z;
}
void TerrainWorld::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
}

int TerrainWorld::get_data_margin_start() const {
	return _data_margin_start;
}
void TerrainWorld::set_data_margin_start(const int value) {
	_data_margin_start = value;
}

int TerrainWorld::get_data_margin_end() const {
	return _data_margin_end;
}
void TerrainWorld::set_data_margin_end(const int value) {
	_data_margin_end = value;
}

int TerrainWorld::get_current_seed() const {
	return _current_seed;
}
void TerrainWorld::set_current_seed(const int value) {
	_current_seed = value;
}

_FORCE_INLINE_ float TerrainWorld::get_world_height() const {
	return _world_height;
}
void TerrainWorld::set_world_height(const float value) {
	_world_height = value;
}

int TerrainWorld::get_max_concurrent_generations() {
	return _max_concurrent_generations;
}
void TerrainWorld::set_max_concurrent_generations(const int value) {
	_max_concurrent_generations = value;
}

Ref<TerrainLibrary> TerrainWorld::get_library() {
	return _library;
}
void TerrainWorld::set_library(const Ref<TerrainLibrary> &library) {
	_library = library;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerrainChunk> c = chunk_get_index(i);

		if (!c.is_valid()) {
			continue;
		}

		c->set_library(_library);
	}
}

Ref<TerrainLevelGenerator> TerrainWorld::get_level_generator() const {
	return _level_generator;
}
void TerrainWorld::set_level_generator(const Ref<TerrainLevelGenerator> &level_generator) {
	_level_generator = level_generator;
}

float TerrainWorld::get_voxel_scale() const {
	return _voxel_scale;
}
void TerrainWorld::set_voxel_scale(const float value) {
	_voxel_scale = value;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerrainChunk> c = chunk_get_index(i);

		if (!c.is_valid()) {
			continue;
		}

		c->set_voxel_scale(_voxel_scale);
	}
}

int TerrainWorld::get_chunk_spawn_range() const {
	return _chunk_spawn_range;
}
void TerrainWorld::set_chunk_spawn_range(const int value) {
	_chunk_spawn_range = value;
}

NodePath TerrainWorld::get_player_path() const {
	return _player_path;
}

void TerrainWorld::set_player_path(const NodePath &player_path) {
	_player_path = player_path;
}

Spatial *TerrainWorld::get_player() const {
	return _player;
}
void TerrainWorld::set_player(Spatial *player) {
	_player = player;
}
void TerrainWorld::set_player_bind(Node *player) {
	set_player(Object::cast_to<Spatial>(player));
}

Ref<TerrainWorldArea> TerrainWorld::world_area_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _world_areas.size(), Ref<TerrainWorldArea>());

	return _world_areas.get(index);
}
void TerrainWorld::world_area_add(const Ref<TerrainWorldArea> &area) {
	_world_areas.push_back(area);
}
void TerrainWorld::world_area_remove(const int index) {
	ERR_FAIL_INDEX(index, _world_areas.size());

	_world_areas.remove(index);
}
void TerrainWorld::world_areas_clear() {
	_world_areas.clear();
}
int TerrainWorld::world_area_get_count() const {
	return _world_areas.size();
}

//Terra Structures

Ref<TerrainStructure> TerrainWorld::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<TerrainStructure>());

	return _voxel_structures.get(index);
}
void TerrainWorld::voxel_structure_add(const Ref<TerrainStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void TerrainWorld::voxel_structure_remove(const Ref<TerrainStructure> &structure) {
	if (!structure.is_valid()) {
		return;
	}

	int index = _voxel_structures.find(structure);

	if (index != -1) {
		_voxel_structures.remove(index);
	}
}
void TerrainWorld::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void TerrainWorld::voxel_structures_clear() {
	_voxel_structures.clear();
}
int TerrainWorld::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void TerrainWorld::voxel_structure_add_at_position(Ref<TerrainStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> TerrainWorld::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void TerrainWorld::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structures_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<TerrainLight> structure = Ref<TerrainLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void TerrainWorld::chunk_add(Ref<TerrainChunk> chunk, const int x, const int z) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND_MSG(chunk->get_voxel_world() != NULL && chunk->get_voxel_world() != this, "Chunk is already owned by an another world!");

	IntPos pos(x, z);

	//ERR_FAIL_COND(_chunks.has(pos));

	chunk->set_voxel_world(this);
	chunk->set_position(x, z);
	chunk->world_transform_changed();

	if (!_chunks.has(pos)) {
		_chunks.set(pos, chunk);
	}

	if (_chunks_vector.find(chunk) == -1) {
		_chunks_vector.push_back(chunk);
	}

	if (is_inside_tree()) {
		chunk->enter_tree();
	}

	if (has_method("_chunk_added")) {
		call("_chunk_added", chunk);
	}

	emit_signal("chunk_added", chunk);

	chunk->build();
}
bool TerrainWorld::chunk_has(const int x, const int z) const {
	return _chunks.has(IntPos(x, z));
}
Ref<TerrainChunk> TerrainWorld::chunk_get(const int x, const int z) {
	IntPos pos(x, z);

	if (_chunks.has(pos)) {
		return _chunks.get(pos);
	}

	return Ref<TerrainChunk>();
}
Ref<TerrainChunk> TerrainWorld::chunk_remove(const int x, const int z) {
	IntPos pos(x, z);

	if (!_chunks.has(pos)) {
		return NULL;
	}

	Ref<TerrainChunk> chunk = _chunks.get(pos);

	chunk->exit_tree();

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		if (_chunks_vector.get(i) == chunk) {
			_chunks_vector.remove(i);
			break;
		}
	}

	_generation_queue.erase(chunk);

	if (chunk->get_is_generating()) {
		chunk->cancel_build();
	}

	//never remove from this here
	//_generating.erase(chunk);

	_chunks.erase(pos);

	emit_signal("chunk_removed", chunk);

	return chunk;
}
Ref<TerrainChunk> TerrainWorld::chunk_remove_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	Ref<TerrainChunk> chunk = _chunks_vector.get(index);
	chunk->exit_tree();

	_chunks_vector.remove(index);
	_chunks.erase(IntPos(chunk->get_position_x(), chunk->get_position_z()));
	_generation_queue.erase(chunk);

	if (chunk->get_is_generating()) {
		chunk->cancel_build();
	}

	//never remove from this here
	//_generating.erase(chunk);

	emit_signal("chunk_removed", chunk);

	return chunk;
}

Ref<TerrainChunk> TerrainWorld::chunk_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int TerrainWorld::chunk_get_count() const {
	return _chunks_vector.size();
}

void TerrainWorld::chunks_clear() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerrainChunk> chunk = _chunks_vector.get(i);

		chunk->exit_tree();

		emit_signal("chunk_removed", chunk);
	}

	_chunks_vector.clear();
	_chunks.clear();
	_generation_queue.clear();

	for (int i = 0; i < _generating.size(); ++i) {
		Ref<TerrainChunk> chunk = _generating[i];

		if (chunk->get_is_generating()) {
			chunk->cancel_build();
		}
	}

	//never remove from this here
	//_generating.clear();
}

Ref<TerrainChunk> TerrainWorld::chunk_get_or_create(int x, int z) {
	Ref<TerrainChunk> chunk = chunk_get(x, z);

	if (!chunk.is_valid()) {
		chunk = chunk_create(x, z);
	}

	return chunk;
}

Ref<TerrainChunk> TerrainWorld::chunk_create(const int x, const int z) {
	Ref<TerrainChunk> c;
	c = call("_create_chunk", x, z, Ref<TerrainChunk>());

	generation_queue_add_to(c);

	return c;
}

void TerrainWorld::chunk_setup(Ref<TerrainChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	call("_create_chunk", chunk->get_position_x(), chunk->get_position_z(), chunk);
}

Ref<TerrainChunk> TerrainWorld::_create_chunk(const int x, const int z, Ref<TerrainChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk.instance();
	}

	//no meshers here

	ERR_FAIL_COND_V(!chunk.is_valid(), NULL);

	chunk->set_name("Chunk[" + String::num(x) + "," + String::num(z) + "]");

	chunk->set_voxel_world(this);

	chunk->set_position(x, z);
	chunk->set_world_height(_world_height);
	chunk->set_library(_library);
	chunk->set_voxel_scale(_voxel_scale);
	chunk->set_size(_chunk_size_x, _chunk_size_z, _data_margin_start, _data_margin_end);
	//chunk->set_translation(Vector3(x * _chunk_size_x * _voxel_scale, y * _chunk_size_y * _voxel_scale, z * _chunk_size_z * _voxel_scale));

	if (!get_active()) {
		chunk->set_visible(false);
	}

	chunk_add(chunk, x, z);

	return chunk;
}

void TerrainWorld::chunk_generate(Ref<TerrainChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_prepare_chunk_for_generation")) {
		call("_prepare_chunk_for_generation", chunk);
	}

	call("_generate_chunk", chunk);

	chunk->build();
}

Vector<Variant> TerrainWorld::chunks_get() {
	VARIANT_ARRAY_GET(_chunks_vector);
}

void TerrainWorld::chunks_set(const Vector<Variant> &chunks) {
	if (is_inside_tree()) {
		for (int i = 0; i < _chunks_vector.size(); ++i) {
			Ref<TerrainChunk> chunk = Ref<TerrainChunk>(_chunks_vector[i]);

			if (chunks.find(chunk) == -1) {
				chunk_remove_index(i);
				_generation_queue.erase(chunk);

				chunk->exit_tree();

				if (chunk->get_is_generating()) {
					chunk->cancel_build();
				}

				//never remove from this here
				//_generating.erase(chunk);

				--i;
			}
		}

		//add the difference
		for (int i = 0; i < chunks.size(); ++i) {
			Ref<TerrainChunk> chunk = Ref<TerrainChunk>(chunks[i]);

			if (!chunk.is_valid()) {
				continue;
			}

			if (_chunks_vector.find(chunk) != -1) {
				continue;
			}

			chunk_add(chunk, chunk->get_position_x(), chunk->get_position_z());
		}
	} else {
		_chunks_vector.clear();

		for (int i = 0; i < chunks.size(); ++i) {
			Ref<TerrainChunk> chunk = Ref<TerrainChunk>(chunks[i]);

			_chunks_vector.push_back(chunk);
		}
	}
}

int TerrainWorld::_get_channel_index_info(const TerrainWorld::ChannelTypeInfo channel_type) {
	return -1;
}

bool TerrainWorld::can_chunk_do_build_step() {
	if (_max_frame_chunk_build_steps == 0) {
		return true;
	}

	return _num_frame_chunk_build_steps++ < _max_frame_chunk_build_steps;
}

bool TerrainWorld::is_position_walkable(const Vector3 &p_pos) {
	int x = static_cast<int>(Math::floor(p_pos.x / (_chunk_size_x * _voxel_scale)));
	int z = static_cast<int>(Math::floor(p_pos.z / (_chunk_size_z * _voxel_scale)));

	Ref<TerrainChunk> c = chunk_get(x, z);

	if (!c.is_valid()) {
		return false;
	}

	return !c->get_is_generating();
}

void TerrainWorld::on_chunk_mesh_generation_finished(Ref<TerrainChunk> p_chunk) {
	call_deferred("emit_signal", "chunk_mesh_generation_finished", p_chunk);
}

void TerrainWorld::generation_queue_add_to(const Ref<TerrainChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	set_process_internal(true);

	_generation_queue.push_back(chunk);
}
Ref<TerrainChunk> TerrainWorld::generation_queue_get_index(int index) {
	ERR_FAIL_INDEX_V(index, _generation_queue.size(), NULL);

	return _generation_queue.get(index);
}
void TerrainWorld::generation_queue_remove_index(int index) {
	ERR_FAIL_INDEX(index, _generation_queue.size());

	_generation_queue.remove(index);
}
int TerrainWorld::generation_queue_get_size() const {
	return _generation_queue.size();
}

void TerrainWorld::generation_add_to(const Ref<TerrainChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	_generating.push_back(chunk);
}
Ref<TerrainChunk> TerrainWorld::generation_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _generating.size(), NULL);

	return _generating.get(index);
}
void TerrainWorld::generation_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _generating.size());

	_generating.remove(index);
}
int TerrainWorld::generation_get_size() const {
	return _generating.size();
}

#ifdef MODULE_PROPS_ENABLED
void TerrainWorld::prop_add(Transform transform, const Ref<PropData> &prop, const bool apply_voxel_scale) {
	ERR_FAIL_COND(!prop.is_valid());

	if (apply_voxel_scale) {
		transform = transform.scaled(Vector3(get_voxel_scale(), get_voxel_scale(), get_voxel_scale()));
	}

	Vector3 wp;
	wp = transform.xform(wp);
	Ref<TerrainChunk> chunk = get_or_create_chunk_at_world_position(wp);

	chunk->prop_add(transform, prop);

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<PropDataEntry> entry = prop->get_prop(i);

		if (!entry.is_valid()) {
			continue;
		}

		Transform t = transform * entry->get_transform();

		wp = t.xform(Vector3());
		chunk = get_or_create_chunk_at_world_position(wp);

		Ref<PropDataProp> prop_entry_data = entry;

		if (prop_entry_data.is_valid()) {
			Ref<PropData> p = prop_entry_data->get_prop();

			if (!p.is_valid()) {
				continue;
			}

			prop_add(t, p, false);

			continue;
		}

		Ref<PropDataScene> scene_data = entry;

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

		Ref<PropDataLight> light_data = entry;

		if (light_data.is_valid()) {
			Ref<TerrainLight> light;
			light.instance();

			light->set_world_position(wp.x / get_voxel_scale(), wp.y / get_voxel_scale(), wp.z / get_voxel_scale());
			light->set_range(light_data->get_light_range());
			light->set_attenuation(light_data->get_light_attenuation());
			light->set_color(light_data->get_light_color());
			light->set_energy(light_data->get_light_energy());
			light->set_indirect_energy(light_data->get_light_indirect_energy());
			light->set_negative(light_data->get_light_negative());
			light->set_specular(light_data->get_light_specular());

			light_add(light);

			continue;
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<PropDataMeshData> mesh_data = entry;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid()) {
				continue;
			}

			Transform chunk_local_tform = t;

			chunk_local_tform.origin = chunk->to_local(chunk_local_tform.origin);

			chunk->mesh_data_resource_add(chunk_local_tform, mdr, mesh_data->get_texture(), Color(1, 1, 1, 1), false);

			continue;
		}
#endif
	}
}
#endif

//Lights
void TerrainWorld::light_add(const Ref<TerrainLight> &light) {
	_lights.push_back(light);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerrainChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_added(light);
		}
	}
}
Ref<TerrainLight> TerrainWorld::light_get(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<TerrainLight>());

	return _lights.get(index);
}
void TerrainWorld::light_remove(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<TerrainLight> light = _lights[index];

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerrainChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_removed(light);
		}
	}
}
int TerrainWorld::light_get_count() const {
	return _lights.size();
}
void TerrainWorld::lights_clear() {
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<TerrainLight> light = _lights[i];

		if (!light.is_valid()) {
			continue;
		}

		for (int j = 0; j < _chunks_vector.size(); ++j) {
			Ref<TerrainChunk> chunk = _chunks_vector[j];

			if (chunk.is_valid()) {
				chunk->world_light_removed(light);
			}
		}
	}

	_lights.clear();
}

Vector<Variant> TerrainWorld::lights_get() {
	VARIANT_ARRAY_GET(_lights);
}
void TerrainWorld::lights_set(const Vector<Variant> &chunks) {
	lights_clear();

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<TerrainLight> light = Ref<TerrainLight>(chunks[i]);

		light_add(light);
	}
}

uint8_t TerrainWorld::get_voxel_at_world_position(const Vector3 &world_position, const int channel_index) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	Ref<TerrainChunk> chunk = chunk_get(x, z);

	if (chunk.is_valid()) {
		return chunk->get_voxel(bx, bz, channel_index);
	}

	return 0;
}

void TerrainWorld::set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	if (get_data_margin_end() > 0) {
		if (bx == 0) {
			Ref<TerrainChunk> chunk = chunk_get_or_create(x - 1, z);
			chunk->set_voxel(data, get_chunk_size_x(), bz, channel_index);

			if (rebuild) {
				chunk->build();
			}
		}

		if (bz == 0) {
			Ref<TerrainChunk> chunk = chunk_get_or_create(x, z - 1);
			chunk->set_voxel(data, bx, get_chunk_size_z(), channel_index);

			if (rebuild) {
				chunk->build();
			}
		}

		if (bx == 0 && bz == 0) {
			Ref<TerrainChunk> chunk = chunk_get_or_create(x - 1, z - 1);
			chunk->set_voxel(data, get_chunk_size_x(), get_chunk_size_z(), channel_index);

			if (rebuild) {
				chunk->build();
			}
		}
	}

	if (get_data_margin_start() > 0) {
		if (bx == get_chunk_size_x() - 1) {
			Ref<TerrainChunk> chunk = chunk_get_or_create(x + 1, z);
			chunk->set_voxel(data, -1, bz, channel_index);

			if (rebuild) {
				chunk->build();
			}
		}

		if (bz == get_chunk_size_z() - 1) {
			Ref<TerrainChunk> chunk = chunk_get_or_create(x, z + 1);
			chunk->set_voxel(data, bx, -1, channel_index);

			if (rebuild) {
				chunk->build();
			}
		}
	}

	Ref<TerrainChunk> chunk = chunk_get_or_create(x, z);
	chunk->set_voxel(data, bx, bz, channel_index);

	if (rebuild) {
		chunk->build();
	}
}

Ref<TerrainChunk> TerrainWorld::get_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get(x, z);
}
Ref<TerrainChunk> TerrainWorld::get_or_create_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get_or_create(x, z);
}

Vector2i TerrainWorld::world_position_to_chunk_position(const Vector3 &p_world_position) {
	// TODO rework this so it works directly with ints.

	Vector3 pos = p_world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return Vector2i(x, z);
}

Vector2i TerrainWorld::world_data_position_to_chunk_position(const Vector2i &p_world_data_position) {
	// TODO rework this so it works directly with ints.

	Vector2 pos = p_world_data_position;

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

	return Vector2i(x, z);
}

Vector2i TerrainWorld::world_position_to_world_data_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	return Vector2i(pos.x, pos.z);
}
uint8_t TerrainWorld::get_voxel_at_world_data_position(const Vector2i &world_data_position, const int channel_index) {
	// TODO rework this so it works directly with ints.

	Vector2 pos = world_data_position;

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	Ref<TerrainChunk> chunk = chunk_get(x, z);

	if (chunk.is_valid()) {
		return chunk->get_voxel(bx, bz, channel_index);
	}

	return 0;
}
void TerrainWorld::set_voxel_at_world_data_position(const Vector2i &world_data_position, const uint8_t data, const int channel_index, const bool p_immediate_build, const bool allow_creating_chunks) {
	// TODO rework this so it works directly with ints.

	Vector2 pos = world_data_position;

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	Ref<TerrainChunk> chunk;

	if (get_data_margin_end() > 0) {
		if (bx == 0) {
			if (allow_creating_chunks) {
				chunk = chunk_get_or_create(x - 1, z);
			} else {
				chunk = chunk_get(x - 1, z);
			}

			if (chunk.is_valid()) {
				chunk->set_voxel(data, get_chunk_size_x(), bz, channel_index);

				if (p_immediate_build) {
					chunk->build_immediate();
				} else {
					chunk->build();
				}
			}
		}

		if (bz == 0) {
			if (allow_creating_chunks) {
				chunk = chunk_get_or_create(x, z - 1);
			} else {
				chunk = chunk_get(x, z - 1);
			}

			if (chunk.is_valid()) {
				chunk->set_voxel(data, bx, get_chunk_size_z(), channel_index);

				if (p_immediate_build) {
					chunk->build_immediate();
				} else {
					chunk->build();
				}
			}
		}

		if (bx == 0 && bz == 0) {
			if (allow_creating_chunks) {
				chunk = chunk_get_or_create(x - 1, z - 1);
			} else {
				chunk = chunk_get(x - 1, z - 1);
			}

			if (chunk.is_valid()) {
				chunk->set_voxel(data, get_chunk_size_x(), get_chunk_size_z(), channel_index);

				if (p_immediate_build) {
					chunk->build_immediate();
				} else {
					chunk->build();
				}
			}
		}
	}

	if (get_data_margin_start() > 0) {
		if (bx == get_chunk_size_x() - 1) {
			if (allow_creating_chunks) {
				chunk = chunk_get_or_create(x + 1, z);
			} else {
				chunk = chunk_get(x + 1, z);
			}

			if (chunk.is_valid()) {
				chunk->set_voxel(data, -1, bz, channel_index);

				if (p_immediate_build) {
					chunk->build_immediate();
				} else {
					chunk->build();
				}
			}
		}

		if (bz == get_chunk_size_z() - 1) {
			if (allow_creating_chunks) {
				chunk = chunk_get_or_create(x, z + 1);
			} else {
				chunk = chunk_get(x, z + 1);
			}

			if (chunk.is_valid()) {
				chunk->set_voxel(data, bx, -1, channel_index);

				if (p_immediate_build) {
					chunk->build_immediate();
				} else {
					chunk->build();
				}
			}
		}
	}

	if (allow_creating_chunks) {
		chunk = chunk_get_or_create(x, z);
	} else {
		chunk = chunk_get(x, z);
	}

	if (chunk.is_valid()) {
		chunk->set_voxel(data, bx, bz, channel_index);

		if (p_immediate_build) {
			chunk->build_immediate();
		} else {
			chunk->build();
		}
	}
}
Ref<TerrainChunk> TerrainWorld::get_chunk_at_world_data_position(const Vector2i &world_data_position) {
	// TODO rework this so it works directly with ints.

	Vector2 pos = world_data_position;

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

	return chunk_get(x, z);
}
Ref<TerrainChunk> TerrainWorld::get_or_create_chunk_at_world_data_position(const Vector2i &world_data_position) {
	// TODO rework this so it works directly with ints.

	Vector2 pos = world_data_position;

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

	return chunk_get_or_create(x, z);
}

void TerrainWorld::set_voxels_at_world_data_position(const Array &p_data, const int p_channel_index, const bool p_immediate_build, const bool p_allow_creating_chunks, const bool p_invalidate_texture_caches) {
	ERR_FAIL_COND(p_data.size() % 2 != 0);

	// TODO rework this so it works directly with ints.

	HashSet<Ref<TerrainChunk>> chunks_to_rebuild;

	for (int i = 0; i < p_data.size(); i += 2) {
		Vector2i world_data_position = p_data[i];
		uint8_t value = p_data[i + 1];

		Vector2 pos = world_data_position;

		//Note: floor is needed to handle negative numbers properly
		int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
		int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_z()));

		int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
		int bz = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_z();

		if (bx < 0) {
			bx += get_chunk_size_x();
		}

		if (bz < 0) {
			bz += get_chunk_size_z();
		}

		Ref<TerrainChunk> chunk;

		if (get_data_margin_end() > 0) {
			if (bx == 0) {
				if (p_allow_creating_chunks) {
					chunk = chunk_get_or_create(x - 1, z);
				} else {
					chunk = chunk_get(x - 1, z);
				}

				if (chunk.is_valid()) {
					chunk->set_voxel(value, get_chunk_size_x(), bz, p_channel_index);

					chunks_to_rebuild.insert(chunk);
				}
			}

			if (bz == 0) {
				if (p_allow_creating_chunks) {
					chunk = chunk_get_or_create(x, z - 1);
				} else {
					chunk = chunk_get(x, z - 1);
				}

				if (chunk.is_valid()) {
					chunk->set_voxel(value, bx, get_chunk_size_z(), p_channel_index);

					chunks_to_rebuild.insert(chunk);
				}
			}

			if (bx == 0 && bz == 0) {
				if (p_allow_creating_chunks) {
					chunk = chunk_get_or_create(x - 1, z - 1);
				} else {
					chunk = chunk_get(x - 1, z - 1);
				}

				if (chunk.is_valid()) {
					chunk->set_voxel(value, get_chunk_size_x(), get_chunk_size_z(), p_channel_index);

					chunks_to_rebuild.insert(chunk);
				}
			}
		}

		if (get_data_margin_start() > 0) {
			if (bx == get_chunk_size_x() - 1) {
				if (p_allow_creating_chunks) {
					chunk = chunk_get_or_create(x + 1, z);
				} else {
					chunk = chunk_get(x + 1, z);
				}

				if (chunk.is_valid()) {
					chunk->set_voxel(value, -1, bz, p_channel_index);

					chunks_to_rebuild.insert(chunk);
				}
			}

			if (bz == get_chunk_size_z() - 1) {
				if (p_allow_creating_chunks) {
					chunk = chunk_get_or_create(x, z + 1);
				} else {
					chunk = chunk_get(x, z + 1);
				}

				if (chunk.is_valid()) {
					chunk->set_voxel(value, bx, -1, p_channel_index);

					chunks_to_rebuild.insert(chunk);
				}
			}
		}

		if (p_allow_creating_chunks) {
			chunk = chunk_get_or_create(x, z);
		} else {
			chunk = chunk_get(x, z);
		}

		if (chunk.is_valid()) {
			chunk->set_voxel(value, bx, bz, p_channel_index);

			chunks_to_rebuild.insert(chunk);
		}
	}

	for (HashSet<Ref<TerrainChunk>>::Iterator iter = chunks_to_rebuild.begin(); iter.valid(); iter.next()) {
		Ref<TerrainChunk> chunk = iter.key();

		if (p_invalidate_texture_caches) {
			chunk->material_cache_key_invalid_set(true);
			chunk->liquid_material_cache_key_invalid_set(true);
		}

		if (p_immediate_build) {
			chunk->build_immediate();
		} else {
			chunk->build();
		}
	}
}

int TerrainWorld::get_channel_index_info(const TerrainWorld::ChannelTypeInfo channel_type) {
	return call("_get_channel_index_info", channel_type);
}

Spatial *TerrainWorld::get_editor_camera() {
#if TOOLS_ENABLED
	SpatialEditor *se = SpatialEditor::get_singleton();

	if (!se) {
		return nullptr;
	}

	SpatialEditorViewport *wp = se->get_editor_viewport(0);

	if (!wp) {
		return nullptr;
	}

	return wp->get_camera();

#else
	return nullptr;
#endif
}

TerrainWorld::TerrainWorld() {
	_active = true;
	_editable = false;

	_is_priority_generation = true;
	_max_concurrent_generations = 3;

	_chunk_size_x = 16;
	_chunk_size_z = 16;
	_current_seed = 0;
	_data_margin_start = 0;
	_data_margin_end = 0;
	_world_height = 256;

	_voxel_scale = 1;
	_chunk_spawn_range = 4;

	_player = NULL;
	_max_frame_chunk_build_steps = 0;
	_num_frame_chunk_build_steps = 0;
}

TerrainWorld ::~TerrainWorld() {
	_chunks.clear();
	_chunks_vector.clear();
	_world_areas.clear();
	_voxel_structures.clear();

	_library.unref();
	_level_generator.unref();

	_player = NULL;

	_generation_queue.clear();
	_generating.clear();

	_lights.clear();
}

void TerrainWorld::_generate_chunk(Ref<TerrainChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (_level_generator.is_valid()) {
		_level_generator->generate_chunk(chunk);
	}

	for (int i = 0; i < _voxel_structures.size(); ++i) {
		Ref<TerrainStructure> structure = _voxel_structures.get(i);

		if (!structure.is_valid()) {
			continue;
		}

		if (structure->get_use_aabb()) {
			if (structure->get_chunk_aabb().has_point(Vector3(chunk->get_position_x(), 1, chunk->get_position_z()))) {
				structure->write_to_chunk(chunk);
			}
		} else {
			structure->write_to_chunk(chunk);
		}
	}
}

void TerrainWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_player_bind(get_node_or_null(get_player_path()));

			set_process_internal(true);
			set_physics_process_internal(true);
			set_notify_transform(true);

			if (_library.is_valid()) {
				_library->refresh_rects();
			}

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerrainChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk_setup(chunk);

					chunk->build();
				}
			}
			break;
		}
		case NOTIFICATION_INTERNAL_PROCESS: {
			_num_frame_chunk_build_steps = 0;

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerrainChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->process(get_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_process(get_process_delta_time());
				}
			}

			if (_is_priority_generation && _generation_queue.empty() && _generating.empty()) {
				_is_priority_generation = false;

				call("_generation_finished");

				emit_signal("generation_finished");

				return;
			}

			for (int i = 0; i < _generating.size(); ++i) {
				Ref<TerrainChunk> chunk = _generating.get(i);

				if (!chunk.is_valid()) {
					_generating.remove(i);
					--i;
					continue;
				}

				if (!chunk->get_is_generating()) {
					_generating.remove(i);
					--i;
					continue;
				}

				if (chunk->is_build_aborted() && chunk->is_safe_to_delete()) {
					_generating.remove(i);
					--i;
					continue;
				}
			}

			if (_generating.size() >= _max_concurrent_generations) {
				return;
			}

			if (_generation_queue.size() == 0) {
				return;
			}

			while (_generating.size() < _max_concurrent_generations && _generation_queue.size() != 0) {
				Ref<TerrainChunk> chunk = _generation_queue.get(0);
				_generation_queue.remove(0);

				ERR_FAIL_COND(!chunk.is_valid());

				_generating.push_back(chunk);

				chunk_generate(chunk);
			}
			break;
		}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerrainChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->physics_process(get_physics_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_physics_process(get_physics_process_delta_time());
				}
			}
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerrainChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					if (chunk->get_voxel_world() == this) {
						chunk->exit_tree();
						chunk->set_voxel_world(NULL);
					}
				}
			}
			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerrainChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->world_transform_changed();
				}
			}
			break;
		}
		case MainLoop::NOTIFICATION_QUITTING: {
			if (_library.is_valid()) {
				_library->notification(MainLoop::NOTIFICATION_QUITTING);
			}
		} break;
	}
}

void TerrainWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("chunk_mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	ADD_SIGNAL(MethodInfo("chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	ADD_SIGNAL(MethodInfo("chunk_removed", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	ClassDB::bind_method(D_METHOD("get_active"), &TerrainWorld::get_active);
	ClassDB::bind_method(D_METHOD("set_active", "value"), &TerrainWorld::set_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

	ClassDB::bind_method(D_METHOD("get_editable"), &TerrainWorld::get_editable);
	ClassDB::bind_method(D_METHOD("set_editable", "value"), &TerrainWorld::set_editable);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editable"), "set_editable", "get_editable");

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &TerrainWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &TerrainWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &TerrainWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &TerrainWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_data_margin_start"), &TerrainWorld::get_data_margin_start);
	ClassDB::bind_method(D_METHOD("set_data_margin_start", "value"), &TerrainWorld::set_data_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_start"), "set_data_margin_start", "get_data_margin_start");

	ClassDB::bind_method(D_METHOD("get_data_margin_end"), &TerrainWorld::get_data_margin_end);
	ClassDB::bind_method(D_METHOD("set_data_margin_end", "value"), &TerrainWorld::set_data_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_end"), "set_data_margin_end", "get_data_margin_end");

	ClassDB::bind_method(D_METHOD("get_world_height"), &TerrainWorld::get_world_height);
	ClassDB::bind_method(D_METHOD("set_world_height", "height"), &TerrainWorld::set_world_height);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "world_height"), "set_world_height", "get_world_height");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &TerrainWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "height"), &TerrainWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &TerrainWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &TerrainWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_library"), &TerrainWorld::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "library"), &TerrainWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_level_generator"), &TerrainWorld::get_level_generator);
	ClassDB::bind_method(D_METHOD("set_level_generator", "level_generator"), &TerrainWorld::set_level_generator);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_generator", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLevelGenerator"), "set_level_generator", "get_level_generator");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &TerrainWorld::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &TerrainWorld::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_chunk_spawn_range"), &TerrainWorld::get_chunk_spawn_range);
	ClassDB::bind_method(D_METHOD("set_chunk_spawn_range", "value"), &TerrainWorld::set_chunk_spawn_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_spawn_range"), "set_chunk_spawn_range", "get_chunk_spawn_range");

	ClassDB::bind_method(D_METHOD("get_player_path"), &TerrainWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &TerrainWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");

	ClassDB::bind_method(D_METHOD("get_player"), &TerrainWorld::get_player);
	ClassDB::bind_method(D_METHOD("set_player", "player"), &TerrainWorld::set_player_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_RESOURCE_TYPE, "Spatial", 0), "set_player", "get_player");

	ClassDB::bind_method(D_METHOD("world_area_get", "index"), &TerrainWorld::world_area_get);
	ClassDB::bind_method(D_METHOD("world_area_add", "area"), &TerrainWorld::world_area_add);
	ClassDB::bind_method(D_METHOD("world_area_remove", "index"), &TerrainWorld::world_area_remove);
	ClassDB::bind_method(D_METHOD("world_areas_clear"), &TerrainWorld::world_areas_clear);
	ClassDB::bind_method(D_METHOD("world_area_get_count"), &TerrainWorld::world_area_get_count);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &TerrainWorld::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &TerrainWorld::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &TerrainWorld::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &TerrainWorld::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structures_clear"), &TerrainWorld::voxel_structures_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &TerrainWorld::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &TerrainWorld::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &TerrainWorld::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &TerrainWorld::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "23/20:TerrainStructure", PROPERTY_USAGE_DEFAULT, "TerrainStructure"), "voxel_structures_set", "voxel_structures_get");

	BIND_VMETHOD(MethodInfo("_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	ClassDB::bind_method(D_METHOD("chunk_add", "chunk", "x", "z"), &TerrainWorld::chunk_add);
	ClassDB::bind_method(D_METHOD("chunk_has", "x", "z"), &TerrainWorld::chunk_has);
	ClassDB::bind_method(D_METHOD("chunk_get", "x", "z"), &TerrainWorld::chunk_get);
	ClassDB::bind_method(D_METHOD("chunk_remove", "x", "z"), &TerrainWorld::chunk_remove);
	ClassDB::bind_method(D_METHOD("chunk_remove_index", "index"), &TerrainWorld::chunk_remove_index);

	ClassDB::bind_method(D_METHOD("chunk_get_index", "index"), &TerrainWorld::chunk_get_index);
	ClassDB::bind_method(D_METHOD("chunk_get_count"), &TerrainWorld::chunk_get_count);

	ClassDB::bind_method(D_METHOD("chunks_clear"), &TerrainWorld::chunks_clear);

	ClassDB::bind_method(D_METHOD("chunks_get"), &TerrainWorld::chunks_get);
	ClassDB::bind_method(D_METHOD("chunks_set"), &TerrainWorld::chunks_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "chunks", PROPERTY_HINT_NONE, "23/20:TerrainChunk", PROPERTY_USAGE_DEFAULT, "TerrainChunk"), "chunks_set", "chunks_get");

	ClassDB::bind_method(D_METHOD("generation_queue_add_to", "chunk"), &TerrainWorld::generation_queue_add_to);
	ClassDB::bind_method(D_METHOD("generation_queue_get_index", "index"), &TerrainWorld::generation_queue_get_index);
	ClassDB::bind_method(D_METHOD("generation_queue_remove_index", "index"), &TerrainWorld::generation_queue_remove_index);
	ClassDB::bind_method(D_METHOD("generation_queue_get_size"), &TerrainWorld::generation_queue_get_size);

	ClassDB::bind_method(D_METHOD("generation_add_to", "chunk"), &TerrainWorld::generation_add_to);
	ClassDB::bind_method(D_METHOD("generation_get_index", "index"), &TerrainWorld::generation_get_index);
	ClassDB::bind_method(D_METHOD("generation_remove_index", "index"), &TerrainWorld::generation_remove_index);
	ClassDB::bind_method(D_METHOD("generation_get_size"), &TerrainWorld::generation_get_size);

	ADD_SIGNAL(MethodInfo("generation_finished"));

	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk"), "_create_chunk", PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "z"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	ClassDB::bind_method(D_METHOD("chunk_get_or_create", "x", "z"), &TerrainWorld::chunk_get_or_create);
	ClassDB::bind_method(D_METHOD("chunk_create", "x", "z"), &TerrainWorld::chunk_create);
	ClassDB::bind_method(D_METHOD("chunk_setup", "chunk"), &TerrainWorld::chunk_setup);

	ClassDB::bind_method(D_METHOD("_create_chunk", "x", "z", "chunk"), &TerrainWorld::_create_chunk);
	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &TerrainWorld::_generate_chunk);

	ClassDB::bind_method(D_METHOD("can_chunk_do_build_step"), &TerrainWorld::can_chunk_do_build_step);
	ClassDB::bind_method(D_METHOD("is_position_walkable", "position"), &TerrainWorld::is_position_walkable);
	ClassDB::bind_method(D_METHOD("on_chunk_mesh_generation_finished", "chunk"), &TerrainWorld::on_chunk_mesh_generation_finished);

#ifdef MODULE_PROPS_ENABLED
	ClassDB::bind_method(D_METHOD("prop_add", "transform", "prop", "apply_voxel_scale"), &TerrainWorld::prop_add, DEFVAL(true));
#endif

	//Lights
	ClassDB::bind_method(D_METHOD("light_add", "light"), &TerrainWorld::light_add);
	ClassDB::bind_method(D_METHOD("light_get", "index"), &TerrainWorld::light_get);
	ClassDB::bind_method(D_METHOD("light_remove", "index"), &TerrainWorld::light_remove);
	ClassDB::bind_method(D_METHOD("light_get_count"), &TerrainWorld::light_get_count);
	ClassDB::bind_method(D_METHOD("lights_clear"), &TerrainWorld::lights_clear);

	ClassDB::bind_method(D_METHOD("lights_get"), &TerrainWorld::lights_get);
	ClassDB::bind_method(D_METHOD("lights_set", "chunks"), &TerrainWorld::lights_set);

	ClassDB::bind_method(D_METHOD("get_voxel_at_world_position", "world_position", "channel_index"), &TerrainWorld::get_voxel_at_world_position);
	ClassDB::bind_method(D_METHOD("set_voxel_at_world_position", "world_position", "data", "channel_index", "rebuild"), &TerrainWorld::set_voxel_at_world_position, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_chunk_at_world_position", "world_position"), &TerrainWorld::get_chunk_at_world_position);
	ClassDB::bind_method(D_METHOD("get_or_create_chunk_at_world_position", "world_position"), &TerrainWorld::get_or_create_chunk_at_world_position);
	ClassDB::bind_method(D_METHOD("world_position_to_chunk_position", "world_position"), &TerrainWorld::world_position_to_chunk_position);
	ClassDB::bind_method(D_METHOD("world_data_position_to_chunk_position", "world_data_position"), &TerrainWorld::world_data_position_to_chunk_position);

	ClassDB::bind_method(D_METHOD("world_position_to_world_data_position", "world_position"), &TerrainWorld::world_position_to_world_data_position);
	ClassDB::bind_method(D_METHOD("get_voxel_at_world_data_position", "world_data_position", "channel_index"), &TerrainWorld::get_voxel_at_world_data_position);
	ClassDB::bind_method(D_METHOD("set_voxel_at_world_data_position", "world_data_position", "data", "channel_index", "rebuild", "allow_creating_chunks "), &TerrainWorld::set_voxel_at_world_data_position, DEFVAL(true), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_chunk_at_world_data_position", "world_data_position"), &TerrainWorld::get_chunk_at_world_data_position);
	ClassDB::bind_method(D_METHOD("get_or_create_chunk_at_world_data_position", "world_data_position"), &TerrainWorld::get_or_create_chunk_at_world_data_position);
	ClassDB::bind_method(D_METHOD("set_voxels_at_world_data_position", "data", "channel_index", "immediate_build", "allow_creating_chunks", "invalidate_texture_caches"), &TerrainWorld::set_voxels_at_world_data_position, DEFVAL(false), DEFVAL(true), DEFVAL(true));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::INT, "ret"), "_get_channel_index_info", PropertyInfo(Variant::INT, "channel_type", PROPERTY_HINT_ENUM, BINDING_STRING_CHANNEL_TYPE_INFO)));

	ClassDB::bind_method(D_METHOD("get_channel_index_info", "channel_type"), &TerrainWorld::get_channel_index_info);
	ClassDB::bind_method(D_METHOD("_get_channel_index_info", "channel_type"), &TerrainWorld::_get_channel_index_info);

	ClassDB::bind_method(D_METHOD("get_editor_camera"), &TerrainWorld::get_editor_camera);

	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_TYPE);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_ISOLEVEL);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_TYPE);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_ISOLEVEL);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_FLOW);

	BIND_CONSTANT(NOTIFICATION_ACTIVE_STATE_CHANGED);
}
