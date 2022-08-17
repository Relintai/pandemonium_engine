#ifndef TERRAIN_2D_WORLD_H
#define TERRAIN_2D_WORLD_H
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

#include "core/engine.h"
#include "core/containers/hash_map.h"

#include "../defines.h"

#include "scene/2d/navigation_2d.h"

#include "../areas/terrain_2d_world_area.h"
#include "../level_generator/terrain_2d_level_generator.h"
#include "../library/terrain_2d_library.h"

#include "core/os/os.h"

#if PROPS_2D_PRESENT
#include "../../props_2d/props/prop_2d_data.h"
#endif

class Terrain2DStructure;
class Terrain2DChunk;
class Prop2DData;

class Terrain2DWorld : public Navigation2D {
	GDCLASS(Terrain2DWorld, Navigation2D);

public:
	enum ChannelTypeInfo {
		CHANNEL_TYPE_INFO_TYPE = 0,
		CHANNEL_TYPE_INFO_ISOLEVEL,
		CHANNEL_TYPE_INFO_LIQUID_FLOW,
	};

	enum {
		NOTIFICATION_ACTIVE_STATE_CHANGED = 9000,
	};

	static const String BINDING_STRING_CHANNEL_TYPE_INFO;

public:
	bool get_active() const;
	void set_active(const bool value);

	bool get_editable() const;
	void set_editable(const bool value);

	int get_chunk_size_x() const;
	void set_chunk_size_x(const int value);

	int get_chunk_size_y() const;
	void set_chunk_size_y(const int value);

	int get_data_margin_start() const;
	void set_data_margin_start(const int value);

	int get_data_margin_end() const;
	void set_data_margin_end(const int value);

	int get_current_seed() const;
	void set_current_seed(const int value);

	int get_max_concurrent_generations();
	void set_max_concurrent_generations(const int value);

	Ref<Terrain2DLibrary> get_library();
	void set_library(const Ref<Terrain2DLibrary> &library);

	Ref<Terrain2DLevelGenerator> get_level_generator() const;
	void set_level_generator(const Ref<Terrain2DLevelGenerator> &level_generator);

	int get_cell_size_x() const;
	void set_cell_size_x(const int value);

	int get_cell_size_y() const;
	void set_cell_size_y(const int value);

	int get_chunk_spawn_range() const;
	void set_chunk_spawn_range(const int value);

	NodePath get_player_path() const;
	void set_player_path(const NodePath &player_path);

	Node2D *get_player() const;
	void set_player(Node2D *player);
	void set_player_bind(Node *player);

	Transform2D mesh_transform_terrain_get();
	void mesh_transform_terrain_set(const Transform2D &value);

	Transform2D mesh_transform_wall_north_get();
	void mesh_transform_wall_north_set(const Transform2D &value);

	Transform2D mesh_transform_wall_south_get();
	void mesh_transform_wall_south_set(const Transform2D &value);

	Transform2D mesh_transform_wall_east_get();
	void mesh_transform_wall_east_set(const Transform2D &value);

	Transform2D mesh_transform_wall_west_get();
	void mesh_transform_wall_west_set(const Transform2D &value);

	//World Areas
	Ref<Terrain2DWorldArea> world_area_get(const int index) const;
	void world_area_add(const Ref<Terrain2DWorldArea> &area);
	void world_area_remove(const int index);
	void world_areas_clear();
	int world_area_get_count() const;

	//Terra Structures
	Ref<Terrain2DStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<Terrain2DStructure> &structure);
	void voxel_structure_remove(const Ref<Terrain2DStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structures_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<Terrain2DStructure> structure, const Vector3 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Chunks
	void chunk_add(Ref<Terrain2DChunk> chunk, const int x, const int y);
	bool chunk_has(const int x, const int y) const;
	Ref<Terrain2DChunk> chunk_get(const int x, const int y);
	Ref<Terrain2DChunk> chunk_remove(const int x, const int y);
	Ref<Terrain2DChunk> chunk_remove_index(const int index);
	Ref<Terrain2DChunk> chunk_get_index(const int index);

	int chunk_get_count() const;

	void chunks_clear();

	Ref<Terrain2DChunk> chunk_get_or_create(const int x, const int y);
	Ref<Terrain2DChunk> chunk_create(const int x, const int y);
	void chunk_setup(Ref<Terrain2DChunk> chunk);

	void chunk_generate(Ref<Terrain2DChunk> chunk);

	Vector<Variant> chunks_get();
	void chunks_set(const Vector<Variant> &chunks);

	bool can_chunk_do_build_step();
	bool is_position_walkable(const Vector2 &p_pos);

	void on_chunk_mesh_generation_finished(Ref<Terrain2DChunk> p_chunk);

	void generation_queue_add_to(const Ref<Terrain2DChunk> &chunk);
	Ref<Terrain2DChunk> generation_queue_get_index(const int index);
	void generation_queue_remove_index(const int index);
	int generation_queue_get_size() const;

	void generation_add_to(const Ref<Terrain2DChunk> &chunk);
	Ref<Terrain2DChunk> generation_get_index(const int index);
	void generation_remove_index(const int index);
	int generation_get_size() const;

#if PROPS_2D_PRESENT
	void prop_add(Transform transform, const Ref<Prop2DData> &prop, const bool apply_scale = true);
#endif

	//Lights
	void light_add(const Ref<Terrain2DLight> &light);
	Ref<Terrain2DLight> light_get(const int index);
	void light_remove(const int index);
	int light_get_count() const;
	void lights_clear();

	Vector<Variant> lights_get();
	void lights_set(const Vector<Variant> &chunks);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector2 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector2 &world_position, const uint8_t data, const int channel_index, const bool rebuild = true);
	Ref<Terrain2DChunk> get_chunk_at_world_position(const Vector2 &world_position);
	Ref<Terrain2DChunk> get_or_create_chunk_at_world_position(const Vector2 &world_position);
	void set_voxel_with_tool(const bool mode_add, const Vector2 hit_position, const int selected_voxel, const int isolevel);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

	Terrain2DWorld();
	~Terrain2DWorld();

protected:
	virtual void _draw();
	virtual void _generate_chunk(Ref<Terrain2DChunk> chunk);
	virtual Ref<Terrain2DChunk> _create_chunk(int x, int y, Ref<Terrain2DChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);
	virtual void _set_voxel_with_tool(const bool mode_add, const Vector2 hit_position, const int selected_voxel, const int isolevel);

	virtual void _notification(int p_what);
	static void _bind_methods();

public:
	struct IntPos {
		int x;
		int y;

		IntPos() {
			x = 0;
			y = 0;
		}

		IntPos(int p_x, int p_y) {
			x = p_x;
			y = p_y;
		}

		IntPos(const Vector2 &p) {
			x = p.x;
			y = p.y;
		}
	};

	struct IntPosHasher {
		static _FORCE_INLINE_ uint32_t hash(const IntPos &v) {
			uint32_t hash = hash_djb2_one_32(v.x);
			hash = hash_djb2_one_32(v.x, hash);
			return hash_djb2_one_32(v.y, hash);
		}
	};

private:
	bool _active;
	bool _editable;

	bool _is_priority_generation;

	int _chunk_size_x;
	int _chunk_size_y;
	int _current_seed;
	int _data_margin_start;
	int _data_margin_end;

	Ref<Terrain2DLibrary> _library;
	Ref<Terrain2DLevelGenerator> _level_generator;
	int _cell_size_x;
	int _cell_size_y;
	int _chunk_spawn_range;

	HashMap<IntPos, Ref<Terrain2DChunk>, IntPosHasher> _chunks;
	Vector<Ref<Terrain2DChunk>> _chunks_vector;

	Vector<Ref<Terrain2DWorldArea>> _world_areas;

	Vector<Ref<Terrain2DStructure>> _voxel_structures;

	NodePath _player_path;
	Node2D *_player;

	int _max_concurrent_generations;
	Vector<Ref<Terrain2DChunk>> _generation_queue;
	Vector<Ref<Terrain2DChunk>> _generating;
	int _max_frame_chunk_build_steps;
	int _num_frame_chunk_build_steps;

	Vector<Ref<Terrain2DLight>> _lights;

	Transform2D _mesh_transform_terrain;
	Transform2D _mesh_transform_wall_north;
	Transform2D _mesh_transform_wall_south;
	Transform2D _mesh_transform_wall_east;
	Transform2D _mesh_transform_wall_west;
};

_FORCE_INLINE_ bool operator==(const Terrain2DWorld::IntPos &a, const Terrain2DWorld::IntPos &b) {
	return a.x == b.x && a.y == b.y;
}

VARIANT_ENUM_CAST(Terrain2DWorld::ChannelTypeInfo);

#endif
