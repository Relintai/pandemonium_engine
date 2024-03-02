/*************************************************************************/
/*  layered_tile_set.cpp                                                 */
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
#include "layered_tile_set.compat.inc"

#include "core/containers/local_vector.h"
#include "core/containers/rb_set.h"
#include "core/core_string_names.h"
#include "core/io/marshalls.h"
#include "core/math/geometry.h"
#include "scene/main/control.h"
#include "scene/resources/mesh/mesh.h"
#include "scene/resources/texture.h"
#include "servers/navigation_2d_server.h"

/////////////////////////////// LayeredTileMapPattern //////////////////////////////////////

void LayeredTileMapPattern::_set_tile_data(const Vector<int> &p_data) {
	int c = p_data.size();
	const int *r = p_data.ptr();

	int offset = 3;
	ERR_FAIL_COND_MSG(c % offset != 0, "Corrupted tile data.");

	clear();

	for (int i = 0; i < c; i += offset) {
		const uint8_t *ptr = (const uint8_t *)&r[i];
		uint8_t local[12];
		for (int j = 0; j < 12; j++) {
			local[j] = ptr[j];
		}

#ifdef BIG_ENDIAN_ENABLED
		SWAP(local[0], local[3]);
		SWAP(local[1], local[2]);
		SWAP(local[4], local[7]);
		SWAP(local[5], local[6]);
		SWAP(local[8], local[11]);
		SWAP(local[9], local[10]);
#endif

		int16_t x = decode_uint16(&local[0]);
		int16_t y = decode_uint16(&local[2]);
		uint16_t source_id = decode_uint16(&local[4]);
		uint16_t atlas_coords_x = decode_uint16(&local[6]);
		uint16_t atlas_coords_y = decode_uint16(&local[8]);
		uint16_t alternative_tile = decode_uint16(&local[10]);
		set_cell(Vector2i(x, y), source_id, Vector2i(atlas_coords_x, atlas_coords_y), alternative_tile);
	}
	emit_signal("changed");
}

Vector<int> LayeredTileMapPattern::_get_tile_data() const {
	// Export tile data to raw format
	Vector<int> data;
	data.resize(pattern.size() * 3);
	int *w = data.ptrw();

	// Save in highest format

	int idx = 0;

	for (const HashMap<Vector2i, LayeredTileMapCell>::Element *E = pattern.front(); E; E = E->next) {
		uint8_t *ptr = (uint8_t *)&w[idx];
		encode_uint16((int16_t)(E->key().x), &ptr[0]);
		encode_uint16((int16_t)(E->key().y), &ptr[2]);
		encode_uint16(E->value().source_id, &ptr[4]);
		encode_uint16(E->value().coord_x, &ptr[6]);
		encode_uint16(E->value().coord_y, &ptr[8]);
		encode_uint16(E->value().alternative_tile, &ptr[10]);
		idx += 3;
	}

	return data;
}

void LayeredTileMapPattern::set_cell(const Vector2i &p_coords, int p_source_id, const Vector2i p_atlas_coords, int p_alternative_tile) {
	ERR_FAIL_COND_MSG(p_coords.x < 0 || p_coords.y < 0, vformat("Cannot set cell with negative coords in a LayeredTileMapPattern. Wrong coords: %s", p_coords));

	size = size.max(p_coords + Vector2i(1, 1));
	pattern[p_coords] = LayeredTileMapCell(p_source_id, p_atlas_coords, p_alternative_tile);
	emit_changed();
}

bool LayeredTileMapPattern::has_cell(const Vector2i &p_coords) const {
	return pattern.has(p_coords);
}

void LayeredTileMapPattern::remove_cell(const Vector2i &p_coords, bool p_update_size) {
	ERR_FAIL_COND(!pattern.has(p_coords));

	pattern.erase(p_coords);
	if (p_update_size) {
		size = Size2i();
		for (const HashMap<Vector2i, LayeredTileMapCell>::Element *E = pattern.front(); E; E = E->next) {
			size = size.max(E->key() + Vector2i(1, 1));
		}
	}
	emit_changed();
}

int LayeredTileMapPattern::get_cell_source_id(const Vector2i &p_coords) const {
	ERR_FAIL_COND_V(!pattern.has(p_coords), LayeredTileSet::INVALID_SOURCE);

	return pattern[p_coords].source_id;
}

Vector2i LayeredTileMapPattern::get_cell_atlas_coords(const Vector2i &p_coords) const {
	ERR_FAIL_COND_V(!pattern.has(p_coords), LayeredTileSetSource::INVALID_ATLAS_COORDS);

	return pattern[p_coords].get_atlas_coords();
}

int LayeredTileMapPattern::get_cell_alternative_tile(const Vector2i &p_coords) const {
	ERR_FAIL_COND_V(!pattern.has(p_coords), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);

	return pattern[p_coords].alternative_tile;
}

PoolVector2iArray LayeredTileMapPattern::get_used_cells() const {
	// Returns the cells used in the tilemap.
	PoolVector2iArray a;
	a.resize(pattern.size());
	int i = 0;
	for (const HashMap<Vector2i, LayeredTileMapCell>::Element *E = pattern.front(); E; E = E->next) {
		Vector2i p(E->key().x, E->key().y);
		a[i++] = p;
	}

	return a;
}

Size2i LayeredTileMapPattern::get_size() const {
	return size;
}

void LayeredTileMapPattern::set_size(const Size2i &p_size) {
	for (const HashMap<Vector2i, LayeredTileMapCell>::Element *E = pattern.front(); E; E = E->next) {
		Vector2i coords = E->key();
		if (p_size.x <= coords.x || p_size.y <= coords.y) {
			ERR_FAIL_MSG(vformat("Cannot set pattern size to %s, it contains a tile at %s. Size can only be increased.", p_size, coords));
		};
	}

	size = p_size;
	emit_changed();
}

bool LayeredTileMapPattern::empty() const {
	return pattern.empty();
};

void LayeredTileMapPattern::clear() {
	size = Size2i();
	pattern.clear();
	emit_changed();
};

bool LayeredTileMapPattern::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "tile_data") {
		if (p_value.is_array()) {
			_set_tile_data(p_value);
			return true;
		}
		return false;
	}
	return false;
}

bool LayeredTileMapPattern::_get(const StringName &p_name, Variant &r_ret) const {
	if (p_name == "tile_data") {
		r_ret = _get_tile_data();
		return true;
	}
	return false;
}

void LayeredTileMapPattern::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::OBJECT, "tile_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL));
}

void LayeredTileMapPattern::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_cell", "coords", "source_id", "atlas_coords", "alternative_tile"), &LayeredTileMapPattern::set_cell, DEFVAL(LayeredTileSet::INVALID_SOURCE), DEFVAL(LayeredTileSetSource::INVALID_ATLAS_COORDS), DEFVAL(LayeredTileSetSource::INVALID_TILE_ALTERNATIVE));
	ClassDB::bind_method(D_METHOD("has_cell", "coords"), &LayeredTileMapPattern::has_cell);
	ClassDB::bind_method(D_METHOD("remove_cell", "coords", "update_size"), &LayeredTileMapPattern::remove_cell);
	ClassDB::bind_method(D_METHOD("get_cell_source_id", "coords"), &LayeredTileMapPattern::get_cell_source_id);
	ClassDB::bind_method(D_METHOD("get_cell_atlas_coords", "coords"), &LayeredTileMapPattern::get_cell_atlas_coords);
	ClassDB::bind_method(D_METHOD("get_cell_alternative_tile", "coords"), &LayeredTileMapPattern::get_cell_alternative_tile);

	ClassDB::bind_method(D_METHOD("get_used_cells"), &LayeredTileMapPattern::get_used_cells);
	ClassDB::bind_method(D_METHOD("get_size"), &LayeredTileMapPattern::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &LayeredTileMapPattern::set_size);
	ClassDB::bind_method(D_METHOD("empty"), &LayeredTileMapPattern::empty);
}

/////////////////////////////// LayeredTileSet //////////////////////////////////////

bool LayeredTileSet::TerrainsPattern::is_valid() const {
	return valid;
}

bool LayeredTileSet::TerrainsPattern::is_erase_pattern() const {
	return not_empty_terrains_count == 0;
}

bool LayeredTileSet::TerrainsPattern::operator<(const TerrainsPattern &p_terrains_pattern) const {
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (is_valid_bit[i] != p_terrains_pattern.is_valid_bit[i]) {
			return is_valid_bit[i] < p_terrains_pattern.is_valid_bit[i];
		}
	}
	if (terrain != p_terrains_pattern.terrain) {
		return terrain < p_terrains_pattern.terrain;
	}
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (is_valid_bit[i] && bits[i] != p_terrains_pattern.bits[i]) {
			return bits[i] < p_terrains_pattern.bits[i];
		}
	}
	return false;
}

bool LayeredTileSet::TerrainsPattern::operator==(const TerrainsPattern &p_terrains_pattern) const {
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (is_valid_bit[i] != p_terrains_pattern.is_valid_bit[i]) {
			return false;
		}
		if (is_valid_bit[i] && bits[i] != p_terrains_pattern.bits[i]) {
			return false;
		}
	}
	if (terrain != p_terrains_pattern.terrain) {
		return false;
	}
	return true;
}

void LayeredTileSet::TerrainsPattern::set_terrain(int p_terrain) {
	ERR_FAIL_COND(p_terrain < -1);

	terrain = p_terrain;
}

int LayeredTileSet::TerrainsPattern::get_terrain() const {
	return terrain;
}

void LayeredTileSet::TerrainsPattern::set_terrain_peering_bit(LayeredTileSet::CellNeighbor p_peering_bit, int p_terrain) {
	ERR_FAIL_COND(p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_MAX);
	ERR_FAIL_COND(!is_valid_bit[p_peering_bit]);
	ERR_FAIL_COND(p_terrain < -1);

	// Update the "is_erase_pattern" status.
	if (p_terrain >= 0 && bits[p_peering_bit] < 0) {
		not_empty_terrains_count++;
	} else if (p_terrain < 0 && bits[p_peering_bit] >= 0) {
		not_empty_terrains_count--;
	}

	bits[p_peering_bit] = p_terrain;
}

int LayeredTileSet::TerrainsPattern::get_terrain_peering_bit(LayeredTileSet::CellNeighbor p_peering_bit) const {
	ERR_FAIL_COND_V(p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_MAX, -1);
	ERR_FAIL_COND_V(!is_valid_bit[p_peering_bit], -1);
	return bits[p_peering_bit];
}

void LayeredTileSet::TerrainsPattern::from_array(Array p_terrains) {
	set_terrain(p_terrains[0]);
	int in_array_index = 1;
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (is_valid_bit[i]) {
			ERR_FAIL_INDEX(in_array_index, p_terrains.size());
			set_terrain_peering_bit(LayeredTileSet::CellNeighbor(i), p_terrains[in_array_index]);
			in_array_index++;
		}
	}
}

Array LayeredTileSet::TerrainsPattern::as_array() const {
	Array output;
	output.push_back(get_terrain());
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (is_valid_bit[i]) {
			output.push_back(bits[i]);
		}
	}
	return output;
}

LayeredTileSet::TerrainsPattern::TerrainsPattern(const LayeredTileSet *p_tile_set, int p_terrain_set) {
	ERR_FAIL_COND(p_terrain_set < 0);
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		is_valid_bit[i] = (p_tile_set->is_valid_terrain_peering_bit(p_terrain_set, LayeredTileSet::CellNeighbor(i)));
		bits[i] = -1;
	}
	valid = true;
}

const int LayeredTileSet::INVALID_SOURCE = -1;

const char *LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[] = {
	"right_side",
	"right_corner",
	"bottom_right_side",
	"bottom_right_corner",
	"bottom_side",
	"bottom_corner",
	"bottom_left_side",
	"bottom_left_corner",
	"left_side",
	"left_corner",
	"top_left_side",
	"top_left_corner",
	"top_side",
	"top_corner",
	"top_right_side",
	"top_right_corner",
};

// -- Shape and layout --
void LayeredTileSet::set_tile_shape(LayeredTileSet::TileShape p_shape) {
	tile_shape = p_shape;

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *E_source = sources.front(); E_source; E_source = E_source->next) {
		E_source->value()->notify_tile_data_properties_should_change();
	}

	terrain_bits_meshes_dirty = true;
	tile_meshes_dirty = true;
	property_list_changed_notify();
	emit_changed();
}
LayeredTileSet::TileShape LayeredTileSet::get_tile_shape() const {
	return tile_shape;
}

void LayeredTileSet::set_tile_layout(LayeredTileSet::TileLayout p_layout) {
	tile_layout = p_layout;
	emit_changed();
}
LayeredTileSet::TileLayout LayeredTileSet::get_tile_layout() const {
	return tile_layout;
}

void LayeredTileSet::set_tile_offset_axis(LayeredTileSet::TileOffsetAxis p_alignment) {
	tile_offset_axis = p_alignment;

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *E_source = sources.front(); E_source; E_source = E_source->next) {
		E_source->value()->notify_tile_data_properties_should_change();
	}

	terrain_bits_meshes_dirty = true;
	tile_meshes_dirty = true;
	emit_changed();
}
LayeredTileSet::TileOffsetAxis LayeredTileSet::get_tile_offset_axis() const {
	return tile_offset_axis;
}

void LayeredTileSet::set_tile_size(Size2i p_size) {
	ERR_FAIL_COND(p_size.x < 1 || p_size.y < 1);
	tile_size = p_size;
	terrain_bits_meshes_dirty = true;
	tile_meshes_dirty = true;
	emit_changed();
}
Size2i LayeredTileSet::get_tile_size() const {
	return tile_size;
}

int LayeredTileSet::get_next_source_id() const {
	return next_source_id;
}

void LayeredTileSet::_update_terrains_cache() {
	if (terrains_cache_dirty) {
		// Organizes tiles into structures.
		per_terrain_pattern_tiles.resize(terrain_sets.size());

		for (int i = 0; i < per_terrain_pattern_tiles.size(); ++i) {
			per_terrain_pattern_tiles[i].clear();
		}

		for (const HashMap<int, Ref<LayeredTileSetSource>>::Element *kv = sources.front(); kv; kv = kv->next) {
			Ref<LayeredTileSetSource> source = kv->value();
			Ref<LayeredTileSetAtlasSource> atlas_source = source;
			if (atlas_source.is_valid()) {
				for (int tile_index = 0; tile_index < source->get_tiles_count(); tile_index++) {
					Vector2i tile_id = source->get_tile_id(tile_index);
					for (int alternative_index = 0; alternative_index < source->get_alternative_tiles_count(tile_id); alternative_index++) {
						int alternative_id = source->get_alternative_tile_id(tile_id, alternative_index);

						// Executed for each tile_data.
						LayeredTileData *tile_data = atlas_source->get_tile_data(tile_id, alternative_id);
						int terrain_set = tile_data->get_terrain_set();
						if (terrain_set >= 0) {
							LayeredTileMapCell cell;
							cell.source_id = kv->key();
							cell.set_atlas_coords(tile_id);
							cell.alternative_tile = alternative_id;

							LayeredTileSet::TerrainsPattern terrains_pattern = tile_data->get_terrains_pattern();

							// Main terrain.
							if (terrains_pattern.get_terrain() >= 0) {
								per_terrain_pattern_tiles[terrain_set][terrains_pattern].insert(cell);
							}

							// Terrain bits.
							for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
								CellNeighbor bit = CellNeighbor(i);
								if (is_valid_terrain_peering_bit(terrain_set, bit)) {
									int terrain = terrains_pattern.get_terrain_peering_bit(bit);
									if (terrain >= 0) {
										per_terrain_pattern_tiles[terrain_set][terrains_pattern].insert(cell);
									}
								}
							}
						}
					}
				}
			}
		}

		// Add the empty cell in the possible patterns and cells.
		for (int i = 0; i < terrain_sets.size(); i++) {
			LayeredTileSet::TerrainsPattern empty_pattern(this, i);

			LayeredTileMapCell empty_cell;
			empty_cell.source_id = LayeredTileSet::INVALID_SOURCE;
			empty_cell.set_atlas_coords(LayeredTileSetSource::INVALID_ATLAS_COORDS);
			empty_cell.alternative_tile = LayeredTileSetSource::INVALID_TILE_ALTERNATIVE;
			per_terrain_pattern_tiles[i][empty_pattern].insert(empty_cell);
		}
		terrains_cache_dirty = false;
	}
}

void LayeredTileSet::_compute_next_source_id() {
	while (sources.has(next_source_id)) {
		next_source_id = (next_source_id + 1) % 1073741824; // 2 ** 30
	};
}

// Sources management
int LayeredTileSet::add_source(Ref<LayeredTileSetSource> p_tile_set_source, int p_atlas_source_id_override) {
	ERR_FAIL_COND_V(!p_tile_set_source.is_valid(), LayeredTileSet::INVALID_SOURCE);
	ERR_FAIL_COND_V_MSG(p_atlas_source_id_override >= 0 && (sources.has(p_atlas_source_id_override)), LayeredTileSet::INVALID_SOURCE, vformat("Cannot create LayeredTileSet atlas source. Another atlas source exists with id %d.", p_atlas_source_id_override));
	ERR_FAIL_COND_V_MSG(p_atlas_source_id_override < 0 && p_atlas_source_id_override != LayeredTileSet::INVALID_SOURCE, LayeredTileSet::INVALID_SOURCE, vformat("Provided source ID %d is not valid. Negative source IDs are not allowed.", p_atlas_source_id_override));

	int new_source_id = p_atlas_source_id_override >= 0 ? p_atlas_source_id_override : next_source_id;
	sources[new_source_id] = p_tile_set_source;
	source_ids.push_back(new_source_id);
	source_ids.sort();
	LayeredTileSet *old_tileset = p_tile_set_source->get_tile_set();
	if (old_tileset != this && old_tileset != nullptr) {
		// If the source is already in a LayeredTileSet (might happen when duplicating), remove it from the other LayeredTileSet.
		old_tileset->remove_source_ptr(p_tile_set_source.ptr());
	}
	p_tile_set_source->set_tile_set(this);
	_compute_next_source_id();

	sources[new_source_id]->connect(CoreStringNames::get_singleton()->changed, this, "_source_changed");

	terrains_cache_dirty = true;
	emit_changed();

	return new_source_id;
}

void LayeredTileSet::remove_source(int p_source_id) {
	ERR_FAIL_COND_MSG(!sources.has(p_source_id), vformat("Cannot remove LayeredTileSet atlas source. No tileset atlas source with id %d.", p_source_id));

	sources[p_source_id]->connect(CoreStringNames::get_singleton()->changed, this, "_source_changed");

	sources[p_source_id]->set_tile_set(nullptr);
	sources.erase(p_source_id);
	source_ids.erase(p_source_id);
	source_ids.sort();

	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::remove_source_ptr(LayeredTileSetSource *p_tile_set_source) {
	for (const HashMap<int, Ref<LayeredTileSetSource>>::Element *kv = sources.front(); kv; kv = kv->next) {
		if (kv->value().ptr() == p_tile_set_source) {
			remove_source(kv->key());
			return;
		}
	}
	ERR_FAIL_MSG(vformat("Attempting to remove source from a tileset, but the tileset doesn't have it: %s", p_tile_set_source));
}

void LayeredTileSet::set_source_id(int p_source_id, int p_new_source_id) {
	ERR_FAIL_COND(p_new_source_id < 0);
	ERR_FAIL_COND_MSG(!sources.has(p_source_id), vformat("Cannot change LayeredTileSet atlas source ID. No tileset atlas source with id %d.", p_source_id));
	if (p_source_id == p_new_source_id) {
		return;
	}

	ERR_FAIL_COND_MSG(sources.has(p_new_source_id), vformat("Cannot change LayeredTileSet atlas source ID. Another atlas source exists with id %d.", p_new_source_id));

	sources[p_new_source_id] = sources[p_source_id];
	sources.erase(p_source_id);

	source_ids.erase(p_source_id);
	source_ids.push_back(p_new_source_id);
	source_ids.sort();

	_compute_next_source_id();

	terrains_cache_dirty = true;
	emit_changed();
}

bool LayeredTileSet::has_source(int p_source_id) const {
	return sources.has(p_source_id);
}

Ref<LayeredTileSetSource> LayeredTileSet::get_source(int p_source_id) const {
	ERR_FAIL_COND_V_MSG(!sources.has(p_source_id), nullptr, vformat("No LayeredTileSet atlas source with id %d.", p_source_id));

	return sources[p_source_id];
}

int LayeredTileSet::get_source_count() const {
	return source_ids.size();
}

int LayeredTileSet::get_source_id(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, source_ids.size(), LayeredTileSet::INVALID_SOURCE);
	return source_ids[p_index];
}

// Rendering
void LayeredTileSet::set_uv_clipping(bool p_uv_clipping) {
	if (uv_clipping == p_uv_clipping) {
		return;
	}
	uv_clipping = p_uv_clipping;
	emit_changed();
}

bool LayeredTileSet::is_uv_clipping() const {
	return uv_clipping;
};

int LayeredTileSet::get_occlusion_layers_count() const {
	return occlusion_layers.size();
};

void LayeredTileSet::add_occlusion_layer(int p_index) {
	if (p_index < 0) {
		p_index = occlusion_layers.size();
	}
	ERR_FAIL_INDEX(p_index, occlusion_layers.size() + 1);
	occlusion_layers.insert(p_index, OcclusionLayer());

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_occlusion_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::move_occlusion_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, occlusion_layers.size());
	ERR_FAIL_INDEX(p_to_pos, occlusion_layers.size() + 1);
	occlusion_layers.insert(p_to_pos, occlusion_layers[p_from_index]);
	occlusion_layers.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_occlusion_layer(p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::remove_occlusion_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, occlusion_layers.size());

	occlusion_layers.remove(p_index);

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_occlusion_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::set_occlusion_layer_light_mask(int p_layer_index, int p_light_mask) {
	ERR_FAIL_INDEX(p_layer_index, occlusion_layers.size());
	occlusion_layers.write[p_layer_index].light_mask = p_light_mask;
	emit_changed();
}

int LayeredTileSet::get_occlusion_layer_light_mask(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, occlusion_layers.size(), 0);
	return occlusion_layers[p_layer_index].light_mask;
}

void LayeredTileSet::set_occlusion_layer_sdf_collision(int p_layer_index, bool p_sdf_collision) {
	ERR_FAIL_INDEX(p_layer_index, occlusion_layers.size());
	occlusion_layers.write[p_layer_index].sdf_collision = p_sdf_collision;
	emit_changed();
}

bool LayeredTileSet::get_occlusion_layer_sdf_collision(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, occlusion_layers.size(), false);
	return occlusion_layers[p_layer_index].sdf_collision;
}

int LayeredTileSet::get_physics_layers_count() const {
	return physics_layers.size();
}

void LayeredTileSet::add_physics_layer(int p_index) {
	if (p_index < 0) {
		p_index = physics_layers.size();
	}
	ERR_FAIL_INDEX(p_index, physics_layers.size() + 1);
	physics_layers.insert(p_index, PhysicsLayer());

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_physics_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::move_physics_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, physics_layers.size());
	ERR_FAIL_INDEX(p_to_pos, physics_layers.size() + 1);
	physics_layers.insert(p_to_pos, physics_layers[p_from_index]);
	physics_layers.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_physics_layer(p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::remove_physics_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, physics_layers.size());
	physics_layers.remove(p_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_physics_layer(p_index);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::set_physics_layer_collision_layer(int p_layer_index, uint32_t p_layer) {
	ERR_FAIL_INDEX(p_layer_index, physics_layers.size());
	physics_layers.write[p_layer_index].collision_layer = p_layer;
	emit_changed();
}

uint32_t LayeredTileSet::get_physics_layer_collision_layer(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, physics_layers.size(), 0);
	return physics_layers[p_layer_index].collision_layer;
}

void LayeredTileSet::set_physics_layer_collision_mask(int p_layer_index, uint32_t p_mask) {
	ERR_FAIL_INDEX(p_layer_index, physics_layers.size());
	physics_layers.write[p_layer_index].collision_mask = p_mask;
	emit_changed();
}

uint32_t LayeredTileSet::get_physics_layer_collision_mask(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, physics_layers.size(), 0);
	return physics_layers[p_layer_index].collision_mask;
}

void LayeredTileSet::set_physics_layer_physics_material(int p_layer_index, Ref<PhysicsMaterial> p_physics_material) {
	ERR_FAIL_INDEX(p_layer_index, physics_layers.size());
	physics_layers.write[p_layer_index].physics_material = p_physics_material;
}

Ref<PhysicsMaterial> LayeredTileSet::get_physics_layer_physics_material(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, physics_layers.size(), Ref<PhysicsMaterial>());
	return physics_layers[p_layer_index].physics_material;
}

// Terrains
int LayeredTileSet::get_terrain_sets_count() const {
	return terrain_sets.size();
}

void LayeredTileSet::add_terrain_set(int p_index) {
	if (p_index < 0) {
		p_index = terrain_sets.size();
	}
	ERR_FAIL_INDEX(p_index, terrain_sets.size() + 1);
	terrain_sets.insert(p_index, TerrainSet());

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_terrain_set(p_index);
	}

	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::move_terrain_set(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, terrain_sets.size());
	ERR_FAIL_INDEX(p_to_pos, terrain_sets.size() + 1);
	terrain_sets.insert(p_to_pos, terrain_sets[p_from_index]);
	terrain_sets.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_terrain_set(p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::remove_terrain_set(int p_index) {
	ERR_FAIL_INDEX(p_index, terrain_sets.size());
	terrain_sets.remove(p_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_terrain_set(p_index);
	}
	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::set_terrain_set_mode(int p_terrain_set, TerrainMode p_terrain_mode) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	terrain_sets.write[p_terrain_set].mode = p_terrain_mode;
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *E_source = sources.front(); E_source; E_source = E_source->next) {
		E_source->value()->notify_tile_data_properties_should_change();
	}

	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

LayeredTileSet::TerrainMode LayeredTileSet::get_terrain_set_mode(int p_terrain_set) const {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES);
	return terrain_sets[p_terrain_set].mode;
}

int LayeredTileSet::get_terrains_count(int p_terrain_set) const {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), -1);
	return terrain_sets[p_terrain_set].terrains.size();
}

void LayeredTileSet::add_terrain(int p_terrain_set, int p_index) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	Vector<Terrain> &terrains = terrain_sets.write[p_terrain_set].terrains;
	if (p_index < 0) {
		p_index = terrains.size();
	}
	ERR_FAIL_INDEX(p_index, terrains.size() + 1);
	terrains.insert(p_index, Terrain());

	// Default name and color
	float hue_rotate = (terrains.size() % 16) / 16.0;
	Color c;
	c.set_hsv(Math::fmod(float(hue_rotate), float(1.0)), 0.5, 0.5);
	terrains.write[p_index].color = c;
	terrains.write[p_index].name = String(vformat("Terrain %d", p_index));

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_terrain(p_terrain_set, p_index);
	}

	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::move_terrain(int p_terrain_set, int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	Vector<Terrain> &terrains = terrain_sets.write[p_terrain_set].terrains;

	ERR_FAIL_INDEX(p_from_index, terrains.size());
	ERR_FAIL_INDEX(p_to_pos, terrains.size() + 1);
	terrains.insert(p_to_pos, terrains[p_from_index]);
	terrains.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_terrain(p_terrain_set, p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::remove_terrain(int p_terrain_set, int p_index) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	Vector<Terrain> &terrains = terrain_sets.write[p_terrain_set].terrains;

	ERR_FAIL_INDEX(p_index, terrains.size());
	terrains.remove(p_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_terrain(p_terrain_set, p_index);
	}
	property_list_changed_notify();
	terrains_cache_dirty = true;
	emit_changed();
}

void LayeredTileSet::set_terrain_name(int p_terrain_set, int p_terrain_index, String p_name) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	ERR_FAIL_INDEX(p_terrain_index, terrain_sets[p_terrain_set].terrains.size());
	terrain_sets.write[p_terrain_set].terrains.write[p_terrain_index].name = p_name;
	emit_changed();
}

String LayeredTileSet::get_terrain_name(int p_terrain_set, int p_terrain_index) const {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), String());
	ERR_FAIL_INDEX_V(p_terrain_index, terrain_sets[p_terrain_set].terrains.size(), String());
	return terrain_sets[p_terrain_set].terrains[p_terrain_index].name;
}

void LayeredTileSet::set_terrain_color(int p_terrain_set, int p_terrain_index, Color p_color) {
	ERR_FAIL_INDEX(p_terrain_set, terrain_sets.size());
	ERR_FAIL_INDEX(p_terrain_index, terrain_sets[p_terrain_set].terrains.size());
	if (p_color.a != 1.0) {
		WARN_PRINT("Terrain color should have alpha == 1.0");
		p_color.a = 1.0;
	}
	terrain_sets.write[p_terrain_set].terrains.write[p_terrain_index].color = p_color;
	emit_changed();
}

Color LayeredTileSet::get_terrain_color(int p_terrain_set, int p_terrain_index) const {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), Color());
	ERR_FAIL_INDEX_V(p_terrain_index, terrain_sets[p_terrain_set].terrains.size(), Color());
	return terrain_sets[p_terrain_set].terrains[p_terrain_index].color;
}

bool LayeredTileSet::is_valid_terrain_peering_bit_for_mode(LayeredTileSet::TerrainMode p_terrain_mode, LayeredTileSet::CellNeighbor p_peering_bit) const {
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_SIDES) {
			if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE) {
				return true;
			}
		}
		if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
			if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER) {
				return true;
			}
		}
	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_SIDES) {
			if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
				return true;
			}
		}
		if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
			if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER ||
					p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
				return true;
			}
		}
	} else {
		if (get_tile_offset_axis() == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
			if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_SIDES) {
				if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
					return true;
				}
			}
			if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
				if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER) {
					return true;
				}
			}
		} else {
			if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_SIDES) {
				if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
					return true;
				}
			}
			if (p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES || p_terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
				if (p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER ||
						p_peering_bit == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER) {
					return true;
				}
			}
		}
	}
	return false;
}

bool LayeredTileSet::is_valid_terrain_peering_bit(int p_terrain_set, LayeredTileSet::CellNeighbor p_peering_bit) const {
	if (p_terrain_set < 0 || p_terrain_set >= get_terrain_sets_count()) {
		return false;
	}

	LayeredTileSet::TerrainMode terrain_mode = get_terrain_set_mode(p_terrain_set);
	return is_valid_terrain_peering_bit_for_mode(terrain_mode, p_peering_bit);
}

// Navigation
int LayeredTileSet::get_navigation_layers_count() const {
	return navigation_layers.size();
}

void LayeredTileSet::add_navigation_layer(int p_index) {
	if (p_index < 0) {
		p_index = navigation_layers.size();
	}
	ERR_FAIL_INDEX(p_index, navigation_layers.size() + 1);
	navigation_layers.insert(p_index, NavigationLayer());

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_navigation_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::move_navigation_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, navigation_layers.size());
	ERR_FAIL_INDEX(p_to_pos, navigation_layers.size() + 1);
	navigation_layers.insert(p_to_pos, navigation_layers[p_from_index]);
	navigation_layers.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_navigation_layer(p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::remove_navigation_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, navigation_layers.size());
	navigation_layers.remove(p_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_navigation_layer(p_index);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::set_navigation_layer_layers(int p_layer_index, uint32_t p_layers) {
	ERR_FAIL_INDEX(p_layer_index, navigation_layers.size());
	navigation_layers.write[p_layer_index].layers = p_layers;
	emit_changed();
}

uint32_t LayeredTileSet::get_navigation_layer_layers(int p_layer_index) const {
	ERR_FAIL_INDEX_V(p_layer_index, navigation_layers.size(), 0);
	return navigation_layers[p_layer_index].layers;
}

void LayeredTileSet::set_navigation_layer_layer_value(int p_layer_index, int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Navigation layer number must be between 1 and 32 inclusive.");

	uint32_t _navigation_layers = get_navigation_layer_layers(p_layer_index);

	if (p_value) {
		_navigation_layers |= 1 << (p_layer_number - 1);
	} else {
		_navigation_layers &= ~(1 << (p_layer_number - 1));
	}

	set_navigation_layer_layers(p_layer_index, _navigation_layers);
}

bool LayeredTileSet::get_navigation_layer_layer_value(int p_layer_index, int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");

	return get_navigation_layer_layers(p_layer_index) & (1 << (p_layer_number - 1));
}

// Custom data.
int LayeredTileSet::get_custom_data_layers_count() const {
	return custom_data_layers.size();
}

void LayeredTileSet::add_custom_data_layer(int p_index) {
	if (p_index < 0) {
		p_index = custom_data_layers.size();
	}
	ERR_FAIL_INDEX(p_index, custom_data_layers.size() + 1);
	custom_data_layers.insert(p_index, CustomDataLayer());

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->add_custom_data_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::move_custom_data_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, custom_data_layers.size());
	ERR_FAIL_INDEX(p_to_pos, custom_data_layers.size() + 1);
	custom_data_layers.insert(p_to_pos, custom_data_layers[p_from_index]);
	custom_data_layers.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->move_custom_data_layer(p_from_index, p_to_pos);
	}
	property_list_changed_notify();
	emit_changed();
}

void LayeredTileSet::remove_custom_data_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, custom_data_layers.size());
	custom_data_layers.remove(p_index);

	String to_erase;

	for (HashMap<String, int>::Element *E = custom_data_layers_by_name.front(); E; E = E->next) {
		if (E->value() == p_index) {
			to_erase = E->key();
		} else if (E->value() > p_index) {
			E->value()--;
		}
	}
	custom_data_layers_by_name.erase(to_erase);

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *source = sources.front(); source; source = source->next) {
		source->value()->remove_custom_data_layer(p_index);
	}

	property_list_changed_notify();
	emit_changed();
}

int LayeredTileSet::get_custom_data_layer_by_name(String p_value) const {
	if (custom_data_layers_by_name.has(p_value)) {
		return custom_data_layers_by_name[p_value];
	} else {
		return -1;
	}
}

void LayeredTileSet::set_custom_data_layer_name(int p_layer_id, String p_value) {
	ERR_FAIL_INDEX(p_layer_id, custom_data_layers.size());

	// Exit if another property has the same name.
	if (!p_value.empty()) {
		for (int other_layer_id = 0; other_layer_id < get_custom_data_layers_count(); other_layer_id++) {
			if (other_layer_id != p_layer_id && get_custom_data_layer_name(other_layer_id) == p_value) {
				ERR_FAIL_MSG(vformat("There is already a custom property named %s", p_value));
			}
		}
	}

	if (p_value.empty() && custom_data_layers_by_name.has(p_value)) {
		custom_data_layers_by_name.erase(p_value);
	} else {
		custom_data_layers_by_name[p_value] = p_layer_id;
	}

	custom_data_layers.write[p_layer_id].name = p_value;
	emit_changed();
}

String LayeredTileSet::get_custom_data_layer_name(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, custom_data_layers.size(), "");
	return custom_data_layers[p_layer_id].name;
}

void LayeredTileSet::set_custom_data_layer_type(int p_layer_id, Variant::Type p_value) {
	ERR_FAIL_INDEX(p_layer_id, custom_data_layers.size());
	custom_data_layers.write[p_layer_id].type = p_value;

	for (HashMap<int, Ref<LayeredTileSetSource>>::Element *E_source = sources.front(); E_source; E_source = E_source->next) {
		E_source->value()->notify_tile_data_properties_should_change();
	}

	emit_changed();
}

Variant::Type LayeredTileSet::get_custom_data_layer_type(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, custom_data_layers.size(), Variant::NIL);
	return custom_data_layers[p_layer_id].type;
}

void LayeredTileSet::set_source_level_tile_proxy(int p_source_from, int p_source_to) {
	ERR_FAIL_COND(p_source_from == LayeredTileSet::INVALID_SOURCE || p_source_to == LayeredTileSet::INVALID_SOURCE);

	source_level_proxies[p_source_from] = p_source_to;

	emit_changed();
}

int LayeredTileSet::get_source_level_tile_proxy(int p_source_from) {
	ERR_FAIL_COND_V(!source_level_proxies.has(p_source_from), LayeredTileSet::INVALID_SOURCE);

	return source_level_proxies[p_source_from];
}

bool LayeredTileSet::has_source_level_tile_proxy(int p_source_from) {
	return source_level_proxies.has(p_source_from);
}

void LayeredTileSet::remove_source_level_tile_proxy(int p_source_from) {
	ERR_FAIL_COND(!source_level_proxies.has(p_source_from));

	source_level_proxies.erase(p_source_from);

	emit_changed();
}

void LayeredTileSet::set_coords_level_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_source_to, Vector2i p_coords_to) {
	ERR_FAIL_COND(p_source_from == LayeredTileSet::INVALID_SOURCE || p_source_to == LayeredTileSet::INVALID_SOURCE);
	ERR_FAIL_COND(p_coords_from == LayeredTileSetSource::INVALID_ATLAS_COORDS || p_coords_to == LayeredTileSetSource::INVALID_ATLAS_COORDS);

	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);

	Array to;
	to.push_back(p_source_to);
	to.push_back(p_coords_to);

	coords_level_proxies[from] = to;

	emit_changed();
}

Array LayeredTileSet::get_coords_level_tile_proxy(int p_source_from, Vector2i p_coords_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);

	ERR_FAIL_COND_V(!coords_level_proxies.has(from), Array());

	return coords_level_proxies[from];
}

bool LayeredTileSet::has_coords_level_tile_proxy(int p_source_from, Vector2i p_coords_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);

	return coords_level_proxies.has(from);
}

void LayeredTileSet::remove_coords_level_tile_proxy(int p_source_from, Vector2i p_coords_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);

	ERR_FAIL_COND(!coords_level_proxies.has(from));

	coords_level_proxies.erase(from);

	emit_changed();
}

void LayeredTileSet::set_alternative_level_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_alternative_from, int p_source_to, Vector2i p_coords_to, int p_alternative_to) {
	ERR_FAIL_COND(p_source_from == LayeredTileSet::INVALID_SOURCE || p_source_to == LayeredTileSet::INVALID_SOURCE);
	ERR_FAIL_COND(p_coords_from == LayeredTileSetSource::INVALID_ATLAS_COORDS || p_coords_to == LayeredTileSetSource::INVALID_ATLAS_COORDS);

	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);
	from.push_back(p_alternative_from);

	Array to;
	to.push_back(p_source_to);
	to.push_back(p_coords_to);
	to.push_back(p_alternative_to);

	alternative_level_proxies[from] = to;

	emit_changed();
}

Array LayeredTileSet::get_alternative_level_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_alternative_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);
	from.push_back(p_alternative_from);

	ERR_FAIL_COND_V(!alternative_level_proxies.has(from), Array());

	return alternative_level_proxies[from];
}

bool LayeredTileSet::has_alternative_level_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_alternative_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);
	from.push_back(p_alternative_from);

	return alternative_level_proxies.has(from);
}

void LayeredTileSet::remove_alternative_level_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_alternative_from) {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);
	from.push_back(p_alternative_from);

	ERR_FAIL_COND(!alternative_level_proxies.has(from));

	alternative_level_proxies.erase(from);

	emit_changed();
}

Array LayeredTileSet::get_source_level_tile_proxies() const {
	Array output;

	for (const RBMap<int, int>::Element *E = source_level_proxies.front(); E; E = E->next()) {
		Array proxy;
		proxy.push_back(E->key());
		proxy.push_back(E->value());
		output.push_back(proxy);
	}

	return output;
}

Array LayeredTileSet::get_coords_level_tile_proxies() const {
	Array output;

	for (const RBMap<Array, Array>::Element *E = coords_level_proxies.front(); E; E = E->next()) {
		Array proxy;
		proxy.append_array(E->key());
		proxy.append_array(E->value());
		output.push_back(proxy);
	}

	return output;
}

Array LayeredTileSet::get_alternative_level_tile_proxies() const {
	Array output;

	for (const RBMap<Array, Array>::Element *E = alternative_level_proxies.front(); E; E = E->next()) {
		Array proxy;
		proxy.append_array(E->key());
		proxy.append_array(E->value());
		output.push_back(proxy);
	}

	return output;
}

Array LayeredTileSet::map_tile_proxy(int p_source_from, Vector2i p_coords_from, int p_alternative_from) const {
	Array from;
	from.push_back(p_source_from);
	from.push_back(p_coords_from);
	from.push_back(p_alternative_from);

	// Check if the tile is valid, and if so, don't map the tile and return the input.
	if (has_source(p_source_from)) {
		Ref<LayeredTileSetSource> source = get_source(p_source_from);
		if (source->has_tile(p_coords_from) && source->has_alternative_tile(p_coords_from, p_alternative_from)) {
			return from;
		}
	}

	// Source, coords and alternative match.
	if (alternative_level_proxies.has(from)) {
		return alternative_level_proxies[from].duplicate();
	}

	// Source and coords match.
	from.pop_back();
	if (coords_level_proxies.has(from)) {
		Array output = coords_level_proxies[from].duplicate();
		output.push_back(p_alternative_from);
		return output;
	}

	// Source matches.
	if (source_level_proxies.has(p_source_from)) {
		Array output;
		output.push_back(source_level_proxies[p_source_from]);
		output.push_back(p_coords_from);
		output.push_back(p_alternative_from);
		return output;
	}

	Array output;
	output.push_back(p_source_from);
	output.push_back(p_coords_from);
	output.push_back(p_alternative_from);
	return output;
}

void LayeredTileSet::cleanup_invalid_tile_proxies() {
	// Source level.
	Vector<int> source_to_remove;
	for (const RBMap<int, int>::Element *E = source_level_proxies.front(); E; E = E->next()) {
		if (has_source(E->key())) {
			source_to_remove.push_back(E->key());
		}
	}
	for (int i = 0; i < source_to_remove.size(); i++) {
		remove_source_level_tile_proxy(source_to_remove[i]);
	}

	// Coords level.
	Vector<Array> coords_to_remove;
	for (const RBMap<Array, Array>::Element *E = coords_level_proxies.front(); E; E = E->next()) {
		Array a = E->key();
		if (has_source(a[0]) && get_source(a[0])->has_tile(a[1])) {
			coords_to_remove.push_back(a);
		}
	}
	for (int i = 0; i < coords_to_remove.size(); i++) {
		Array a = coords_to_remove[i];
		remove_coords_level_tile_proxy(a[0], a[1]);
	}

	// Alternative level.
	Vector<Array> alternative_to_remove;
	for (const RBMap<Array, Array>::Element *E = alternative_level_proxies.front(); E; E = E->next()) {
		Array a = E->key();
		if (has_source(a[0]) && get_source(a[0])->has_tile(a[1]) && get_source(a[0])->has_alternative_tile(a[1], a[2])) {
			alternative_to_remove.push_back(a);
		}
	}
	for (int i = 0; i < alternative_to_remove.size(); i++) {
		Array a = alternative_to_remove[i];
		remove_alternative_level_tile_proxy(a[0], a[1], a[2]);
	}
}

void LayeredTileSet::clear_tile_proxies() {
	source_level_proxies.clear();
	coords_level_proxies.clear();
	alternative_level_proxies.clear();

	emit_changed();
}

int LayeredTileSet::add_pattern(Ref<LayeredTileMapPattern> p_pattern, int p_index) {
	ERR_FAIL_COND_V(!p_pattern.is_valid(), -1);
	ERR_FAIL_COND_V_MSG(p_pattern->empty(), -1, "Cannot add an empty pattern to the LayeredTileSet.");

	for (uint32_t i = 0; i < patterns.size(); ++i) {
		const Ref<LayeredTileMapPattern> &pattern = patterns[i];
		ERR_FAIL_COND_V_MSG(pattern == p_pattern, -1, "LayeredTileSet has already this pattern.");
	}

	ERR_FAIL_COND_V(p_index > (int)patterns.size(), -1);
	if (p_index < 0) {
		p_index = patterns.size();
	}
	patterns.insert(p_index, p_pattern);
	emit_changed();
	return p_index;
}

Ref<LayeredTileMapPattern> LayeredTileSet::get_pattern(int p_index) {
	ERR_FAIL_INDEX_V(p_index, (int)patterns.size(), Ref<LayeredTileMapPattern>());
	return patterns[p_index];
}

void LayeredTileSet::remove_pattern(int p_index) {
	ERR_FAIL_INDEX(p_index, (int)patterns.size());
	patterns.remove(p_index);
	emit_changed();
}

int LayeredTileSet::get_patterns_count() {
	return patterns.size();
}

RBSet<LayeredTileSet::TerrainsPattern> LayeredTileSet::get_terrains_pattern_set(int p_terrain_set) {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), RBSet<LayeredTileSet::TerrainsPattern>());

	_update_terrains_cache();

	RBSet<LayeredTileSet::TerrainsPattern> output;

	const RBMap<LayeredTileSet::TerrainsPattern, RBSet<LayeredTileMapCell>> &m = per_terrain_pattern_tiles[p_terrain_set];

	for (const RBMap<LayeredTileSet::TerrainsPattern, RBSet<LayeredTileMapCell>>::Element *kv = m.front(); kv; kv = kv->next()) {
		output.insert(kv->key());
	}

	return output;
}

RBSet<LayeredTileMapCell> LayeredTileSet::get_tiles_for_terrains_pattern(int p_terrain_set, TerrainsPattern p_terrain_tile_pattern) {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), RBSet<LayeredTileMapCell>());

	_update_terrains_cache();

	return per_terrain_pattern_tiles[p_terrain_set][p_terrain_tile_pattern];
}

LayeredTileMapCell LayeredTileSet::get_random_tile_from_terrains_pattern(int p_terrain_set, LayeredTileSet::TerrainsPattern p_terrain_tile_pattern) {
	ERR_FAIL_INDEX_V(p_terrain_set, terrain_sets.size(), LayeredTileMapCell());
	_update_terrains_cache();

	// Count the sum of probabilities.
	double sum = 0.0;
	RBSet<LayeredTileMapCell> set = per_terrain_pattern_tiles[p_terrain_set][p_terrain_tile_pattern];

	for (const RBSet<LayeredTileMapCell>::Element *E = set.front(); E; E = E->next()) {
		if (E->get().source_id >= 0) {
			Ref<LayeredTileSetSource> source = sources[E->get().source_id];
			Ref<LayeredTileSetAtlasSource> atlas_source = source;
			if (atlas_source.is_valid()) {
				LayeredTileData *tile_data = atlas_source->get_tile_data(E->get().get_atlas_coords(), E->get().alternative_tile);
				sum += tile_data->get_probability();
			} else {
				sum += 1.0;
			}
		} else {
			sum += 1.0;
		}
	}

	// Generate a random number.
	double count = 0.0;
	double picked = Math::random(0.0, sum);

	// Pick the tile.
	for (const RBSet<LayeredTileMapCell>::Element *E = set.front(); E; E = E->next()) {
		if (E->get().source_id >= 0) {
			Ref<LayeredTileSetSource> source = sources[E->get().source_id];

			Ref<LayeredTileSetAtlasSource> atlas_source = source;
			if (atlas_source.is_valid()) {
				LayeredTileData *tile_data = atlas_source->get_tile_data(E->get().get_atlas_coords(), E->get().alternative_tile);
				count += tile_data->get_probability();
			} else {
				count += 1.0;
			}
		} else {
			count += 1.0;
		}

		if (count >= picked) {
			return E->get();
		}
	}

	ERR_FAIL_V(LayeredTileMapCell());
}

Vector<Vector2> LayeredTileSet::get_tile_shape_polygon() const {
	Vector<Vector2> points;
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		points.push_back(Vector2(-0.5, -0.5));
		points.push_back(Vector2(0.5, -0.5));
		points.push_back(Vector2(0.5, 0.5));
		points.push_back(Vector2(-0.5, 0.5));
	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		points.push_back(Vector2(0.0, -0.5));
		points.push_back(Vector2(-0.5, 0.0));
		points.push_back(Vector2(0.0, 0.5));
		points.push_back(Vector2(0.5, 0.0));
	} else {
		float overlap = 0.0;
		switch (tile_shape) {
			case LayeredTileSet::TILE_SHAPE_HEXAGON:
				overlap = 0.25;
				break;
			case LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE:
				overlap = 0.0;
				break;
			default:
				break;
		}

		points.push_back(Vector2(0.0, -0.5));
		points.push_back(Vector2(-0.5, overlap - 0.5));
		points.push_back(Vector2(-0.5, 0.5 - overlap));
		points.push_back(Vector2(0.0, 0.5));
		points.push_back(Vector2(0.5, 0.5 - overlap));
		points.push_back(Vector2(0.5, overlap - 0.5));

		if (get_tile_offset_axis() == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL) {
			for (int i = 0; i < points.size(); i++) {
				points.write[i] = Vector2(points[i].y, points[i].x);
			}
		}
	}
	return points;
}

void LayeredTileSet::draw_tile_shape(CanvasItem *p_canvas_item, Transform2D p_transform, Color p_color, bool p_filled, Ref<Texture> p_texture) {
	if (tile_meshes_dirty) {
		Vector<Vector2> shape = get_tile_shape_polygon();
		Vector<Vector2> uvs;
		uvs.resize(shape.size());
		for (int i = 0; i < shape.size(); i++) {
			uvs.write[i] = shape[i] + Vector2(0.5, 0.5);
		}

		Vector<Color> colors;
		colors.resize(shape.size());
		colors.fill(Color(1.0, 1.0, 1.0, 1.0));

		// Filled mesh.
		tile_filled_mesh->clear_surfaces();

		Array a;
		a.resize(Mesh::ARRAY_MAX);
		a[Mesh::ARRAY_VERTEX] = shape;
		a[Mesh::ARRAY_TEX_UV] = uvs;
		a[Mesh::ARRAY_COLOR] = colors;
		a[Mesh::ARRAY_INDEX] = Geometry::triangulate_polygon(shape);
		tile_filled_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, a, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);

		// Lines mesh.
		tile_lines_mesh->clear_surfaces();
		a.clear();
		a.resize(Mesh::ARRAY_MAX);
		// Add the first point again when drawing lines.
		shape.push_back(shape[0]);
		colors.push_back(colors[0]);
		a[Mesh::ARRAY_VERTEX] = shape;
		a[Mesh::ARRAY_COLOR] = colors;
		tile_lines_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINE_STRIP, a, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);

		tile_meshes_dirty = false;
	}

	if (p_filled) {
		p_canvas_item->draw_mesh(tile_filled_mesh, p_texture, Ref<Texture>(), p_transform, p_color);
	} else {
		p_canvas_item->draw_mesh(tile_lines_mesh, Ref<Texture>(), Ref<Texture>(), p_transform, p_color);
	}
}

Vector2 LayeredTileSet::map_to_local(const Vector2i &p_pos) const {
	// SHOULD RETURN THE CENTER OF THE CELL.
	Vector2 ret = p_pos;

	if (tile_shape == LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE || tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON || tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		// Technically, those 3 shapes are equivalent, as they are basically half-offset, but with different levels or overlap.
		// square = no overlap, hexagon = 0.25 overlap, isometric = 0.5 overlap.
		if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
			switch (tile_layout) {
				case LayeredTileSet::TILE_LAYOUT_STACKED:
					ret = Vector2(ret.x + (Math::posmod(ret.y, 2) == 0 ? 0.0 : 0.5), ret.y);
					break;
				case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
					ret = Vector2(ret.x + (Math::posmod(ret.y, 2) == 1 ? 0.0 : 0.5), ret.y);
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
					ret = Vector2(ret.x + ret.y / 2, ret.y);
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
					ret = Vector2(ret.x / 2, ret.y * 2 + ret.x);
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
					ret = Vector2((ret.x + ret.y) / 2, ret.y - ret.x);
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
					ret = Vector2((ret.x - ret.y) / 2, ret.y + ret.x);
					break;
			}
		} else { // TILE_OFFSET_AXIS_VERTICAL.
			switch (tile_layout) {
				case LayeredTileSet::TILE_LAYOUT_STACKED:
					ret = Vector2(ret.x, ret.y + (Math::posmod(ret.x, 2) == 0 ? 0.0 : 0.5));
					break;
				case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
					ret = Vector2(ret.x, ret.y + (Math::posmod(ret.x, 2) == 1 ? 0.0 : 0.5));
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
					ret = Vector2(ret.x * 2 + ret.y, ret.y / 2);
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
					ret = Vector2(ret.x, ret.y + ret.x / 2);
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
					ret = Vector2(ret.x + ret.y, (ret.y - ret.x) / 2);
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
					ret = Vector2(ret.x - ret.y, (ret.y + ret.x) / 2);
					break;
			}
		}
	}

	// Multiply by the overlapping ratio.
	double overlapping_ratio = 1.0;
	if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
			overlapping_ratio = 0.5;
		} else if (tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON) {
			overlapping_ratio = 0.75;
		}
		ret.y *= overlapping_ratio;
	} else { // TILE_OFFSET_AXIS_VERTICAL.
		if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
			overlapping_ratio = 0.5;
		} else if (tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON) {
			overlapping_ratio = 0.75;
		}
		ret.x *= overlapping_ratio;
	}

	return (ret + Vector2(0.5, 0.5)) * tile_size;
}

Vector2i LayeredTileSet::local_to_map(const Vector2 &p_local_position) const {
	Vector2 ret = p_local_position;
	ret /= tile_size;

	// Divide by the overlapping ratio.
	double overlapping_ratio = 1.0;
	if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
			overlapping_ratio = 0.5;
		} else if (tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON) {
			overlapping_ratio = 0.75;
		}
		ret.y /= overlapping_ratio;
	} else { // TILE_OFFSET_AXIS_VERTICAL.
		if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
			overlapping_ratio = 0.5;
		} else if (tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON) {
			overlapping_ratio = 0.75;
		}
		ret.x /= overlapping_ratio;
	}

	// For each half-offset shape, we check if we are in the corner of the tile, and thus should correct the local position accordingly.
	if (tile_shape == LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE || tile_shape == LayeredTileSet::TILE_SHAPE_HEXAGON || tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		// Technically, those 3 shapes are equivalent, as they are basically half-offset, but with different levels or overlap.
		// square = no overlap, hexagon = 0.25 overlap, isometric = 0.5 overlap.
		if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
			// Smart floor of the position
			Vector2 raw_pos = ret;
			if (Math::posmod(Math::floor(ret.y), 2) ^ (tile_layout == LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET)) {
				ret = Vector2(Math::floor(ret.x + 0.5) - 0.5, Math::floor(ret.y));
			} else {
				ret = ret.floor();
			}

			// Compute the tile offset, and if we might the output for a neighbor top tile.
			Vector2 in_tile_pos = raw_pos - ret;
			bool in_top_left_triangle = (in_tile_pos - Vector2(0.5, 0.0)).cross(Vector2(-0.5, 1.0 / overlapping_ratio - 1)) <= 0;
			bool in_top_right_triangle = (in_tile_pos - Vector2(0.5, 0.0)).cross(Vector2(0.5, 1.0 / overlapping_ratio - 1)) > 0;

			switch (tile_layout) {
				case LayeredTileSet::TILE_LAYOUT_STACKED:
					ret = ret.floor();
					if (in_top_left_triangle) {
						ret += Vector2i(Math::posmod(Math::floor(ret.y), 2) ? 0 : -1, -1);
					} else if (in_top_right_triangle) {
						ret += Vector2i(Math::posmod(Math::floor(ret.y), 2) ? 1 : 0, -1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
					ret = ret.floor();
					if (in_top_left_triangle) {
						ret += Vector2i(Math::posmod(Math::floor(ret.y), 2) ? -1 : 0, -1);
					} else if (in_top_right_triangle) {
						ret += Vector2i(Math::posmod(Math::floor(ret.y), 2) ? 0 : 1, -1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
					ret = Vector2(ret.x - ret.y / 2, ret.y).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(0, -1);
					} else if (in_top_right_triangle) {
						ret += Vector2i(1, -1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
					ret = Vector2(ret.x * 2, ret.y / 2 - ret.x).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, 0);
					} else if (in_top_right_triangle) {
						ret += Vector2i(1, -1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
					ret = Vector2(ret.x - ret.y / 2, ret.y / 2 + ret.x).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(0, -1);
					} else if (in_top_right_triangle) {
						ret += Vector2i(1, 0);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
					ret = Vector2(ret.x + ret.y / 2, ret.y / 2 - ret.x).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, 0);
					} else if (in_top_right_triangle) {
						ret += Vector2i(0, -1);
					}
					break;
			}
		} else { // TILE_OFFSET_AXIS_VERTICAL.
			// Smart floor of the position.
			Vector2 raw_pos = ret;
			if (Math::posmod(Math::floor(ret.x), 2) ^ (tile_layout == LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET)) {
				ret = Vector2(Math::floor(ret.x), Math::floor(ret.y + 0.5) - 0.5);
			} else {
				ret = ret.floor();
			}

			// Compute the tile offset, and if we might the output for a neighbor top tile.
			Vector2 in_tile_pos = raw_pos - ret;
			bool in_top_left_triangle = (in_tile_pos - Vector2(0.0, 0.5)).cross(Vector2(1.0 / overlapping_ratio - 1, -0.5)) > 0;
			bool in_bottom_left_triangle = (in_tile_pos - Vector2(0.0, 0.5)).cross(Vector2(1.0 / overlapping_ratio - 1, 0.5)) <= 0;

			switch (tile_layout) {
				case LayeredTileSet::TILE_LAYOUT_STACKED:
					ret = ret.floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, Math::posmod(Math::floor(ret.x), 2) ? 0 : -1);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(-1, Math::posmod(Math::floor(ret.x), 2) ? 1 : 0);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
					ret = ret.floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, Math::posmod(Math::floor(ret.x), 2) ? -1 : 0);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(-1, Math::posmod(Math::floor(ret.x), 2) ? 0 : 1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
					ret = Vector2(ret.x / 2 - ret.y, ret.y * 2).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(0, -1);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(-1, 1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
					ret = Vector2(ret.x, ret.y - ret.x / 2).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, 0);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(-1, 1);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
					ret = Vector2(ret.x / 2 - ret.y, ret.y + ret.x / 2).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(0, -1);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(-1, 0);
					}
					break;
				case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
					ret = Vector2(ret.x / 2 + ret.y, ret.y - ret.x / 2).floor();
					if (in_top_left_triangle) {
						ret += Vector2i(-1, 0);
					} else if (in_bottom_left_triangle) {
						ret += Vector2i(0, 1);
					}
					break;
			}
		}
	} else {
		ret = (ret + Vector2(0.00005, 0.00005)).floor();
	}
	return Vector2i(ret);
}

bool LayeredTileSet::is_existing_neighbor(LayeredTileSet::CellNeighbor p_cell_neighbor) const {
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		return p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER;

	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		return p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER ||
				p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE;
	} else {
		if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
			return p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE;
		} else {
			return p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE ||
					p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE;
		}
	}
}

Vector2i LayeredTileSet::get_neighbor_cell(const Vector2i &p_coords, LayeredTileSet::CellNeighbor p_cell_neighbor) const {
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		switch (p_cell_neighbor) {
			case LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE:
				return p_coords + Vector2i(1, 0);
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
				return p_coords + Vector2i(1, 1);
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
				return p_coords + Vector2i(0, 1);
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
				return p_coords + Vector2i(-1, 1);
			case LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE:
				return p_coords + Vector2i(-1, 0);
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
				return p_coords + Vector2i(-1, -1);
			case LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE:
				return p_coords + Vector2i(0, -1);
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
				return p_coords + Vector2i(1, -1);
			default:
				ERR_FAIL_V(p_coords);
		}
	} else { // Half-offset shapes (square and hexagon).
		switch (tile_layout) {
			case LayeredTileSet::TILE_LAYOUT_STACKED: {
				if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
					bool is_offset = p_coords.y % 2;
					if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
						return p_coords + Vector2i(1, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
						return p_coords + Vector2i(is_offset ? 1 : 0, 1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
						return p_coords + Vector2i(0, 2);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
						return p_coords + Vector2i(is_offset ? 0 : -1, 1);
					} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
						return p_coords + Vector2i(-1, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
						return p_coords + Vector2i(is_offset ? 0 : -1, -1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
						return p_coords + Vector2i(0, -2);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
						return p_coords + Vector2i(is_offset ? 1 : 0, -1);
					} else {
						ERR_FAIL_V(p_coords);
					}
				} else {
					bool is_offset = p_coords.x % 2;

					if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
						return p_coords + Vector2i(0, 1);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
						return p_coords + Vector2i(1, is_offset ? 1 : 0);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
						return p_coords + Vector2i(2, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
						return p_coords + Vector2i(1, is_offset ? 0 : -1);
					} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
						return p_coords + Vector2i(0, -1);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
						return p_coords + Vector2i(-1, is_offset ? 0 : -1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
						return p_coords + Vector2i(-2, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
						return p_coords + Vector2i(-1, is_offset ? 1 : 0);
					} else {
						ERR_FAIL_V(p_coords);
					}
				}
			} break;
			case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET: {
				if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
					bool is_offset = p_coords.y % 2;

					if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
						return p_coords + Vector2i(1, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
						return p_coords + Vector2i(is_offset ? 0 : 1, 1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
						return p_coords + Vector2i(0, 2);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
						return p_coords + Vector2i(is_offset ? -1 : 0, 1);
					} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
						return p_coords + Vector2i(-1, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
						return p_coords + Vector2i(is_offset ? -1 : 0, -1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
						return p_coords + Vector2i(0, -2);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
						return p_coords + Vector2i(is_offset ? 0 : 1, -1);
					} else {
						ERR_FAIL_V(p_coords);
					}
				} else {
					bool is_offset = p_coords.x % 2;

					if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
						return p_coords + Vector2i(0, 1);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
						return p_coords + Vector2i(1, is_offset ? 0 : 1);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
						return p_coords + Vector2i(2, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
						return p_coords + Vector2i(1, is_offset ? -1 : 0);
					} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
							(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
						return p_coords + Vector2i(0, -1);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
						return p_coords + Vector2i(-1, is_offset ? -1 : 0);
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
						return p_coords + Vector2i(-2, 0);
					} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
						return p_coords + Vector2i(-1, is_offset ? 0 : 1);
					} else {
						ERR_FAIL_V(p_coords);
					}
				}
			} break;
			case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
			case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN: {
				if ((tile_layout == LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT) ^ (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
					if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
							return p_coords + Vector2i(1, 0);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
							return p_coords + Vector2i(-1, 2);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
							return p_coords + Vector2i(-1, 0);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
							return p_coords + Vector2i(1, -2);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, -1);
						} else {
							ERR_FAIL_V(p_coords);
						}

					} else {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
							return p_coords + Vector2i(0, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
							return p_coords + Vector2i(2, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, -1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
							return p_coords + Vector2i(0, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
							return p_coords + Vector2i(-2, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 1);
						} else {
							ERR_FAIL_V(p_coords);
						}
					}
				} else {
					if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
							return p_coords + Vector2i(2, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
							return p_coords + Vector2i(0, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
							return p_coords + Vector2i(-2, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
							return p_coords + Vector2i(0, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, -1);
						} else {
							ERR_FAIL_V(p_coords);
						}

					} else {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
							return p_coords + Vector2i(-1, 2);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
							return p_coords + Vector2i(1, 0);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, -1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
							return p_coords + Vector2i(1, -2);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
							return p_coords + Vector2i(-1, 0);

						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 1);
						} else {
							ERR_FAIL_V(p_coords);
						}
					}
				}
			} break;
			case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
			case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN: {
				if ((tile_layout == LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT) ^ (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
					if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
							return p_coords + Vector2i(1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
							return p_coords + Vector2i(-1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
							return p_coords + Vector2i(-1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
							return p_coords + Vector2i(1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else {
							ERR_FAIL_V(p_coords);
						}

					} else {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
							return p_coords + Vector2i(1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
							return p_coords + Vector2i(1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
							return p_coords + Vector2i(-1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
							return p_coords + Vector2i(-1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else {
							ERR_FAIL_V(p_coords);
						}
					}
				} else {
					if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE)) {
							return p_coords + Vector2i(1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) {
							return p_coords + Vector2i(1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE)) {
							return p_coords + Vector2i(-1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) {
							return p_coords + Vector2i(-1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else {
							ERR_FAIL_V(p_coords);
						}

					} else {
						if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE)) {
							return p_coords + Vector2i(-1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE) {
							return p_coords + Vector2i(0, 1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER) {
							return p_coords + Vector2i(1, 1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE) {
							return p_coords + Vector2i(1, 0);
						} else if ((tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER) ||
								(tile_shape != LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE)) {
							return p_coords + Vector2i(1, -1);
						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE) {
							return p_coords + Vector2i(0, -1);
						} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC && p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER) {
							return p_coords + Vector2i(-1, -1);

						} else if (p_cell_neighbor == LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE) {
							return p_coords + Vector2i(-1, 0);
						} else {
							ERR_FAIL_V(p_coords);
						}
					}
				}
			} break;
			default:
				ERR_FAIL_V(p_coords);
		}
	}
}

PoolVector2iArray LayeredTileSet::get_surrounding_cells(const Vector2i &p_coords) const {
	PoolVector2iArray around;
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE));
	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE));
		around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
	} else {
		if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
		} else {
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE));
			around.push_back(get_neighbor_cell(p_coords, LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
		}
	}

	return around;
}

Vector2i LayeredTileSet::map_pattern(const Vector2i &p_position_in_tilemap, const Vector2i &p_coords_in_pattern, Ref<LayeredTileMapPattern> p_pattern) const {
	ERR_FAIL_COND_V(p_pattern.is_null(), Vector2i());
	ERR_FAIL_COND_V(!p_pattern->has_cell(p_coords_in_pattern), Vector2i());

	Vector2i output = p_position_in_tilemap + p_coords_in_pattern;
	if (tile_shape != LayeredTileSet::TILE_SHAPE_SQUARE) {
		if (tile_layout == LayeredTileSet::TILE_LAYOUT_STACKED) {
			if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL && bool(p_position_in_tilemap.y % 2) && bool(p_coords_in_pattern.y % 2)) {
				output.x += 1;
			} else if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL && bool(p_position_in_tilemap.x % 2) && bool(p_coords_in_pattern.x % 2)) {
				output.y += 1;
			}
		} else if (tile_layout == LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET) {
			if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL && bool(p_position_in_tilemap.y % 2) && bool(p_coords_in_pattern.y % 2)) {
				output.x -= 1;
			} else if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL && bool(p_position_in_tilemap.x % 2) && bool(p_coords_in_pattern.x % 2)) {
				output.y -= 1;
			}
		}
	}

	return output;
}

void LayeredTileSet::draw_cells_outline(CanvasItem *p_canvas_item, const RBSet<Vector2i> &p_cells, Color p_color, Transform2D p_transform) const {
	Vector<Vector2> polygon = get_tile_shape_polygon();

	for (const RBSet<Vector2i>::Element *SE = p_cells.front(); SE; SE = SE->next()) {
		const Vector2i &E = SE->get();

		Vector2 center = map_to_local(E);

#define DRAW_SIDE_IF_NEEDED(side, polygon_index_from, polygon_index_to)                     \
	if (!p_cells.has(get_neighbor_cell(E, side))) {                                         \
		Vector2 from = p_transform.xform(center + polygon[polygon_index_from] * tile_size); \
		Vector2 to = p_transform.xform(center + polygon[polygon_index_to] * tile_size);     \
		p_canvas_item->draw_line(from, to, p_color);                                        \
	}

		if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE, 1, 2);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE, 2, 3);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE, 3, 0);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE, 0, 1);
		} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE, 2, 3);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE, 1, 2);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE, 0, 1);
			DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE, 3, 0);
		} else {
			if (tile_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE, 3, 4);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE, 2, 3);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE, 1, 2);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE, 0, 1);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE, 5, 0);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE, 4, 5);
			} else {
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE, 3, 4);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE, 4, 5);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE, 5, 0);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE, 0, 1);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE, 1, 2);
				DRAW_SIDE_IF_NEEDED(LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE, 2, 3);
			}
		}
	}
#undef DRAW_SIDE_IF_NEEDED
}

Vector<Point2> LayeredTileSet::get_terrain_polygon(int p_terrain_set) {
	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		return _get_square_terrain_polygon(tile_size);
	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		return _get_isometric_terrain_polygon(tile_size);
	} else {
		float overlap = 0.0;
		switch (tile_shape) {
			case LayeredTileSet::TILE_SHAPE_HEXAGON:
				overlap = 0.25;
				break;
			case LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE:
				overlap = 0.0;
				break;
			default:
				break;
		}
		return _get_half_offset_terrain_polygon(tile_size, overlap, tile_offset_axis);
	}
}

Vector<Point2> LayeredTileSet::get_terrain_peering_bit_polygon(int p_terrain_set, LayeredTileSet::CellNeighbor p_bit) {
	ERR_FAIL_COND_V(p_terrain_set < 0 || p_terrain_set >= get_terrain_sets_count(), Vector<Point2>());

	LayeredTileSet::TerrainMode terrain_mode = get_terrain_set_mode(p_terrain_set);

	if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
		if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
			return _get_square_corner_or_side_terrain_peering_bit_polygon(tile_size, p_bit);
		} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
			return _get_square_corner_terrain_peering_bit_polygon(tile_size, p_bit);
		} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
			return _get_square_side_terrain_peering_bit_polygon(tile_size, p_bit);
		}
	} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
		if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
			return _get_isometric_corner_or_side_terrain_peering_bit_polygon(tile_size, p_bit);
		} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
			return _get_isometric_corner_terrain_peering_bit_polygon(tile_size, p_bit);
		} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
			return _get_isometric_side_terrain_peering_bit_polygon(tile_size, p_bit);
		}
	} else {
		float overlap = 0.0;
		switch (tile_shape) {
			case LayeredTileSet::TILE_SHAPE_HEXAGON:
				overlap = 0.25;
				break;
			case LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE:
				overlap = 0.0;
				break;
			default:
				break;
		}
		if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
			return _get_half_offset_corner_or_side_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, p_bit);
		} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
			return _get_half_offset_corner_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, p_bit);
		} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
			return _get_half_offset_side_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, p_bit);
		}
	}
}

#define TERRAIN_ALPHA 0.6

void LayeredTileSet::draw_terrains(CanvasItem *p_canvas_item, Transform2D p_transform, const LayeredTileData *p_tile_data) {
	ERR_FAIL_NULL(p_tile_data);

	if (terrain_bits_meshes_dirty) {
		// Recompute the meshes.
		terrain_peering_bits_meshes.clear();

		for (int terrain_mode_index = 0; terrain_mode_index < 3; terrain_mode_index++) {
			TerrainMode terrain_mode = TerrainMode(terrain_mode_index);

			// Center terrain
			Vector<Vector2> polygon;
			if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
				polygon = _get_square_terrain_polygon(tile_size);
			} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
				polygon = _get_isometric_terrain_polygon(tile_size);
			} else {
				float overlap = 0.0;
				switch (tile_shape) {
					case LayeredTileSet::TILE_SHAPE_HEXAGON:
						overlap = 0.25;
						break;
					case LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE:
						overlap = 0.0;
						break;
					default:
						break;
				}
				polygon = _get_half_offset_terrain_polygon(tile_size, overlap, tile_offset_axis);
			}
			{
				Ref<ArrayMesh> mesh;
				mesh.instance();
				Vector<Vector2> uvs;
				uvs.resize(polygon.size());
				Vector<Color> colors;
				colors.resize(polygon.size());
				colors.fill(Color(1.0, 1.0, 1.0, 1.0));
				Array a;
				a.resize(Mesh::ARRAY_MAX);
				a[Mesh::ARRAY_VERTEX] = polygon;
				a[Mesh::ARRAY_TEX_UV] = uvs;
				a[Mesh::ARRAY_COLOR] = colors;
				a[Mesh::ARRAY_INDEX] = Geometry::triangulate_polygon(polygon);
				mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, a, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);
				terrain_meshes[terrain_mode] = mesh;
			}
			// Peering bits
			for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
				CellNeighbor bit = CellNeighbor(i);

				if (is_valid_terrain_peering_bit_for_mode(terrain_mode, bit)) {
					if (tile_shape == LayeredTileSet::TILE_SHAPE_SQUARE) {
						if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
							polygon = _get_square_corner_or_side_terrain_peering_bit_polygon(tile_size, bit);
						} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
							polygon = _get_square_corner_terrain_peering_bit_polygon(tile_size, bit);
						} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
							polygon = _get_square_side_terrain_peering_bit_polygon(tile_size, bit);
						}
					} else if (tile_shape == LayeredTileSet::TILE_SHAPE_ISOMETRIC) {
						if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
							polygon = _get_isometric_corner_or_side_terrain_peering_bit_polygon(tile_size, bit);
						} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
							polygon = _get_isometric_corner_terrain_peering_bit_polygon(tile_size, bit);
						} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
							polygon = _get_isometric_side_terrain_peering_bit_polygon(tile_size, bit);
						}
					} else {
						float overlap = 0.0;
						switch (tile_shape) {
							case LayeredTileSet::TILE_SHAPE_HEXAGON:
								overlap = 0.25;
								break;
							case LayeredTileSet::TILE_SHAPE_HALF_OFFSET_SQUARE:
								overlap = 0.0;
								break;
							default:
								break;
						}
						if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS_AND_SIDES) {
							polygon = _get_half_offset_corner_or_side_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, bit);
						} else if (terrain_mode == LayeredTileSet::TERRAIN_MODE_MATCH_CORNERS) {
							polygon = _get_half_offset_corner_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, bit);
						} else { // LayeredTileData::TERRAIN_MODE_MATCH_SIDES
							polygon = _get_half_offset_side_terrain_peering_bit_polygon(tile_size, overlap, tile_offset_axis, bit);
						}
					}
					{
						Ref<ArrayMesh> mesh;
						mesh.instance();
						Vector<Vector2> uvs;
						uvs.resize(polygon.size());
						Vector<Color> colors;
						colors.resize(polygon.size());
						colors.fill(Color(1.0, 1.0, 1.0, 1.0));
						Array a;
						a.resize(Mesh::ARRAY_MAX);
						a[Mesh::ARRAY_VERTEX] = polygon;
						a[Mesh::ARRAY_TEX_UV] = uvs;
						a[Mesh::ARRAY_COLOR] = colors;
						a[Mesh::ARRAY_INDEX] = Geometry::triangulate_polygon(polygon);
						mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, a, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);
						terrain_peering_bits_meshes[terrain_mode][bit] = mesh;
					}
				}
			}
		}
		terrain_bits_meshes_dirty = false;
	}

	int terrain_set = p_tile_data->get_terrain_set();
	if (terrain_set < 0) {
		return;
	}
	LayeredTileSet::TerrainMode terrain_mode = get_terrain_set_mode(terrain_set);

	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), p_transform);
	int terrain_id = p_tile_data->get_terrain();
	if (terrain_id >= 0) {
		Color color = get_terrain_color(terrain_set, terrain_id);
		color.a = TERRAIN_ALPHA;
		p_canvas_item->draw_mesh(terrain_meshes[terrain_mode], Ref<Texture>(), Ref<Texture>(), Transform2D(), color);
	}

	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		CellNeighbor bit = CellNeighbor(i);
		if (is_valid_terrain_peering_bit(terrain_set, bit)) {
			terrain_id = p_tile_data->get_terrain_peering_bit(bit);
			if (terrain_id >= 0) {
				Color color = get_terrain_color(terrain_set, terrain_id);
				color.a = TERRAIN_ALPHA;
				p_canvas_item->draw_mesh(terrain_peering_bits_meshes[terrain_mode][bit], Ref<Texture>(), Ref<Texture>(), Transform2D(), color);
			}
		}
	}
	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), Transform2D());
}

Vector<Vector<Ref<Texture>>> LayeredTileSet::generate_terrains_icons(Size2i p_size) {
	// Counts the number of matching terrain tiles and find the best matching icon.
	struct Count {
		int count = 0;
		float probability = 0.0;
		Ref<Texture> texture;
		Rect2i region;
	};
	Vector<Vector<Ref<Texture>>> output;
	LocalVector<LocalVector<Count>> counts;
	output.resize(get_terrain_sets_count());
	counts.resize(get_terrain_sets_count());
	for (int terrain_set = 0; terrain_set < get_terrain_sets_count(); terrain_set++) {
		output.write[terrain_set].resize(get_terrains_count(terrain_set));
		counts[terrain_set].resize(get_terrains_count(terrain_set));
	}

	for (int source_index = 0; source_index < get_source_count(); source_index++) {
		int source_id = get_source_id(source_index);
		Ref<LayeredTileSetSource> source = get_source(source_id);

		Ref<LayeredTileSetAtlasSource> atlas_source = source;
		if (atlas_source.is_valid()) {
			for (int tile_index = 0; tile_index < source->get_tiles_count(); tile_index++) {
				Vector2i tile_id = source->get_tile_id(tile_index);
				for (int alternative_index = 0; alternative_index < source->get_alternative_tiles_count(tile_id); alternative_index++) {
					int alternative_id = source->get_alternative_tile_id(tile_id, alternative_index);

					LayeredTileData *tile_data = atlas_source->get_tile_data(tile_id, alternative_id);
					int terrain_set = tile_data->get_terrain_set();
					if (terrain_set >= 0) {
						ERR_FAIL_INDEX_V(terrain_set, get_terrain_sets_count(), Vector<Vector<Ref<Texture>>>());

						LocalVector<int> bit_counts;
						bit_counts.resize(get_terrains_count(terrain_set));
						for (int terrain = 0; terrain < get_terrains_count(terrain_set); terrain++) {
							bit_counts[terrain] = 0;
						}
						if (tile_data->get_terrain() >= 0) {
							bit_counts[tile_data->get_terrain()] += 10;
						}
						for (int terrain_bit = 0; terrain_bit < LayeredTileSet::CELL_NEIGHBOR_MAX; terrain_bit++) {
							LayeredTileSet::CellNeighbor cell_neighbor = LayeredTileSet::CellNeighbor(terrain_bit);
							if (is_valid_terrain_peering_bit(terrain_set, cell_neighbor)) {
								int terrain = tile_data->get_terrain_peering_bit(cell_neighbor);
								if (terrain >= 0) {
									if (terrain >= (int)bit_counts.size()) {
										WARN_PRINT(vformat("Invalid terrain peering bit: %d", terrain));
									} else {
										bit_counts[terrain] += 1;
									}
								}
							}
						}

						for (int terrain = 0; terrain < get_terrains_count(terrain_set); terrain++) {
							if ((bit_counts[terrain] > counts[terrain_set][terrain].count) || (bit_counts[terrain] == counts[terrain_set][terrain].count && tile_data->get_probability() > counts[terrain_set][terrain].probability)) {
								counts[terrain_set][terrain].count = bit_counts[terrain];
								counts[terrain_set][terrain].probability = tile_data->get_probability();
								counts[terrain_set][terrain].texture = atlas_source->get_texture();
								counts[terrain_set][terrain].region = atlas_source->get_tile_texture_region(tile_id);
							}
						}
					}
				}
			}
		}
	}

	// Generate the icons.
	for (int terrain_set = 0; terrain_set < get_terrain_sets_count(); terrain_set++) {
		for (int terrain = 0; terrain < get_terrains_count(terrain_set); terrain++) {
			Ref<Image> dst_image;
			dst_image.instance();
			if (counts[terrain_set][terrain].count > 0) {
				// Get the best tile.
				Ref<Texture> src_texture = counts[terrain_set][terrain].texture;
				ERR_FAIL_COND_V(src_texture.is_null(), output);
				Ref<Image> src_image = src_texture->get_data();
				ERR_FAIL_COND_V(src_image.is_null(), output);
				Rect2i region = counts[terrain_set][terrain].region;

				dst_image->create(region.size.x, region.size.y, false, src_image->get_format());
				dst_image->blit_rect(src_image, region, Point2i());
				dst_image->convert(Image::FORMAT_RGBA8);
				dst_image->resize(p_size.x, p_size.y, Image::INTERPOLATE_NEAREST);
			} else {
				dst_image->create(1, 1, false, Image::FORMAT_RGBA8);
				dst_image->set_pixel(0, 0, get_terrain_color(terrain_set, terrain));
			}
			Ref<ImageTexture> icon;
			icon.instance();
			icon->create_from_image(dst_image);
			icon->set_size_override(p_size);
			output.write[terrain_set].write[terrain] = icon;
		}
	}
	return output;
}

void LayeredTileSet::_source_changed() {
	terrains_cache_dirty = true;
	emit_changed();
}

Vector<Point2> LayeredTileSet::_get_square_terrain_polygon(Vector2i p_size) {
	Rect2 rect(-Vector2(p_size) / 6.0, Vector2(p_size) / 3.0);

	Vector<Point2> ret;
	ret.resize(4);

	ret.write[0] = rect.position;
	ret.write[1] = Vector2(rect.get_end().x, rect.position.y);
	ret.write[2] = rect.get_end();
	ret.write[3] = Vector2(rect.position.x, rect.get_end().y);

	return ret;
}

Vector<Point2> LayeredTileSet::_get_square_corner_or_side_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Rect2 bit_rect;
	bit_rect.size = Vector2(p_size) / 3;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE:
			bit_rect.position = Vector2(1, -1);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
			bit_rect.position = Vector2(1, 1);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
			bit_rect.position = Vector2(-1, 1);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
			bit_rect.position = Vector2(-3, 1);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE:
			bit_rect.position = Vector2(-3, -1);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
			bit_rect.position = Vector2(-3, -3);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE:
			bit_rect.position = Vector2(-1, -3);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
			bit_rect.position = Vector2(1, -3);
			break;
		default:
			break;
	}
	bit_rect.position *= Vector2(p_size) / 6.0;

	Vector<Vector2> polygon;
	polygon.resize(4);

	polygon.write[0] = bit_rect.position;
	polygon.write[1] = Vector2(bit_rect.get_end().x, bit_rect.position.y);
	polygon.write[2] = bit_rect.get_end();
	polygon.write[3] = Vector2(bit_rect.position.x, bit_rect.get_end().y);

	return polygon;
}

Vector<Point2> LayeredTileSet::_get_square_corner_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(3, 3) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(1, 0) * unit);
			polygon.push_back(Vector2(1, 1) * unit);
			polygon.push_back(Vector2(0, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(-3, 3) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(-1, 0) * unit);
			polygon.push_back(Vector2(-1, 1) * unit);
			polygon.push_back(Vector2(0, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(-3, -3) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(-1, 0) * unit);
			polygon.push_back(Vector2(-1, -1) * unit);
			polygon.push_back(Vector2(0, -1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(3, -3) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(1, 0) * unit);
			polygon.push_back(Vector2(1, -1) * unit);
			polygon.push_back(Vector2(0, -1) * unit);
			break;
		default:
			break;
	}
	return polygon;
}

Vector<Point2> LayeredTileSet::_get_square_side_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE:
			polygon.push_back(Vector2(1, -1) * unit);
			polygon.push_back(Vector2(3, -3) * unit);
			polygon.push_back(Vector2(3, 3) * unit);
			polygon.push_back(Vector2(1, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
			polygon.push_back(Vector2(-1, 1) * unit);
			polygon.push_back(Vector2(-3, 3) * unit);
			polygon.push_back(Vector2(3, 3) * unit);
			polygon.push_back(Vector2(1, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE:
			polygon.push_back(Vector2(-1, -1) * unit);
			polygon.push_back(Vector2(-3, -3) * unit);
			polygon.push_back(Vector2(-3, 3) * unit);
			polygon.push_back(Vector2(-1, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE:
			polygon.push_back(Vector2(-1, -1) * unit);
			polygon.push_back(Vector2(-3, -3) * unit);
			polygon.push_back(Vector2(3, -3) * unit);
			polygon.push_back(Vector2(1, -1) * unit);
			break;
		default:
			break;
	}
	return polygon;
}

Vector<Point2> LayeredTileSet::_get_isometric_terrain_polygon(Vector2i p_size) {
	Vector2 unit = Vector2(p_size) / 6.0;

	Vector<Point2> ret;
	ret.resize(4);

	ret.write[0] = Vector2(1, 0) * unit;
	ret.write[1] = Vector2(0, 1) * unit;
	ret.write[2] = Vector2(-1, 0) * unit;
	ret.write[3] = Vector2(0, -1) * unit;

	return ret;
}

Vector<Point2> LayeredTileSet::_get_isometric_corner_or_side_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER:
			polygon.push_back(Vector2(1, 0) * unit);
			polygon.push_back(Vector2(2, -1) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(2, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
			polygon.push_back(Vector2(0, 1) * unit);
			polygon.push_back(Vector2(1, 2) * unit);
			polygon.push_back(Vector2(2, 1) * unit);
			polygon.push_back(Vector2(1, 0) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER:
			polygon.push_back(Vector2(0, 1) * unit);
			polygon.push_back(Vector2(-1, 2) * unit);
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(1, 2) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
			polygon.push_back(Vector2(0, 1) * unit);
			polygon.push_back(Vector2(-1, 2) * unit);
			polygon.push_back(Vector2(-2, 1) * unit);
			polygon.push_back(Vector2(-1, 0) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER:
			polygon.push_back(Vector2(-1, 0) * unit);
			polygon.push_back(Vector2(-2, -1) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(-2, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
			polygon.push_back(Vector2(0, -1) * unit);
			polygon.push_back(Vector2(-1, -2) * unit);
			polygon.push_back(Vector2(-2, -1) * unit);
			polygon.push_back(Vector2(-1, 0) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER:
			polygon.push_back(Vector2(0, -1) * unit);
			polygon.push_back(Vector2(-1, -2) * unit);
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(1, -2) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
			polygon.push_back(Vector2(0, -1) * unit);
			polygon.push_back(Vector2(1, -2) * unit);
			polygon.push_back(Vector2(2, -1) * unit);
			polygon.push_back(Vector2(1, 0) * unit);
			break;
		default:
			break;
	}
	return polygon;
}

Vector<Point2> LayeredTileSet::_get_isometric_corner_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER:
			polygon.push_back(Vector2(0.5, -0.5) * unit);
			polygon.push_back(Vector2(1.5, -1.5) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(1.5, 1.5) * unit);
			polygon.push_back(Vector2(0.5, 0.5) * unit);
			polygon.push_back(Vector2(1, 0) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER:
			polygon.push_back(Vector2(-0.5, 0.5) * unit);
			polygon.push_back(Vector2(-1.5, 1.5) * unit);
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(1.5, 1.5) * unit);
			polygon.push_back(Vector2(0.5, 0.5) * unit);
			polygon.push_back(Vector2(0, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER:
			polygon.push_back(Vector2(-0.5, -0.5) * unit);
			polygon.push_back(Vector2(-1.5, -1.5) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(-1.5, 1.5) * unit);
			polygon.push_back(Vector2(-0.5, 0.5) * unit);
			polygon.push_back(Vector2(-1, 0) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER:
			polygon.push_back(Vector2(-0.5, -0.5) * unit);
			polygon.push_back(Vector2(-1.5, -1.5) * unit);
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(1.5, -1.5) * unit);
			polygon.push_back(Vector2(0.5, -0.5) * unit);
			polygon.push_back(Vector2(0, -1) * unit);
			break;
		default:
			break;
	}
	return polygon;
}

Vector<Point2> LayeredTileSet::_get_isometric_side_terrain_peering_bit_polygon(Vector2i p_size, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	switch (p_bit) {
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
			polygon.push_back(Vector2(1, 0) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(0, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
			polygon.push_back(Vector2(-1, 0) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(0, 3) * unit);
			polygon.push_back(Vector2(0, 1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
			polygon.push_back(Vector2(-1, 0) * unit);
			polygon.push_back(Vector2(-3, 0) * unit);
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(0, -1) * unit);
			break;
		case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
			polygon.push_back(Vector2(1, 0) * unit);
			polygon.push_back(Vector2(3, 0) * unit);
			polygon.push_back(Vector2(0, -3) * unit);
			polygon.push_back(Vector2(0, -1) * unit);
			break;
		default:
			break;
	}
	return polygon;
}

Vector<Point2> LayeredTileSet::_get_half_offset_terrain_polygon(Vector2i p_size, float p_overlap, LayeredTileSet::TileOffsetAxis p_offset_axis) {
	Vector2 unit = Vector2(p_size) / 6.0;

	Vector<Point2> ret;
	ret.resize(6);

	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		ret.write[0] = Vector2(1, 1.0 - p_overlap * 2.0) * unit;
		ret.write[1] = Vector2(0, 1) * unit;
		ret.write[2] = Vector2(-1, 1.0 - p_overlap * 2.0) * unit;
		ret.write[3] = Vector2(-1, -1.0 + p_overlap * 2.0) * unit;
		ret.write[4] = Vector2(0, -1) * unit;
		ret.write[5] = Vector2(1, -1.0 + p_overlap * 2.0) * unit;

	} else {
		ret.write[0] = Vector2(1, 0) * unit;
		ret.write[1] = Vector2(1.0 - p_overlap * 2.0, -1) * unit;
		ret.write[2] = Vector2(-1.0 + p_overlap * 2.0, -1) * unit;
		ret.write[3] = Vector2(-1, 0) * unit;
		ret.write[4] = Vector2(-1.0 + p_overlap * 2.0, 1) * unit;
		ret.write[5] = Vector2(1.0 - p_overlap * 2.0, 1) * unit;
	}

	return ret;
}

Vector<Point2> LayeredTileSet::_get_half_offset_corner_or_side_terrain_peering_bit_polygon(Vector2i p_size, float p_overlap, LayeredTileSet::TileOffsetAxis p_offset_axis, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 point_list[18] = {
		Vector2(3, (3.0 * (1.0 - p_overlap * 2.0)) / 2.0),
		Vector2(3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(2, 3.0 * (1.0 - (p_overlap * 2.0) * 2.0 / 3.0)),
		Vector2(1, 3.0 - p_overlap * 2.0),
		Vector2(0, 3),
		Vector2(-1, 3.0 - p_overlap * 2.0),
		Vector2(-2, 3.0 * (1.0 - (p_overlap * 2.0) * 2.0 / 3.0)),
		Vector2(-3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(-3, (3.0 * (1.0 - p_overlap * 2.0)) / 2.0),
		Vector2(-3, -(3.0 * (1.0 - p_overlap * 2.0)) / 2.0),
		Vector2(-3, -3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(-2, -3.0 * (1.0 - (p_overlap * 2.0) * 2.0 / 3.0)),
		Vector2(-1, -(3.0 - p_overlap * 2.0)),
		Vector2(0, -3),
		Vector2(1, -(3.0 - p_overlap * 2.0)),
		Vector2(2, -3.0 * (1.0 - (p_overlap * 2.0) * 2.0 / 3.0)),
		Vector2(3, -3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(3, -(3.0 * (1.0 - p_overlap * 2.0)) / 2.0)
	};

	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		for (int i = 0; i < 18; i++) {
			point_list[i] = point_list[i] * unit;
		}

		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE:
				polygon.push_back(point_list[17]);
				polygon.push_back(point_list[0]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER:
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[6]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
				polygon.push_back(point_list[6]);
				polygon.push_back(point_list[7]);
				polygon.push_back(point_list[8]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE:
				polygon.push_back(point_list[8]);
				polygon.push_back(point_list[9]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
				polygon.push_back(point_list[9]);
				polygon.push_back(point_list[10]);
				polygon.push_back(point_list[11]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
				polygon.push_back(point_list[11]);
				polygon.push_back(point_list[12]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER:
				polygon.push_back(point_list[12]);
				polygon.push_back(point_list[13]);
				polygon.push_back(point_list[14]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
				polygon.push_back(point_list[14]);
				polygon.push_back(point_list[15]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
				polygon.push_back(point_list[15]);
				polygon.push_back(point_list[16]);
				polygon.push_back(point_list[17]);
				break;
			default:
				break;
		}
	} else {
		for (int i = 0; i < 18; i++) {
			point_list[i] = Vector2(point_list[i].y, point_list[i].x) * unit;
		}

		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER:
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
				polygon.push_back(point_list[17]);
				polygon.push_back(point_list[0]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
				polygon.push_back(point_list[15]);
				polygon.push_back(point_list[16]);
				polygon.push_back(point_list[17]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
				polygon.push_back(point_list[14]);
				polygon.push_back(point_list[15]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER:
				polygon.push_back(point_list[12]);
				polygon.push_back(point_list[13]);
				polygon.push_back(point_list[14]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
				polygon.push_back(point_list[11]);
				polygon.push_back(point_list[12]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
				polygon.push_back(point_list[9]);
				polygon.push_back(point_list[10]);
				polygon.push_back(point_list[11]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE:
				polygon.push_back(point_list[8]);
				polygon.push_back(point_list[9]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
				polygon.push_back(point_list[6]);
				polygon.push_back(point_list[7]);
				polygon.push_back(point_list[8]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[6]);
				break;
			default:
				break;
		}
	}

	int half_polygon_size = polygon.size();
	for (int i = 0; i < half_polygon_size; i++) {
		polygon.push_back(polygon[half_polygon_size - 1 - i] / 3.0);
	}

	return polygon;
}

Vector<Point2> LayeredTileSet::_get_half_offset_corner_terrain_peering_bit_polygon(Vector2i p_size, float p_overlap, LayeredTileSet::TileOffsetAxis p_offset_axis, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 point_list[12] = {
		Vector2(3, 0),
		Vector2(3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(1.5, (3.0 * (1.0 - p_overlap * 2.0) + 3.0) / 2.0),
		Vector2(0, 3),
		Vector2(-1.5, (3.0 * (1.0 - p_overlap * 2.0) + 3.0) / 2.0),
		Vector2(-3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(-3, 0),
		Vector2(-3, -3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(-1.5, -(3.0 * (1.0 - p_overlap * 2.0) + 3.0) / 2.0),
		Vector2(0, -3),
		Vector2(1.5, -(3.0 * (1.0 - p_overlap * 2.0) + 3.0) / 2.0),
		Vector2(3, -3.0 * (1.0 - p_overlap * 2.0))
	};

	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		for (int i = 0; i < 12; i++) {
			point_list[i] = point_list[i] * unit;
		}
		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_CORNER:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[6]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
				polygon.push_back(point_list[6]);
				polygon.push_back(point_list[7]);
				polygon.push_back(point_list[8]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_CORNER:
				polygon.push_back(point_list[8]);
				polygon.push_back(point_list[9]);
				polygon.push_back(point_list[10]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
				polygon.push_back(point_list[10]);
				polygon.push_back(point_list[11]);
				polygon.push_back(point_list[0]);
				break;
			default:
				break;
		}
	} else {
		for (int i = 0; i < 12; i++) {
			point_list[i] = Vector2(point_list[i].y, point_list[i].x) * unit;
		}

		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_RIGHT_CORNER:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER:
				polygon.push_back(point_list[10]);
				polygon.push_back(point_list[11]);
				polygon.push_back(point_list[0]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_LEFT_CORNER:
				polygon.push_back(point_list[8]);
				polygon.push_back(point_list[9]);
				polygon.push_back(point_list[10]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_CORNER:
				polygon.push_back(point_list[6]);
				polygon.push_back(point_list[7]);
				polygon.push_back(point_list[8]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_CORNER:
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[6]);
				break;
			default:
				break;
		}
	}

	int half_polygon_size = polygon.size();
	for (int i = 0; i < half_polygon_size; i++) {
		polygon.push_back(polygon[half_polygon_size - 1 - i] / 3.0);
	}

	return polygon;
}

Vector<Point2> LayeredTileSet::_get_half_offset_side_terrain_peering_bit_polygon(Vector2i p_size, float p_overlap, LayeredTileSet::TileOffsetAxis p_offset_axis, LayeredTileSet::CellNeighbor p_bit) {
	Vector2 point_list[6] = {
		Vector2(3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(0, 3),
		Vector2(-3, 3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(-3, -3.0 * (1.0 - p_overlap * 2.0)),
		Vector2(0, -3),
		Vector2(3, -3.0 * (1.0 - p_overlap * 2.0))
	};

	Vector2 unit = Vector2(p_size) / 6.0;
	Vector<Vector2> polygon;
	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_HORIZONTAL) {
		for (int i = 0; i < 6; i++) {
			point_list[i] = point_list[i] * unit;
		}

		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_RIGHT_SIDE:
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[0]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_LEFT_SIDE:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				break;
			default:
				break;
		}
	} else {
		for (int i = 0; i < 6; i++) {
			point_list[i] = Vector2(point_list[i].y, point_list[i].x) * unit;
		}

		switch (p_bit) {
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE:
				polygon.push_back(point_list[0]);
				polygon.push_back(point_list[1]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
				polygon.push_back(point_list[5]);
				polygon.push_back(point_list[0]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE:
				polygon.push_back(point_list[4]);
				polygon.push_back(point_list[5]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_LEFT_SIDE:
				polygon.push_back(point_list[3]);
				polygon.push_back(point_list[4]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_SIDE:
				polygon.push_back(point_list[2]);
				polygon.push_back(point_list[3]);
				break;
			case LayeredTileSet::CELL_NEIGHBOR_TOP_RIGHT_SIDE:
				polygon.push_back(point_list[1]);
				polygon.push_back(point_list[2]);
				break;
			default:
				break;
		}
	}

	int half_polygon_size = polygon.size();
	for (int i = 0; i < half_polygon_size; i++) {
		polygon.push_back(polygon[half_polygon_size - 1 - i] / 3.0);
	}

	return polygon;
}

void LayeredTileSet::reset_state() {
	// Rendering
	occlusion_layers.clear();
	tile_lines_mesh.instance();
	tile_filled_mesh.instance();
	tile_meshes_dirty = true;

	// Physics
	physics_layers.clear();

	// Terrains
	terrain_sets.clear();
	terrain_meshes.clear();
	terrain_peering_bits_meshes.clear();
	per_terrain_pattern_tiles.clear();
	terrains_cache_dirty = true;

	// Navigation
	navigation_layers.clear();

	custom_data_layers.clear();
	custom_data_layers_by_name.clear();

	// Proxies
	source_level_proxies.clear();
	coords_level_proxies.clear();
	alternative_level_proxies.clear();

#ifndef DISABLE_DEPRECATED
	for (const HashMap<int, CompatibilityTileData *>::Element *E = compatibility_data.front(); E; E = E->next) {
		memdelete(E->value());
	}

	compatibility_data.clear();
#endif // DISABLE_DEPRECATED

	while (!source_ids.empty()) {
		remove_source(source_ids[0]);
	}

	tile_shape = TILE_SHAPE_SQUARE;
	tile_layout = TILE_LAYOUT_STACKED;
	tile_offset_axis = TILE_OFFSET_AXIS_HORIZONTAL;
	tile_size = Size2i(16, 16);
}

Vector2i LayeredTileSet::transform_coords_layout(const Vector2i &p_coords, LayeredTileSet::TileOffsetAxis p_offset_axis, LayeredTileSet::TileLayout p_from_layout, LayeredTileSet::TileLayout p_to_layout) {
	// Transform to stacked layout.
	Vector2i output = p_coords;
	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL) {
		SWAP(output.x, output.y);
	}
	switch (p_from_layout) {
		case LayeredTileSet::TILE_LAYOUT_STACKED:
			break;
		case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
			if (output.y % 2) {
				output.x -= 1;
			}
			break;
		case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
		case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
			if ((p_from_layout == LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT) ^ (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
				if (output.y < 0 && bool(output.y % 2)) {
					output = Vector2i(output.x + output.y / 2 - 1, output.y);
				} else {
					output = Vector2i(output.x + output.y / 2, output.y);
				}
			} else {
				if (output.x < 0 && bool(output.x % 2)) {
					output = Vector2i(output.x / 2 - 1, output.x + output.y * 2);
				} else {
					output = Vector2i(output.x / 2, output.x + output.y * 2);
				}
			}
			break;
		case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
		case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
			if ((p_from_layout == LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT) ^ (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
				if ((output.x + output.y) < 0 && (output.x - output.y) % 2) {
					output = Vector2i((output.x + output.y) / 2 - 1, output.y - output.x);
				} else {
					output = Vector2i((output.x + output.y) / 2, -output.x + output.y);
				}
			} else {
				if ((output.x - output.y) < 0 && (output.x + output.y) % 2) {
					output = Vector2i((output.x - output.y) / 2 - 1, output.x + output.y);
				} else {
					output = Vector2i((output.x - output.y) / 2, output.x + output.y);
				}
			}
			break;
	}

	switch (p_to_layout) {
		case LayeredTileSet::TILE_LAYOUT_STACKED:
			break;
		case LayeredTileSet::TILE_LAYOUT_STACKED_OFFSET:
			if (output.y % 2) {
				output.x += 1;
			}
			break;
		case LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT:
		case LayeredTileSet::TILE_LAYOUT_STAIRS_DOWN:
			if ((p_to_layout == LayeredTileSet::TILE_LAYOUT_STAIRS_RIGHT) ^ (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
				if (output.y < 0 && (output.y % 2)) {
					output = Vector2i(output.x - output.y / 2 + 1, output.y);
				} else {
					output = Vector2i(output.x - output.y / 2, output.y);
				}
			} else {
				if (output.y % 2) {
					if (output.y < 0) {
						output = Vector2i(2 * output.x + 1, -output.x + output.y / 2 - 1);
					} else {
						output = Vector2i(2 * output.x + 1, -output.x + output.y / 2);
					}
				} else {
					output = Vector2i(2 * output.x, -output.x + output.y / 2);
				}
			}
			break;
		case LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT:
		case LayeredTileSet::TILE_LAYOUT_DIAMOND_DOWN:
			if ((p_to_layout == LayeredTileSet::TILE_LAYOUT_DIAMOND_RIGHT) ^ (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL)) {
				if (output.y % 2) {
					if (output.y > 0) {
						output = Vector2i(output.x - output.y / 2, output.x + output.y / 2 + 1);
					} else {
						output = Vector2i(output.x - output.y / 2 + 1, output.x + output.y / 2);
					}
				} else {
					output = Vector2i(output.x - output.y / 2, output.x + output.y / 2);
				}
			} else {
				if (output.y % 2) {
					if (output.y < 0) {
						output = Vector2i(output.x + output.y / 2, -output.x + output.y / 2 - 1);
					} else {
						output = Vector2i(output.x + output.y / 2 + 1, -output.x + output.y / 2);
					}
				} else {
					output = Vector2i(output.x + output.y / 2, -output.x + output.y / 2);
				}
			}
			break;
	}

	if (p_offset_axis == LayeredTileSet::TILE_OFFSET_AXIS_VERTICAL) {
		SWAP(output.x, output.y);
	}

	return output;
}

const Vector2i LayeredTileSetSource::INVALID_ATLAS_COORDS = Vector2i(-1, -1);
const int LayeredTileSetSource::INVALID_TILE_ALTERNATIVE = -1;

#ifndef DISABLE_DEPRECATED
void LayeredTileSet::_compatibility_conversion() {
	for (HashMap<int, CompatibilityTileData *>::Element *E = compatibility_data.front(); E; E = E->next) {
		CompatibilityTileData *ctd = E->value();

		// Add the texture
		LayeredTileSetAtlasSource *atlas_source = memnew(LayeredTileSetAtlasSource);
		int source_id = add_source(Ref<LayeredTileSetSource>(atlas_source));

		atlas_source->set_texture(ctd->texture);

		// Handle each tile as a new source. Not optimal but at least it should stay compatible.
		switch (ctd->tile_mode) {
			case COMPATIBILITY_TILE_MODE_SINGLE_TILE: {
				atlas_source->set_margins(ctd->region.get_position());
				atlas_source->set_texture_region_size(ctd->region.get_size());

				Vector2i coords;
				for (int flags = 0; flags < 8; flags++) {
					bool flip_h = flags & 1;
					bool flip_v = flags & 2;
					bool transpose = flags & 4;

					Transform2D xform;
					xform = flip_h ? xform.scaled(Size2(-1, 1)) : xform;
					xform = flip_v ? xform.scaled(Size2(1, -1)) : xform;
					xform = transpose ? Transform2D(xform[1], xform[0], Vector2()) : xform;

					int alternative_tile = 0;
					if (!atlas_source->has_tile(coords)) {
						atlas_source->create_tile(coords);
					} else {
						alternative_tile = atlas_source->create_alternative_tile(coords);
					}

					// Add to the mapping.
					Array key_array;
					key_array.push_back(flip_h);
					key_array.push_back(flip_v);
					key_array.push_back(transpose);

					Array value_array;
					value_array.push_back(source_id);
					value_array.push_back(coords);
					value_array.push_back(alternative_tile);

					if (!compatibility_tilemap_mapping.has(E->key())) {
						compatibility_tilemap_mapping[E->key()] = RBMap<Array, Array>();
					}
					compatibility_tilemap_mapping[E->key()][key_array] = value_array;
					compatibility_tilemap_mapping_tile_modes[E->key()] = COMPATIBILITY_TILE_MODE_SINGLE_TILE;

					LayeredTileData *tile_data = atlas_source->get_tile_data(coords, alternative_tile);
					ERR_CONTINUE(!tile_data);

					tile_data->set_flip_h(flip_h);
					tile_data->set_flip_v(flip_v);
					tile_data->set_transpose(transpose);
					tile_data->set_material(ctd->material);
					tile_data->set_modulate(ctd->modulate);
					tile_data->set_z_index(ctd->z_index);

					if (ctd->occluder.is_valid()) {
						if (get_occlusion_layers_count() < 1) {
							add_occlusion_layer();
						};
						Ref<OccluderPolygon2D> occluder = ctd->occluder->duplicate();
						PoolVector<Vector2> polygon = ctd->occluder->get_polygon();
						for (int index = 0; index < polygon.size(); index++) {
							polygon.set(index, xform.xform(polygon[index] - ctd->region.get_size() / 2.0));
						}
						occluder->set_polygon(polygon);
						tile_data->set_occluder(0, occluder);
					}
					if (ctd->navigation.is_valid()) {
						if (get_navigation_layers_count() < 1) {
							add_navigation_layer();
						}
						Ref<NavigationPolygon> navigation = ctd->navigation->duplicate();
						PoolVector<Vector2> vertices = navigation->get_vertices();
						for (int index = 0; index < vertices.size(); index++) {
							vertices.set(index, xform.xform(vertices[index] - ctd->region.get_size() / 2.0));
						}
						navigation->set_vertices(vertices);
						tile_data->set_navigation_polygon(0, navigation);
					}

					tile_data->set_z_index(ctd->z_index);

					// Add the shapes.
					if (ctd->shapes.size() > 0) {
						if (get_physics_layers_count() < 1) {
							add_physics_layer();
						}
					}
					for (int k = 0; k < ctd->shapes.size(); k++) {
						CompatibilityShapeData csd = ctd->shapes[k];
						if (csd.autotile_coords == coords) {
							Ref<ConvexPolygonShape2D> convex_shape = csd.shape; // Only ConvexPolygonShape2D are supported, which is the default type used by the 3.x editor
							if (convex_shape.is_valid()) {
								Vector<Vector2> polygon = convex_shape->get_points();
								for (int point_index = 0; point_index < polygon.size(); point_index++) {
									polygon.write[point_index] = xform.xform(csd.transform.xform(polygon[point_index]) - ctd->region.get_size() / 2.0);
								}
								tile_data->set_collision_polygons_count(0, tile_data->get_collision_polygons_count(0) + 1);
								int index = tile_data->get_collision_polygons_count(0) - 1;
								tile_data->set_collision_polygon_one_way(0, index, csd.one_way);
								tile_data->set_collision_polygon_one_way_margin(0, index, csd.one_way_margin);
								tile_data->set_collision_polygon_points(0, index, polygon);
							}
						}
					}
				}
				// Update the size count.
				if (!compatibility_size_count.has(ctd->region.get_size())) {
					compatibility_size_count[ctd->region.get_size()] = 0;
				}
				compatibility_size_count[ctd->region.get_size()]++;
			} break;
			case COMPATIBILITY_TILE_MODE_AUTO_TILE: {
				// Not supported. It would need manual conversion.
				WARN_PRINT_ONCE("Could not convert 3.x autotiles to 4.x. This operation cannot be done automatically, autotiles must be re-created using the terrain system.");
			} break;
			case COMPATIBILITY_TILE_MODE_ATLAS_TILE: {
				atlas_source->set_margins(ctd->region.get_position());
				atlas_source->set_separation(Vector2i(ctd->autotile_spacing, ctd->autotile_spacing));
				atlas_source->set_texture_region_size(ctd->autotile_tile_size);

				Size2i atlas_size = ctd->region.get_size() / (ctd->autotile_tile_size + atlas_source->get_separation());
				for (int i = 0; i < atlas_size.x; i++) {
					for (int j = 0; j < atlas_size.y; j++) {
						Vector2i coords = Vector2i(i, j);

						for (int flags = 0; flags < 8; flags++) {
							bool flip_h = flags & 1;
							bool flip_v = flags & 2;
							bool transpose = flags & 4;

							Transform2D xform;
							xform = flip_h ? xform.scaled(Size2(-1, 1)) : xform;
							xform = flip_v ? xform.scaled(Size2(1, -1)) : xform;
							xform = transpose ? Transform2D(xform[1], xform[0], Vector2()) : xform;

							int alternative_tile = 0;
							if (!atlas_source->has_tile(coords)) {
								atlas_source->create_tile(coords);
							} else {
								alternative_tile = atlas_source->create_alternative_tile(coords);
							}

							// Add to the mapping.
							Array key_array;
							key_array.push_back(coords);
							key_array.push_back(flip_h);
							key_array.push_back(flip_v);
							key_array.push_back(transpose);

							Array value_array;
							value_array.push_back(source_id);
							value_array.push_back(coords);
							value_array.push_back(alternative_tile);

							if (!compatibility_tilemap_mapping.has(E->key())) {
								compatibility_tilemap_mapping[E->key()] = RBMap<Array, Array>();
							}
							compatibility_tilemap_mapping[E->key()][key_array] = value_array;
							compatibility_tilemap_mapping_tile_modes[E->key()] = COMPATIBILITY_TILE_MODE_ATLAS_TILE;

							LayeredTileData *tile_data = atlas_source->get_tile_data(coords, alternative_tile);

							tile_data->set_flip_h(flip_h);
							tile_data->set_flip_v(flip_v);
							tile_data->set_transpose(transpose);
							tile_data->set_material(ctd->material);
							tile_data->set_modulate(ctd->modulate);
							tile_data->set_z_index(ctd->z_index);
							if (ctd->autotile_occluder_map.has(coords)) {
								if (get_occlusion_layers_count() < 1) {
									add_occlusion_layer();
								}
								Ref<OccluderPolygon2D> occluder = ctd->autotile_occluder_map[coords]->duplicate();
								PoolVector<Vector2> polygon = ctd->occluder->get_polygon();
								for (int index = 0; index < polygon.size(); index++) {
									polygon.set(index, xform.xform(polygon[index] - ctd->region.get_size() / 2.0));
								}
								occluder->set_polygon(polygon);
								tile_data->set_occluder(0, occluder);
							}
							if (ctd->autotile_navpoly_map.has(coords)) {
								if (get_navigation_layers_count() < 1) {
									add_navigation_layer();
								}
								Ref<NavigationPolygon> navigation = ctd->autotile_navpoly_map[coords]->duplicate();
								PoolVector<Vector2> vertices = navigation->get_vertices();
								for (int index = 0; index < vertices.size(); index++) {
									vertices.set(index, xform.xform(vertices[index] - ctd->region.get_size() / 2.0));
								}
								navigation->set_vertices(vertices);
								tile_data->set_navigation_polygon(0, navigation);
							}
							if (ctd->autotile_priority_map.has(coords)) {
								tile_data->set_probability(ctd->autotile_priority_map[coords]);
							}
							if (ctd->autotile_z_index_map.has(coords)) {
								tile_data->set_z_index(ctd->autotile_z_index_map[coords]);
							}

							// Add the shapes.
							if (ctd->shapes.size() > 0) {
								if (get_physics_layers_count() < 1) {
									add_physics_layer();
								}
							}
							for (int k = 0; k < ctd->shapes.size(); k++) {
								CompatibilityShapeData csd = ctd->shapes[k];
								if (csd.autotile_coords == coords) {
									Ref<ConvexPolygonShape2D> convex_shape = csd.shape; // Only ConvexPolygonShape2D are supported, which is the default type used by the 3.x editor
									if (convex_shape.is_valid()) {
										Vector<Vector2> polygon = convex_shape->get_points();
										for (int point_index = 0; point_index < polygon.size(); point_index++) {
											polygon.write[point_index] = xform.xform(csd.transform.xform(polygon[point_index]) - ctd->autotile_tile_size / 2.0);
										}
										tile_data->set_collision_polygons_count(0, tile_data->get_collision_polygons_count(0) + 1);
										int index = tile_data->get_collision_polygons_count(0) - 1;
										tile_data->set_collision_polygon_one_way(0, index, csd.one_way);
										tile_data->set_collision_polygon_one_way_margin(0, index, csd.one_way_margin);
										tile_data->set_collision_polygon_points(0, index, polygon);
									}
								}
							}

							// -- TODO: handle --
							// Those are offset for the whole atlas, they are likely useless for the atlases, but might make sense for single tiles.
							// texture offset
							// occluder_offset
							// navigation_offset

							// For terrains, ignored for now?
							// bitmask_mode
							// bitmask_flags
						}
					}
				}

				// Update the size count.
				if (!compatibility_size_count.has(ctd->region.get_size())) {
					compatibility_size_count[ctd->autotile_tile_size] = 0;
				}
				compatibility_size_count[ctd->autotile_tile_size] += atlas_size.x * atlas_size.y;
			} break;
		}

		// Offset all shapes
		for (int k = 0; k < ctd->shapes.size(); k++) {
			Ref<ConvexPolygonShape2D> convex = ctd->shapes[k].shape;
			if (convex.is_valid()) {
				Vector<Vector2> points = convex->get_points();
				for (int i_point = 0; i_point < points.size(); i_point++) {
					points.write[i_point] = points[i_point] - get_tile_size() / 2;
				}
				convex->set_points(points);
			}
		}
	}

	// Update the LayeredTileSet tile_size according to the most common size found.
	Vector2i max_size = get_tile_size();
	int max_count = 0;

	for (HashMap<Vector2i, int>::Element *kv = compatibility_size_count.front(); kv; kv = kv->next) {
		if (kv->value() > max_count) {
			max_size = kv->key();
			max_count = kv->value();
		}
	}

	set_tile_size(max_size);

	// Reset compatibility data (besides the histogram counts)
	for (const HashMap<int, CompatibilityTileData *>::Element *E = compatibility_data.front(); E; E = E->next) {
		memdelete(E->value());
	}

	compatibility_data = HashMap<int, CompatibilityTileData *>();
}

Array LayeredTileSet::compatibility_tilemap_map(int p_tile_id, Vector2i p_coords, bool p_flip_h, bool p_flip_v, bool p_transpose) {
	Array cannot_convert_array;
	cannot_convert_array.push_back(LayeredTileSet::INVALID_SOURCE);
	cannot_convert_array.push_back(LayeredTileSetAtlasSource::INVALID_ATLAS_COORDS);
	cannot_convert_array.push_back(LayeredTileSetAtlasSource::INVALID_TILE_ALTERNATIVE);

	if (!compatibility_tilemap_mapping.has(p_tile_id)) {
		return cannot_convert_array;
	}

	int tile_mode = compatibility_tilemap_mapping_tile_modes[p_tile_id];
	switch (tile_mode) {
		case COMPATIBILITY_TILE_MODE_SINGLE_TILE: {
			Array a;
			a.push_back(p_flip_h);
			a.push_back(p_flip_v);
			a.push_back(p_transpose);
			return compatibility_tilemap_mapping[p_tile_id][a];
		}
		case COMPATIBILITY_TILE_MODE_AUTO_TILE:
			return cannot_convert_array;
			break;
		case COMPATIBILITY_TILE_MODE_ATLAS_TILE: {
			Array a;
			a.push_back(p_coords);
			a.push_back(p_flip_h);
			a.push_back(p_flip_v);
			a.push_back(p_transpose);
			return compatibility_tilemap_mapping[p_tile_id][a];
		}
		default:
			return cannot_convert_array;
			break;
	}
};

#endif // DISABLE_DEPRECATED

bool LayeredTileSet::_set(const StringName &p_name, const Variant &p_value) {
	Vector<String> components = String(p_name).split("/", true, 2);

#ifndef DISABLE_DEPRECATED
	// TODO: This should be moved to a dedicated conversion system (see #50691)
	if (components.size() >= 1 && components[0].is_valid_integer()) {
		int id = components[0].to_int();

		// Get or create the compatibility object
		CompatibilityTileData *ctd;
		HashMap<int, CompatibilityTileData *>::Element *E = compatibility_data.find(id);

		if (!E) {
			ctd = memnew(CompatibilityTileData);
			compatibility_data.insert(id, ctd);
		} else {
			ctd = E->value();
		}

		if (components.size() < 2) {
			return false;
		}

		String what = components[1];

		if (what == "name") {
			ctd->name = p_value;
		} else if (what == "texture") {
			ctd->texture = p_value;
		} else if (what == "tex_offset") {
			ctd->tex_offset = p_value;
		} else if (what == "material") {
			ctd->material = p_value;
		} else if (what == "modulate") {
			ctd->modulate = p_value;
		} else if (what == "region") {
			ctd->region = p_value;
		} else if (what == "tile_mode") {
			ctd->tile_mode = p_value;
		} else if (what.left(9) == "autotile") {
			what = what.substr(9);
			if (what == "bitmask_mode") {
				ctd->autotile_bitmask_mode = p_value;
			} else if (what == "icon_coordinate") {
				ctd->autotile_icon_coordinate = p_value;
			} else if (what == "tile_size") {
				ctd->autotile_tile_size = p_value;
			} else if (what == "spacing") {
				ctd->autotile_spacing = p_value;
			} else if (what == "bitmask_flags") {
				if (p_value.is_array()) {
					Array p = p_value;
					Vector2i last_coord;
					while (p.size() > 0) {
						if (p[0].get_type() == Variant::VECTOR2) {
							last_coord = p[0];
						} else if (p[0].get_type() == Variant::INT) {
							ctd->autotile_bitmask_flags.insert(last_coord, p[0]);
						}
						p.pop_front();
					}
				}
			} else if (what == "occluder_map") {
				Array p = p_value;
				Vector2 last_coord;
				while (p.size() > 0) {
					if (p[0].get_type() == Variant::VECTOR2) {
						last_coord = p[0];
					} else if (p[0].get_type() == Variant::OBJECT) {
						ctd->autotile_occluder_map.insert(last_coord, p[0]);
					}
					p.pop_front();
				}
			} else if (what == "navpoly_map") {
				Array p = p_value;
				Vector2 last_coord;
				while (p.size() > 0) {
					if (p[0].get_type() == Variant::VECTOR2) {
						last_coord = p[0];
					} else if (p[0].get_type() == Variant::OBJECT) {
						ctd->autotile_navpoly_map.insert(last_coord, p[0]);
					}
					p.pop_front();
				}
			} else if (what == "priority_map") {
				Array p = p_value;
				Vector3 val;
				Vector2 v;
				int priority;
				while (p.size() > 0) {
					val = p[0];
					if (val.z > 1) {
						v.x = val.x;
						v.y = val.y;
						priority = (int)val.z;
						ctd->autotile_priority_map.insert(v, priority);
					}
					p.pop_front();
				}
			} else if (what == "z_index_map") {
				Array p = p_value;
				Vector3 val;
				Vector2 v;
				int z_index;
				while (p.size() > 0) {
					val = p[0];
					if (val.z != 0) {
						v.x = val.x;
						v.y = val.y;
						z_index = (int)val.z;
						ctd->autotile_z_index_map.insert(v, z_index);
					}
					p.pop_front();
				}
			}

		} else if (what == "shapes") {
			Array p = p_value;
			for (int i = 0; i < p.size(); i++) {
				CompatibilityShapeData csd;
				Dictionary d = p[i];
				for (int j = 0; j < d.size(); j++) {
					String key = d.get_key_at_index(j);
					if (key == "autotile_coord") {
						csd.autotile_coords = d[key];
					} else if (key == "one_way") {
						csd.one_way = d[key];
					} else if (key == "one_way_margin") {
						csd.one_way_margin = d[key];
					} else if (key == "shape") {
						csd.shape = d[key];
					} else if (key == "shape_transform") {
						csd.transform = d[key];
					}
				}
				ctd->shapes.push_back(csd);
			}
		} else if (what == "occluder") {
			ctd->occluder = p_value;
		} else if (what == "navigation") {
			ctd->navigation = p_value;

			/*
		// IGNORED FOR NOW, they seem duplicated data compared to the shapes array
		} else if (what == "shape") {
		} else if (what == "shape_offset") {
		} else if (what == "shape_transform") {
		} else if (what == "shape_one_way") {
		} else if (what == "shape_one_way_margin") {
		}
		// IGNORED FOR NOW, maybe useless ?
		else if (what == "occluder_offset") {
			// Not
		} else if (what == "navigation_offset") {
		}
		*/

		} else if (what == "z_index") {
			ctd->z_index = p_value;

			// TODO: remove the conversion from here, it's not where it should be done (see #50691)
			_compatibility_conversion();
		} else {
			return false;
		}
	} else {
#endif // DISABLE_DEPRECATED

		// This is now a new property.
		if (components.size() == 2 && components[0].begins_with("occlusion_layer_") && components[0].trim_prefix("occlusion_layer_").is_valid_integer()) {
			// Occlusion layers.
			int index = components[0].trim_prefix("occlusion_layer_").to_int();
			ERR_FAIL_COND_V(index < 0, false);
			if (components[1] == "light_mask") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (index >= occlusion_layers.size()) {
					add_occlusion_layer();
				}
				set_occlusion_layer_light_mask(index, p_value);
				return true;
			} else if (components[1] == "sdf_collision") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::BOOL, false);
				while (index >= occlusion_layers.size()) {
					add_occlusion_layer();
				}
				set_occlusion_layer_sdf_collision(index, p_value);
				return true;
			}
		} else if (components.size() == 2 && components[0].begins_with("physics_layer_") && components[0].trim_prefix("physics_layer_").is_valid_integer()) {
			// Physics layers.
			int index = components[0].trim_prefix("physics_layer_").to_int();
			ERR_FAIL_COND_V(index < 0, false);
			if (components[1] == "collision_layer") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (index >= physics_layers.size()) {
					add_physics_layer();
				}
				set_physics_layer_collision_layer(index, p_value);
				return true;
			} else if (components[1] == "collision_mask") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (index >= physics_layers.size()) {
					add_physics_layer();
				}
				set_physics_layer_collision_mask(index, p_value);
				return true;
			} else if (components[1] == "physics_material") {
				Ref<PhysicsMaterial> physics_material = p_value;
				while (index >= physics_layers.size()) {
					add_physics_layer();
				}
				set_physics_layer_physics_material(index, physics_material);
				return true;
			}
		} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer()) {
			// Terrains.
			int terrain_set_index = components[0].trim_prefix("terrain_set_").to_int();
			ERR_FAIL_COND_V(terrain_set_index < 0, false);
			if (components[1] == "mode") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (terrain_set_index >= terrain_sets.size()) {
					add_terrain_set();
				}
				set_terrain_set_mode(terrain_set_index, TerrainMode(int(p_value)));
			} else if (components.size() >= 3 && components[1].begins_with("terrain_") && components[1].trim_prefix("terrain_").is_valid_integer()) {
				int terrain_index = components[1].trim_prefix("terrain_").to_int();
				ERR_FAIL_COND_V(terrain_index < 0, false);
				if (components[2] == "name") {
					ERR_FAIL_COND_V(p_value.get_type() != Variant::STRING, false);
					while (terrain_set_index >= terrain_sets.size()) {
						add_terrain_set();
					}
					while (terrain_index >= terrain_sets[terrain_set_index].terrains.size()) {
						add_terrain(terrain_set_index);
					}
					set_terrain_name(terrain_set_index, terrain_index, p_value);
					return true;
				} else if (components[2] == "color") {
					ERR_FAIL_COND_V(p_value.get_type() != Variant::COLOR, false);
					while (terrain_set_index >= terrain_sets.size()) {
						add_terrain_set();
					}
					while (terrain_index >= terrain_sets[terrain_set_index].terrains.size()) {
						add_terrain(terrain_set_index);
					}
					set_terrain_color(terrain_set_index, terrain_index, p_value);
					return true;
				}
			}
		} else if (components.size() == 2 && components[0].begins_with("navigation_layer_") && components[0].trim_prefix("navigation_layer_").is_valid_integer()) {
			// Navigation layers.
			int index = components[0].trim_prefix("navigation_layer_").to_int();
			ERR_FAIL_COND_V(index < 0, false);
			if (components[1] == "layers") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (index >= navigation_layers.size()) {
					add_navigation_layer();
				}
				set_navigation_layer_layers(index, p_value);
				return true;
			}
		} else if (components.size() == 2 && components[0].begins_with("custom_data_layer_") && components[0].trim_prefix("custom_data_layer_").is_valid_integer()) {
			// Custom data layers.
			int index = components[0].trim_prefix("custom_data_layer_").to_int();
			ERR_FAIL_COND_V(index < 0, false);
			if (components[1] == "name") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::STRING, false);
				while (index >= custom_data_layers.size()) {
					add_custom_data_layer();
				}
				set_custom_data_layer_name(index, p_value);
				return true;
			} else if (components[1] == "type") {
				ERR_FAIL_COND_V(p_value.get_type() != Variant::INT, false);
				while (index >= custom_data_layers.size()) {
					add_custom_data_layer();
				}
				set_custom_data_layer_type(index, Variant::Type(int(p_value)));
				return true;
			}
		} else if (components.size() == 2 && components[0] == "sources" && components[1].is_valid_integer()) {
			// Create source only if it does not exists.
			int source_id = components[1].to_int();

			if (has_source(source_id)) {
				remove_source(source_id);
			}
			add_source(p_value, source_id);
			return true;
		} else if (components.size() == 2 && components[0] == "tile_proxies") {
			ERR_FAIL_COND_V(p_value.get_type() != Variant::ARRAY, false);
			Array a = p_value;
			ERR_FAIL_COND_V(a.size() % 2 != 0, false);
			if (components[1] == "source_level") {
				for (int i = 0; i < a.size(); i += 2) {
					set_source_level_tile_proxy(a[i], a[i + 1]);
				}
				return true;
			} else if (components[1] == "coords_level") {
				for (int i = 0; i < a.size(); i += 2) {
					Array key = a[i];
					Array value = a[i + 1];
					set_coords_level_tile_proxy(key[0], key[1], value[0], value[1]);
				}
				return true;
			} else if (components[1] == "alternative_level") {
				for (int i = 0; i < a.size(); i += 2) {
					Array key = a[i];
					Array value = a[i + 1];
					set_alternative_level_tile_proxy(key[0], key[1], key[2], value[0], value[1], value[2]);
				}
				return true;
			}
			return false;
		} else if (components.size() == 1 && components[0].begins_with("pattern_") && components[0].trim_prefix("pattern_").is_valid_integer()) {
			int pattern_index = components[0].trim_prefix("pattern_").to_int();
			for (int i = patterns.size(); i <= pattern_index; i++) {
				add_pattern(p_value);
			}
			return true;
		}

#ifndef DISABLE_DEPRECATED
	}
#endif // DISABLE_DEPRECATED

	return false;
}

bool LayeredTileSet::_get(const StringName &p_name, Variant &r_ret) const {
	Vector<String> components = String(p_name).split("/", true, 2);

	if (components.size() == 2 && components[0].begins_with("occlusion_layer_") && components[0].trim_prefix("occlusion_layer_").is_valid_integer()) {
		// Occlusion layers.
		int index = components[0].trim_prefix("occlusion_layer_").to_int();
		if (index < 0 || index >= occlusion_layers.size()) {
			return false;
		}
		if (components[1] == "light_mask") {
			r_ret = get_occlusion_layer_light_mask(index);
			return true;
		} else if (components[1] == "sdf_collision") {
			r_ret = get_occlusion_layer_sdf_collision(index);
			return true;
		}
	} else if (components.size() == 2 && components[0].begins_with("physics_layer_") && components[0].trim_prefix("physics_layer_").is_valid_integer()) {
		// Physics layers.
		int index = components[0].trim_prefix("physics_layer_").to_int();
		if (index < 0 || index >= physics_layers.size()) {
			return false;
		}
		if (components[1] == "collision_layer") {
			r_ret = get_physics_layer_collision_layer(index);
			return true;
		} else if (components[1] == "collision_mask") {
			r_ret = get_physics_layer_collision_mask(index);
			return true;
		} else if (components[1] == "physics_material") {
			r_ret = get_physics_layer_physics_material(index);
			return true;
		}
	} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer()) {
		// Terrains.
		int terrain_set_index = components[0].trim_prefix("terrain_set_").to_int();
		if (terrain_set_index < 0 || terrain_set_index >= terrain_sets.size()) {
			return false;
		}
		if (components[1] == "mode") {
			r_ret = get_terrain_set_mode(terrain_set_index);
			return true;
		} else if (components.size() >= 3 && components[1].begins_with("terrain_") && components[1].trim_prefix("terrain_").is_valid_integer()) {
			int terrain_index = components[1].trim_prefix("terrain_").to_int();
			if (terrain_index < 0 || terrain_index >= terrain_sets[terrain_set_index].terrains.size()) {
				return false;
			}
			if (components[2] == "name") {
				r_ret = get_terrain_name(terrain_set_index, terrain_index);
				return true;
			} else if (components[2] == "color") {
				r_ret = get_terrain_color(terrain_set_index, terrain_index);
				return true;
			}
		}
	} else if (components.size() == 2 && components[0].begins_with("navigation_layer_") && components[0].trim_prefix("navigation_layer_").is_valid_integer()) {
		// navigation layers.
		int index = components[0].trim_prefix("navigation_layer_").to_int();
		if (index < 0 || index >= navigation_layers.size()) {
			return false;
		}
		if (components[1] == "layers") {
			r_ret = get_navigation_layer_layers(index);
			return true;
		}
	} else if (components.size() == 2 && components[0].begins_with("custom_data_layer_") && components[0].trim_prefix("custom_data_layer_").is_valid_integer()) {
		// Custom data layers.
		int index = components[0].trim_prefix("custom_data_layer_").to_int();
		if (index < 0 || index >= custom_data_layers.size()) {
			return false;
		}
		if (components[1] == "name") {
			r_ret = get_custom_data_layer_name(index);
			return true;
		} else if (components[1] == "type") {
			r_ret = get_custom_data_layer_type(index);
			return true;
		}
	} else if (components.size() == 2 && components[0] == "sources" && components[1].is_valid_integer()) {
		// Atlases data.
		int source_id = components[1].to_int();

		if (has_source(source_id)) {
			r_ret = get_source(source_id);
			return true;
		} else {
			return false;
		}
	} else if (components.size() == 2 && components[0] == "tile_proxies") {
		if (components[1] == "source_level") {
			Array a;
			for (const RBMap<int, int>::Element *E = source_level_proxies.front(); E; E = E->next()) {
				a.push_back(E->key());
				a.push_back(E->value());
			}
			r_ret = a;
			return true;
		} else if (components[1] == "coords_level") {
			Array a;
			for (const RBMap<Array, Array>::Element *E = coords_level_proxies.front(); E; E = E->next()) {
				a.push_back(E->key());
				a.push_back(E->value());
			}
			r_ret = a;
			return true;
		} else if (components[1] == "alternative_level") {
			Array a;
			for (const RBMap<Array, Array>::Element *E = alternative_level_proxies.front(); E; E = E->next()) {
				a.push_back(E->key());
				a.push_back(E->value());
			}
			r_ret = a;
			return true;
		}
		return false;
	} else if (components.size() == 1 && components[0].begins_with("pattern_") && components[0].trim_prefix("pattern_").is_valid_integer()) {
		int pattern_index = components[0].trim_prefix("pattern_").to_int();
		if (pattern_index < 0 || pattern_index >= (int)patterns.size()) {
			return false;
		}
		r_ret = patterns[pattern_index];
		return true;
	}

	return false;
}

void LayeredTileSet::_get_property_list(List<PropertyInfo> *p_list) const {
	PropertyInfo property_info;
	// Rendering.
	p_list->push_back(PropertyInfo(Variant::NIL, "Rendering", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	for (int i = 0; i < occlusion_layers.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::INT, vformat("occlusion_layer_%d/light_mask", i), PROPERTY_HINT_LAYERS_2D_RENDER));

		// occlusion_layer_%d/sdf_collision
		property_info = PropertyInfo(Variant::BOOL, vformat("occlusion_layer_%d/sdf_collision", i));
		if (occlusion_layers[i].sdf_collision == false) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		p_list->push_back(property_info);
	}

	// Physics.
	p_list->push_back(PropertyInfo(Variant::NIL, "Physics", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	for (int i = 0; i < physics_layers.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::INT, vformat("physics_layer_%d/collision_layer", i), PROPERTY_HINT_LAYERS_2D_PHYSICS));

		// physics_layer_%d/collision_mask
		property_info = PropertyInfo(Variant::INT, vformat("physics_layer_%d/collision_mask", i), PROPERTY_HINT_LAYERS_2D_PHYSICS);
		if (physics_layers[i].collision_mask == 1) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		p_list->push_back(property_info);

		// physics_layer_%d/physics_material
		property_info = PropertyInfo(Variant::OBJECT, vformat("physics_layer_%d/physics_material", i), PROPERTY_HINT_RESOURCE_TYPE, "PhysicsMaterial");
		if (!physics_layers[i].physics_material.is_valid()) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		p_list->push_back(property_info);
	}

	// Terrains.
	p_list->push_back(PropertyInfo(Variant::NIL, "Terrains", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	for (int terrain_set_index = 0; terrain_set_index < terrain_sets.size(); terrain_set_index++) {
		p_list->push_back(PropertyInfo(Variant::INT, vformat("terrain_set_%d/mode", terrain_set_index), PROPERTY_HINT_ENUM, "Match Corners and Sides,Match Corners,Match Sides"));
		p_list->push_back(PropertyInfo(Variant::NIL, vformat("terrain_set_%d/terrains", terrain_set_index), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_ARRAY, vformat("terrain_set_%d/terrain_", terrain_set_index)));
		for (int terrain_index = 0; terrain_index < terrain_sets[terrain_set_index].terrains.size(); terrain_index++) {
			p_list->push_back(PropertyInfo(Variant::STRING, vformat("terrain_set_%d/terrain_%d/name", terrain_set_index, terrain_index)));
			p_list->push_back(PropertyInfo(Variant::COLOR, vformat("terrain_set_%d/terrain_%d/color", terrain_set_index, terrain_index)));
		}
	}

	// Navigation.
	p_list->push_back(PropertyInfo(Variant::NIL, "Navigation", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	for (int i = 0; i < navigation_layers.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::INT, vformat("navigation_layer_%d/layers", i), PROPERTY_HINT_LAYERS_2D_NAVIGATION));
	}

	// Custom data.
	String argt = "Any";
	for (int i = 1; i < Variant::VARIANT_MAX; i++) {
		argt += "," + Variant::get_type_name(Variant::Type(i));
	}
	p_list->push_back(PropertyInfo(Variant::NIL, "Custom Data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	for (int i = 0; i < custom_data_layers.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::STRING, vformat("custom_data_layer_%d/name", i)));
		p_list->push_back(PropertyInfo(Variant::INT, vformat("custom_data_layer_%d/type", i), PROPERTY_HINT_ENUM, argt));
	}

	// Sources.
	// Note: sources have to be listed in at the end as some TileData rely on the LayeredTileSet properties being initialized first.

	for (const HashMap<int, Ref<LayeredTileSetSource>>::Element *E_source = sources.front(); E_source; E_source = E_source->next) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, vformat("sources/%d", E_source->key()), PROPERTY_HINT_RESOURCE_TYPE, "LayeredTileSetAtlasSource", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE));
	}

	// Tile Proxies.
	// Note: proxies need to be set after sources are set.
	p_list->push_back(PropertyInfo(Variant::NIL, "Tile Proxies", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

	p_list->push_back(PropertyInfo(Variant::ARRAY, "tile_proxies/source_level", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	p_list->push_back(PropertyInfo(Variant::ARRAY, "tile_proxies/coords_level", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	p_list->push_back(PropertyInfo(Variant::ARRAY, "tile_proxies/alternative_level", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));

	// Patterns.
	for (unsigned int pattern_index = 0; pattern_index < patterns.size(); pattern_index++) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, vformat("pattern_%d", pattern_index), PROPERTY_HINT_RESOURCE_TYPE, "LayeredTileMapPattern", PROPERTY_USAGE_NOEDITOR));
	}
}

void LayeredTileSet::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "tile_layout" && tile_shape == TILE_SHAPE_SQUARE) {
		//p_property.usage ^= PROPERTY_USAGE_READ_ONLY;
		p_property.usage = 0;
	} else if (p_property.name == "tile_offset_axis" && tile_shape == TILE_SHAPE_SQUARE) {
		//p_property.usage ^= PROPERTY_USAGE_READ_ONLY;
		p_property.usage = 0;
	}
}

void LayeredTileSet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_source_changed"), &LayeredTileSet::_source_changed);

	// Sources management.
	ClassDB::bind_method(D_METHOD("get_next_source_id"), &LayeredTileSet::get_next_source_id);
	ClassDB::bind_method(D_METHOD("add_source", "source", "atlas_source_id_override"), &LayeredTileSet::add_source, DEFVAL(LayeredTileSet::INVALID_SOURCE));
	ClassDB::bind_method(D_METHOD("remove_source", "source_id"), &LayeredTileSet::remove_source);
	ClassDB::bind_method(D_METHOD("set_source_id", "source_id", "new_source_id"), &LayeredTileSet::set_source_id);
	ClassDB::bind_method(D_METHOD("get_source_count"), &LayeredTileSet::get_source_count);
	ClassDB::bind_method(D_METHOD("get_source_id", "index"), &LayeredTileSet::get_source_id);
	ClassDB::bind_method(D_METHOD("has_source", "source_id"), &LayeredTileSet::has_source);
	ClassDB::bind_method(D_METHOD("get_source", "source_id"), &LayeredTileSet::get_source);

	// Shape and layout.
	ClassDB::bind_method(D_METHOD("set_tile_shape", "shape"), &LayeredTileSet::set_tile_shape);
	ClassDB::bind_method(D_METHOD("get_tile_shape"), &LayeredTileSet::get_tile_shape);
	ClassDB::bind_method(D_METHOD("set_tile_layout", "layout"), &LayeredTileSet::set_tile_layout);
	ClassDB::bind_method(D_METHOD("get_tile_layout"), &LayeredTileSet::get_tile_layout);
	ClassDB::bind_method(D_METHOD("set_tile_offset_axis", "alignment"), &LayeredTileSet::set_tile_offset_axis);
	ClassDB::bind_method(D_METHOD("get_tile_offset_axis"), &LayeredTileSet::get_tile_offset_axis);
	ClassDB::bind_method(D_METHOD("set_tile_size", "size"), &LayeredTileSet::set_tile_size);
	ClassDB::bind_method(D_METHOD("get_tile_size"), &LayeredTileSet::get_tile_size);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_shape", PROPERTY_HINT_ENUM, "Square,Isometric,Half-Offset Square,Hexagon"), "set_tile_shape", "get_tile_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_layout", PROPERTY_HINT_ENUM, "Stacked,Stacked Offset,Stairs Right,Stairs Down,Diamond Right,Diamond Down"), "set_tile_layout", "get_tile_layout");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_offset_axis", PROPERTY_HINT_ENUM, "Horizontal Offset,Vertical Offset"), "set_tile_offset_axis", "get_tile_offset_axis");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "tile_size", PROPERTY_HINT_NONE, "suffix:px"), "set_tile_size", "get_tile_size");

	// Rendering.
	ClassDB::bind_method(D_METHOD("set_uv_clipping", "uv_clipping"), &LayeredTileSet::set_uv_clipping);
	ClassDB::bind_method(D_METHOD("is_uv_clipping"), &LayeredTileSet::is_uv_clipping);

	ClassDB::bind_method(D_METHOD("get_occlusion_layers_count"), &LayeredTileSet::get_occlusion_layers_count);
	ClassDB::bind_method(D_METHOD("add_occlusion_layer", "to_position"), &LayeredTileSet::add_occlusion_layer, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_occlusion_layer", "layer_index", "to_position"), &LayeredTileSet::move_occlusion_layer);
	ClassDB::bind_method(D_METHOD("remove_occlusion_layer", "layer_index"), &LayeredTileSet::remove_occlusion_layer);
	ClassDB::bind_method(D_METHOD("set_occlusion_layer_light_mask", "layer_index", "light_mask"), &LayeredTileSet::set_occlusion_layer_light_mask);
	ClassDB::bind_method(D_METHOD("get_occlusion_layer_light_mask", "layer_index"), &LayeredTileSet::get_occlusion_layer_light_mask);
	ClassDB::bind_method(D_METHOD("set_occlusion_layer_sdf_collision", "layer_index", "sdf_collision"), &LayeredTileSet::set_occlusion_layer_sdf_collision);
	ClassDB::bind_method(D_METHOD("get_occlusion_layer_sdf_collision", "layer_index"), &LayeredTileSet::get_occlusion_layer_sdf_collision);

	// Physics
	ClassDB::bind_method(D_METHOD("get_physics_layers_count"), &LayeredTileSet::get_physics_layers_count);
	ClassDB::bind_method(D_METHOD("add_physics_layer", "to_position"), &LayeredTileSet::add_physics_layer, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_physics_layer", "layer_index", "to_position"), &LayeredTileSet::move_physics_layer);
	ClassDB::bind_method(D_METHOD("remove_physics_layer", "layer_index"), &LayeredTileSet::remove_physics_layer);
	ClassDB::bind_method(D_METHOD("set_physics_layer_collision_layer", "layer_index", "layer"), &LayeredTileSet::set_physics_layer_collision_layer);
	ClassDB::bind_method(D_METHOD("get_physics_layer_collision_layer", "layer_index"), &LayeredTileSet::get_physics_layer_collision_layer);
	ClassDB::bind_method(D_METHOD("set_physics_layer_collision_mask", "layer_index", "mask"), &LayeredTileSet::set_physics_layer_collision_mask);
	ClassDB::bind_method(D_METHOD("get_physics_layer_collision_mask", "layer_index"), &LayeredTileSet::get_physics_layer_collision_mask);
	ClassDB::bind_method(D_METHOD("set_physics_layer_physics_material", "layer_index", "physics_material"), &LayeredTileSet::set_physics_layer_physics_material);
	ClassDB::bind_method(D_METHOD("get_physics_layer_physics_material", "layer_index"), &LayeredTileSet::get_physics_layer_physics_material);

	// Terrains
	ClassDB::bind_method(D_METHOD("get_terrain_sets_count"), &LayeredTileSet::get_terrain_sets_count);
	ClassDB::bind_method(D_METHOD("add_terrain_set", "to_position"), &LayeredTileSet::add_terrain_set, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_terrain_set", "terrain_set", "to_position"), &LayeredTileSet::move_terrain_set);
	ClassDB::bind_method(D_METHOD("remove_terrain_set", "terrain_set"), &LayeredTileSet::remove_terrain_set);
	ClassDB::bind_method(D_METHOD("set_terrain_set_mode", "terrain_set", "mode"), &LayeredTileSet::set_terrain_set_mode);
	ClassDB::bind_method(D_METHOD("get_terrain_set_mode", "terrain_set"), &LayeredTileSet::get_terrain_set_mode);

	ClassDB::bind_method(D_METHOD("get_terrains_count", "terrain_set"), &LayeredTileSet::get_terrains_count);
	ClassDB::bind_method(D_METHOD("add_terrain", "terrain_set", "to_position"), &LayeredTileSet::add_terrain, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_terrain", "terrain_set", "terrain_index", "to_position"), &LayeredTileSet::move_terrain);
	ClassDB::bind_method(D_METHOD("remove_terrain", "terrain_set", "terrain_index"), &LayeredTileSet::remove_terrain);
	ClassDB::bind_method(D_METHOD("set_terrain_name", "terrain_set", "terrain_index", "name"), &LayeredTileSet::set_terrain_name);
	ClassDB::bind_method(D_METHOD("get_terrain_name", "terrain_set", "terrain_index"), &LayeredTileSet::get_terrain_name);
	ClassDB::bind_method(D_METHOD("set_terrain_color", "terrain_set", "terrain_index", "color"), &LayeredTileSet::set_terrain_color);
	ClassDB::bind_method(D_METHOD("get_terrain_color", "terrain_set", "terrain_index"), &LayeredTileSet::get_terrain_color);

	// Navigation
	ClassDB::bind_method(D_METHOD("get_navigation_layers_count"), &LayeredTileSet::get_navigation_layers_count);
	ClassDB::bind_method(D_METHOD("add_navigation_layer", "to_position"), &LayeredTileSet::add_navigation_layer, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_navigation_layer", "layer_index", "to_position"), &LayeredTileSet::move_navigation_layer);
	ClassDB::bind_method(D_METHOD("remove_navigation_layer", "layer_index"), &LayeredTileSet::remove_navigation_layer);
	ClassDB::bind_method(D_METHOD("set_navigation_layer_layers", "layer_index", "layers"), &LayeredTileSet::set_navigation_layer_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_layers", "layer_index"), &LayeredTileSet::get_navigation_layer_layers);
	ClassDB::bind_method(D_METHOD("set_navigation_layer_layer_value", "layer_index", "layer_number", "value"), &LayeredTileSet::set_navigation_layer_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_layer_value", "layer_index", "layer_number"), &LayeredTileSet::get_navigation_layer_layer_value);

	// Custom data
	ClassDB::bind_method(D_METHOD("get_custom_data_layers_count"), &LayeredTileSet::get_custom_data_layers_count);
	ClassDB::bind_method(D_METHOD("add_custom_data_layer", "to_position"), &LayeredTileSet::add_custom_data_layer, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_custom_data_layer", "layer_index", "to_position"), &LayeredTileSet::move_custom_data_layer);
	ClassDB::bind_method(D_METHOD("remove_custom_data_layer", "layer_index"), &LayeredTileSet::remove_custom_data_layer);
	ClassDB::bind_method(D_METHOD("get_custom_data_layer_by_name", "layer_name"), &LayeredTileSet::get_custom_data_layer_by_name);
	ClassDB::bind_method(D_METHOD("set_custom_data_layer_name", "layer_index", "layer_name"), &LayeredTileSet::set_custom_data_layer_name);
	ClassDB::bind_method(D_METHOD("get_custom_data_layer_name", "layer_index"), &LayeredTileSet::get_custom_data_layer_name);
	ClassDB::bind_method(D_METHOD("set_custom_data_layer_type", "layer_index", "layer_type"), &LayeredTileSet::set_custom_data_layer_type);
	ClassDB::bind_method(D_METHOD("get_custom_data_layer_type", "layer_index"), &LayeredTileSet::get_custom_data_layer_type);

	// Tile proxies
	ClassDB::bind_method(D_METHOD("set_source_level_tile_proxy", "source_from", "source_to"), &LayeredTileSet::set_source_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("get_source_level_tile_proxy", "source_from"), &LayeredTileSet::get_source_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("has_source_level_tile_proxy", "source_from"), &LayeredTileSet::has_source_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("remove_source_level_tile_proxy", "source_from"), &LayeredTileSet::remove_source_level_tile_proxy);

	ClassDB::bind_method(D_METHOD("set_coords_level_tile_proxy", "p_source_from", "coords_from", "source_to", "coords_to"), &LayeredTileSet::set_coords_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("get_coords_level_tile_proxy", "source_from", "coords_from"), &LayeredTileSet::get_coords_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("has_coords_level_tile_proxy", "source_from", "coords_from"), &LayeredTileSet::has_coords_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("remove_coords_level_tile_proxy", "source_from", "coords_from"), &LayeredTileSet::remove_coords_level_tile_proxy);

	ClassDB::bind_method(D_METHOD("set_alternative_level_tile_proxy", "source_from", "coords_from", "alternative_from", "source_to", "coords_to", "alternative_to"), &LayeredTileSet::set_alternative_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("get_alternative_level_tile_proxy", "source_from", "coords_from", "alternative_from"), &LayeredTileSet::get_alternative_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("has_alternative_level_tile_proxy", "source_from", "coords_from", "alternative_from"), &LayeredTileSet::has_alternative_level_tile_proxy);
	ClassDB::bind_method(D_METHOD("remove_alternative_level_tile_proxy", "source_from", "coords_from", "alternative_from"), &LayeredTileSet::remove_alternative_level_tile_proxy);

	ClassDB::bind_method(D_METHOD("map_tile_proxy", "source_from", "coords_from", "alternative_from"), &LayeredTileSet::map_tile_proxy);

	ClassDB::bind_method(D_METHOD("cleanup_invalid_tile_proxies"), &LayeredTileSet::cleanup_invalid_tile_proxies);
	ClassDB::bind_method(D_METHOD("clear_tile_proxies"), &LayeredTileSet::clear_tile_proxies);

	// Patterns
	ClassDB::bind_method(D_METHOD("add_pattern", "pattern", "index"), &LayeredTileSet::add_pattern, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_pattern", "index"), &LayeredTileSet::get_pattern, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("remove_pattern", "index"), &LayeredTileSet::remove_pattern);
	ClassDB::bind_method(D_METHOD("get_patterns_count"), &LayeredTileSet::get_patterns_count);

	ADD_GROUP("Rendering", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "uv_clipping"), "set_uv_clipping", "is_uv_clipping");
	ADD_ARRAY("occlusion_layers", "occlusion_layer_");

	ADD_GROUP("", "");
	ADD_ARRAY("physics_layers", "physics_layer_");
	ADD_ARRAY("terrain_sets", "terrain_set_");
	ADD_ARRAY("navigation_layers", "navigation_layer_");
	ADD_ARRAY("custom_data_layers", "custom_data_layer_");

	// -- Enum binding --
	BIND_ENUM_CONSTANT(TILE_SHAPE_SQUARE);
	BIND_ENUM_CONSTANT(TILE_SHAPE_ISOMETRIC);
	BIND_ENUM_CONSTANT(TILE_SHAPE_HALF_OFFSET_SQUARE);
	BIND_ENUM_CONSTANT(TILE_SHAPE_HEXAGON);

	BIND_ENUM_CONSTANT(TILE_LAYOUT_STACKED);
	BIND_ENUM_CONSTANT(TILE_LAYOUT_STACKED_OFFSET);
	BIND_ENUM_CONSTANT(TILE_LAYOUT_STAIRS_RIGHT);
	BIND_ENUM_CONSTANT(TILE_LAYOUT_STAIRS_DOWN);
	BIND_ENUM_CONSTANT(TILE_LAYOUT_DIAMOND_RIGHT);
	BIND_ENUM_CONSTANT(TILE_LAYOUT_DIAMOND_DOWN);

	BIND_ENUM_CONSTANT(TILE_OFFSET_AXIS_HORIZONTAL);
	BIND_ENUM_CONSTANT(TILE_OFFSET_AXIS_VERTICAL);

	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_RIGHT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_RIGHT_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_LEFT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_BOTTOM_LEFT_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_LEFT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_LEFT_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_LEFT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_LEFT_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_CORNER);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_RIGHT_SIDE);
	BIND_ENUM_CONSTANT(CELL_NEIGHBOR_TOP_RIGHT_CORNER);

	BIND_ENUM_CONSTANT(TERRAIN_MODE_MATCH_CORNERS_AND_SIDES);
	BIND_ENUM_CONSTANT(TERRAIN_MODE_MATCH_CORNERS);
	BIND_ENUM_CONSTANT(TERRAIN_MODE_MATCH_SIDES);
}

LayeredTileSet::LayeredTileSet() {
	// Instantiate the tile meshes.
	tile_lines_mesh.instance();
	tile_filled_mesh.instance();
}

LayeredTileSet::~LayeredTileSet() {
#ifndef DISABLE_DEPRECATED
	for (const HashMap<int, CompatibilityTileData *>::Element *E = compatibility_data.front(); E; E = E->next) {
		memdelete(E->value());
	}
#endif // DISABLE_DEPRECATED
	while (!source_ids.empty()) {
		remove_source(source_ids[0]);
	}
}

/////////////////////////////// LayeredTileSetSource //////////////////////////////////////

void LayeredTileSetSource::set_tile_set(const LayeredTileSet *p_tile_set) {
	tile_set = p_tile_set;
}

LayeredTileSet *LayeredTileSetSource::get_tile_set() const {
	return (LayeredTileSet *)tile_set;
}

void LayeredTileSetSource::reset_state() {
	tile_set = nullptr;
};

void LayeredTileSetSource::_bind_methods() {
	// Base tiles
	ClassDB::bind_method(D_METHOD("get_tiles_count"), &LayeredTileSetSource::get_tiles_count);
	ClassDB::bind_method(D_METHOD("get_tile_id", "index"), &LayeredTileSetSource::get_tile_id);
	ClassDB::bind_method(D_METHOD("has_tile", "atlas_coords"), &LayeredTileSetSource::has_tile);

	// Alternative tiles
	ClassDB::bind_method(D_METHOD("get_alternative_tiles_count", "atlas_coords"), &LayeredTileSetSource::get_alternative_tiles_count);
	ClassDB::bind_method(D_METHOD("get_alternative_tile_id", "atlas_coords", "index"), &LayeredTileSetSource::get_alternative_tile_id);
	ClassDB::bind_method(D_METHOD("has_alternative_tile", "atlas_coords", "alternative_tile"), &LayeredTileSetSource::has_alternative_tile);
}

/////////////////////////////// LayeredTileSetAtlasSource //////////////////////////////////////

void LayeredTileSetAtlasSource::set_tile_set(const LayeredTileSet *p_tile_set) {
	tile_set = p_tile_set;

	// Set the LayeredTileSet on all TileData.
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->set_tile_set(tile_set);
		}
	}
}

const LayeredTileSet *LayeredTileSetAtlasSource::get_tile_set() const {
	return tile_set;
}

void LayeredTileSetAtlasSource::notify_tile_data_properties_should_change() {
	// Set the LayeredTileSet on all TileData.
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->notify_tile_data_properties_should_change();
		}
	}
}

void LayeredTileSetAtlasSource::add_occlusion_layer(int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_occlusion_layer(p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_occlusion_layer(int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_occlusion_layer(p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_occlusion_layer(int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_occlusion_layer(p_index);
		}
	}
}

void LayeredTileSetAtlasSource::add_physics_layer(int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_physics_layer(p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_physics_layer(int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_physics_layer(p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_physics_layer(int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_physics_layer(p_index);
		}
	}
}

void LayeredTileSetAtlasSource::add_terrain_set(int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_terrain_set(p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_terrain_set(int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_terrain_set(p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_terrain_set(int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_terrain_set(p_index);
		}
	}
}

void LayeredTileSetAtlasSource::add_terrain(int p_terrain_set, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_terrain(p_terrain_set, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_terrain(int p_terrain_set, int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_terrain(p_terrain_set, p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_terrain(int p_terrain_set, int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_terrain(p_terrain_set, p_index);
		}
	}
}

void LayeredTileSetAtlasSource::add_navigation_layer(int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_navigation_layer(p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_navigation_layer(int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_navigation_layer(p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_navigation_layer(int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_navigation_layer(p_index);
		}
	}
}

void LayeredTileSetAtlasSource::add_custom_data_layer(int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->add_custom_data_layer(p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::move_custom_data_layer(int p_from_index, int p_to_pos) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->move_custom_data_layer(p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetAtlasSource::remove_custom_data_layer(int p_index) {
	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			E_alternative->value()->remove_custom_data_layer(p_index);
		}
	}
}

void LayeredTileSetAtlasSource::reset_state() {
	tile_set = nullptr;

	for (HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		for (const HashMap<int, LayeredTileData *>::Element *E_tile_data = E_tile->value().alternatives.front(); E_tile_data; E_tile_data = E_tile_data->next) {
			memdelete(E_tile_data->value());
		}
	}
	_coords_mapping_cache.clear();
	tiles.clear();
	tiles_ids.clear();
	_queue_update_padded_texture();
}

void LayeredTileSetAtlasSource::set_texture(Ref<Texture> p_texture) {
	if (texture.is_valid()) {
		texture->disconnect(CoreStringNames::get_singleton()->changed, this, "_queue_update_padded_texture");
	}

	texture = p_texture;

	if (texture.is_valid()) {
		texture->connect(CoreStringNames::get_singleton()->changed, this, "_queue_update_padded_texture");
	}

	_queue_update_padded_texture();
	emit_changed();
}

Ref<Texture> LayeredTileSetAtlasSource::get_texture() const {
	return texture;
}

void LayeredTileSetAtlasSource::set_margins(Vector2i p_margins) {
	if (p_margins.x < 0 || p_margins.y < 0) {
		WARN_PRINT("Atlas source margins should be positive.");
		margins = Vector2i(MAX(0, p_margins.x), MAX(0, p_margins.y));
	} else {
		margins = p_margins;
	}

	_queue_update_padded_texture();
	emit_changed();
}

Vector2i LayeredTileSetAtlasSource::get_margins() const {
	return margins;
}

void LayeredTileSetAtlasSource::set_separation(Vector2i p_separation) {
	if (p_separation.x < 0 || p_separation.y < 0) {
		WARN_PRINT("Atlas source separation should be positive.");
		separation = Vector2i(MAX(0, p_separation.x), MAX(0, p_separation.y));
	} else {
		separation = p_separation;
	}

	_queue_update_padded_texture();
	emit_changed();
}

Vector2i LayeredTileSetAtlasSource::get_separation() const {
	return separation;
}

void LayeredTileSetAtlasSource::set_texture_region_size(Vector2i p_tile_size) {
	if (p_tile_size.x <= 0 || p_tile_size.y <= 0) {
		WARN_PRINT("Atlas source tile_size should be strictly positive.");
		texture_region_size = Vector2i(MAX(1, p_tile_size.x), MAX(1, p_tile_size.y));
	} else {
		texture_region_size = p_tile_size;
	}

	_queue_update_padded_texture();
	emit_changed();
}

Vector2i LayeredTileSetAtlasSource::get_texture_region_size() const {
	return texture_region_size;
}

void LayeredTileSetAtlasSource::set_use_texture_padding(bool p_use_padding) {
	if (use_texture_padding == p_use_padding) {
		return;
	}
	use_texture_padding = p_use_padding;
	_queue_update_padded_texture();
	emit_changed();
}

bool LayeredTileSetAtlasSource::get_use_texture_padding() const {
	return use_texture_padding;
}

Vector2i LayeredTileSetAtlasSource::get_atlas_grid_size() const {
	Ref<Texture> txt = get_texture();
	if (!txt.is_valid()) {
		return Vector2i();
	}

	ERR_FAIL_COND_V(texture_region_size.x <= 0 || texture_region_size.y <= 0, Vector2i());

	Size2i valid_area = txt->get_size() - margins;

	// Compute the number of valid tiles in the tiles atlas
	Size2i grid_size;
	if (valid_area.x >= texture_region_size.x && valid_area.y >= texture_region_size.y) {
		valid_area -= texture_region_size;
		grid_size = Size2i(1, 1) + valid_area / (texture_region_size + separation);
	}
	return grid_size;
}

bool LayeredTileSetAtlasSource::_set(const StringName &p_name, const Variant &p_value) {
	Vector<String> components = String(p_name).split("/", true, 2);

	// Compute the vector2i if we have coordinates.
	Vector<String> coords_split = components[0].split(":");
	Vector2i coords = LayeredTileSetSource::INVALID_ATLAS_COORDS;
	if (coords_split.size() == 2 && coords_split[0].is_valid_integer() && coords_split[1].is_valid_integer()) {
		coords = Vector2i(coords_split[0].to_int(), coords_split[1].to_int());
	}

	// Properties.
	if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
		// Create the tile if needed.
		if (!has_tile(coords)) {
			create_tile(coords);
		}
		if (components.size() >= 2) {
			// Properties.
			if (components[1] == "size_in_atlas") {
				move_tile_in_atlas(coords, coords, p_value);
				return true;
			} else if (components[1] == "next_alternative_id") {
				tiles[coords].next_alternative_id = p_value;
				return true;
			} else if (components[1] == "animation_columns") {
				set_tile_animation_columns(coords, p_value);
				return true;
			} else if (components[1] == "animation_separation") {
				set_tile_animation_separation(coords, p_value);
				return true;
			} else if (components[1] == "animation_speed") {
				set_tile_animation_speed(coords, p_value);
				return true;
			} else if (components[1] == "animation_mode") {
				set_tile_animation_mode(coords, VariantCaster<LayeredTileSetAtlasSource::TileAnimationMode>::cast(p_value));
				return true;
			} else if (components[1] == "animation_frames_count") {
				set_tile_animation_frames_count(coords, p_value);
				return true;
			} else if (components.size() >= 3 && components[1].begins_with("animation_frame_") && components[1].trim_prefix("animation_frame_").is_valid_integer()) {
				int frame = components[1].trim_prefix("animation_frame_").to_int();
				if (components[2] == "duration") {
					if (frame >= get_tile_animation_frames_count(coords)) {
						set_tile_animation_frames_count(coords, frame + 1);
					}
					set_tile_animation_frame_duration(coords, frame, p_value);
					return true;
				}
				return false;
			} else if (components[1].is_valid_integer()) {
				int alternative_id = components[1].to_int();
				if (alternative_id != LayeredTileSetSource::INVALID_TILE_ALTERNATIVE) {
					// Create the alternative if needed ?
					if (!has_alternative_tile(coords, alternative_id)) {
						create_alternative_tile(coords, alternative_id);
					}
					if (!tiles[coords].alternatives.has(alternative_id)) {
						tiles[coords].alternatives[alternative_id] = memnew(LayeredTileData);
						tiles[coords].alternatives[alternative_id]->set_tile_set(tile_set);
						tiles[coords].alternatives[alternative_id]->set_allow_transform(alternative_id > 0);
						tiles[coords].alternatives_ids.push_back(alternative_id);
					}
					if (components.size() >= 3) {
						bool valid;
						tiles[coords].alternatives[alternative_id]->set(components[2], p_value, &valid);
						return valid;
					} else {
						// Only create the alternative if it did not exist yet.
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool LayeredTileSetAtlasSource::_get(const StringName &p_name, Variant &r_ret) const {
	Vector<String> components = String(p_name).split("/", true, 2);

	// Properties.
	Vector<String> coords_split = components[0].split(":");
	if (coords_split.size() == 2 && coords_split[0].is_valid_integer() && coords_split[1].is_valid_integer()) {
		Vector2i coords = Vector2i(coords_split[0].to_int(), coords_split[1].to_int());
		if (tiles.has(coords)) {
			if (components.size() >= 2) {
				// Properties.
				if (components[1] == "size_in_atlas") {
					r_ret = tiles[coords].size_in_atlas;
					return true;
				} else if (components[1] == "next_alternative_id") {
					r_ret = tiles[coords].next_alternative_id;
					return true;
				} else if (components[1] == "animation_columns") {
					r_ret = get_tile_animation_columns(coords);
					return true;
				} else if (components[1] == "animation_separation") {
					r_ret = get_tile_animation_separation(coords);
					return true;
				} else if (components[1] == "animation_speed") {
					r_ret = get_tile_animation_speed(coords);
					return true;
				} else if (components[1] == "animation_mode") {
					r_ret = get_tile_animation_mode(coords);
					return true;
				} else if (components[1] == "animation_frames_count") {
					r_ret = get_tile_animation_frames_count(coords);
					return true;
				} else if (components.size() >= 3 && components[1].begins_with("animation_frame_") && components[1].trim_prefix("animation_frame_").is_valid_integer()) {
					int frame = components[1].trim_prefix("animation_frame_").to_int();
					if (frame < 0 || frame >= get_tile_animation_frames_count(coords)) {
						return false;
					}
					if (components[2] == "duration") {
						r_ret = get_tile_animation_frame_duration(coords, frame);
						return true;
					}
					return false;
				} else if (components[1].is_valid_integer()) {
					int alternative_id = components[1].to_int();
					if (alternative_id != LayeredTileSetSource::INVALID_TILE_ALTERNATIVE && tiles[coords].alternatives.has(alternative_id)) {
						if (components.size() >= 3) {
							bool valid;
							r_ret = tiles[coords].alternatives[alternative_id]->get(components[2], &valid);
							return valid;
						} else {
							// Only to notify the tile alternative exists.
							r_ret = alternative_id;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void LayeredTileSetAtlasSource::_get_property_list(List<PropertyInfo> *p_list) const {
	// Atlases data.
	PropertyInfo property_info;

	for (const HashMap<Vector2i, TileAlternativesData>::Element *E_tile = tiles.front(); E_tile; E_tile = E_tile->next) {
		List<PropertyInfo> tile_property_list;

		// size_in_atlas
		property_info = PropertyInfo(Variant::VECTOR2I, "size_in_atlas", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().size_in_atlas == Vector2i(1, 1)) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// next_alternative_id
		property_info = PropertyInfo(Variant::INT, "next_alternative_id", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().next_alternative_id == 1) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// animation_columns.
		property_info = PropertyInfo(Variant::INT, "animation_columns", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().animation_columns == 0) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// animation_separation.
		property_info = PropertyInfo(Variant::INT, "animation_separation", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().animation_separation == Vector2i()) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// animation_speed.
		property_info = PropertyInfo(Variant::REAL, "animation_speed", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().animation_speed == 1.0) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// animation_mode.
		property_info = PropertyInfo(Variant::INT, "animation_mode", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
		if (E_tile->value().animation_mode == TILE_ANIMATION_MODE_DEFAULT) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		tile_property_list.push_back(property_info);

		// animation_frames_count.
		tile_property_list.push_back(PropertyInfo(Variant::INT, "animation_frames_count", PROPERTY_HINT_NONE, "", 0));

		// animation_frame_*.
		bool store_durations = tiles[E_tile->key()].animation_frames_durations.size() >= 2;
		for (int i = 0; i < (int)tiles[E_tile->key()].animation_frames_durations.size(); i++) {
			property_info = PropertyInfo(Variant::REAL, vformat("animation_frame_%d/duration", i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR);
			if (!store_durations) {
				property_info.usage ^= PROPERTY_USAGE_STORAGE;
			}
			tile_property_list.push_back(property_info);
		}

		for (const HashMap<int, LayeredTileData *>::Element *E_alternative = E_tile->value().alternatives.front(); E_alternative; E_alternative = E_alternative->next) {
			// Add a dummy property to show the alternative exists.
			tile_property_list.push_back(PropertyInfo(Variant::INT, vformat("%d", E_alternative->key()), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));

			// Get the alternative tile's properties and append them to the list of properties.
			List<PropertyInfo> alternative_property_list;
			E_alternative->value()->get_property_list(&alternative_property_list);

			for (List<PropertyInfo>::Element *alternative_property_info = alternative_property_list.front(); alternative_property_info; alternative_property_info = alternative_property_info->next()) {
				Variant default_value = ClassDB::class_get_default_property_value("LayeredTileData", alternative_property_info->get().name);
				Variant value = E_alternative->value()->get(alternative_property_info->get().name);
				if (default_value.get_type() != Variant::NIL && bool(Variant::evaluate(Variant::OP_EQUAL, value, default_value))) {
					alternative_property_info->get().usage ^= PROPERTY_USAGE_STORAGE;
				}
				alternative_property_info->get().name = vformat("%s/%s", vformat("%d", E_alternative->key()), alternative_property_info->get().name);
				tile_property_list.push_back(alternative_property_info->get());
			}
		}

		// Add all alternative.

		for (List<PropertyInfo>::Element *tile_property_info = tile_property_list.front(); tile_property_info; tile_property_info = tile_property_info->next()) {
			tile_property_info->get().name = vformat("%s/%s", vformat("%d:%d", E_tile->key().x, E_tile->key().y), tile_property_info->get().name);
			p_list->push_back(tile_property_info->get());
		}
	}
}

void LayeredTileSetAtlasSource::create_tile(const Vector2i p_atlas_coords, const Vector2i p_size) {
	// Create a tile if it does not exists.
	ERR_FAIL_COND(p_atlas_coords.x < 0 || p_atlas_coords.y < 0);
	ERR_FAIL_COND(p_size.x <= 0 || p_size.y <= 0);

	bool room_for_tile = has_room_for_tile(p_atlas_coords, p_size, 1, Vector2i(), 1);
	ERR_FAIL_COND_MSG(!room_for_tile, "Cannot create tile. The tile is outside the texture or tiles are already present in the space the tile would cover.");

	// Initialize the tile data.
	TileAlternativesData tad;
	tad.size_in_atlas = p_size;
	tad.animation_frames_durations.push_back(1.0);
	tad.alternatives[0] = memnew(LayeredTileData);
	tad.alternatives[0]->set_tile_set(tile_set);
	tad.alternatives[0]->set_allow_transform(false);
	tad.alternatives[0]->connect("changed", this, "emit_changed");
	//tad.alternatives[0]->property_list_changed_notify();
	tad.alternatives[0]->_change_notify();
	tad.alternatives_ids.push_back(0);

	// Create and resize the tile.
	tiles.insert(p_atlas_coords, tad);
	tiles_ids.push_back(p_atlas_coords);
	tiles_ids.sort();

	_create_coords_mapping_cache(p_atlas_coords);
	_queue_update_padded_texture();

	emit_signal("changed");
}

void LayeredTileSetAtlasSource::remove_tile(Vector2i p_atlas_coords) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));

	// Remove all covered positions from the mapping cache
	_clear_coords_mapping_cache(p_atlas_coords);

	// Free tile data.

	for (const HashMap<int, LayeredTileData *>::Element *E_tile_data = tiles[p_atlas_coords].alternatives.front(); E_tile_data; E_tile_data = E_tile_data->next) {
		memdelete(E_tile_data->value());
	}

	// Delete the tile
	tiles.erase(p_atlas_coords);
	tiles_ids.erase(p_atlas_coords);
	tiles_ids.sort();

	_queue_update_padded_texture();

	emit_signal("changed");
}

bool LayeredTileSetAtlasSource::has_tile(Vector2i p_atlas_coords) const {
	return tiles.has(p_atlas_coords);
}

Vector2i LayeredTileSetAtlasSource::get_tile_at_coords(Vector2i p_atlas_coords) const {
	if (!_coords_mapping_cache.has(p_atlas_coords)) {
		return INVALID_ATLAS_COORDS;
	}

	return _coords_mapping_cache[p_atlas_coords];
}

void LayeredTileSetAtlasSource::set_tile_animation_columns(const Vector2i p_atlas_coords, int p_frame_columns) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_COND(p_frame_columns < 0);

	TileAlternativesData &tad = tiles[p_atlas_coords];
	bool room_for_tile = has_room_for_tile(p_atlas_coords, tad.size_in_atlas, p_frame_columns, tad.animation_separation, tad.animation_frames_durations.size(), p_atlas_coords);
	ERR_FAIL_COND_MSG(!room_for_tile, "Cannot set animation columns count, tiles are already present in the space the tile would cover.");

	_clear_coords_mapping_cache(p_atlas_coords);

	tiles[p_atlas_coords].animation_columns = p_frame_columns;

	_create_coords_mapping_cache(p_atlas_coords);
	_queue_update_padded_texture();

	emit_signal("changed");
}

int LayeredTileSetAtlasSource::get_tile_animation_columns(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), 1, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	return tiles[p_atlas_coords].animation_columns;
}

void LayeredTileSetAtlasSource::set_tile_animation_separation(const Vector2i p_atlas_coords, const Vector2i p_separation) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_COND(p_separation.x < 0 || p_separation.y < 0);

	TileAlternativesData &tad = tiles[p_atlas_coords];
	bool room_for_tile = has_room_for_tile(p_atlas_coords, tad.size_in_atlas, tad.animation_columns, p_separation, tad.animation_frames_durations.size(), p_atlas_coords);
	ERR_FAIL_COND_MSG(!room_for_tile, "Cannot set animation columns count, tiles are already present in the space the tile would cover.");

	_clear_coords_mapping_cache(p_atlas_coords);

	tiles[p_atlas_coords].animation_separation = p_separation;

	_create_coords_mapping_cache(p_atlas_coords);
	_queue_update_padded_texture();

	emit_signal("changed");
}

Vector2i LayeredTileSetAtlasSource::get_tile_animation_separation(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), Vector2i(), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	return tiles[p_atlas_coords].animation_separation;
}

void LayeredTileSetAtlasSource::set_tile_animation_speed(const Vector2i p_atlas_coords, real_t p_speed) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_COND(p_speed <= 0);

	tiles[p_atlas_coords].animation_speed = p_speed;

	emit_signal("changed");
}

real_t LayeredTileSetAtlasSource::get_tile_animation_speed(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), 1.0, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	return tiles[p_atlas_coords].animation_speed;
}

void LayeredTileSetAtlasSource::set_tile_animation_mode(const Vector2i p_atlas_coords, LayeredTileSetAtlasSource::TileAnimationMode p_mode) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));

	tiles[p_atlas_coords].animation_mode = p_mode;

	emit_signal("changed");
}

LayeredTileSetAtlasSource::TileAnimationMode LayeredTileSetAtlasSource::get_tile_animation_mode(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), TILE_ANIMATION_MODE_DEFAULT, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));

	return tiles[p_atlas_coords].animation_mode;
}

void LayeredTileSetAtlasSource::set_tile_animation_frames_count(const Vector2i p_atlas_coords, int p_frames_count) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_COND(p_frames_count < 1);

	int old_size = tiles[p_atlas_coords].animation_frames_durations.size();
	if (p_frames_count == old_size) {
		return;
	}

	TileAlternativesData &tad = tiles[p_atlas_coords];
	bool room_for_tile = has_room_for_tile(p_atlas_coords, tad.size_in_atlas, tad.animation_columns, tad.animation_separation, p_frames_count, p_atlas_coords);
	ERR_FAIL_COND_MSG(!room_for_tile, "Cannot set animation columns count, tiles are already present in the space the tile would cover.");

	_clear_coords_mapping_cache(p_atlas_coords);

	tiles[p_atlas_coords].animation_frames_durations.resize(p_frames_count);
	for (int i = old_size; i < p_frames_count; i++) {
		tiles[p_atlas_coords].animation_frames_durations[i] = 1.0;
	}

	_create_coords_mapping_cache(p_atlas_coords);
	_queue_update_padded_texture();

	property_list_changed_notify();

	emit_signal("changed");
}

int LayeredTileSetAtlasSource::get_tile_animation_frames_count(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), 1, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	return tiles[p_atlas_coords].animation_frames_durations.size();
}

void LayeredTileSetAtlasSource::set_tile_animation_frame_duration(const Vector2i p_atlas_coords, int p_frame_index, real_t p_duration) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_INDEX(p_frame_index, (int)tiles[p_atlas_coords].animation_frames_durations.size());
	ERR_FAIL_COND(p_duration <= 0.0);

	tiles[p_atlas_coords].animation_frames_durations[p_frame_index] = p_duration;

	emit_signal("changed");
}

real_t LayeredTileSetAtlasSource::get_tile_animation_frame_duration(const Vector2i p_atlas_coords, int p_frame_index) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), 1, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	ERR_FAIL_INDEX_V(p_frame_index, (int)tiles[p_atlas_coords].animation_frames_durations.size(), 0.0);
	return tiles[p_atlas_coords].animation_frames_durations[p_frame_index];
}

real_t LayeredTileSetAtlasSource::get_tile_animation_total_duration(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), 1, vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));

	real_t sum = 0.0;
	for (const real_t &duration : tiles[p_atlas_coords].animation_frames_durations) {
		sum += duration;
	}
	return sum;
}

Vector2i LayeredTileSetAtlasSource::get_tile_size_in_atlas(Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), Vector2i(-1, -1), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));

	return tiles[p_atlas_coords].size_in_atlas;
}

int LayeredTileSetAtlasSource::get_tiles_count() const {
	return tiles_ids.size();
}

Vector2i LayeredTileSetAtlasSource::get_tile_id(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, tiles_ids.size(), LayeredTileSetSource::INVALID_ATLAS_COORDS);
	return tiles_ids[p_index];
}

bool LayeredTileSetAtlasSource::has_room_for_tile(Vector2i p_atlas_coords, Vector2i p_size, int p_animation_columns, Vector2i p_animation_separation, int p_frames_count, Vector2i p_ignored_tile) const {
	if (p_atlas_coords.x < 0 || p_atlas_coords.y < 0) {
		return false;
	}
	if (p_size.x <= 0 || p_size.y <= 0) {
		return false;
	}
	if (p_frames_count <= 0) {
		return false;
	}
	Size2i atlas_grid_size = get_atlas_grid_size();
	for (int frame = 0; frame < p_frames_count; frame++) {
		Vector2i frame_coords = p_atlas_coords + (p_size + p_animation_separation) * ((p_animation_columns > 0) ? Vector2i(frame % p_animation_columns, frame / p_animation_columns) : Vector2i(frame, 0));
		for (int x = 0; x < p_size.x; x++) {
			for (int y = 0; y < p_size.y; y++) {
				Vector2i coords = frame_coords + Vector2i(x, y);
				if (_coords_mapping_cache.has(coords) && _coords_mapping_cache[coords] != p_ignored_tile) {
					return false;
				}
				if (coords.x >= atlas_grid_size.x || coords.y >= atlas_grid_size.y) {
					return false;
				}
			}
		}
	}
	return true;
}

bool LayeredTileSetAtlasSource::has_tiles_outside_texture() const {
	for (const KeyValue<Vector2i, LayeredTileSetAtlasSource::TileAlternativesData> &E : tiles) {
		if (!has_room_for_tile(E.key, E.value.size_in_atlas, E.value.animation_columns, E.value.animation_separation, E.value.animation_frames_durations.size(), E.key)) {
			return true;
		}
	}
	return false;
}

Vector<Vector2i> LayeredTileSetAtlasSource::get_tiles_outside_texture() const {
	Vector<Vector2i> to_return;

	for (const KeyValue<Vector2i, LayeredTileSetAtlasSource::TileAlternativesData> &E : tiles) {
		if (!has_room_for_tile(E.key, E.value.size_in_atlas, E.value.animation_columns, E.value.animation_separation, E.value.animation_frames_durations.size(), E.key)) {
			to_return.push_back(E.key);
		}
	}
	return to_return;
}

void LayeredTileSetAtlasSource::clear_tiles_outside_texture() {
	LocalVector<Vector2i> to_remove;

	for (const KeyValue<Vector2i, LayeredTileSetAtlasSource::TileAlternativesData> &E : tiles) {
		if (!has_room_for_tile(E.key, E.value.size_in_atlas, E.value.animation_columns, E.value.animation_separation, E.value.animation_frames_durations.size(), E.key)) {
			to_remove.push_back(E.key);
		}
	}

	for (const Vector2i &v : to_remove) {
		remove_tile(v);
	}
}

PoolVector2Array LayeredTileSetAtlasSource::get_tiles_to_be_removed_on_change(Ref<Texture> p_texture, Vector2i p_margins, Vector2i p_separation, Vector2i p_texture_region_size) {
	ERR_FAIL_COND_V(p_margins.x < 0 || p_margins.y < 0, PoolVector2Array());
	ERR_FAIL_COND_V(p_separation.x < 0 || p_separation.y < 0, PoolVector2Array());
	ERR_FAIL_COND_V(p_texture_region_size.x <= 0 || p_texture_region_size.y <= 0, PoolVector2Array());

	// Compute the new atlas grid size.
	Size2 new_grid_size;
	if (p_texture.is_valid()) {
		Size2i valid_area = p_texture->get_size() - p_margins;

		// Compute the number of valid tiles in the tiles atlas
		if (valid_area.x >= p_texture_region_size.x && valid_area.y >= p_texture_region_size.y) {
			valid_area -= p_texture_region_size;
			new_grid_size = Size2i(1, 1) + valid_area / (p_texture_region_size + p_separation);
		}
	}

	Vector<Vector2> output;
	for (KeyValue<Vector2i, TileAlternativesData> &E : tiles) {
		for (unsigned int frame = 0; frame < E.value.animation_frames_durations.size(); frame++) {
			Vector2i frame_coords = E.key + (E.value.size_in_atlas + E.value.animation_separation) * ((E.value.animation_columns > 0) ? Vector2i(frame % E.value.animation_columns, frame / E.value.animation_columns) : Vector2i(frame, 0));
			frame_coords += E.value.size_in_atlas;
			if (frame_coords.x > new_grid_size.x || frame_coords.y > new_grid_size.y) {
				output.push_back(E.key);
				break;
			}
		}
	}
	return output;
}

Rect2i LayeredTileSetAtlasSource::get_tile_texture_region(Vector2i p_atlas_coords, int p_frame) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), Rect2i(), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_INDEX_V(p_frame, (int)tiles[p_atlas_coords].animation_frames_durations.size(), Rect2i());

	const TileAlternativesData &tad = tiles[p_atlas_coords];

	Vector2i size_in_atlas = tad.size_in_atlas;
	Vector2 region_size = texture_region_size * size_in_atlas + separation * (size_in_atlas - Vector2i(1, 1));

	Vector2i frame_coords = p_atlas_coords + (size_in_atlas + tad.animation_separation) * ((tad.animation_columns > 0) ? Vector2i(p_frame % tad.animation_columns, p_frame / tad.animation_columns) : Vector2i(p_frame, 0));
	Vector2 origin = margins + (frame_coords * (texture_region_size + separation));

	return Rect2(origin, region_size);
}

bool LayeredTileSetAtlasSource::is_position_in_tile_texture_region(const Vector2i p_atlas_coords, int p_alternative_tile, Vector2 p_position) const {
	Size2 size = get_tile_texture_region(p_atlas_coords).size;
	Rect2 rect = Rect2(-size / 2 - get_tile_data(p_atlas_coords, p_alternative_tile)->get_texture_origin(), size);

	return rect.has_point(p_position);
}

int LayeredTileSetAtlasSource::alternative_no_transform(int p_alternative_id) {
	return p_alternative_id & ~(TRANSFORM_FLIP_H | TRANSFORM_FLIP_V | TRANSFORM_TRANSPOSE);
}

// Getters for texture and tile region (padded or not)
Ref<Texture> LayeredTileSetAtlasSource::get_runtime_texture() const {
	if (use_texture_padding) {
		return padded_texture;
	} else {
		return texture;
	}
}

Rect2i LayeredTileSetAtlasSource::get_runtime_tile_texture_region(Vector2i p_atlas_coords, int p_frame) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), Rect2i(), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_INDEX_V(p_frame, (int)tiles[p_atlas_coords].animation_frames_durations.size(), Rect2i());

	Rect2i src_rect = get_tile_texture_region(p_atlas_coords, p_frame);
	if (use_texture_padding) {
		const TileAlternativesData &tad = tiles[p_atlas_coords];
		Vector2i frame_coords = p_atlas_coords + (tad.size_in_atlas + tad.animation_separation) * ((tad.animation_columns > 0) ? Vector2i(p_frame % tad.animation_columns, p_frame / tad.animation_columns) : Vector2i(p_frame, 0));
		Vector2i base_pos = frame_coords * (texture_region_size + Vector2i(2, 2)) + Vector2i(1, 1);

		return Rect2i(base_pos, src_rect.size);
	} else {
		return src_rect;
	}
}

void LayeredTileSetAtlasSource::move_tile_in_atlas(Vector2i p_atlas_coords, Vector2i p_new_atlas_coords, Vector2i p_new_size) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));

	TileAlternativesData &tad = tiles[p_atlas_coords];

	// Compute the actual new rect from arguments.
	Vector2i new_atlas_coords = (p_new_atlas_coords != INVALID_ATLAS_COORDS) ? p_new_atlas_coords : p_atlas_coords;
	Vector2i new_size = (p_new_size != Vector2i(-1, -1)) ? p_new_size : tad.size_in_atlas;

	if (new_atlas_coords == p_atlas_coords && new_size == tad.size_in_atlas) {
		return;
	}

	bool room_for_tile = has_room_for_tile(new_atlas_coords, new_size, tad.animation_columns, tad.animation_separation, tad.animation_frames_durations.size(), p_atlas_coords);
	ERR_FAIL_COND_MSG(!room_for_tile, vformat("Cannot move tile at position %s with size %s. Tile already present.", new_atlas_coords, new_size));

	_clear_coords_mapping_cache(p_atlas_coords);

	// Move the tile and update its size.
	if (new_atlas_coords != p_atlas_coords) {
		tiles[new_atlas_coords] = tiles[p_atlas_coords];
		tiles.erase(p_atlas_coords);

		tiles_ids.erase(p_atlas_coords);
		tiles_ids.push_back(new_atlas_coords);
		tiles_ids.sort();
	}
	tiles[new_atlas_coords].size_in_atlas = new_size;

	_create_coords_mapping_cache(new_atlas_coords);
	_queue_update_padded_texture();

	emit_signal("changed");
}

int LayeredTileSetAtlasSource::create_alternative_tile(const Vector2i p_atlas_coords, int p_alternative_id_override) {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE, vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_COND_V_MSG(p_alternative_id_override >= 0 && tiles[p_atlas_coords].alternatives.has(p_alternative_id_override), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE, vformat("Cannot create alternative tile. Another alternative exists with id %d.", p_alternative_id_override));

	int new_alternative_id = p_alternative_id_override >= 0 ? p_alternative_id_override : tiles[p_atlas_coords].next_alternative_id;

	tiles[p_atlas_coords].alternatives[new_alternative_id] = memnew(LayeredTileData);
	tiles[p_atlas_coords].alternatives[new_alternative_id]->set_tile_set(tile_set);
	tiles[p_atlas_coords].alternatives[new_alternative_id]->set_allow_transform(true);
	tiles[p_atlas_coords].alternatives[new_alternative_id]->connect("changed", callable_mp((Resource *)this, &LayeredTileSetAtlasSource::emit_changed));
	//tiles[p_atlas_coords].alternatives[new_alternative_id]->property_list_changed_notify();
	tiles[p_atlas_coords].alternatives[new_alternative_id]->_change_notify();
	tiles[p_atlas_coords].alternatives_ids.push_back(new_alternative_id);
	tiles[p_atlas_coords].alternatives_ids.sort();
	_compute_next_alternative_id(p_atlas_coords);

	emit_signal("changed");

	return new_alternative_id;
}

void LayeredTileSetAtlasSource::remove_alternative_tile(const Vector2i p_atlas_coords, int p_alternative_tile) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_COND_MSG(!tiles[p_atlas_coords].alternatives.has(p_alternative_tile), vformat("LayeredTileSetAtlasSource has no alternative with id %d for tile coords %s.", p_alternative_tile, String(p_atlas_coords)));
	p_alternative_tile = alternative_no_transform(p_alternative_tile);
	ERR_FAIL_COND_MSG(p_alternative_tile == 0, "Cannot remove the alternative with id 0, the base tile alternative cannot be removed.");

	memdelete(tiles[p_atlas_coords].alternatives[p_alternative_tile]);
	tiles[p_atlas_coords].alternatives.erase(p_alternative_tile);
	tiles[p_atlas_coords].alternatives_ids.erase(p_alternative_tile);
	tiles[p_atlas_coords].alternatives_ids.sort();

	emit_signal("changed");
}

void LayeredTileSetAtlasSource::set_alternative_tile_id(const Vector2i p_atlas_coords, int p_alternative_tile, int p_new_id) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_COND_MSG(!tiles[p_atlas_coords].alternatives.has(p_alternative_tile), vformat("LayeredTileSetAtlasSource has no alternative with id %d for tile coords %s.", p_alternative_tile, String(p_atlas_coords)));
	p_alternative_tile = alternative_no_transform(p_alternative_tile);
	ERR_FAIL_COND_MSG(p_alternative_tile == 0, "Cannot change the alternative with id 0, the base tile alternative cannot be modified.");

	ERR_FAIL_COND_MSG(tiles[p_atlas_coords].alternatives.has(p_new_id), vformat("LayeredTileSetAtlasSource has already an alternative with id %d at %s.", p_new_id, String(p_atlas_coords)));

	tiles[p_atlas_coords].alternatives[p_new_id] = tiles[p_atlas_coords].alternatives[p_alternative_tile];
	tiles[p_atlas_coords].alternatives_ids.push_back(p_new_id);

	tiles[p_atlas_coords].alternatives.erase(p_alternative_tile);
	tiles[p_atlas_coords].alternatives_ids.erase(p_alternative_tile);
	tiles[p_atlas_coords].alternatives_ids.sort();

	emit_signal("changed");
}

bool LayeredTileSetAtlasSource::has_alternative_tile(const Vector2i p_atlas_coords, int p_alternative_tile) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), false, vformat("The LayeredTileSetAtlasSource atlas has no tile at %s.", String(p_atlas_coords)));
	return tiles[p_atlas_coords].alternatives.has(alternative_no_transform(p_alternative_tile));
}

int LayeredTileSetAtlasSource::get_next_alternative_tile_id(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE, vformat("The LayeredTileSetAtlasSource atlas has no tile at %s.", String(p_atlas_coords)));
	return tiles[p_atlas_coords].next_alternative_id;
}

int LayeredTileSetAtlasSource::get_alternative_tiles_count(const Vector2i p_atlas_coords) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), -1, vformat("The LayeredTileSetAtlasSource atlas has no tile at %s.", String(p_atlas_coords)));
	return tiles[p_atlas_coords].alternatives_ids.size();
}

int LayeredTileSetAtlasSource::get_alternative_tile_id(const Vector2i p_atlas_coords, int p_index) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE, vformat("The LayeredTileSetAtlasSource atlas has no tile at %s.", String(p_atlas_coords)));
	p_index = alternative_no_transform(p_index);
	ERR_FAIL_INDEX_V(p_index, tiles[p_atlas_coords].alternatives_ids.size(), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);

	return tiles[p_atlas_coords].alternatives_ids[p_index];
}

LayeredTileData *LayeredTileSetAtlasSource::get_tile_data(const Vector2i p_atlas_coords, int p_alternative_tile) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), nullptr, vformat("The LayeredTileSetAtlasSource atlas has no tile at %s.", String(p_atlas_coords)));
	p_alternative_tile = alternative_no_transform(p_alternative_tile);
	ERR_FAIL_COND_V_MSG(!tiles[p_atlas_coords].alternatives.has(p_alternative_tile), nullptr, vformat("LayeredTileSetAtlasSource has no alternative with id %d for tile coords %s.", p_alternative_tile, String(p_atlas_coords)));

	return tiles[p_atlas_coords].alternatives[p_alternative_tile];
}

void LayeredTileSetAtlasSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &LayeredTileSetAtlasSource::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &LayeredTileSetAtlasSource::get_texture);
	ClassDB::bind_method(D_METHOD("set_margins", "margins"), &LayeredTileSetAtlasSource::set_margins);
	ClassDB::bind_method(D_METHOD("get_margins"), &LayeredTileSetAtlasSource::get_margins);
	ClassDB::bind_method(D_METHOD("set_separation", "separation"), &LayeredTileSetAtlasSource::set_separation);
	ClassDB::bind_method(D_METHOD("get_separation"), &LayeredTileSetAtlasSource::get_separation);
	ClassDB::bind_method(D_METHOD("set_texture_region_size", "texture_region_size"), &LayeredTileSetAtlasSource::set_texture_region_size);
	ClassDB::bind_method(D_METHOD("get_texture_region_size"), &LayeredTileSetAtlasSource::get_texture_region_size);
	ClassDB::bind_method(D_METHOD("set_use_texture_padding", "use_texture_padding"), &LayeredTileSetAtlasSource::set_use_texture_padding);
	ClassDB::bind_method(D_METHOD("get_use_texture_padding"), &LayeredTileSetAtlasSource::get_use_texture_padding);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D", PROPERTY_USAGE_NOEDITOR), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "margins", PROPERTY_HINT_NONE, "suffix:px", PROPERTY_USAGE_NOEDITOR), "set_margins", "get_margins");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "separation", PROPERTY_HINT_NONE, "suffix:px", PROPERTY_USAGE_NOEDITOR), "set_separation", "get_separation");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "texture_region_size", PROPERTY_HINT_NONE, "suffix:px", PROPERTY_USAGE_NOEDITOR), "set_texture_region_size", "get_texture_region_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_texture_padding", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_use_texture_padding", "get_use_texture_padding");

	// Base tiles
	ClassDB::bind_method(D_METHOD("create_tile", "atlas_coords", "size"), &LayeredTileSetAtlasSource::create_tile, DEFVAL(Vector2i(1, 1)));
	ClassDB::bind_method(D_METHOD("remove_tile", "atlas_coords"), &LayeredTileSetAtlasSource::remove_tile); // Remove a tile. If p_tile_key.alternative_tile if different from 0, remove the alternative
	ClassDB::bind_method(D_METHOD("move_tile_in_atlas", "atlas_coords", "new_atlas_coords", "new_size"), &LayeredTileSetAtlasSource::move_tile_in_atlas, DEFVAL(INVALID_ATLAS_COORDS), DEFVAL(Vector2i(-1, -1)));
	ClassDB::bind_method(D_METHOD("get_tile_size_in_atlas", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_size_in_atlas);

	ClassDB::bind_method(D_METHOD("has_room_for_tile", "atlas_coords", "size", "animation_columns", "animation_separation", "frames_count", "ignored_tile"), &LayeredTileSetAtlasSource::has_room_for_tile, DEFVAL(INVALID_ATLAS_COORDS));
	ClassDB::bind_method(D_METHOD("get_tiles_to_be_removed_on_change", "texture", "margins", "separation", "texture_region_size"), &LayeredTileSetAtlasSource::get_tiles_to_be_removed_on_change);
	ClassDB::bind_method(D_METHOD("get_tile_at_coords", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_at_coords);

	ClassDB::bind_method(D_METHOD("has_tiles_outside_texture"), &LayeredTileSetAtlasSource::has_tiles_outside_texture);
	ClassDB::bind_method(D_METHOD("clear_tiles_outside_texture"), &LayeredTileSetAtlasSource::clear_tiles_outside_texture);

	ClassDB::bind_method(D_METHOD("set_tile_animation_columns", "atlas_coords", "frame_columns"), &LayeredTileSetAtlasSource::set_tile_animation_columns);
	ClassDB::bind_method(D_METHOD("get_tile_animation_columns", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_columns);
	ClassDB::bind_method(D_METHOD("set_tile_animation_separation", "atlas_coords", "separation"), &LayeredTileSetAtlasSource::set_tile_animation_separation);
	ClassDB::bind_method(D_METHOD("get_tile_animation_separation", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_separation);
	ClassDB::bind_method(D_METHOD("set_tile_animation_speed", "atlas_coords", "speed"), &LayeredTileSetAtlasSource::set_tile_animation_speed);
	ClassDB::bind_method(D_METHOD("get_tile_animation_speed", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_speed);
	ClassDB::bind_method(D_METHOD("set_tile_animation_mode", "atlas_coords", "mode"), &LayeredTileSetAtlasSource::set_tile_animation_mode);
	ClassDB::bind_method(D_METHOD("get_tile_animation_mode", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_mode);
	ClassDB::bind_method(D_METHOD("set_tile_animation_frames_count", "atlas_coords", "frames_count"), &LayeredTileSetAtlasSource::set_tile_animation_frames_count);
	ClassDB::bind_method(D_METHOD("get_tile_animation_frames_count", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_frames_count);
	ClassDB::bind_method(D_METHOD("set_tile_animation_frame_duration", "atlas_coords", "frame_index", "duration"), &LayeredTileSetAtlasSource::set_tile_animation_frame_duration);
	ClassDB::bind_method(D_METHOD("get_tile_animation_frame_duration", "atlas_coords", "frame_index"), &LayeredTileSetAtlasSource::get_tile_animation_frame_duration);
	ClassDB::bind_method(D_METHOD("get_tile_animation_total_duration", "atlas_coords"), &LayeredTileSetAtlasSource::get_tile_animation_total_duration);

	// Alternative tiles
	ClassDB::bind_method(D_METHOD("create_alternative_tile", "atlas_coords", "alternative_id_override"), &LayeredTileSetAtlasSource::create_alternative_tile, DEFVAL(INVALID_TILE_ALTERNATIVE));
	ClassDB::bind_method(D_METHOD("remove_alternative_tile", "atlas_coords", "alternative_tile"), &LayeredTileSetAtlasSource::remove_alternative_tile);
	ClassDB::bind_method(D_METHOD("set_alternative_tile_id", "atlas_coords", "alternative_tile", "new_id"), &LayeredTileSetAtlasSource::set_alternative_tile_id);
	ClassDB::bind_method(D_METHOD("get_next_alternative_tile_id", "atlas_coords"), &LayeredTileSetAtlasSource::get_next_alternative_tile_id);

	ClassDB::bind_method(D_METHOD("get_tile_data", "atlas_coords", "alternative_tile"), &LayeredTileSetAtlasSource::get_tile_data);

	// Helpers.
	ClassDB::bind_method(D_METHOD("get_atlas_grid_size"), &LayeredTileSetAtlasSource::get_atlas_grid_size);
	ClassDB::bind_method(D_METHOD("get_tile_texture_region", "atlas_coords", "frame"), &LayeredTileSetAtlasSource::get_tile_texture_region, DEFVAL(0));

	// Getters for texture and tile region (padded or not)
	ClassDB::bind_method(D_METHOD("get_runtime_texture"), &LayeredTileSetAtlasSource::get_runtime_texture);
	ClassDB::bind_method(D_METHOD("get_runtime_tile_texture_region", "atlas_coords", "frame"), &LayeredTileSetAtlasSource::get_runtime_tile_texture_region);

	ClassDB::bind_method(D_METHOD("_queue_update_padded_texture"), &LayeredTileSetAtlasSource::_queue_update_padded_texture);

	BIND_ENUM_CONSTANT(TILE_ANIMATION_MODE_DEFAULT)
	BIND_ENUM_CONSTANT(TILE_ANIMATION_MODE_RANDOM_START_TIMES)
	BIND_ENUM_CONSTANT(TILE_ANIMATION_MODE_MAX)

	BIND_CONSTANT(TRANSFORM_FLIP_H)
	BIND_CONSTANT(TRANSFORM_FLIP_V)
	BIND_CONSTANT(TRANSFORM_TRANSPOSE)
}

LayeredTileSetAtlasSource::~LayeredTileSetAtlasSource() {
	// Free everything needed.
	for (KeyValue<Vector2i, TileAlternativesData> &E_alternatives : tiles) {
		for (KeyValue<int, TileData *> &E_tile_data : E_alternatives.value.alternatives) {
			memdelete(E_tile_data.value);
		}
	}
}

TileData *LayeredTileSetAtlasSource::_get_atlas_tile_data(Vector2i p_atlas_coords, int p_alternative_tile) {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), nullptr, vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	p_alternative_tile = alternative_no_transform(p_alternative_tile);
	ERR_FAIL_COND_V_MSG(!tiles[p_atlas_coords].alternatives.has(p_alternative_tile), nullptr, vformat("LayeredTileSetAtlasSource has no alternative with id %d for tile coords %s.", p_alternative_tile, String(p_atlas_coords)));

	return tiles[p_atlas_coords].alternatives[p_alternative_tile];
}

const TileData *LayeredTileSetAtlasSource::_get_atlas_tile_data(Vector2i p_atlas_coords, int p_alternative_tile) const {
	ERR_FAIL_COND_V_MSG(!tiles.has(p_atlas_coords), nullptr, vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));
	ERR_FAIL_COND_V_MSG(!tiles[p_atlas_coords].alternatives.has(p_alternative_tile), nullptr, vformat("LayeredTileSetAtlasSource has no alternative with id %d for tile coords %s.", p_alternative_tile, String(p_atlas_coords)));

	return tiles[p_atlas_coords].alternatives[p_alternative_tile];
}

void LayeredTileSetAtlasSource::_compute_next_alternative_id(const Vector2i p_atlas_coords) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", String(p_atlas_coords)));

	while (tiles[p_atlas_coords].alternatives.has(tiles[p_atlas_coords].next_alternative_id)) {
		tiles[p_atlas_coords].next_alternative_id = (tiles[p_atlas_coords].next_alternative_id % 1073741823) + 1; // 2 ** 30
	};
}

void LayeredTileSetAtlasSource::_clear_coords_mapping_cache(Vector2i p_atlas_coords) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));
	TileAlternativesData &tad = tiles[p_atlas_coords];
	for (int frame = 0; frame < (int)tad.animation_frames_durations.size(); frame++) {
		Vector2i frame_coords = p_atlas_coords + (tad.size_in_atlas + tad.animation_separation) * ((tad.animation_columns > 0) ? Vector2i(frame % tad.animation_columns, frame / tad.animation_columns) : Vector2i(frame, 0));
		for (int x = 0; x < tad.size_in_atlas.x; x++) {
			for (int y = 0; y < tad.size_in_atlas.y; y++) {
				Vector2i coords = frame_coords + Vector2i(x, y);
				if (!_coords_mapping_cache.has(coords)) {
					WARN_PRINT(vformat("LayeredTileSetAtlasSource has no cached tile at position %s, the position cache might be corrupted.", coords));
				} else {
					if (_coords_mapping_cache[coords] != p_atlas_coords) {
						WARN_PRINT(vformat("The position cache at position %s is pointing to a wrong tile, the position cache might be corrupted.", coords));
					}
					_coords_mapping_cache.erase(coords);
				}
			}
		}
	}
}

void LayeredTileSetAtlasSource::_create_coords_mapping_cache(Vector2i p_atlas_coords) {
	ERR_FAIL_COND_MSG(!tiles.has(p_atlas_coords), vformat("LayeredTileSetAtlasSource has no tile at %s.", Vector2i(p_atlas_coords)));

	TileAlternativesData &tad = tiles[p_atlas_coords];
	for (int frame = 0; frame < (int)tad.animation_frames_durations.size(); frame++) {
		Vector2i frame_coords = p_atlas_coords + (tad.size_in_atlas + tad.animation_separation) * ((tad.animation_columns > 0) ? Vector2i(frame % tad.animation_columns, frame / tad.animation_columns) : Vector2i(frame, 0));
		for (int x = 0; x < tad.size_in_atlas.x; x++) {
			for (int y = 0; y < tad.size_in_atlas.y; y++) {
				Vector2i coords = frame_coords + Vector2i(x, y);
				if (_coords_mapping_cache.has(coords)) {
					WARN_PRINT(vformat("The cache already has a tile for position %s, the position cache might be corrupted.", coords));
				}
				_coords_mapping_cache[coords] = p_atlas_coords;
			}
		}
	}
}

void LayeredTileSetAtlasSource::_queue_update_padded_texture() {
	padded_texture_needs_update = true;
	callable_mp(this, &LayeredTileSetAtlasSource::_update_padded_texture).call_deferred();
}

Ref<ImageTexture> LayeredTileSetAtlasSource::_create_padded_image_texture(const Ref<Texture> &p_source) {
	ERR_FAIL_COND_V(p_source.is_null(), Ref<ImageTexture>());

	Ref<Image> src_image = p_source->get_data();
	if (src_image.is_null()) {
		Ref<ImageTexture> ret;
		ret.instance();
		return ret;
	}

	Size2 size = get_atlas_grid_size() * (texture_region_size + Vector2i(2, 2));
	Ref<Image> image = Image::create_empty(size.x, size.y, false, src_image->get_format());

	for (KeyValue<Vector2i, TileAlternativesData> kv : tiles) {
		for (int frame = 0; frame < (int)kv.value.animation_frames_durations.size(); frame++) {
			// Compute the source rects.
			Rect2i src_rect = get_tile_texture_region(kv.key, frame);

			Rect2i top_src_rect = Rect2i(src_rect.position, Vector2i(src_rect.size.x, 1));
			Rect2i bottom_src_rect = Rect2i(src_rect.position + Vector2i(0, src_rect.size.y - 1), Vector2i(src_rect.size.x, 1));
			Rect2i left_src_rect = Rect2i(src_rect.position, Vector2i(1, src_rect.size.y));
			Rect2i right_src_rect = Rect2i(src_rect.position + Vector2i(src_rect.size.x - 1, 0), Vector2i(1, src_rect.size.y));

			// Copy the tile and the paddings.
			Vector2i frame_coords = kv.key + (kv.value.size_in_atlas + kv.value.animation_separation) * ((kv.value.animation_columns > 0) ? Vector2i(frame % kv.value.animation_columns, frame / kv.value.animation_columns) : Vector2i(frame, 0));
			Vector2i base_pos = frame_coords * (texture_region_size + Vector2i(2, 2)) + Vector2i(1, 1);

			image->blit_rect(*src_image, src_rect, base_pos);

			// Sides
			image->blit_rect(*src_image, top_src_rect, base_pos + Vector2i(0, -1));
			image->blit_rect(*src_image, bottom_src_rect, base_pos + Vector2i(0, src_rect.size.y));
			image->blit_rect(*src_image, left_src_rect, base_pos + Vector2i(-1, 0));
			image->blit_rect(*src_image, right_src_rect, base_pos + Vector2i(src_rect.size.x, 0));

			// Corners
			image->blit_rect(*src_image, Rect2i(src_rect.position, Vector2i(1, 1)), base_pos + Vector2i(-1, -1));
			image->blit_rect(*src_image, Rect2i(src_rect.position + Vector2i(src_rect.size.x - 1, 0), Vector2i(1, 1)), base_pos + Vector2i(src_rect.size.x, -1));
			image->blit_rect(*src_image, Rect2i(src_rect.position + Vector2i(0, src_rect.size.y - 1), Vector2i(1, 1)), base_pos + Vector2i(-1, src_rect.size.y));
			image->blit_rect(*src_image, Rect2i(src_rect.position + Vector2i(src_rect.size.x - 1, src_rect.size.y - 1), Vector2i(1, 1)), base_pos + Vector2i(src_rect.size.x, src_rect.size.y));
		}
	}

	return ImageTexture::create_from_image(image);
}

void LayeredTileSetAtlasSource::_update_padded_texture() {
	if (!padded_texture_needs_update) {
		return;
	}
	padded_texture_needs_update = false;

	if (padded_texture.is_valid()) {
		padded_texture->disconnect(CoreStringNames::get_singleton()->changed, callable_mp(this, &LayeredTileSetAtlasSource::_queue_update_padded_texture));
	}

	padded_texture = Ref<Texture>();

	if (texture.is_null()) {
		return;
	}

	if (!use_texture_padding) {
		return;
	}

	padded_texture.instance();

	Ref<Texture> src_canvas_texture = texture;
	if (src_canvas_texture.is_valid()) {
		// Use all textures.
		// Diffuse
		Ref<Texture> src = src_canvas_texture->get_diffuse_texture();
		Ref<ImageTexture> image_texture;
		if (src.is_valid()) {
			padded_texture->set_diffuse_texture(_create_padded_image_texture(src));
		}

		// Normal
		src = src_canvas_texture->get_normal_texture();
		if (src.is_valid()) {
			padded_texture->set_normal_texture(_create_padded_image_texture(src));
		}

		// Specular
		src = src_canvas_texture->get_specular_texture();
		if (src.is_valid()) {
			padded_texture->set_specular_texture(_create_padded_image_texture(src));
		}

		// Other properties.
		padded_texture->set_specular_color(src_canvas_texture->get_specular_color());
		padded_texture->set_specular_shininess(src_canvas_texture->get_specular_shininess());
		padded_texture->set_texture_filter(src_canvas_texture->get_texture_filter());
		padded_texture->set_texture_repeat(src_canvas_texture->get_texture_repeat());
	} else {
		// Use only diffuse.
		Ref<ImageTexture> image_texture = _create_padded_image_texture(texture);
		padded_texture->set_diffuse_texture(image_texture);
	}
	padded_texture->connect(CoreStringNames::get_singleton()->changed, callable_mp(this, &LayeredTileSetAtlasSource::_queue_update_padded_texture));
	emit_changed();
}

/////////////////////////////// LayeredTileSetScenesCollectionSource //////////////////////////////////////

void LayeredTileSetScenesCollectionSource::_compute_next_alternative_id() {
	while (scenes.has(next_scene_id)) {
		next_scene_id = (next_scene_id % 1073741823) + 1; // 2 ** 30
	};
}

int LayeredTileSetScenesCollectionSource::get_tiles_count() const {
	return 1;
}

Vector2i LayeredTileSetScenesCollectionSource::get_tile_id(int p_tile_index) const {
	ERR_FAIL_COND_V(p_tile_index != 0, LayeredTileSetSource::INVALID_ATLAS_COORDS);
	return Vector2i();
}

bool LayeredTileSetScenesCollectionSource::has_tile(Vector2i p_atlas_coords) const {
	return p_atlas_coords == Vector2i();
}

int LayeredTileSetScenesCollectionSource::get_alternative_tiles_count(const Vector2i p_atlas_coords) const {
	return scenes_ids.size();
}

int LayeredTileSetScenesCollectionSource::get_alternative_tile_id(const Vector2i p_atlas_coords, int p_index) const {
	ERR_FAIL_COND_V(p_atlas_coords != Vector2i(), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);
	ERR_FAIL_INDEX_V(p_index, scenes_ids.size(), LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);

	return scenes_ids[p_index];
}

bool LayeredTileSetScenesCollectionSource::has_alternative_tile(const Vector2i p_atlas_coords, int p_alternative_tile) const {
	ERR_FAIL_COND_V(p_atlas_coords != Vector2i(), false);
	return scenes.has(p_alternative_tile);
}

int LayeredTileSetScenesCollectionSource::create_scene_tile(Ref<PackedScene> p_packed_scene, int p_id_override) {
	ERR_FAIL_COND_V_MSG(p_id_override >= 0 && scenes.has(p_id_override), INVALID_TILE_ALTERNATIVE, vformat("Cannot create scene tile. Another scene tile exists with id %d.", p_id_override));

	int new_scene_id = p_id_override >= 0 ? p_id_override : next_scene_id;

	scenes[new_scene_id] = SceneData();
	scenes_ids.push_back(new_scene_id);
	scenes_ids.sort();
	set_scene_tile_scene(new_scene_id, p_packed_scene);
	_compute_next_alternative_id();

	emit_signal("changed");

	return new_scene_id;
}

void LayeredTileSetScenesCollectionSource::set_scene_tile_id(int p_id, int p_new_id) {
	ERR_FAIL_COND(p_new_id < 0);
	ERR_FAIL_COND(!has_scene_tile_id(p_id));
	ERR_FAIL_COND(has_scene_tile_id(p_new_id));

	scenes[p_new_id] = SceneData();
	scenes[p_new_id] = scenes[p_id];
	scenes_ids.push_back(p_new_id);
	scenes_ids.sort();

	_compute_next_alternative_id();

	scenes.erase(p_id);
	scenes_ids.erase(p_id);

	emit_signal("changed");
}

void LayeredTileSetScenesCollectionSource::set_scene_tile_scene(int p_id, Ref<PackedScene> p_packed_scene) {
	ERR_FAIL_COND(!scenes.has(p_id));
	if (p_packed_scene.is_valid()) {
		// Check if it extends CanvasItem.
		Ref<SceneState> scene_state = p_packed_scene->get_state();
		String type;
		while (scene_state.is_valid() && type.empty()) {
			// Make sure we have a root node. Supposed to be at 0 index because find_node_by_path() does not seem to work.
			ERR_FAIL_COND(scene_state->get_node_count() < 1);

			type = scene_state->get_node_type(0);
			scene_state = scene_state->get_base_scene_state();
		}
		ERR_FAIL_COND_EDMSG(type.empty(), vformat("Invalid PackedScene for LayeredTileSetScenesCollectionSource: %s. Could not get the type of the root node.", p_packed_scene->get_path()));
		bool extends_correct_class = ClassDB::is_parent_class(type, "CanvasItem");
		ERR_FAIL_COND_EDMSG(!extends_correct_class, vformat("Invalid PackedScene for LayeredTileSetScenesCollectionSource: %s. Root node should extend CanvasItem. Found %s instead.", p_packed_scene->get_path(), type));

		scenes[p_id].scene = p_packed_scene;
	} else {
		scenes[p_id].scene = Ref<PackedScene>();
	}
	emit_signal("changed");
}

Ref<PackedScene> LayeredTileSetScenesCollectionSource::get_scene_tile_scene(int p_id) const {
	ERR_FAIL_COND_V(!scenes.has(p_id), Ref<PackedScene>());
	return scenes[p_id].scene;
}

void LayeredTileSetScenesCollectionSource::set_scene_tile_display_placeholder(int p_id, bool p_display_placeholder) {
	ERR_FAIL_COND(!scenes.has(p_id));

	scenes[p_id].display_placeholder = p_display_placeholder;

	emit_signal("changed");
}

bool LayeredTileSetScenesCollectionSource::get_scene_tile_display_placeholder(int p_id) const {
	ERR_FAIL_COND_V(!scenes.has(p_id), false);
	return scenes[p_id].display_placeholder;
}

void LayeredTileSetScenesCollectionSource::remove_scene_tile(int p_id) {
	ERR_FAIL_COND(!scenes.has(p_id));

	scenes.erase(p_id);
	scenes_ids.erase(p_id);
	emit_signal("changed");
}

int LayeredTileSetScenesCollectionSource::get_next_scene_tile_id() const {
	return next_scene_id;
}

bool LayeredTileSetScenesCollectionSource::_set(const StringName &p_name, const Variant &p_value) {
	Vector<String> components = String(p_name).split("/", true, 2);

	if (components.size() >= 2 && components[0] == "scenes" && components[1].is_valid_integer()) {
		int scene_id = components[1].to_int();
		if (components.size() >= 3 && components[2] == "scene") {
			if (has_scene_tile_id(scene_id)) {
				set_scene_tile_scene(scene_id, p_value);
			} else {
				create_scene_tile(p_value, scene_id);
			}
			return true;
		} else if (components.size() >= 3 && components[2] == "display_placeholder") {
			if (!has_scene_tile_id(scene_id)) {
				create_scene_tile(p_value, scene_id);
			}

			return true;
		}
	}

	return false;
}

bool LayeredTileSetScenesCollectionSource::_get(const StringName &p_name, Variant &r_ret) const {
	Vector<String> components = String(p_name).split("/", true, 2);

	if (components.size() >= 2 && components[0] == "scenes" && components[1].is_valid_integer() && scenes.has(components[1].to_int())) {
		if (components.size() >= 3 && components[2] == "scene") {
			r_ret = scenes[components[1].to_int()].scene;
			return true;
		} else if (components.size() >= 3 && components[2] == "display_placeholder") {
			r_ret = scenes[components[1].to_int()].scene;
			return true;
		}
	}

	return false;
}

void LayeredTileSetScenesCollectionSource::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < scenes_ids.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, vformat("%s/%d/%s", PNAME("scenes"), scenes_ids[i], PNAME("scene")), PROPERTY_HINT_RESOURCE_TYPE, "LayeredTileSetScenesCollectionSource"));

		PropertyInfo property_info = PropertyInfo(Variant::BOOL, vformat("%s/%d/%s", PNAME("scenes"), scenes_ids[i], PNAME("display_placeholder")));
		if (scenes[scenes_ids[i]].display_placeholder == false) {
			property_info.usage ^= PROPERTY_USAGE_STORAGE;
		}
		p_list->push_back(property_info);
	}
}

void LayeredTileSetScenesCollectionSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene_tiles_count"), &LayeredTileSetScenesCollectionSource::get_scene_tiles_count);
	ClassDB::bind_method(D_METHOD("get_scene_tile_id", "index"), &LayeredTileSetScenesCollectionSource::get_scene_tile_id);
	ClassDB::bind_method(D_METHOD("has_scene_tile_id", "id"), &LayeredTileSetScenesCollectionSource::has_scene_tile_id);
	ClassDB::bind_method(D_METHOD("create_scene_tile", "packed_scene", "id_override"), &LayeredTileSetScenesCollectionSource::create_scene_tile, DEFVAL(INVALID_TILE_ALTERNATIVE));
	ClassDB::bind_method(D_METHOD("set_scene_tile_id", "id", "new_id"), &LayeredTileSetScenesCollectionSource::set_scene_tile_id);
	ClassDB::bind_method(D_METHOD("set_scene_tile_scene", "id", "packed_scene"), &LayeredTileSetScenesCollectionSource::set_scene_tile_scene);
	ClassDB::bind_method(D_METHOD("get_scene_tile_scene", "id"), &LayeredTileSetScenesCollectionSource::get_scene_tile_scene);
	ClassDB::bind_method(D_METHOD("set_scene_tile_display_placeholder", "id", "display_placeholder"), &LayeredTileSetScenesCollectionSource::set_scene_tile_display_placeholder);
	ClassDB::bind_method(D_METHOD("get_scene_tile_display_placeholder", "id"), &LayeredTileSetScenesCollectionSource::get_scene_tile_display_placeholder);
	ClassDB::bind_method(D_METHOD("remove_scene_tile", "id"), &LayeredTileSetScenesCollectionSource::remove_scene_tile);
	ClassDB::bind_method(D_METHOD("get_next_scene_tile_id"), &LayeredTileSetScenesCollectionSource::get_next_scene_tile_id);
}

/////////////////////////////// TileData //////////////////////////////////////

void LayeredTileData::set_tile_set(const LayeredTileSet *p_tile_set) {
	tile_set = p_tile_set;
	notify_tile_data_properties_should_change();
}

void LayeredTileData::notify_tile_data_properties_should_change() {
	if (!tile_set) {
		return;
	}

	occluders.resize(tile_set->get_occlusion_layers_count());
	physics.resize(tile_set->get_physics_layers_count());
	for (int bit_index = 0; bit_index < 16; bit_index++) {
		if (terrain_set < 0 || terrain_peering_bits[bit_index] >= tile_set->get_terrains_count(terrain_set)) {
			terrain_peering_bits[bit_index] = -1;
		}
	}
	navigation.resize(tile_set->get_navigation_layers_count());

	// Convert custom data to the new type.
	custom_data.resize(tile_set->get_custom_data_layers_count());
	for (int i = 0; i < custom_data.size(); i++) {
		if (custom_data[i].get_type() != tile_set->get_custom_data_layer_type(i)) {
			Variant new_val;
			Callable::CallError error;
			if (Variant::can_convert(custom_data[i].get_type(), tile_set->get_custom_data_layer_type(i))) {
				const Variant *args[] = { &custom_data[i] };
				Variant::construct(tile_set->get_custom_data_layer_type(i), new_val, args, 1, error);
			} else {
				Variant::construct(tile_set->get_custom_data_layer_type(i), new_val, nullptr, 0, error);
			}
			custom_data.write[i] = new_val;
		}
	}

	property_list_changed_notify();
	emit_signal("changed");
}

void LayeredTileData::add_occlusion_layer(int p_to_pos) {
	if (p_to_pos < 0) {
		p_to_pos = occluders.size();
	}
	ERR_FAIL_INDEX(p_to_pos, occluders.size() + 1);
	occluders.insert(p_to_pos, OcclusionLayerTileData());
}

void LayeredTileData::move_occlusion_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, occluders.size());
	ERR_FAIL_INDEX(p_to_pos, occluders.size() + 1);
	occluders.insert(p_to_pos, occluders[p_from_index]);
	occluders.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
}

void LayeredTileData::remove_occlusion_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, occluders.size());
	occluders.remove(p_index);
}

void LayeredTileData::add_physics_layer(int p_to_pos) {
	if (p_to_pos < 0) {
		p_to_pos = physics.size();
	}
	ERR_FAIL_INDEX(p_to_pos, physics.size() + 1);
	physics.insert(p_to_pos, PhysicsLayerTileData());
}

void LayeredTileData::move_physics_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, physics.size());
	ERR_FAIL_INDEX(p_to_pos, physics.size() + 1);
	physics.insert(p_to_pos, physics[p_from_index]);
	physics.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
}

void LayeredTileData::remove_physics_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, physics.size());
	physics.remove(p_index);
}

void LayeredTileData::add_terrain_set(int p_to_pos) {
	if (p_to_pos >= 0 && p_to_pos <= terrain_set) {
		terrain_set += 1;
	}
}

void LayeredTileData::move_terrain_set(int p_from_index, int p_to_pos) {
	if (p_from_index == terrain_set) {
		terrain_set = (p_from_index < p_to_pos) ? p_to_pos - 1 : p_to_pos;
	} else {
		if (p_from_index < terrain_set) {
			terrain_set -= 1;
		}
		if (p_to_pos <= terrain_set) {
			terrain_set += 1;
		}
	}
}

void LayeredTileData::remove_terrain_set(int p_index) {
	if (p_index == terrain_set) {
		terrain_set = -1;
		for (int i = 0; i < 16; i++) {
			terrain_peering_bits[i] = -1;
		}
	} else if (terrain_set > p_index) {
		terrain_set -= 1;
	}
}

void LayeredTileData::add_terrain(int p_terrain_set, int p_to_pos) {
	if (terrain_set == p_terrain_set) {
		for (int i = 0; i < 16; i++) {
			if (p_to_pos >= 0 && p_to_pos <= terrain_peering_bits[i]) {
				terrain_peering_bits[i] += 1;
			}
		}
	}
}

void LayeredTileData::move_terrain(int p_terrain_set, int p_from_index, int p_to_pos) {
	if (terrain_set == p_terrain_set) {
		for (int i = 0; i < 16; i++) {
			if (p_from_index == terrain_peering_bits[i]) {
				terrain_peering_bits[i] = (p_from_index < p_to_pos) ? p_to_pos - 1 : p_to_pos;
			} else {
				if (p_from_index < terrain_peering_bits[i]) {
					terrain_peering_bits[i] -= 1;
				}
				if (p_to_pos <= terrain_peering_bits[i]) {
					terrain_peering_bits[i] += 1;
				}
			}
		}
	}
}

void LayeredTileData::remove_terrain(int p_terrain_set, int p_index) {
	if (terrain_set == p_terrain_set) {
		if (terrain == p_index) {
			terrain = -1;
		} else if (terrain > p_index) {
			terrain -= 1;
		}

		for (int i = 0; i < 16; i++) {
			if (terrain_peering_bits[i] == p_index) {
				terrain_peering_bits[i] = -1;
			} else if (terrain_peering_bits[i] > p_index) {
				terrain_peering_bits[i] -= 1;
			}
		}
	}
}

void LayeredTileData::add_navigation_layer(int p_to_pos) {
	if (p_to_pos < 0) {
		p_to_pos = navigation.size();
	}
	ERR_FAIL_INDEX(p_to_pos, navigation.size() + 1);
	navigation.insert(p_to_pos, NavigationLayerTileData());
}

void LayeredTileData::move_navigation_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, navigation.size());
	ERR_FAIL_INDEX(p_to_pos, navigation.size() + 1);
	navigation.insert(p_to_pos, navigation[p_from_index]);
	navigation.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
}

void LayeredTileData::remove_navigation_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, navigation.size());
	navigation.remove(p_index);
}

void LayeredTileData::add_custom_data_layer(int p_to_pos) {
	if (p_to_pos < 0) {
		p_to_pos = custom_data.size();
	}
	ERR_FAIL_INDEX(p_to_pos, custom_data.size() + 1);
	custom_data.insert(p_to_pos, Variant());
}

void LayeredTileData::move_custom_data_layer(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, custom_data.size());
	ERR_FAIL_INDEX(p_to_pos, custom_data.size() + 1);
	custom_data.insert(p_to_pos, custom_data[p_from_index]);
	custom_data.remove(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
}

void LayeredTileData::remove_custom_data_layer(int p_index) {
	ERR_FAIL_INDEX(p_index, custom_data.size());
	custom_data.remove(p_index);
}

void LayeredTileData::set_allow_transform(bool p_allow_transform) {
	allow_transform = p_allow_transform;
}

bool LayeredTileData::is_allowing_transform() const {
	return allow_transform;
}

LayeredTileData *LayeredTileData::duplicate() {
	TileData *output = memnew(TileData);
	output->tile_set = tile_set;

	output->allow_transform = allow_transform;

	// Rendering
	output->flip_h = flip_h;
	output->flip_v = flip_v;
	output->transpose = transpose;
	output->texture_origin = texture_origin;
	output->material = material;
	output->modulate = modulate;
	output->z_index = z_index;
	output->y_sort_origin = y_sort_origin;
	output->occluders = occluders;
	// Physics
	output->physics = physics;
	// Terrain
	output->terrain_set = -1;
	memcpy(output->terrain_peering_bits, terrain_peering_bits, 16 * sizeof(int));
	// Navigation
	output->navigation = navigation;
	// Misc
	output->probability = probability;
	// Custom data
	output->custom_data = custom_data;

	return output;
}

// Rendering
void LayeredTileData::set_flip_h(bool p_flip_h) {
	ERR_FAIL_COND_MSG(!allow_transform && p_flip_h, "Transform is only allowed for alternative tiles (with its alternative_id != 0)");
	flip_h = p_flip_h;
	emit_signal("changed");
}
bool LayeredTileData::get_flip_h() const {
	return flip_h;
}

void LayeredTileData::set_flip_v(bool p_flip_v) {
	ERR_FAIL_COND_MSG(!allow_transform && p_flip_v, "Transform is only allowed for alternative tiles (with its alternative_id != 0)");
	flip_v = p_flip_v;
	emit_signal("changed");
}

bool LayeredTileData::get_flip_v() const {
	return flip_v;
}

void LayeredTileData::set_transpose(bool p_transpose) {
	ERR_FAIL_COND_MSG(!allow_transform && p_transpose, "Transform is only allowed for alternative tiles (with its alternative_id != 0)");
	transpose = p_transpose;
	emit_signal("changed");
}
bool LayeredTileData::get_transpose() const {
	return transpose;
}

void LayeredTileData::set_texture_origin(Vector2i p_texture_origin) {
	texture_origin = p_texture_origin;
	emit_signal("changed");
}

Vector2i LayeredTileData::get_texture_origin() const {
	return texture_origin;
}

void LayeredTileData::set_material(Ref<Material> p_material) {
	material = p_material;
	emit_signal("changed");
}
Ref<Material> LayeredTileData::get_material() const {
	return material;
}

void LayeredTileData::set_modulate(Color p_modulate) {
	modulate = p_modulate;
	emit_signal("changed");
}
Color LayeredTileData::get_modulate() const {
	return modulate;
}

void LayeredTileData::set_z_index(int p_z_index) {
	z_index = p_z_index;
	emit_signal("changed");
}
int LayeredTileData::get_z_index() const {
	return z_index;
}

void LayeredTileData::set_y_sort_origin(int p_y_sort_origin) {
	y_sort_origin = p_y_sort_origin;
	emit_signal("changed");
}
int LayeredTileData::get_y_sort_origin() const {
	return y_sort_origin;
}

void LayeredTileData::set_occluder(int p_layer_id, Ref<OccluderPolygon2D> p_occluder_polygon) {
	ERR_FAIL_INDEX(p_layer_id, occluders.size());
	occluders.write[p_layer_id].occluder = p_occluder_polygon;
	occluders.write[p_layer_id].transformed_occluders.clear();
	emit_signal("changed");
}

Ref<OccluderPolygon2D> LayeredTileData::get_occluder(int p_layer_id, bool p_flip_h, bool p_flip_v, bool p_transpose) const {
	ERR_FAIL_INDEX_V(p_layer_id, occluders.size(), Ref<OccluderPolygon2D>());

	const OcclusionLayerTileData &layer_tile_data = occluders[p_layer_id];

	int key = int(p_flip_h) | int(p_flip_v) << 1 | int(p_transpose) << 2;
	if (key == 0) {
		return layer_tile_data.occluder;
	}

	if (layer_tile_data.occluder.is_null()) {
		return Ref<OccluderPolygon2D>();
	}

	HashMap<int, Ref<OccluderPolygon2D>>::Iterator I = layer_tile_data.transformed_occluders.find(key);
	if (!I) {
		Ref<OccluderPolygon2D> transformed_polygon;
		transformed_polygon.instance();
		transformed_polygon->set_polygon(get_transformed_vertices(layer_tile_data.occluder->get_polygon(), p_flip_h, p_flip_v, p_transpose));
		layer_tile_data.transformed_occluders[key] = transformed_polygon;
		return transformed_polygon;
	} else {
		return I->value;
	}
}

// Physics
void LayeredTileData::set_constant_linear_velocity(int p_layer_id, const Vector2 &p_velocity) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	physics.write[p_layer_id].linear_velocity = p_velocity;
	emit_signal("changed");
}

Vector2 LayeredTileData::get_constant_linear_velocity(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), Vector2());
	return physics[p_layer_id].linear_velocity;
}

void LayeredTileData::set_constant_angular_velocity(int p_layer_id, real_t p_velocity) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	physics.write[p_layer_id].angular_velocity = p_velocity;
	emit_signal("changed");
}

real_t LayeredTileData::get_constant_angular_velocity(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), 0.0);
	return physics[p_layer_id].angular_velocity;
}

void LayeredTileData::set_collision_polygons_count(int p_layer_id, int p_polygons_count) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	ERR_FAIL_COND(p_polygons_count < 0);
	if (p_polygons_count == physics.write[p_layer_id].polygons.size()) {
		return;
	}
	physics.write[p_layer_id].polygons.resize(p_polygons_count);
	property_list_changed_notify();
	emit_signal("changed");
}

int LayeredTileData::get_collision_polygons_count(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), 0);
	return physics[p_layer_id].polygons.size();
}

void LayeredTileData::add_collision_polygon(int p_layer_id) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	physics.write[p_layer_id].polygons.push_back(PhysicsLayerTileData::PolygonShapeTileData());
	emit_signal("changed");
}

void LayeredTileData::remove_collision_polygon(int p_layer_id, int p_polygon_index) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	ERR_FAIL_INDEX(p_polygon_index, physics[p_layer_id].polygons.size());
	physics.write[p_layer_id].polygons.remove(p_polygon_index);
	emit_signal("changed");
}

void LayeredTileData::set_collision_polygon_points(int p_layer_id, int p_polygon_index, Vector<Vector2> p_polygon) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	ERR_FAIL_INDEX(p_polygon_index, physics[p_layer_id].polygons.size());
	ERR_FAIL_COND_MSG(p_polygon.size() != 0 && p_polygon.size() < 3, "Invalid polygon. Needs either 0 or more than 3 points.");

	TileData::PhysicsLayerTileData::PolygonShapeTileData &polygon_shape_tile_data = physics.write[p_layer_id].polygons.write[p_polygon_index];

	if (p_polygon.empty()) {
		polygon_shape_tile_data.shapes.clear();
	} else {
		// Decompose into convex shapes.
		Vector<Vector<Vector2>> decomp = Geometry::decompose_polygon_in_convex(p_polygon);
		ERR_FAIL_COND_MSG(decomp.empty(), "Could not decompose the polygon into convex shapes.");

		polygon_shape_tile_data.shapes.resize(decomp.size());
		for (int i = 0; i < decomp.size(); i++) {
			Ref<ConvexPolygonShape2D> shape;
			shape.instance();
			shape->set_points(decomp[i]);
			polygon_shape_tile_data.shapes[i] = shape;
		}
	}
	polygon_shape_tile_data.transformed_shapes.clear();
	polygon_shape_tile_data.polygon = p_polygon;
	emit_signal("changed");
}

Vector<Vector2> LayeredTileData::get_collision_polygon_points(int p_layer_id, int p_polygon_index) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), Vector<Vector2>());
	ERR_FAIL_INDEX_V(p_polygon_index, physics[p_layer_id].polygons.size(), Vector<Vector2>());
	return physics[p_layer_id].polygons[p_polygon_index].polygon;
}

void LayeredTileData::set_collision_polygon_one_way(int p_layer_id, int p_polygon_index, bool p_one_way) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	ERR_FAIL_INDEX(p_polygon_index, physics[p_layer_id].polygons.size());
	physics.write[p_layer_id].polygons.write[p_polygon_index].one_way = p_one_way;
	emit_signal("changed");
}

bool LayeredTileData::is_collision_polygon_one_way(int p_layer_id, int p_polygon_index) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), false);
	ERR_FAIL_INDEX_V(p_polygon_index, physics[p_layer_id].polygons.size(), false);
	return physics[p_layer_id].polygons[p_polygon_index].one_way;
}

void LayeredTileData::set_collision_polygon_one_way_margin(int p_layer_id, int p_polygon_index, float p_one_way_margin) {
	ERR_FAIL_INDEX(p_layer_id, physics.size());
	ERR_FAIL_INDEX(p_polygon_index, physics[p_layer_id].polygons.size());
	physics.write[p_layer_id].polygons.write[p_polygon_index].one_way_margin = p_one_way_margin;
	emit_signal("changed");
}

float LayeredTileData::get_collision_polygon_one_way_margin(int p_layer_id, int p_polygon_index) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), 0.0);
	ERR_FAIL_INDEX_V(p_polygon_index, physics[p_layer_id].polygons.size(), 0.0);
	return physics[p_layer_id].polygons[p_polygon_index].one_way_margin;
}

int LayeredTileData::get_collision_polygon_shapes_count(int p_layer_id, int p_polygon_index) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), 0);
	ERR_FAIL_INDEX_V(p_polygon_index, physics[p_layer_id].polygons.size(), 0);
	return physics[p_layer_id].polygons[p_polygon_index].shapes.size();
}

Ref<ConvexPolygonShape2D> LayeredTileData::get_collision_polygon_shape(int p_layer_id, int p_polygon_index, int shape_index, bool p_flip_h, bool p_flip_v, bool p_transpose) const {
	ERR_FAIL_INDEX_V(p_layer_id, physics.size(), Ref<ConvexPolygonShape2D>());
	ERR_FAIL_INDEX_V(p_polygon_index, physics[p_layer_id].polygons.size(), Ref<ConvexPolygonShape2D>());
	ERR_FAIL_INDEX_V(shape_index, (int)physics[p_layer_id].polygons[p_polygon_index].shapes.size(), Ref<ConvexPolygonShape2D>());

	const PhysicsLayerTileData &layer_tile_data = physics[p_layer_id];
	const PhysicsLayerTileData::PolygonShapeTileData &shapes_data = layer_tile_data.polygons[p_polygon_index];

	int key = int(p_flip_h) | int(p_flip_v) << 1 | int(p_transpose) << 2;
	if (key == 0) {
		return shapes_data.shapes[shape_index];
	}
	if (shapes_data.shapes[shape_index].is_null()) {
		return Ref<ConvexPolygonShape2D>();
	}

	HashMap<int, LocalVector<Ref<ConvexPolygonShape2D>>>::Iterator I = shapes_data.transformed_shapes.find(key);
	if (!I) {
		int size = shapes_data.shapes.size();
		shapes_data.transformed_shapes[key].resize(size);
		for (int i = 0; i < size; i++) {
			Ref<ConvexPolygonShape2D> transformed_polygon;
			transformed_polygon.instance();
			transformed_polygon->set_points(get_transformed_vertices(shapes_data.shapes[shape_index]->get_points(), p_flip_h, p_flip_v, p_transpose));
			shapes_data.transformed_shapes[key][i] = transformed_polygon;
		}
		return shapes_data.transformed_shapes[key][shape_index];
	} else {
		return I->value[shape_index];
	}
}

// Terrain
void LayeredTileData::set_terrain_set(int p_terrain_set) {
	ERR_FAIL_COND(p_terrain_set < -1);
	if (p_terrain_set == terrain_set) {
		return;
	}
	if (tile_set) {
		ERR_FAIL_COND(p_terrain_set >= tile_set->get_terrain_sets_count());
		terrain = -1;
		for (int i = 0; i < 16; i++) {
			terrain_peering_bits[i] = -1;
		}
	}
	terrain_set = p_terrain_set;
	property_list_changed_notify();
	emit_signal("changed");
}

int LayeredTileData::get_terrain_set() const {
	return terrain_set;
}

void LayeredTileData::set_terrain(int p_terrain) {
	ERR_FAIL_COND(terrain_set < 0);
	ERR_FAIL_COND(p_terrain < -1);
	if (tile_set) {
		ERR_FAIL_COND(p_terrain >= tile_set->get_terrains_count(terrain_set));
	}
	terrain = p_terrain;
	emit_signal("changed");
}

int LayeredTileData::get_terrain() const {
	return terrain;
}

void LayeredTileData::set_terrain_peering_bit(LayeredTileSet::CellNeighbor p_peering_bit, int p_terrain_index) {
	ERR_FAIL_INDEX(p_peering_bit, LayeredTileSet::CellNeighbor::CELL_NEIGHBOR_MAX);
	ERR_FAIL_COND(terrain_set < 0);
	ERR_FAIL_COND(p_terrain_index < -1);
	if (tile_set) {
		ERR_FAIL_COND(p_terrain_index >= tile_set->get_terrains_count(terrain_set));
		ERR_FAIL_COND(!is_valid_terrain_peering_bit(p_peering_bit));
	}
	terrain_peering_bits[p_peering_bit] = p_terrain_index;
	emit_signal("changed");
}

int LayeredTileData::get_terrain_peering_bit(LayeredTileSet::CellNeighbor p_peering_bit) const {
	ERR_FAIL_COND_V(!is_valid_terrain_peering_bit(p_peering_bit), -1);
	return terrain_peering_bits[p_peering_bit];
}

bool LayeredTileData::is_valid_terrain_peering_bit(LayeredTileSet::CellNeighbor p_peering_bit) const {
	ERR_FAIL_NULL_V(tile_set, false);

	return tile_set->is_valid_terrain_peering_bit(terrain_set, p_peering_bit);
}

LayeredTileSet::TerrainsPattern LayeredTileData::get_terrains_pattern() const {
	ERR_FAIL_NULL_V(tile_set, LayeredTileSet::TerrainsPattern());

	LayeredTileSet::TerrainsPattern output(tile_set, terrain_set);
	output.set_terrain(terrain);
	for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
		if (tile_set->is_valid_terrain_peering_bit(terrain_set, LayeredTileSet::CellNeighbor(i))) {
			output.set_terrain_peering_bit(LayeredTileSet::CellNeighbor(i), get_terrain_peering_bit(LayeredTileSet::CellNeighbor(i)));
		}
	}
	return output;
}

// Navigation
void LayeredTileData::set_navigation_polygon(int p_layer_id, Ref<NavigationPolygon> p_navigation_polygon) {
	ERR_FAIL_INDEX(p_layer_id, navigation.size());
	navigation.write[p_layer_id].navigation_polygon = p_navigation_polygon;
	navigation.write[p_layer_id].transformed_navigation_polygon.clear();
	emit_signal("changed");
}

Ref<NavigationPolygon> LayeredTileData::get_navigation_polygon(int p_layer_id, bool p_flip_h, bool p_flip_v, bool p_transpose) const {
	ERR_FAIL_INDEX_V(p_layer_id, navigation.size(), Ref<NavigationPolygon>());

	const NavigationLayerTileData &layer_tile_data = navigation[p_layer_id];

	int key = int(p_flip_h) | int(p_flip_v) << 1 | int(p_transpose) << 2;
	if (key == 0) {
		return layer_tile_data.navigation_polygon;
	}

	if (layer_tile_data.navigation_polygon.is_null()) {
		return Ref<NavigationPolygon>();
	}

	HashMap<int, Ref<NavigationPolygon>>::Iterator I = layer_tile_data.transformed_navigation_polygon.find(key);
	if (!I) {
		Ref<NavigationPolygon> transformed_polygon;
		transformed_polygon.instance();

		PoolVector2Array new_points = get_transformed_vertices(layer_tile_data.navigation_polygon->get_vertices(), p_flip_h, p_flip_v, p_transpose);
		transformed_polygon->set_vertices(new_points);

		for (int i = 0; i < layer_tile_data.navigation_polygon->get_outline_count(); i++) {
			PoolVector2Array new_outline = get_transformed_vertices(layer_tile_data.navigation_polygon->get_outline(i), p_flip_h, p_flip_v, p_transpose);
			transformed_polygon->add_outline(new_outline);
		}

		PackedInt32Array indices;
		indices.resize(new_points.size());
		int *w = indices.ptrw();
		for (int i = 0; i < new_points.size(); i++) {
			w[i] = i;
		}
		transformed_polygon->add_polygon(indices);
		layer_tile_data.transformed_navigation_polygon[key] = transformed_polygon;
		return transformed_polygon;
	} else {
		return I->value;
	}
}

// Misc
void LayeredTileData::set_probability(float p_probability) {
	ERR_FAIL_COND(p_probability < 0.0);
	probability = p_probability;
	emit_signal("changed");
}
float LayeredTileData::get_probability() const {
	return probability;
}

// Custom data
void LayeredTileData::set_custom_data(String p_layer_name, Variant p_value) {
	ERR_FAIL_NULL(tile_set);
	int p_layer_id = tile_set->get_custom_data_layer_by_name(p_layer_name);
	ERR_FAIL_COND_MSG(p_layer_id < 0, vformat("LayeredTileSet has no layer with name: %s", p_layer_name));
	set_custom_data_by_layer_id(p_layer_id, p_value);
}

Variant LayeredTileData::get_custom_data(String p_layer_name) const {
	ERR_FAIL_NULL_V(tile_set, Variant());
	int p_layer_id = tile_set->get_custom_data_layer_by_name(p_layer_name);
	ERR_FAIL_COND_V_MSG(p_layer_id < 0, Variant(), vformat("LayeredTileSet has no layer with name: %s", p_layer_name));
	return get_custom_data_by_layer_id(p_layer_id);
}

void LayeredTileData::set_custom_data_by_layer_id(int p_layer_id, Variant p_value) {
	ERR_FAIL_INDEX(p_layer_id, custom_data.size());
	custom_data.write[p_layer_id] = p_value;
	emit_signal("changed");
}

Variant LayeredTileData::get_custom_data_by_layer_id(int p_layer_id) const {
	ERR_FAIL_INDEX_V(p_layer_id, custom_data.size(), Variant());
	return custom_data[p_layer_id];
}

PoolVector2Array LayeredTileData::get_transformed_vertices(const PoolVector2Array &p_vertices, bool p_flip_h, bool p_flip_v, bool p_transpose) {
	const Vector2 *r = p_vertices.ptr();
	int size = p_vertices.size();

	PoolVector2Array new_points;
	new_points.resize(size);
	Vector2 *w = new_points.ptrw();

	for (int i = 0; i < size; i++) {
		Vector2 v;
		if (p_transpose) {
			v = Vector2(r[i].y, r[i].x);
		} else {
			v = r[i];
		}

		if (p_flip_h) {
			v.x *= -1;
		}
		if (p_flip_v) {
			v.y *= -1;
		}
		w[i] = v;
	}
	return new_points;
}

bool LayeredTileData::_set(const StringName &p_name, const Variant &p_value) {
#ifndef DISABLE_DEPRECATED
	if (p_name == "texture_offset") {
		texture_origin = p_value;
		return true;
	}
#endif

	Vector<String> components = String(p_name).split("/", true, 2);

	if (components.size() == 2 && components[0].begins_with("occlusion_layer_") && components[0].trim_prefix("occlusion_layer_").is_valid_integer()) {
		// Occlusion layers.
		int layer_index = components[0].trim_prefix("occlusion_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygon") {
			Ref<OccluderPolygon2D> polygon = p_value;

			if (layer_index >= occluders.size()) {
				if (tile_set) {
					return false;
				} else {
					occluders.resize(layer_index + 1);
				}
			}
			set_occluder(layer_index, polygon);
			return true;
		}
	} else if (components.size() >= 2 && components[0].begins_with("physics_layer_") && components[0].trim_prefix("physics_layer_").is_valid_integer()) {
		// Physics layers.
		int layer_index = components[0].trim_prefix("physics_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components.size() == 2) {
			if (layer_index >= physics.size()) {
				if (tile_set) {
					return false;
				} else {
					physics.resize(layer_index + 1);
				}
			}
			if (components[1] == "linear_velocity") {
				set_constant_linear_velocity(layer_index, p_value);
				return true;
			} else if (components[1] == "angular_velocity") {
				set_constant_angular_velocity(layer_index, p_value);
				return true;
			} else if (components[1] == "polygons_count") {
				if (p_value.get_type() != Variant::INT) {
					return false;
				}
				set_collision_polygons_count(layer_index, p_value);
				return true;
			}
		} else if (components.size() == 3 && components[1].begins_with("polygon_") && components[1].trim_prefix("polygon_").is_valid_integer()) {
			int polygon_index = components[1].trim_prefix("polygon_").to_int();
			ERR_FAIL_COND_V(polygon_index < 0, false);

			if (components[2] == "points" || components[2] == "one_way" || components[2] == "one_way_margin") {
				if (layer_index >= physics.size()) {
					if (tile_set) {
						return false;
					} else {
						physics.resize(layer_index + 1);
					}
				}

				if (polygon_index >= physics[layer_index].polygons.size()) {
					physics.write[layer_index].polygons.resize(polygon_index + 1);
				}
			}
			if (components[2] == "points") {
				Vector<Vector2> polygon = p_value;
				set_collision_polygon_points(layer_index, polygon_index, polygon);
				return true;
			} else if (components[2] == "one_way") {
				set_collision_polygon_one_way(layer_index, polygon_index, p_value);
				return true;
			} else if (components[2] == "one_way_margin") {
				set_collision_polygon_one_way_margin(layer_index, polygon_index, p_value);
				return true;
			}
		}
	} else if (components.size() == 2 && components[0].begins_with("navigation_layer_") && components[0].trim_prefix("navigation_layer_").is_valid_integer()) {
		// Navigation layers.
		int layer_index = components[0].trim_prefix("navigation_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygon") {
			Ref<NavigationPolygon> polygon = p_value;

			if (layer_index >= navigation.size()) {
				if (tile_set) {
					return false;
				} else {
					navigation.resize(layer_index + 1);
				}
			}
			set_navigation_polygon(layer_index, polygon);
			return true;
		}
	} else if (components.size() == 2 && components[0] == "terrains_peering_bit") {
		// Terrains.
		for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
			LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
			if (components[1] == LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]) {
				set_terrain_peering_bit(bit, p_value);
				return true;
			}
		}
		return false;
	} else if (components.size() == 1 && components[0].begins_with("custom_data_") && components[0].trim_prefix("custom_data_").is_valid_integer()) {
		// Custom data layers.
		int layer_index = components[0].trim_prefix("custom_data_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);

		if (layer_index >= custom_data.size()) {
			if (tile_set) {
				return false;
			} else {
				custom_data.resize(layer_index + 1);
			}
		}
		set_custom_data_by_layer_id(layer_index, p_value);

		return true;
	}

	return false;
}

bool LayeredTileData::_get(const StringName &p_name, Variant &r_ret) const {
#ifndef DISABLE_DEPRECATED
	if (p_name == "texture_offset") {
		r_ret = texture_origin;
		return true;
	}
#endif

	Vector<String> components = String(p_name).split("/", true, 2);

	if (tile_set) {
		if (components.size() == 2 && components[0].begins_with("occlusion_layer") && components[0].trim_prefix("occlusion_layer_").is_valid_integer()) {
			// Occlusion layers.
			int layer_index = components[0].trim_prefix("occlusion_layer_").to_int();
			ERR_FAIL_COND_V(layer_index < 0, false);
			if (layer_index >= occluders.size()) {
				return false;
			}
			if (components[1] == "polygon") {
				r_ret = get_occluder(layer_index);
				return true;
			}
		} else if (components.size() >= 2 && components[0].begins_with("physics_layer_") && components[0].trim_prefix("physics_layer_").is_valid_integer()) {
			// Physics layers.
			int layer_index = components[0].trim_prefix("physics_layer_").to_int();
			ERR_FAIL_COND_V(layer_index < 0, false);
			if (layer_index >= physics.size()) {
				return false;
			}

			if (components.size() == 2) {
				if (components[1] == "linear_velocity") {
					r_ret = get_constant_linear_velocity(layer_index);
					return true;
				} else if (components[1] == "angular_velocity") {
					r_ret = get_constant_angular_velocity(layer_index);
					return true;
				} else if (components[1] == "polygons_count") {
					r_ret = get_collision_polygons_count(layer_index);
					return true;
				}
			} else if (components.size() == 3 && components[1].begins_with("polygon_") && components[1].trim_prefix("polygon_").is_valid_integer()) {
				int polygon_index = components[1].trim_prefix("polygon_").to_int();
				ERR_FAIL_COND_V(polygon_index < 0, false);
				if (polygon_index >= physics[layer_index].polygons.size()) {
					return false;
				}
				if (components[2] == "points") {
					r_ret = get_collision_polygon_points(layer_index, polygon_index);
					return true;
				} else if (components[2] == "one_way") {
					r_ret = is_collision_polygon_one_way(layer_index, polygon_index);
					return true;
				} else if (components[2] == "one_way_margin") {
					r_ret = get_collision_polygon_one_way_margin(layer_index, polygon_index);
					return true;
				}
			}
		} else if (components.size() == 2 && components[0] == "terrains_peering_bit") {
			// Terrains.
			for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
				if (components[1] == LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]) {
					r_ret = terrain_peering_bits[i];
					return true;
				}
			}
			return false;
		} else if (components.size() == 2 && components[0].begins_with("navigation_layer_") && components[0].trim_prefix("navigation_layer_").is_valid_integer()) {
			// Occlusion layers.
			int layer_index = components[0].trim_prefix("navigation_layer_").to_int();
			ERR_FAIL_COND_V(layer_index < 0, false);
			if (layer_index >= navigation.size()) {
				return false;
			}
			if (components[1] == "polygon") {
				r_ret = get_navigation_polygon(layer_index);
				return true;
			}
		} else if (components.size() == 1 && components[0].begins_with("custom_data_") && components[0].trim_prefix("custom_data_").is_valid_integer()) {
			// Custom data layers.
			int layer_index = components[0].trim_prefix("custom_data_").to_int();
			ERR_FAIL_COND_V(layer_index < 0, false);
			if (layer_index >= custom_data.size()) {
				return false;
			}
			r_ret = get_custom_data_by_layer_id(layer_index);
			return true;
		}
	}

	return false;
}

void LayeredTileData::_get_property_list(List<PropertyInfo> *p_list) const {
	PropertyInfo property_info;
	// Add the groups manually.
	if (tile_set) {
		// Occlusion layers.
		p_list->push_back(PropertyInfo(Variant::NIL, "Rendering", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));

		for (int i = 0; i < occluders.size(); i++) {
			// occlusion_layer_%d/polygon
			property_info = PropertyInfo(Variant::OBJECT, vformat("occlusion_layer_%d/%s", i, "polygon"), PROPERTY_HINT_RESOURCE_TYPE, "OccluderPolygon2D", PROPERTY_USAGE_DEFAULT);
			if (occluders[i].occluder.is_null()) {
				property_info.usage ^= PROPERTY_USAGE_STORAGE;
			}
			p_list->push_back(property_info);
		}

		// Physics layers.
		p_list->push_back(PropertyInfo(Variant::NIL, "Physics", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
		for (int i = 0; i < physics.size(); i++) {
			p_list->push_back(PropertyInfo(Variant::VECTOR2, vformat("physics_layer_%d/%s", i, "linear_velocity"), PROPERTY_HINT_NONE));
			p_list->push_back(PropertyInfo(Variant::REAL, vformat("physics_layer_%d/%s", i, "angular_velocity"), PROPERTY_HINT_NONE));
			p_list->push_back(PropertyInfo(Variant::INT, vformat("physics_layer_%d/%s", i, "polygons_count"), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR));

			for (int j = 0; j < physics[i].polygons.size(); j++) {
				// physics_layer_%d/points
				property_info = PropertyInfo(Variant::ARRAY, vformat("physics_layer_%d/polygon_%d/%s", i, j, "points"), PROPERTY_HINT_ARRAY_TYPE, "Vector2", PROPERTY_USAGE_DEFAULT);
				if (physics[i].polygons[j].polygon.empty()) {
					property_info.usage ^= PROPERTY_USAGE_STORAGE;
				}
				p_list->push_back(property_info);

				// physics_layer_%d/polygon_%d/one_way
				property_info = PropertyInfo(Variant::BOOL, vformat("physics_layer_%d/polygon_%d/%s", i, j, "one_way"));
				if (physics[i].polygons[j].one_way == false) {
					property_info.usage ^= PROPERTY_USAGE_STORAGE;
				}
				p_list->push_back(property_info);

				// physics_layer_%d/polygon_%d/one_way_margin
				property_info = PropertyInfo(Variant::REAL, vformat("physics_layer_%d/polygon_%d/%s", i, j, "one_way_margin"));
				if (physics[i].polygons[j].one_way_margin == 1.0) {
					property_info.usage ^= PROPERTY_USAGE_STORAGE;
				}
				p_list->push_back(property_info);
			}
		}

		// Terrain data
		if (terrain_set >= 0) {
			p_list->push_back(PropertyInfo(Variant::NIL, "Terrains", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
			for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
				LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
				if (is_valid_terrain_peering_bit(bit)) {
					property_info = PropertyInfo(Variant::INT, vformat("%s/%s", "terrains_peering_bit", LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]));
					if (get_terrain_peering_bit(bit) == -1) {
						property_info.usage ^= PROPERTY_USAGE_STORAGE;
					}
					p_list->push_back(property_info);
				}
			}
		}

		// Navigation layers.
		p_list->push_back(PropertyInfo(Variant::NIL, "Navigation", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
		for (int i = 0; i < navigation.size(); i++) {
			property_info = PropertyInfo(Variant::OBJECT, vformat("navigation_layer_%d/%s", i, "polygon"), PROPERTY_HINT_RESOURCE_TYPE, "NavigationPolygon", PROPERTY_USAGE_DEFAULT);
			if (navigation[i].navigation_polygon.is_null()) {
				property_info.usage ^= PROPERTY_USAGE_STORAGE;
			}
			p_list->push_back(property_info);
		}

		// Custom data layers.
		p_list->push_back(PropertyInfo(Variant::NIL, "Custom Data", PROPERTY_HINT_NONE, "custom_data_", PROPERTY_USAGE_GROUP));
		for (int i = 0; i < custom_data.size(); i++) {
			Variant default_val;
			Callable::CallError error;
			Variant::construct(custom_data[i].get_type(), default_val, nullptr, 0, error);
			property_info = PropertyInfo(tile_set->get_custom_data_layer_type(i), vformat("custom_data_%d", i), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_NIL_IS_VARIANT);
			if (custom_data[i] == default_val) {
				property_info.usage ^= PROPERTY_USAGE_STORAGE;
			}
			p_list->push_back(property_info);
		}
	}
}

void LayeredTileData::_bind_methods() {
	// Rendering.
	ClassDB::bind_method(D_METHOD("set_flip_h", "flip_h"), &LayeredTileData::set_flip_h);
	ClassDB::bind_method(D_METHOD("get_flip_h"), &LayeredTileData::get_flip_h);
	ClassDB::bind_method(D_METHOD("set_flip_v", "flip_v"), &LayeredTileData::set_flip_v);
	ClassDB::bind_method(D_METHOD("get_flip_v"), &LayeredTileData::get_flip_v);
	ClassDB::bind_method(D_METHOD("set_transpose", "transpose"), &LayeredTileData::set_transpose);
	ClassDB::bind_method(D_METHOD("get_transpose"), &LayeredTileData::get_transpose);
	ClassDB::bind_method(D_METHOD("set_material", "material"), &LayeredTileData::set_material);
	ClassDB::bind_method(D_METHOD("get_material"), &LayeredTileData::get_material);
	ClassDB::bind_method(D_METHOD("set_texture_origin", "texture_origin"), &LayeredTileData::set_texture_origin);
	ClassDB::bind_method(D_METHOD("get_texture_origin"), &LayeredTileData::get_texture_origin);
	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &LayeredTileData::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &LayeredTileData::get_modulate);
	ClassDB::bind_method(D_METHOD("set_z_index", "z_index"), &LayeredTileData::set_z_index);
	ClassDB::bind_method(D_METHOD("get_z_index"), &LayeredTileData::get_z_index);
	ClassDB::bind_method(D_METHOD("set_y_sort_origin", "y_sort_origin"), &LayeredTileData::set_y_sort_origin);
	ClassDB::bind_method(D_METHOD("get_y_sort_origin"), &LayeredTileData::get_y_sort_origin);

	ClassDB::bind_method(D_METHOD("set_occluder", "layer_id", "occluder_polygon"), &LayeredTileData::set_occluder);
	ClassDB::bind_method(D_METHOD("get_occluder", "layer_id", "flip_h", "flip_v", "transpose"), &LayeredTileData::get_occluder, DEFVAL(false), DEFVAL(false), DEFVAL(false));

	// Physics.
	ClassDB::bind_method(D_METHOD("set_constant_linear_velocity", "layer_id", "velocity"), &LayeredTileData::set_constant_linear_velocity);
	ClassDB::bind_method(D_METHOD("get_constant_linear_velocity", "layer_id"), &LayeredTileData::get_constant_linear_velocity);
	ClassDB::bind_method(D_METHOD("set_constant_angular_velocity", "layer_id", "velocity"), &LayeredTileData::set_constant_angular_velocity);
	ClassDB::bind_method(D_METHOD("get_constant_angular_velocity", "layer_id"), &LayeredTileData::get_constant_angular_velocity);
	ClassDB::bind_method(D_METHOD("set_collision_polygons_count", "layer_id", "polygons_count"), &LayeredTileData::set_collision_polygons_count);
	ClassDB::bind_method(D_METHOD("get_collision_polygons_count", "layer_id"), &LayeredTileData::get_collision_polygons_count);
	ClassDB::bind_method(D_METHOD("add_collision_polygon", "layer_id"), &LayeredTileData::add_collision_polygon);
	ClassDB::bind_method(D_METHOD("remove_collision_polygon", "layer_id", "polygon_index"), &LayeredTileData::remove_collision_polygon);
	ClassDB::bind_method(D_METHOD("set_collision_polygon_points", "layer_id", "polygon_index", "polygon"), &LayeredTileData::set_collision_polygon_points);
	ClassDB::bind_method(D_METHOD("get_collision_polygon_points", "layer_id", "polygon_index"), &LayeredTileData::get_collision_polygon_points);
	ClassDB::bind_method(D_METHOD("set_collision_polygon_one_way", "layer_id", "polygon_index", "one_way"), &LayeredTileData::set_collision_polygon_one_way);
	ClassDB::bind_method(D_METHOD("is_collision_polygon_one_way", "layer_id", "polygon_index"), &LayeredTileData::is_collision_polygon_one_way);
	ClassDB::bind_method(D_METHOD("set_collision_polygon_one_way_margin", "layer_id", "polygon_index", "one_way_margin"), &LayeredTileData::set_collision_polygon_one_way_margin);
	ClassDB::bind_method(D_METHOD("get_collision_polygon_one_way_margin", "layer_id", "polygon_index"), &LayeredTileData::get_collision_polygon_one_way_margin);

	// Terrain
	ClassDB::bind_method(D_METHOD("set_terrain_set", "terrain_set"), &LayeredTileData::set_terrain_set);
	ClassDB::bind_method(D_METHOD("get_terrain_set"), &LayeredTileData::get_terrain_set);
	ClassDB::bind_method(D_METHOD("set_terrain", "terrain"), &LayeredTileData::set_terrain);
	ClassDB::bind_method(D_METHOD("get_terrain"), &LayeredTileData::get_terrain);
	ClassDB::bind_method(D_METHOD("set_terrain_peering_bit", "peering_bit", "terrain"), &LayeredTileData::set_terrain_peering_bit);
	ClassDB::bind_method(D_METHOD("get_terrain_peering_bit", "peering_bit"), &LayeredTileData::get_terrain_peering_bit);

	// Navigation
	ClassDB::bind_method(D_METHOD("set_navigation_polygon", "layer_id", "navigation_polygon"), &LayeredTileData::set_navigation_polygon);
	ClassDB::bind_method(D_METHOD("get_navigation_polygon", "layer_id", "flip_h", "flip_v", "transpose"), &LayeredTileData::get_navigation_polygon, DEFVAL(false), DEFVAL(false), DEFVAL(false));

	// Misc.
	ClassDB::bind_method(D_METHOD("set_probability", "probability"), &LayeredTileData::set_probability);
	ClassDB::bind_method(D_METHOD("get_probability"), &LayeredTileData::get_probability);

	// Custom data.
	ClassDB::bind_method(D_METHOD("set_custom_data", "layer_name", "value"), &LayeredTileData::set_custom_data);
	ClassDB::bind_method(D_METHOD("get_custom_data", "layer_name"), &LayeredTileData::get_custom_data);
	ClassDB::bind_method(D_METHOD("set_custom_data_by_layer_id", "layer_id", "value"), &LayeredTileData::set_custom_data_by_layer_id);
	ClassDB::bind_method(D_METHOD("get_custom_data_by_layer_id", "layer_id"), &LayeredTileData::get_custom_data_by_layer_id);

	ADD_GROUP("Rendering", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transpose"), "set_transpose", "get_transpose");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "texture_origin", PROPERTY_HINT_NONE, "suffix:px"), "set_texture_origin", "get_texture_origin");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "CanvasItemMaterial,ShaderMaterial"), "set_material", "get_material");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "z_index"), "set_z_index", "get_z_index");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y_sort_origin"), "set_y_sort_origin", "get_y_sort_origin");

	ADD_GROUP("Terrains", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "terrain_set"), "set_terrain_set", "get_terrain_set");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "terrain"), "set_terrain", "get_terrain");

	ADD_GROUP("Miscellaneous", "");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "probability"), "set_probability", "get_probability");

	ADD_SIGNAL(MethodInfo("changed"));
}
