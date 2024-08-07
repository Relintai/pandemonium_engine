/*************************************************************************/
/*  layered_tile_set_editor.cpp                                          */
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

#include "layered_tile_set_editor.h"

#include "core/core_string_names.h"

#include "layered_tile_data_editors.h"
#include "layered_tiles_editor_plugin.h"

#include "editor/editor_file_dialog.h"
#include "editor/editor_file_system.h"
#include "editor/editor_inspector.h"
#include "editor/editor_node.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

#include "core/object/undo_redo.h"

#include "scene/gui/box_container.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/tab_container.h"
#include "scene/main/control.h"

#include <limits.h>

LayeredTileSetEditor *LayeredTileSetEditor::singleton = nullptr;

void LayeredTileSetEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	ERR_FAIL_COND(!tile_set.is_valid());

	if (!can_drop_data_fw(p_point, p_data, p_from)) {
		return;
	}

	if (p_from == sources_list) {
		// Handle dropping a texture in the list of atlas resources.
		Dictionary d = p_data;
		Vector<String> files = d["files"];
		_load_texture_files(files);
	}
}

bool LayeredTileSetEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	ERR_FAIL_COND_V(!tile_set.is_valid(), false);

	if (read_only) {
		return false;
	}

	if (p_from == sources_list) {
		Dictionary d = p_data;

		if (!d.has("type")) {
			return false;
		}

		// Check if we have a Texture2D.
		if (String(d["type"]) == "files") {
			Vector<String> files = d["files"];

			if (files.size() == 0) {
				return false;
			}

			for (int i = 0; i < files.size(); i++) {
				String ftype = EditorFileSystem::get_singleton()->get_file_type(files[i]);

				if (!ClassDB::is_parent_class(ftype, "Texture")) {
					return false;
				}
			}

			return true;
		}
	}
	return false;
}

void LayeredTileSetEditor::_load_texture_files(const Vector<String> &p_paths) {
	int source_id = LayeredTileSet::INVALID_SOURCE;
	Vector<Ref<LayeredTileSetAtlasSource>> atlases;

	for (int i = 0; i < p_paths.size(); ++i) {
		const String &p_path = p_paths[i];

		Ref<Texture> texture = ResourceLoader::load(p_path);

		if (texture.is_null()) {
			EditorNode::get_singleton()->show_warning(TTR("Invalid texture selected."));
			continue;
		}

		// Retrieve the id for the next created source.
		source_id = tile_set->get_next_source_id();

		// Actually create the new source.
		Ref<LayeredTileSetAtlasSource> atlas_source = memnew(LayeredTileSetAtlasSource);
		atlas_source->set_texture(texture);
		atlas_source->set_texture_region_size(tile_set->get_tile_size());

		UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
		undo_redo->create_action(TTR("Add a new atlas source"));
		undo_redo->add_do_method(*tile_set, "add_source", atlas_source, source_id);
		undo_redo->add_undo_method(*tile_set, "remove_source", source_id);
		undo_redo->commit_action();

		atlases.push_back(atlas_source);
	}

	if (!atlases.empty()) {
		tile_set_atlas_source_editor->init_new_atlases(atlases);
	}

	// Update the selected source (thus triggering an update).
	_update_sources_list(source_id);
}

void LayeredTileSetEditor::_update_sources_list(int force_selected_id) {
	if (tile_set.is_null()) {
		return;
	}

	// Get the previously selected id.
	int old_selected = LayeredTileSet::INVALID_SOURCE;
	if (sources_list->get_current() >= 0) {
		int source_id = sources_list->get_item_metadata(sources_list->get_current());
		if (tile_set->has_source(source_id)) {
			old_selected = source_id;
		}
	}

	int to_select = LayeredTileSet::INVALID_SOURCE;
	if (force_selected_id >= 0) {
		to_select = force_selected_id;
	} else if (old_selected >= 0) {
		to_select = old_selected;
	}

	// Clear the list.
	sources_list->clear();

	// Update the atlas sources.
	List<int> source_ids = LayeredTilesEditorUtils::get_singleton()->get_sorted_sources(tile_set);

	for (const List<int>::Element *E = source_ids.front(); E; E = E->next()) {
		const int source_id = E->get();

		LayeredTileSetSource *source = *tile_set->get_source(source_id);

		Ref<Texture> texture;
		String item_text;

		// Common to all type of sources.
		if (!source->get_name().empty()) {
			item_text = source->get_name();
		}

		// Atlas source.
		LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
		if (atlas_source) {
			texture = atlas_source->get_texture();
			if (item_text.empty()) {
				if (texture.is_valid()) {
					item_text = texture->get_path().get_file();
				} else {
					item_text = vformat(TTR("No Texture Atlas Source (ID: %d)"), source_id);
				}
			}
		}

		// Scene collection source.
		LayeredTileSetScenesCollectionSource *scene_collection_source = Object::cast_to<LayeredTileSetScenesCollectionSource>(source);
		if (scene_collection_source) {
			texture = get_theme_icon("PackedScene", "EditorIcons");
			if (item_text.empty()) {
				if (scene_collection_source->get_scene_tiles_count() > 0) {
					item_text = vformat(TTR("Scene Collection Source (ID: %d)"), source_id);
				} else {
					item_text = vformat(TTR("Empty Scene Collection Source (ID: %d)"), source_id);
				}
			}
		}

		// Use default if not valid.
		if (item_text.empty()) {
			item_text = vformat(TTR("Unknown Type Source (ID: %d)"), source_id);
		}
		if (!texture.is_valid()) {
			texture = missing_texture_texture;
		}

		sources_list->add_item(item_text, texture);
		sources_list->set_item_metadata(sources_list->get_item_count() - 1, source_id);
	}

	// Set again the current selected item if needed.
	if (to_select >= 0) {
		for (int i = 0; i < sources_list->get_item_count(); i++) {
			if ((int)sources_list->get_item_metadata(i) == to_select) {
				sources_list->set_current(i);
				sources_list->ensure_current_is_visible();
				if (old_selected != to_select) {
					sources_list->emit_signal("item_selected", sources_list->get_current());
				}
				break;
			}
		}
	}

	// If nothing is selected, select the first entry.
	if (sources_list->get_current() < 0 && sources_list->get_item_count() > 0) {
		sources_list->set_current(0);
		if (old_selected != int(sources_list->get_item_metadata(0))) {
			sources_list->emit_signal("item_selected", sources_list->get_current());
		}
	}

	// If there is no source left, hide all editors and show the label.
	_source_selected(sources_list->get_current());

	// Synchronize the lists.
	LayeredTilesEditorUtils::get_singleton()->set_sources_lists_current(sources_list->get_current());
}

void LayeredTileSetEditor::_source_selected(int p_source_index) {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Update the selected source.
	sources_delete_button->set_disabled(p_source_index < 0 || read_only);

	if (p_source_index >= 0) {
		int source_id = sources_list->get_item_metadata(p_source_index);
		LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(*tile_set->get_source(source_id));
		LayeredTileSetScenesCollectionSource *scenes_collection_source = Object::cast_to<LayeredTileSetScenesCollectionSource>(*tile_set->get_source(source_id));
		if (atlas_source) {
			no_source_selected_label->hide();
			tile_set_atlas_source_editor->edit(*tile_set, atlas_source, source_id);
			tile_set_atlas_source_editor->show();
			tile_set_scenes_collection_source_editor->hide();
		} else if (scenes_collection_source) {
			no_source_selected_label->hide();
			tile_set_atlas_source_editor->hide();
			tile_set_scenes_collection_source_editor->edit(*tile_set, scenes_collection_source, source_id);
			tile_set_scenes_collection_source_editor->show();
		} else {
			no_source_selected_label->show();
			tile_set_atlas_source_editor->hide();
			tile_set_scenes_collection_source_editor->hide();
		}
	} else {
		no_source_selected_label->show();
		tile_set_atlas_source_editor->hide();
		tile_set_scenes_collection_source_editor->hide();
	}
}

void LayeredTileSetEditor::_source_delete_pressed() {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Update the selected source.
	int to_delete = sources_list->get_item_metadata(sources_list->get_current());

	Ref<LayeredTileSetSource> source = tile_set->get_source(to_delete);

	// Remove the source.
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action(TTR("Remove source"));
	undo_redo->add_do_method(*tile_set, "remove_source", to_delete);
	undo_redo->add_undo_method(*tile_set, "add_source", source, to_delete);
	undo_redo->commit_action();

	_update_sources_list();
}

void LayeredTileSetEditor::_source_add_id_pressed(int p_id_pressed) {
	ERR_FAIL_COND(!tile_set.is_valid());

	switch (p_id_pressed) {
		case 0: {
			if (!texture_file_dialog) {
				texture_file_dialog = memnew(EditorFileDialog);
				add_child(texture_file_dialog);
				texture_file_dialog->set_mode(EditorFileDialog::MODE_OPEN_FILES);
				texture_file_dialog->connect("files_selected", this, "_load_texture_files");

				List<String> extensions;
				ResourceLoader::get_recognized_extensions_for_type("Texture", &extensions);

				for (const List<String>::Element *E = extensions.front(); E; E = E->next()) {
					texture_file_dialog->add_filter("*." + E->get() + ";" + E->get().to_upper());
				}
			}
			texture_file_dialog->popup_centered_ratio();
		} break;
		case 1: {
			int source_id = tile_set->get_next_source_id();

			Ref<LayeredTileSetScenesCollectionSource> scene_collection_source = memnew(LayeredTileSetScenesCollectionSource);

			// Add a new source.
			UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
			undo_redo->create_action(TTR("Add atlas source"));
			undo_redo->add_do_method(*tile_set, "add_source", scene_collection_source, source_id);
			undo_redo->add_undo_method(*tile_set, "remove_source", source_id);
			undo_redo->commit_action();

			_update_sources_list(source_id);
		} break;
		default:
			ERR_FAIL();
	}
}

void LayeredTileSetEditor::_sources_advanced_menu_id_pressed(int p_id_pressed) {
	ERR_FAIL_COND(!tile_set.is_valid());

	switch (p_id_pressed) {
		case 0: {
			atlas_merging_dialog->update_tile_set(tile_set);
			atlas_merging_dialog->popup_centered_ratio(0.5);
		} break;
		case 1: {
			tile_proxies_manager_dialog->update_tile_set(tile_set);
			tile_proxies_manager_dialog->popup_centered_ratio(0.5);
		} break;
	}
}

void LayeredTileSetEditor::_set_source_sort(int p_sort) {
	LayeredTilesEditorUtils::get_singleton()->set_sorting_option(p_sort);
	for (int i = 0; i != LayeredTilesEditorUtils::SOURCE_SORT_MAX; i++) {
		source_sort_button->get_popup()->set_item_checked(i, (i == (int)p_sort));
	}

	int old_selected = LayeredTileSet::INVALID_SOURCE;
	if (sources_list->get_current() >= 0) {
		int source_id = sources_list->get_item_metadata(sources_list->get_current());
		if (tile_set->has_source(source_id)) {
			old_selected = source_id;
		}
	}
	_update_sources_list(old_selected);
	EditorSettings::get_singleton()->set_project_metadata("editor_metadata", "tile_source_sort", p_sort);
}

void LayeredTileSetEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_load_texture_files"), &LayeredTileSetEditor::_load_texture_files);
	ClassDB::bind_method(D_METHOD("_tile_set_changed"), &LayeredTileSetEditor::_tile_set_changed);
	ClassDB::bind_method(D_METHOD("_pattern_preview_done"), &LayeredTileSetEditor::_pattern_preview_done);
	ClassDB::bind_method(D_METHOD("_tab_changed"), &LayeredTileSetEditor::_tab_changed);

	ClassDB::bind_method(D_METHOD("drop_data_fw"), &LayeredTileSetEditor::drop_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &LayeredTileSetEditor::can_drop_data_fw);

	ClassDB::bind_method(D_METHOD("_set_source_sort"), &LayeredTileSetEditor::_set_source_sort);
	ClassDB::bind_method(D_METHOD("_source_selected"), &LayeredTileSetEditor::_source_selected);

	ClassDB::bind_method(D_METHOD("_source_delete_pressed"), &LayeredTileSetEditor::_source_delete_pressed);
	ClassDB::bind_method(D_METHOD("_source_add_id_pressed"), &LayeredTileSetEditor::_source_add_id_pressed);
	ClassDB::bind_method(D_METHOD("_sources_advanced_menu_id_pressed"), &LayeredTileSetEditor::_sources_advanced_menu_id_pressed);
	ClassDB::bind_method(D_METHOD("_update_sources_list"), &LayeredTileSetEditor::_update_sources_list);
	ClassDB::bind_method(D_METHOD("_patterns_item_list_gui_input"), &LayeredTileSetEditor::_patterns_item_list_gui_input);
	ClassDB::bind_method(D_METHOD("_move_tile_set_array_element"), &LayeredTileSetEditor::_move_tile_set_array_element);
}

void LayeredTileSetEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			sources_delete_button->set_icon(get_theme_icon("Remove", "EditorIcons"));
			sources_add_button->set_icon(get_theme_icon("Add", "EditorIcons"));
			source_sort_button->set_icon(get_theme_icon("Sort", "EditorIcons"));
			sources_advanced_menu_button->set_icon(get_theme_icon("GuiTabMenuHl", "EditorIcons"));
			missing_texture_texture = get_theme_icon("LayeredTileSet", "EditorIcons");
			expanded_area->add_theme_style_override("panel", get_theme_stylebox("panel", "Tree"));
			_update_sources_list();
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (tile_set_changed_needs_update) {
				if (tile_set.is_valid()) {
					tile_set->set_edited(true);
				}

				read_only = false;
				//if (tile_set.is_valid()) {
				//	read_only = EditorNode::get_singleton()->is_resource_read_only(tile_set);
				//}

				_update_sources_list();
				_update_patterns_list();

				sources_add_button->set_disabled(read_only);
				sources_advanced_menu_button->set_disabled(read_only);
				source_sort_button->set_disabled(read_only);

				tile_set_changed_needs_update = false;
			}
		} break;

		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (!is_visible_in_tree()) {
				remove_expanded_editor();
			}
		} break;
	}
}

void LayeredTileSetEditor::_patterns_item_list_gui_input(const Ref<InputEvent> &p_event) {
	ERR_FAIL_COND(!tile_set.is_valid());

	if (ED_IS_SHORTCUT("layered_tiles_editor/delete", p_event) && p_event->is_pressed() && !p_event->is_echo()) {
		Vector<int> selected = patterns_item_list->get_selected_items();
		UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
		undo_redo->create_action(TTR("Remove LayeredTileSet patterns"));
		for (int i = 0; i < selected.size(); i++) {
			int pattern_index = selected[i];
			undo_redo->add_do_method(*tile_set, "remove_pattern", pattern_index);
			undo_redo->add_undo_method(*tile_set, "add_pattern", tile_set->get_pattern(pattern_index), pattern_index);
		}
		undo_redo->commit_action();
		patterns_item_list->accept_event();
	}
}

void LayeredTileSetEditor::_pattern_preview_done(Ref<LayeredTileMapPattern> p_pattern, Ref<Texture> p_texture) {
	// TODO optimize ?
	for (int i = 0; i < patterns_item_list->get_item_count(); i++) {
		if (patterns_item_list->get_item_metadata(i) == p_pattern) {
			patterns_item_list->set_item_icon(i, p_texture);
			break;
		}
	}
}

void LayeredTileSetEditor::_update_patterns_list() {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Recreate the items.
	patterns_item_list->clear();
	for (int i = 0; i < tile_set->get_patterns_count(); i++) {
		patterns_item_list->add_item("");
		int id = patterns_item_list->get_item_count() - 1;
		patterns_item_list->set_item_metadata(id, tile_set->get_pattern(i));
		patterns_item_list->set_item_tooltip(id, vformat(TTR("Index: %d"), i));

		Ref<FuncRef> fref;
		fref.instance();
		fref->set_instance(this);
		fref->set_function("_pattern_preview_done");

		LayeredTilesEditorUtils::get_singleton()->queue_pattern_preview(tile_set, tile_set->get_pattern(i), fref);
	}

	// Update the label visibility.
	patterns_help_label->set_visible(patterns_item_list->get_item_count() == 0);
}

void LayeredTileSetEditor::_tile_set_changed() {
	tile_set_changed_needs_update = true;
}

void LayeredTileSetEditor::_tab_changed(int p_tab_changed) {
	split_container->set_visible(p_tab_changed == 0);
	patterns_item_list->set_visible(p_tab_changed == 1);
}

void LayeredTileSetEditor::_move_tile_set_array_element(Object *p_undo_redo, Object *p_edited, const String &p_array_prefix, int p_from_index, int p_to_pos) {
	UndoRedo *undo_redo = Object::cast_to<UndoRedo>(p_undo_redo);
	ERR_FAIL_NULL(undo_redo);

	LayeredTileSet *ed_tile_set = Object::cast_to<LayeredTileSet>(p_edited);
	if (!ed_tile_set) {
		return;
	}

	Vector<String> components = String(p_array_prefix).split("/", true, 2);

	// Compute the array indices to save.
	int begin = 0;
	int end;
	if (p_array_prefix == "occlusion_layer_") {
		end = ed_tile_set->get_occlusion_layers_count();
	} else if (p_array_prefix == "physics_layer_") {
		end = ed_tile_set->get_physics_layers_count();
	} else if (p_array_prefix == "terrain_set_") {
		end = ed_tile_set->get_terrain_sets_count();
	} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer() && components[1] == "terrain_") {
		int terrain_set = components[0].trim_prefix("terrain_set_").to_int();
		end = ed_tile_set->get_terrains_count(terrain_set);
	} else if (p_array_prefix == "navigation_layer_") {
		end = ed_tile_set->get_navigation_layers_count();
	} else if (p_array_prefix == "avoidance_layer_") {
		end = ed_tile_set->get_avoidance_layers_count();
	} else if (p_array_prefix == "custom_data_layer_") {
		end = ed_tile_set->get_custom_data_layers_count();
	} else {
		ERR_FAIL_MSG("Invalid array prefix for LayeredTileSet.");
	}
	if (p_from_index < 0) {
		// Adding new.
		if (p_to_pos >= 0) {
			begin = p_to_pos;
		} else {
			end = 0; // Nothing to save when adding at the end.
		}
	} else if (p_to_pos < 0) {
		// Removing.
		begin = p_from_index;
	} else {
		// Moving.
		begin = MIN(p_from_index, p_to_pos);
		end = MIN(MAX(p_from_index, p_to_pos) + 1, end);
	}

#define ADD_UNDO(obj, property) undo_redo->add_undo_property(obj, property, obj->get(property));

	// Add undo method to adding array element.
	if (p_array_prefix == "occlusion_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_occlusion_layer", p_to_pos < 0 ? ed_tile_set->get_occlusion_layers_count() : p_to_pos);
		}
	} else if (p_array_prefix == "physics_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_physics_layer", p_to_pos < 0 ? ed_tile_set->get_physics_layers_count() : p_to_pos);
		}
	} else if (p_array_prefix == "terrain_set_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_terrain_set", p_to_pos < 0 ? ed_tile_set->get_terrain_sets_count() : p_to_pos);
		}
	} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer() && components[1] == "terrain_") {
		int terrain_set = components[0].trim_prefix("terrain_set_").to_int();
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_terrain", terrain_set, p_to_pos < 0 ? ed_tile_set->get_terrains_count(terrain_set) : p_to_pos);
		}
	} else if (p_array_prefix == "navigation_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_navigation_layer", p_to_pos < 0 ? ed_tile_set->get_navigation_layers_count() : p_to_pos);
		}
	} else if (p_array_prefix == "avoidance_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_avoidance_layer", p_to_pos < 0 ? ed_tile_set->get_avoidance_layers_count() : p_to_pos);
		}
	} else if (p_array_prefix == "custom_data_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_undo_method(ed_tile_set, "remove_custom_data_layer", p_to_pos < 0 ? ed_tile_set->get_custom_data_layers_count() : p_to_pos);
		}
	}

	// Save layers' properties.
	List<PropertyInfo> properties;
	ed_tile_set->get_property_list(&properties);

	for (List<PropertyInfo>::Element *E = properties.front(); E; E = E->next()) {
		PropertyInfo pi = E->get();

		if (pi.name.begins_with(p_array_prefix)) {
			String str = pi.name.trim_prefix(p_array_prefix);
			int to_char_index = 0;
			while (to_char_index < str.length()) {
				if (!is_digit(str[to_char_index])) {
					break;
				}
				to_char_index++;
			}
			if (to_char_index > 0) {
				int array_index = str.left(to_char_index).to_int();
				if (array_index >= begin && array_index < end) {
					ADD_UNDO(ed_tile_set, pi.name);
				}
			}
		}
	}

	// Save properties for LayeredTileSetAtlasSources tile data
	for (int i = 0; i < ed_tile_set->get_source_count(); i++) {
		int source_id = ed_tile_set->get_source_id(i);

		Ref<LayeredTileSetAtlasSource> tas = ed_tile_set->get_source(source_id);
		if (tas.is_valid()) {
			for (int j = 0; j < tas->get_tiles_count(); j++) {
				Vector2i tile_id = tas->get_tile_id(j);
				for (int k = 0; k < tas->get_alternative_tiles_count(tile_id); k++) {
					int alternative_id = tas->get_alternative_tile_id(tile_id, k);
					LayeredTileData *tile_data = tas->get_tile_data(tile_id, alternative_id);
					ERR_FAIL_NULL(tile_data);

					// Actually saving stuff.
					if (p_array_prefix == "occlusion_layer_") {
						for (int layer_index = begin; layer_index < end; layer_index++) {
							ADD_UNDO(tile_data, vformat("occlusion_layer_%d/polygon", layer_index));
						}
					} else if (p_array_prefix == "physics_layer_") {
						for (int layer_index = begin; layer_index < end; layer_index++) {
							ADD_UNDO(tile_data, vformat("physics_layer_%d/polygons_count", layer_index));
							for (int polygon_index = 0; polygon_index < tile_data->get_collision_polygons_count(layer_index); polygon_index++) {
								ADD_UNDO(tile_data, vformat("physics_layer_%d/polygon_%d/points", layer_index, polygon_index));
								ADD_UNDO(tile_data, vformat("physics_layer_%d/polygon_%d/one_way", layer_index, polygon_index));
								ADD_UNDO(tile_data, vformat("physics_layer_%d/polygon_%d/one_way_margin", layer_index, polygon_index));
							}
						}
					} else if (p_array_prefix == "terrain_set_") {
						ADD_UNDO(tile_data, "terrain_set");
						for (int terrain_set_index = begin; terrain_set_index < end; terrain_set_index++) {
							for (int l = 0; l < LayeredTileSet::CELL_NEIGHBOR_MAX; l++) {
								LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(l);
								if (tile_data->is_valid_terrain_peering_bit(bit)) {
									ADD_UNDO(tile_data, "terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[l]));
								}
							}
						}
					} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer() && components[1] == "terrain_") {
						for (int terrain_index = 0; terrain_index < LayeredTileSet::CELL_NEIGHBOR_MAX; terrain_index++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(terrain_index);
							if (tile_data->is_valid_terrain_peering_bit(bit)) {
								ADD_UNDO(tile_data, "terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[terrain_index]));
							}
						}
					} else if (p_array_prefix == "navigation_layer_") {
						for (int layer_index = begin; layer_index < end; layer_index++) {
							ADD_UNDO(tile_data, vformat("navigation_layer_%d/polygon", layer_index));
						}
					} else if (p_array_prefix == "avoidance_layer_") {
						for (int layer_index = begin; layer_index < end; layer_index++) {
							ADD_UNDO(tile_data, vformat("avoidance_layer_%d/radius", layer_index));
							ADD_UNDO(tile_data, vformat("avoidance_layer_%d/polygon", layer_index));
						}
					} else if (p_array_prefix == "custom_data_layer_") {
						for (int layer_index = begin; layer_index < end; layer_index++) {
							ADD_UNDO(tile_data, vformat("custom_data_%d", layer_index));
						}
					}
				}
			}
		}
	}
#undef ADD_UNDO

	// Add do method to add/remove array element.
	if (p_array_prefix == "occlusion_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_occlusion_layer", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_occlusion_layer", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_occlusion_layer", p_from_index, p_to_pos);
		}
	} else if (p_array_prefix == "physics_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_physics_layer", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_physics_layer", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_physics_layer", p_from_index, p_to_pos);
		}
	} else if (p_array_prefix == "terrain_set_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_terrain_set", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_terrain_set", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_terrain_set", p_from_index, p_to_pos);
		}
	} else if (components.size() >= 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer() && components[1] == "terrain_") {
		int terrain_set = components[0].trim_prefix("terrain_set_").to_int();
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_terrain", terrain_set, p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_terrain", terrain_set, p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_terrain", terrain_set, p_from_index, p_to_pos);
		}
	} else if (p_array_prefix == "navigation_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_navigation_layer", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_navigation_layer", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_navigation_layer", p_from_index, p_to_pos);
		}
	} else if (p_array_prefix == "avoidance_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_avoidance_layer", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_avoidance_layer", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_avoidance_layer", p_from_index, p_to_pos);
		}
	} else if (p_array_prefix == "custom_data_layer_") {
		if (p_from_index < 0) {
			undo_redo->add_do_method(ed_tile_set, "add_custom_data_layer", p_to_pos);
		} else if (p_to_pos < 0) {
			undo_redo->add_do_method(ed_tile_set, "remove_custom_data_layer", p_from_index);
		} else {
			undo_redo->add_do_method(ed_tile_set, "move_custom_data_layer", p_from_index, p_to_pos);
		}
	}
}

void LayeredTileSetEditor::_undo_redo_inspector_callback(Object *p_undo_redo, Object *p_edited, const String &p_property, const Variant &p_new_value) {
	UndoRedo *undo_redo = Object::cast_to<UndoRedo>(p_undo_redo);
	ERR_FAIL_NULL(undo_redo);

#define ADD_UNDO(obj, property) undo_redo->add_undo_property(obj, property, obj->get(property));
	LayeredTileSet *ed_tile_set = Object::cast_to<LayeredTileSet>(p_edited);
	if (ed_tile_set) {
		Vector<String> components = p_property.split("/", true, 3);
		for (int i = 0; i < ed_tile_set->get_source_count(); i++) {
			int source_id = ed_tile_set->get_source_id(i);

			Ref<LayeredTileSetAtlasSource> tas = ed_tile_set->get_source(source_id);
			if (tas.is_valid()) {
				for (int j = 0; j < tas->get_tiles_count(); j++) {
					Vector2i tile_id = tas->get_tile_id(j);
					for (int k = 0; k < tas->get_alternative_tiles_count(tile_id); k++) {
						int alternative_id = tas->get_alternative_tile_id(tile_id, k);
						LayeredTileData *tile_data = tas->get_tile_data(tile_id, alternative_id);
						ERR_FAIL_NULL(tile_data);

						if (components.size() == 2 && components[0].begins_with("terrain_set_") && components[0].trim_prefix("terrain_set_").is_valid_integer() && components[1] == "mode") {
							ADD_UNDO(tile_data, "terrain_set");
							ADD_UNDO(tile_data, "terrain");
							for (int l = 0; l < LayeredTileSet::CELL_NEIGHBOR_MAX; l++) {
								LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(l);
								if (tile_data->is_valid_terrain_peering_bit(bit)) {
									ADD_UNDO(tile_data, "terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[l]));
								}
							}
						} else if (components.size() == 2 && components[0].begins_with("custom_data_layer_") && components[0].trim_prefix("custom_data_layer_").is_valid_integer() && components[1] == "type") {
							int custom_data_layer = components[0].trim_prefix("custom_data_layer_").is_valid_integer();
							ADD_UNDO(tile_data, vformat("custom_data_%d", custom_data_layer));
						}
					}
				}
			}
		}
	}
#undef ADD_UNDO
}

void LayeredTileSetEditor::edit(Ref<LayeredTileSet> p_tile_set) {
	if (p_tile_set == tile_set) {
		return;
	}

	// Remove listener.
	if (tile_set.is_valid()) {
		tile_set->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
	}

	// Change the edited object.
	tile_set = p_tile_set;

	// Add the listener again and check for read-only status.
	if (tile_set.is_valid()) {
		sources_add_button->set_disabled(read_only);
		sources_advanced_menu_button->set_disabled(read_only);
		source_sort_button->set_disabled(read_only);

		tile_set->connect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed");
		if (first_edit) {
			first_edit = false;
			_set_source_sort(EditorSettings::get_singleton()->get_project_metadata("editor_metadata", "tile_source_sort", 0));
		} else {
			_update_sources_list();
		}
		_update_patterns_list();
	}
}

void LayeredTileSetEditor::add_expanded_editor(Control *p_editor) {
	expanded_editor = p_editor;
	expanded_editor_parent = p_editor->get_parent()->get_instance_id();

	// Find the scrollable control this node belongs to.
	Node *check_parent = expanded_editor->get_parent();
	Control *parent_container = nullptr;
	while (check_parent) {
		parent_container = Object::cast_to<EditorInspector>(check_parent);
		if (parent_container) {
			break;
		}
		parent_container = Object::cast_to<ScrollContainer>(check_parent);
		if (parent_container) {
			break;
		}
		check_parent = check_parent->get_parent();
	}
	ERR_FAIL_NULL(parent_container);

	expanded_editor->set_meta("reparented", true);
	expanded_editor->get_parent()->remove_child(expanded_editor);
	expanded_area->add_child(expanded_editor);
	expanded_area->show();
	expanded_area->set_size(Vector2(parent_container->get_global_rect().get_end().x - expanded_area->get_global_position().x, expanded_area->get_size().y));

	for (uint32_t i = 0; i < disable_on_expand.size(); ++i) {
		SplitContainer *split = disable_on_expand[i];

		split->set_dragger_visibility(SplitContainer::DRAGGER_HIDDEN);
	}
}

void LayeredTileSetEditor::remove_expanded_editor() {
	if (!expanded_editor) {
		return;
	}

	Node *original_parent = Object::cast_to<Node>(ObjectDB::get_instance(expanded_editor_parent));
	if (original_parent) {
		expanded_editor->remove_meta("reparented");
		expanded_editor->get_parent()->remove_child(expanded_editor);
		original_parent->add_child(expanded_editor);
	} else {
		expanded_editor->queue_delete();
	}
	expanded_editor = nullptr;
	expanded_editor_parent = ObjectID();
	expanded_area->hide();

	for (uint32_t i = 0; i < disable_on_expand.size(); ++i) {
		SplitContainer *split = disable_on_expand[i];

		split->set_dragger_visibility(SplitContainer::DRAGGER_VISIBLE);
	}
}

void LayeredTileSetEditor::register_split(SplitContainer *p_split) {
	disable_on_expand.push_back(p_split);
}

LayeredTileSetEditor::LayeredTileSetEditor() {
	singleton = this;

	set_process_internal(true);

	VBoxContainer *main_vb = memnew(VBoxContainer);
	add_child(main_vb);
	main_vb->set_anchors_and_margins_preset(PRESET_WIDE);

	// Tabs.
	tabs_bar = memnew(Tabs);
	tabs_bar->set_clip_tabs(false);
	tabs_bar->add_tab(TTR("Tiles"));
	tabs_bar->add_tab(TTR("Patterns"));
	tabs_bar->connect("tab_changed", this, "_tab_changed");

	tile_set_toolbar = memnew(HBoxContainer);
	tile_set_toolbar->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_set_toolbar->add_child(tabs_bar);
	main_vb->add_child(tile_set_toolbar);

	//// Tiles ////
	// Split container.
	split_container = memnew(HSplitContainer);
	split_container->set_name(TTR("Tiles"));
	split_container->set_h_size_flags(SIZE_EXPAND_FILL);
	split_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_vb->add_child(split_container);

	// Sources list.
	VBoxContainer *split_container_left_side = memnew(VBoxContainer);
	split_container_left_side->set_h_size_flags(SIZE_EXPAND_FILL);
	split_container_left_side->set_v_size_flags(SIZE_EXPAND_FILL);
	split_container_left_side->set_stretch_ratio(0.25);
	split_container_left_side->set_custom_minimum_size(Size2(70, 0) * EDSCALE);
	split_container->add_child(split_container_left_side);

	source_sort_button = memnew(MenuButton);
	source_sort_button->set_flat(false);
	source_sort_button->set_theme_type_variation("FlatButton");
	source_sort_button->set_tooltip(TTR("Sort Sources"));

	PopupMenu *p = source_sort_button->get_popup();
	p->connect("id_pressed", this, "_set_source_sort");
	p->add_radio_check_item(TTR("Sort by ID (Ascending)"), LayeredTilesEditorUtils::SOURCE_SORT_ID);
	p->add_radio_check_item(TTR("Sort by ID (Descending)"), LayeredTilesEditorUtils::SOURCE_SORT_ID_REVERSE);
	p->add_radio_check_item(TTR("Sort by Name (Ascending)"), LayeredTilesEditorUtils::SOURCE_SORT_NAME);
	p->add_radio_check_item(TTR("Sort by Name (Descending)"), LayeredTilesEditorUtils::SOURCE_SORT_NAME_REVERSE);
	p->set_item_checked(LayeredTilesEditorUtils::SOURCE_SORT_ID, true);

	sources_list = memnew(ItemList);
	sources_list->set_fixed_icon_size(Size2(60, 60) * EDSCALE);
	sources_list->set_h_size_flags(SIZE_EXPAND_FILL);
	sources_list->set_v_size_flags(SIZE_EXPAND_FILL);
	sources_list->connect("item_selected", this, "_source_selected");
	sources_list->connect("item_selected", LayeredTilesEditorUtils::get_singleton(), "set_sources_lists_current");
	sources_list->connect("visibility_changed", LayeredTilesEditorUtils::get_singleton(), "synchronize_sources_list", varray(sources_list, source_sort_button));
	sources_list->add_user_signal(MethodInfo("sort_request"));
	sources_list->connect("sort_request", this, "_update_sources_list", varray(-1));

	sources_list->set_drag_forwarding(this);

	split_container_left_side->add_child(sources_list);

	HBoxContainer *sources_bottom_actions = memnew(HBoxContainer);
	sources_bottom_actions->set_alignment(BoxContainer::ALIGN_END);
	split_container_left_side->add_child(sources_bottom_actions);

	sources_delete_button = memnew(Button);
	sources_delete_button->set_theme_type_variation("FlatButton");
	sources_delete_button->set_disabled(true);
	sources_delete_button->connect("pressed", this, "_source_delete_pressed");
	sources_bottom_actions->add_child(sources_delete_button);

	sources_add_button = memnew(MenuButton);
	sources_add_button->set_flat(false);
	sources_add_button->set_theme_type_variation("FlatButton");
	sources_add_button->get_popup()->add_item(TTR("Atlas"));
	sources_add_button->get_popup()->add_item(TTR("Scenes Collection"));
	sources_add_button->get_popup()->connect("id_pressed", this, "_source_add_id_pressed");
	sources_bottom_actions->add_child(sources_add_button);

	sources_advanced_menu_button = memnew(MenuButton);
	sources_advanced_menu_button->set_flat(false);
	sources_advanced_menu_button->set_theme_type_variation("FlatButton");
	sources_advanced_menu_button->get_popup()->add_item(TTR("Open Atlas Merging Tool"));
	sources_advanced_menu_button->get_popup()->add_item(TTR("Manage Tile Proxies"));
	sources_advanced_menu_button->get_popup()->connect("id_pressed", this, "_sources_advanced_menu_id_pressed");
	sources_bottom_actions->add_child(sources_advanced_menu_button);
	sources_bottom_actions->add_child(source_sort_button);

	atlas_merging_dialog = memnew(AtlasMergingDialog);
	add_child(atlas_merging_dialog);

	tile_proxies_manager_dialog = memnew(LayeredTileProxiesManagerDialog);
	add_child(tile_proxies_manager_dialog);

	// Right side container.
	VBoxContainer *split_container_right_side = memnew(VBoxContainer);
	split_container_right_side->set_h_size_flags(SIZE_EXPAND_FILL);
	split_container_right_side->set_v_size_flags(SIZE_EXPAND_FILL);
	split_container->add_child(split_container_right_side);

	// No source selected.
	no_source_selected_label = memnew(Label);
	no_source_selected_label->set_text(TTR("No LayeredTileSet source selected. Select or create a LayeredTileSet source.\nYou can create a new source by using the Add button on the left or by dropping a tileset texture onto the source list."));
	no_source_selected_label->set_h_size_flags(SIZE_EXPAND_FILL);
	no_source_selected_label->set_v_size_flags(SIZE_EXPAND_FILL);
	no_source_selected_label->set_align(Label::ALIGN_CENTER);
	no_source_selected_label->set_valign(Label::VALIGN_CENTER);
	split_container_right_side->add_child(no_source_selected_label);

	// Atlases editor.
	tile_set_atlas_source_editor = memnew(LayeredTileSetAtlasSourceEditor);
	tile_set_atlas_source_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_set_atlas_source_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	tile_set_atlas_source_editor->connect("source_id_changed", this, "_update_sources_list");
	split_container_right_side->add_child(tile_set_atlas_source_editor);
	tile_set_atlas_source_editor->hide();

	// Scenes collection editor.
	tile_set_scenes_collection_source_editor = memnew(LayeredTileSetScenesCollectionSourceEditor);
	tile_set_scenes_collection_source_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	tile_set_scenes_collection_source_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	tile_set_scenes_collection_source_editor->connect("source_id_changed", this, "_update_sources_list");
	split_container_right_side->add_child(tile_set_scenes_collection_source_editor);
	tile_set_scenes_collection_source_editor->hide();

	//// Patterns ////
	int thumbnail_size = 64;
	patterns_item_list = memnew(ItemList);
	patterns_item_list->set_max_columns(0);
	patterns_item_list->set_icon_mode(ItemList::ICON_MODE_TOP);
	patterns_item_list->set_fixed_column_width(thumbnail_size * 3 / 2);
	patterns_item_list->set_max_text_lines(2);
	patterns_item_list->set_fixed_icon_size(Size2(thumbnail_size, thumbnail_size));
	patterns_item_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	patterns_item_list->connect("gui_input", this, "_patterns_item_list_gui_input");
	main_vb->add_child(patterns_item_list);
	patterns_item_list->hide();

	patterns_help_label = memnew(Label);
	patterns_help_label->set_text(TTR("Add new patterns in the LayeredTileMap editing mode."));
	patterns_help_label->set_align(Label::ALIGN_CENTER);
	patterns_help_label->set_anchors_and_margins_preset(Control::PRESET_CENTER);
	patterns_item_list->add_child(patterns_help_label);

	// Expanded editor
	expanded_area = memnew(PanelContainer);
	add_child(expanded_area);
	expanded_area->set_anchors_and_margins_preset(PRESET_LEFT_WIDE);
	expanded_area->hide();

	// Registers UndoRedo inspector callback.

	Ref<FuncRef> fref;
	fref.instance();
	fref->set_instance(this);
	fref->set_function("_move_tile_set_array_element");

	EditorNode::get_editor_data().add_move_array_element_function("LayeredTileSet", fref);
	//EditorNode::get_editor_data().add_undo_redo_inspector_hook_callback(callable_mp(this, &LayeredTileSetEditor::_undo_redo_inspector_callback));
}

void LayeredTileSourceInspectorPlugin::_show_id_edit_dialog(Object *p_for_source) {
	if (!id_edit_dialog) {
		id_edit_dialog = memnew(ConfirmationDialog);
		LayeredTileSetEditor::get_singleton()->add_child(id_edit_dialog);

		VBoxContainer *vbox = memnew(VBoxContainer);
		id_edit_dialog->add_child(vbox);

		Label *label = memnew(Label(TTR("Warning: Modifying a source ID will result in all LayeredTileMaps using that source to reference an invalid source instead. This may result in unexpected data loss. Change this ID carefully.")));
		label->set_autowrap(true);
		label->set_custom_minimum_size(Vector2(400, 0) * EDSCALE);
		vbox->add_child(label);

		id_input = memnew(SpinBox);
		vbox->add_child(id_input);
		id_input->set_max(INT_MAX);

		id_edit_dialog->connect("confirmed", this, "_confirm_change_id");
	}
	edited_source = p_for_source;
	id_input->set_value(p_for_source->get("id"));

	//id_edit_dialog->popup_centered(Vector2i(400, 0) * EDSCALE);
	id_edit_dialog->popup_centered();

	id_input->get_line_edit()->call_deferred("grab_focus");
}

void LayeredTileSourceInspectorPlugin::_confirm_change_id() {
	edited_source->set("id", id_input->get_value());
	id_label->set_text(itos(edited_source->get("id"))); // Use get(), because the provided ID might've been invalid.
}

bool LayeredTileSourceInspectorPlugin::can_handle(Object *p_object) {
	return p_object->is_class("LayeredTileSetAtlasSourceProxyObject") || p_object->is_class("LayeredTileSetScenesCollectionProxyObject");
}

bool LayeredTileSourceInspectorPlugin::parse_property(Object *p_object, Variant::Type p_type, const String &p_path, PropertyHint p_hint, const String &p_hint_text, int p_usage) {
	if (p_path == "id") {
		const Variant value = p_object->get("id");
		if (value.get_type() == Variant::NIL) { // May happen if the object is not yet initialized.
			return true;
		}

		EditorProperty *ep = memnew(EditorProperty);

		HBoxContainer *hbox = memnew(HBoxContainer);
		hbox->set_alignment(BoxContainer::ALIGN_CENTER);

		id_label = memnew(Label(itos(value)));
		id_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		hbox->add_child(id_label);

		Button *button = memnew(Button(TTR("Edit")));
		button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		hbox->add_child(button);
		button->connect("pressed", this, "_show_id_edit_dialog", varray(p_object));

		ep->add_child(hbox);
		add_property_editor(p_path, ep);
		return true;
	}
	return false;
}

void LayeredTileSourceInspectorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_confirm_change_id"), &LayeredTileSourceInspectorPlugin::_confirm_change_id);
	ClassDB::bind_method(D_METHOD("_show_id_edit_dialog"), &LayeredTileSourceInspectorPlugin::_show_id_edit_dialog);
}
