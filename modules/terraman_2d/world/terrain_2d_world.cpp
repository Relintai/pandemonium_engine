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

#include "terrain_2d_world.h"

#include "core/object/message_queue.h"
#include "terrain_2d_chunk.h"
#include "terrain_2d_structure.h"

#include "../defines.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_2D_ENABLED
#include "../../props_2d/props/prop_2d_data.h"
#include "../../props_2d/props/prop_2d_data_entry.h"
#include "../../props_2d/props/prop_2d_data_light.h"
#include "../../props_2d/props/prop_2d_data_prop.h"
#include "../../props_2d/props/prop_2d_data_scene.h"
#endif

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif

#if TOOLS_ENABLED
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"
#endif

const String Terrain2DWorld::BINDING_STRING_CHANNEL_TYPE_INFO = "Type,Isolevel,Liquid,Liquid Level";

bool Terrain2DWorld::get_active() const {
	return _active;
}
void Terrain2DWorld::set_active(const bool value) {
	_active = value;

	notification(NOTIFICATION_ACTIVE_STATE_CHANGED);
}

bool Terrain2DWorld::get_editable() const {
	return _editable;
}
void Terrain2DWorld::set_editable(const bool value) {
	_editable = value;
}

int Terrain2DWorld::get_chunk_size_x() const {
	return _chunk_size_x;
}
void Terrain2DWorld::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int Terrain2DWorld::get_chunk_size_y() const {
	return _chunk_size_y;
}
void Terrain2DWorld::set_chunk_size_y(const int value) {
	_chunk_size_y = value;
}

int Terrain2DWorld::get_data_margin_start() const {
	return _data_margin_start;
}
void Terrain2DWorld::set_data_margin_start(const int value) {
	_data_margin_start = value;
}

int Terrain2DWorld::get_data_margin_end() const {
	return _data_margin_end;
}
void Terrain2DWorld::set_data_margin_end(const int value) {
	_data_margin_end = value;
}

int Terrain2DWorld::get_current_seed() const {
	return _current_seed;
}
void Terrain2DWorld::set_current_seed(const int value) {
	_current_seed = value;
}

int Terrain2DWorld::get_max_concurrent_generations() {
	return _max_concurrent_generations;
}
void Terrain2DWorld::set_max_concurrent_generations(const int value) {
	_max_concurrent_generations = value;
}

Ref<Terrain2DLibrary> Terrain2DWorld::get_library() {
	return _library;
}
void Terrain2DWorld::set_library(const Ref<Terrain2DLibrary> &library) {
	_library = library;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<Terrain2DChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_library(_library);
	}
}

Ref<Terrain2DLevelGenerator> Terrain2DWorld::get_level_generator() const {
	return _level_generator;
}
void Terrain2DWorld::set_level_generator(const Ref<Terrain2DLevelGenerator> &level_generator) {
	_level_generator = level_generator;
}

int Terrain2DWorld::get_cell_size_x() const {
	return _cell_size_x;
}
void Terrain2DWorld::set_cell_size_x(const int value) {
	_cell_size_x = value;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<Terrain2DChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_cell_size_x(_cell_size_x);
	}
}

int Terrain2DWorld::get_cell_size_y() const {
	return _cell_size_y;
}
void Terrain2DWorld::set_cell_size_y(const int value) {
	_cell_size_y = value;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<Terrain2DChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_cell_size_y(_cell_size_y);
	}
}

int Terrain2DWorld::get_chunk_spawn_range() const {
	return _chunk_spawn_range;
}
void Terrain2DWorld::set_chunk_spawn_range(const int value) {
	_chunk_spawn_range = value;
}

NodePath Terrain2DWorld::get_player_path() const {
	return _player_path;
}

void Terrain2DWorld::set_player_path(const NodePath &player_path) {
	_player_path = player_path;
}

Node2D *Terrain2DWorld::get_player() const {
	return _player;
}
void Terrain2DWorld::set_player(Node2D *player) {
	_player = player;
}
void Terrain2DWorld::set_player_bind(Node *player) {
	set_player(Object::cast_to<Node2D>(player));
}

Transform2D Terrain2DWorld::mesh_transform_terrain_get() {
	return _mesh_transform_terrain;
}
void Terrain2DWorld::mesh_transform_terrain_set(const Transform2D &value) {
	_mesh_transform_terrain = value;
}

Transform2D Terrain2DWorld::mesh_transform_wall_north_get() {
	return _mesh_transform_wall_north;
}
void Terrain2DWorld::mesh_transform_wall_north_set(const Transform2D &value) {
	_mesh_transform_wall_north = value;
}

Transform2D Terrain2DWorld::mesh_transform_wall_south_get() {
	return _mesh_transform_wall_south;
}
void Terrain2DWorld::mesh_transform_wall_south_set(const Transform2D &value) {
	_mesh_transform_wall_south = value;
}

Transform2D Terrain2DWorld::mesh_transform_wall_east_get() {
	return _mesh_transform_wall_east;
}
void Terrain2DWorld::mesh_transform_wall_east_set(const Transform2D &value) {
	_mesh_transform_wall_east = value;
}

Transform2D Terrain2DWorld::mesh_transform_wall_west_get() {
	return _mesh_transform_wall_west;
}
void Terrain2DWorld::mesh_transform_wall_west_set(const Transform2D &value) {
	_mesh_transform_wall_west = value;
}

Ref<Terrain2DWorldArea> Terrain2DWorld::world_area_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _world_areas.size(), Ref<Terrain2DWorldArea>());

	return _world_areas.get(index);
}
void Terrain2DWorld::world_area_add(const Ref<Terrain2DWorldArea> &area) {
	_world_areas.push_back(area);
}
void Terrain2DWorld::world_area_remove(const int index) {
	ERR_FAIL_INDEX(index, _world_areas.size());

	_world_areas.remove(index);
}
void Terrain2DWorld::world_areas_clear() {
	_world_areas.clear();
}
int Terrain2DWorld::world_area_get_count() const {
	return _world_areas.size();
}

//Terra Structures

Ref<Terrain2DStructure> Terrain2DWorld::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<Terrain2DStructure>());

	return _voxel_structures.get(index);
}
void Terrain2DWorld::voxel_structure_add(const Ref<Terrain2DStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void Terrain2DWorld::voxel_structure_remove(const Ref<Terrain2DStructure> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1)
		_voxel_structures.remove(index);
}
void Terrain2DWorld::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void Terrain2DWorld::voxel_structures_clear() {
	_voxel_structures.clear();
}
int Terrain2DWorld::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void Terrain2DWorld::voxel_structure_add_at_position(Ref<Terrain2DStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _cell_size_x));
	structure->set_position_y(static_cast<int>(world_position.z / _cell_size_y));

	voxel_structure_add(structure);
}

Vector<Variant> Terrain2DWorld::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void Terrain2DWorld::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structures_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<Terrain2DLight> structure = Ref<Terrain2DLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void Terrain2DWorld::chunk_add(Ref<Terrain2DChunk> chunk, const int x, const int z) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND_MSG(chunk->get_voxel_world() != NULL && chunk->get_voxel_world() != this, "Chunk is already owned by an another world!");

	IntPos pos(x, z);

	//ERR_FAIL_COND(_chunks.has(pos));

	chunk->set_voxel_world(this);
	chunk->set_position(x, z);
	chunk->world_transform_changed();

	if (!_chunks.has(pos))
		_chunks.set(pos, chunk);

	if (_chunks_vector.find(chunk) == -1)
		_chunks_vector.push_back(chunk);

	if (is_inside_tree())
		chunk->enter_tree();

	if (has_method("_chunk_added")) {
		call("_chunk_added", chunk);
	}

	emit_signal("chunk_added", chunk);

	update();
}
bool Terrain2DWorld::chunk_has(const int x, const int z) const {
	return _chunks.has(IntPos(x, z));
}
Ref<Terrain2DChunk> Terrain2DWorld::chunk_get(const int x, const int z) {
	IntPos pos(x, z);

	if (_chunks.has(pos))
		return _chunks.get(pos);

	return Ref<Terrain2DChunk>();
}
Ref<Terrain2DChunk> Terrain2DWorld::chunk_remove(const int x, const int z) {
	IntPos pos(x, z);

	if (!_chunks.has(pos)) {
		return NULL;
	}

	Ref<Terrain2DChunk> chunk = _chunks.get(pos);

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
	update();

	return chunk;
}
Ref<Terrain2DChunk> Terrain2DWorld::chunk_remove_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	Ref<Terrain2DChunk> chunk = _chunks_vector.get(index);
	chunk->exit_tree();

	_chunks_vector.remove(index);
	_chunks.erase(IntPos(chunk->get_position_x(), chunk->get_position_y()));
	_generation_queue.erase(chunk);

	if (chunk->get_is_generating()) {
		chunk->cancel_build();
	}

	//never remove from this here
	//_generating.erase(chunk);

	emit_signal("chunk_removed", chunk);
	update();

	return chunk;
}

Ref<Terrain2DChunk> Terrain2DWorld::chunk_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int Terrain2DWorld::chunk_get_count() const {
	return _chunks_vector.size();
}

void Terrain2DWorld::chunks_clear() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<Terrain2DChunk> chunk = _chunks_vector.get(i);

		chunk->exit_tree();

		emit_signal("chunk_removed", chunk);
	}

	_chunks_vector.clear();
	_chunks.clear();
	_generation_queue.clear();

	for (int i = 0; i < _generating.size(); ++i) {
		Ref<Terrain2DChunk> chunk = _generating[i];

		if (chunk->get_is_generating()) {
			chunk->cancel_build();
		}
	}

	//never remove from this here
	//_generating.clear();

	update();
}

Ref<Terrain2DChunk> Terrain2DWorld::chunk_get_or_create(int x, int z) {
	Ref<Terrain2DChunk> chunk = chunk_get(x, z);

	if (!chunk.is_valid()) {
		chunk = chunk_create(x, z);
	}

	return chunk;
}

Ref<Terrain2DChunk> Terrain2DWorld::chunk_create(const int x, const int z) {
	Ref<Terrain2DChunk> c;
	c = call("_create_chunk", x, z, Ref<Terrain2DChunk>());

	generation_queue_add_to(c);

	return c;
}

void Terrain2DWorld::chunk_setup(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	call("_create_chunk", chunk->get_position_x(), chunk->get_position_y(), chunk);
}

Ref<Terrain2DChunk> Terrain2DWorld::_create_chunk(const int x, const int y, Ref<Terrain2DChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk.instance();
	}

	//no meshers here

	ERR_FAIL_COND_V(!chunk.is_valid(), NULL);

	chunk->set_name("Chunk[" + String::num(x) + "," + String::num(y) + "]");

	chunk->set_voxel_world(this);

	chunk->set_position(x, y);
	chunk->set_library(_library);
	chunk->set_cell_size_x(_cell_size_x);
	chunk->set_cell_size_y(_cell_size_y);

	//This way it can be changed per chunk (or for areas)
	chunk->mesh_transform_terrain_set(mesh_transform_terrain_get());
	chunk->mesh_transform_wall_north_set(mesh_transform_wall_north_get());
	chunk->mesh_transform_wall_south_set(mesh_transform_wall_south_get());
	chunk->mesh_transform_wall_east_set(mesh_transform_wall_east_get());
	chunk->mesh_transform_wall_west_set(mesh_transform_wall_west_get());

	chunk->set_size(_chunk_size_x, _chunk_size_y, _data_margin_start, _data_margin_end);
	//chunk->set_translation(Vector3(x * _chunk_size_x * _voxel_scale, y * _chunk_size_y * _voxel_scale, z * _chunk_size_z * _voxel_scale));

	if (!get_active()) {
		chunk->set_visible(false);
	}

	chunk_add(chunk, x, y);

	return chunk;
}

void Terrain2DWorld::chunk_generate(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_prepare_chunk_for_generation")) {
		call("_prepare_chunk_for_generation", chunk);
	}

	call("_generate_chunk", chunk);

	chunk->build();
}

Vector<Variant> Terrain2DWorld::chunks_get() {
	VARIANT_ARRAY_GET(_chunks_vector);
}

void Terrain2DWorld::chunks_set(const Vector<Variant> &chunks) {
	if (is_inside_tree()) {
		for (int i = 0; i < _chunks_vector.size(); ++i) {
			Ref<Terrain2DChunk> chunk = Ref<Terrain2DChunk>(_chunks_vector[i]);

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
			Ref<Terrain2DChunk> chunk = Ref<Terrain2DChunk>(chunks[i]);

			if (!chunk.is_valid())
				continue;

			if (_chunks_vector.find(chunk) != -1)
				continue;

			chunk_add(chunk, chunk->get_position_x(), chunk->get_position_y());
		}
	} else {
		_chunks_vector.clear();

		for (int i = 0; i < chunks.size(); ++i) {
			Ref<Terrain2DChunk> chunk = Ref<Terrain2DChunk>(chunks[i]);

			_chunks_vector.push_back(chunk);
		}
	}
}

int Terrain2DWorld::_get_channel_index_info(const Terrain2DWorld::ChannelTypeInfo channel_type) {
	return -1;
}

void Terrain2DWorld::_set_voxel_with_tool(const bool mode_add, const Vector2 hit_position, const int selected_voxel, const int isolevel) {
	Vector2 pos = hit_position;
	pos.x *= _cell_size_x;
	pos.y *= _cell_size_y;

	int channel_type = get_channel_index_info(Terrain2DWorld::CHANNEL_TYPE_INFO_TYPE);
	int channel_isolevel = get_channel_index_info(Terrain2DWorld::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (channel_isolevel == -1) {
		set_voxel_at_world_position(pos, selected_voxel, channel_type);
	} else {
		set_voxel_at_world_position(pos, selected_voxel, channel_type, false);
		set_voxel_at_world_position(pos, isolevel, channel_isolevel);
	}
}

bool Terrain2DWorld::can_chunk_do_build_step() {
	if (_max_frame_chunk_build_steps == 0) {
		return true;
	}

	return _num_frame_chunk_build_steps++ < _max_frame_chunk_build_steps;
}

bool Terrain2DWorld::is_position_walkable(const Vector2 &p_pos) {
	int x = static_cast<int>(Math::floor(p_pos.x / (_chunk_size_x * _cell_size_x)));
	int z = static_cast<int>(Math::floor(p_pos.y / (_chunk_size_y * _cell_size_y)));

	Ref<Terrain2DChunk> c = chunk_get(x, z);

	if (!c.is_valid())
		return false;

	return !c->get_is_generating();
}

void Terrain2DWorld::on_chunk_mesh_generation_finished(Ref<Terrain2DChunk> p_chunk) {
	call_deferred("emit_signal", "chunk_mesh_generation_finished", p_chunk);
}

void Terrain2DWorld::generation_queue_add_to(const Ref<Terrain2DChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	set_process_internal(true);

	_generation_queue.push_back(chunk);
}
Ref<Terrain2DChunk> Terrain2DWorld::generation_queue_get_index(int index) {
	ERR_FAIL_INDEX_V(index, _generation_queue.size(), NULL);

	return _generation_queue.get(index);
}
void Terrain2DWorld::generation_queue_remove_index(int index) {
	ERR_FAIL_INDEX(index, _generation_queue.size());

	_generation_queue.remove(index);
}
int Terrain2DWorld::generation_queue_get_size() const {
	return _generation_queue.size();
}

void Terrain2DWorld::generation_add_to(const Ref<Terrain2DChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	_generating.push_back(chunk);
}
Ref<Terrain2DChunk> Terrain2DWorld::generation_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _generating.size(), NULL);

	return _generating.get(index);
}
void Terrain2DWorld::generation_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _generating.size());

	_generating.remove(index);
}
int Terrain2DWorld::generation_get_size() const {
	return _generating.size();
}

#ifdef MODULE_PROPS_2D_ENABLED
void Terrain2DWorld::prop_add(Transform transform, const Ref<Prop2DData> &prop, const bool apply_voxel_scale) {
	//TODO
	/*
	ERR_FAIL_COND(!prop.is_valid());

	if (apply_voxel_scale) {
		transform = transform.scaled(Vector3(get_voxel_scale(), get_voxel_scale(), get_voxel_scale()));
	}

	Vector3 wp;
	wp = transform.xform(wp);
	Ref<Terrain2DChunk> chunk = get_or_create_chunk_at_world_position(wp);

	chunk->prop_add(transform, prop);

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<Prop2DDataEntry> entry = prop->get_prop(i);

		if (!entry.is_valid())
			continue;

		Transform t = transform * entry->get_transform();

		wp = t.xform(Vector3());
		chunk = get_or_create_chunk_at_world_position(wp);

		Ref<Prop2DDataProp2D> prop_entry_data = entry;

		if (prop_entry_data.is_valid()) {
			Ref<Prop2DData> p = prop_entry_data->get_prop();

			if (!p.is_valid())
				continue;

			prop_add(t, p, false);

			continue;
		}

		Ref<Prop2DDataScene> scene_data = entry;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid())
				continue;

			Node *n = sc->instance();
			add_child(n);
			n->set_owner(this);

			Spatial *sp = Object::cast_to<Spatial>(n);

			if (sp) {
				sp->set_transform(t);
			}

			continue;
		}

		Ref<Prop2DDataLight> light_data = entry;

		if (light_data.is_valid()) {
			Ref<Terrain2DLight> light;
			light.instance();

			light->set_world_position(wp.x / get_voxel_scale(), wp.y / get_voxel_scale());
			light->set_color(light_data->get_light_color());
			light->set_size(light_data->get_light_size());

			light_add(light);

			continue;
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<Prop2DDataMeshData> mesh_data = entry;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid())
				continue;

			Transform chunk_local_tform = t;

			chunk_local_tform.origin = chunk->to_local(chunk_local_tform.origin);

			chunk->mesh_data_resource_add(chunk_local_tform, mdr, mesh_data->get_texture(), Color(1, 1, 1, 1), false);

			continue;
		}
#endif
	}

	*/
}
#endif

//Lights
void Terrain2DWorld::light_add(const Ref<Terrain2DLight> &light) {
	_lights.push_back(light);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<Terrain2DChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_added(light);
		}
	}
}
Ref<Terrain2DLight> Terrain2DWorld::light_get(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<Terrain2DLight>());

	return _lights.get(index);
}
void Terrain2DWorld::light_remove(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<Terrain2DLight> light = _lights[index];

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<Terrain2DChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_removed(light);
		}
	}
}
int Terrain2DWorld::light_get_count() const {
	return _lights.size();
}
void Terrain2DWorld::lights_clear() {
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<Terrain2DLight> light = _lights[i];

		if (!light.is_valid())
			continue;

		for (int j = 0; j < _chunks_vector.size(); ++j) {
			Ref<Terrain2DChunk> chunk = _chunks_vector[j];

			if (chunk.is_valid()) {
				chunk->world_light_removed(light);
			}
		}
	}

	_lights.clear();
}

Vector<Variant> Terrain2DWorld::lights_get() {
	VARIANT_ARRAY_GET(_lights);
}
void Terrain2DWorld::lights_set(const Vector<Variant> &chunks) {
	lights_clear();

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<Terrain2DLight> light = Ref<Terrain2DLight>(chunks[i]);

		light_add(light);
	}
}

uint8_t Terrain2DWorld::get_voxel_at_world_position(const Vector2 &world_position, const int channel_index) {
	Vector2 pos = world_position;
	pos.x /= _cell_size_x;
	pos.y /= _cell_size_y;

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int by = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_y();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (by < 0) {
		by += get_chunk_size_y();
	}

	Ref<Terrain2DChunk> chunk = chunk_get(x, y);

	if (chunk.is_valid())
		return chunk->get_voxel(bx, by, channel_index);

	return 0;
}

void Terrain2DWorld::set_voxel_at_world_position(const Vector2 &world_position, const uint8_t data, const int channel_index, const bool rebuild) {
	Vector2 pos = world_position;
	pos.x /= _cell_size_x;
	pos.y /= _cell_size_y;

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int by = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_y();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (by < 0) {
		by += get_chunk_size_y();
	}

	if (get_data_margin_end() > 0) {
		if (bx == 0) {
			Ref<Terrain2DChunk> chunk = chunk_get_or_create(x - 1, y);
			chunk->set_voxel(data, get_chunk_size_x(), by, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (by == 0) {
			Ref<Terrain2DChunk> chunk = chunk_get_or_create(x, y - 1);
			chunk->set_voxel(data, bx, get_chunk_size_y(), channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	if (get_data_margin_start() > 0) {
		if (bx == get_chunk_size_x() - 1) {
			Ref<Terrain2DChunk> chunk = chunk_get_or_create(x + 1, y);
			chunk->set_voxel(data, -1, by, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (by == get_chunk_size_y() - 1) {
			Ref<Terrain2DChunk> chunk = chunk_get_or_create(x, y + 1);
			chunk->set_voxel(data, bx, -1, channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	Ref<Terrain2DChunk> chunk = chunk_get_or_create(x, y);
	chunk->set_voxel(data, bx, by, channel_index);

	if (rebuild)
		chunk->build();
}

Ref<Terrain2DChunk> Terrain2DWorld::get_chunk_at_world_position(const Vector2 &world_position) {
	Vector2 pos = world_position;
	pos.x /= _cell_size_x;
	pos.y /= _cell_size_y;

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));

	return chunk_get(x, y);
}
Ref<Terrain2DChunk> Terrain2DWorld::get_or_create_chunk_at_world_position(const Vector2 &world_position) {
	Vector2 pos = world_position;
	pos.x /= _cell_size_x;
	pos.y /= _cell_size_y;

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));

	return chunk_get_or_create(x, z);
}

void Terrain2DWorld::set_voxel_with_tool(const bool mode_add, const Vector2 hit_position, const int selected_voxel, const int isolevel) {
	call("_set_voxel_with_tool", mode_add, hit_position, selected_voxel, isolevel);
}

int Terrain2DWorld::get_channel_index_info(const Terrain2DWorld::ChannelTypeInfo channel_type) {
	return call("_get_channel_index_info", channel_type);
}

Terrain2DWorld::Terrain2DWorld() {
	_active = true;
	_editable = false;

	_is_priority_generation = true;
	_max_concurrent_generations = 3;

	_chunk_size_x = 16;
	_chunk_size_y = 16;
	_current_seed = 0;
	_data_margin_start = 0;
	_data_margin_end = 0;

	_cell_size_x = 32;
	_cell_size_y = 32;
	_chunk_spawn_range = 4;

	_player = NULL;
	_max_frame_chunk_build_steps = 0;
	_num_frame_chunk_build_steps = 0;
}

Terrain2DWorld ::~Terrain2DWorld() {
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

void Terrain2DWorld::_draw() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<Terrain2DChunk> chunk = _chunks_vector[i];

		ERR_CONTINUE(!chunk.is_valid());

		if (chunk->get_visible()) {
			chunk->draw();
		}
	}
}

void Terrain2DWorld::_generate_chunk(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (_level_generator.is_valid())
		_level_generator->generate_chunk(chunk);

	for (int i = 0; i < _voxel_structures.size(); ++i) {
		Ref<Terrain2DStructure> structure = _voxel_structures.get(i);

		if (!structure.is_valid())
			continue;

		if (structure->get_use_rect()) {
			if (structure->get_chunk_rect().has_point(Vector2(chunk->get_position_x(), chunk->get_position_y())))
				structure->write_to_chunk(chunk);
		} else {
			structure->write_to_chunk(chunk);
		}
	}
}

void Terrain2DWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_player_bind(get_node_or_null(get_player_path()));

			set_process_internal(true);
			set_physics_process_internal(true);
			set_notify_transform(true);

			if (_library.is_valid())
				_library->refresh_rects();

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<Terrain2DChunk> chunk = _chunks_vector[i];

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
				Ref<Terrain2DChunk> chunk = _chunks_vector[i];

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
				update();

				return;
			}

			bool needs_update = false;
			for (int i = 0; i < _generating.size(); ++i) {
				Ref<Terrain2DChunk> chunk = _generating.get(i);

				if (!chunk.is_valid()) {
					_generating.remove(i);
					--i;
					continue;
				}

				if (!chunk->get_is_generating()) {
					_generating.remove(i);
					--i;
					needs_update = true;
					continue;
				}

				if (chunk->is_build_aborted() && chunk->is_safe_to_delete()) {
					_generating.remove(i);
					--i;
					continue;
				}
			}

			if (needs_update) {
				update();
			}

			if (_generating.size() >= _max_concurrent_generations)
				return;

			if (_generation_queue.size() == 0)
				return;

			while (_generating.size() < _max_concurrent_generations && _generation_queue.size() != 0) {
				Ref<Terrain2DChunk> chunk = _generation_queue.get(0);
				_generation_queue.remove(0);

				ERR_FAIL_COND(!chunk.is_valid());

				_generating.push_back(chunk);

				chunk_generate(chunk);
			}
			break;
		}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<Terrain2DChunk> chunk = _chunks_vector[i];

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
				Ref<Terrain2DChunk> chunk = _chunks_vector[i];

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
				Ref<Terrain2DChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->world_transform_changed();
				}
			}
			break;
		}
		case NOTIFICATION_DRAW: {
			call("_draw");
		} break;
	}
}

void Terrain2DWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("chunk_mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	ADD_SIGNAL(MethodInfo("chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	ADD_SIGNAL(MethodInfo("chunk_removed", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	ClassDB::bind_method(D_METHOD("get_active"), &Terrain2DWorld::get_active);
	ClassDB::bind_method(D_METHOD("set_active", "value"), &Terrain2DWorld::set_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

	ClassDB::bind_method(D_METHOD("get_editable"), &Terrain2DWorld::get_editable);
	ClassDB::bind_method(D_METHOD("set_editable", "value"), &Terrain2DWorld::set_editable);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editable"), "set_editable", "get_editable");

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &Terrain2DWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &Terrain2DWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &Terrain2DWorld::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &Terrain2DWorld::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_data_margin_start"), &Terrain2DWorld::get_data_margin_start);
	ClassDB::bind_method(D_METHOD("set_data_margin_start", "value"), &Terrain2DWorld::set_data_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_start"), "set_data_margin_start", "get_data_margin_start");

	ClassDB::bind_method(D_METHOD("get_data_margin_end"), &Terrain2DWorld::get_data_margin_end);
	ClassDB::bind_method(D_METHOD("set_data_margin_end", "value"), &Terrain2DWorld::set_data_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_end"), "set_data_margin_end", "get_data_margin_end");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &Terrain2DWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "height"), &Terrain2DWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &Terrain2DWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &Terrain2DWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_library"), &Terrain2DWorld::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "library"), &Terrain2DWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_level_generator"), &Terrain2DWorld::get_level_generator);
	ClassDB::bind_method(D_METHOD("set_level_generator", "level_generator"), &Terrain2DWorld::set_level_generator);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_generator", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLevelGenerator"), "set_level_generator", "get_level_generator");

	ClassDB::bind_method(D_METHOD("get_cell_size_x"), &Terrain2DWorld::get_cell_size_x);
	ClassDB::bind_method(D_METHOD("set_cell_size_x", "value"), &Terrain2DWorld::set_cell_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_x"), "set_cell_size_x", "get_cell_size_x");

	ClassDB::bind_method(D_METHOD("get_cell_size_y"), &Terrain2DWorld::get_cell_size_y);
	ClassDB::bind_method(D_METHOD("set_cell_size_y", "value"), &Terrain2DWorld::set_cell_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_y"), "set_cell_size_y", "get_cell_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_spawn_range"), &Terrain2DWorld::get_chunk_spawn_range);
	ClassDB::bind_method(D_METHOD("set_chunk_spawn_range", "value"), &Terrain2DWorld::set_chunk_spawn_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_spawn_range"), "set_chunk_spawn_range", "get_chunk_spawn_range");

	ClassDB::bind_method(D_METHOD("get_player_path"), &Terrain2DWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &Terrain2DWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");

	ClassDB::bind_method(D_METHOD("get_player"), &Terrain2DWorld::get_player);
	ClassDB::bind_method(D_METHOD("set_player", "player"), &Terrain2DWorld::set_player_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_RESOURCE_TYPE, "node2D", 0), "set_player", "get_player");

	ADD_GROUP("Mesh Transforms", "mesh_transform");
	ClassDB::bind_method(D_METHOD("mesh_transform_terrain_get"), &Terrain2DWorld::mesh_transform_terrain_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_terrain_set", "player"), &Terrain2DWorld::mesh_transform_terrain_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_terrain"), "mesh_transform_terrain_set", "mesh_transform_terrain_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_north_get"), &Terrain2DWorld::mesh_transform_wall_north_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_north_set", "player"), &Terrain2DWorld::mesh_transform_wall_north_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_north"), "mesh_transform_wall_north_set", "mesh_transform_wall_north_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_south_get"), &Terrain2DWorld::mesh_transform_wall_south_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_south_set", "player"), &Terrain2DWorld::mesh_transform_wall_south_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_south"), "mesh_transform_wall_south_set", "mesh_transform_wall_south_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_east_get"), &Terrain2DWorld::mesh_transform_wall_east_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_east_set", "player"), &Terrain2DWorld::mesh_transform_wall_east_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_east"), "mesh_transform_wall_east_set", "mesh_transform_wall_east_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_west_get"), &Terrain2DWorld::mesh_transform_wall_west_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_west_set", "player"), &Terrain2DWorld::mesh_transform_wall_west_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_west"), "mesh_transform_wall_west_set", "mesh_transform_wall_west_get");

	ClassDB::bind_method(D_METHOD("world_area_get", "index"), &Terrain2DWorld::world_area_get);
	ClassDB::bind_method(D_METHOD("world_area_add", "area"), &Terrain2DWorld::world_area_add);
	ClassDB::bind_method(D_METHOD("world_area_remove", "index"), &Terrain2DWorld::world_area_remove);
	ClassDB::bind_method(D_METHOD("world_areas_clear"), &Terrain2DWorld::world_areas_clear);
	ClassDB::bind_method(D_METHOD("world_area_get_count"), &Terrain2DWorld::world_area_get_count);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &Terrain2DWorld::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &Terrain2DWorld::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &Terrain2DWorld::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &Terrain2DWorld::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structures_clear"), &Terrain2DWorld::voxel_structures_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &Terrain2DWorld::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &Terrain2DWorld::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &Terrain2DWorld::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &Terrain2DWorld::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "23/19:Terrain2DStructure", PROPERTY_USAGE_DEFAULT, "Terrain2DStructure"), "voxel_structures_set", "voxel_structures_get");

	BIND_VMETHOD(MethodInfo("_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	ClassDB::bind_method(D_METHOD("chunk_add", "chunk", "x", "y"), &Terrain2DWorld::chunk_add);
	ClassDB::bind_method(D_METHOD("chunk_has", "x", "y"), &Terrain2DWorld::chunk_has);
	ClassDB::bind_method(D_METHOD("chunk_get", "x", "y"), &Terrain2DWorld::chunk_get);
	ClassDB::bind_method(D_METHOD("chunk_remove", "x", "y"), &Terrain2DWorld::chunk_remove);
	ClassDB::bind_method(D_METHOD("chunk_remove_index", "index"), &Terrain2DWorld::chunk_remove_index);

	ClassDB::bind_method(D_METHOD("chunk_get_index", "index"), &Terrain2DWorld::chunk_get_index);
	ClassDB::bind_method(D_METHOD("chunk_get_count"), &Terrain2DWorld::chunk_get_count);

	ClassDB::bind_method(D_METHOD("chunks_clear"), &Terrain2DWorld::chunks_clear);

	ClassDB::bind_method(D_METHOD("chunks_get"), &Terrain2DWorld::chunks_get);
	ClassDB::bind_method(D_METHOD("chunks_set"), &Terrain2DWorld::chunks_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "chunks", PROPERTY_HINT_NONE, "23/19:Terrain2DChunk", PROPERTY_USAGE_DEFAULT, "Terrain2DChunk"), "chunks_set", "chunks_get");

	ClassDB::bind_method(D_METHOD("generation_queue_add_to", "chunk"), &Terrain2DWorld::generation_queue_add_to);
	ClassDB::bind_method(D_METHOD("generation_queue_get_index", "index"), &Terrain2DWorld::generation_queue_get_index);
	ClassDB::bind_method(D_METHOD("generation_queue_remove_index", "index"), &Terrain2DWorld::generation_queue_remove_index);
	ClassDB::bind_method(D_METHOD("generation_queue_get_size"), &Terrain2DWorld::generation_queue_get_size);

	ClassDB::bind_method(D_METHOD("generation_add_to", "chunk"), &Terrain2DWorld::generation_add_to);
	ClassDB::bind_method(D_METHOD("generation_get_index", "index"), &Terrain2DWorld::generation_get_index);
	ClassDB::bind_method(D_METHOD("generation_remove_index", "index"), &Terrain2DWorld::generation_remove_index);
	ClassDB::bind_method(D_METHOD("generation_get_size"), &Terrain2DWorld::generation_get_size);

	ADD_SIGNAL(MethodInfo("generation_finished"));

	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk"), "_create_chunk", PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "y"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	ClassDB::bind_method(D_METHOD("chunk_get_or_create", "x", "y"), &Terrain2DWorld::chunk_get_or_create);
	ClassDB::bind_method(D_METHOD("chunk_create", "x", "y"), &Terrain2DWorld::chunk_create);
	ClassDB::bind_method(D_METHOD("chunk_setup", "chunk"), &Terrain2DWorld::chunk_setup);

	ClassDB::bind_method(D_METHOD("_create_chunk", "x", "y", "chunk"), &Terrain2DWorld::_create_chunk);
	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &Terrain2DWorld::_generate_chunk);

	ClassDB::bind_method(D_METHOD("can_chunk_do_build_step"), &Terrain2DWorld::can_chunk_do_build_step);
	ClassDB::bind_method(D_METHOD("is_position_walkable", "position"), &Terrain2DWorld::is_position_walkable);
	ClassDB::bind_method(D_METHOD("on_chunk_mesh_generation_finished", "chunk"), &Terrain2DWorld::on_chunk_mesh_generation_finished);

#ifdef MODULE_PROPS_2D_ENABLED
	ClassDB::bind_method(D_METHOD("prop_add", "transform", "prop", "apply_voxel_scale"), &Terrain2DWorld::prop_add, DEFVAL(true));
#endif

	//Lights
	ClassDB::bind_method(D_METHOD("light_add", "light"), &Terrain2DWorld::light_add);
	ClassDB::bind_method(D_METHOD("light_get", "index"), &Terrain2DWorld::light_get);
	ClassDB::bind_method(D_METHOD("light_remove", "index"), &Terrain2DWorld::light_remove);
	ClassDB::bind_method(D_METHOD("light_get_count"), &Terrain2DWorld::light_get_count);
	ClassDB::bind_method(D_METHOD("lights_clear"), &Terrain2DWorld::lights_clear);

	ClassDB::bind_method(D_METHOD("lights_get"), &Terrain2DWorld::lights_get);
	ClassDB::bind_method(D_METHOD("lights_set", "chunks"), &Terrain2DWorld::lights_set);

	ClassDB::bind_method(D_METHOD("get_voxel_at_world_position", "world_position", "channel_index"), &Terrain2DWorld::get_voxel_at_world_position);
	ClassDB::bind_method(D_METHOD("set_voxel_at_world_position", "world_position", "data", "channel_index", "rebuild"), &Terrain2DWorld::set_voxel_at_world_position, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_chunk_at_world_position", "world_position"), &Terrain2DWorld::get_chunk_at_world_position);
	ClassDB::bind_method(D_METHOD("get_or_create_chunk_at_world_position", "world_position"), &Terrain2DWorld::get_or_create_chunk_at_world_position);

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::INT, "ret"), "_get_channel_index_info", PropertyInfo(Variant::INT, "channel_type", PROPERTY_HINT_ENUM, BINDING_STRING_CHANNEL_TYPE_INFO)));

	ClassDB::bind_method(D_METHOD("get_channel_index_info", "channel_type"), &Terrain2DWorld::get_channel_index_info);
	ClassDB::bind_method(D_METHOD("_get_channel_index_info", "channel_type"), &Terrain2DWorld::_get_channel_index_info);

	BIND_VMETHOD(MethodInfo("_set_voxel_with_tool",
			PropertyInfo(Variant::BOOL, "mode_add"),
			PropertyInfo(Variant::VECTOR2, "hit_position"),
			PropertyInfo(Variant::INT, "selected_voxel"),
			PropertyInfo(Variant::INT, "isolevel")));

	ClassDB::bind_method(D_METHOD("set_voxel_with_tool", "mode_add", "hit_position", "selected_voxel", "isolevel"), &Terrain2DWorld::set_voxel_with_tool);
	ClassDB::bind_method(D_METHOD("_set_voxel_with_tool", "mode_add", "hit_position", "selected_voxel", "isolevel"), &Terrain2DWorld::_set_voxel_with_tool);

	BIND_VMETHOD(MethodInfo("_draw"));
	ClassDB::bind_method(D_METHOD("_draw"), &Terrain2DWorld::_draw);

	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_TYPE);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_ISOLEVEL);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_FLOW);

	BIND_CONSTANT(NOTIFICATION_ACTIVE_STATE_CHANGED);
}
