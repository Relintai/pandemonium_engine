/*************************************************************************/
/*  layered_tile_set_scenes_collection_source_editor.cpp                 */
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

#include "layered_tile_set_scenes_collection_source_editor.h"

#include "editor/editor_file_system.h"
#include "editor/editor_node.h"
#include "editor/editor_resource_preview.h"
#include "editor/editor_settings.h"

#include "core/object/undo_redo.h"

#include "editor/editor_file_dialog.h"
#include "layered_tile_set_editor.h"

#include "editor/editor_scale.h"

#include "scene/gui/button.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"
#include "scene/gui/split_container.h"

#include "core/core_string_names.h"

void LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::set_id(int p_id) {
	ERR_FAIL_COND(p_id < 0);
	if (source_id == p_id) {
		return;
	}
	ERR_FAIL_COND_MSG(tile_set->has_source(p_id), vformat("Cannot change LayeredTileSet Scenes Collection source ID. Another LayeredTileSet source exists with id %d.", p_id));

	int previous_source = source_id;
	source_id = p_id; // source_id must be updated before, because it's used by the source list update.
	tile_set->set_source_id(previous_source, p_id);
	emit_signal("changed", "id");
}

int LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::get_id() {
	return source_id;
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::_changed_callback(Object *p_changed, const char *p_prop) {
	property_list_changed_notify();
}

bool LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;
	if (name == "name") {
		// Use the resource_name property to store the source's name.
		name = "resource_name";
	}
	bool valid = false;
	tile_set_scenes_collection_source->set(name, p_value, &valid);
	if (valid) {
		emit_signal("changed", String(name).utf8().get_data());
	}
	return valid;
}

bool LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::_get(const StringName &p_name, Variant &r_ret) const {
	if (!tile_set_scenes_collection_source) {
		return false;
	}
	String name = p_name;
	if (name == "name") {
		// Use the resource_name property to store the source's name.
		name = "resource_name";
	}
	bool valid = false;
	r_ret = tile_set_scenes_collection_source->get(name, &valid);
	return valid;
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::STRING, "name", PROPERTY_HINT_NONE, ""));
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::_bind_methods() {
	// -- Shape and layout --
	ClassDB::bind_method(D_METHOD("set_id", "id"), &LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::get_id);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "what")));
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionProxyObject::edit(Ref<LayeredTileSet> p_tile_set, LayeredTileSetScenesCollectionSource *p_tile_set_scenes_collection_source, int p_source_id) {
	ERR_FAIL_COND(!p_tile_set.is_valid());
	ERR_FAIL_NULL(p_tile_set_scenes_collection_source);
	ERR_FAIL_COND(p_source_id < 0);
	ERR_FAIL_COND(p_tile_set->get_source(p_source_id) != p_tile_set_scenes_collection_source);

	if (tile_set == p_tile_set && tile_set_scenes_collection_source == p_tile_set_scenes_collection_source && source_id == p_source_id) {
		return;
	}

	// Disconnect to changes.
	if (tile_set_scenes_collection_source) {
		tile_set_scenes_collection_source->remove_change_receptor(this);
	}

	tile_set = p_tile_set;
	tile_set_scenes_collection_source = p_tile_set_scenes_collection_source;
	source_id = p_source_id;

	// Connect to changes.
	if (tile_set_scenes_collection_source) {
		tile_set_scenes_collection_source->remove_change_receptor(this);
		tile_set_scenes_collection_source->add_change_receptor(this);
	}

	property_list_changed_notify();
}

// -- Proxy object used by the tile inspector --
bool LayeredTileSetScenesCollectionSourceEditor::LayeredSceneTileProxyObject::_set(const StringName &p_name, const Variant &p_value) {
	if (!tile_set_scenes_collection_source) {
		return false;
	}

	if (p_name == "id") {
		int as_int = int(p_value);
		ERR_FAIL_COND_V(as_int < 0, false);
		ERR_FAIL_COND_V(tile_set_scenes_collection_source->has_scene_tile_id(as_int), false);
		tile_set_scenes_collection_source->set_scene_tile_id(scene_id, as_int);
		scene_id = as_int;
		emit_signal("changed", "id");
		for (int i = 0; i < tile_set_scenes_collection_source_editor->scene_tiles_list->get_item_count(); i++) {
			if (int(tile_set_scenes_collection_source_editor->scene_tiles_list->get_item_metadata(i)) == scene_id) {
				tile_set_scenes_collection_source_editor->scene_tiles_list->select(i);
				break;
			}
		}
		return true;
	} else if (p_name == "scene") {
		tile_set_scenes_collection_source->set_scene_tile_scene(scene_id, p_value);
		emit_signal("changed", "scene");
		return true;
	} else if (p_name == "display_placeholder") {
		tile_set_scenes_collection_source->set_scene_tile_display_placeholder(scene_id, p_value);
		emit_signal("changed", "display_placeholder");
		return true;
	}

	return false;
}

bool LayeredTileSetScenesCollectionSourceEditor::LayeredSceneTileProxyObject::_get(const StringName &p_name, Variant &r_ret) const {
	if (!tile_set_scenes_collection_source) {
		return false;
	}

	if (p_name == "id") {
		r_ret = scene_id;
		return true;
	} else if (p_name == "scene") {
		r_ret = tile_set_scenes_collection_source->get_scene_tile_scene(scene_id);
		return true;
	} else if (p_name == "display_placeholder") {
		r_ret = tile_set_scenes_collection_source->get_scene_tile_display_placeholder(scene_id);
		return true;
	}

	return false;
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredSceneTileProxyObject::_get_property_list(List<PropertyInfo> *p_list) const {
	if (!tile_set_scenes_collection_source) {
		return;
	}

	p_list->push_back(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_NONE, ""));
	p_list->push_back(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"));
	p_list->push_back(PropertyInfo(Variant::BOOL, "display_placeholder", PROPERTY_HINT_NONE, ""));
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredSceneTileProxyObject::edit(LayeredTileSetScenesCollectionSource *p_tile_set_scenes_collection_source, int p_scene_id) {
	ERR_FAIL_NULL(p_tile_set_scenes_collection_source);
	ERR_FAIL_COND(!p_tile_set_scenes_collection_source->has_scene_tile_id(p_scene_id));

	if (tile_set_scenes_collection_source == p_tile_set_scenes_collection_source && scene_id == p_scene_id) {
		return;
	}

	tile_set_scenes_collection_source = p_tile_set_scenes_collection_source;
	scene_id = p_scene_id;

	property_list_changed_notify();
}

void LayeredTileSetScenesCollectionSourceEditor::LayeredSceneTileProxyObject::_bind_methods() {
	ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "what")));
}

void LayeredTileSetScenesCollectionSourceEditor::_scenes_collection_source_proxy_object_changed(const String &p_what) {
	if (p_what == "id") {
		emit_signal("source_id_changed", scenes_collection_source_proxy_object->get_id());
	}
}

void LayeredTileSetScenesCollectionSourceEditor::_tile_set_scenes_collection_source_changed() {
	tile_set_scenes_collection_source_changed_needs_update = true;
}

void LayeredTileSetScenesCollectionSourceEditor::_scene_thumbnail_done(const String &p_path, const Ref<Texture> &p_preview, const Ref<Texture> &p_small_preview, const Variant &p_ud) {
	int index = p_ud;

	if (index >= 0 && index < scene_tiles_list->get_item_count()) {
		scene_tiles_list->set_item_icon(index, p_preview);
	}
}

void LayeredTileSetScenesCollectionSourceEditor::_scenes_list_item_activated(int p_index) {
	Ref<PackedScene> packed_scene = tile_set_scenes_collection_source->get_scene_tile_scene(scene_tiles_list->get_item_metadata(p_index));
	if (packed_scene.is_valid()) {
		EditorNode::get_singleton()->open_request(packed_scene->get_path());
	}
}

void LayeredTileSetScenesCollectionSourceEditor::_source_add_pressed() {
	if (!scene_select_dialog) {
		scene_select_dialog = memnew(EditorFileDialog);
		add_child(scene_select_dialog);
		scene_select_dialog->set_mode(EditorFileDialog::MODE_OPEN_FILE);
		scene_select_dialog->connect("file_selected", this, "_scene_file_selected");

		scene_select_dialog->add_filter("*.tscn;TSCN");
		scene_select_dialog->add_filter("*.scn;SCN");
	}

	scene_select_dialog->popup_centered_ratio();
}

void LayeredTileSetScenesCollectionSourceEditor::_scene_file_selected(const String &p_path) {
	Ref<PackedScene> scene = ResourceLoader::load(p_path);

	int scene_id = tile_set_scenes_collection_source->get_next_scene_tile_id();
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action(TTR("Add a Scene Tile"));
	undo_redo->add_do_method(tile_set_scenes_collection_source, "create_scene_tile", scene, scene_id);
	undo_redo->add_undo_method(tile_set_scenes_collection_source, "remove_scene_tile", scene_id);
	undo_redo->commit_action();
	_update_scenes_list();
	_update_action_buttons();
	_update_tile_inspector();
}

void LayeredTileSetScenesCollectionSourceEditor::_source_delete_pressed() {
	Vector<int> selected_indices = scene_tiles_list->get_selected_items();
	ERR_FAIL_COND(selected_indices.empty());
	int scene_id = scene_tiles_list->get_item_metadata(selected_indices[0]);

	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action(TTR("Remove a Scene Tile"));
	undo_redo->add_do_method(tile_set_scenes_collection_source, "remove_scene_tile", scene_id);
	undo_redo->add_undo_method(tile_set_scenes_collection_source, "create_scene_tile", tile_set_scenes_collection_source->get_scene_tile_scene(scene_id), scene_id);
	undo_redo->commit_action();
	_update_scenes_list();
	_update_action_buttons();
	_update_tile_inspector();
}

void LayeredTileSetScenesCollectionSourceEditor::_update_source_inspector() {
	// Update the proxy object.
	scenes_collection_source_proxy_object->edit(tile_set, tile_set_scenes_collection_source, tile_set_source_id);
}

void LayeredTileSetScenesCollectionSourceEditor::_update_tile_inspector() {
	Vector<int> selected_indices = scene_tiles_list->get_selected_items();
	bool has_atlas_tile_selected = (selected_indices.size() > 0);

	// Update the proxy object.
	if (has_atlas_tile_selected) {
		int scene_id = scene_tiles_list->get_item_metadata(selected_indices[0]);
		tile_proxy_object->edit(tile_set_scenes_collection_source, scene_id);
	}

	// Update visibility.
	tile_inspector_label->set_visible(has_atlas_tile_selected);
	tile_inspector->set_visible(has_atlas_tile_selected);
}

void LayeredTileSetScenesCollectionSourceEditor::_update_action_buttons() {
	Vector<int> selected_indices = scene_tiles_list->get_selected_items();
	scene_tile_delete_button->set_disabled(selected_indices.size() <= 0);
}

void LayeredTileSetScenesCollectionSourceEditor::_update_scenes_list() {
	if (!tile_set_scenes_collection_source) {
		return;
	}

	// Get the previously selected id.
	Vector<int> selected_indices = scene_tiles_list->get_selected_items();
	int old_selected_scene_id = (selected_indices.size() > 0) ? int(scene_tiles_list->get_item_metadata(selected_indices[0])) : -1;

	// Clear the list.
	scene_tiles_list->clear();

	// Rebuild the list.
	int to_reselect = -1;
	for (int i = 0; i < tile_set_scenes_collection_source->get_scene_tiles_count(); i++) {
		int scene_id = tile_set_scenes_collection_source->get_scene_tile_id(i);

		Ref<PackedScene> scene = tile_set_scenes_collection_source->get_scene_tile_scene(scene_id);

		int item_index = 0;
		if (scene.is_valid()) {
			scene_tiles_list->add_item(vformat("%s (path:%s id:%d)", scene->get_path().get_file().get_basename(), scene->get_path(), scene_id));
			item_index = scene_tiles_list->get_item_count() - 1;
			Variant udata = i;
			EditorResourcePreview::get_singleton()->queue_edited_resource_preview(scene, this, "_scene_thumbnail_done", udata);
		} else {
			scene_tiles_list->add_item(TTR("Tile with Invalid Scene"), get_theme_icon("PackedScene", "EditorIcons"));
			item_index = scene_tiles_list->get_item_count() - 1;
		}
		scene_tiles_list->set_item_metadata(item_index, scene_id);

		if (old_selected_scene_id >= 0 && scene_id == old_selected_scene_id) {
			to_reselect = i;
		}
	}
	if (scene_tiles_list->get_item_count() == 0) {
		scene_tiles_list->add_item(TTR("Drag and drop scenes here or use the Add button."));
		scene_tiles_list->set_item_disabled(scene_tiles_list->get_item_count() - 1, true);
	}

	// Reselect if needed.
	if (to_reselect >= 0) {
		scene_tiles_list->select(to_reselect);
	}

	// Icon size update.
	int int_size = int(EDITOR_GET("filesystem/file_dialog/thumbnail_size")) * EDSCALE;
	scene_tiles_list->set_fixed_icon_size(Vector2(int_size, int_size));
}

void LayeredTileSetScenesCollectionSourceEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			scenes_collection_source_inspector->edit(scenes_collection_source_proxy_object);
			/*
			scenes_collection_source_inspector->add_custom_property_description("LayeredTileSetScenesCollectionProxyObject", "id", TTR("The tile's unique identifier within this LayeredTileSet. Each tile stores its source ID, so changing one may make tiles invalid."));
			scenes_collection_source_inspector->add_custom_property_description("LayeredTileSetScenesCollectionProxyObject", "name", TTR("The human-readable name for the scene collection. Use a descriptive name here for organizational purposes (such as \"obstacles\", \"decoration\", etc.)."));
			*/

			tile_inspector->edit(tile_proxy_object);

			/*
			tile_inspector->add_custom_property_description("LayeredSceneTileProxyObject", "id", TTR("ID of the scene tile in the collection. Each painted tile has associated ID, so changing this property may cause your LayeredTileMaps to not display properly."));
			tile_inspector->add_custom_property_description("LayeredSceneTileProxyObject", "scene", TTR("Absolute path to the scene associated with this tile."));
			tile_inspector->add_custom_property_description("LayeredSceneTileProxyObject", "display_placeholder", TTR("If [code]true[/code], a placeholder marker will be displayed on top of the scene's preview. The marker is displayed anyway if the scene has no valid preview."));
			*/
		} break;

		case NOTIFICATION_THEME_CHANGED: {
			scene_tile_add_button->set_icon(get_theme_icon("Add", "EditorIcons"));
			scene_tile_delete_button->set_icon(get_theme_icon("Remove", "EditorIcons"));
			_update_scenes_list();
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (tile_set_scenes_collection_source_changed_needs_update) {
				// Update everything.
				_update_source_inspector();
				_update_scenes_list();
				_update_action_buttons();
				_update_tile_inspector();
				tile_set_scenes_collection_source_changed_needs_update = false;
			}
		} break;

		case NOTIFICATION_VISIBILITY_CHANGED: {
			// Update things just in case.
			_update_scenes_list();
			_update_action_buttons();
		} break;
	}
}

void LayeredTileSetScenesCollectionSourceEditor::edit(Ref<LayeredTileSet> p_tile_set, LayeredTileSetScenesCollectionSource *p_tile_set_scenes_collection_source, int p_source_id) {
	ERR_FAIL_COND(!p_tile_set.is_valid());
	ERR_FAIL_NULL(p_tile_set_scenes_collection_source);
	ERR_FAIL_COND(p_source_id < 0);
	ERR_FAIL_COND(p_tile_set->get_source(p_source_id) != p_tile_set_scenes_collection_source);

	if (p_tile_set == tile_set && p_tile_set_scenes_collection_source == tile_set_scenes_collection_source && p_source_id == tile_set_source_id) {
		return;
	}

	// Remove listener for old objects.
	if (tile_set_scenes_collection_source) {
		tile_set_scenes_collection_source->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_scenes_collection_source_changed");
	}

	// Change the edited object.
	tile_set = p_tile_set;
	tile_set_scenes_collection_source = p_tile_set_scenes_collection_source;
	tile_set_source_id = p_source_id;

	if (tile_set.is_valid()) {
		scenes_collection_source_inspector->set_read_only(false);
		tile_inspector->set_read_only(false);

		scene_tile_add_button->set_disabled(false);
	}

	// Add the listener again.
	if (tile_set_scenes_collection_source) {
		tile_set_scenes_collection_source->connect(CoreStringNames::get_singleton()->changed, this, "_tile_set_scenes_collection_source_changed");
	}

	// Update everything.
	_update_source_inspector();
	_update_scenes_list();
	_update_action_buttons();
	_update_tile_inspector();
}

void LayeredTileSetScenesCollectionSourceEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	if (!can_drop_data_fw(p_point, p_data, p_from)) {
		return;
	}

	if (p_from == scene_tiles_list) {
		// Handle dropping a texture in the list of atlas resources.
		Dictionary d = p_data;
		Vector<String> files = d["files"];
		for (int i = 0; i < files.size(); i++) {
			Ref<PackedScene> resource = ResourceLoader::load(files[i]);
			if (resource.is_valid()) {
				int scene_id = tile_set_scenes_collection_source->get_next_scene_tile_id();
				UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
				undo_redo->create_action(TTR("Add a Scene Tile"));
				undo_redo->add_do_method(tile_set_scenes_collection_source, "create_scene_tile", resource, scene_id);
				undo_redo->add_undo_method(tile_set_scenes_collection_source, "remove_scene_tile", scene_id);
				undo_redo->commit_action();
			}
		}

		_update_scenes_list();
		_update_action_buttons();
		_update_tile_inspector();
	}
}

bool LayeredTileSetScenesCollectionSourceEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	if (p_from == scene_tiles_list) {
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

				if (!ClassDB::is_parent_class(ftype, "PackedScene")) {
					return false;
				}
			}

			return true;
		}
	}
	return false;
}

void LayeredTileSetScenesCollectionSourceEditor::_tile_proxy_object_changed(const String &p_what) {
	_update_scenes_list();
	_update_action_buttons();
}

void LayeredTileSetScenesCollectionSourceEditor::_scene_tiles_list_item_selected(int p_index) {
	_update_tile_inspector();
	_update_action_buttons();
}

void LayeredTileSetScenesCollectionSourceEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("source_id_changed", PropertyInfo(Variant::INT, "source_id")));

	ClassDB::bind_method(D_METHOD("_scene_thumbnail_done"), &LayeredTileSetScenesCollectionSourceEditor::_scene_thumbnail_done);
	ClassDB::bind_method(D_METHOD("_scene_file_selected"), &LayeredTileSetScenesCollectionSourceEditor::_scene_file_selected);

	ClassDB::bind_method(D_METHOD("_tile_set_scenes_collection_source_changed"), &LayeredTileSetScenesCollectionSourceEditor::_tile_set_scenes_collection_source_changed);
	ClassDB::bind_method(D_METHOD("drop_data_fw"), &LayeredTileSetScenesCollectionSourceEditor::drop_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &LayeredTileSetScenesCollectionSourceEditor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("_scenes_collection_source_proxy_object_changed"), &LayeredTileSetScenesCollectionSourceEditor::_scenes_collection_source_proxy_object_changed);

	ClassDB::bind_method(D_METHOD("_tile_proxy_object_changed"), &LayeredTileSetScenesCollectionSourceEditor::_tile_proxy_object_changed);

	ClassDB::bind_method(D_METHOD("_scenes_list_item_activated"), &LayeredTileSetScenesCollectionSourceEditor::_scenes_list_item_activated);
	ClassDB::bind_method(D_METHOD("_source_add_pressed"), &LayeredTileSetScenesCollectionSourceEditor::_source_add_pressed);
	ClassDB::bind_method(D_METHOD("_source_delete_pressed"), &LayeredTileSetScenesCollectionSourceEditor::_source_delete_pressed);
	ClassDB::bind_method(D_METHOD("_scene_tiles_list_item_selected"), &LayeredTileSetScenesCollectionSourceEditor::_scene_tiles_list_item_selected);
}

LayeredTileSetScenesCollectionSourceEditor::LayeredTileSetScenesCollectionSourceEditor() {
	// -- Right side --
	HSplitContainer *split_container_right_side = memnew(HSplitContainer);
	split_container_right_side->set_h_size_flags(SIZE_EXPAND_FILL);
	add_child(split_container_right_side);

	// Middle panel.
	ScrollContainer *middle_panel = memnew(ScrollContainer);
	middle_panel->set_v_size_flags(SIZE_EXPAND_FILL);
	middle_panel->set_enable_h_scroll(false);
	middle_panel->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
	split_container_right_side->add_child(middle_panel);

	VBoxContainer *middle_vbox_container = memnew(VBoxContainer);
	middle_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);
	middle_panel->add_child(middle_vbox_container);

	// Scenes collection source inspector.
	scenes_collection_source_inspector_label = memnew(Label);
	scenes_collection_source_inspector_label->set_text(TTR("Scenes collection properties:"));
	middle_vbox_container->add_child(scenes_collection_source_inspector_label);

	scenes_collection_source_proxy_object = memnew(LayeredTileSetScenesCollectionProxyObject());
	scenes_collection_source_proxy_object->connect("changed", this, "_scenes_collection_source_proxy_object_changed");

	scenes_collection_source_inspector = memnew(EditorInspector);
	scenes_collection_source_inspector->set_enable_h_scroll(false);
	scenes_collection_source_inspector->set_enable_v_scroll(false);
	scenes_collection_source_inspector->set_use_doc_hints(true);
	scenes_collection_source_inspector->set_undo_redo(EditorNode::get_singleton()->get_undo_redo());
	scenes_collection_source_inspector->add_inspector_plugin(memnew(LayeredTileSourceInspectorPlugin));
	middle_vbox_container->add_child(scenes_collection_source_inspector);

	// Tile inspector.
	tile_inspector_label = memnew(Label);
	tile_inspector_label->set_text(TTR("Tile properties:"));
	tile_inspector_label->hide();
	middle_vbox_container->add_child(tile_inspector_label);

	tile_proxy_object = memnew(LayeredSceneTileProxyObject(this));
	tile_proxy_object->connect("changed", this, "_update_scenes_list");

	tile_inspector = memnew(EditorInspector);
	tile_inspector->set_enable_v_scroll(false);
	tile_inspector->set_enable_h_scroll(false);
	tile_inspector->set_use_doc_hints(true);
	tile_inspector->set_use_folding(true);
	tile_inspector->set_undo_redo(EditorNode::get_singleton()->get_undo_redo());
	middle_vbox_container->add_child(tile_inspector);

	// Scenes list.
	VBoxContainer *right_vbox_container = memnew(VBoxContainer);
	split_container_right_side->add_child(right_vbox_container);

	scene_tiles_list = memnew(ItemList);
	scene_tiles_list->set_h_size_flags(SIZE_EXPAND_FILL);
	scene_tiles_list->set_v_size_flags(SIZE_EXPAND_FILL);

	scene_tiles_list->set_drag_forwarding(this);

	scene_tiles_list->connect("item_selected", this, "_scene_tiles_list_item_selected");
	scene_tiles_list->connect("item_activated", this, "_scenes_list_item_activated");
	right_vbox_container->add_child(scene_tiles_list);

	HBoxContainer *scenes_bottom_actions = memnew(HBoxContainer);
	right_vbox_container->add_child(scenes_bottom_actions);

	scene_tile_add_button = memnew(Button);
	scene_tile_add_button->set_theme_type_variation("FlatButton");
	scene_tile_add_button->connect("pressed", this, "_source_add_pressed");
	scenes_bottom_actions->add_child(scene_tile_add_button);

	scene_tile_delete_button = memnew(Button);
	scene_tile_delete_button->set_theme_type_variation("FlatButton");
	scene_tile_delete_button->set_disabled(true);
	scene_tile_delete_button->connect("pressed", this, "_source_delete_pressed");
	scenes_bottom_actions->add_child(scene_tile_delete_button);
}

LayeredTileSetScenesCollectionSourceEditor::~LayeredTileSetScenesCollectionSourceEditor() {
	memdelete(scenes_collection_source_proxy_object);
	memdelete(tile_proxy_object);
}
