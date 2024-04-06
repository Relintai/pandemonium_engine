#ifndef LAYERED_TILE_MAP_LAYER_H
#define LAYERED_TILE_MAP_LAYER_H

/*************************************************************************/
/*  layered_tile_map_layer.h                                             */
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

#include "layered_tile_set.h"
#include "scene/2d/y_sort.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_FASTNOISE_ENABLED
#include "../fastnoise/noise.h"
#endif

class LayeredTileSetAtlasSource;
class LayeredTileMap;

enum LayeredTileMapLayerDataFormat {
	LAYERED_TILE_MAP_LAYER_DATA_FORMAT_0 = 0,
	LAYERED_TILE_MAP_LAYER_DATA_FORMAT_MAX,
};

class TerrainConstraint {
private:
	Ref<LayeredTileSet> tile_set;
	Vector2i base_cell_coords;
	int bit = -1;
	int terrain = -1;

	int priority = 1;

public:
	bool operator<(const TerrainConstraint &p_other) const {
		if (base_cell_coords == p_other.base_cell_coords) {
			return bit < p_other.bit;
		}
		return base_cell_coords < p_other.base_cell_coords;
	}

	String to_string() const {
		return vformat("Constraint {pos:%s, bit:%d, terrain:%d, priority:%d}", base_cell_coords, bit, terrain, priority);
	}

	Vector2i get_base_cell_coords() const {
		return base_cell_coords;
	}

	bool is_center_bit() const {
		return bit == 0;
	}

	HashMap<Vector2i, LayeredTileSet::CellNeighbor> get_overlapping_coords_and_peering_bits() const;

	void set_terrain(int p_terrain) {
		terrain = p_terrain;
	}

	int get_terrain() const {
		return terrain;
	}

	void set_priority(int p_priority) {
		priority = p_priority;
	}

	int get_priority() const {
		return priority;
	}

	TerrainConstraint(Ref<LayeredTileSet> p_tile_set, const Vector2i &p_position, int p_terrain); // For the center terrain bit
	TerrainConstraint(Ref<LayeredTileSet> p_tile_set, const Vector2i &p_position, const LayeredTileSet::CellNeighbor &p_bit, int p_terrain); // For peering bits
	TerrainConstraint(){};
};

#ifdef DEBUG_ENABLED
class DebugQuadrant;
#endif // DEBUG_ENABLED
class RenderingQuadrant;

struct CellData {
	Vector2i coords;
	LayeredTileMapCell cell;

#ifdef MODULE_FASTNOISE_ENABLED
	uint8_t rao;
#endif

	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	Color vertex_light_color;
#endif

	// Debug.
	SelfList<CellData> debug_quadrant_list_element;

	// Rendering.
	Ref<RenderingQuadrant> rendering_quadrant;
	SelfList<CellData> rendering_quadrant_list_element;
	LocalVector<RID> occluders;

	// Physics.
	LocalVector<RID> bodies;

	// Navigation.
	LocalVector<RID> navigation_regions;

	// Scenes.
	String scene;

	// Runtime LayeredTileData cache.
	LayeredTileData *runtime_tile_data_cache = nullptr;

	// List elements.
	SelfList<CellData> dirty_list_element;

	bool operator<(const CellData &p_other) const {
		return coords < p_other.coords;
	}

	// For those, copy everything but SelfList elements.
	void operator=(const CellData &p_other) {
		coords = p_other.coords;
		cell = p_other.cell;
		occluders = p_other.occluders;
		bodies = p_other.bodies;
		navigation_regions = p_other.navigation_regions;
		scene = p_other.scene;
		runtime_tile_data_cache = p_other.runtime_tile_data_cache;
	}

	CellData(const CellData &p_other) :
			debug_quadrant_list_element(this),
			rendering_quadrant_list_element(this),
			dirty_list_element(this) {
		coords = p_other.coords;
		cell = p_other.cell;
		occluders = p_other.occluders;
		bodies = p_other.bodies;
		navigation_regions = p_other.navigation_regions;
		scene = p_other.scene;
		runtime_tile_data_cache = p_other.runtime_tile_data_cache;
	}

	CellData() :
			debug_quadrant_list_element(this),
			rendering_quadrant_list_element(this),
			dirty_list_element(this) {
	}
};

// For compatibility reasons, we use another comparator for Y-sorted layers.
struct CellDataYSortedComparator {
	_FORCE_INLINE_ bool operator()(const CellData &p_a, const CellData &p_b) const {
		return p_a.coords.x == p_b.coords.x ? (p_a.coords.y < p_b.coords.y) : (p_a.coords.x > p_b.coords.x);
	}
};

#ifdef DEBUG_ENABLED
class DebugQuadrant : public Reference {
	GDCLASS(DebugQuadrant, Reference);

public:
	Vector2i quadrant_coords;
	SelfList<CellData>::List cells;
	RID canvas_item;

	SelfList<DebugQuadrant> dirty_quadrant_list_element;

	DebugQuadrant() :
			dirty_quadrant_list_element(this) {
	}

	~DebugQuadrant() {
		cells.clear();
	}
};
#endif // DEBUG_ENABLED

class RenderingQuadrant : public Reference {
	GDCLASS(RenderingQuadrant, Reference);

public:
	struct CoordsWorldComparator {
		_ALWAYS_INLINE_ bool operator()(const Vector2 &p_a, const Vector2 &p_b) const {
			// We sort the cells by their local coords, as it is needed by rendering.
			if (p_a.y == p_b.y) {
				return p_a.x > p_b.x;
			} else {
				return p_a.y < p_b.y;
			}
		}
	};

	Vector2i quadrant_coords;
	SelfList<CellData>::List cells;
	List<RID> canvas_items;
	Vector2 canvas_items_position;

	SelfList<RenderingQuadrant> dirty_quadrant_list_element;

	RenderingQuadrant() :
			dirty_quadrant_list_element(this) {
	}

	~RenderingQuadrant() {
		cells.clear();
	}
};

class LayeredTileMapLayer : public YSort {
	GDCLASS(LayeredTileMapLayer, YSort);

public:
	enum HighlightMode {
		HIGHLIGHT_MODE_DEFAULT,
		HIGHLIGHT_MODE_ABOVE,
		HIGHLIGHT_MODE_BELOW,
	};

	enum DebugVisibilityMode {
		DEBUG_VISIBILITY_MODE_DEFAULT,
		DEBUG_VISIBILITY_MODE_FORCE_SHOW,
		DEBUG_VISIBILITY_MODE_FORCE_HIDE,
	};

	enum DirtyFlags {
		DIRTY_FLAGS_LAYER_ENABLED = 0,
		
		DIRTY_FLAGS_LAYER_IN_TREE,
		DIRTY_FLAGS_LAYER_IN_CANVAS,
		DIRTY_FLAGS_LAYER_LOCAL_TRANSFORM,
		DIRTY_FLAGS_LAYER_VISIBILITY,
		DIRTY_FLAGS_LAYER_SELF_MODULATE,
		DIRTY_FLAGS_LAYER_Y_SORT_ENABLED,
		DIRTY_FLAGS_LAYER_Y_SORT_ORIGIN,
		DIRTY_FLAGS_LAYER_Z_INDEX,
		DIRTY_FLAGS_LAYER_LIGHT_MASK,
		DIRTY_FLAGS_LAYER_TEXTURE_FILTER,
		DIRTY_FLAGS_LAYER_TEXTURE_REPEAT,
		DIRTY_FLAGS_LAYER_RENDERING_QUADRANT_SIZE,
		DIRTY_FLAGS_LAYER_COLLISION_ENABLED,
		DIRTY_FLAGS_LAYER_USE_KINEMATIC_BODIES,
		DIRTY_FLAGS_LAYER_COLLISION_VISIBILITY_MODE,
		DIRTY_FLAGS_LAYER_NAVIGATION_ENABLED,
		DIRTY_FLAGS_LAYER_NAVIGATION_MAP,
		DIRTY_FLAGS_LAYER_NAVIGATION_VISIBILITY_MODE,
		DIRTY_FLAGS_LAYER_RUNTIME_UPDATE,

	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
		DIRTY_FLAGS_LAYER_VERTEX_LIGHTS,
#endif

#ifdef MODULE_FASTNOISE_ENABLED
		DIRTY_FLAGS_LAYER_RAO,
#endif

		DIRTY_FLAGS_LAYER_INDEX_IN_TILE_MAP_NODE, // For compatibility.

		DIRTY_FLAGS_LAYER_GROUP_SELECTED_LAYERS,
		DIRTY_FLAGS_LAYER_GROUP_HIGHLIGHT_SELECTED,
		
		DIRTY_FLAGS_TILE_SET,

		DIRTY_FLAGS_MAX,
	};

private:
	// Properties.
	HashMap<Vector2i, CellData> tile_map_layer_data;
	
	bool enabled = true;
	
	Ref<LayeredTileSet> tile_set;

	HighlightMode highlight_mode = HIGHLIGHT_MODE_DEFAULT;
	
	int y_sort_origin = 0;
	int rendering_quadrant_size = 16;
	
	bool collision_enabled = true;
	bool use_kinematic_bodies = false;
	DebugVisibilityMode collision_visibility_mode = DEBUG_VISIBILITY_MODE_DEFAULT;

	bool navigation_enabled = true;
	RID navigation_map_override;
	DebugVisibilityMode navigation_visibility_mode = DEBUG_VISIBILITY_MODE_DEFAULT;

	// Internal.
	HashMap<Vector2i, CellData> tile_map;
	bool pending_update = false;

	// For keeping compatibility with LayeredTileMap.
	LayeredTileMap *tile_map_node = nullptr;
	int layer_index_in_tile_map_node = -1;

	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	bool _use_vertex_lights;
#endif

	//RAO
#ifdef MODULE_FASTNOISE_ENABLED
	real_t _rao_strength;
	Ref<FastNoise> _rao_noise;
#endif

	// Dirty flag. Allows knowing what was modified since the last update.
	struct {
		bool flags[DIRTY_FLAGS_MAX] = { false };
		SelfList<CellData>::List cell_list;
	} dirty;
	bool in_destructor = false;

	// Rect cache.
	mutable Rect2 rect_cache;
	mutable bool rect_cache_dirty = true;
	mutable Rect2i used_rect_cache;
	mutable bool used_rect_cache_dirty = true;

	// Runtime tile data.
	bool _runtime_update_tile_data_was_cleaned_up = false;
	void _build_runtime_update_tile_data();
	void _build_runtime_update_tile_data_for_cell(CellData &r_cell_data, bool p_use_tilemap_for_runtime, bool p_auto_add_to_dirty_list = false);
	bool _runtime_update_needs_all_cells_cleaned_up = false;
	void _clear_runtime_update_tile_data();
	void _clear_runtime_update_tile_data_for_cell(CellData &r_cell_data);

	// Per-system methods.
#ifdef DEBUG_ENABLED
	HashMap<Vector2i, Ref<DebugQuadrant>> debug_quadrant_map;
	Vector2i _coords_to_debug_quadrant_coords(const Vector2i &p_coords) const;
	bool _debug_was_cleaned_up = false;
	void _debug_update();
	void _debug_quadrants_update_cell(CellData &r_cell_data, SelfList<DebugQuadrant>::List &r_dirty_debug_quadrant_list);
#endif // DEBUG_ENABLED

	HashMap<Vector2i, Ref<RenderingQuadrant>> rendering_quadrant_map;
	bool _rendering_was_cleaned_up = false;
	void _rendering_update();
	void _rendering_notification(int p_what);
	void _rendering_quadrants_update_cell(CellData &r_cell_data, SelfList<RenderingQuadrant>::List &r_dirty_rendering_quadrant_list);
	void _rendering_occluders_clear_cell(CellData &r_cell_data);
	void _rendering_occluders_update_cell(CellData &r_cell_data);
#ifdef DEBUG_ENABLED
	void _rendering_draw_cell_debug(const RID &p_canvas_item, const Vector2 &p_quadrant_pos, const CellData &r_cell_data);
#endif // DEBUG_ENABLED

	HashMap<RID, Vector2i> bodies_coords; // Mapping for RID to coords.
	bool _physics_was_cleaned_up = false;
	void _physics_update();
	void _physics_notification(int p_what);
	void _physics_clear_cell(CellData &r_cell_data);
	void _physics_update_cell(CellData &r_cell_data);
#ifdef DEBUG_ENABLED
	void _physics_draw_cell_debug(const RID &p_canvas_item, const Vector2 &p_quadrant_pos, const CellData &r_cell_data);
#endif // DEBUG_ENABLED

	bool _navigation_was_cleaned_up = false;
	void _navigation_update();
	void _navigation_notification(int p_what);
	void _navigation_clear_cell(CellData &r_cell_data);
	void _navigation_update_cell(CellData &r_cell_data);
#ifdef DEBUG_ENABLED
	void _navigation_draw_cell_debug(const RID &p_canvas_item, const Vector2 &p_quadrant_pos, const CellData &r_cell_data);
#endif // DEBUG_ENABLED

	bool _scenes_was_cleaned_up = false;
	void _scenes_update();
	void _scenes_clear_cell(CellData &r_cell_data);
	void _scenes_update_cell(CellData &r_cell_data);
#ifdef DEBUG_ENABLED
	void _scenes_draw_cell_debug(const RID &p_canvas_item, const Vector2 &p_quadrant_pos, const CellData &r_cell_data);
#endif // DEBUG_ENABLED

	// Terrains.
	LayeredTileSet::TerrainsPattern _get_best_terrain_pattern_for_constraints(int p_terrain_set, const Vector2i &p_position, const RBSet<TerrainConstraint> &p_constraints, LayeredTileSet::TerrainsPattern p_current_pattern) const;
	RBSet<TerrainConstraint> _get_terrain_constraints_from_added_pattern(const Vector2i &p_position, int p_terrain_set, LayeredTileSet::TerrainsPattern p_terrains_pattern) const;
	RBSet<TerrainConstraint> _get_terrain_constraints_from_painted_cells_list(const RBSet<Vector2i> &p_painted, int p_terrain_set, bool p_ignore_empty_terrains) const;

	void _tile_set_changed();

	void _renamed();
	void _update_notify_local_transform();

	// Internal updates.
	void _queue_internal_update();
	void _deferred_internal_update();
	void _internal_update();
	
	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	void _on_vertex_lights_map_changed(RID map);
#endif

protected:
	void _notification(int p_what);
	
	static void _bind_methods();

public:
	// LayeredTileMap node.
	void set_as_tile_map_internal_node(int p_index);
	
	int get_index_in_tile_map() const {
		return layer_index_in_tile_map_node;
	}
	const HashMap<Vector2i, CellData> &get_tile_map_layer_data() const {
		return tile_map_layer_data;
	}

	// Rect caching.
	Rect2 get_rect(bool &r_changed) const;

	// Terrains.
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_constraints(const Vector<Vector2i> &p_to_replace, int p_terrain_set, const RBSet<TerrainConstraint> &p_constraints) const; // Not exposed.
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_connect(const Vector<Vector2i> &p_coords_array, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true) const; // Not exposed.
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_path(const Vector<Vector2i> &p_coords_array, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true) const; // Not exposed.
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_pattern(const Vector<Vector2i> &p_coords_array, int p_terrain_set, LayeredTileSet::TerrainsPattern p_terrains_pattern, bool p_ignore_empty_terrains = true) const; // Not exposed.

	// Not exposed to users.
	LayeredTileMapCell get_cell(const Vector2i &p_coords) const;

	////////////// Exposed functions //////////////

	// --- Cells manipulation ---
	// Generic cells manipulations and data access.
	void set_cell(const Vector2i &p_coords, int p_source_id = LayeredTileSet::INVALID_SOURCE, const Vector2i &p_atlas_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS, int p_alternative_tile = 0);
	void erase_cell(const Vector2i &p_coords);

	void fix_invalid_tiles();
	void clear();
	
	int get_cell_source_id(const Vector2i &p_coords) const;
	Vector2i get_cell_atlas_coords(const Vector2i &p_coords) const;
	int get_cell_alternative_tile(const Vector2i &p_coords) const;
	LayeredTileData *get_cell_tile_data(const Vector2i &p_coords) const; // Helper method to make accessing the data easier.

	PoolVector2iArray get_used_cells() const;
	PoolVector2iArray get_used_cells_by_id(int p_source_id = LayeredTileSet::INVALID_SOURCE, const Vector2i &p_atlas_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS, int p_alternative_tile = LayeredTileSetSource::INVALID_TILE_ALTERNATIVE) const;
	Rect2i get_used_rect() const;

	// Patterns.
	Ref<LayeredTileMapPattern> get_pattern(PoolVector2iArray p_coords_array);
	void set_pattern(const Vector2i &p_position, const Ref<LayeredTileMapPattern> p_pattern);

	// Terrains.
	void set_cells_terrain_connect(PoolVector2iArray p_cells, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);
	void set_cells_terrain_path(PoolVector2iArray p_path, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);

	// --- Physics helpers ---
	bool has_body_rid(RID p_physics_body) const;
	Vector2i get_coords_for_body_rid(RID p_physics_body) const; // For finding tiles from collision.

	// --- Runtime ---
	void update_internals();
	void notify_runtime_tile_data_update();

	// --- Shortcuts to methods defined in TileSet ---
	Vector2i map_pattern(const Vector2i &p_position_in_tilemap, const Vector2i &p_coords_in_pattern, Ref<LayeredTileMapPattern> p_pattern);
	PoolVector2iArray get_surrounding_cells(const Vector2i &p_coords);
	Vector2i get_neighbor_cell(const Vector2i &p_coords, LayeredTileSet::CellNeighbor p_cell_neighbor) const;
	Vector2 map_to_local(const Vector2i &p_pos) const;
	Vector2i local_to_map(const Vector2 &p_pos) const;

	// --- Accessors ---
	void set_tile_map_data_from_array(const Vector<uint8_t> &p_data);
	Vector<uint8_t> get_tile_map_data_as_array() const;

	void set_enabled(bool p_enabled);
	bool is_enabled() const;
	void set_tile_set(const Ref<LayeredTileSet> &p_tile_set);
	Ref<LayeredTileSet> get_tile_set() const;

	void set_highlight_mode(HighlightMode p_highlight_mode);
	HighlightMode get_highlight_mode() const;
	
	virtual void set_self_modulate(const Color &p_self_modulate);
	virtual void set_y_sort_enabled(bool p_y_sort_enabled);
	void set_y_sort_origin(int p_y_sort_origin);
	int get_y_sort_origin() const;
	virtual void set_z_index(int p_z_index);
	virtual void set_light_mask(int p_light_mask);
	void set_rendering_quadrant_size(int p_size);
	int get_rendering_quadrant_size() const;

	void set_collision_enabled(bool p_enabled);
	bool is_collision_enabled() const;
	void set_use_kinematic_bodies(bool p_use_kinematic_bodies);
	bool is_using_kinematic_bodies() const;
	void set_collision_visibility_mode(DebugVisibilityMode p_show_collision);
	DebugVisibilityMode get_collision_visibility_mode() const;

	void set_navigation_enabled(bool p_enabled);
	bool is_navigation_enabled() const;
	void set_navigation_map(RID p_map);
	RID get_navigation_map() const;
	void set_navigation_visibility_mode(DebugVisibilityMode p_show_navigation);
	DebugVisibilityMode get_navigation_visibility_mode() const;
	
	// Virtual function to modify the LayeredTileData at runtime.
	bool use_tile_data_runtime_update(const Vector2i &p_coords);
	void tile_data_runtime_update(const Vector2i &p_coords, LayeredTileData *p_tile_data);


	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	void set_use_vertex_lights(const bool p_use);
	bool get_use_vertex_lights() const;
#endif

	//RAO
#ifdef MODULE_FASTNOISE_ENABLED
	Ref<FastNoise> get_rao_noise();
	void set_rao_noise(const Ref<FastNoise> &noise);

	void set_rao_strength(const real_t p_strength);
	real_t get_rao_strength() const;
#endif
	
	LayeredTileMapLayer();
	~LayeredTileMapLayer();
};

VARIANT_ENUM_CAST(LayeredTileMapLayer::DebugVisibilityMode);

#endif // TILE_MAP_LAYER_H
