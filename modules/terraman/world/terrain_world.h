#ifndef TERRAIN_WORLD_H
#define TERRAIN_WORLD_H

/*************************************************************************/
/*  terrain_world.h                                                      */
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

#include "core/config/engine.h"
#include "core/containers/hash_map.h"

#include "../defines.h"

#include "scene/main/spatial.h"

#include "../areas/terrain_world_area.h"
#include "../level_generator/terrain_level_generator.h"
#include "../library/terrain_library.h"

#include "core/os/os.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED
#include "../../props/props/prop_data.h"
#endif

class TerrainStructure;
class TerrainChunk;
class PropData;
class MeshDataResource;
class TerrainWorldChunkDataManager;

class TerrainWorld : public Spatial {
	GDCLASS(TerrainWorld, Spatial);

public:
	enum ChannelTypeInfo {
		CHANNEL_TYPE_INFO_TYPE = 0,
		CHANNEL_TYPE_INFO_ISOLEVEL,
		CHANNEL_TYPE_INFO_LIQUID_TYPE,
		CHANNEL_TYPE_INFO_LIQUID_ISOLEVEL,
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

	Ref<TerrainWorldChunkDataManager> get_world_chunk_data_manager() const;
	void set_world_chunk_data_manager(const Ref<TerrainWorldChunkDataManager> &p_data_manager);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

#ifdef MODULE_VERTEX_LIGHTS_3D_ENABLED
	bool get_use_vertex_lights_3d() const;
	void set_use_vertex_lights_3d(const bool value);
#endif

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
	Ref<TerrainChunk> chunk_get_or_load(const int x, const int z);
	Ref<TerrainChunk> chunk_load(const int x, const int z);
	Ref<TerrainChunk> chunk_create(const int x, const int z);
	void chunk_setup(Ref<TerrainChunk> chunk);

	void chunk_generate(Ref<TerrainChunk> chunk);

	void force_save_all_chunks();

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

	void scene_add(const Ref<PackedScene> &p_scene, const Transform &p_transform = Transform(), const Node *p_node = NULL, const bool p_original = true, const String &p_name = String());

#ifdef MODULE_PROPS_ENABLED
	void prop_add(Transform transform, const Ref<PropData> &prop, const bool apply_voxel_scale = true, const bool p_original = true, const String &p_name = String());
#endif

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	void mesh_data_resource_add(const Ref<MeshDataResource> &p_mesh, const Transform &p_transform, const Ref<Texture> &p_texture = Ref<Texture>(), const Color &p_color = Color(1, 1, 1, 1), const bool p_original = true, const String &p_name = String());
	void mesh_data_resource_add_material(const Ref<MeshDataResource> &p_mesh, const Transform &p_transform, const Ref<Texture> &p_texture = Ref<Texture>(), const Ref<Material> &p_material = Ref<Material>(), const bool p_original = true, const String &p_name = String());
#endif

	//Lights
	void light_add(const Ref<TerrainLight> &light);
	void light_remove(const Ref<TerrainLight> &light);
	void lights_clear();

	void world_light_added(const Ref<TerrainLight> &light);
	void world_light_removed(const Ref<TerrainLight> &light);
	void world_light_moved(const Ref<TerrainLight> &light);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector3 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild = true);
	Ref<TerrainChunk> get_chunk_at_world_position(const Vector3 &world_position);
	Ref<TerrainChunk> get_or_create_chunk_at_world_position(const Vector3 &world_position);
	Vector2i world_position_to_chunk_position(const Vector3 &p_world_position);
	Vector2i world_data_position_to_chunk_position(const Vector2i &p_world_data_position);

	Vector2i world_position_to_world_data_position(const Vector3 &world_position);
	Vector3i world_position_to_world_data_position_3d(const Vector3 &world_position);
	Vector3 world_data_position_to_world_position(const Vector2i &p_position);
	Vector3 world_data_position_to_world_position_3d(const Vector3i &p_position);

	uint8_t get_voxel_at_world_data_position(const Vector2i &world_data_position, const int channel_index);
	void set_voxel_at_world_data_position(const Vector2i &world_data_position, const uint8_t data, const int channel_index, const bool p_immediate_build = true, const bool allow_creating_chunks = true);
	Ref<TerrainChunk> get_chunk_at_world_data_position(const Vector2i &world_data_position);
	Ref<TerrainChunk> get_or_create_chunk_at_world_data_position(const Vector2i &world_data_position);
	void set_voxels_at_world_data_position(const Array &p_data, const int p_channel_index, const bool p_immediate_build = false, const bool p_allow_creating_chunks = true, const bool p_invalidate_texture_caches = true);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

	Spatial *get_editor_camera();

	TerrainWorld();
	~TerrainWorld();

protected:
	virtual void _generate_chunk(Ref<TerrainChunk> chunk);
	virtual Ref<TerrainChunk> _create_chunk(int x, int z, Ref<TerrainChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);

	virtual void _validate_property(PropertyInfo &property) const;
	virtual void _notification(int p_what);
	static void _bind_methods();

#ifdef MODULE_VERTEX_LIGHTS_3D_ENABLED
	void _on_vertex_lights_3d_map_changed(RID p_map);
#endif

public:
	struct IntPos {
		int x;
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
#ifdef MODULE_VERTEX_LIGHTS_3D_ENABLED
	bool _use_vertex_lights_3d;
#endif

	Ref<TerrainLibrary> _library;
	Ref<TerrainLevelGenerator> _level_generator;
	Ref<TerrainWorldChunkDataManager> _world_chunk_data_manager;
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
};

_FORCE_INLINE_ bool operator==(const TerrainWorld::IntPos &a, const TerrainWorld::IntPos &b) {
	return a.x == b.x && a.z == b.z;
}

VARIANT_ENUM_CAST(TerrainWorld::ChannelTypeInfo);

#endif
