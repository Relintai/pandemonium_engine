#ifndef LAYERED_TILE_MAP_H
#define LAYERED_TILE_MAP_H

/*************************************************************************/
/*  layered_tile_map.h                                                   */
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

#include "layered_tile_map_layer.h"
#include "layered_tile_set.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_FASTNOISE_ENABLED
#include "modules/fastnoise/fastnoise_noise_params.h"
#include "modules/fastnoise/noise.h"
#endif

class Control;
class LayeredTileMapLayer;
class TerrainConstraint;

enum LayeredTileMapDataFormat {
	TILE_MAP_DATA_FORMAT_1 = 0,
	TILE_MAP_DATA_FORMAT_2,
	TILE_MAP_DATA_FORMAT_3,
	TILE_MAP_DATA_FORMAT_MAX,
};

class LayeredTileMap : public YSort {
	GDCLASS(LayeredTileMap, YSort)

public:
	// Kept for compatibility, but should use LayeredTileMapLayer::VisibilityMode instead.
	enum VisibilityMode {
		VISIBILITY_MODE_DEFAULT,
		VISIBILITY_MODE_FORCE_SHOW,
		VISIBILITY_MODE_FORCE_HIDE,
	};

private:
	friend class LayeredTileSetPlugin;

	// A compatibility enum to specify how is the data if formatted.
	mutable LayeredTileMapDataFormat format = LayeredTileMapDataFormat::TILE_MAP_DATA_FORMAT_3;

	static constexpr float FP_ADJUST = 0.00001;

	// Properties.
	Ref<LayeredTileSet> tile_set;
	int rendering_quadrant_size = 16;
	bool collision_animatable = false;
	VisibilityMode collision_visibility_mode = VISIBILITY_MODE_DEFAULT;
	VisibilityMode navigation_visibility_mode = VISIBILITY_MODE_DEFAULT;

	// Layers.
	LocalVector<LayeredTileMapLayer *> layers;
	LayeredTileMapLayer *default_layer; // Dummy layer to fetch default values.

	// Transforms for collision_animatable.
	Transform2D last_valid_transform;
	Transform2D new_transform;

	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	bool _use_vertex_lights;
#endif

	//RAO
#ifdef MODULE_FASTNOISE_ENABLED
	bool _use_rao;
	real_t _rao_strength;
	Ref<FastnoiseNoiseParams> _noise_params;

	Ref<FastNoise> _rao_noise;
#endif

	void _tile_set_changed();

	void _emit_changed();

	// Kept for compatibility with TileMap. With TileMapLayers as individual nodes, the format is stored directly in the array.
	void _set_tile_map_data_using_compatibility_format(int p_layer, LayeredTileMapDataFormat p_format, const Vector<int> &p_data);
	Vector<int> _get_tile_map_data_using_compatibility_format(int p_layer) const;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;

	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const;
#endif

#ifndef DISABLE_DEPRECATED
	void force_update(int p_layer);
#endif

	void set_rendering_quadrant_size(int p_size);
	int get_rendering_quadrant_size() const;

	static void draw_tile(RID p_canvas_item, const Vector2 &p_position, const Ref<LayeredTileSet> p_tile_set, int p_atlas_source_id, const Vector2i &p_atlas_coords, int p_alternative_tile, int p_frame = -1, Color p_modulation = Color(1.0, 1.0, 1.0, 1.0), const LayeredTileData *p_tile_data_override = nullptr, real_t p_normalized_animation_offset = 0.0);

	// Accessors.
	void set_tileset(const Ref<LayeredTileSet> &p_tileset);
	Ref<LayeredTileSet> get_tileset() const;

	// Layers management.
	int get_layers_count() const;
	void add_layer(int p_to_pos);
	void move_layer(int p_layer, int p_to_pos);
	void remove_layer(int p_layer);
	LayeredTileMapLayer *get_layer(const int p_layer) const;

	void set_layer_name(int p_layer, String p_name);
	String get_layer_name(int p_layer) const;
	void set_layer_enabled(int p_layer, bool p_visible);
	bool is_layer_enabled(int p_layer) const;
	void set_layer_modulate(int p_layer, Color p_modulate);
	Color get_layer_modulate(int p_layer) const;
	void set_layer_y_sort_enabled(int p_layer, bool p_enabled);
	bool is_layer_y_sort_enabled(int p_layer) const;
	void set_layer_y_sort_origin(int p_layer, int p_y_sort_origin);
	int get_layer_y_sort_origin(int p_layer) const;
	void set_layer_z_index(int p_layer, int p_z_index);
	int get_layer_z_index(int p_layer) const;
	void set_layer_navigation_enabled(int p_layer, bool p_enabled);
	bool is_layer_navigation_enabled(int p_layer) const;
	void set_layer_navigation_map(int p_layer, RID p_map);
	RID get_layer_navigation_map(int p_layer) const;

	void set_collision_animatable(bool p_collision_animatable);
	bool is_collision_animatable() const;

	// Debug visibility modes.
	void set_collision_visibility_mode(VisibilityMode p_show_collision);
	VisibilityMode get_collision_visibility_mode() const;

	void set_navigation_visibility_mode(VisibilityMode p_show_navigation);
	VisibilityMode get_navigation_visibility_mode() const;

	// Cells accessors.
	void set_cell(int p_layer, const Vector2i &p_coords, int p_source_id = LayeredTileSet::INVALID_SOURCE, const Vector2i p_atlas_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS, int p_alternative_tile = 0);
	void erase_cell(int p_layer, const Vector2i &p_coords);
	int get_cell_source_id(int p_layer, const Vector2i &p_coords, bool p_use_proxies = false) const;
	Vector2i get_cell_atlas_coords(int p_layer, const Vector2i &p_coords, bool p_use_proxies = false) const;
	int get_cell_alternative_tile(int p_layer, const Vector2i &p_coords, bool p_use_proxies = false) const;
	// Helper method to make accessing the data easier.
	LayeredTileData *get_cell_tile_data(int p_layer, const Vector2i &p_coords, bool p_use_proxies = false) const;

	// Patterns.
	Ref<LayeredTileMapPattern> get_pattern(int p_layer, PoolVector2iArray p_coords_array);
	Vector2i map_pattern(const Vector2i &p_position_in_tilemap, const Vector2i &p_coords_in_pattern, Ref<LayeredTileMapPattern> p_pattern);
	void set_pattern(int p_layer, const Vector2i &p_position, const Ref<LayeredTileMapPattern> p_pattern);

	// Terrains (Not exposed).
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_constraints(int p_layer, const Vector<Vector2i> &p_to_replace, int p_terrain_set, const RBSet<TerrainConstraint> &p_constraints);
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_connect(int p_layer, const Vector<Vector2i> &p_coords_array, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_path(int p_layer, const Vector<Vector2i> &p_coords_array, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);
	HashMap<Vector2i, LayeredTileSet::TerrainsPattern> terrain_fill_pattern(int p_layer, const Vector<Vector2i> &p_coords_array, int p_terrain_set, LayeredTileSet::TerrainsPattern p_terrains_pattern, bool p_ignore_empty_terrains = true);

	// Terrains (exposed).
	void set_cells_terrain_connect(int p_layer, PoolVector2iArray p_cells, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);
	void set_cells_terrain_path(int p_layer, PoolVector2iArray p_path, int p_terrain_set, int p_terrain, bool p_ignore_empty_terrains = true);

	//VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	void set_use_vertex_lights(const bool p_use);
	bool get_use_vertex_lights() const;
#endif

	//RAO
#ifdef MODULE_FASTNOISE_ENABLED
	void rao_set_use(bool p_rao);
	bool rao_get_use() const;

	void rao_set_strength(const real_t p_strength);
	real_t rao_get_strength() const;

	void rao_set_noise_params(const Ref<FastnoiseNoiseParams> &noise);
	Ref<FastnoiseNoiseParams> rao_get_noise_params();
#endif

	// Not exposed to users.
	LayeredTileMapCell get_cell(int p_layer, const Vector2i &p_coords, bool p_use_proxies = false) const;
	int get_effective_quadrant_size(int p_layer) const;

	virtual void set_y_sort_enabled(bool p_enable);

	Vector2 map_to_local(const Vector2i &p_pos) const;
	Vector2i local_to_map(const Vector2 &p_pos) const;
	bool is_existing_neighbor(LayeredTileSet::CellNeighbor p_cell_neighbor) const;
	Vector2i get_neighbor_cell(const Vector2i &p_coords, LayeredTileSet::CellNeighbor p_cell_neighbor) const;

	PoolVector2iArray get_used_cells(int p_layer) const;
	PoolVector2iArray get_used_cells_by_id(int p_layer, int p_source_id = LayeredTileSet::INVALID_SOURCE, const Vector2i p_atlas_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS, int p_alternative_tile = LayeredTileSetSource::INVALID_TILE_ALTERNATIVE) const;
	Rect2i get_used_rect() const;

	// Override some methods of the CanvasItem class to pass the changes to the quadrants CanvasItems.
	virtual void set_light_mask(int p_light_mask);
	virtual void set_self_modulate(const Color &p_self_modulate);

	// For finding tiles from collision.
	Vector2i get_coords_for_body_rid(RID p_physics_body);
	// For getting their layers as well.
	int get_layer_for_body_rid(RID p_physics_body);

	// Fixing and clearing methods.
	void fix_invalid_tiles();

#ifdef TOOLS_ENABLED
	// Moving layers outside of TileMap.
	LayeredTileMapLayer *duplicate_layer_from_internal(int p_layer);
#endif // TOOLS_ENABLED

	// Clears tiles from a given layer.
	void clear_layer(int p_layer);
	void clear();

	// Force a LayeredTileMap update.
	void update_internals();
	void notify_runtime_tile_data_update(int p_layer = -1);

	// Helpers?
	PoolVector2iArray get_surrounding_cells(const Vector2i &p_coords);

	// Virtual function to modify the LayeredTileData at runtime.
	bool use_tile_data_runtime_update(const int p_layer, const Vector2i &p_coords);
	void tile_data_runtime_update(const int p_layer, const Vector2i &p_coords, LayeredTileData *p_tile_data);

	// Configuration warnings.
	String get_configuration_warning() const;

	LayeredTileMap();
	~LayeredTileMap();
};

VARIANT_ENUM_CAST(LayeredTileMap::VisibilityMode);

#endif // TILE_MAP_H
