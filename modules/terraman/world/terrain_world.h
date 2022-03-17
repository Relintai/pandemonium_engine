#ifndef TERRAIN_WORLD_H
#define TERRAIN_WORLD_H
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




#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/config/engine.h"
#include "core/templates/hash_map.h"
#else
#include "core/engine.h"
#include "core/hash_map.h"
#endif

#include "../defines.h"

#include navigation_h

#include "../areas/terrain_world_area.h"
#include "../level_generator/terrain_level_generator.h"
#include "../library/terrain_library.h"

#include "core/os/os.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

class TerrainStructure;
class TerrainChunk;
class PropData;

class TerrainWorld : public Navigation {
	GDCLASS(TerrainWorld, Navigation);

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

	int get_chunk_size_z() const;
	void set_chunk_size_z(const int value);

	int get_data_margin_start() const;
	void set_data_margin_start(const int value);

	int get_data_margin_end() const;
	void set_data_margin_end(const int value);

	int get_current_seed() const;
	void set_current_seed(const int value);

	float get_world_height() const;
	void set_world_height(const float value);

	int get_max_concurrent_generations();
	void set_max_concurrent_generations(const int value);

	Ref<TerrainLibrary> get_library();
	void set_library(const Ref<TerrainLibrary> &library);

	Ref<TerrainLevelGenerator> get_level_generator() const;
	void set_level_generator(const Ref<TerrainLevelGenerator> &level_generator);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	int get_chunk_spawn_range() const;
	void set_chunk_spawn_range(const int value);

	NodePath get_player_path() const;
	void set_player_path(const NodePath &player_path);

	Spatial *get_player() const;
	void set_player(Spatial *player);
	void set_player_bind(Node *player);

	//World Areas
	Ref<TerrainWorldArea> world_area_get(const int index) const;
	void world_area_add(const Ref<TerrainWorldArea> &area);
	void world_area_remove(const int index);
	void world_areas_clear();
	int world_area_get_count() const;

	//Terra Structures
	Ref<TerrainStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<TerrainStructure> &structure);
	void voxel_structure_remove(const Ref<TerrainStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structures_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<TerrainStructure> structure, const Vector3 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Chunks
	void chunk_add(Ref<TerrainChunk> chunk, const int x, const int z);
	bool chunk_has(const int x, const int z) const;
	Ref<TerrainChunk> chunk_get(const int x, const int z);
	Ref<TerrainChunk> chunk_remove(const int x, const int z);
	Ref<TerrainChunk> chunk_remove_index(const int index);
	Ref<TerrainChunk> chunk_get_index(const int index);

	int chunk_get_count() const;

	void chunks_clear();

	Ref<TerrainChunk> chunk_get_or_create(const int x, const int z);
	Ref<TerrainChunk> chunk_create(const int x, const int z);
	void chunk_setup(Ref<TerrainChunk> chunk);

	void chunk_generate(Ref<TerrainChunk> chunk);

	Vector<Variant> chunks_get();
	void chunks_set(const Vector<Variant> &chunks);

	bool can_chunk_do_build_step();
	bool is_position_walkable(const Vector3 &p_pos);

	void on_chunk_mesh_generation_finished(Ref<TerrainChunk> p_chunk);

	void generation_queue_add_to(const Ref<TerrainChunk> &chunk);
	Ref<TerrainChunk> generation_queue_get_index(const int index);
	void generation_queue_remove_index(const int index);
	int generation_queue_get_size() const;

	void generation_add_to(const Ref<TerrainChunk> &chunk);
	Ref<TerrainChunk> generation_get_index(const int index);
	void generation_remove_index(const int index);
	int generation_get_size() const;

#if PROPS_PRESENT
	void prop_add(Transform transform, const Ref<PropData> &prop, const bool apply_voxel_scale = true);
#endif

	//Lights
	void light_add(const Ref<TerrainLight> &light);
	Ref<TerrainLight> light_get(const int index);
	void light_remove(const int index);
	int light_get_count() const;
	void lights_clear();

	Vector<Variant> lights_get();
	void lights_set(const Vector<Variant> &chunks);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector3 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild = true);
	Ref<TerrainChunk> get_chunk_at_world_position(const Vector3 &world_position);
	Ref<TerrainChunk> get_or_create_chunk_at_world_position(const Vector3 &world_position);
	void set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

	Spatial *get_editor_camera();

#if VERSION_MAJOR >= 4
	GDVIRTUAL1(_chunk_added, Ref<TerrainChunk>);

	GDVIRTUAL0(_generation_finished);

	GDVIRTUAL3R(Ref<TerrainChunk>, _create_chunk, int, int, Ref<TerrainChunk>);
	GDVIRTUAL1(_prepare_chunk_for_generation, Ref<TerrainChunk>);
	GDVIRTUAL1(_generate_chunk, Ref<TerrainChunk>);

	GDVIRTUAL1R(int, _get_channel_index_info, int);

	GDVIRTUAL5(_set_voxel_with_tool, bool, Vector3, Vector3, int, int);
#endif

	TerrainWorld();
	~TerrainWorld();

protected:
	virtual void _generate_chunk(Ref<TerrainChunk> chunk);
	virtual Ref<TerrainChunk> _create_chunk(int x, int z, Ref<TerrainChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);
	virtual void _set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

	virtual void _notification(int p_what);
	static void _bind_methods();

public:
	struct IntPos {
		int x;
		int y;
		int z;

		IntPos() {
			x = 0;
			z = 0;
		}

		IntPos(int p_x, int p_z) {
			x = p_x;
			z = p_z;
		}

		IntPos(const Vector2 &p) {
			x = p.x;
			z = p.y;
		}
	};

	struct IntPosHasher {
		static _FORCE_INLINE_ uint32_t hash(const IntPos &v) {
			uint32_t hash = hash_djb2_one_32(v.x);
			hash = hash_djb2_one_32(v.x, hash);
			return hash_djb2_one_32(v.z, hash);
		}
	};

private:
	bool _active;
	bool _editable;

	bool _is_priority_generation;

	int _chunk_size_x;
	int _chunk_size_z;
	int _current_seed;
	int _data_margin_start;
	int _data_margin_end;
	float _world_height;

	Ref<TerrainLibrary> _library;
	Ref<TerrainLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;

	HashMap<IntPos, Ref<TerrainChunk>, IntPosHasher> _chunks;
	Vector<Ref<TerrainChunk>> _chunks_vector;

	Vector<Ref<TerrainWorldArea>> _world_areas;

	Vector<Ref<TerrainStructure>> _voxel_structures;

	NodePath _player_path;
	Spatial *_player;

	int _max_concurrent_generations;
	Vector<Ref<TerrainChunk>> _generation_queue;
	Vector<Ref<TerrainChunk>> _generating;
	int _max_frame_chunk_build_steps;
	int _num_frame_chunk_build_steps;

	Vector<Ref<TerrainLight>> _lights;
};

_FORCE_INLINE_ bool operator==(const TerrainWorld::IntPos &a, const TerrainWorld::IntPos &b) {
	return a.x == b.x && a.z == b.z;
}

VARIANT_ENUM_CAST(TerrainWorld::ChannelTypeInfo);

#endif
