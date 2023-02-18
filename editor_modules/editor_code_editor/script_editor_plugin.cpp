/*************************************************************************/
/*  script_editor_plugin.cpp                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "script_editor_plugin.h"

#include "core/config/project_settings.h"
#include "core/containers/pair.h"
#include "core/containers/rb_map.h"
#include "core/error/error_macros.h"
#include "core/input/input.h"
#include "core/input/input_event.h"
#include "core/io/config_file.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/math/color.h"
#include "core/math/math_funcs.h"
#include "core/math/transform_2d.h"
#include "core/object/class_db.h"
#include "core/object/undo_redo.h"
#include "core/os/file_access.h"
#include "core/os/keyboard.h"
#include "core/os/main_loop.h"
#include "core/os/memory.h"
#include "core/os/os.h"
#include "core/string/string_name.h"
#include "core/variant/dictionary.h"
#include "core/version_generated.gen.h"
#include "editor/editor_data.h"
#include "editor/editor_file_dialog.h"
#include "editor/editor_file_system.h"
#include "editor/editor_help.h"
#include "editor/editor_help_search.h"
#include "editor/editor_node.h"
#include "editor/editor_run_script.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/filesystem_dock.h"
#include "editor/find_in_files.h"
#include "editor/inspector_dock.h"
#include "editor/node_dock.h"
#include "editor/script_create_dialog.h"
#include "editor/script_editor_debugger.h"
#include "scene/2d/canvas_item.h"
#include "scene/gui/button.h"
#include "scene/gui/control.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/separator.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tool_button.h"
#include "scene/gui/tree.h"
#include "scene/main/node.h"
#include "scene/main/scene_tree.h"
#include "scene/main/timer.h"
#include "scene/main/viewport.h"
#include "scene/resources/text_file.h"
#include "scene/resources/texture.h"
#include "scene/scene_string_names.h"

#include "editor_script_editor_quick_open.h"
#include "editor_script_text_editor.h"
#include "editor_text_editor.h"

#include "editor_script_editor.h"

static bool _is_built_in_script(Script *p_script) {
	String path = p_script->get_path();

	return path.find("::") != -1;
}

void ScriptEditorPlugin::edit(Object *p_object) {
	if (Object::cast_to<Script>(p_object)) {
		Script *p_script = Object::cast_to<Script>(p_object);
		String res_path = p_script->get_path().get_slice("::", 0);

		if (_is_built_in_script(p_script)) {
			if (ResourceLoader::get_resource_type(res_path) == "PackedScene") {
				if (!EditorNode::get_singleton()->is_scene_open(res_path)) {
					EditorNode::get_singleton()->load_scene(res_path);
				}
			} else {
				EditorNode::get_singleton()->load_resource(res_path);
			}
		}
		script_editor->edit(p_script);
	} else if (Object::cast_to<TextFile>(p_object)) {
		script_editor->edit(Object::cast_to<TextFile>(p_object));
	}
}

bool ScriptEditorPlugin::handles(Object *p_object) const {
	if (Object::cast_to<TextFile>(p_object)) {
		return true;
	}

	Script *script = Object::cast_to<Script>(p_object);
	if (script) {
		return script->get_language() != nullptr; // Could be a PluginScript with no language attached.
	}

	return p_object->is_class("Script");
}

void ScriptEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		script_editor->show();
		script_editor->set_process(true);
		script_editor->ensure_select_current();
	} else {
		script_editor->hide();
		script_editor->set_process(false);
	}
}

void ScriptEditorPlugin::selected_notify() {
	script_editor->ensure_select_current();
}

void ScriptEditorPlugin::save_external_data() {
	script_editor->save_all_scripts();
}

void ScriptEditorPlugin::apply_changes() {
	script_editor->apply_scripts();
}

void ScriptEditorPlugin::restore_global_state() {
}

void ScriptEditorPlugin::save_global_state() {
}

void ScriptEditorPlugin::set_window_layout(Ref<ConfigFile> p_layout) {
	script_editor->set_window_layout(p_layout);
}

void ScriptEditorPlugin::get_window_layout(Ref<ConfigFile> p_layout) {
	script_editor->get_window_layout(p_layout);
}

void ScriptEditorPlugin::get_breakpoints(List<String> *p_breakpoints) {
	script_editor->get_breakpoints(p_breakpoints);
}

void ScriptEditorPlugin::edited_scene_changed() {
	script_editor->edited_scene_changed();
}

ScriptEditorPlugin::ScriptEditorPlugin(EditorNode *p_node) {
	EditorScriptTextEditor::register_editor(); //register one for text scripts
	TextEditor::register_editor();

	editor = p_node;
	script_editor = memnew(ScriptEditor(p_node));
	editor->get_viewport()->add_child(script_editor);
	script_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	script_editor->hide();

	EDITOR_DEF("text_editor/files/auto_reload_scripts_on_external_change", true);
	ScriptServer::set_reload_scripts_on_save(EDITOR_DEF("text_editor/files/auto_reload_and_parse_scripts_on_save", true));
	EDITOR_DEF("text_editor/files/open_dominant_script_on_scene_change", true);
	EDITOR_DEF("text_editor/external/use_external_editor", false);
	EDITOR_DEF("text_editor/external/exec_path", "");
	EDITOR_DEF("text_editor/script_list/script_temperature_enabled", true);
	EDITOR_DEF("text_editor/script_list/highlight_current_script", true);
	EDITOR_DEF("text_editor/script_list/script_temperature_history_size", 15);
	EDITOR_DEF("text_editor/script_list/current_script_background_color", Color(1, 1, 1, 0.3));
	EDITOR_DEF("text_editor/script_list/group_help_pages", true);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "text_editor/script_list/sort_scripts_by", PROPERTY_HINT_ENUM, "Name,Path,None"));
	EDITOR_DEF("text_editor/script_list/sort_scripts_by", 0);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "text_editor/script_list/list_script_names_as", PROPERTY_HINT_ENUM, "Name,Parent Directory And Name,Full Path"));
	EDITOR_DEF("text_editor/script_list/list_script_names_as", 0);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "text_editor/external/exec_path", PROPERTY_HINT_GLOBAL_FILE));
	EDITOR_DEF("text_editor/external/exec_flags", "{file}");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "text_editor/external/exec_flags", PROPERTY_HINT_PLACEHOLDER_TEXT, "Call flags with placeholders: {project}, {file}, {col}, {line}."));

	ED_SHORTCUT("script_editor/reopen_closed_script", TTR("Reopen Closed Script"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_T);
	ED_SHORTCUT("script_editor/clear_recent", TTR("Clear Recent Scripts"));
}

ScriptEditorPlugin::~ScriptEditorPlugin() {
}
