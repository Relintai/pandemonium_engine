/*************************************************************************/
/*  layered_tile_set_atlas_source_editor.cpp                             */
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

#include "layered_tile_set_atlas_source_editor.h"

#include "layered_tiles_editor_plugin.h"

#include "editor/editor_inspector.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

#include "editor/editor_scale.h"
#include "editor/progress_dialog.h"
#include "layered_tile_set_editor.h"

#include "core/input/input.h"

#include "core/object/undo_redo.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/separator.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/main/control.h"

#include "core/core_string_names.h"
#include "core/math/geometry.h"
#include "core/os/keyboard.h"

#include "scene/resources/navigation_2d/navigation_mesh_source_geometry_data_2d.h"
#include "servers/navigation/navigation_mesh_generator.h"

#include "servers/navigation_2d_server.h"

#include "modules/modules_enabled.gen.h"

#include <limits.h>

void LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::set_id(int p_id) {
	ERR_FAIL_COND(p_id < 0);
	if (source_id == p_id) {
		return;
	}
	ERR_FAIL_COND_MSG(tile_set->has_source(p_id), vformat("Cannot change LayeredTileSet Atlas Source ID. Another source exists with id %d.", p_id));

	int previous_source = source_id;
	source_id = p_id; // source_id must be updated before, because it's used by the source list update.
	tile_set->set_source_id(previous_source, p_id);
	emit_signal("changed", "id");
}

int LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::get_id() const {
	return source_id;
}

void LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::_changed_callback(Object *p_changed, const char *p_prop) {
	property_list_changed_notify();
}

bool LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "id") {
		set_id(p_value);
		return true;
	}
	String name = p_name;
	if (name == "name") {
		// Use the resource_name property to store the source's name.
		name = "resource_name";
	}
	bool valid = false;
	tile_set_atlas_source->set(name, p_value, &valid);
	if (valid) {
		emit_signal("changed", String(name).utf8().get_data());
	}
	return valid;
}

bool LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::_get(const StringName &p_name, Variant &r_ret) const {
	if (!tile_set_atlas_source.is_valid()) {
		return false;
	}
	if (p_name == "id") {
		r_ret = get_id();
		return true;
	}
	String name = p_name;
	if (name == "name") {
		// Use the resource_name property to store the source's name.
		name = "resource_name";
	}
	bool valid = false;
	r_ret = tile_set_atlas_source->get(name, &valid);
	return valid;
}

void LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::NIL, TTR("Atlas"), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_CATEGORY));
	p_list->push_back(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_RANGE, "0," + itos(INT_MAX) + ",1"));
	p_list->push_back(PropertyInfo(Variant::STRING, "name"));
	p_list->push_back(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"));
	p_list->push_back(PropertyInfo(Variant::VECTOR2I, "margins", PROPERTY_HINT_NONE, "suffix:px"));
	p_list->push_back(PropertyInfo(Variant::VECTOR2I, "separation", PROPERTY_HINT_NONE, "suffix:px"));
	p_list->push_back(PropertyInfo(Variant::VECTOR2I, "texture_region_size", PROPERTY_HINT_NONE, "suffix:px"));
	p_list->push_back(PropertyInfo(Variant::BOOL, "use_texture_padding"));
}

void LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::_bind_methods() {
	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "what")));
}

void LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceProxyObject::edit(Ref<LayeredTileSet> p_tile_set, Ref<LayeredTileSetAtlasSource> p_tile_set_atlas_source, int p_source_id) {
	ERR_FAIL_COND(!p_tile_set_atlas_source.is_valid());
	ERR_FAIL_COND(p_source_id < 0);
	ERR_FAIL_COND(p_tile_set.is_valid() && p_tile_set->get_source(p_source_id) != p_tile_set_atlas_source);

	if (p_tile_set == tile_set && p_tile_set_atlas_source == tile_set_atlas_source && p_source_id == source_id) {
		return;
	}

	// Disconnect to changes.
	if (tile_set_atlas_source.is_valid()) {
		tile_set_atlas_source->remove_change_receptor(this);
	}

	tile_set = p_tile_set;
	tile_set_atlas_source = p_tile_set_atlas_source;
	source_id = p_source_id;

	// Connect to changes.
	if (tile_set_atlas_source.is_valid()) {
		tile_set_atlas_source->remove_change_receptor(this);
		tile_set_atlas_source->add_change_receptor(this);
	}

	property_list_changed_notify();
}

// -- Proxy object used by the tile inspector --
bool LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject::_set(const StringName &p_name, const Variant &p_value) {
	if (!tile_set_atlas_source.is_valid()) {
		return false;
	}

	// ID and size related properties.
	if (tiles.size() == 1) {
		const Vector2i coords = tiles.front()->get().tile;
		const int &alternative = tiles.front()->get().alternative;

		if (alternative == 0) {
			Vector<String> components = String(p_name).split("/", true, 2);
			if (p_name == "atlas_coords") {
				Vector2i as_vector2i = p_value;
				bool has_room_for_tile = tile_set_atlas_source->has_room_for_tile(as_vector2i, tile_set_atlas_source->get_tile_size_in_atlas(coords), tile_set_atlas_source->get_tile_animation_columns(coords), tile_set_atlas_source->get_tile_animation_separation(coords), tile_set_atlas_source->get_tile_animation_frames_count(coords), coords);
				ERR_FAIL_COND_V_MSG(!has_room_for_tile, false, "Cannot move the tile, invalid coordinates or not enough room in the atlas for the tile and its animation frames.");

				if (tiles_set_atlas_source_editor->selection.front()->get().tile == coords) {
					tiles_set_atlas_source_editor->selection.clear();
					tiles_set_atlas_source_editor->selection.insert({ as_vector2i, 0 });
					tiles_set_atlas_source_editor->_update_tile_id_label();
				}

				tile_set_atlas_source->move_tile_in_atlas(coords, as_vector2i);
				tiles.clear();
				tiles.insert({ as_vector2i, 0 });
				emit_signal("changed", "atlas_coords");
				return true;
			} else if (p_name == "size_in_atlas") {
				Vector2i as_vector2i = p_value;
				bool has_room_for_tile = tile_set_atlas_source->has_room_for_tile(coords, as_vector2i, tile_set_atlas_source->get_tile_animation_columns(coords), tile_set_atlas_source->get_tile_animation_separation(coords), tile_set_atlas_source->get_tile_animation_frames_count(coords), coords);
				ERR_FAIL_COND_V_MSG(!has_room_for_tile, false, "Invalid size or not enough room in the atlas for the tile.");
				tile_set_atlas_source->move_tile_in_atlas(coords, LayeredTileSetSource::INVALID_ATLAS_COORDS, as_vector2i);
				emit_signal("changed", "size_in_atlas");
				return true;
			}
		} else if (alternative > 0) {
			if (p_name == "alternative_id") {
				int as_int = int(p_value);
				ERR_FAIL_COND_V(as_int < 0, false);
				ERR_FAIL_COND_V_MSG(tile_set_atlas_source->has_alternative_tile(coords, as_int), false, vformat("Cannot change alternative tile ID. Another alternative exists with id %d for tile at coords %s.", as_int, coords));

				if (tiles_set_atlas_source_editor->selection.front()->get().alternative == alternative) {
					tiles_set_atlas_source_editor->selection.clear();
					tiles_set_atlas_source_editor->selection.insert({ coords, as_int });
				}

				int previous_alternative_tile = alternative;
				tiles.clear();
				tiles.insert({ coords, as_int }); // tiles must be updated before.
				tile_set_atlas_source->set_alternative_tile_id(coords, previous_alternative_tile, as_int);

				emit_signal("changed", "alternative_id");
				return true;
			}
		}
	}

	// Animation.
	// Check if all tiles have an alternative_id of 0.
	bool all_alternatve_id_zero = true;

	for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		TileSelection tile = E->get();

		if (tile.alternative != 0) {
			all_alternatve_id_zero = false;
			break;
		}
	}

	if (all_alternatve_id_zero) {
		Vector<String> components = String(p_name).split("/", true, 2);
		if (p_name == "animation_columns") {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				bool has_room_for_tile = tile_set_atlas_source->has_room_for_tile(tile.tile, tile_set_atlas_source->get_tile_size_in_atlas(tile.tile), p_value, tile_set_atlas_source->get_tile_animation_separation(tile.tile), tile_set_atlas_source->get_tile_animation_frames_count(tile.tile), tile.tile);
				if (!has_room_for_tile) {
					ERR_PRINT(vformat("Cannot change the number of columns to %s for tile animation. Not enough room in the atlas to layout %s frame(s).", p_value, tile_set_atlas_source->get_tile_animation_frames_count(tile.tile)));
				} else {
					tile_set_atlas_source->set_tile_animation_columns(tile.tile, p_value);
				}
			}
			emit_signal("changed", "animation_columns");
			return true;
		} else if (p_name == "animation_separation") {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				bool has_room_for_tile = tile_set_atlas_source->has_room_for_tile(tile.tile, tile_set_atlas_source->get_tile_size_in_atlas(tile.tile), tile_set_atlas_source->get_tile_animation_columns(tile.tile), p_value, tile_set_atlas_source->get_tile_animation_frames_count(tile.tile), tile.tile);
				if (!has_room_for_tile) {
					ERR_PRINT(vformat("Cannot change separation between frames of the animation to %s. Not enough room in the atlas to layout %s frame(s).", p_value, tile_set_atlas_source->get_tile_animation_frames_count(tile.tile)));
				} else {
					tile_set_atlas_source->set_tile_animation_separation(tile.tile, p_value);
				}
			}
			emit_signal("changed", "animation_separation");
			return true;
		} else if (p_name == "animation_speed") {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				tile_set_atlas_source->set_tile_animation_speed(tile.tile, p_value);
			}
			emit_signal("changed", "animation_speed");
			return true;
		} else if (p_name == "animation_mode") {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				tile_set_atlas_source->set_tile_animation_mode(tile.tile, VariantCaster<LayeredTileSetAtlasSource::TileAnimationMode>::cast(p_value));
			}
			emit_signal("changed", "animation_mode");
			return true;
		} else if (p_name == "animation_frames_count") {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				int frame_count = p_value;
				if (frame_count == 0) {
					frame_count = 1;
				}

				bool has_room_for_tile = tile_set_atlas_source->has_room_for_tile(tile.tile, tile_set_atlas_source->get_tile_size_in_atlas(tile.tile), tile_set_atlas_source->get_tile_animation_columns(tile.tile), tile_set_atlas_source->get_tile_animation_separation(tile.tile), frame_count, tile.tile);
				if (!has_room_for_tile) {
					ERR_PRINT(vformat("Cannot add frames to the animation, not enough room in the atlas to layout %s frames.", frame_count));
				} else {
					tile_set_atlas_source->set_tile_animation_frames_count(tile.tile, frame_count);
				}
			}
			property_list_changed_notify();
			emit_signal("changed", "animation_separation");
			return true;
		} else if (components.size() == 2 && components[0].begins_with("animation_frame_") && components[0].trim_prefix("animation_frame_").is_valid_integer()) {
			for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
				TileSelection tile = E->get();

				int frame = components[0].trim_prefix("animation_frame_").to_int();
				if (frame < 0 || frame >= tile_set_atlas_source->get_tile_animation_frames_count(tile.tile)) {
					ERR_PRINT(vformat("No tile animation frame with index %d", frame));
				} else {
					if (components[1] == "duration") {
						tile_set_atlas_source->set_tile_animation_frame_duration(tile.tile, frame, p_value);
					}
				}
			}
			return true;
		}
	}

	// Other properties.
	bool any_valid = false;

	for (const RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		const Vector2i &coords = E->get().tile;
		const int &alternative = E->get().alternative;

		bool valid = false;
		LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);
		ERR_FAIL_NULL_V(tile_data, false);
		tile_data->set(p_name, p_value, &valid);

		any_valid |= valid;
	}

	if (any_valid) {
		emit_signal("changed", String(p_name).utf8().get_data());
	}

	return any_valid;
}

bool LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject::_get(const StringName &p_name, Variant &r_ret) const {
	if (!tile_set_atlas_source.is_valid()) {
		return false;
	}

	// ID and size related properties.s
	if (tiles.size() == 1) {
		const Vector2i &coords = tiles.front()->get().tile;
		const int &alternative = tiles.front()->get().alternative;

		if (alternative == 0) {
			Vector<String> components = String(p_name).split("/", true, 2);
			if (p_name == "atlas_coords") {
				r_ret = coords;
				return true;
			} else if (p_name == "size_in_atlas") {
				r_ret = tile_set_atlas_source->get_tile_size_in_atlas(coords);
				return true;
			}
		} else if (alternative > 0) {
			if (p_name == "alternative_id") {
				r_ret = alternative;
				return true;
			}
		}
	}

	// Animation.
	// Check if all tiles have an alternative_id of 0.
	bool all_alternatve_id_zero = true;
	for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		TileSelection tile = E->get();

		if (tile.alternative != 0) {
			all_alternatve_id_zero = false;
			break;
		}
	}

	if (all_alternatve_id_zero) {
		const Vector2i &coords = tiles.front()->get().tile;

		Vector<String> components = String(p_name).split("/", true, 2);
		if (p_name == "animation_columns") {
			r_ret = tile_set_atlas_source->get_tile_animation_columns(coords);
			return true;
		} else if (p_name == "animation_separation") {
			r_ret = tile_set_atlas_source->get_tile_animation_separation(coords);
			return true;
		} else if (p_name == "animation_speed") {
			r_ret = tile_set_atlas_source->get_tile_animation_speed(coords);
			return true;
		} else if (p_name == "animation_mode") {
			r_ret = tile_set_atlas_source->get_tile_animation_mode(coords);
			return true;
		} else if (p_name == "animation_frames_count") {
			r_ret = tile_set_atlas_source->get_tile_animation_frames_count(coords);
			return true;
		} else if (components.size() == 2 && components[0].begins_with("animation_frame_") && components[0].trim_prefix("animation_frame_").is_valid_integer()) {
			int frame = components[0].trim_prefix("animation_frame_").to_int();
			if (components[1] == "duration") {
				if (frame < 0 || frame >= tile_set_atlas_source->get_tile_animation_frames_count(coords)) {
					return false;
				}
				r_ret = tile_set_atlas_source->get_tile_animation_frame_duration(coords, frame);
				return true;
			}
		}
	}

	for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		// Return the first tile with a property matching the name.
		// Note: It's a little bit annoying, but the behavior is the same the one in MultiNodeEdit.
		const Vector2i &coords = E->get().tile;
		const int &alternative = E->get().alternative;

		LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);
		ERR_FAIL_NULL_V(tile_data, false);

		bool valid = false;
		r_ret = tile_data->get(p_name, &valid);
		if (valid) {
			return true;
		}
	}

	return false;
}

void LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject::_get_property_list(List<PropertyInfo> *p_list) const {
	if (!tile_set_atlas_source.is_valid()) {
		return;
	}

	// ID and size related properties.
	if (tiles.size() == 1) {
		if (tiles.front()->get().alternative == 0) {
			p_list->push_back(PropertyInfo(Variant::NIL, TTR("Base Tile"), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_CATEGORY));
			p_list->push_back(PropertyInfo(Variant::VECTOR2I, "atlas_coords"));
			p_list->push_back(PropertyInfo(Variant::VECTOR2I, "size_in_atlas"));
		} else {
			p_list->push_back(PropertyInfo(Variant::NIL, TTR("Alternative Tile"), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_CATEGORY));
			p_list->push_back(PropertyInfo(Variant::INT, "alternative_id"));
		}
	} else {
		p_list->push_back(PropertyInfo(Variant::NIL, TTR("Tiles"), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_CATEGORY));
	}

	// Animation.
	// Check if all tiles have an alternative_id of 0.
	bool all_alternatve_id_zero = true;
	for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		TileSelection tile = E->get();

		if (tile.alternative != 0) {
			all_alternatve_id_zero = false;
			break;
		}
	}

	if (all_alternatve_id_zero) {
		p_list->push_back(PropertyInfo(Variant::NIL, "Animation", PROPERTY_HINT_NONE, "animation_", PROPERTY_USAGE_GROUP));
		p_list->push_back(PropertyInfo(Variant::INT, "animation_columns"));
		p_list->push_back(PropertyInfo(Variant::VECTOR2I, "animation_separation"));
		p_list->push_back(PropertyInfo(Variant::REAL, "animation_speed"));
		p_list->push_back(PropertyInfo(Variant::INT, "animation_mode", PROPERTY_HINT_ENUM, "Default,Random Start Times"));
		p_list->push_back(PropertyInfo(Variant::INT, "animation_frames_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_ARRAY, "Frames,animation_frame_"));
		// Not optimal, but returns value for the first tile. This is similar to what MultiNodeEdit does.
		if (tile_set_atlas_source->get_tile_animation_frames_count(tiles.front()->get().tile) == 1) {
			p_list->push_back(PropertyInfo(Variant::REAL, "animation_frame_0/duration", PROPERTY_HINT_NONE, "suffix:s"));
		} else {
			for (int i = 0; i < tile_set_atlas_source->get_tile_animation_frames_count(tiles.front()->get().tile); i++) {
				p_list->push_back(PropertyInfo(Variant::REAL, vformat("animation_frame_%d/%s", i, "duration"), PROPERTY_HINT_NONE, "suffix:s"));
			}
		}
	}

	// Get the list of properties common to all tiles (similar to what's done in MultiNodeEdit).
	struct PropertyId {
		int occurence_id = 0;
		String property;
		bool operator<(const PropertyId &p_other) const {
			return occurence_id == p_other.occurence_id ? property < p_other.property : occurence_id < p_other.occurence_id;
		}
	};
	struct PLData {
		int uses = 0;
		PropertyInfo property_info;
	};
	RBMap<PropertyId, PLData> usage;

	List<PLData *> data_list;
	for (RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		const Vector2i &coords = E->get().tile;
		const int &alternative = E->get().alternative;

		LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);
		ERR_FAIL_NULL(tile_data);

		List<PropertyInfo> list;
		tile_data->get_property_list(&list);

		HashMap<String, int> counts; // Counts the number of time a property appears (useful for groups that may appear more than once)
		for (List<PropertyInfo>::Element *E_property = list.front(); E_property; E_property = E_property->next()) {
			// Don't show category for TileData.
			if (E_property->get().usage & PROPERTY_USAGE_CATEGORY) {
				continue;
			}

			const String &property_string = E_property->get().name;
			if (!tile_data->is_allowing_transform() && (property_string == "flip_h" || property_string == "flip_v" || property_string == "transpose")) {
				continue;
			}

			if (!counts.has(property_string)) {
				counts[property_string] = 1;
			} else {
				counts[property_string] += 1;
			}

			PropertyInfo stored_property_info = E_property->get();
			stored_property_info.usage |= PROPERTY_USAGE_STORAGE; // Ignore the storage flag in comparing properties.

			PropertyId id = { counts[property_string], property_string };
			if (!usage.has(id)) {
				usage[id] = { 1, stored_property_info };
				data_list.push_back(&usage[id]);
			} else if (usage[id].property_info == stored_property_info) {
				usage[id].uses += 1;
			}
		}
	}

	// Add only properties that are common to all tiles.

	for (const List<PLData *>::Element *E = data_list.front(); E; E = E->next()) {
		if (E->get()->uses == tiles.size()) {
			p_list->push_back(E->get()->property_info);
		}
	}
}

void LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject::edit(Ref<LayeredTileSetAtlasSource> p_tile_set_atlas_source, const RBSet<TileSelection> &p_tiles) {
	ERR_FAIL_COND(!p_tile_set_atlas_source.is_valid());
	ERR_FAIL_COND(p_tiles.empty());

	for (const RBSet<TileSelection>::Element *E = p_tiles.front(); E; E = E->next()) {
		ERR_FAIL_COND(E->get().tile == LayeredTileSetSource::INVALID_ATLAS_COORDS);
		ERR_FAIL_COND(E->get().alternative < 0);
	}

	// Disconnect to changes.
	for (const RBSet<TileSelection>::Element *E = tiles.front(); E; E = E->next()) {
		const Vector2i &coords = E->get().tile;
		const int &alternative = E->get().alternative;

		if (tile_set_atlas_source.is_valid() && tile_set_atlas_source->has_tile(coords) && tile_set_atlas_source->has_alternative_tile(coords, alternative)) {
			LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);

			tile_data->remove_change_receptor(this);
		}
	}

	tile_set_atlas_source = p_tile_set_atlas_source;
	tiles = RBSet<TileSelection>(p_tiles);

	// Connect to changes.
	for (const RBSet<TileSelection>::Element *E = p_tiles.front(); E; E = E->next()) {
		const Vector2i &coords = E->get().tile;
		const int &alternative = E->get().alternative;

		if (tile_set_atlas_source->has_tile(coords) && tile_set_atlas_source->has_alternative_tile(coords, alternative)) {
			LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);

			tile_data->remove_change_receptor(this);
			tile_data->add_change_receptor(this);
		}
	}

	property_list_changed_notify();
}

void LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject::_bind_methods() {
	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "what")));
}

void LayeredTileSetAtlasSourceEditor::_inspector_property_selected(const String &p_property) {
	selected_property = p_property;
	_update_atlas_view();
	_update_current_tile_data_editor();
}

void LayeredTileSetAtlasSourceEditor::_update_tile_id_label() {
	if (selection.size() == 1) {
		TileSelection selected = selection.front()->get();
		tool_tile_id_label->set_text(vformat("%d, %s, %d", tile_set_atlas_source_id, selected.tile, selected.alternative));
		tool_tile_id_label->set_tooltip(vformat(TTR("Selected tile:\nSource: %d\nAtlas coordinates: %s\nAlternative: %d"), tile_set_atlas_source_id, selected.tile, selected.alternative));
		tool_tile_id_label->show();
	} else {
		tool_tile_id_label->hide();
	}
}

void LayeredTileSetAtlasSourceEditor::_update_source_inspector() {
	// Update the proxy object.
	atlas_source_proxy_object->edit(tile_set, tile_set_atlas_source, tile_set_atlas_source_id);
}

void LayeredTileSetAtlasSourceEditor::_update_fix_selected_and_hovered_tiles() {
	// Fix selected.
	for (RBSet<TileSelection>::Element *E = selection.front(); E;) {
		RBSet<TileSelection>::Element *N = E->next();
		TileSelection selected = E->get();
		if (!tile_set_atlas_source->has_tile(selected.tile) || !tile_set_atlas_source->has_alternative_tile(selected.tile, selected.alternative)) {
			selection.erase(E);
		}
		E = N;
	}

	// Fix hovered.
	if (!tile_set_atlas_source->has_tile(hovered_base_tile_coords)) {
		hovered_base_tile_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS;
	}
	Vector2i coords = Vector2i(hovered_alternative_tile_coords.x, hovered_alternative_tile_coords.y);
	int alternative = hovered_alternative_tile_coords.z;
	if (!tile_set_atlas_source->has_tile(coords) || !tile_set_atlas_source->has_alternative_tile(coords, alternative)) {
		hovered_alternative_tile_coords = Vector3i(LayeredTileSetSource::INVALID_ATLAS_COORDS.x, LayeredTileSetSource::INVALID_ATLAS_COORDS.y, LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);
	}
}

void LayeredTileSetAtlasSourceEditor::_update_atlas_source_inspector() {
	// Update visibility.
	bool inspector_visible = tools_button_group->get_pressed_button() == tool_setup_atlas_source_button;
	atlas_source_inspector->set_visible(inspector_visible);
	atlas_source_inspector->set_read_only(read_only);
}

void LayeredTileSetAtlasSourceEditor::_update_tile_inspector() {
	// Update visibility.
	if (tools_button_group->get_pressed_button() == tool_select_button) {
		if (!selection.empty()) {
			tile_proxy_object->edit(tile_set_atlas_source, selection);
		}
		tile_inspector->set_visible(!selection.empty());
		tile_inspector_no_tile_selected_label->set_visible(selection.empty());
	} else {
		tile_inspector->hide();
		tile_inspector_no_tile_selected_label->hide();
	}
	tile_inspector->set_read_only(read_only);
}

void LayeredTileSetAtlasSourceEditor::_update_tile_data_editors() {
	String previously_selected;
	if (tile_data_editors_tree && tile_data_editors_tree->get_selected()) {
		previously_selected = tile_data_editors_tree->get_selected()->get_metadata(0);
	}

	tile_data_editors_tree->clear();

	if (tile_set.is_null()) {
		return;
	}

	TreeItem *root = tile_data_editors_tree->create_item();

	TreeItem *group;
#define ADD_TILE_DATA_EDITOR_GROUP(text)               \
	group = tile_data_editors_tree->create_item(root); \
	group->set_custom_bg_color(0, group_color);        \
	group->set_selectable(0, false);                   \
	group->set_disable_folding(true);                  \
	group->set_text(0, text);

	TreeItem *item;
#define ADD_TILE_DATA_EDITOR(parent, text, property)    \
	item = tile_data_editors_tree->create_item(parent); \
	item->set_text(0, text);                            \
	item->set_metadata(0, property);                    \
	if (property == previously_selected) {              \
		item->select(0);                                \
	}

	// Theming.
	tile_data_editors_tree->add_theme_constant_override("v_separation", 1);
	tile_data_editors_tree->add_theme_constant_override("h_separation", 3);

	Color group_color = get_theme_color("prop_category", "EditorTheme");

	// List of editors.
	// --- Rendering ---
	ADD_TILE_DATA_EDITOR_GROUP(TTR("Rendering"));

	ADD_TILE_DATA_EDITOR(group, TTR("Texture Origin"), "texture_origin");
	if (!tile_data_editors.has("texture_origin")) {
		TileDataTextureOriginEditor *tile_data_texture_origin_editor = memnew(TileDataTextureOriginEditor);
		tile_data_texture_origin_editor->hide();
		tile_data_texture_origin_editor->setup_property_editor(Variant::VECTOR2I, "texture_origin");
		tile_data_texture_origin_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_texture_origin_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["texture_origin"] = tile_data_texture_origin_editor;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Modulate"), "modulate");
	if (!tile_data_editors.has("modulate")) {
		TileDataDefaultEditor *tile_data_modulate_editor = memnew(TileDataDefaultEditor());
		tile_data_modulate_editor->hide();
		tile_data_modulate_editor->setup_property_editor(Variant::COLOR, "modulate", "", Color(1.0, 1.0, 1.0, 1.0));
		tile_data_modulate_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_modulate_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["modulate"] = tile_data_modulate_editor;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Z Index"), "z_index");
	if (!tile_data_editors.has("z_index")) {
		TileDataDefaultEditor *tile_data_z_index_editor = memnew(TileDataDefaultEditor());
		tile_data_z_index_editor->hide();
		tile_data_z_index_editor->setup_property_editor(Variant::INT, "z_index");
		tile_data_z_index_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_z_index_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["z_index"] = tile_data_z_index_editor;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Y Sort Origin"), "y_sort_origin");
	if (!tile_data_editors.has("y_sort_origin")) {
		TileDataYSortEditor *tile_data_y_sort_editor = memnew(TileDataYSortEditor);
		tile_data_y_sort_editor->hide();
		tile_data_y_sort_editor->setup_property_editor(Variant::INT, "y_sort_origin");
		tile_data_y_sort_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_y_sort_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["y_sort_origin"] = tile_data_y_sort_editor;
	}

	for (int i = 0; i < tile_set->get_occlusion_layers_count(); i++) {
		ADD_TILE_DATA_EDITOR(group, vformat(TTR("Occlusion Layer %d"), i), vformat("occlusion_layer_%d", i));
		if (!tile_data_editors.has(vformat("occlusion_layer_%d", i))) {
			TileDataOcclusionShapeEditor *tile_data_occlusion_shape_editor = memnew(TileDataOcclusionShapeEditor());
			tile_data_occlusion_shape_editor->hide();
			tile_data_occlusion_shape_editor->set_occlusion_layer(i);
			tile_data_occlusion_shape_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
			tile_data_occlusion_shape_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
			tile_data_editors[vformat("occlusion_layer_%d", i)] = tile_data_occlusion_shape_editor;
		}
	}
	for (int i = tile_set->get_occlusion_layers_count(); tile_data_editors.has(vformat("occlusion_layer_%d", i)); i++) {
		tile_data_editors[vformat("occlusion_layer_%d", i)]->queue_delete();
		tile_data_editors.erase(vformat("occlusion_layer_%d", i));
	}

	// --- Rendering ---
	ADD_TILE_DATA_EDITOR(root, TTR("Terrains"), "terrain_set");
	if (!tile_data_editors.has("terrain_set")) {
		TileDataTerrainsEditor *tile_data_terrains_editor = memnew(TileDataTerrainsEditor);
		tile_data_terrains_editor->hide();
		tile_data_terrains_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_terrains_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["terrain_set"] = tile_data_terrains_editor;
	}

	// --- Miscellaneous ---
	ADD_TILE_DATA_EDITOR(root, TTR("Probability"), "probability");
	if (!tile_data_editors.has("probability")) {
		TileDataDefaultEditor *tile_data_probability_editor = memnew(TileDataDefaultEditor());
		tile_data_probability_editor->hide();
		tile_data_probability_editor->setup_property_editor(Variant::REAL, "probability", "", 1.0);
		tile_data_probability_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tile_data_probability_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["probability"] = tile_data_probability_editor;
	}

	Color disabled_color = get_theme_color("font_disabled_color", "EditorTheme");

	// --- Physics ---
	ADD_TILE_DATA_EDITOR_GROUP(TTR("Physics"));
	for (int i = 0; i < tile_set->get_physics_layers_count(); i++) {
		ADD_TILE_DATA_EDITOR(group, vformat(TTR("Physics Layer %d"), i), vformat("physics_layer_%d", i));
		if (!tile_data_editors.has(vformat("physics_layer_%d", i))) {
			TileDataCollisionEditor *tile_data_collision_editor = memnew(TileDataCollisionEditor());
			tile_data_collision_editor->hide();
			tile_data_collision_editor->set_physics_layer(i);
			tile_data_collision_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
			tile_data_collision_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
			tile_data_editors[vformat("physics_layer_%d", i)] = tile_data_collision_editor;
		}
	}
	for (int i = tile_set->get_physics_layers_count(); tile_data_editors.has(vformat("physics_layer_%d", i)); i++) {
		tile_data_editors[vformat("physics_layer_%d", i)]->queue_delete();
		tile_data_editors.erase(vformat("physics_layer_%d", i));
	}

	if (tile_set->get_physics_layers_count() == 0) {
		item = tile_data_editors_tree->create_item(group);
		item->set_icon(0, get_theme_icon("Info", "EditorIcons"));
		item->set_icon_modulate(0, disabled_color);
		item->set_text(0, TTR("No physics layers"));
		item->set_tooltip(0, TTR("Create and customize physics layers in the inspector of the LayeredTileSet resource."));
		item->set_selectable(0, false);
		item->set_custom_color(0, disabled_color);
	}

	// --- Navigation ---
	ADD_TILE_DATA_EDITOR_GROUP(TTR("Navigation"));
	for (int i = 0; i < tile_set->get_navigation_layers_count(); i++) {
		ADD_TILE_DATA_EDITOR(group, vformat(TTR("Navigation Layer %d"), i), vformat("navigation_layer_%d", i));
		if (!tile_data_editors.has(vformat("navigation_layer_%d", i))) {
			TileDataNavigationEditor *tile_data_navigation_editor = memnew(TileDataNavigationEditor());
			tile_data_navigation_editor->hide();
			tile_data_navigation_editor->set_navigation_layer(i);
			tile_data_navigation_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
			tile_data_navigation_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
			tile_data_editors[vformat("navigation_layer_%d", i)] = tile_data_navigation_editor;
		}
	}
	for (int i = tile_set->get_navigation_layers_count(); tile_data_editors.has(vformat("navigation_layer_%d", i)); i++) {
		tile_data_editors[vformat("navigation_layer_%d", i)]->queue_delete();
		tile_data_editors.erase(vformat("navigation_layer_%d", i));
	}

	if (tile_set->get_navigation_layers_count() == 0) {
		item = tile_data_editors_tree->create_item(group);
		item->set_icon(0, get_theme_icon("Info", "EditorIcons"));
		item->set_icon_modulate(0, disabled_color);
		item->set_text(0, TTR("No navigation layers"));
		item->set_tooltip(0, TTR("Create and customize navigation layers in the inspector of the LayeredTileSet resource."));
		item->set_selectable(0, false);
		item->set_custom_color(0, disabled_color);
	}

	// --- Avoidance ---
	ADD_TILE_DATA_EDITOR_GROUP(TTR("Avoidance"));
	for (int i = 0; i < tile_set->get_avoidance_layers_count(); i++) {
		ADD_TILE_DATA_EDITOR(group, vformat(TTR("Avoidance Layer %d"), i), vformat("avoidance_layer_%d", i));
		if (!tile_data_editors.has(vformat("avoidance_layer_%d", i))) {
			TileDataAvoidanceEditor *tile_data_avoidance_editor = memnew(TileDataAvoidanceEditor());
			tile_data_avoidance_editor->hide();
			tile_data_avoidance_editor->set_avoidance_layer(i);
			tile_data_avoidance_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
			tile_data_avoidance_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
			tile_data_editors[vformat("avoidance_layer_%d", i)] = tile_data_avoidance_editor;
		}
	}
	for (int i = tile_set->get_avoidance_layers_count(); tile_data_editors.has(vformat("avoidance_layer_%d", i)); i++) {
		tile_data_editors[vformat("avoidance_layer_%d", i)]->queue_delete();
		tile_data_editors.erase(vformat("avoidance_layer_%d", i));
	}

	if (tile_set->get_avoidance_layers_count() == 0) {
		item = tile_data_editors_tree->create_item(group);
		item->set_icon(0, get_theme_icon("Info", "EditorIcons"));
		item->set_icon_modulate(0, disabled_color);
		item->set_text(0, TTR("No avoidance layers"));
		item->set_tooltip(0, TTR("Create and customize avoidance layers in the inspector of the LayeredTileSet resource."));
		item->set_selectable(0, false);
		item->set_custom_color(0, disabled_color);
	}

	// --- Custom Data ---
	ADD_TILE_DATA_EDITOR_GROUP(TTR("Custom Data"));
	for (int i = 0; i < tile_set->get_custom_data_layers_count(); i++) {
		String editor_name = vformat("custom_data_%d", i);
		String prop_name = tile_set->get_custom_data_layer_name(i);
		Variant::Type prop_type = tile_set->get_custom_data_layer_type(i);

		if (prop_name.empty()) {
			ADD_TILE_DATA_EDITOR(group, vformat(TTR("Custom Data %d"), i), editor_name);
		} else {
			ADD_TILE_DATA_EDITOR(group, prop_name, editor_name);
		}

		// If the type of the edited property has been changed, delete the
		// editor and create a new one.
		if (tile_data_editors.has(editor_name) && ((TileDataDefaultEditor *)tile_data_editors[editor_name])->get_property_type() != prop_type) {
			tile_data_editors[vformat("custom_data_%d", i)]->queue_delete();
			tile_data_editors.erase(vformat("custom_data_%d", i));
		}
		if (!tile_data_editors.has(editor_name)) {
			TileDataDefaultEditor *tile_data_custom_data_editor = memnew(TileDataDefaultEditor());
			tile_data_custom_data_editor->hide();
			tile_data_custom_data_editor->setup_property_editor(prop_type, editor_name, prop_name);
			tile_data_custom_data_editor->connect("needs_redraw", tile_atlas_control_unscaled, "update");
			tile_data_custom_data_editor->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
			tile_data_editors[editor_name] = tile_data_custom_data_editor;
		}
	}
	for (int i = tile_set->get_custom_data_layers_count(); tile_data_editors.has(vformat("custom_data_%d", i)); i++) {
		tile_data_editors[vformat("custom_data_%d", i)]->queue_delete();
		tile_data_editors.erase(vformat("custom_data_%d", i));
	}

	if (tile_set->get_custom_data_layers_count() == 0) {
		item = tile_data_editors_tree->create_item(group);
		item->set_icon(0, get_theme_icon("Info", "EditorIcons"));
		item->set_icon_modulate(0, disabled_color);
		item->set_text(0, TTR("No custom data layers"));
		item->set_tooltip(0, TTR("Create and customize custom data layers in the inspector of the LayeredTileSet resource."));
		item->set_selectable(0, false);
		item->set_custom_color(0, disabled_color);
	}

	//tile_data_editors_tree->update_minimum_size();

	// VertexLights2D
#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	ADD_TILE_DATA_EDITOR_GROUP(TTR("VertexLight2D"));

	ADD_TILE_DATA_EDITOR(group, TTR("Enabled"), "vertex_light_enabled");
	if (!tile_data_editors.has("vertex_light_enabled")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::BOOL, "vertex_light_enabled");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_enabled"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Position"), "vertex_light_position");
	if (!tile_data_editors.has("vertex_light_position")) {
		TileDataPositionEditor *tde = memnew(TileDataPositionEditor);
		tde->hide();
		tde->setup_property_editor(Variant::VECTOR2I, "vertex_light_position");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_position"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Range"), "vertex_light_range");
	if (!tile_data_editors.has("vertex_light_range")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::VECTOR2I, "vertex_light_range", "", Vector2i(32, 32));
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_range"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Attenuation"), "vertex_light_attenuation");
	if (!tile_data_editors.has("vertex_light_attenuation")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::REAL, "vertex_light_attenuation", "", 1, PROPERTY_HINT_EXP_EASING, "attenuation");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_attenuation"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Color"), "vertex_light_color");
	if (!tile_data_editors.has("vertex_light_color")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::COLOR, "vertex_light_color", "", Color(1, 1, 1, 1));
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_color"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Light Mode"), "set_vertex_light_mode");
	if (!tile_data_editors.has("set_vertex_light_mode")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::INT, "set_vertex_light_mode", "", 0, PROPERTY_HINT_ENUM, "Add,Sub,Mix");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["set_vertex_light_mode"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Z Range"), "vertex_light_z_range");
	if (!tile_data_editors.has("vertex_light_z_range")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::VECTOR2I, "vertex_light_z_range", "", Vector2i(-1024, 1024), PROPERTY_HINT_RANGE, itos(RS::CANVAS_ITEM_Z_MIN) + "," + itos(RS::CANVAS_ITEM_Z_MAX) + ",1");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_z_range"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Layer Range"), "vertex_light_layer_range");
	if (!tile_data_editors.has("vertex_light_layer_range")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::VECTOR2I, "vertex_light_layer_range", "", Vector2i(-512, 512), PROPERTY_HINT_RANGE, "-512,512,1");
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_layer_range"] = tde;
	}

	ADD_TILE_DATA_EDITOR(group, TTR("Item Cull Mask"), "vertex_light_item_cull_mask");
	if (!tile_data_editors.has("vertex_light_item_cull_mask")) {
		TileDataDefaultEditor *tde = memnew(TileDataDefaultEditor());
		tde->hide();
		tde->setup_property_editor(Variant::INT, "vertex_light_item_cull_mask", "", 1, PROPERTY_HINT_LAYERS_2D_RENDER);
		tde->connect("needs_redraw", tile_atlas_control_unscaled, "update");
		tde->connect("needs_redraw", alternative_tiles_control_unscaled, "update");
		tile_data_editors["vertex_light_item_cull_mask"] = tde;
	}
#endif

#undef ADD_TILE_DATA_EDITOR_GROUP
#undef ADD_TILE_DATA_EDITOR

	// Add tile data editors as children.
	for (HashMap<String, LayeredTileDataEditor *>::Element *E = tile_data_editors.front(); E; E = E->next) {
		// Tile Data Editor.
		LayeredTileDataEditor *tile_data_editor = E->value();
		if (!tile_data_editor->is_inside_tree()) {
			tile_data_painting_editor_container->add_child(tile_data_editor);
		}
		tile_data_editor->set_tile_set(tile_set);

		// Toolbar.
		Control *toolbar = tile_data_editor->get_toolbar();
		if (!toolbar->is_inside_tree()) {
			tool_settings_tile_data_toolbar_container->add_child(toolbar);
		}
		toolbar->hide();
	}

	// Update visibility.
	bool is_visible = tools_button_group->get_pressed_button() == tool_paint_button;
	tile_data_editor_dropdown_button->set_visible(is_visible);
	if (tile_data_editors_tree->get_selected()) {
		tile_data_editor_dropdown_button->set_text(tile_data_editors_tree->get_selected()->get_text(0));
	} else {
		tile_data_editor_dropdown_button->set_text(TTR("Select a property editor"));
	}
	tile_data_editors_scroll->set_visible(is_visible);
}

void LayeredTileSetAtlasSourceEditor::_update_current_tile_data_editor() {
	// Find the property to use.
	String property;
	if (tools_button_group->get_pressed_button() == tool_select_button && tile_inspector->is_visible() && !tile_inspector->get_selected_path().empty()) {
		Vector<String> components = tile_inspector->get_selected_path().split("/");
		if (components.size() >= 1) {
			property = components[0];

			// Workaround for terrains as they don't have a common first component.
			if (property.begins_with("terrains_")) {
				property = "terrain_set";
			}
		}
	} else if (tools_button_group->get_pressed_button() == tool_paint_button && tile_data_editors_tree->get_selected()) {
		property = tile_data_editors_tree->get_selected()->get_metadata(0);
		tile_data_editor_dropdown_button->set_text(tile_data_editors_tree->get_selected()->get_text(0));
	}

	// Hide all editors but the current one.

	for (const HashMap<String, LayeredTileDataEditor *>::Element *E = tile_data_editors.front(); E; E = E->next) {
		E->value()->hide();
		E->value()->get_toolbar()->hide();
	}
	if (tile_data_editors.has(property)) {
		current_tile_data_editor = tile_data_editors[property];
	} else {
		current_tile_data_editor = nullptr;
	}

	// Get the correct editor for the TileData's property.
	if (current_tile_data_editor) {
		current_tile_data_editor_toolbar = current_tile_data_editor->get_toolbar();
		current_property = property;
		current_tile_data_editor->set_visible(tools_button_group->get_pressed_button() == tool_paint_button);
		current_tile_data_editor_toolbar->set_visible(tools_button_group->get_pressed_button() == tool_paint_button);
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_data_editor_dropdown_button_draw() {
	if (!has_theme_icon("arrow", "OptionButton")) {
		return;
	}

	RID ci = tile_data_editor_dropdown_button->get_canvas_item();
	Ref<Texture> arrow = Control::get_theme_icon("arrow", "OptionButton");
	Color clr = Color(1, 1, 1);
	if (get_theme_constant("modulate_arrow")) {
		switch (tile_data_editor_dropdown_button->get_draw_mode()) {
			case BaseButton::DRAW_PRESSED:
				clr = get_theme_color("font_pressed_color");
				break;
			case BaseButton::DRAW_HOVER:
				clr = get_theme_color("font_hover_color");
				break;
			case BaseButton::DRAW_DISABLED:
				clr = get_theme_color("font_disabled_color");
				break;
			default:
				if (tile_data_editor_dropdown_button->has_focus()) {
					clr = get_theme_color("font_focus_color");
				} else {
					clr = get_theme_color("font_color");
				}
		}
	}

	Size2 size = tile_data_editor_dropdown_button->get_size();

	Point2 ofs;
	ofs = Point2(size.width - arrow->get_width() - get_theme_constant("arrow_margin", "OptionButton"), int(Math::abs((size.height - arrow->get_height()) / 2)));
	arrow->draw(ci, ofs, clr);
}

void LayeredTileSetAtlasSourceEditor::_tile_data_editor_dropdown_button_pressed() {
	Size2 size = tile_data_editor_dropdown_button->get_size();
	Size2 ts = tile_data_editors_tree->get_size();
	tile_data_editors_popup->set_position(tile_data_editor_dropdown_button->get_global_position() + Size2(0, size.height * get_global_transform().get_scale().y));
	tile_data_editors_popup->set_size(Size2(size.width, ts.height) * get_global_transform().get_scale());
	tile_data_editors_popup->popup();
}

void LayeredTileSetAtlasSourceEditor::_tile_data_editors_tree_selected() {
	tile_data_editors_popup->call_deferred("hide");
	_update_current_tile_data_editor();
	tile_atlas_control->update();
	tile_atlas_control_unscaled->update();
	alternative_tiles_control->update();
	alternative_tiles_control_unscaled->update();
}

void LayeredTileSetAtlasSourceEditor::_update_atlas_view() {
	// Update the atlas display.
	tile_atlas_view->set_atlas_source(*tile_set, tile_set_atlas_source, tile_set_atlas_source_id);

	// Create a bunch of buttons to add alternative tiles.
	for (int i = 0; i < alternative_tiles_control->get_child_count(); i++) {
		alternative_tiles_control->get_child(i)->queue_delete();
	}

	if (tile_set.is_null()) {
		return;
	} else {
		tile_create_help->set_visible(tools_button_group->get_pressed_button() == tool_setup_atlas_source_button);
	}

	Vector2i pos;
	Vector2 texture_region_base_size = tile_set_atlas_source->get_texture_region_size();
	int texture_region_base_size_min = MIN(texture_region_base_size.x, texture_region_base_size.y);
	for (int i = 0; i < tile_set_atlas_source->get_tiles_count(); i++) {
		Vector2i tile_id = tile_set_atlas_source->get_tile_id(i);
		int alternative_count = tile_set_atlas_source->get_alternative_tiles_count(tile_id);
		if (alternative_count > 1) {
			// Compute the right extremity of alternative.
			int y_increment = 0;
			pos.x = 0;
			for (int j = 1; j < alternative_count; j++) {
				int alternative_id = tile_set_atlas_source->get_alternative_tile_id(tile_id, j);
				Rect2i rect = tile_atlas_view->get_alternative_tile_rect(tile_id, alternative_id);
				pos.x = MAX(pos.x, rect.get_end().x);
				y_increment = MAX(y_increment, rect.size.y);
			}

			// Create and position the button.
			Button *button = memnew(Button);
			button->set_flat(true);
			button->set_expand_icon(true);
			button->set_icon(get_theme_icon("Add", "EditorIcons"));
			button->add_theme_style_override("normal", memnew(StyleBoxEmpty));
			button->add_theme_style_override("hover", memnew(StyleBoxEmpty));
			button->add_theme_style_override("focus", memnew(StyleBoxEmpty));
			button->add_theme_style_override("pressed", memnew(StyleBoxEmpty));
			button->add_theme_style_override("disabled", memnew(StyleBoxEmpty));
			button->add_theme_font_override("font", memnew(BitmapFont));
			button->connect("pressed", tile_set_atlas_source, "create_alternative_tile", varray(tile_id, LayeredTileSetSource::INVALID_TILE_ALTERNATIVE));
			button->set_position(Vector2(pos.x, pos.y + (y_increment - texture_region_base_size.y) / 2.0));
			button->set_size(Vector2(texture_region_base_size_min, texture_region_base_size_min));
			alternative_tiles_control->add_child(button);

			pos.y += y_increment;
		}
	}
	tile_atlas_view->set_padding(Side::SIDE_RIGHT, texture_region_base_size_min);

	// Redraw everything.
	tile_atlas_control->update();
	tile_atlas_control_unscaled->update();
	alternative_tiles_control->update();
	alternative_tiles_control_unscaled->update();
	tile_atlas_view->update();

	// Synchronize atlas view.
	LayeredTilesEditorUtils::get_singleton()->synchronize_atlas_view(tile_atlas_view);
}

void LayeredTileSetAtlasSourceEditor::_update_toolbar() {
	// Show the tools and settings.
	if (tools_button_group->get_pressed_button() == tool_setup_atlas_source_button) {
		if (current_tile_data_editor_toolbar) {
			current_tile_data_editor_toolbar->hide();
		}
		tools_settings_erase_button->show();
		tool_advanced_menu_button->show();
	} else if (tools_button_group->get_pressed_button() == tool_select_button) {
		if (current_tile_data_editor_toolbar) {
			current_tile_data_editor_toolbar->hide();
		}
		tools_settings_erase_button->hide();
		tool_advanced_menu_button->hide();
	} else if (tools_button_group->get_pressed_button() == tool_paint_button) {
		if (current_tile_data_editor_toolbar) {
			current_tile_data_editor_toolbar->show();
		}
		tools_settings_erase_button->hide();
		tool_advanced_menu_button->hide();
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_atlas_control_mouse_exited() {
	hovered_base_tile_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS;
	tile_atlas_control->update();
	tile_atlas_control_unscaled->update();
	tile_atlas_view->update();
}

void LayeredTileSetAtlasSourceEditor::_tile_atlas_view_transform_changed(real_t p_zoom, Vector2 p_scroll) {
	tile_atlas_control->update();
	tile_atlas_control_unscaled->update();
}

void LayeredTileSetAtlasSourceEditor::_tile_atlas_control_gui_input(const Ref<InputEvent> &p_event) {
	// Update the hovered coords.
	hovered_base_tile_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position());

	// Forward the event to the current tile data editor if we are in the painting mode.
	if (tools_button_group->get_pressed_button() == tool_paint_button) {
		if (current_tile_data_editor) {
			current_tile_data_editor->forward_painting_atlas_gui_input(tile_atlas_view, tile_set_atlas_source, p_event);
		}
		// Update only what's needed.
		tile_set_changed_needs_update = false;

		tile_atlas_control->update();
		tile_atlas_control_unscaled->update();
		alternative_tiles_control->update();
		alternative_tiles_control_unscaled->update();
		tile_atlas_view->update();
		return;
	} else {
		// Handle the event.
		Ref<InputEventMouseMotion> mm = p_event;
		if (mm.is_valid()) {
			Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
			Vector2i last_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_last_mouse_pos, true);
			Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position());

			Vector2i grid_size = tile_set_atlas_source->get_atlas_grid_size();

			if (drag_type == DRAG_TYPE_CREATE_BIG_TILE) {
				// Create big tile.
				new_base_tiles_coords = new_base_tiles_coords.max(Vector2i(0, 0)).min(grid_size - Vector2i(1, 1));

				Rect2i new_rect = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
				new_rect.size += Vector2i(1, 1);
				// Check if the new tile can fit in the new rect.
				if (tile_set_atlas_source->has_room_for_tile(new_rect.position, new_rect.size, tile_set_atlas_source->get_tile_animation_columns(drag_current_tile), tile_set_atlas_source->get_tile_animation_separation(drag_current_tile), tile_set_atlas_source->get_tile_animation_frames_count(drag_current_tile), drag_current_tile)) {
					// Move and resize the tile.
					tile_set_atlas_source->move_tile_in_atlas(drag_current_tile, new_rect.position, new_rect.size);
					drag_current_tile = new_rect.position;
				}
			} else if (drag_type == DRAG_TYPE_CREATE_TILES) {
				// Create tiles.
				last_base_tiles_coords = last_base_tiles_coords.max(Vector2i(0, 0)).min(grid_size - Vector2i(1, 1));
				new_base_tiles_coords = new_base_tiles_coords.max(Vector2i(0, 0)).min(grid_size - Vector2i(1, 1));

				Vector<Point2i> line = Geometry::bresenham_line(last_base_tiles_coords.x, new_base_tiles_coords.x, last_base_tiles_coords.y, new_base_tiles_coords.y);
				for (int i = 0; i < line.size(); i++) {
					if (tile_set_atlas_source->get_tile_at_coords(line[i]) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						tile_set_atlas_source->create_tile(line[i]);
						drag_modified_tiles.insert(line[i]);
					}
				}

				drag_last_mouse_pos = tile_atlas_control->get_local_mouse_position();

			} else if (drag_type == DRAG_TYPE_REMOVE_TILES) {
				// Remove tiles.
				last_base_tiles_coords = last_base_tiles_coords.max(Vector2i(0, 0)).min(grid_size - Vector2i(1, 1));
				new_base_tiles_coords = new_base_tiles_coords.max(Vector2i(0, 0)).min(grid_size - Vector2i(1, 1));

				Vector<Point2i> line = Geometry::bresenham_line(last_base_tiles_coords.x, new_base_tiles_coords.x, last_base_tiles_coords.y, new_base_tiles_coords.y);
				for (int i = 0; i < line.size(); i++) {
					Vector2i base_tile_coords = tile_set_atlas_source->get_tile_at_coords(line[i]);
					if (base_tile_coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						drag_modified_tiles.insert(base_tile_coords);
					}
				}

				drag_last_mouse_pos = tile_atlas_control->get_local_mouse_position();
			} else if (drag_type == DRAG_TYPE_MOVE_TILE) {
				// Move tile.
				Vector2 mouse_offset = (Vector2(tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile)) / 2.0 - Vector2(0.5, 0.5)) * tile_set->get_tile_size();
				Vector2i coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position() - mouse_offset);
				if (drag_current_tile != coords && tile_set_atlas_source->has_room_for_tile(coords, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile), tile_set_atlas_source->get_tile_animation_columns(drag_current_tile), tile_set_atlas_source->get_tile_animation_separation(drag_current_tile), tile_set_atlas_source->get_tile_animation_frames_count(drag_current_tile), drag_current_tile)) {
					tile_set_atlas_source->move_tile_in_atlas(drag_current_tile, coords);
					selection.clear();
					selection.insert({ coords, 0 });
					drag_current_tile = coords;

					// Update only what's needed.
					tile_set_changed_needs_update = false;
					_update_tile_inspector();
					_update_atlas_view();
					_update_tile_id_label();
					_update_current_tile_data_editor();
				}
			} else if (drag_type == DRAG_TYPE_MAY_POPUP_MENU) {
				if (Vector2(drag_start_mouse_pos).distance_to(tile_atlas_control->get_local_mouse_position()) > 5.0 * EDSCALE) {
					drag_type = DRAG_TYPE_NONE;
				}
			} else if (drag_type >= DRAG_TYPE_RESIZE_TOP_LEFT && drag_type <= DRAG_TYPE_RESIZE_LEFT) {
				// Resizing a tile.
				Rect2i old_rect = Rect2i(drag_current_tile, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile));
				Rect2i new_rect = old_rect;

				if (drag_type == DRAG_TYPE_RESIZE_LEFT || drag_type == DRAG_TYPE_RESIZE_TOP_LEFT || drag_type == DRAG_TYPE_RESIZE_BOTTOM_LEFT) {
					new_base_tiles_coords = _get_drag_offset_tile_coords(Vector2i(-1, 0));
					new_rect.position.x = MIN(new_base_tiles_coords.x + 1, old_rect.get_end().x - 1);
					new_rect.size.x = old_rect.get_end().x - new_rect.position.x;
				}
				if (drag_type == DRAG_TYPE_RESIZE_TOP || drag_type == DRAG_TYPE_RESIZE_TOP_LEFT || drag_type == DRAG_TYPE_RESIZE_TOP_RIGHT) {
					new_base_tiles_coords = _get_drag_offset_tile_coords(Vector2i(0, -1));
					new_rect.position.y = MIN(new_base_tiles_coords.y + 1, old_rect.get_end().y - 1);
					new_rect.size.y = old_rect.get_end().y - new_rect.position.y;
				}

				if (drag_type == DRAG_TYPE_RESIZE_RIGHT || drag_type == DRAG_TYPE_RESIZE_TOP_RIGHT || drag_type == DRAG_TYPE_RESIZE_BOTTOM_RIGHT) {
					new_base_tiles_coords = _get_drag_offset_tile_coords(Vector2i(1, 0));
					new_rect.set_end(Vector2i(MAX(new_base_tiles_coords.x, old_rect.position.x + 1), new_rect.get_end().y));
				}
				if (drag_type == DRAG_TYPE_RESIZE_BOTTOM || drag_type == DRAG_TYPE_RESIZE_BOTTOM_LEFT || drag_type == DRAG_TYPE_RESIZE_BOTTOM_RIGHT) {
					new_base_tiles_coords = _get_drag_offset_tile_coords(Vector2i(0, 1));
					new_rect.set_end(Vector2i(new_rect.get_end().x, MAX(new_base_tiles_coords.y, old_rect.position.y + 1)));
				}

				if (tile_set_atlas_source->has_room_for_tile(new_rect.position, new_rect.size, tile_set_atlas_source->get_tile_animation_columns(drag_current_tile), tile_set_atlas_source->get_tile_animation_separation(drag_current_tile), tile_set_atlas_source->get_tile_animation_frames_count(drag_current_tile), drag_current_tile)) {
					tile_set_atlas_source->move_tile_in_atlas(drag_current_tile, new_rect.position, new_rect.size);
					selection.clear();
					selection.insert({ new_rect.position, 0 });
					drag_current_tile = new_rect.position;

					// Update only what's needed.
					tile_set_changed_needs_update = false;
					_update_tile_inspector();
					_update_atlas_view();
					_update_tile_id_label();
					_update_current_tile_data_editor();
				}
			}

			// Redraw for the hovered tile.
			tile_atlas_control->update();
			tile_atlas_control_unscaled->update();
			alternative_tiles_control->update();
			alternative_tiles_control_unscaled->update();
			tile_atlas_view->update();
			return;
		}

		Ref<InputEventMouseButton> mb = p_event;
		if (mb.is_valid()) {
			Vector2 mouse_local_pos = tile_atlas_control->get_local_mouse_position();
			if (mb->get_button_index() == BUTTON_LEFT) {
				if (mb->is_pressed()) {
					// Left click pressed.
					if (tools_button_group->get_pressed_button() == tool_setup_atlas_source_button) {
						if (tools_settings_erase_button->is_pressed()) {
							// Erasing
							if (mb->get_control() || mb->get_shift()) {
								// Remove tiles using rect.

								// Setup the dragging info.
								drag_type = DRAG_TYPE_REMOVE_TILES_USING_RECT;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;
							} else {
								// Remove tiles.

								// Setup the dragging info.
								drag_type = DRAG_TYPE_REMOVE_TILES;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;

								// Remove a first tile.
								Vector2i coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos);
								if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
									coords = tile_set_atlas_source->get_tile_at_coords(coords);
								}
								if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
									drag_modified_tiles.insert(coords);
								}
							}
						} else {
							// Creating
							if (mb->get_shift()) {
								// Create a big tile.
								Vector2i coords = tile_atlas_view->get_atlas_tile_coords_at_pos(mouse_local_pos);
								if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS && tile_set_atlas_source->get_tile_at_coords(coords) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
									// Setup the dragging info, only if we start on an empty tile.
									drag_type = DRAG_TYPE_CREATE_BIG_TILE;
									drag_start_mouse_pos = mouse_local_pos;
									drag_last_mouse_pos = drag_start_mouse_pos;
									drag_current_tile = coords;

									// Create a tile.
									tile_set_atlas_source->create_tile(coords);
								}
							} else if (mb->get_control()) {
								// Create tiles using rect.
								drag_type = DRAG_TYPE_CREATE_TILES_USING_RECT;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;
							} else {
								// Create tiles.

								// Setup the dragging info.
								drag_type = DRAG_TYPE_CREATE_TILES;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;

								// Create a first tile if needed.
								Vector2i coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos);
								if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS && tile_set_atlas_source->get_tile_at_coords(coords) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
									tile_set_atlas_source->create_tile(coords);
									drag_modified_tiles.insert(coords);
								}
							}
						}
					} else if (tools_button_group->get_pressed_button() == tool_select_button) {
						// Dragging a handle.
						drag_type = DRAG_TYPE_NONE;
						if (selection.size() == 1) {
							TileSelection selected = selection.front()->get();
							if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS && selected.alternative == 0) {
								Vector2i size_in_atlas = tile_set_atlas_source->get_tile_size_in_atlas(selected.tile);
								Rect2 region = tile_set_atlas_source->get_tile_texture_region(selected.tile);
								Size2 zoomed_size = resize_handle->get_size() / tile_atlas_view->get_zoom();
								Rect2 rect = region.grow_individual(zoomed_size.x, zoomed_size.y, 0, 0);
								const Vector2i coords[] = { Vector2i(0, 0), Vector2i(1, 0), Vector2i(1, 1), Vector2i(0, 1) };
								const Vector2i directions[] = { Vector2i(0, -1), Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0) };
								bool can_grow[4];
								for (int i = 0; i < 4; i++) {
									can_grow[i] = tile_set_atlas_source->has_room_for_tile(selected.tile + directions[i], tile_set_atlas_source->get_tile_size_in_atlas(selected.tile), tile_set_atlas_source->get_tile_animation_columns(selected.tile), tile_set_atlas_source->get_tile_animation_separation(selected.tile), tile_set_atlas_source->get_tile_animation_frames_count(selected.tile), selected.tile);
									can_grow[i] |= (i % 2 == 0) ? size_in_atlas.y > 1 : size_in_atlas.x > 1;
								}
								for (int i = 0; i < 4; i++) {
									Vector2 pos = rect.position + rect.size * coords[i];
									if (can_grow[i] && can_grow[(i + 3) % 4] && Rect2(pos, zoomed_size).has_point(mouse_local_pos)) {
										drag_type = (DragType)((int)DRAG_TYPE_RESIZE_TOP_LEFT + i * 2);
										drag_start_mouse_pos = mouse_local_pos;
										drag_last_mouse_pos = drag_start_mouse_pos;
										drag_current_tile = selected.tile;
										drag_start_tile_shape = Rect2i(selected.tile, tile_set_atlas_source->get_tile_size_in_atlas(selected.tile));
									}
									Vector2 next_pos = rect.position + rect.size * coords[(i + 1) % 4];
									if (can_grow[i] && Rect2((pos + next_pos) / 2.0, zoomed_size).has_point(mouse_local_pos)) {
										drag_type = (DragType)((int)DRAG_TYPE_RESIZE_TOP + i * 2);
										drag_start_mouse_pos = mouse_local_pos;
										drag_last_mouse_pos = drag_start_mouse_pos;
										drag_current_tile = selected.tile;
										drag_start_tile_shape = Rect2i(selected.tile, tile_set_atlas_source->get_tile_size_in_atlas(selected.tile));
									}
								}
							}
						}

						// Selecting then dragging a tile.
						if (drag_type == DRAG_TYPE_NONE) {
							TileSelection selected = { LayeredTileSetSource::INVALID_ATLAS_COORDS, LayeredTileSetSource::INVALID_TILE_ALTERNATIVE };
							Vector2i coords = tile_atlas_view->get_atlas_tile_coords_at_pos(mouse_local_pos);
							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								coords = tile_set_atlas_source->get_tile_at_coords(coords);
								if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
									selected = { coords, 0 };
								}
							}

							bool shift = mb->get_shift();
							if (!shift && selection.size() == 1 && selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS && selection.has(selected)) {
								// Start move dragging.
								drag_type = DRAG_TYPE_MOVE_TILE;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;
								drag_current_tile = selected.tile;
								drag_start_tile_shape = Rect2i(selected.tile, tile_set_atlas_source->get_tile_size_in_atlas(selected.tile));
							} else {
								// Start selection dragging.
								drag_type = DRAG_TYPE_RECT_SELECT;
								drag_start_mouse_pos = mouse_local_pos;
								drag_last_mouse_pos = drag_start_mouse_pos;
							}
						}
					}
				} else {
					// Left click released.
					_end_dragging();
				}
				tile_atlas_control->update();
				tile_atlas_control_unscaled->update();
				alternative_tiles_control->update();
				alternative_tiles_control_unscaled->update();
				tile_atlas_view->update();
				return;
			} else if (mb->get_button_index() == BUTTON_RIGHT) {
				// Right click pressed.
				if (mb->is_pressed()) {
					drag_type = DRAG_TYPE_MAY_POPUP_MENU;
					drag_start_mouse_pos = tile_atlas_control->get_local_mouse_position();
				} else {
					// Right click released.
					_end_dragging();
				}
				tile_atlas_control->update();
				tile_atlas_control_unscaled->update();
				alternative_tiles_control->update();
				alternative_tiles_control_unscaled->update();
				tile_atlas_view->update();
				return;
			}
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_end_dragging() {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	switch (drag_type) {
		case DRAG_TYPE_CREATE_TILES:
			undo_redo->create_action(TTR("Create tiles"));

			for (const RBSet<Vector2i>::Element *E = drag_modified_tiles.front(); E; E = E->next()) {
				undo_redo->add_do_method(tile_set_atlas_source, "create_tile", E->get());
				undo_redo->add_undo_method(tile_set_atlas_source, "remove_tile", E->get());
			}
			undo_redo->commit_action();
			break;
		case DRAG_TYPE_CREATE_BIG_TILE:
			undo_redo->create_action(TTR("Create a tile"));
			undo_redo->add_do_method(tile_set_atlas_source, "create_tile", drag_current_tile, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile));
			undo_redo->add_undo_method(tile_set_atlas_source, "remove_tile", drag_current_tile);
			undo_redo->commit_action();
			break;
		case DRAG_TYPE_REMOVE_TILES: {
			List<PropertyInfo> list;
			tile_set_atlas_source->get_property_list(&list);
			HashMap<Vector2i, List<const PropertyInfo *>> per_tile = _group_properties_per_tiles(list, tile_set_atlas_source);
			undo_redo->create_action(TTR("Remove tiles"));
			for (const RBSet<Vector2i>::Element *E = drag_modified_tiles.front(); E; E = E->next()) {
				Vector2i coords = E->get();
				undo_redo->add_do_method(tile_set_atlas_source, "remove_tile", coords);
				undo_redo->add_undo_method(tile_set_atlas_source, "create_tile", coords);
				if (per_tile.has(coords)) {
					for (List<const PropertyInfo *>::Element *E_property = per_tile[coords].front(); E_property; E_property = E_property->next()) {
						String property = E_property->get()->name;
						Variant value = tile_set_atlas_source->get(property);
						if (value.get_type() != Variant::NIL) {
							undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
						}
					}
				}
			}
			undo_redo->commit_action();
		} break;
		case DRAG_TYPE_CREATE_TILES_USING_RECT: {
			Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
			Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
			Rect2i area = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
			area.set_end((area.get_end() + Vector2i(1, 1)).min(tile_set_atlas_source->get_atlas_grid_size()));
			undo_redo->create_action(TTR("Create tiles"));
			for (int x = area.get_position().x; x < area.get_end().x; x++) {
				for (int y = area.get_position().y; y < area.get_end().y; y++) {
					Vector2i coords = Vector2i(x, y);
					if (tile_set_atlas_source->get_tile_at_coords(coords) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						undo_redo->add_do_method(tile_set_atlas_source, "create_tile", coords);
						undo_redo->add_undo_method(tile_set_atlas_source, "remove_tile", coords);
					}
				}
			}
			undo_redo->commit_action();
		} break;
		case DRAG_TYPE_REMOVE_TILES_USING_RECT: {
			Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
			Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
			Rect2i area = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
			area.set_end((area.get_end() + Vector2i(1, 1)).min(tile_set_atlas_source->get_atlas_grid_size()));
			List<PropertyInfo> list;
			tile_set_atlas_source->get_property_list(&list);
			HashMap<Vector2i, List<const PropertyInfo *>> per_tile = _group_properties_per_tiles(list, tile_set_atlas_source);

			RBSet<Vector2i> to_delete;
			for (int x = area.get_position().x; x < area.get_end().x; x++) {
				for (int y = area.get_position().y; y < area.get_end().y; y++) {
					Vector2i coords = tile_set_atlas_source->get_tile_at_coords(Vector2i(x, y));
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						to_delete.insert(coords);
					}
				}
			}

			undo_redo->create_action(TTR("Remove tiles"));
			undo_redo->add_do_method(this, "_set_selection_from_array", Array());

			for (const RBSet<Vector2i>::Element *E = to_delete.front(); E; E = E->next()) {
				Vector2i coords = E->get();
				undo_redo->add_do_method(tile_set_atlas_source, "remove_tile", coords);
				undo_redo->add_undo_method(tile_set_atlas_source, "create_tile", coords);
				if (per_tile.has(coords)) {
					for (List<const PropertyInfo *>::Element *E_property = per_tile[coords].front(); E_property; E_property = E_property->next()) {
						String property = E_property->get()->name;
						Variant value = tile_set_atlas_source->get(property);
						if (value.get_type() != Variant::NIL) {
							undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
						}
					}
				}
			}
			undo_redo->add_undo_method(this, "_set_selection_from_array", _get_selection_as_array());
			undo_redo->commit_action();
		} break;
		case DRAG_TYPE_MOVE_TILE:
			if (drag_current_tile != drag_start_tile_shape.position) {
				undo_redo->create_action(TTR("Move a tile"));
				undo_redo->add_do_method(tile_set_atlas_source, "move_tile_in_atlas", drag_start_tile_shape.position, drag_current_tile, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile));
				undo_redo->add_do_method(this, "_set_selection_from_array", _get_selection_as_array());
				undo_redo->add_undo_method(tile_set_atlas_source, "move_tile_in_atlas", drag_current_tile, drag_start_tile_shape.position, drag_start_tile_shape.size);
				Array array;
				array.push_back(drag_start_tile_shape.position);
				array.push_back(0);
				undo_redo->add_undo_method(this, "_set_selection_from_array", array);
				undo_redo->commit_action();
			}
			break;
		case DRAG_TYPE_RECT_SELECT: {
			Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
			Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
			ERR_FAIL_COND(start_base_tiles_coords == LayeredTileSetSource::INVALID_ATLAS_COORDS);
			ERR_FAIL_COND(new_base_tiles_coords == LayeredTileSetSource::INVALID_ATLAS_COORDS);

			Rect2i region = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
			region.size += Vector2i(1, 1);

			undo_redo->create_action(TTR("Select tiles"));
			undo_redo->add_undo_method(this, "_set_selection_from_array", _get_selection_as_array());

			// Determine if we clear, then add or remove to the selection.
			bool add_to_selection = true;
			if (Input::get_singleton()->is_key_pressed(KEY_SHIFT)) {
				Vector2i coords = tile_set_atlas_source->get_tile_at_coords(start_base_tiles_coords);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					if (selection.has({ coords, 0 })) {
						add_to_selection = false;
					}
				}
			} else {
				selection.clear();
			}

			// Modify the selection.
			for (int x = region.position.x; x < region.get_end().x; x++) {
				for (int y = region.position.y; y < region.get_end().y; y++) {
					Vector2i coords = Vector2i(x, y);
					coords = tile_set_atlas_source->get_tile_at_coords(coords);
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						if (add_to_selection && !selection.has({ coords, 0 })) {
							selection.insert({ coords, 0 });
						} else if (!add_to_selection && selection.has({ coords, 0 })) {
							selection.erase({ coords, 0 });
						}
					}
				}
			}
			_update_tile_inspector();
			_update_tile_id_label();
			_update_current_tile_data_editor();
			undo_redo->add_do_method(this, "_set_selection_from_array", _get_selection_as_array());
			undo_redo->commit_action();
		} break;
		case DRAG_TYPE_MAY_POPUP_MENU: {
			Vector2 mouse_local_pos = tile_atlas_control->get_local_mouse_position();
			TileSelection selected = { tile_atlas_view->get_atlas_tile_coords_at_pos(mouse_local_pos), 0 };
			if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				selected.tile = tile_set_atlas_source->get_tile_at_coords(selected.tile);
			}

			// Set the selection if needed.
			if (selection.size() <= 1) {
				if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					undo_redo->create_action(TTR("Select tiles"));
					undo_redo->add_undo_method(this, "_set_selection_from_array", _get_selection_as_array());
					selection.clear();
					selection.insert(selected);
					undo_redo->add_do_method(this, "_set_selection_from_array", _get_selection_as_array());
					undo_redo->commit_action();
					_update_tile_inspector();
					_update_tile_id_label();
					_update_current_tile_data_editor();
				}
			}

			// Pops up the correct menu, depending on whether we have a tile or not.
			if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS && selection.has(selected)) {
				// We have a tile.
				menu_option_coords = selected.tile;
				menu_option_alternative = 0;
				base_tile_popup_menu->set_position(get_global_transform().xform(get_local_mouse_position()));
				base_tile_popup_menu->popup();
			} else if (hovered_base_tile_coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				// We don't have a tile, but can create one.
				menu_option_coords = hovered_base_tile_coords;
				menu_option_alternative = LayeredTileSetSource::INVALID_TILE_ALTERNATIVE;
				empty_base_tile_popup_menu->set_position(get_global_transform().xform(get_local_mouse_position()));
				empty_base_tile_popup_menu->popup();
			}
		} break;
		case DRAG_TYPE_RESIZE_TOP_LEFT:
		case DRAG_TYPE_RESIZE_TOP:
		case DRAG_TYPE_RESIZE_TOP_RIGHT:
		case DRAG_TYPE_RESIZE_RIGHT:
		case DRAG_TYPE_RESIZE_BOTTOM_RIGHT:
		case DRAG_TYPE_RESIZE_BOTTOM:
		case DRAG_TYPE_RESIZE_BOTTOM_LEFT:
		case DRAG_TYPE_RESIZE_LEFT:
			if (drag_start_tile_shape != Rect2i(drag_current_tile, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile))) {
				undo_redo->create_action(TTR("Resize a tile"));
				undo_redo->add_do_method(tile_set_atlas_source, "move_tile_in_atlas", drag_start_tile_shape.position, drag_current_tile, tile_set_atlas_source->get_tile_size_in_atlas(drag_current_tile));
				undo_redo->add_do_method(this, "_set_selection_from_array", _get_selection_as_array());
				undo_redo->add_undo_method(tile_set_atlas_source, "move_tile_in_atlas", drag_current_tile, drag_start_tile_shape.position, drag_start_tile_shape.size);
				Array array;
				array.push_back(drag_start_tile_shape.position);
				array.push_back(0);
				undo_redo->add_undo_method(this, "_set_selection_from_array", array);
				undo_redo->commit_action();
			}
			break;
		default:
			break;
	}

	drag_modified_tiles.clear();
	drag_type = DRAG_TYPE_NONE;
	// Change mouse accordingly.
}

HashMap<Vector2i, List<const PropertyInfo *>> LayeredTileSetAtlasSourceEditor::_group_properties_per_tiles(const List<PropertyInfo> &r_list, const LayeredTileSetAtlasSource *p_atlas) {
	// Group properties per tile.
	HashMap<Vector2i, List<const PropertyInfo *>> per_tile;
	for (const List<PropertyInfo>::Element *E_property = r_list.front(); E_property; E_property = E_property->next()) {
		Vector<String> components = String(E_property->get().name).split("/", true, 1);
		if (components.size() >= 1) {
			Vector<String> coord_arr = components[0].split(":");
			if (coord_arr.size() == 2 && coord_arr[0].is_valid_integer() && coord_arr[1].is_valid_integer()) {
				Vector2i coords = Vector2i(coord_arr[0].to_int(), coord_arr[1].to_int());
				per_tile[coords].push_back(&(E_property->get()));
			}
		}
	}
	return per_tile;
}

void LayeredTileSetAtlasSourceEditor::_menu_option(int p_option) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	switch (p_option) {
		case TILE_DELETE: {
			List<PropertyInfo> list;
			tile_set_atlas_source->get_property_list(&list);
			HashMap<Vector2i, List<const PropertyInfo *>> per_tile = _group_properties_per_tiles(list, tile_set_atlas_source);
			undo_redo->create_action(TTR("Remove tile"));

			// Remove tiles
			RBSet<Vector2i> removed;

			for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
				TileSelection selected = E->get();

				if (selected.alternative == 0) {
					// Remove a tile.
					undo_redo->add_do_method(tile_set_atlas_source, "remove_tile", selected.tile);
					undo_redo->add_undo_method(tile_set_atlas_source, "create_tile", selected.tile);
					removed.insert(selected.tile);
					if (per_tile.has(selected.tile)) {
						for (List<const PropertyInfo *>::Element *E_property = per_tile[selected.tile].front(); E_property; E_property = E_property->next()) {
							String property = E_property->get()->name;
							Variant value = tile_set_atlas_source->get(property);
							if (value.get_type() != Variant::NIL) {
								undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
							}
						}
					}
				}
			}

			// Remove alternatives
			for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
				TileSelection selected = E->get();

				if (selected.alternative > 0 && !removed.has(selected.tile)) {
					// Remove an alternative tile.
					undo_redo->add_do_method(tile_set_atlas_source, "remove_alternative_tile", selected.tile, selected.alternative);
					undo_redo->add_undo_method(tile_set_atlas_source, "create_alternative_tile", selected.tile, selected.alternative);
					if (per_tile.has(selected.tile)) {
						for (List<const PropertyInfo *>::Element *E_property = per_tile[selected.tile].front(); E_property; E_property = E_property->next()) {
							Vector<String> components = E_property->get()->name.split("/", true, 2);
							if (components.size() >= 2 && components[1].is_valid_integer() && components[1].to_int() == selected.alternative) {
								String property = E_property->get()->name;
								Variant value = tile_set_atlas_source->get(property);
								if (value.get_type() != Variant::NIL) {
									undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
								}
							}
						}
					}
				}
			}
			undo_redo->commit_action();
			_update_fix_selected_and_hovered_tiles();
			_update_tile_id_label();
		} break;
		case TILE_CREATE: {
			undo_redo->create_action(TTR("Create a tile"));
			undo_redo->add_do_method(tile_set_atlas_source, "create_tile", menu_option_coords);
			Array array;
			array.push_back(menu_option_coords);
			array.push_back(0);
			undo_redo->add_do_method(this, "_set_selection_from_array", array);
			undo_redo->add_undo_method(tile_set_atlas_source, "remove_tile", menu_option_coords);
			undo_redo->add_undo_method(this, "_set_selection_from_array", _get_selection_as_array());
			undo_redo->commit_action();
			_update_tile_id_label();
		} break;
		case TILE_CREATE_ALTERNATIVE: {
			undo_redo->create_action(TTR("Create tile alternatives"));
			Array array;
			for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
				if (E->get().alternative == 0) {
					int next_id = tile_set_atlas_source->get_next_alternative_tile_id(E->get().tile);
					undo_redo->add_do_method(tile_set_atlas_source, "create_alternative_tile", E->get().tile, next_id);
					array.push_back(E->get().tile);
					array.push_back(next_id);
					undo_redo->add_undo_method(tile_set_atlas_source, "remove_alternative_tile", E->get().tile, next_id);
				}
			}
			undo_redo->add_do_method(this, "_set_selection_from_array", array);
			undo_redo->add_undo_method(this, "_set_selection_from_array", _get_selection_as_array());
			undo_redo->commit_action();
			_update_tile_id_label();
		} break;
		case ADVANCED_AUTO_CREATE_TILES: {
			atlases_to_auto_create_tiles.clear();
			atlases_to_auto_create_tiles.push_back(tile_set_atlas_source);
			_auto_create_tiles();
		} break;
		case ADVANCED_AUTO_REMOVE_TILES: {
			_auto_remove_tiles();
		} break;
		case ADVANCED_CLEANUP_TILES: {
			_cleanup_outside_tiles();
		} break;
	}
}

void LayeredTileSetAtlasSourceEditor::shortcut_input(const Ref<InputEvent> &p_event) {
	// Check for shortcuts.
	if (ED_IS_SHORTCUT("layered_tiles_editor/delete_tile", p_event)) {
		if (tools_button_group->get_pressed_button() == tool_select_button && !selection.empty()) {
			_menu_option(TILE_DELETE);
			accept_event();
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_set_selection_from_array(const Array &p_selection) {
	ERR_FAIL_COND((p_selection.size() % 2) != 0);
	selection.clear();
	for (int i = 0; i < p_selection.size() / 2; i++) {
		TileSelection selected = { p_selection[i * 2], p_selection[i * 2 + 1] };
		if (tile_set_atlas_source->has_tile(selected.tile) && tile_set_atlas_source->has_alternative_tile(selected.tile, selected.alternative)) {
			selection.insert(selected);
		}
	}
	_update_tile_inspector();
	_update_tile_id_label();
	_update_atlas_view();
	_update_current_tile_data_editor();
}

Array LayeredTileSetAtlasSourceEditor::_get_selection_as_array() {
	Array output;
	for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
		output.push_back(E->get().tile);
		output.push_back(E->get().alternative);
	}
	return output;
}

void LayeredTileSetAtlasSourceEditor::_tile_atlas_control_draw() {
	// Draw the selected tile.
	if (tools_button_group->get_pressed_button() == tool_select_button) {
		for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
			TileSelection selected = E->get();
			if (selected.alternative == 0) {
				// Draw the rect.
				for (int frame = 0; frame < tile_set_atlas_source->get_tile_animation_frames_count(selected.tile); frame++) {
					Color color = Color(0.0, 1.0, 0.0, frame == 0 ? 1.0 : 0.3);
					Rect2 region = tile_set_atlas_source->get_tile_texture_region(selected.tile, frame);
					LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, region, color);
				}
			}
		}

		if (selection.size() == 1) {
			// Draw the resize handles (only when it's possible to expand).
			TileSelection selected = selection.front()->get();
			if (selected.alternative == 0) {
				Vector2i size_in_atlas = tile_set_atlas_source->get_tile_size_in_atlas(selected.tile);
				Size2 zoomed_size = resize_handle->get_size() / tile_atlas_view->get_zoom();
				Rect2 region = tile_set_atlas_source->get_tile_texture_region(selected.tile);
				Rect2 rect = region.grow_individual(zoomed_size.x, zoomed_size.y, 0, 0);
				const Vector2i coords[] = { Vector2i(0, 0), Vector2i(1, 0), Vector2i(1, 1), Vector2i(0, 1) };
				const Vector2i directions[] = { Vector2i(0, -1), Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0) };
				bool can_grow[4];
				for (int i = 0; i < 4; i++) {
					can_grow[i] = tile_set_atlas_source->has_room_for_tile(selected.tile + directions[i], tile_set_atlas_source->get_tile_size_in_atlas(selected.tile), tile_set_atlas_source->get_tile_animation_columns(selected.tile), tile_set_atlas_source->get_tile_animation_separation(selected.tile), tile_set_atlas_source->get_tile_animation_frames_count(selected.tile), selected.tile);
					can_grow[i] |= (i % 2 == 0) ? size_in_atlas.y > 1 : size_in_atlas.x > 1;
				}
				for (int i = 0; i < 4; i++) {
					Vector2 pos = rect.position + rect.size * coords[i];
					if (can_grow[i] && can_grow[(i + 3) % 4]) {
						tile_atlas_control->draw_texture_rect(resize_handle, Rect2(pos, zoomed_size), false);
					} else {
						tile_atlas_control->draw_texture_rect(resize_handle_disabled, Rect2(pos, zoomed_size), false);
					}
					Vector2 next_pos = rect.position + rect.size * coords[(i + 1) % 4];
					if (can_grow[i]) {
						tile_atlas_control->draw_texture_rect(resize_handle, Rect2((pos + next_pos) / 2.0, zoomed_size), false);
					} else {
						tile_atlas_control->draw_texture_rect(resize_handle_disabled, Rect2((pos + next_pos) / 2.0, zoomed_size), false);
					}
				}
			}
		}
	}

	if (drag_type == DRAG_TYPE_REMOVE_TILES) {
		// Draw the tiles to be removed.

		for (const RBSet<Vector2i>::Element *E = drag_modified_tiles.front(); E; E = E->next()) {
			for (int frame = 0; frame < tile_set_atlas_source->get_tile_animation_frames_count(E->get()); frame++) {
				LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, tile_set_atlas_source->get_tile_texture_region(E->get(), frame), Color(0.0, 0.0, 0.0));
			}
		}
	} else if (drag_type == DRAG_TYPE_RECT_SELECT || drag_type == DRAG_TYPE_REMOVE_TILES_USING_RECT) {
		// Draw tiles to be removed.
		Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
		Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
		Rect2i area = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
		area.set_end((area.get_end() + Vector2i(1, 1)).min(tile_set_atlas_source->get_atlas_grid_size()));

		Color color = Color(0.0, 0.0, 0.0);
		if (drag_type == DRAG_TYPE_RECT_SELECT) {
			color = Color(1.0, 1.0, 0.0);
		}

		RBSet<Vector2i> to_paint;
		for (int x = area.get_position().x; x < area.get_end().x; x++) {
			for (int y = area.get_position().y; y < area.get_end().y; y++) {
				Vector2i coords = tile_set_atlas_source->get_tile_at_coords(Vector2i(x, y));
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					to_paint.insert(coords);
				}
			}
		}

		for (const RBSet<Vector2i>::Element *E = to_paint.front(); E; E = E->next()) {
			Vector2i coords = E->get();
			LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, tile_set_atlas_source->get_tile_texture_region(coords), color);
		}
	} else if (drag_type == DRAG_TYPE_CREATE_TILES_USING_RECT) {
		// Draw tiles to be created.
		Vector2i margins = tile_set_atlas_source->get_margins();
		Vector2i separation = tile_set_atlas_source->get_separation();
		Vector2i tile_size = tile_set_atlas_source->get_texture_region_size();

		Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
		Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
		Rect2i area = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
		area.set_end((area.get_end() + Vector2i(1, 1)).min(tile_set_atlas_source->get_atlas_grid_size()));
		for (int x = area.get_position().x; x < area.get_end().x; x++) {
			for (int y = area.get_position().y; y < area.get_end().y; y++) {
				Vector2i coords = Vector2i(x, y);
				if (tile_set_atlas_source->get_tile_at_coords(coords) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					Vector2i origin = margins + (coords * (tile_size + separation));
					LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, Rect2i(origin, tile_size));
				}
			}
		}
	}

	// Draw the hovered tile.
	if (drag_type == DRAG_TYPE_REMOVE_TILES_USING_RECT || drag_type == DRAG_TYPE_CREATE_TILES_USING_RECT) {
		// Draw the rect.
		Vector2i start_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_mouse_pos, true);
		Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position(), true);
		Rect2i area = Rect2i(start_base_tiles_coords, new_base_tiles_coords - start_base_tiles_coords).abs();
		area.set_end((area.get_end() + Vector2i(1, 1)).min(tile_set_atlas_source->get_atlas_grid_size()));
		Vector2i margins = tile_set_atlas_source->get_margins();
		Vector2i separation = tile_set_atlas_source->get_separation();
		Vector2i tile_size = tile_set_atlas_source->get_texture_region_size();
		Vector2i origin = margins + (area.position * (tile_size + separation));
		LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, Rect2i(origin, area.size * tile_size));
	} else {
		Vector2i grid_size = tile_set_atlas_source->get_atlas_grid_size();
		if (hovered_base_tile_coords.x >= 0 && hovered_base_tile_coords.y >= 0 && hovered_base_tile_coords.x < grid_size.x && hovered_base_tile_coords.y < grid_size.y) {
			Vector2i hovered_tile = tile_set_atlas_source->get_tile_at_coords(hovered_base_tile_coords);
			if (hovered_tile != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				// Draw existing hovered tile.
				for (int frame = 0; frame < tile_set_atlas_source->get_tile_animation_frames_count(hovered_tile); frame++) {
					Color color = Color(1.0, 0.8, 0.0, frame == 0 ? 0.6 : 0.3);
					LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, tile_set_atlas_source->get_tile_texture_region(hovered_tile, frame), color);
				}
			} else {
				// Draw empty tile, only in add/remove tiles mode.
				if (tools_button_group->get_pressed_button() == tool_setup_atlas_source_button) {
					Vector2i margins = tile_set_atlas_source->get_margins();
					Vector2i separation = tile_set_atlas_source->get_separation();
					Vector2i tile_size = tile_set_atlas_source->get_texture_region_size();
					Vector2i origin = margins + (hovered_base_tile_coords * (tile_size + separation));
					LayeredTilesEditorUtils::draw_selection_rect(tile_atlas_control, Rect2i(origin, tile_size));
				}
			}
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_atlas_control_unscaled_draw() {
	if (current_tile_data_editor) {
		// Draw the preview of the selected property.
		for (int i = 0; i < tile_set_atlas_source->get_tiles_count(); i++) {
			Vector2i coords = tile_set_atlas_source->get_tile_id(i);
			Rect2i texture_region = tile_set_atlas_source->get_tile_texture_region(coords);
			Vector2 position = ((Rect2)texture_region).get_center() + tile_set_atlas_source->get_tile_data(coords, 0)->get_texture_origin();

			Transform2D xform = tile_atlas_control->get_parent_control()->get_transform();
			xform.translate_local(position);

			if (tools_button_group->get_pressed_button() == tool_select_button && selection.has({ coords, 0 })) {
				continue;
			}

			LayeredTileMapCell cell;
			cell.source_id = tile_set_atlas_source_id;
			cell.set_atlas_coords(coords);
			cell.alternative_tile = 0;
			current_tile_data_editor->draw_over_tile(tile_atlas_control_unscaled, xform, cell);
		}

		// Draw the selection on top of other.
		if (tools_button_group->get_pressed_button() == tool_select_button) {
			for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
				if (E->get().alternative != 0) {
					continue;
				}
				Rect2i texture_region = tile_set_atlas_source->get_tile_texture_region(E->get().tile);
				Vector2 position = ((Rect2)texture_region).get_center() + tile_set_atlas_source->get_tile_data(E->get().tile, 0)->get_texture_origin();

				Transform2D xform = tile_atlas_control->get_parent_control()->get_transform();
				xform.translate_local(position);

				LayeredTileMapCell cell;
				cell.source_id = tile_set_atlas_source_id;
				cell.set_atlas_coords(E->get().tile);
				cell.alternative_tile = 0;
				current_tile_data_editor->draw_over_tile(tile_atlas_control_unscaled, xform, cell, true);
			}
		}

		// Call the TileData's editor custom draw function.
		if (tools_button_group->get_pressed_button() == tool_paint_button) {
			Transform2D xform = tile_atlas_control->get_parent_control()->get_transform();
			current_tile_data_editor->forward_draw_over_atlas(tile_atlas_view, tile_set_atlas_source, tile_atlas_control_unscaled, xform);
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_gui_input(const Ref<InputEvent> &p_event) {
	// Update the hovered alternative tile.
	hovered_alternative_tile_coords = tile_atlas_view->get_alternative_tile_at_pos(alternative_tiles_control->get_local_mouse_position());

	// Forward the event to the current tile data editor if we are in the painting mode.
	if (tools_button_group->get_pressed_button() == tool_paint_button) {
		if (current_tile_data_editor) {
			current_tile_data_editor->forward_painting_alternatives_gui_input(tile_atlas_view, tile_set_atlas_source, p_event);
		}
		tile_atlas_control->update();
		tile_atlas_control_unscaled->update();
		alternative_tiles_control->update();
		alternative_tiles_control_unscaled->update();
		tile_atlas_view->update();
		return;
	}

	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		tile_atlas_control->update();
		tile_atlas_control_unscaled->update();
		alternative_tiles_control->update();
		alternative_tiles_control_unscaled->update();

		if (drag_type == DRAG_TYPE_MAY_POPUP_MENU) {
			if (Vector2(drag_start_mouse_pos).distance_to(alternative_tiles_control->get_local_mouse_position()) > 5.0 * EDSCALE) {
				drag_type = DRAG_TYPE_NONE;
			}
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		Vector2 mouse_local_pos = alternative_tiles_control->get_local_mouse_position();
		if (mb->get_button_index() == BUTTON_LEFT) {
			if (mb->is_pressed()) {
				// Left click pressed.
				if (tools_button_group->get_pressed_button() == tool_select_button) {
					Vector3 tile = tile_atlas_view->get_alternative_tile_at_pos(mouse_local_pos);

					selection.clear();
					TileSelection selected = { Vector2i(tile.x, tile.y), int(tile.z) };
					if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						selection.insert(selected);
					}

					_update_tile_inspector();
					_update_tile_id_label();
				}
			}
		} else if (mb->get_button_index() == BUTTON_RIGHT) {
			if (mb->is_pressed()) {
				drag_type = DRAG_TYPE_MAY_POPUP_MENU;
				drag_start_mouse_pos = alternative_tiles_control->get_local_mouse_position();
			} else {
				if (drag_type == DRAG_TYPE_MAY_POPUP_MENU) {
					// Right click released and wasn't dragged too far
					Vector3 tile = tile_atlas_view->get_alternative_tile_at_pos(mouse_local_pos);

					selection.clear();
					TileSelection selected = { Vector2i(tile.x, tile.y), int(tile.z) };
					if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						selection.insert(selected);
					}

					_update_tile_inspector();
					_update_tile_id_label();

					if (selection.size() == 1) {
						selected = selection.front()->get();
						menu_option_coords = selected.tile;
						menu_option_alternative = selected.alternative;
						alternative_tile_popup_menu->set_position(get_global_transform().xform(get_local_mouse_position()));
						alternative_tile_popup_menu->popup();
					}
				}

				drag_type = DRAG_TYPE_NONE;
			}
		}
		tile_atlas_control->update();
		tile_atlas_control_unscaled->update();
		alternative_tiles_control->update();
		alternative_tiles_control_unscaled->update();
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_mouse_exited() {
	hovered_alternative_tile_coords = Vector3i(LayeredTileSetSource::INVALID_ATLAS_COORDS.x, LayeredTileSetSource::INVALID_ATLAS_COORDS.y, LayeredTileSetSource::INVALID_TILE_ALTERNATIVE);
	tile_atlas_control->update();
	tile_atlas_control_unscaled->update();
	alternative_tiles_control->update();
	alternative_tiles_control_unscaled->update();
}

void LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_draw() {
	// Update the hovered alternative tile.
	if (tools_button_group->get_pressed_button() == tool_select_button) {
		// Draw hovered tile.
		Vector2i coords = Vector2(hovered_alternative_tile_coords.x, hovered_alternative_tile_coords.y);
		if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
			Rect2i rect = tile_atlas_view->get_alternative_tile_rect(coords, hovered_alternative_tile_coords.z);
			if (rect != Rect2i()) {
				LayeredTilesEditorUtils::draw_selection_rect(alternative_tiles_control, rect, Color(1.0, 0.8, 0.0, 0.5));
			}
		}

		// Draw selected tile.
		for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
			TileSelection selected = E->get();
			if (selected.alternative >= 1) {
				Rect2i rect = tile_atlas_view->get_alternative_tile_rect(selected.tile, selected.alternative);
				if (rect != Rect2i()) {
					LayeredTilesEditorUtils::draw_selection_rect(alternative_tiles_control, rect);
				}
			}
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_unscaled_draw() {
	// Draw the preview of the selected property.
	if (current_tile_data_editor) {
		// Draw the preview of the currently selected property.
		for (int i = 0; i < tile_set_atlas_source->get_tiles_count(); i++) {
			Vector2i coords = tile_set_atlas_source->get_tile_id(i);
			for (int j = 0; j < tile_set_atlas_source->get_alternative_tiles_count(coords); j++) {
				int alternative_tile = tile_set_atlas_source->get_alternative_tile_id(coords, j);
				if (alternative_tile == 0) {
					continue;
				}
				Rect2i rect = tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
				Vector2 position = rect.get_center() + tile_set_atlas_source->get_tile_data(coords, alternative_tile)->get_texture_origin();

				Transform2D xform = alternative_tiles_control->get_parent_control()->get_transform();
				xform.translate_local(position);

				if (tools_button_group->get_pressed_button() == tool_select_button && selection.has({ coords, alternative_tile })) {
					continue;
				}

				LayeredTileMapCell cell;
				cell.source_id = tile_set_atlas_source_id;
				cell.set_atlas_coords(coords);
				cell.alternative_tile = alternative_tile;
				current_tile_data_editor->draw_over_tile(alternative_tiles_control_unscaled, xform, cell);
			}
		}

		// Draw the selection on top of other.
		if (tools_button_group->get_pressed_button() == tool_select_button) {
			for (const RBSet<TileSelection>::Element *E = selection.front(); E; E = E->next()) {
				if (E->get().alternative == 0) {
					continue;
				}
				Rect2i rect = tile_atlas_view->get_alternative_tile_rect(E->get().tile, E->get().alternative);
				Vector2 position = rect.get_center() + tile_set_atlas_source->get_tile_data(E->get().tile, E->get().alternative)->get_texture_origin();

				Transform2D xform = alternative_tiles_control->get_parent_control()->get_transform();
				xform.translate_local(position);

				LayeredTileMapCell cell;
				cell.source_id = tile_set_atlas_source_id;
				cell.set_atlas_coords(E->get().tile);
				cell.alternative_tile = E->get().alternative;
				current_tile_data_editor->draw_over_tile(alternative_tiles_control_unscaled, xform, cell, true);
			}
		}

		// Call the TileData's editor custom draw function.
		if (tools_button_group->get_pressed_button() == tool_paint_button) {
			Transform2D xform = tile_atlas_control->get_parent_control()->get_transform();
			current_tile_data_editor->forward_draw_over_alternatives(tile_atlas_view, tile_set_atlas_source, alternative_tiles_control_unscaled, xform);
		}
	}
}

void LayeredTileSetAtlasSourceEditor::_tile_set_changed() {
	if (tile_set->get_source_count() == 0) {
		// No sources, so nothing to do here anymore.
		tile_set->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
		tile_set = Ref<LayeredTileSet>();
		return;
	}

	tile_set_changed_needs_update = true;
}

void LayeredTileSetAtlasSourceEditor::_tile_proxy_object_changed(const String &p_what) {
	tile_set_changed_needs_update = false; // Avoid updating too many things.
	_update_atlas_view();
}

void LayeredTileSetAtlasSourceEditor::_atlas_source_proxy_object_changed(const String &p_what) {
	if (p_what == "texture" && !atlas_source_proxy_object->get("texture").is_null()) {
		atlases_to_auto_create_tiles.clear();
		atlases_to_auto_create_tiles.push_back(tile_set_atlas_source);
		confirm_auto_create_tiles->popup_centered();
	} else if (p_what == "id") {
		emit_signal("source_id_changed", atlas_source_proxy_object->get_id());
	}
}

void LayeredTileSetAtlasSourceEditor::_undo_redo_inspector_callback(Object *p_undo_redo, Object *p_edited, const String &p_property, const Variant &p_new_value) {
	UndoRedo *undo_redo = Object::cast_to<UndoRedo>(p_undo_redo);
	ERR_FAIL_NULL(undo_redo);

#define ADD_UNDO(obj, property) undo_redo->add_undo_property(obj, property, obj->get(property));

	LayeredAtlasTileProxyObject *tile_data_proxy = Object::cast_to<LayeredAtlasTileProxyObject>(p_edited);
	if (tile_data_proxy) {
		Vector<String> components = String(p_property).split("/", true, 2);
		if (components.size() == 2 && components[1] == "polygons_count") {
			int layer_index = components[0].trim_prefix("physics_layer_").to_int();
			int new_polygons_count = p_new_value;
			int old_polygons_count = tile_data_proxy->get(vformat("physics_layer_%d/polygons_count", layer_index));
			if (new_polygons_count < old_polygons_count) {
				for (int i = new_polygons_count; i < old_polygons_count; i++) {
					ADD_UNDO(tile_data_proxy, vformat("physics_layer_%d/polygon_%d/points", layer_index, i));
					ADD_UNDO(tile_data_proxy, vformat("physics_layer_%d/polygon_%d/one_way", layer_index, i));
					ADD_UNDO(tile_data_proxy, vformat("physics_layer_%d/polygon_%d/one_way_margin", layer_index, i));
				}
			}
		} else if (p_property == "terrain_set") {
			int current_terrain_set = tile_data_proxy->get("terrain_set");
			ADD_UNDO(tile_data_proxy, "terrain");
			for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
				LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
				if (tile_set->is_valid_terrain_peering_bit(current_terrain_set, bit)) {
					ADD_UNDO(tile_data_proxy, "terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]));
				}
			}
		}
	}
#undef ADD_UNDO
}

Vector2i LayeredTileSetAtlasSourceEditor::_get_drag_offset_tile_coords(const Vector2i &p_offset) const {
	Vector2i half_tile_size = tile_set->get_tile_size() / 2;
	Vector2i new_base_tiles_coords = tile_atlas_view->get_atlas_tile_coords_at_pos(tile_atlas_control->get_local_mouse_position() + half_tile_size * p_offset);
	return new_base_tiles_coords.max(Vector2i(-1, -1)).min(tile_set_atlas_source->get_atlas_grid_size());
}

void LayeredTileSetAtlasSourceEditor::edit(Ref<LayeredTileSet> p_tile_set, LayeredTileSetAtlasSource *p_tile_set_atlas_source, int p_source_id) {
	ERR_FAIL_COND(!p_tile_set.is_valid());
	ERR_FAIL_NULL(p_tile_set_atlas_source);
	ERR_FAIL_COND(p_source_id < 0);
	ERR_FAIL_COND(p_tile_set->get_source(p_source_id) != p_tile_set_atlas_source);

	if (p_tile_set == tile_set && p_tile_set_atlas_source == tile_set_atlas_source && p_source_id == tile_set_atlas_source_id) {
		return;
	}

	// Remove listener for old objects.
	if (tile_set.is_valid()) {
		tile_set->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
	}

	if (tile_set_atlas_source) {
		tile_set_atlas_source->disconnect(CoreStringNames::get_singleton()->changed, this, "_update_source_texture");
		if (atlas_source_texture.is_valid()) {
			atlas_source_texture->disconnect(CoreStringNames::get_singleton()->changed, this, "_check_outside_tiles");
			atlas_source_texture = Ref<Texture>();
		}
	}

	// Clear the selection.
	selection.clear();

	// Change the edited object.
	tile_set = p_tile_set;
	tile_set_atlas_source = p_tile_set_atlas_source;
	tile_set_atlas_source_id = p_source_id;

	if (tile_set.is_valid()) {
		tile_set->connect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
	}

	if (tile_set_atlas_source) {
		tile_set_atlas_source->connect(CoreStringNames::get_singleton()->changed, this, "_update_source_texture");
		_update_source_texture();
	}

	if (read_only && tools_button_group->get_pressed_button() == tool_paint_button) {
		tool_paint_button->set_pressed(false);
		tool_setup_atlas_source_button->set_pressed(true);
	}

	// Disable buttons in read-only mode.
	tool_paint_button->set_disabled(read_only);
	tools_settings_erase_button->set_disabled(read_only);
	tool_advanced_menu_button->set_disabled(read_only);

	// Update everything.
	_update_source_inspector();

	// Update the selected tile.
	_update_fix_selected_and_hovered_tiles();
	_update_tile_id_label();
	_update_atlas_view();
	_update_atlas_source_inspector();
	_update_tile_inspector();
	_update_tile_data_editors();
	_update_current_tile_data_editor();
}

void LayeredTileSetAtlasSourceEditor::init_new_atlases(const Vector<Ref<LayeredTileSetAtlasSource>> &p_atlases) {
	tool_setup_atlas_source_button->set_pressed(true);
	atlases_to_auto_create_tiles = p_atlases;
	confirm_auto_create_tiles->popup_centered();
}

void LayeredTileSetAtlasSourceEditor::_update_source_texture() {
	if (tile_set_atlas_source && tile_set_atlas_source->get_texture() == atlas_source_texture) {
		return;
	}

	if (atlas_source_texture.is_valid()) {
		atlas_source_texture->disconnect(CoreStringNames::get_singleton()->changed, this, "_check_outside_tiles");
		atlas_source_texture = Ref<Texture>();
	}

	if (!tile_set_atlas_source || tile_set_atlas_source->get_texture().is_null()) {
		return;
	}
	atlas_source_texture = tile_set_atlas_source->get_texture();
	atlas_source_texture->connect(CoreStringNames::get_singleton()->changed, this, "_check_outside_tiles", varray(), CONNECT_DEFERRED);
	_check_outside_tiles();
}

void LayeredTileSetAtlasSourceEditor::_check_outside_tiles() {
	ERR_FAIL_NULL(tile_set_atlas_source);
	outside_tiles_warning->set_visible(!read_only && tile_set_atlas_source->has_tiles_outside_texture());
	tool_advanced_menu_button->get_popup()->set_item_disabled(tool_advanced_menu_button->get_popup()->get_item_index(ADVANCED_CLEANUP_TILES), !tile_set_atlas_source->has_tiles_outside_texture());
}

void LayeredTileSetAtlasSourceEditor::_cleanup_outside_tiles() {
	ERR_FAIL_NULL(tile_set_atlas_source);

	List<PropertyInfo> list;
	tile_set_atlas_source->get_property_list(&list);
	HashMap<Vector2i, List<const PropertyInfo *>> per_tile = _group_properties_per_tiles(list, tile_set_atlas_source);
	Vector<Vector2i> tiles_outside = tile_set_atlas_source->get_tiles_outside_texture();

	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action(TTR("Remove Tiles Outside the Texture"));

	undo_redo->add_do_method(tile_set_atlas_source, "clear_tiles_outside_texture");

	for (int i = 0; i < tiles_outside.size(); ++i) {
		const Vector2i &coords = tiles_outside[i];

		undo_redo->add_undo_method(tile_set_atlas_source, "create_tile", coords);
		if (per_tile.has(coords)) {
			for (List<const PropertyInfo *>::Element *E_property = per_tile[coords].front(); E_property; E_property = E_property->next()) {
				String property = E_property->get()->name;
				Variant value = tile_set_atlas_source->get(property);
				if (value.get_type() != Variant::NIL) {
					undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
				}
			}
		}
	}

	undo_redo->add_do_method(this, "_check_outside_tiles");
	undo_redo->add_undo_method(this, "_check_outside_tiles");
	undo_redo->commit_action();
	outside_tiles_warning->hide();
}

void LayeredTileSetAtlasSourceEditor::_auto_create_tiles() {
	for (int i = 0; i < atlases_to_auto_create_tiles.size(); ++i) {
		Ref<LayeredTileSetAtlasSource> atlas_source = atlases_to_auto_create_tiles[i];

		if (atlas_source.is_valid()) {
			Ref<Texture> texture = atlas_source->get_texture();
			if (texture.is_valid()) {
				Vector2i margins = atlas_source->get_margins();
				Vector2i separation = atlas_source->get_separation();
				Vector2i texture_region_size = atlas_source->get_texture_region_size();
				Size2i grid_size = atlas_source->get_atlas_grid_size();
				UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
				undo_redo->create_action(TTR("Create tiles in non-transparent texture regions"));
				for (int y = 0; y < grid_size.y; y++) {
					for (int x = 0; x < grid_size.x; x++) {
						// Check if we have a tile at the coord.
						Vector2i coords = Vector2i(x, y);
						if (atlas_source->get_tile_at_coords(coords) == LayeredTileSetSource::INVALID_ATLAS_COORDS) {
							// Check if the texture is empty at the given coords.
							Rect2i region = Rect2i(margins + (coords * (texture_region_size + separation)), texture_region_size);
							bool is_opaque = false;
							for (int region_x = region.get_position().x; region_x < region.get_end().x; region_x++) {
								for (int region_y = region.get_position().y; region_y < region.get_end().y; region_y++) {
									if (texture->is_pixel_opaque(region_x, region_y)) {
										is_opaque = true;
										break;
									}
								}
								if (is_opaque) {
									break;
								}
							}

							// If we do have opaque pixels, create a tile.
							if (is_opaque) {
								undo_redo->add_do_method(*atlas_source, "create_tile", coords);
								undo_redo->add_undo_method(*atlas_source, "remove_tile", coords);
							}
						}
					}
				}
				undo_redo->commit_action();
			}
		}
	}

	_cancel_auto_create_tiles();
}

void LayeredTileSetAtlasSourceEditor::_cancel_auto_create_tiles() {
	atlases_to_auto_create_tiles.clear();
}

void LayeredTileSetAtlasSourceEditor::_auto_remove_tiles() {
	if (!tile_set_atlas_source) {
		return;
	}

	Ref<Texture> texture = tile_set_atlas_source->get_texture();
	if (texture.is_valid()) {
		Vector2i margins = tile_set_atlas_source->get_margins();
		Vector2i separation = tile_set_atlas_source->get_separation();
		Vector2i texture_region_size = tile_set_atlas_source->get_texture_region_size();
		Vector2i grid_size = tile_set_atlas_source->get_atlas_grid_size();

		UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
		undo_redo->create_action(TTR("Remove tiles in fully transparent texture regions"));

		List<PropertyInfo> list;
		tile_set_atlas_source->get_property_list(&list);
		HashMap<Vector2i, List<const PropertyInfo *>> per_tile = _group_properties_per_tiles(list, tile_set_atlas_source);

		for (int i = 0; i < tile_set_atlas_source->get_tiles_count(); i++) {
			Vector2i coords = tile_set_atlas_source->get_tile_id(i);
			Vector2i size_in_atlas = tile_set_atlas_source->get_tile_size_in_atlas(coords);

			// Skip tiles outside texture.
			if ((coords.x + size_in_atlas.x) > grid_size.x || (coords.y + size_in_atlas.y) > grid_size.y) {
				continue;
			}

			// Check if the texture is empty at the given coords.
			Rect2i region = Rect2i(margins + (coords * (texture_region_size + separation)), texture_region_size * size_in_atlas);
			bool is_opaque = false;
			for (int region_x = region.get_position().x; region_x < region.get_end().x; region_x++) {
				for (int region_y = region.get_position().y; region_y < region.get_end().y; region_y++) {
					if (texture->is_pixel_opaque(region_x, region_y)) {
						is_opaque = true;
						break;
					}
				}
				if (is_opaque) {
					break;
				}
			}

			// If we do have opaque pixels, create a tile.
			if (!is_opaque) {
				undo_redo->add_do_method(tile_set_atlas_source, "remove_tile", coords);
				undo_redo->add_undo_method(tile_set_atlas_source, "create_tile", coords);
				if (per_tile.has(coords)) {
					for (List<const PropertyInfo *>::Element *E_property = per_tile[coords].front(); E_property; E_property = E_property->next()) {
						String property = E_property->get()->name;
						Variant value = tile_set_atlas_source->get(property);
						if (value.get_type() != Variant::NIL) {
							undo_redo->add_undo_method(tile_set_atlas_source, "set", E_property->get()->name, value);
						}
					}
				}
			}
		}
		undo_redo->commit_action();
	}
}

void LayeredTileSetAtlasSourceEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			atlas_source_inspector->edit(atlas_source_proxy_object);
			/*
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "id", TTR("The tile's unique identifier within this LayeredTileSet. Each tile stores its source ID, so changing one may make tiles invalid."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "name", TTR("The human-readable name for the atlas. Use a descriptive name here for organizational purposes (such as \"terrain\", \"decoration\", etc.)."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "texture", TTR("The image from which the tiles will be created."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "margins", TTR("The margins on the image's edges that should not be selectable as tiles (in pixels). Increasing this can be useful if you download a tilesheet image that has margins on the edges (e.g. for attribution)."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "separation", TTR("The separation between each tile on the atlas in pixels. Increasing this can be useful if the tilesheet image you're using contains guides (such as outlines between every tile)."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "texture_region_size", TTR("The size of each tile on the atlas in pixels. In most cases, this should match the tile size defined in the LayeredTileMap property (although this is not strictly necessary)."));
			atlas_source_inspector->add_custom_property_description("LayeredTileSetAtlasSourceProxyObject", "use_texture_padding", TTR("If checked, adds a 1-pixel transparent edge around each tile to prevent texture bleeding when filtering is enabled. It's recommended to leave this enabled unless you're running into rendering issues due to texture padding."));
			*/

			tile_inspector->edit(tile_proxy_object);
			/*
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "atlas_coords", TTR("The position of the tile's top-left corner in the atlas. The position and size must be within the atlas and can't overlap another tile.\nEach painted tile has associated atlas coords, so changing this property may cause your LayeredTileMaps to not display properly."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "size_in_atlas", TTR("The unit size of the tile."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "animation_columns", TTR("Number of columns for the animation grid. If number of columns is lower than number of frames, the animation will automatically adjust row count."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "animation_separation", TTR("The space (in tiles) between each frame of the animation."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "animation_speed", TTR("Animation speed in frames per second."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "animation_mode", TTR("Determines how animation will start. In \"Default\" mode all tiles start animating at the same frame. In \"Random Start Times\" mode, each tile starts animation with a random offset."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "flip_h", TTR("If [code]true[/code], the tile is horizontally flipped."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "flip_v", TTR("If [code]true[/code], the tile is vertically flipped."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "transpose", TTR("If [code]true[/code], the tile is rotated 90 degrees [i]counter-clockwise[/i] and then flipped vertically. In practice, this means that to rotate a tile by 90 degrees clockwise without flipping it, you should enable [b]Flip H[/b] and [b]Transpose[/b]. To rotate a tile by 180 degrees clockwise, enable [b]Flip H[/b] and [b]Flip V[/b]. To rotate a tile by 270 degrees clockwise, enable [b]Flip V[/b] and [b]Transpose[/b]."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "texture_origin", TTR("The origin to use for drawing the tile. This can be used to visually offset the tile compared to the base tile."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "modulate", TTR("The color multiplier to use when rendering the tile."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "material", TTR("The material to use for this tile. This can be used to apply a different blend mode or custom shaders to a single tile."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "z_index", TTR("The sorting order for this tile. Higher values will make the tile render in front of others on the same layer. The index is relative to the LayeredTileMap's own Z index."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "y_sort_origin", TTR("The vertical offset to use for tile sorting based on its Y coordinate (in pixels). This allows using layers as if they were on different height for top-down games. Adjusting this can help alleviate issues with sorting certain tiles. Only effective if Y Sort Enabled is true on the LayeredTileMap layer the tile is placed on."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "terrain_set", TTR("The index of the terrain set this tile belongs to. [code]-1[/code] means it will not be used in terrains."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "terrain", TTR("The index of the terrain inside the terrain set this tile belongs to. [code]-1[/code] means it will not be used in terrains."));
			tile_inspector->add_custom_property_description("LayeredAtlasTileProxyObject", "probability", TTR("The relative probability of this tile appearing when painting with \"Place Random Tile\" enabled."));
			*/
		} break;

		case NOTIFICATION_THEME_CHANGED: {
			tool_setup_atlas_source_button->set_icon(get_theme_icon("Tools", "EditorIcons"));
			tool_select_button->set_icon(get_theme_icon("ToolSelect", "EditorIcons"));
			tool_paint_button->set_icon(get_theme_icon("Paint", "EditorIcons"));

			tools_settings_erase_button->set_icon(get_theme_icon("Eraser", "EditorIcons"));
			tool_advanced_menu_button->set_icon(get_theme_icon("GuiTabMenuHl", "EditorIcons"));
			outside_tiles_warning->set_texture(get_theme_icon("StatusWarning", "EditorIcons"));

			resize_handle = get_theme_icon("EditorHandle", "EditorIcons");
			resize_handle_disabled = get_theme_icon("EditorHandleDisabled", "EditorIcons");
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (tile_set_changed_needs_update) {
				// Read-only is off by default
				read_only = false;

				// Disable buttons in read-only mode.
				tool_paint_button->set_disabled(read_only);
				tools_settings_erase_button->set_disabled(read_only);
				tool_advanced_menu_button->set_disabled(read_only);

				// Update everything.
				_update_source_inspector();

				// Update the selected tile.
				_update_fix_selected_and_hovered_tiles();
				_update_tile_id_label();
				_update_atlas_view();
				_update_atlas_source_inspector();
				_update_tile_inspector();
				_update_tile_data_editors();
				_update_current_tile_data_editor();

				tile_set_changed_needs_update = false;
			}
		} break;

		case NOTIFICATION_EXIT_TREE: {
			for (HashMap<String, LayeredTileDataEditor *>::Element *E = tile_data_editors.front(); E; E = E->next) {
				Control *toolbar = E->value()->get_toolbar();
				if (toolbar->get_parent() == tool_settings_tile_data_toolbar_container) {
					tool_settings_tile_data_toolbar_container->remove_child(toolbar);
				}
			}
		} break;
	}
}

void LayeredTileSetAtlasSourceEditor::_tools_button_group_pressed(Object *p_button) {
	_update_fix_selected_and_hovered_tiles();
	_update_tile_id_label();
	_update_atlas_source_inspector();
	_update_tile_inspector();
	_update_tile_data_editors();
	_update_current_tile_data_editor();
	_update_atlas_view();
	_update_toolbar();
}

void LayeredTileSetAtlasSourceEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_set_selection_from_array"), &LayeredTileSetAtlasSourceEditor::_set_selection_from_array);
	ClassDB::bind_method(D_METHOD("_check_outside_tiles"), &LayeredTileSetAtlasSourceEditor::_check_outside_tiles);
	ClassDB::bind_method(D_METHOD("_tile_set_changed"), &LayeredTileSetAtlasSourceEditor::_tile_set_changed);
	ClassDB::bind_method(D_METHOD("_tools_button_group_pressed"), &LayeredTileSetAtlasSourceEditor::_tools_button_group_pressed);
	ClassDB::bind_method(D_METHOD("_update_source_texture"), &LayeredTileSetAtlasSourceEditor::_update_source_texture);
	ClassDB::bind_method(D_METHOD("_tile_proxy_object_changed"), &LayeredTileSetAtlasSourceEditor::_tile_proxy_object_changed);
	ClassDB::bind_method(D_METHOD("_inspector_property_selected"), &LayeredTileSetAtlasSourceEditor::_inspector_property_selected);
	ClassDB::bind_method(D_METHOD("_tile_data_editor_dropdown_button_draw"), &LayeredTileSetAtlasSourceEditor::_tile_data_editor_dropdown_button_draw);
	ClassDB::bind_method(D_METHOD("_tile_data_editor_dropdown_button_pressed"), &LayeredTileSetAtlasSourceEditor::_tile_data_editor_dropdown_button_pressed);
	ClassDB::bind_method(D_METHOD("_tile_data_editors_tree_selected"), &LayeredTileSetAtlasSourceEditor::_tile_data_editors_tree_selected);
	ClassDB::bind_method(D_METHOD("_atlas_source_proxy_object_changed"), &LayeredTileSetAtlasSourceEditor::_atlas_source_proxy_object_changed);
	ClassDB::bind_method(D_METHOD("_menu_option"), &LayeredTileSetAtlasSourceEditor::_menu_option);
	ClassDB::bind_method(D_METHOD("_tile_atlas_control_draw"), &LayeredTileSetAtlasSourceEditor::_tile_atlas_control_draw);
	ClassDB::bind_method(D_METHOD("_tile_atlas_control_mouse_exited"), &LayeredTileSetAtlasSourceEditor::_tile_atlas_control_mouse_exited);
	ClassDB::bind_method(D_METHOD("_tile_atlas_control_gui_input"), &LayeredTileSetAtlasSourceEditor::_tile_atlas_control_gui_input);
	ClassDB::bind_method(D_METHOD("_tile_atlas_control_unscaled_draw"), &LayeredTileSetAtlasSourceEditor::_tile_atlas_control_unscaled_draw);
	ClassDB::bind_method(D_METHOD("_tile_alternatives_control_draw"), &LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_draw);
	ClassDB::bind_method(D_METHOD("_tile_alternatives_control_mouse_exited"), &LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_mouse_exited);
	ClassDB::bind_method(D_METHOD("_tile_alternatives_control_gui_input"), &LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_gui_input);
	ClassDB::bind_method(D_METHOD("_tile_alternatives_control_unscaled_draw"), &LayeredTileSetAtlasSourceEditor::_tile_alternatives_control_unscaled_draw);
	ClassDB::bind_method(D_METHOD("_auto_create_tiles"), &LayeredTileSetAtlasSourceEditor::_auto_create_tiles);
	ClassDB::bind_method(D_METHOD("_cancel_auto_create_tiles"), &LayeredTileSetAtlasSourceEditor::_cancel_auto_create_tiles);
	ClassDB::bind_method(D_METHOD("_tile_atlas_view_transform_changed"), &LayeredTileSetAtlasSourceEditor::_tile_atlas_view_transform_changed);

	ADD_SIGNAL(MethodInfo("source_id_changed", PropertyInfo(Variant::INT, "source_id")));
}

LayeredTileSetAtlasSourceEditor::LayeredTileSetAtlasSourceEditor() {
	set_shortcut_context(this);
	set_process_shortcut_input(true);
	set_process_internal(true);
	LayeredTileSetEditor::get_singleton()->register_split(this);

	// Middle panel.
	VBoxContainer *middle_vbox_container = memnew(VBoxContainer);
	middle_vbox_container->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
	add_child(middle_vbox_container);

	// -- Toolbox --
	tools_button_group.instance();
	tools_button_group->connect("pressed", this, "_tools_button_group_pressed");

	HBoxContainer *toolbox = memnew(HBoxContainer);
	middle_vbox_container->add_child(toolbox);

	tool_setup_atlas_source_button = memnew(Button);
	tool_setup_atlas_source_button->set_text(TTR("Setup"));
	tool_setup_atlas_source_button->set_theme_type_variation("FlatButton");
	tool_setup_atlas_source_button->set_toggle_mode(true);
	tool_setup_atlas_source_button->set_pressed(true);
	tool_setup_atlas_source_button->set_button_group(tools_button_group);
	tool_setup_atlas_source_button->set_tooltip(TTR("Atlas setup. Add/Remove tiles tool (use the shift key to create big tiles, control for rectangle editing)."));
	toolbox->add_child(tool_setup_atlas_source_button);

	tool_select_button = memnew(Button);
	tool_select_button->set_text(TTR("Select"));
	tool_select_button->set_theme_type_variation("FlatButton");
	tool_select_button->set_toggle_mode(true);
	tool_select_button->set_pressed(false);
	tool_select_button->set_button_group(tools_button_group);
	tool_select_button->set_tooltip(TTR("Select tiles."));
	toolbox->add_child(tool_select_button);

	tool_paint_button = memnew(Button);
	tool_paint_button->set_text(TTR("Paint"));
	tool_paint_button->set_theme_type_variation("FlatButton");
	tool_paint_button->set_toggle_mode(true);
	tool_paint_button->set_button_group(tools_button_group);
	tool_paint_button->set_tooltip(TTR("Paint properties."));
	toolbox->add_child(tool_paint_button);

	// Tile inspector.
	tile_proxy_object = memnew(LayeredAtlasTileProxyObject(this));
	tile_proxy_object->connect("changed", this, "_tile_proxy_object_changed");

	tile_inspector = memnew(EditorInspector);
	tile_inspector->set_v_size_flags(SIZE_EXPAND_FILL);
	tile_inspector->set_show_categories(true);
	tile_inspector->set_use_doc_hints(true);
	tile_inspector->set_use_folding(true);
	tile_inspector->connect("property_selected", this, "_inspector_property_selected");
	tile_inspector->set_undo_redo(EditorNode::get_singleton()->get_undo_redo());
	middle_vbox_container->add_child(tile_inspector);

	tile_inspector_no_tile_selected_label = memnew(Label);
	tile_inspector_no_tile_selected_label->set_v_size_flags(SIZE_EXPAND | SIZE_SHRINK_CENTER);
	tile_inspector_no_tile_selected_label->set_align(Label::ALIGN_CENTER);
	tile_inspector_no_tile_selected_label->set_text(TTR("No tiles selected.\nSelect one or more tiles from the palette to edit its properties."));
	tile_inspector_no_tile_selected_label->set_autowrap(true);
	middle_vbox_container->add_child(tile_inspector_no_tile_selected_label);

	// Property values palette.
	tile_data_editors_scroll = memnew(ScrollContainer);
	tile_data_editors_scroll->set_v_size_flags(SIZE_EXPAND_FILL);
	middle_vbox_container->add_child(tile_data_editors_scroll);

	VBoxContainer *tile_data_editors_vbox = memnew(VBoxContainer);
	tile_data_editors_vbox->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_data_editors_scroll->add_child(tile_data_editors_vbox);

	tile_data_editors_popup = memnew(Popup);

	tile_data_editors_label = memnew(Label);
	tile_data_editors_label->set_text(TTR("Paint Properties:"));
	tile_data_editors_label->set_theme_type_variation("HeaderSmall");
	tile_data_editors_vbox->add_child(tile_data_editors_label);

	tile_data_editor_dropdown_button = memnew(Button);
	tile_data_editor_dropdown_button->connect("draw", this, "_tile_data_editor_dropdown_button_draw");
	tile_data_editor_dropdown_button->connect("pressed", this, "_tile_data_editor_dropdown_button_pressed");
	tile_data_editors_vbox->add_child(tile_data_editor_dropdown_button);
	tile_data_editor_dropdown_button->add_child(tile_data_editors_popup);

	tile_data_editors_tree = memnew(Tree);
	tile_data_editors_tree->set_hide_root(true);
	tile_data_editors_tree->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	tile_data_editors_tree->set_h_scroll_enabled(false);
	tile_data_editors_tree->set_v_scroll_enabled(false);
	tile_data_editors_tree->connect("item_selected", this, "_tile_data_editors_tree_selected");
	tile_data_editors_popup->add_child(tile_data_editors_tree);

	tile_data_painting_editor_container = memnew(VBoxContainer);
	tile_data_painting_editor_container->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_data_editors_vbox->add_child(tile_data_painting_editor_container);

	// Atlas source inspector.
	atlas_source_proxy_object = memnew(LayeredTileSetAtlasSourceProxyObject());
	atlas_source_proxy_object->connect("changed", this, "_atlas_source_proxy_object_changed");

	atlas_source_inspector = memnew(EditorInspector);
	atlas_source_inspector->set_v_size_flags(SIZE_EXPAND_FILL);
	atlas_source_inspector->set_show_categories(true);
	atlas_source_inspector->set_use_doc_hints(true);
	atlas_source_inspector->set_undo_redo(EditorNode::get_singleton()->get_undo_redo());
	atlas_source_inspector->add_inspector_plugin(memnew(LayeredTileSourceInspectorPlugin));
	middle_vbox_container->add_child(atlas_source_inspector);

	// -- Right side --
	VBoxContainer *right_vbox_container = memnew(VBoxContainer);
	add_child(right_vbox_container);

	// Tool settings.
	tool_settings = memnew(HBoxContainer);
	right_vbox_container->add_child(tool_settings);

	tool_settings_tile_data_toolbar_container = memnew(HBoxContainer);
	tool_settings->add_child(tool_settings_tile_data_toolbar_container);

	tools_settings_erase_button = memnew(Button);
	tools_settings_erase_button->set_theme_type_variation("FlatButton");
	tools_settings_erase_button->set_toggle_mode(true);
	tools_settings_erase_button->set_shortcut(ED_SHORTCUT("layered_tiles_editor/eraser", TTR("Eraser"), KEY_E));
	tools_settings_erase_button->set_shortcut_context(this);
	tool_settings->add_child(tools_settings_erase_button);

	tool_advanced_menu_button = memnew(MenuButton);
	tool_advanced_menu_button->set_flat(false);
	tool_advanced_menu_button->set_theme_type_variation("FlatMenuButton");
	tool_advanced_menu_button->get_popup()->add_item(TTR("Create Tiles in Non-Transparent Texture Regions"), ADVANCED_AUTO_CREATE_TILES);
	tool_advanced_menu_button->get_popup()->add_item(TTR("Remove Tiles in Fully Transparent Texture Regions"), ADVANCED_AUTO_REMOVE_TILES);
	tool_advanced_menu_button->get_popup()->add_item(TTR("Remove Tiles Outside the Texture"), ADVANCED_CLEANUP_TILES);
	tool_advanced_menu_button->get_popup()->connect("id_pressed", this, "_menu_option");
	tool_settings->add_child(tool_advanced_menu_button);

	outside_tiles_warning = memnew(TextureRect);
	outside_tiles_warning->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	outside_tiles_warning->set_tooltip(vformat(TTR("The current atlas source has tiles outside the texture.\nYou can clear it using \"%s\" option in the 3 dots menu."), TTR("Remove Tiles Outside the Texture")));
	outside_tiles_warning->hide();
	tool_settings->add_child(outside_tiles_warning);

	_update_toolbar();

	// Right side of toolbar.
	Control *middle_space = memnew(Control);
	middle_space->set_h_size_flags(SIZE_EXPAND_FILL);
	tool_settings->add_child(middle_space);

	tool_tile_id_label = memnew(Label);
	tool_tile_id_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	tool_settings->add_child(tool_tile_id_label);
	_update_tile_id_label();

	// Right panel.
	VBoxContainer *right_panel = memnew(VBoxContainer);
	right_panel->set_h_size_flags(SIZE_EXPAND_FILL);
	right_panel->set_v_size_flags(SIZE_EXPAND_FILL);
	right_vbox_container->add_child(right_panel);

	// Tile atlas view.
	tile_atlas_view = memnew(LayeredTileAtlasView);
	tile_atlas_view->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_atlas_view->set_v_size_flags(SIZE_EXPAND_FILL);
	tile_atlas_view->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
	tile_atlas_view->connect("transform_changed", LayeredTilesEditorUtils::get_singleton(), "set_atlas_view_transform");
	tile_atlas_view->connect("transform_changed", this, "_tile_atlas_view_transform_changed");
	right_panel->add_child(tile_atlas_view);

	tile_create_help = memnew(VBoxContainer);
	tile_atlas_view->add_child(tile_create_help);
	tile_create_help->set_mouse_filter(MOUSE_FILTER_IGNORE);

	Label *help_label = memnew(Label(TTR("Hold Ctrl to create multiple tiles.")));
	tile_create_help->add_child(help_label);

	help_label = memnew(Label(TTR("Hold Shift to create big tiles.")));
	tile_create_help->add_child(help_label);

	tile_create_help->set_anchors_and_margins_preset(Control::PRESET_BOTTOM_LEFT, Control::PRESET_MODE_MINSIZE);
	tile_create_help->set_h_grow_direction(GrowDirection::GROW_DIRECTION_END);
	tile_create_help->set_v_grow_direction(GrowDirection::GROW_DIRECTION_BEGIN);

	base_tile_popup_menu = memnew(PopupMenu);
	base_tile_popup_menu->add_shortcut(ED_SHORTCUT("layered_tiles_editor/delete", TTR("Delete"), KEY_DELETE), TILE_DELETE);
	base_tile_popup_menu->add_item(TTR("Create an Alternative Tile"), TILE_CREATE_ALTERNATIVE);
	base_tile_popup_menu->connect("id_pressed", this, "_menu_option");
	tile_atlas_view->add_child(base_tile_popup_menu);

	empty_base_tile_popup_menu = memnew(PopupMenu);
	empty_base_tile_popup_menu->add_item(TTR("Create a Tile"), TILE_CREATE);
	empty_base_tile_popup_menu->connect("id_pressed", this, "_menu_option");
	tile_atlas_view->add_child(empty_base_tile_popup_menu);

	tile_atlas_control = memnew(LayeredTileAtlasControl(this));
	tile_atlas_control->connect("draw", this, "_tile_atlas_control_draw");
	tile_atlas_control->connect("mouse_exited", this, "_tile_atlas_control_mouse_exited");
	tile_atlas_control->connect("gui_input", this, "_tile_atlas_control_gui_input");
	tile_atlas_view->add_control_over_atlas_tiles(tile_atlas_control);

	tile_atlas_control_unscaled = memnew(Control);
	tile_atlas_control_unscaled->connect("draw", this, "_tile_atlas_control_unscaled_draw");
	tile_atlas_view->add_control_over_atlas_tiles(tile_atlas_control_unscaled, false);
	tile_atlas_control_unscaled->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

	alternative_tile_popup_menu = memnew(PopupMenu);
	alternative_tile_popup_menu->add_shortcut(ED_SHORTCUT("layered_tiles_editor/delete_tile", TTR("Delete"), KEY_DELETE), TILE_DELETE);
	alternative_tile_popup_menu->connect("id_pressed", this, "_menu_option");
	tile_atlas_view->add_child(alternative_tile_popup_menu);

	alternative_tiles_control = memnew(Control);
	alternative_tiles_control->connect("draw", this, "_tile_alternatives_control_draw");
	alternative_tiles_control->connect("mouse_exited", this, "_tile_alternatives_control_mouse_exited");
	alternative_tiles_control->connect("gui_input", this, "_tile_alternatives_control_gui_input");
	tile_atlas_view->add_control_over_alternative_tiles(alternative_tiles_control);

	alternative_tiles_control_unscaled = memnew(Control);
	alternative_tiles_control_unscaled->connect("draw", this, "_tile_alternatives_control_unscaled_draw");
	tile_atlas_view->add_control_over_alternative_tiles(alternative_tiles_control_unscaled, false);
	alternative_tiles_control_unscaled->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

	//EditorNode::get_editor_data().add_undo_redo_inspector_hook_callback(callable_mp(this, &LayeredTileSetAtlasSourceEditor::_undo_redo_inspector_callback));

	// -- Dialogs --
	confirm_auto_create_tiles = memnew(ConfirmationDialog);
	confirm_auto_create_tiles->set_title(TTR("Auto Create Tiles in Non-Transparent Texture Regions?"));
	confirm_auto_create_tiles->set_text(TTR("The atlas's texture was modified.\nWould you like to automatically create tiles in the atlas?"));
	confirm_auto_create_tiles->get_ok()->set_text(TTR("Yes"));
	confirm_auto_create_tiles->get_cancel()->set_text(TTR("No"));
	confirm_auto_create_tiles->connect("confirmed", this, "_auto_create_tiles");
	confirm_auto_create_tiles->get_cancel()->connect("pressed", this, "_cancel_auto_create_tiles");
	add_child(confirm_auto_create_tiles);

	// Inspector plugin.
	Ref<EditorInspectorPluginTileData> tile_data_inspector_plugin;
	tile_data_inspector_plugin.instance();
	EditorInspector::add_inspector_plugin(tile_data_inspector_plugin);
}

LayeredTileSetAtlasSourceEditor::~LayeredTileSetAtlasSourceEditor() {
	memdelete(tile_proxy_object);
	memdelete(atlas_source_proxy_object);

	// Remove listener for old objects, so the LayeredTileSet doesn't
	// try to call the destroyed LayeredTileSetAtlasSourceEditor.
	if (tile_set.is_valid()) {
		tile_set->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
	}
}

////// EditorPropertyTilePolygon //////

void EditorPropertyTilePolygon::_add_focusable_children(Node *p_node) {
	Control *control = Object::cast_to<Control>(p_node);
	if (control && control->get_focus_mode() != Control::FOCUS_NONE) {
		add_focusable(control);
	}
	for (int i = 0; i < p_node->get_child_count(); i++) {
		_add_focusable_children(p_node->get_child(i));
	}
}

void EditorPropertyTilePolygon::_polygons_changed() {
	if (String(count_property).empty()) {
		if (base_type == "OccluderPolygon2D") {
			// Single OccluderPolygon2D.
			Ref<OccluderPolygon2D> occluder;
			if (generic_tile_polygon_editor->get_polygon_count() >= 1) {
				occluder.instance();
				occluder->set_polygon(Variant(generic_tile_polygon_editor->get_polygon(0)));
			}
			emit_changed(get_edited_property(), occluder);
		} else if (base_type == "NavigationPolygon") {
			Ref<NavigationPolygon> navigation_polygon;
			if (generic_tile_polygon_editor->get_polygon_count() >= 1) {
				navigation_polygon.instance();

				if (generic_tile_polygon_editor->get_polygon_count() > 0) {
					Ref<NavigationMeshSourceGeometryData2D> source_geometry_data;
					source_geometry_data.instance();
					for (int i = 0; i < generic_tile_polygon_editor->get_polygon_count(); i++) {
						Vector<Vector2> polygon = generic_tile_polygon_editor->get_polygon(i);
						navigation_polygon->add_outline(Variant(polygon));
						source_geometry_data->add_traversable_outline(Variant(polygon));
					}
					navigation_polygon->set_agent_radius(0.0);
					NavigationMeshGenerator::get_singleton()->bake_2d_from_source_geometry_data(navigation_polygon, source_geometry_data);
				} else {
					navigation_polygon->clear();
				}
			}
			emit_changed(get_edited_property(), navigation_polygon);
		} else if (base_type == "Vector") {
			Vector<Vector2> result;

			if (generic_tile_polygon_editor->get_polygon_count() >= 1) {
				result = generic_tile_polygon_editor->get_polygon(0);
			}

			emit_changed(get_edited_property(), result);
		}
	} else {
		if (base_type.empty()) {
			// Multiple array of vertices.
			Vector<String> changed_properties;
			Array values;
			int count = generic_tile_polygon_editor->get_polygon_count();
			changed_properties.push_back(count_property);
			values.push_back(count);
			for (int i = 0; i < count; i++) {
				changed_properties.push_back(vformat(element_pattern, i));
				values.push_back(generic_tile_polygon_editor->get_polygon(i));
			}
			emit_signal("multiple_properties_changed", changed_properties, values, false);
		}
	}
}

void EditorPropertyTilePolygon::update_property() {
	LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject *atlas_tile_proxy_object = Object::cast_to<LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject>(get_edited_object());
	ERR_FAIL_NULL(atlas_tile_proxy_object);
	ERR_FAIL_COND(atlas_tile_proxy_object->get_edited_tiles().empty());

	Ref<LayeredTileSetAtlasSource> tile_set_atlas_source = atlas_tile_proxy_object->get_edited_tile_set_atlas_source();
	generic_tile_polygon_editor->set_tile_set(Ref<LayeredTileSet>(tile_set_atlas_source->get_tile_set()));

	// Set the background
	Vector2i coords = atlas_tile_proxy_object->get_edited_tiles().front()->get().tile;
	int alternative = atlas_tile_proxy_object->get_edited_tiles().front()->get().alternative;
	LayeredTileData *tile_data = tile_set_atlas_source->get_tile_data(coords, alternative);
	generic_tile_polygon_editor->set_background(tile_set_atlas_source->get_texture(), tile_set_atlas_source->get_tile_texture_region(coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());

	// Reset the polygons.
	generic_tile_polygon_editor->clear_polygons();

	if (String(count_property).empty()) {
		if (base_type == "OccluderPolygon2D") {
			// Single OccluderPolygon2D.
			Ref<OccluderPolygon2D> occluder = get_edited_object()->get(get_edited_property());
			generic_tile_polygon_editor->clear_polygons();
			if (occluder.is_valid()) {
				generic_tile_polygon_editor->add_polygon(Variant(occluder->get_polygon()));
			}
		} else if (base_type == "NavigationPolygon") {
			// Single OccluderPolygon2D.
			Ref<NavigationPolygon> navigation_polygon = get_edited_object()->get(get_edited_property());
			generic_tile_polygon_editor->clear_polygons();
			if (navigation_polygon.is_valid()) {
				for (int i = 0; i < navigation_polygon->get_outline_count(); i++) {
					generic_tile_polygon_editor->add_polygon(Variant(navigation_polygon->get_outline(i)));
				}
			}
		} else if (base_type == "Vector") {
			// Single PoolVector2Array
			Vector<Vector2> polygon = get_edited_object()->get(get_edited_property());
			generic_tile_polygon_editor->clear_polygons();

			if (polygon.size() > 0) {
				generic_tile_polygon_editor->add_polygon(polygon);
			}
		}
	} else {
		int count = get_edited_object()->get(count_property);
		if (base_type.empty()) {
			// Multiple array of vertices.
			generic_tile_polygon_editor->clear_polygons();
			for (int i = 0; i < count; i++) {
				generic_tile_polygon_editor->add_polygon(get_edited_object()->get(vformat(element_pattern, i)));
			}
		}
	}
}

void EditorPropertyTilePolygon::setup_single_mode(const StringName &p_property, const String &p_base_type) {
	set_object_and_property(nullptr, p_property);
	base_type = p_base_type;

	generic_tile_polygon_editor->set_multiple_polygon_mode(false);
}

void EditorPropertyTilePolygon::setup_multiple_mode(const StringName &p_property, const StringName &p_count_property, const String &p_element_pattern, const String &p_base_type) {
	set_object_and_property(nullptr, p_property);
	count_property = p_count_property;
	element_pattern = p_element_pattern;
	base_type = p_base_type;

	generic_tile_polygon_editor->set_multiple_polygon_mode(true);
}

EditorPropertyTilePolygon::EditorPropertyTilePolygon() {
	// Setup the polygon editor.
	generic_tile_polygon_editor = memnew(GenericTilePolygonEditor);
	generic_tile_polygon_editor->set_use_undo_redo(false);
	generic_tile_polygon_editor->clear_polygons();
	add_child(generic_tile_polygon_editor);
	generic_tile_polygon_editor->connect("polygons_changed", this, "_polygons_changed");

	// Add all focussable children of generic_tile_polygon_editor as focussable.
	_add_focusable_children(generic_tile_polygon_editor);
}

void EditorPropertyTilePolygon::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_polygons_changed"), &EditorPropertyTilePolygon::_polygons_changed);
}

////// EditorInspectorPluginTileData //////

bool EditorInspectorPluginTileData::can_handle(Object *p_object) {
	return Object::cast_to<LayeredTileSetAtlasSourceEditor::LayeredAtlasTileProxyObject>(p_object) != nullptr;
}

bool EditorInspectorPluginTileData::parse_property(Object *p_object, Variant::Type p_type, const String &p_path, PropertyHint p_hint, const String &p_hint_text, int p_usage) {
	Vector<String> components = String(p_path).split("/", true, 2);
	if (components.size() == 2 && components[0].begins_with("occlusion_layer_") && components[0].trim_prefix("occlusion_layer_").is_valid_integer()) {
		// Occlusion layers.
		int layer_index = components[0].trim_prefix("occlusion_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygon") {
			EditorPropertyTilePolygon *ep = memnew(EditorPropertyTilePolygon);
			ep->setup_single_mode(p_path, "OccluderPolygon2D");
			add_property_editor(p_path, ep);
			return true;
		}
	} else if (components.size() >= 2 && components[0].begins_with("physics_layer_") && components[0].trim_prefix("physics_layer_").is_valid_integer()) {
		// Physics layers.
		int layer_index = components[0].trim_prefix("physics_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygons_count") {
			EditorPropertyTilePolygon *ep = memnew(EditorPropertyTilePolygon);
			ep->setup_multiple_mode(vformat("physics_layer_%d/polygons", layer_index), vformat("physics_layer_%d/polygons_count", layer_index), vformat("physics_layer_%d/polygon_%%d/points", layer_index), "");
			Vector<String> properties;
			properties.push_back(p_path);
			int count = p_object->get(vformat("physics_layer_%d/polygons_count", layer_index));
			for (int i = 0; i < count; i++) {
				properties.push_back(vformat(vformat("physics_layer_%d/polygon_%d/points", layer_index, i)));
			}
			add_property_editor_for_multiple_properties("Polygons", properties, ep);
			return true;
		} else if (components.size() == 3 && components[1].begins_with("polygon_") && components[1].trim_prefix("polygon_").is_valid_integer()) {
			int polygon_index = components[1].trim_prefix("polygon_").to_int();
			ERR_FAIL_COND_V(polygon_index < 0, false);
			if (components[2] == "points") {
				return true;
			}
		}
	} else if (components.size() == 2 && components[0].begins_with("navigation_layer_") && components[0].trim_prefix("navigation_layer_").is_valid_integer()) {
		// Navigation layers.
		int layer_index = components[0].trim_prefix("navigation_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygon") {
			EditorPropertyTilePolygon *ep = memnew(EditorPropertyTilePolygon);
			ep->setup_single_mode(p_path, "NavigationPolygon");
			add_property_editor(p_path, ep);
			return true;
		}
	} else if (components.size() == 2 && components[0].begins_with("avoidance_layer_") && components[0].trim_prefix("avoidance_layer_").is_valid_integer()) {
		// Avoidance layers.
		int layer_index = components[0].trim_prefix("avoidance_layer_").to_int();
		ERR_FAIL_COND_V(layer_index < 0, false);
		if (components[1] == "polygon") {
			EditorPropertyTilePolygon *ep = memnew(EditorPropertyTilePolygon);
			ep->setup_single_mode(p_path, "Vector");
			add_property_editor(p_path, ep);
			return true;
		}
	}
	return false;
}

Control::CursorShape LayeredTileSetAtlasSourceEditor::LayeredTileAtlasControl::get_cursor_shape(const Point2 &p_pos) const {
	Control::CursorShape cursor_shape = get_default_cursor_shape();
	if (editor->drag_type == DRAG_TYPE_NONE) {
		if (editor->selection.size() == 1) {
			// Change the cursor depending on the hovered thing.
			TileSelection selected = editor->selection.front()->get();
			if (selected.tile != LayeredTileSetSource::INVALID_ATLAS_COORDS && selected.alternative == 0) {
				Transform2D xform = editor->tile_atlas_control->get_global_transform().affine_inverse() * get_global_transform();
				Vector2 mouse_local_pos = xform.xform(p_pos);
				Vector2i size_in_atlas = editor->tile_set_atlas_source->get_tile_size_in_atlas(selected.tile);
				Rect2 region = editor->tile_set_atlas_source->get_tile_texture_region(selected.tile);
				Size2 zoomed_size = editor->resize_handle->get_size() / editor->tile_atlas_view->get_zoom();
				Rect2 rect = region.grow_individual(zoomed_size.x, zoomed_size.y, 0, 0);
				const Vector2i coords[] = { Vector2i(0, 0), Vector2i(1, 0), Vector2i(1, 1), Vector2i(0, 1) };
				const Vector2i directions[] = { Vector2i(0, -1), Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0) };
				bool can_grow[4];
				for (int i = 0; i < 4; i++) {
					can_grow[i] = editor->tile_set_atlas_source->has_room_for_tile(selected.tile + directions[i], editor->tile_set_atlas_source->get_tile_size_in_atlas(selected.tile), editor->tile_set_atlas_source->get_tile_animation_columns(selected.tile), editor->tile_set_atlas_source->get_tile_animation_separation(selected.tile), editor->tile_set_atlas_source->get_tile_animation_frames_count(selected.tile), selected.tile);
					can_grow[i] |= (i % 2 == 0) ? size_in_atlas.y > 1 : size_in_atlas.x > 1;
				}
				for (int i = 0; i < 4; i++) {
					Vector2 pos = rect.position + rect.size * coords[i];
					if (can_grow[i] && can_grow[(i + 3) % 4] && Rect2(pos, zoomed_size).has_point(mouse_local_pos)) {
						cursor_shape = (i % 2) ? CURSOR_BDIAGSIZE : CURSOR_FDIAGSIZE;
					}
					Vector2 next_pos = rect.position + rect.size * coords[(i + 1) % 4];
					if (can_grow[i] && Rect2((pos + next_pos) / 2.0, zoomed_size).has_point(mouse_local_pos)) {
						cursor_shape = (i % 2) ? CURSOR_HSIZE : CURSOR_VSIZE;
					}
				}
			}
		}
	} else {
		switch (editor->drag_type) {
			case DRAG_TYPE_RESIZE_TOP_LEFT:
			case DRAG_TYPE_RESIZE_BOTTOM_RIGHT:
				cursor_shape = CURSOR_FDIAGSIZE;
				break;
			case DRAG_TYPE_RESIZE_TOP:
			case DRAG_TYPE_RESIZE_BOTTOM:
				cursor_shape = CURSOR_VSIZE;
				break;
			case DRAG_TYPE_RESIZE_TOP_RIGHT:
			case DRAG_TYPE_RESIZE_BOTTOM_LEFT:
				cursor_shape = CURSOR_BDIAGSIZE;
				break;
			case DRAG_TYPE_RESIZE_LEFT:
			case DRAG_TYPE_RESIZE_RIGHT:
				cursor_shape = CURSOR_HSIZE;
				break;
			case DRAG_TYPE_MOVE_TILE:
				cursor_shape = CURSOR_MOVE;
				break;
			default:
				break;
		}
	}
	return cursor_shape;
}
