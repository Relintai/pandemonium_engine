/*************************************************************************/
/*  editor_script_editor.cpp                                             */
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

#include "editor_script_editor.h"

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
#include "scene/gui/button.h"
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
#include "scene/main/canvas_item.h"
#include "scene/main/control.h"
#include "scene/main/node.h"
#include "scene/main/scene_string_names.h"
#include "scene/main/scene_tree.h"
#include "scene/main/timer.h"
#include "scene/main/viewport.h"
#include "scene/resources/text_file.h"
#include "scene/resources/texture.h"

#include "editor_script_editor_quick_open.h"
#include "editor_script_text_editor.h"
#include "editor_text_editor.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_SHADER_EDITOR_ENABLED
#include "shader_editor/shader_editor_plugin.h"
#endif

class EditorScriptCodeCompletionCache : public ScriptCodeCompletionCache {
	struct Cache {
		uint64_t time_loaded;
		RES cache;

		Cache() {
			time_loaded = 0;
		}
	};

	RBMap<String, Cache> cached;

public:
	uint64_t max_time_cache;
	int max_cache_size;

	void cleanup() {
		List<RBMap<String, Cache>::Element *> to_clean;

		RBMap<String, Cache>::Element *I = cached.front();
		while (I) {
			if ((OS::get_singleton()->get_ticks_msec() - I->get().time_loaded) > max_time_cache) {
				to_clean.push_back(I);
			}
			I = I->next();
		}

		while (to_clean.front()) {
			cached.erase(to_clean.front()->get());
			to_clean.pop_front();
		}
	}

	virtual RES get_cached_resource(const String &p_path) {
		RBMap<String, Cache>::Element *E = cached.find(p_path);
		if (!E) {
			Cache c;
			c.cache = ResourceLoader::load(p_path);
			E = cached.insert(p_path, c);
		}

		E->get().time_loaded = OS::get_singleton()->get_ticks_msec();

		if (cached.size() > max_cache_size) {
			uint64_t older;
			RBMap<String, Cache>::Element *O = cached.front();
			older = O->get().time_loaded;
			RBMap<String, Cache>::Element *I = O;
			while (I) {
				if (I->get().time_loaded < older) {
					older = I->get().time_loaded;
					O = I;
				}
				I = I->next();
			}

			if (O != E) { //should never happen..
				cached.erase(O);
			}
		}

		return E->get().cache;
	}

	EditorScriptCodeCompletionCache() {
		max_cache_size = 128;
		max_time_cache = 5 * 60 * 1000; //minutes, five
	}

	virtual ~EditorScriptCodeCompletionCache() {}
};

/////////////////////////////////

EditorScriptEditor *EditorScriptEditor::script_editor = nullptr;

/*** SCRIPT EDITOR ******/

String EditorScriptEditor::_get_debug_tooltip(const String &p_text, Node *_se) {
	String val = debugger->get_var_value(p_text);
	if (val != String()) {
		return p_text + ": " + val;
	} else {
		return String();
	}
}

void EditorScriptEditor::_breaked(bool p_breaked, bool p_can_debug, const String &p_reason, bool p_has_stackdump) {
	if (bool(EditorSettings::get_singleton()->get("text_editor/external/use_external_editor"))) {
		return;
	}

	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_NEXT), !(p_breaked && p_can_debug));
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_STEP), !(p_breaked && p_can_debug));
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_BREAK), p_breaked);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_CONTINUE), !p_breaked);

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		se->set_debugger_active(p_breaked);
	}
}

void EditorScriptEditor::_show_debugger(bool p_show) {
}

void EditorScriptEditor::_script_created(Ref<Script> p_script) {
	editor->push_item(p_script.operator->());
}

void EditorScriptEditor::_goto_script_line2(int p_line) {
	EditorScriptEditorBase *current = _get_current_editor();
	if (current) {
		current->goto_line(p_line);
	}
}

void EditorScriptEditor::_goto_script_line(REF p_script, int p_line) {
	Ref<Script> script = Object::cast_to<Script>(*p_script);
	if (script.is_valid() && (script->has_source_code() || script->get_path().is_resource_file())) {
		if (edit(p_script, p_line, 0)) {
			editor->push_item(p_script.ptr());

			EditorScriptEditorBase *current = _get_current_editor();
			if (EditorScriptTextEditor *script_text_editor = Object::cast_to<EditorScriptTextEditor>(current)) {
				script_text_editor->goto_line_centered(p_line);
			} else if (current) {
				current->goto_line(p_line, true);
			}
		}
	}
}

void EditorScriptEditor::_set_execution(REF p_script, int p_line) {
	Ref<Script> script = Object::cast_to<Script>(*p_script);
	if (script.is_valid() && (script->has_source_code() || script->get_path().is_resource_file())) {
		for (int i = 0; i < tab_container->get_child_count(); i++) {
			EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
			if (!se) {
				continue;
			}

			if ((script != nullptr && se->get_edited_resource() == p_script) || se->get_edited_resource()->get_path() == script->get_path()) {
				se->set_executing_line(p_line);
			}
		}
	}
}

void EditorScriptEditor::_clear_execution(REF p_script) {
	Ref<Script> script = Object::cast_to<Script>(*p_script);
	if (script.is_valid() && (script->has_source_code() || script->get_path().is_resource_file())) {
		for (int i = 0; i < tab_container->get_child_count(); i++) {
			EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
			if (!se) {
				continue;
			}

			if ((script != nullptr && se->get_edited_resource() == p_script) || se->get_edited_resource()->get_path() == script->get_path()) {
				se->clear_executing_line();
			}
		}
	}
}

EditorScriptEditorBase *EditorScriptEditor::_get_current_editor() const {
	int selected = tab_container->get_current_tab();
	if (selected < 0 || selected >= tab_container->get_child_count()) {
		return nullptr;
	}

	return Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(selected));
}

Control *EditorScriptEditor::_get_base_editor() const {
	return Object::cast_to<EditorScriptTextEditor>(_get_current_editor())->get_code_editor_text_edit();
}

void EditorScriptEditor::_update_history_arrows() {
	script_back->set_disabled(history_pos <= 0);
	script_forward->set_disabled(history_pos >= history.size() - 1);
}

void EditorScriptEditor::_save_history() {
	if (history_pos >= 0 && history_pos < history.size() && history[history_pos].control == tab_container->get_current_tab_control()) {
		Node *n = tab_container->get_current_tab_control();

		if (Object::cast_to<EditorScriptEditorBase>(n)) {
			history.write[history_pos].state = Object::cast_to<EditorScriptEditorBase>(n)->get_edit_state();
		}
		if (Object::cast_to<EditorHelp>(n)) {
			history.write[history_pos].state = Object::cast_to<EditorHelp>(n)->get_scroll();
		}
	}

	history.resize(history_pos + 1);
	ScriptHistory sh;
	sh.control = tab_container->get_current_tab_control();
	sh.state = Variant();

	history.push_back(sh);
	history_pos++;

	_update_history_arrows();
}

void EditorScriptEditor::_go_to_tab(int p_idx) {
	EditorScriptEditorBase *current = _get_current_editor();
	if (current) {
		if (current->is_unsaved()) {
			current->apply_code();
		}
	}

	Control *c = Object::cast_to<Control>(tab_container->get_child(p_idx));
	if (!c) {
		return;
	}

	if (history_pos >= 0 && history_pos < history.size() && history[history_pos].control == tab_container->get_current_tab_control()) {
		Node *n = tab_container->get_current_tab_control();

		if (Object::cast_to<EditorScriptEditorBase>(n)) {
			history.write[history_pos].state = Object::cast_to<EditorScriptEditorBase>(n)->get_edit_state();
		}
		if (Object::cast_to<EditorHelp>(n)) {
			history.write[history_pos].state = Object::cast_to<EditorHelp>(n)->get_scroll();
		}
	}

	history.resize(history_pos + 1);
	ScriptHistory sh;
	sh.control = c;
	sh.state = Variant();

	history.push_back(sh);
	history_pos++;

	tab_container->set_current_tab(p_idx);

	c = tab_container->get_current_tab_control();

	if (Object::cast_to<EditorScriptEditorBase>(c)) {
		script_name_label->set_text(Object::cast_to<EditorScriptEditorBase>(c)->get_name());
		script_icon->set_texture(Object::cast_to<EditorScriptEditorBase>(c)->get_icon());
		if (is_visible_in_tree()) {
			Object::cast_to<EditorScriptEditorBase>(c)->ensure_focus();
		}

		Ref<Script> script = Object::cast_to<EditorScriptEditorBase>(c)->get_edited_resource();
		if (script != nullptr) {
			notify_script_changed(script);
		}

		Object::cast_to<EditorScriptEditorBase>(c)->validate();
	}
	if (Object::cast_to<EditorHelp>(c)) {
		script_name_label->set_text(Object::cast_to<EditorHelp>(c)->get_class());
		script_icon->set_texture(get_theme_icon("Help", "EditorIcons"));
		if (is_visible_in_tree()) {
			Object::cast_to<EditorHelp>(c)->set_focused();
		}
	}

	c->set_meta("__editor_pass", ++edit_pass);
	_update_history_arrows();
	_update_script_colors();
	_update_members_overview();
	_update_help_overview();
	_update_selected_editor_menu();
	_update_members_overview_visibility();
	_update_help_overview_visibility();
}

void EditorScriptEditor::_add_recent_script(String p_path) {
	if (p_path.empty()) {
		return;
	}

	Array rc = EditorSettings::get_singleton()->get_project_metadata("recent_files", "scripts", Array());
	if (rc.find(p_path) != -1) {
		rc.erase(p_path);
	}
	rc.push_front(p_path);
	if (rc.size() > 10) {
		rc.resize(10);
	}

	EditorSettings::get_singleton()->set_project_metadata("recent_files", "scripts", rc);
	_update_recent_scripts();
}

void EditorScriptEditor::_update_recent_scripts() {
	Array rc = EditorSettings::get_singleton()->get_project_metadata("recent_files", "scripts", Array());
	recent_scripts->clear();

	String path;
	for (int i = 0; i < rc.size(); i++) {
		path = rc[i];
		recent_scripts->add_item(path.replace("res://", ""));
	}

	recent_scripts->add_separator();
	recent_scripts->add_shortcut(ED_SHORTCUT("script_editor/clear_recent", TTR("Clear Recent Files")));
	recent_scripts->set_item_disabled(recent_scripts->get_item_id(recent_scripts->get_item_count() - 1), rc.empty());

	recent_scripts->set_as_minsize();
}

void EditorScriptEditor::_open_recent_script(int p_idx) {
	// clear button
	if (p_idx == recent_scripts->get_item_count() - 1) {
		EditorSettings::get_singleton()->set_project_metadata("recent_files", "scripts", Array());
		call_deferred("_update_recent_scripts");
		return;
	}

	Array rc = EditorSettings::get_singleton()->get_project_metadata("recent_files", "scripts", Array());
	ERR_FAIL_INDEX(p_idx, rc.size());

	String path = rc[p_idx];
	// if its not on disk its a help file or deleted
	if (FileAccess::exists(path)) {
		List<String> extensions;
		ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);

		if (extensions.find(path.get_extension())) {
			Ref<Script> script = ResourceLoader::load(path);
			if (script.is_valid()) {
				edit(script, true);
				return;
			}
		}

		Error err;
		Ref<TextFile> text_file = _load_text_file(path, &err);
		if (text_file.is_valid()) {
			edit(text_file, true);
			return;
		}
		// if it's a path then it's most likely a deleted file not help
	} else if (path.find("::") != -1) {
		// built-in script
		String res_path = path.get_slice("::", 0);
		if (ResourceLoader::get_resource_type(res_path) == "PackedScene") {
			if (!EditorNode::get_singleton()->is_scene_open(res_path)) {
				EditorNode::get_singleton()->load_scene(res_path);
			}
		} else {
			EditorNode::get_singleton()->load_resource(res_path);
		}
		Ref<Script> script = ResourceLoader::load(path);
		if (script.is_valid()) {
			edit(script, true);
			return;
		}
	} else if (!path.is_resource_file()) {
		_help_class_open(path);
		return;
	}

	rc.remove(p_idx);
	EditorSettings::get_singleton()->set_project_metadata("recent_files", "scripts", rc);
	_update_recent_scripts();
	_show_error_dialog(path);
}

void EditorScriptEditor::_show_error_dialog(String p_path) {
	error_dialog->set_text(vformat(TTR("Can't open '%s'. The file could have been moved or deleted."), p_path));
	error_dialog->popup_centered_minsize();
}

void EditorScriptEditor::_close_tab(int p_idx, bool p_save, bool p_history_back) {
	int selected = p_idx;
	if (selected < 0 || selected >= tab_container->get_child_count()) {
		return;
	}

	Node *tselected = tab_container->get_child(selected);

	EditorScriptEditorBase *current = Object::cast_to<EditorScriptEditorBase>(tselected);
	if (current) {
		Ref<Script> script = current->get_edited_resource();
		if (p_save) {
			// Do not try to save internal scripts
			if (!script.is_valid() || !(script->is_built_in())) {
				save_current_script();
			}
		}

		if (script != nullptr) {
			previous_scripts.push_back(script->get_path());
			notify_script_close(script);
		}
	}

	// roll back to previous tab
	if (p_history_back) {
		_history_back();
	}

	//remove from history
	history.resize(history_pos + 1);

	for (int i = 0; i < history.size(); i++) {
		if (history[i].control == tselected) {
			history.remove(i);
			i--;
			history_pos--;
		}
	}

	if (history_pos >= history.size()) {
		history_pos = history.size() - 1;
	}

	int idx = tab_container->get_current_tab();
	if (current) {
		current->clear_edit_menu();
	}
	memdelete(tselected);
	if (idx >= tab_container->get_child_count()) {
		idx = tab_container->get_child_count() - 1;
	}
	if (idx >= 0) {
		if (history_pos >= 0) {
			idx = history[history_pos].control->get_index();
		}
		tab_container->set_current_tab(idx);
	} else {
		_update_selected_editor_menu();
	}

	_update_history_arrows();

	_update_script_names();
	_update_members_overview_visibility();
	_update_help_overview_visibility();
	_save_layout();
}

void EditorScriptEditor::_close_current_tab(bool p_save) {
	_close_tab(tab_container->get_current_tab(), p_save);
}

void EditorScriptEditor::_close_discard_current_tab(const String &p_str) {
	_close_tab(tab_container->get_current_tab(), false);
	erase_tab_confirm->hide();
}

void EditorScriptEditor::_close_docs_tab() {
	int child_count = tab_container->get_child_count();
	for (int i = child_count - 1; i >= 0; i--) {
		EditorHelp *se = Object::cast_to<EditorHelp>(tab_container->get_child(i));

		if (se) {
			_close_tab(i, true, false);
		}
	}
}

void EditorScriptEditor::_copy_script_path() {
	EditorScriptEditorBase *se = _get_current_editor();
	if (se) {
		RES script = se->get_edited_resource();
		OS::get_singleton()->set_clipboard(script->get_path());
	}
}

void EditorScriptEditor::_close_other_tabs() {
	int current_idx = tab_container->get_current_tab();
	for (int i = tab_container->get_child_count() - 1; i >= 0; i--) {
		if (i != current_idx) {
			script_close_queue.push_back(i);
		}
	}
	_queue_close_tabs();
}

void EditorScriptEditor::_close_all_tabs() {
	for (int i = tab_container->get_child_count() - 1; i >= 0; i--) {
		script_close_queue.push_back(i);
	}
	_queue_close_tabs();
}

void EditorScriptEditor::_queue_close_tabs() {
	while (!script_close_queue.empty()) {
		int idx = script_close_queue.front()->get();
		script_close_queue.pop_front();

		tab_container->set_current_tab(idx);
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(idx));
		if (se) {
			// Maybe there are unsaved changes.
			if (se->is_unsaved()) {
				_ask_close_current_unsaved_tab(se);
				erase_tab_confirm->connect(SceneStringNames::get_singleton()->visibility_changed, this, "_queue_close_tabs", varray(), CONNECT_ONESHOT | CONNECT_DEFERRED);
				break;
			}
		}

		_close_current_tab(false);
	}
}

void EditorScriptEditor::_ask_close_current_unsaved_tab(EditorScriptEditorBase *current) {
	erase_tab_confirm->set_text(TTR("Close and save changes?") + "\n\"" + current->get_name() + "\"");
	erase_tab_confirm->popup_centered_minsize();
}

void EditorScriptEditor::_resave_scripts(const String &p_str) {
	apply_scripts();

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		RES script = se->get_edited_resource();

		if (script->is_built_in()) {
			continue; //internal script, who cares
		}

		if (trim_trailing_whitespace_on_save) {
			se->trim_trailing_whitespace();
		}

		se->insert_final_newline();

		if (convert_indent_on_save) {
			if (use_space_indentation) {
				se->convert_indent_to_spaces();
			} else {
				se->convert_indent_to_tabs();
			}
		}

		Ref<TextFile> text_file = script;
		if (text_file != nullptr) {
			se->apply_code();
			_save_text_file(text_file, text_file->get_path());
			break;
		} else {
			editor->save_resource(script);
		}
		se->tag_saved_version();
	}

	disk_changed->hide();
}

void EditorScriptEditor::reload_scripts() {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		RES edited_res = se->get_edited_resource();

		if (edited_res->is_built_in()) {
			continue; //internal script, who cares
		}

		uint64_t last_date = edited_res->get_last_modified_time();
		uint64_t date = FileAccess::get_modified_time(edited_res->get_path());

		if (last_date == date) {
			continue;
		}

		Ref<Script> script = edited_res;
		if (script != nullptr) {
			Ref<Script> rel_script = ResourceLoader::load(script->get_path(), script->get_class(), true);
			ERR_CONTINUE(!rel_script.is_valid());
			script->set_source_code(rel_script->get_source_code());
			script->set_last_modified_time(rel_script->get_last_modified_time());
			script->reload();
		}

		Ref<TextFile> text_file = edited_res;
		if (text_file != nullptr) {
			Error err;
			Ref<TextFile> rel_text_file = _load_text_file(text_file->get_path(), &err);
			ERR_CONTINUE(!rel_text_file.is_valid());
			text_file->set_text(rel_text_file->get_text());
			text_file->set_last_modified_time(rel_text_file->get_last_modified_time());
		}
		se->reload_text();
	}

	disk_changed->hide();
	_update_script_names();
}

void EditorScriptEditor::_res_saved_callback(const Ref<Resource> &p_res) {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		RES script = se->get_edited_resource();

		if (script == p_res) {
			se->tag_saved_version();
		}
	}

	_update_script_names();
	_trigger_live_script_reload();
}

void EditorScriptEditor::_scene_saved_callback(const String &p_path) {
	// If scene was saved, mark all built-in scripts from that scene as saved.
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		RES edited_res = se->get_edited_resource();

		if (!edited_res->get_path().empty() && edited_res->get_path().find("::") == -1) {
			continue; // External script, who cares.
		}

		if (edited_res->get_path().get_slice("::", 0) == p_path) {
			se->tag_saved_version();
		}

		Ref<Script> scr = edited_res;
		if (scr.is_valid() && scr->is_tool()) {
			scr->reload(true);
		}
	}
}

void EditorScriptEditor::_trigger_live_script_reload() {
	if (!pending_auto_reload && auto_reload_running_scripts) {
		call_deferred("_live_auto_reload_running_scripts");
		pending_auto_reload = true;
	}
}

void EditorScriptEditor::_live_auto_reload_running_scripts() {
	pending_auto_reload = false;
	debugger->reload_scripts();
}

bool EditorScriptEditor::_test_script_times_on_disk(RES p_for_script) {
	disk_changed_list->clear();
	TreeItem *r = disk_changed_list->create_item();
	disk_changed_list->set_hide_root(true);

	bool need_ask = false;
	bool need_reload = false;
	bool use_autoreload = bool(EDITOR_DEF("text_editor/files/auto_reload_scripts_on_external_change", false));

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (se) {
			RES edited_res = se->get_edited_resource();
			if (p_for_script.is_valid() && edited_res.is_valid() && p_for_script != edited_res) {
				continue;
			}

			if (edited_res->is_built_in()) {
				continue; //internal script, who cares
			}

			uint64_t last_date = edited_res->get_last_modified_time();
			uint64_t date = FileAccess::get_modified_time(edited_res->get_path());

			if (last_date != date) {
				TreeItem *ti = disk_changed_list->create_item(r);
				ti->set_text(0, edited_res->get_path().get_file());

				if (!use_autoreload || se->is_unsaved()) {
					need_ask = true;
				}
				need_reload = true;
			}
		}
	}

	if (need_reload) {
		if (!need_ask) {
			script_editor->reload_scripts();
			need_reload = false;
		} else {
			disk_changed->call_deferred("popup_centered_ratio", 0.5);
		}
	}

	return need_reload;
}

void EditorScriptEditor::_file_dialog_action(String p_file) {
	switch (file_dialog_option) {
		case FILE_NEW_TEXTFILE: {
			Error err;
			FileAccess *file = FileAccess::open(p_file, FileAccess::WRITE, &err);
			if (err) {
				editor->show_warning(TTR("Error writing TextFile:") + "\n" + p_file, TTR("Error!"));
				break;
			}
			file->close();
			memdelete(file);
			FALLTHROUGH;
		}
		case FILE_OPEN: {
			List<String> extensions;
			ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);
			if (extensions.find(p_file.get_extension())) {
				Ref<Script> scr = ResourceLoader::load(p_file);
				if (!scr.is_valid()) {
					editor->show_warning(TTR("Could not load file at:") + "\n\n" + p_file, TTR("Error!"));
					file_dialog_option = -1;
					return;
				}

				edit(scr);
				file_dialog_option = -1;
				return;
			}

			Error error;
			Ref<TextFile> text_file = _load_text_file(p_file, &error);
			if (error != OK) {
				editor->show_warning(TTR("Could not load file at:") + "\n\n" + p_file, TTR("Error!"));
			}

			if (text_file.is_valid()) {
				edit(text_file);
				file_dialog_option = -1;
				return;
			}
		} break;
		case FILE_SAVE_AS: {
			EditorScriptEditorBase *current = _get_current_editor();
			if (current) {
				RES resource = current->get_edited_resource();
				String path = ProjectSettings::get_singleton()->localize_path(p_file);
				Error err = _save_text_file(resource, path);

				if (err != OK) {
					editor->show_accept(TTR("Error saving file!"), TTR("OK"));
					return;
				}

				resource->set_path(path);
				_update_script_names();
			}
		} break;
		case THEME_SAVE_AS: {
			if (!EditorSettings::get_singleton()->save_text_editor_theme_as(p_file)) {
				editor->show_warning(TTR("Error while saving theme."), TTR("Error Saving"));
			}
		} break;
		case THEME_IMPORT: {
			if (!EditorSettings::get_singleton()->import_text_editor_theme(p_file)) {
				editor->show_warning(TTR("Error importing theme."), TTR("Error Importing"));
			}
		} break;
	}
	file_dialog_option = -1;
}

Ref<Script> EditorScriptEditor::_get_current_script() {
	EditorScriptEditorBase *current = _get_current_editor();

	if (current) {
		Ref<Script> script = current->get_edited_resource();
		return script != nullptr ? script : nullptr;
	} else {
		return nullptr;
	}
}

Array EditorScriptEditor::_get_open_scripts() const {
	Array ret;
	Vector<Ref<Script>> scripts = get_open_scripts();
	int scrits_amount = scripts.size();
	for (int idx_script = 0; idx_script < scrits_amount; idx_script++) {
		ret.push_back(scripts[idx_script]);
	}
	return ret;
}

bool EditorScriptEditor::toggle_scripts_panel() {
	list_split->set_visible(!list_split->is_visible());
	EditorSettings::get_singleton()->set_project_metadata("scripts_panel", "show_scripts_panel", list_split->is_visible());
	return list_split->is_visible();
}

bool EditorScriptEditor::is_scripts_panel_toggled() {
	return list_split->is_visible();
}

void EditorScriptEditor::_menu_option(int p_option) {
	EditorScriptEditorBase *current = _get_current_editor();
	switch (p_option) {
		case FILE_NEW: {
			script_create_dialog->config("Node", "new_script", false, false);
			script_create_dialog->popup_centered();
		} break;
		case FILE_NEW_TEXTFILE: {
			file_dialog->set_mode(EditorFileDialog::MODE_SAVE_FILE);
			file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
			file_dialog_option = FILE_NEW_TEXTFILE;

			file_dialog->clear_filters();
			file_dialog->popup_centered_ratio();
			file_dialog->set_title(TTR("New Text File..."));
		} break;
		case FILE_OPEN: {
			file_dialog->set_mode(EditorFileDialog::MODE_OPEN_FILE);
			file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
			file_dialog_option = FILE_OPEN;

			List<String> extensions;
			ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);
			file_dialog->clear_filters();
			for (int i = 0; i < extensions.size(); i++) {
				file_dialog->add_filter("*." + extensions[i] + " ; " + extensions[i].to_upper());
			}

			file_dialog->popup_centered_ratio();
			file_dialog->set_title(TTR("Open File"));
			return;
		} break;
		case FILE_REOPEN_CLOSED: {
			if (previous_scripts.empty()) {
				return;
			}

			String path = previous_scripts.back()->get();
			previous_scripts.pop_back();

			List<String> extensions;
			ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);
			bool built_in = !path.is_resource_file();

			if (extensions.find(path.get_extension()) || built_in) {
				if (built_in) {
					String res_path = path.get_slice("::", 0);
					if (ResourceLoader::get_resource_type(res_path) == "PackedScene") {
						if (!EditorNode::get_singleton()->is_scene_open(res_path)) {
							EditorNode::get_singleton()->load_scene(res_path);
							script_editor->call_deferred("_menu_option", p_option);
							previous_scripts.push_back(path); //repeat the operation
							return;
						}
					} else {
						EditorNode::get_singleton()->load_resource(res_path);
					}
				}

				Ref<Script> scr = ResourceLoader::load(path);
				if (!scr.is_valid()) {
					editor->show_warning(TTR("Could not load file at:") + "\n\n" + path, TTR("Error!"));
					file_dialog_option = -1;
					return;
				}

				edit(scr);
				file_dialog_option = -1;
				return;
			} else {
				Error error;
				Ref<TextFile> text_file = _load_text_file(path, &error);
				if (error != OK) {
					editor->show_warning(TTR("Could not load file at:") + "\n\n" + path, TTR("Error!"));
				}

				if (text_file.is_valid()) {
					edit(text_file);
					file_dialog_option = -1;
					return;
				}
			}
		} break;
		case FILE_SAVE_ALL: {
			if (_test_script_times_on_disk()) {
				return;
			}

			save_all_scripts();
		} break;
		case SEARCH_IN_FILES: {
			_on_find_in_files_requested("");
		} break;
		case REPLACE_IN_FILES: {
			_on_replace_in_files_requested("");
		} break;
		case SEARCH_HELP: {
			help_search_dialog->popup_dialog();
		} break;
		case WINDOW_NEXT: {
			_history_forward();
		} break;
		case WINDOW_PREV: {
			_history_back();
		} break;
		case WINDOW_SORT: {
			_sort_list_on_update = true;
			_update_script_names();
		} break;
		case DEBUG_KEEP_DEBUGGER_OPEN: {
			bool ischecked = debug_menu->get_popup()->is_item_checked(debug_menu->get_popup()->get_item_index(DEBUG_KEEP_DEBUGGER_OPEN));
			if (debugger) {
				debugger->set_hide_on_stop(ischecked);
			}
			debug_menu->get_popup()->set_item_checked(debug_menu->get_popup()->get_item_index(DEBUG_KEEP_DEBUGGER_OPEN), !ischecked);
			EditorSettings::get_singleton()->set_project_metadata("debug_options", "keep_debugger_open", !ischecked);
		} break;
		case DEBUG_WITH_EXTERNAL_EDITOR: {
			bool ischecked = debug_menu->get_popup()->is_item_checked(debug_menu->get_popup()->get_item_index(DEBUG_WITH_EXTERNAL_EDITOR));
			if (debugger) {
				debugger->set_debug_with_external_editor(!ischecked);
			}
			debug_menu->get_popup()->set_item_checked(debug_menu->get_popup()->get_item_index(DEBUG_WITH_EXTERNAL_EDITOR), !ischecked);
			EditorSettings::get_singleton()->set_project_metadata("debug_options", "debug_with_external_editor", !ischecked);
		} break;
		case TOGGLE_SCRIPTS_PANEL: {
			if (current) {
				EditorScriptTextEditor *editor = Object::cast_to<EditorScriptTextEditor>(current);
				toggle_scripts_panel();
				if (editor) {
					editor->update_toggle_scripts_button();
				}
			} else {
				toggle_scripts_panel();
			}
		}
	}

	if (current) {
		switch (p_option) {
			case FILE_SAVE: {
				save_current_script();
			} break;
			case FILE_SAVE_AS: {
				if (trim_trailing_whitespace_on_save) {
					current->trim_trailing_whitespace();
				}

				current->insert_final_newline();

				if (convert_indent_on_save) {
					if (use_space_indentation) {
						current->convert_indent_to_spaces();
					} else {
						current->convert_indent_to_tabs();
					}
				}

				RES resource = current->get_edited_resource();
				Ref<TextFile> text_file = resource;
				if (text_file != nullptr) {
					file_dialog->set_mode(EditorFileDialog::MODE_SAVE_FILE);
					file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
					file_dialog_option = FILE_SAVE_AS;

					List<String> extensions;
					ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);
					file_dialog->clear_filters();
					file_dialog->set_current_dir(text_file->get_path().get_base_dir());
					file_dialog->set_current_file(text_file->get_path().get_file());
					file_dialog->popup_centered_ratio();
					file_dialog->set_title(TTR("Save File As..."));
					break;
				}

				editor->push_item(resource.ptr());
				editor->save_resource_as(resource);

			} break;

			case FILE_TOOL_RELOAD:
			case FILE_TOOL_RELOAD_SOFT: {
				current->reload(p_option == FILE_TOOL_RELOAD_SOFT);

			} break;
			case FILE_RUN: {
				Ref<Script> scr = current->get_edited_resource();
				if (scr == nullptr || scr.is_null()) {
					EditorNode::get_singleton()->show_warning(TTR("Can't obtain the script for running."));
					break;
				}

				current->apply_code();
				Error err = scr->reload(false); //hard reload script before running always

				if (err != OK) {
					EditorNode::get_singleton()->show_warning(TTR("Script failed reloading, check console for errors."));
					return;
				}
				if (!scr->is_tool()) {
					EditorNode::get_singleton()->show_warning(TTR("Script is not in tool mode, will not be able to run."));
					return;
				}

				if (!ClassDB::is_parent_class(scr->get_instance_base_type(), "EditorScript")) {
					EditorNode::get_singleton()->show_warning(TTR("To run this script, it must inherit EditorScript and be set to tool mode."));
					return;
				}

				Ref<EditorScript> es = memnew(EditorScript);
				es->set_script(scr.get_ref_ptr());
				es->set_editor(EditorNode::get_singleton());

				es->_run();

				EditorNode::get_undo_redo()->clear_history();
			} break;
			case FILE_CLOSE: {
				if (current->is_unsaved()) {
					_ask_close_current_unsaved_tab(current);
				} else {
					_close_current_tab(false);
				}
			} break;
			case FILE_COPY_PATH: {
				_copy_script_path();
			} break;
			case SHOW_IN_FILE_SYSTEM: {
				const RES script = current->get_edited_resource();
				String path = script->get_path();
				if (!path.empty()) {
					if (path.find("::") != -1) { // Built-in.
						path = path.get_slice("::", 0); // Show the scene instead.
					}

					FileSystemDock *file_system_dock = EditorNode::get_singleton()->get_filesystem_dock();
					file_system_dock->navigate_to_path(path);
					file_system_dock->ensure_visible();
				}
			} break;
			case CLOSE_DOCS: {
				_close_docs_tab();
			} break;
			case CLOSE_OTHER_TABS: {
				_close_other_tabs();
			} break;
			case CLOSE_ALL: {
				_close_all_tabs();
			} break;
			case DEBUG_NEXT: {
				if (debugger) {
					debugger->debug_next();
				}
			} break;
			case DEBUG_STEP: {
				if (debugger) {
					debugger->debug_step();
				}

			} break;
			case DEBUG_BREAK: {
				if (debugger) {
					debugger->debug_break();
				}

			} break;
			case DEBUG_CONTINUE: {
				if (debugger) {
					debugger->debug_continue();
				}

			} break;
			case WINDOW_MOVE_UP: {
				if (tab_container->get_current_tab() > 0) {
					tab_container->move_child(current, tab_container->get_current_tab() - 1);
					tab_container->set_current_tab(tab_container->get_current_tab() - 1);
					_update_script_names();
				}
			} break;
			case WINDOW_MOVE_DOWN: {
				if (tab_container->get_current_tab() < tab_container->get_child_count() - 1) {
					tab_container->move_child(current, tab_container->get_current_tab() + 1);
					tab_container->set_current_tab(tab_container->get_current_tab() + 1);
					_update_script_names();
				}
			} break;
			default: {
				if (p_option >= WINDOW_SELECT_BASE) {
					tab_container->set_current_tab(p_option - WINDOW_SELECT_BASE);
					_update_script_names();
				}
			}
		}
	} else {
		EditorHelp *help = Object::cast_to<EditorHelp>(tab_container->get_current_tab_control());
		if (help) {
			switch (p_option) {
				case HELP_SEARCH_FIND: {
					help->popup_search();
				} break;
				case HELP_SEARCH_FIND_NEXT: {
					help->search_again();
				} break;
				case HELP_SEARCH_FIND_PREVIOUS: {
					help->search_again(true);
				} break;
				case FILE_CLOSE: {
					_close_current_tab();
				} break;
				case CLOSE_DOCS: {
					_close_docs_tab();
				} break;
				case CLOSE_OTHER_TABS: {
					_close_other_tabs();
				} break;
				case CLOSE_ALL: {
					_close_all_tabs();
				} break;
				case WINDOW_MOVE_UP: {
					if (tab_container->get_current_tab() > 0) {
						tab_container->move_child(help, tab_container->get_current_tab() - 1);
						tab_container->set_current_tab(tab_container->get_current_tab() - 1);
						_update_script_names();
					}
				} break;
				case WINDOW_MOVE_DOWN: {
					if (tab_container->get_current_tab() < tab_container->get_child_count() - 1) {
						tab_container->move_child(help, tab_container->get_current_tab() + 1);
						tab_container->set_current_tab(tab_container->get_current_tab() + 1);
						_update_script_names();
					}
				} break;
			}
		}
	}
}

void EditorScriptEditor::_update_debug_options() {
	bool keep_debugger_open = EditorSettings::get_singleton()->get_project_metadata("debug_options", "keep_debugger_open", false);
	bool debug_with_external_editor = EditorSettings::get_singleton()->get_project_metadata("debug_options", "debug_with_external_editor", false);

	if (keep_debugger_open) {
		_menu_option(DEBUG_KEEP_DEBUGGER_OPEN);
	}
	if (debug_with_external_editor) {
		_menu_option(DEBUG_WITH_EXTERNAL_EDITOR);
	}
}

void EditorScriptEditor::_theme_option(int p_option) {
	switch (p_option) {
		case THEME_IMPORT: {
			file_dialog->set_mode(EditorFileDialog::MODE_OPEN_FILE);
			file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
			file_dialog_option = THEME_IMPORT;
			file_dialog->clear_filters();
			file_dialog->add_filter("*.tet");
			file_dialog->popup_centered_ratio();
			file_dialog->set_title(TTR("Import Theme"));
		} break;
		case THEME_RELOAD: {
			EditorSettings::get_singleton()->load_text_editor_theme();
		} break;
		case THEME_SAVE: {
			if (EditorSettings::get_singleton()->is_default_text_editor_theme()) {
				EditorScriptEditor::_show_save_theme_as_dialog();
			} else if (!EditorSettings::get_singleton()->save_text_editor_theme()) {
				editor->show_warning(TTR("Error while saving theme"), TTR("Error saving"));
			}
		} break;
		case THEME_SAVE_AS: {
			EditorScriptEditor::_show_save_theme_as_dialog();
		} break;
	}
}

void EditorScriptEditor::_show_save_theme_as_dialog() {
	file_dialog->set_mode(EditorFileDialog::MODE_SAVE_FILE);
	file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_dialog_option = THEME_SAVE_AS;
	file_dialog->clear_filters();
	file_dialog->add_filter("*.tet");
	file_dialog->set_current_path(EditorSettings::get_singleton()->get_text_editor_themes_dir().plus_file(EditorSettings::get_singleton()->get("text_editor/theme/color_theme")));
	file_dialog->popup_centered_ratio();
	file_dialog->set_title(TTR("Save Theme As..."));
}

bool EditorScriptEditor::_has_docs_tab() const {
	const int child_count = tab_container->get_child_count();
	for (int i = 0; i < child_count; i++) {
		if (Object::cast_to<EditorHelp>(tab_container->get_child(i))) {
			return true;
		}
	}
	return false;
}

bool EditorScriptEditor::_has_script_tab() const {
	const int child_count = tab_container->get_child_count();
	for (int i = 0; i < child_count; i++) {
		if (Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i))) {
			return true;
		}
	}
	return false;
}

void EditorScriptEditor::_prepare_file_menu() {
	PopupMenu *menu = file_menu->get_popup();
	const bool current_is_doc = _get_current_editor() == nullptr;

	menu->set_item_disabled(menu->get_item_index(FILE_REOPEN_CLOSED), previous_scripts.empty());

	menu->set_item_disabled(menu->get_item_index(FILE_SAVE), current_is_doc);
	menu->set_item_disabled(menu->get_item_index(FILE_SAVE_AS), current_is_doc);
	menu->set_item_disabled(menu->get_item_index(FILE_SAVE_ALL), !_has_script_tab());

	menu->set_item_disabled(menu->get_item_index(FILE_TOOL_RELOAD_SOFT), current_is_doc);
	menu->set_item_disabled(menu->get_item_index(FILE_COPY_PATH), current_is_doc);
	menu->set_item_disabled(menu->get_item_index(SHOW_IN_FILE_SYSTEM), current_is_doc);

	menu->set_item_disabled(menu->get_item_index(WINDOW_PREV), history_pos <= 0);
	menu->set_item_disabled(menu->get_item_index(WINDOW_NEXT), history_pos >= history.size() - 1);

	menu->set_item_disabled(menu->get_item_index(FILE_CLOSE), tab_container->get_child_count() < 1);
	menu->set_item_disabled(menu->get_item_index(CLOSE_ALL), tab_container->get_child_count() < 1);
	menu->set_item_disabled(menu->get_item_index(CLOSE_OTHER_TABS), tab_container->get_child_count() <= 1);
	menu->set_item_disabled(menu->get_item_index(CLOSE_DOCS), !_has_docs_tab());

	menu->set_item_disabled(menu->get_item_index(FILE_RUN), current_is_doc);
}

void EditorScriptEditor::_tab_changed(int p_which) {
	ensure_select_current();
}

void EditorScriptEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			editor->connect("play_pressed", this, "_editor_play");
			editor->connect("pause_pressed", this, "_editor_pause");
			editor->connect("stop_pressed", this, "_editor_stop");
			editor->connect("script_add_function_request", this, "_add_callback");
			editor->connect("resource_saved", this, "_res_saved_callback");
			editor->connect("scene_saved", this, "_scene_saved_callback");
			script_list->connect("item_selected", this, "_script_selected");

			members_overview->connect("item_selected", this, "_members_overview_selected");
			help_overview->connect("item_selected", this, "_help_overview_selected");
			script_split->connect("dragged", this, "_script_split_dragged");

			EditorSettings::get_singleton()->connect("settings_changed", this, "_editor_settings_changed");
			FALLTHROUGH;
		}
		case NOTIFICATION_THEME_CHANGED: {
			help_search->set_icon(get_theme_icon("HelpSearch", "EditorIcons"));

			script_forward->set_icon(get_theme_icon("Forward", "EditorIcons"));
			script_back->set_icon(get_theme_icon("Back", "EditorIcons"));

			members_overview_alphabeta_sort_button->set_icon(get_theme_icon("Sort", "EditorIcons"));

			filter_scripts->set_right_icon(get_theme_icon("Search", "EditorIcons"));
			filter_methods->set_right_icon(get_theme_icon("Search", "EditorIcons"));

			filename->add_theme_style_override("normal", editor->get_gui_base()->get_theme_stylebox("normal", "LineEdit"));

			recent_scripts->set_as_minsize();
		} break;

		case NOTIFICATION_READY: {
			get_tree()->connect("tree_changed", this, "_tree_changed");
			editor->get_inspector_dock()->connect("request_help", this, "_request_help");
			editor->connect("request_help_search", this, "_help_search");

			_update_debug_options();
		} break;

		case NOTIFICATION_EXIT_TREE: {
			editor->disconnect("play_pressed", this, "_editor_play");
			editor->disconnect("pause_pressed", this, "_editor_pause");
			editor->disconnect("stop_pressed", this, "_editor_stop");
		} break;

		case MainLoop::NOTIFICATION_WM_FOCUS_IN: {
			_test_script_times_on_disk();
			_update_modified_scripts_for_external_editor();
		} break;

		case CanvasItem::NOTIFICATION_VISIBILITY_CHANGED: {
			if (is_visible()) {
				find_in_files_button->show();
			} else {
				if (find_in_files->is_visible_in_tree()) {
					editor->hide_bottom_panel();
				}
				find_in_files_button->hide();
			}

		} break;

		default:
			break;
	}
}

bool EditorScriptEditor::can_take_away_focus() const {
	EditorScriptEditorBase *current = _get_current_editor();
	if (current) {
		return current->can_lose_focus_on_node_selection();
	} else {
		return true;
	}
}

void EditorScriptEditor::close_builtin_scripts_from_scene(const String &p_scene) {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));

		if (se) {
			Ref<Script> script = se->get_edited_resource();
			if (script == nullptr || !script.is_valid()) {
				continue;
			}

			if (script->is_built_in() && script->get_path().begins_with(p_scene)) { //is an internal script and belongs to scene being closed
				_close_tab(i, false);
				i--;
			}
		}
	}
}

void EditorScriptEditor::edited_scene_changed() {
	_update_modified_scripts_for_external_editor();
}

void EditorScriptEditor::notify_script_close(const Ref<Script> &p_script) {
	emit_signal("script_close", p_script);
}

void EditorScriptEditor::notify_script_changed(const Ref<Script> &p_script) {
	emit_signal("editor_script_changed", p_script);
}

void EditorScriptEditor::get_breakpoints(List<String> *p_breakpoints) {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		Ref<Script> script = se->get_edited_resource();
		if (script == nullptr) {
			continue;
		}

		List<int> bpoints;
		se->get_breakpoints(&bpoints);
		String base = script->get_path();
		if (base.begins_with("local://") || base == "") {
			continue;
		}

		for (List<int>::Element *E = bpoints.front(); E; E = E->next()) {
			p_breakpoints->push_back(base + ":" + itos(E->get() + 1));
		}
	}
}

void EditorScriptEditor::ensure_focus_current() {
	if (!is_inside_tree()) {
		return;
	}

	EditorScriptEditorBase *current = _get_current_editor();
	if (current) {
		current->ensure_focus();
	}
}

void EditorScriptEditor::_members_overview_selected(int p_idx) {
	EditorScriptEditorBase *se = _get_current_editor();
	if (!se) {
		return;
	}
	// Go to the member's line and reset the cursor column. We can't change scroll_position
	// directly until we have gone to the line first, since code might be folded.
	se->goto_line(members_overview->get_item_metadata(p_idx));
	Dictionary state = se->get_edit_state();
	state["column"] = 0;
	state["scroll_position"] = members_overview->get_item_metadata(p_idx);
	se->set_edit_state(state);
}

void EditorScriptEditor::_help_overview_selected(int p_idx) {
	Node *current = tab_container->get_child(tab_container->get_current_tab());
	EditorHelp *se = Object::cast_to<EditorHelp>(current);
	if (!se) {
		return;
	}
	se->scroll_to_section(help_overview->get_item_metadata(p_idx));
}

void EditorScriptEditor::_script_selected(int p_idx) {
	grab_focus_block = !Input::get_singleton()->is_mouse_button_pressed(1); //amazing hack, simply amazing

	_go_to_tab(script_list->get_item_metadata(p_idx));
	grab_focus_block = false;
}

void EditorScriptEditor::ensure_select_current() {
	if (tab_container->get_child_count() && tab_container->get_current_tab() >= 0) {
		EditorScriptEditorBase *se = _get_current_editor();
		if (se) {
			se->enable_editor();

			if (!grab_focus_block && is_visible_in_tree()) {
				se->ensure_focus();
			}
		}
	}

	_update_selected_editor_menu();
}

void EditorScriptEditor::_find_scripts(Node *p_base, Node *p_current, RBSet<Ref<Script>> &used) {
	if (p_current != p_base && p_current->get_owner() != p_base) {
		return;
	}

	if (p_current->get_script_instance()) {
		Ref<Script> scr = p_current->get_script();
		if (scr.is_valid()) {
			used.insert(scr);
		}
	}

	for (int i = 0; i < p_current->get_child_count(); i++) {
		_find_scripts(p_base, p_current->get_child(i), used);
	}
}

struct _EditorScriptEditorItemData {
	String name;
	String sort_key;
	Ref<Texture> icon;
	int index;
	String tooltip;
	bool used;
	int category;
	Node *ref;

	bool operator<(const _EditorScriptEditorItemData &id) const {
		if (category == id.category) {
			if (sort_key == id.sort_key) {
				return index < id.index;
			} else {
				return sort_key < id.sort_key;
			}
		} else {
			return category < id.category;
		}
	}
};

void EditorScriptEditor::_update_members_overview_visibility() {
	EditorScriptEditorBase *se = _get_current_editor();
	if (!se) {
		members_overview_alphabeta_sort_button->set_visible(false);
		members_overview->set_visible(false);
		overview_vbox->set_visible(false);
		return;
	}

	if (members_overview_enabled && se->show_members_overview()) {
		members_overview_alphabeta_sort_button->set_visible(true);
		members_overview->set_visible(true);
		overview_vbox->set_visible(true);
	} else {
		members_overview_alphabeta_sort_button->set_visible(false);
		members_overview->set_visible(false);
		overview_vbox->set_visible(false);
	}
}

void EditorScriptEditor::_toggle_members_overview_alpha_sort(bool p_alphabetic_sort) {
	EditorSettings::get_singleton()->set("text_editor/tools/sort_members_outline_alphabetically", p_alphabetic_sort);
	_update_members_overview();
}

void EditorScriptEditor::_update_members_overview() {
	members_overview->clear();

	EditorScriptEditorBase *se = _get_current_editor();
	if (!se) {
		return;
	}

	Vector<String> functions = se->get_functions();
	if (EditorSettings::get_singleton()->get("text_editor/tools/sort_members_outline_alphabetically")) {
		functions.sort();
	}

	for (int i = 0; i < functions.size(); i++) {
		String filter = filter_methods->get_text();
		String name = functions[i].get_slice(":", 0);
		if (filter == "" || filter.is_subsequence_ofi(name)) {
			members_overview->add_item(name);
			members_overview->set_item_metadata(members_overview->get_item_count() - 1, functions[i].get_slice(":", 1).to_int() - 1);
		}
	}

	String path = se->get_edited_resource()->get_path();
	bool built_in = !path.is_resource_file();
	String name = built_in ? path.get_file() : se->get_name();
	filename->set_text(name);
}

void EditorScriptEditor::_update_help_overview_visibility() {
	int selected = tab_container->get_current_tab();
	if (selected < 0 || selected >= tab_container->get_child_count()) {
		help_overview->set_visible(false);
		return;
	}

	Node *current = tab_container->get_child(tab_container->get_current_tab());
	EditorHelp *se = Object::cast_to<EditorHelp>(current);
	if (!se) {
		help_overview->set_visible(false);
		return;
	}

	if (help_overview_enabled) {
		members_overview_alphabeta_sort_button->set_visible(false);
		help_overview->set_visible(true);
		overview_vbox->set_visible(true);
		filename->set_text(se->get_name());
	} else {
		help_overview->set_visible(false);
		overview_vbox->set_visible(false);
	}
}

void EditorScriptEditor::_update_help_overview() {
	help_overview->clear();

	int selected = tab_container->get_current_tab();
	if (selected < 0 || selected >= tab_container->get_child_count()) {
		return;
	}

	Node *current = tab_container->get_child(tab_container->get_current_tab());
	EditorHelp *se = Object::cast_to<EditorHelp>(current);
	if (!se) {
		return;
	}

	Vector<Pair<String, int>> sections = se->get_sections();
	for (int i = 0; i < sections.size(); i++) {
		help_overview->add_item(sections[i].first);
		help_overview->set_item_metadata(i, sections[i].second);
	}
}

void EditorScriptEditor::_update_script_colors() {
	bool script_temperature_enabled = EditorSettings::get_singleton()->get("text_editor/script_list/script_temperature_enabled");
	bool highlight_current = EditorSettings::get_singleton()->get("text_editor/script_list/highlight_current_script");

	int hist_size = EditorSettings::get_singleton()->get("text_editor/script_list/script_temperature_history_size");
	Color hot_color = get_theme_color("accent_color", "Editor");
	Color cold_color = get_theme_color("font_color", "Editor");

	for (int i = 0; i < script_list->get_item_count(); i++) {
		int c = script_list->get_item_metadata(i);
		Node *n = tab_container->get_child(c);
		if (!n) {
			continue;
		}

		script_list->set_item_custom_bg_color(i, Color(0, 0, 0, 0));

		bool current = tab_container->get_current_tab() == c;
		if (current && highlight_current) {
			script_list->set_item_custom_bg_color(i, EditorSettings::get_singleton()->get("text_editor/script_list/current_script_background_color"));

		} else if (script_temperature_enabled) {
			if (!n->has_meta("__editor_pass")) {
				continue;
			}

			int pass = n->get_meta("__editor_pass");
			int h = edit_pass - pass;
			if (h > hist_size) {
				continue;
			}
			int non_zero_hist_size = (hist_size == 0) ? 1 : hist_size;
			float v = Math::ease((edit_pass - pass) / float(non_zero_hist_size), 0.4);

			script_list->set_item_custom_fg_color(i, hot_color.linear_interpolate(cold_color, v));
		}
	}
}

void EditorScriptEditor::_update_script_names() {
	if (restoring_layout) {
		return;
	}

	RBSet<Ref<Script>> used;
	Node *edited = EditorNode::get_singleton()->get_edited_scene();
	if (edited) {
		_find_scripts(edited, edited, used);
	}

	script_list->clear();
	bool split_script_help = EditorSettings::get_singleton()->get("text_editor/script_list/group_help_pages");
	ScriptSortBy sort_by = (ScriptSortBy)(int)EditorSettings::get_singleton()->get("text_editor/script_list/sort_scripts_by");
	ScriptListName display_as = (ScriptListName)(int)EditorSettings::get_singleton()->get("text_editor/script_list/list_script_names_as");

	Vector<_EditorScriptEditorItemData> sedata;

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (se) {
			Ref<Texture> icon = se->get_icon();
			String path = se->get_edited_resource()->get_path();
			String name = se->get_name();

			_EditorScriptEditorItemData sd;
			sd.icon = icon;
			sd.name = name;
			sd.tooltip = path;
			sd.index = i;
			sd.used = used.has(se->get_edited_resource());
			sd.category = 0;
			sd.ref = se;

			switch (sort_by) {
				case SORT_BY_NAME: {
					sd.sort_key = name.to_lower();
				} break;
				case SORT_BY_PATH: {
					sd.sort_key = path;
				} break;
				case SORT_BY_NONE: {
					sd.sort_key = "";
				} break;
			}

			switch (display_as) {
				case DISPLAY_NAME: {
					sd.name = name;
				} break;
				case DISPLAY_DIR_AND_NAME: {
					if (!path.get_base_dir().get_file().empty()) {
						sd.name = path.get_base_dir().get_file().plus_file(name);
					} else {
						sd.name = name;
					}
				} break;
				case DISPLAY_FULL_PATH: {
					sd.name = path;
				} break;
			}

			sedata.push_back(sd);
		}

		Vector<String> disambiguated_script_names;
		Vector<String> full_script_paths;
		for (int j = 0; j < sedata.size(); j++) {
			String name = sedata[j].name.replace("(*)", "");
			ScriptListName script_display = (ScriptListName)(int)EditorSettings::get_singleton()->get("text_editor/script_list/list_script_names_as");
			switch (script_display) {
				case DISPLAY_NAME: {
					name = name.get_file();
				} break;
				case DISPLAY_DIR_AND_NAME: {
					name = name.get_base_dir().get_file().plus_file(name.get_file());
				} break;
				default:
					break;
			}

			disambiguated_script_names.push_back(name);
			full_script_paths.push_back(sedata[j].tooltip);
		}

		EditorNode::disambiguate_filenames(full_script_paths, disambiguated_script_names);

		for (int j = 0; j < sedata.size(); j++) {
			if (sedata[j].name.ends_with("(*)")) {
				sedata.write[j].name = disambiguated_script_names[j] + "(*)";
			} else {
				sedata.write[j].name = disambiguated_script_names[j];
			}
		}

		EditorHelp *eh = Object::cast_to<EditorHelp>(tab_container->get_child(i));
		if (eh) {
			String name = eh->get_class();
			Ref<Texture> icon = get_theme_icon("Help", "EditorIcons");
			String tooltip = vformat(TTR("%s Class Reference"), name);

			_EditorScriptEditorItemData sd;
			sd.icon = icon;
			sd.name = name;
			sd.sort_key = name.to_lower();
			sd.tooltip = tooltip;
			sd.index = i;
			sd.used = false;
			sd.category = split_script_help ? 1 : 0;
			sd.ref = eh;

			sedata.push_back(sd);
		}
	}

	if (_sort_list_on_update && !sedata.empty()) {
		sedata.sort();

		// change actual order of tab_container so that the order can be rearranged by user
		int cur_tab = tab_container->get_current_tab();
		int prev_tab = tab_container->get_previous_tab();
		int new_cur_tab = -1;
		int new_prev_tab = -1;
		for (int i = 0; i < sedata.size(); i++) {
			tab_container->move_child(sedata[i].ref, i);
			if (new_prev_tab == -1 && sedata[i].index == prev_tab) {
				new_prev_tab = i;
			}
			if (new_cur_tab == -1 && sedata[i].index == cur_tab) {
				new_cur_tab = i;
			}
			// Update index of sd entries for sorted order
			_EditorScriptEditorItemData sd = sedata[i];
			sd.index = i;
			sedata.set(i, sd);
		}
		tab_container->set_current_tab(new_prev_tab);
		tab_container->set_current_tab(new_cur_tab);
		_sort_list_on_update = false;
	}

	Vector<_EditorScriptEditorItemData> sedata_filtered;
	for (int i = 0; i < sedata.size(); i++) {
		String filter = filter_scripts->get_text();
		if (filter == "" || filter.is_subsequence_ofi(sedata[i].name)) {
			sedata_filtered.push_back(sedata[i]);
		}
	}

	for (int i = 0; i < sedata_filtered.size(); i++) {
		script_list->add_item(sedata_filtered[i].name, sedata_filtered[i].icon);
		int index = script_list->get_item_count() - 1;
		script_list->set_item_tooltip(index, sedata_filtered[i].tooltip);
		script_list->set_item_metadata(index, sedata_filtered[i].index); /* Saving as metadata the script's index in the tab container and not the filtered one */
		if (sedata_filtered[i].used) {
			script_list->set_item_custom_bg_color(index, Color(88 / 255.0, 88 / 255.0, 60 / 255.0));
		}
		if (tab_container->get_current_tab() == sedata_filtered[i].index) {
			script_list->select(index);
			script_name_label->set_text(sedata_filtered[i].name);
			script_icon->set_texture(sedata_filtered[i].icon);
			EditorScriptEditorBase *se = _get_current_editor();
			if (se) {
				se->enable_editor();
				_update_selected_editor_menu();
			}
		}
	}

	if (!waiting_update_names) {
		_update_members_overview();
		_update_help_overview();
	} else {
		waiting_update_names = false;
	}
	_update_members_overview_visibility();
	_update_help_overview_visibility();
	_update_script_colors();
}

void EditorScriptEditor::_update_script_connections() {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptTextEditor *ste = Object::cast_to<EditorScriptTextEditor>(tab_container->get_child(i));
		if (!ste) {
			continue;
		}
		ste->_update_connected_methods();
	}
}

Ref<TextFile> EditorScriptEditor::_load_text_file(const String &p_path, Error *r_error) {
	if (r_error) {
		*r_error = ERR_FILE_CANT_OPEN;
	}

	String local_path = ProjectSettings::get_singleton()->localize_path(p_path);
	String path = ResourceLoader::path_remap(local_path);

	TextFile *text_file = memnew(TextFile);
	Ref<TextFile> text_res(text_file);
	Error err = text_file->load_text(path);

	ERR_FAIL_COND_V_MSG(err != OK, RES(), "Cannot load text file '" + path + "'.");

	text_file->set_file_path(local_path);
	text_file->set_path(local_path, true);

	if (ResourceLoader::get_timestamp_on_load()) {
		text_file->set_last_modified_time(FileAccess::get_modified_time(path));
	}

	if (r_error) {
		*r_error = OK;
	}

	return text_res;
}

Error EditorScriptEditor::_save_text_file(Ref<TextFile> p_text_file, const String &p_path) {
	Ref<TextFile> sqscr = p_text_file;
	ERR_FAIL_COND_V(sqscr.is_null(), ERR_INVALID_PARAMETER);

	String source = sqscr->get_text();

	Error err;
	FileAccess *file = FileAccess::open(p_path, FileAccess::WRITE, &err);

	ERR_FAIL_COND_V_MSG(err, err, "Cannot save text file '" + p_path + "'.");

	file->store_string(source);
	if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
		memdelete(file);
		return ERR_CANT_CREATE;
	}
	file->close();
	memdelete(file);

	if (ResourceSaver::get_timestamp_on_save()) {
		p_text_file->set_last_modified_time(FileAccess::get_modified_time(p_path));
	}

	_res_saved_callback(sqscr);
	return OK;
}

bool EditorScriptEditor::edit(const RES &p_resource, int p_line, int p_col, bool p_grab_focus) {
	if (p_resource.is_null()) {
		return false;
	}

	Ref<Script> script = p_resource;

	// Don't open dominant script if using an external editor.
	bool use_external_editor =
			EditorSettings::get_singleton()->get("text_editor/external/use_external_editor") ||
			(script.is_valid() && script->get_language()->overrides_external_editor());
	use_external_editor = use_external_editor && !(script.is_valid() && (script->get_path().empty() || script->get_path().find("::") != -1)); // Ignore external editor for built-in scripts.
	const bool open_dominant = EditorSettings::get_singleton()->get("text_editor/files/open_dominant_script_on_scene_change");

	const bool should_open = (open_dominant && !use_external_editor) || !EditorNode::get_singleton()->is_changing_scene();

	if (script.is_valid() && script->get_language()->overrides_external_editor()) {
		if (should_open) {
			Error err = script->get_language()->open_in_external_editor(script, p_line >= 0 ? p_line : 0, p_col);
			if (err != OK) {
				ERR_PRINT("Couldn't open script in the overridden external text editor");
			}
		}
		return false;
	}

	if (use_external_editor &&
			(debugger->get_dump_stack_script() != p_resource || debugger->get_debug_with_external_editor()) &&
			p_resource->get_path().is_resource_file() &&
			p_resource->get_class_name() != StringName("VisualScript")) {
		String path = EditorSettings::get_singleton()->get("text_editor/external/exec_path");
		String flags = EditorSettings::get_singleton()->get("text_editor/external/exec_flags");

		List<String> args;
		bool has_file_flag = false;
		String script_path = ProjectSettings::get_singleton()->globalize_path(p_resource->get_path());

		if (flags.size()) {
			String project_path = ProjectSettings::get_singleton()->get_resource_path();

			flags = flags.replacen("{line}", itos(p_line > 0 ? p_line : 0));
			flags = flags.replacen("{col}", itos(p_col));
			flags = flags.strip_edges().replace("\\\\", "\\");

			int from = 0;
			int num_chars = 0;
			bool inside_quotes = false;

			for (int i = 0; i < flags.size(); i++) {
				if (flags[i] == '"' && (!i || flags[i - 1] != '\\')) {
					if (!inside_quotes) {
						from++;
					}
					inside_quotes = !inside_quotes;

				} else if (flags[i] == '\0' || (!inside_quotes && flags[i] == ' ')) {
					String arg = flags.substr(from, num_chars);
					if (arg.find("{file}") != -1) {
						has_file_flag = true;
					}

					// do path replacement here, else there will be issues with spaces and quotes
					arg = arg.replacen("{project}", project_path);
					arg = arg.replacen("{file}", script_path);
					args.push_back(arg);

					from = i + 1;
					num_chars = 0;
				} else {
					num_chars++;
				}
			}
		}

		// Default to passing script path if no {file} flag is specified.
		if (!has_file_flag) {
			args.push_back(script_path);
		}

		Error err = OS::get_singleton()->execute(path, args, false);
		if (err == OK) {
			return false;
		}
		WARN_PRINT("Couldn't open external text editor, using internal");
	}

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		if ((script != nullptr && se->get_edited_resource() == p_resource) || se->get_edited_resource()->get_path() == p_resource->get_path()) {
			if (should_open) {
				se->enable_editor();

				if (tab_container->get_current_tab() != i) {
					_go_to_tab(i);
					_update_script_names();
				}
				if (is_visible_in_tree()) {
					se->ensure_focus();
				}

				if (p_line > 0) {
					se->goto_line(p_line - 1);
				}
			}
			_update_script_names();
			script_list->ensure_current_is_visible();
			return true;
		}
	}

	// doesn't have it, make a new one

	EditorScriptEditorBase *se = nullptr;

	for (int i = script_editor_func_count - 1; i >= 0; i--) {
		se = script_editor_funcs[i](p_resource);
		if (se) {
			break;
		}
	}
	ERR_FAIL_COND_V(!se, false);

	se->set_edited_resource(p_resource);

	if (p_resource->get_class_name() != StringName("VisualScript")) {
		bool highlighter_set = false;
		for (int i = 0; i < syntax_highlighters.size(); i++) {
			Ref<EditorSyntaxHighlighter> highlighter = syntax_highlighters[i]->_create();
			se->add_syntax_highlighter(highlighter);

			if (script != nullptr && !highlighter_set) {
				Array languages = highlighter->_get_supported_languages();
				if (languages.find(script->get_language()->get_name()) > -1) {
					se->set_syntax_highlighter(highlighter);
					highlighter_set = true;
				}
			}
		}
	}

	tab_container->add_child(se);

	if (p_grab_focus) {
		se->enable_editor();
	}

	se->set_tooltip_request_func("_get_debug_tooltip", this);
	if (se->get_edit_menu()) {
		se->get_edit_menu()->hide();
		menu_hb->add_child(se->get_edit_menu());
		menu_hb->move_child(se->get_edit_menu(), 1);
	}

	if (p_grab_focus) {
		_go_to_tab(tab_container->get_tab_count() - 1);
		_add_recent_script(p_resource->get_path());
	}

	_sort_list_on_update = true;
	_update_script_names();
	_save_layout();
	se->connect("name_changed", this, "_update_script_names");
	se->connect("edited_script_changed", this, "_script_changed");
	se->connect("request_help", this, "_help_search");
	se->connect("request_open_script_at_line", this, "_goto_script_line");
	se->connect("go_to_help", this, "_help_class_goto");
	se->connect("request_save_history", this, "_save_history");
	se->connect("search_in_files_requested", this, "_on_find_in_files_requested");
	se->connect("replace_in_files_requested", this, "_on_replace_in_files_requested");

	//test for modification, maybe the script was not edited but was loaded

	_test_script_times_on_disk(p_resource);
	_update_modified_scripts_for_external_editor(p_resource);

	if (p_line > 0) {
		se->goto_line(p_line - 1);
	}

	notify_script_changed(p_resource);
	return true;
}

void EditorScriptEditor::save_current_script() {
	EditorScriptEditorBase *current = _get_current_editor();
	if (!current || _test_script_times_on_disk()) {
		return;
	}

	if (trim_trailing_whitespace_on_save) {
		current->trim_trailing_whitespace();
	}

	current->insert_final_newline();

	if (convert_indent_on_save) {
		if (use_space_indentation) {
			current->convert_indent_to_spaces();
		} else {
			current->convert_indent_to_tabs();
		}
	}

	RES resource = current->get_edited_resource();
	Ref<TextFile> text_file = resource;

	if (text_file != nullptr) {
		current->apply_code();
		_save_text_file(text_file, text_file->get_path());
		return;
	}

	if (resource->get_path().empty() || resource->get_path().find("::") != -1) {
		// If built-in script, save the scene instead.
		const String scene_path = resource->get_path().get_slice("::", 0);
		if (!scene_path.empty()) {
			Vector<String> scene_to_save;
			scene_to_save.push_back(scene_path);
			editor->save_scene_list(scene_to_save);
		}
	} else {
		editor->save_resource(resource);
	}
}

void EditorScriptEditor::save_all_scripts() {
	Vector<String> scenes_to_save;

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		if (convert_indent_on_save) {
			if (use_space_indentation) {
				se->convert_indent_to_spaces();
			} else {
				se->convert_indent_to_tabs();
			}
		}

		if (trim_trailing_whitespace_on_save) {
			se->trim_trailing_whitespace();
		}

		se->insert_final_newline();

		if (!se->is_unsaved()) {
			continue;
		}

		RES edited_res = se->get_edited_resource();
		if (edited_res.is_valid()) {
			se->apply_code();
		}

		if (!edited_res->is_built_in()) {
			Ref<TextFile> text_file = edited_res;
			if (text_file != nullptr) {
				_save_text_file(text_file, text_file->get_path());
				continue;
			}
			editor->save_resource(edited_res); //external script, save it
		} else {
			// For built-in scripts, save their scenes instead.
			const String scene_path = edited_res->get_path().get_slice("::", 0);
			if (scenes_to_save.find(scene_path) > -1) {
				scenes_to_save.push_back(scene_path);
			}
		}
	}

	if (!scenes_to_save.empty()) {
		editor->save_scene_list(scenes_to_save);
	}

	_update_script_names();
	EditorFileSystem::get_singleton()->update_script_classes();
}

void EditorScriptEditor::apply_scripts() const {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}
		se->apply_code();
	}
}

void EditorScriptEditor::open_script_create_dialog(const String &p_base_name, const String &p_base_path) {
	_menu_option(FILE_NEW);
	script_create_dialog->config(p_base_name, p_base_path);
}

void EditorScriptEditor::_editor_play() {
	debugger->start();
	debug_menu->get_popup()->grab_focus();
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_NEXT), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_STEP), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_BREAK), false);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_CONTINUE), true);
}

void EditorScriptEditor::_editor_pause() {
}
void EditorScriptEditor::_editor_stop() {
	debugger->stop();
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_NEXT), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_STEP), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_BREAK), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_CONTINUE), true);

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		se->set_debugger_active(false);
	}
}

void EditorScriptEditor::_add_callback(Object *p_obj, const String &p_function, const PoolStringArray &p_args) {
	ERR_FAIL_COND(!p_obj);
	Ref<Script> script = p_obj->get_script();
	ERR_FAIL_COND(!script.is_valid());

	editor->push_item(script.ptr());

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}
		if (se->get_edited_resource() != script) {
			continue;
		}

		se->add_callback(p_function, p_args);

		_go_to_tab(i);

		script_list->select(script_list->find_metadata(i));

		// Save the current script so the changes can be picked up by an external editor.
		if (!script.ptr()->is_built_in()) { // But only if it's not built-in script.
			save_current_script();
		}

		break;
	}
}

void EditorScriptEditor::_save_layout() {
	if (restoring_layout) {
		return;
	}

	editor->save_layout();
}

void EditorScriptEditor::_editor_settings_changed() {
	trim_trailing_whitespace_on_save = EditorSettings::get_singleton()->get("text_editor/files/trim_trailing_whitespace_on_save");
	convert_indent_on_save = EditorSettings::get_singleton()->get("text_editor/indent/convert_indent_on_save");
	use_space_indentation = EditorSettings::get_singleton()->get("text_editor/indent/type");

	members_overview_enabled = EditorSettings::get_singleton()->get("text_editor/script_list/show_members_overview");
	help_overview_enabled = EditorSettings::get_singleton()->get("text_editor/help/show_help_index");
	_update_members_overview_visibility();
	_update_help_overview_visibility();

	_update_autosave_timer();

	if (current_theme == "") {
		current_theme = EditorSettings::get_singleton()->get("text_editor/theme/color_theme");
	} else if (current_theme != EditorSettings::get_singleton()->get("text_editor/theme/color_theme")) {
		current_theme = EditorSettings::get_singleton()->get("text_editor/theme/color_theme");
		EditorSettings::get_singleton()->load_text_editor_theme();
	}

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		se->update_settings();
	}
	_update_script_colors();
	_update_script_names();

	ScriptServer::set_reload_scripts_on_save(EDITOR_DEF("text_editor/files/auto_reload_and_parse_scripts_on_save", true));
}

void EditorScriptEditor::_autosave_scripts() {
	save_all_scripts();
}

void EditorScriptEditor::_update_autosave_timer() {
	if (!autosave_timer->is_inside_tree()) {
		return;
	}

	float autosave_time = EditorSettings::get_singleton()->get("text_editor/files/autosave_interval_secs");
	if (autosave_time > 0) {
		autosave_timer->set_wait_time(autosave_time);
		autosave_timer->start();
	} else {
		autosave_timer->stop();
	}
}

void EditorScriptEditor::_tree_changed() {
	if (waiting_update_names) {
		return;
	}

	waiting_update_names = true;
	call_deferred("_update_script_names");
	call_deferred("_update_script_connections");
}

void EditorScriptEditor::_script_split_dragged(float) {
	_save_layout();
}

Variant EditorScriptEditor::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	if (tab_container->get_child_count() == 0) {
		return Variant();
	}

	Node *cur_node = tab_container->get_child(tab_container->get_current_tab());

	HBoxContainer *drag_preview = memnew(HBoxContainer);
	String preview_name = "";
	Ref<Texture> preview_icon;

	EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(cur_node);
	if (se) {
		preview_name = se->get_name();
		preview_icon = se->get_icon();
	}
	EditorHelp *eh = Object::cast_to<EditorHelp>(cur_node);
	if (eh) {
		preview_name = eh->get_class();
		preview_icon = get_theme_icon("Help", "EditorIcons");
	}

	if (!preview_icon.is_null()) {
		TextureRect *tf = memnew(TextureRect);
		tf->set_texture(preview_icon);
		drag_preview->add_child(tf);
	}
	Label *label = memnew(Label(preview_name));
	drag_preview->add_child(label);
	set_drag_preview(drag_preview);

	Dictionary drag_data;
	drag_data["type"] = "script_list_element"; // using a custom type because node caused problems when dragging to scene tree
	drag_data["script_list_element"] = cur_node;

	return drag_data;
}

bool EditorScriptEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	Dictionary d = p_data;
	if (!d.has("type")) {
		return false;
	}

	if (String(d["type"]) == "script_list_element") {
		Node *node = d["script_list_element"];

		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(node);
		if (se) {
			return true;
		}
		EditorHelp *eh = Object::cast_to<EditorHelp>(node);
		if (eh) {
			return true;
		}
	}

	if (String(d["type"]) == "nodes") {
		Array nodes = d["nodes"];
		if (nodes.size() == 0) {
			return false;
		}
		Node *node = get_node((nodes[0]));

		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(node);
		if (se) {
			return true;
		}
		EditorHelp *eh = Object::cast_to<EditorHelp>(node);
		if (eh) {
			return true;
		}
	}

	if (String(d["type"]) == "files") {
		Vector<String> files = d["files"];

		if (files.size() == 0) {
			return false; //weird
		}

		for (int i = 0; i < files.size(); i++) {
			String file = files[i];
			if (file == "" || !FileAccess::exists(file)) {
				continue;
			}
			Ref<Script> scr = ResourceLoader::load(file);
			if (scr.is_valid()) {
				return true;
			}
		}
		return true;
	}

	return false;
}

void EditorScriptEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	if (!can_drop_data_fw(p_point, p_data, p_from)) {
		return;
	}

	Dictionary d = p_data;
	if (!d.has("type")) {
		return;
	}

	if (String(d["type"]) == "script_list_element") {
		Node *node = d["script_list_element"];

		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(node);
		EditorHelp *eh = Object::cast_to<EditorHelp>(node);
		if (se || eh) {
			int new_index = 0;
			if (script_list->get_item_count() > 0) {
				new_index = script_list->get_item_metadata(script_list->get_item_at_position(p_point));
			}
			tab_container->move_child(node, new_index);
			tab_container->set_current_tab(new_index);
			_update_script_names();
		}
	}

	if (String(d["type"]) == "nodes") {
		Array nodes = d["nodes"];
		if (nodes.size() == 0) {
			return;
		}
		Node *node = get_node(nodes[0]);

		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(node);
		EditorHelp *eh = Object::cast_to<EditorHelp>(node);
		if (se || eh) {
			int new_index = 0;
			if (script_list->get_item_count() > 0) {
				new_index = script_list->get_item_metadata(script_list->get_item_at_position(p_point));
			}
			tab_container->move_child(node, new_index);
			tab_container->set_current_tab(new_index);
			_update_script_names();
		}
	}

	if (String(d["type"]) == "files") {
		Vector<String> files = d["files"];

		int new_index = 0;
		if (script_list->get_item_count() > 0) {
			new_index = script_list->get_item_metadata(script_list->get_item_at_position(p_point));
		}
		int num_tabs_before = tab_container->get_child_count();
		for (int i = 0; i < files.size(); i++) {
			String file = files[i];
			if (file == "" || !FileAccess::exists(file)) {
				continue;
			}
			Ref<Script> scr = ResourceLoader::load(file);
			if (scr.is_valid()) {
				edit(scr);
				const int num_tabs = tab_container->get_child_count();
				if (num_tabs > num_tabs_before) {
					tab_container->move_child(tab_container->get_child(tab_container->get_child_count() - 1), new_index);
					num_tabs_before = num_tabs;
				} else if (num_tabs > 0) { /* Maybe script was already open */
					tab_container->move_child(tab_container->get_child(tab_container->get_current_tab()), new_index);
				}
			}
		}
		if (tab_container->get_child_count() > 0) {
			tab_container->set_current_tab(new_index);
		}
		_update_script_names();
	}
}

void EditorScriptEditor::_input(const Ref<InputEvent> &p_event) {
	// This feature can be disabled to avoid interfering with other uses of the additional
	// mouse buttons, such as push-to-talk in a VoIP program.
	if (EDITOR_GET("text_editor/navigation/mouse_extra_buttons_navigate_history")) {
		const Ref<InputEventMouseButton> mb = p_event;

		// Navigate the script history using additional mouse buttons present on some mice.
		// This must be hardcoded as the editor shortcuts dialog doesn't allow assigning
		// more than one shortcut per action.
		if (mb.is_valid() && mb->is_pressed() && is_visible_in_tree() && is_focus_owner_in_shortcut_context() && !get_viewport()->gui_has_modal_stack()) {
			if (mb->get_button_index() == BUTTON_XBUTTON1) {
				_history_back();
				accept_event();
			}

			if (mb->get_button_index() == BUTTON_XBUTTON2) {
				_history_forward();
				accept_event();
			}
		}
	}
}

void EditorScriptEditor::_shortcut_input(const Ref<InputEvent> &p_event) {
	ERR_FAIL_COND(p_event.is_null());

	if (!is_visible_in_tree() || !p_event->is_pressed() || p_event->is_echo()) {
		return;
	}

	if (ED_IS_SHORTCUT("script_editor/next_script", p_event)) {
		if (script_list->get_item_count() > 1) {
			int next_tab = script_list->get_current() + 1;
			next_tab %= script_list->get_item_count();
			_go_to_tab(script_list->get_item_metadata(next_tab));
			_update_script_names();
		}
		accept_event();
	}

	if (ED_IS_SHORTCUT("script_editor/prev_script", p_event)) {
		if (script_list->get_item_count() > 1) {
			int next_tab = script_list->get_current() - 1;
			next_tab = next_tab >= 0 ? next_tab : script_list->get_item_count() - 1;
			_go_to_tab(script_list->get_item_metadata(next_tab));
			_update_script_names();
		}
		accept_event();
	}

	if (ED_IS_SHORTCUT("script_editor/window_move_up", p_event)) {
		_menu_option(WINDOW_MOVE_UP);
		accept_event();
	}

	if (ED_IS_SHORTCUT("script_editor/window_move_down", p_event)) {
		_menu_option(WINDOW_MOVE_DOWN);
		accept_event();
	}
}

void EditorScriptEditor::_script_list_gui_input(const Ref<InputEvent> &ev) {
	Ref<InputEventMouseButton> mb = ev;
	if (mb.is_valid() && mb->is_pressed()) {
		switch (mb->get_button_index()) {
			case BUTTON_MIDDLE: {
				// Right-click selects automatically; middle-click does not.
				int idx = script_list->get_item_at_position(mb->get_position(), true);
				if (idx >= 0) {
					script_list->select(idx);
					_script_selected(idx);
					_menu_option(FILE_CLOSE);
				}
			} break;

			case BUTTON_RIGHT: {
				_make_script_list_context_menu();
			} break;
		}
	}
}

void EditorScriptEditor::_make_script_list_context_menu() {
	context_menu->clear();

	int selected = tab_container->get_current_tab();
	if (selected < 0 || selected >= tab_container->get_child_count()) {
		return;
	}

	EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(selected));
	if (se) {
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/save"), FILE_SAVE);
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/save_as"), FILE_SAVE_AS);
	}
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/close_file"), FILE_CLOSE);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/close_all"), CLOSE_ALL);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/close_other_tabs"), CLOSE_OTHER_TABS);
	context_menu->add_separator();
	if (se) {
		Ref<Script> scr = se->get_edited_resource();
		if (scr != nullptr) {
			context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/reload_script_soft"), FILE_TOOL_RELOAD_SOFT);
			if (!scr.is_null() && scr->is_tool()) {
				context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/run_file"), FILE_RUN);
				context_menu->add_separator();
			}
		}
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/copy_path"), FILE_COPY_PATH);
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/show_in_file_system"), SHOW_IN_FILE_SYSTEM);
		context_menu->add_separator();
	}

	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/window_move_up"), WINDOW_MOVE_UP);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/window_move_down"), WINDOW_MOVE_DOWN);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/window_sort"), WINDOW_SORT);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_editor/toggle_scripts_panel"), TOGGLE_SCRIPTS_PANEL);

	context_menu->set_item_disabled(context_menu->get_item_index(CLOSE_ALL), tab_container->get_child_count() <= 0);
	context_menu->set_item_disabled(context_menu->get_item_index(CLOSE_OTHER_TABS), tab_container->get_child_count() <= 1);
	context_menu->set_item_disabled(context_menu->get_item_index(WINDOW_MOVE_UP), tab_container->get_current_tab() <= 0);
	context_menu->set_item_disabled(context_menu->get_item_index(WINDOW_MOVE_DOWN), tab_container->get_current_tab() >= tab_container->get_child_count() - 1);
	context_menu->set_item_disabled(context_menu->get_item_index(WINDOW_SORT), tab_container->get_child_count() <= 1);

	context_menu->set_position(get_global_transform().xform(get_local_mouse_position()));
	context_menu->set_size(Vector2(1, 1));
	context_menu->popup();
}

void EditorScriptEditor::set_window_layout(Ref<ConfigFile> p_layout) {
	if (!bool(EDITOR_DEF("text_editor/files/restore_scripts_on_load", true))) {
		return;
	}

	if (!p_layout->has_section_key("EditorScriptEditor", "open_scripts") && !p_layout->has_section_key("EditorScriptEditor", "open_help")) {
		return;
	}

	Array scripts = p_layout->get_value("EditorScriptEditor", "open_scripts");
	Array helps;
	if (p_layout->has_section_key("EditorScriptEditor", "open_help")) {
		helps = p_layout->get_value("EditorScriptEditor", "open_help");
	}

	restoring_layout = true;

	List<String> extensions;
	ResourceLoader::get_recognized_extensions_for_type("Script", &extensions);

	for (int i = 0; i < scripts.size(); i++) {
		String path = scripts[i];

		Dictionary script_info = scripts[i];
		if (!script_info.empty()) {
			path = script_info["path"];
		}

		if (!FileAccess::exists(path)) {
			continue;
		}

		if (extensions.find(path.get_extension())) {
			Ref<Script> scr = ResourceLoader::load(path);
			if (!scr.is_valid()) {
				continue;
			}
			if (!edit(scr, false)) {
				continue;
			}
		} else {
			Error error;
			Ref<TextFile> text_file = _load_text_file(path, &error);
			if (error != OK || !text_file.is_valid()) {
				continue;
			}
			if (!edit(text_file, false)) {
				continue;
			}
		}

		if (!script_info.empty()) {
			EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(tab_container->get_tab_count() - 1));
			if (se) {
				se->set_edit_state(script_info["state"]);
			}
		}
	}

	for (int i = 0; i < helps.size(); i++) {
		String path = helps[i];
		if (path == "") { // invalid, skip
			continue;
		}
		_help_class_open(path);
	}

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		tab_container->get_child(i)->set_meta("__editor_pass", Variant());
	}

	if (p_layout->has_section_key("EditorScriptEditor", "split_offset")) {
		script_split->set_split_offset(p_layout->get_value("EditorScriptEditor", "split_offset"));
	}

	restoring_layout = false;

	_update_script_names();
}

void EditorScriptEditor::get_window_layout(Ref<ConfigFile> p_layout) {
	Array scripts;
	Array helps;

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (se) {
			String path = se->get_edited_resource()->get_path();
			if (!path.is_resource_file()) {
				continue;
			}

			Dictionary script_info;
			script_info["path"] = path;
			script_info["state"] = se->get_edit_state();

			scripts.push_back(script_info);
		}

		EditorHelp *eh = Object::cast_to<EditorHelp>(tab_container->get_child(i));

		if (eh) {
			helps.push_back(eh->get_class());
		}
	}

	p_layout->set_value("EditorScriptEditor", "open_scripts", scripts);
	p_layout->set_value("EditorScriptEditor", "open_help", helps);
	p_layout->set_value("EditorScriptEditor", "split_offset", script_split->get_split_offset());
}

void EditorScriptEditor::_help_class_open(const String &p_class) {
	if (p_class == "") {
		return;
	}

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorHelp *eh = Object::cast_to<EditorHelp>(tab_container->get_child(i));

		if (eh && eh->get_class() == p_class) {
			_go_to_tab(i);
			_update_script_names();
			return;
		}
	}

	EditorHelp *eh = memnew(EditorHelp);

	eh->set_name(p_class);
	tab_container->add_child(eh);
	_go_to_tab(tab_container->get_tab_count() - 1);
	eh->go_to_class(p_class, 0);
	eh->connect("go_to_help", this, "_help_class_goto");
	_add_recent_script(p_class);
	_sort_list_on_update = true;
	_update_script_names();
	_save_layout();
}

void EditorScriptEditor::_help_class_goto(const String &p_desc) {
	String cname = p_desc.get_slice(":", 1);

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorHelp *eh = Object::cast_to<EditorHelp>(tab_container->get_child(i));

		if (eh && eh->get_class() == cname) {
			_go_to_tab(i);
			eh->go_to_help(p_desc);
			_update_script_names();
			return;
		}
	}

	EditorHelp *eh = memnew(EditorHelp);

	eh->set_name(cname);
	tab_container->add_child(eh);
	_go_to_tab(tab_container->get_tab_count() - 1);
	eh->go_to_help(p_desc);
	eh->connect("go_to_help", this, "_help_class_goto");
	_add_recent_script(eh->get_class());
	_sort_list_on_update = true;
	_update_script_names();
	_save_layout();
}

void EditorScriptEditor::_update_selected_editor_menu() {
	for (int i = 0; i < tab_container->get_child_count(); i++) {
		bool current = tab_container->get_current_tab() == i;

		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (se && se->get_edit_menu()) {
			if (current) {
				se->get_edit_menu()->show();
			} else {
				se->get_edit_menu()->hide();
			}
		}
	}

	EditorHelp *eh = Object::cast_to<EditorHelp>(tab_container->get_current_tab_control());
	script_search_menu->get_popup()->clear();
	if (eh) {
		script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/find", TTR("Find..."), KEY_MASK_CMD | KEY_F), HELP_SEARCH_FIND);
		script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/find_next", TTR("Find Next"), KEY_F3), HELP_SEARCH_FIND_NEXT);
		script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/find_previous", TTR("Find Previous"), KEY_MASK_SHIFT | KEY_F3), HELP_SEARCH_FIND_PREVIOUS);
		script_search_menu->get_popup()->add_separator();
		script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/find_in_files", TTR("Find in Files"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_F), SEARCH_IN_FILES);
		script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/replace_in_files", TTR("Replace in Files"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_R), REPLACE_IN_FILES);
		script_search_menu->show();
	} else {
		if (tab_container->get_child_count() == 0) {
			script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/find_in_files", TTR("Find in Files"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_F), SEARCH_IN_FILES);
			script_search_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/replace_in_files", TTR("Replace in Files"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_R), REPLACE_IN_FILES);
			script_search_menu->show();
		} else {
			script_search_menu->hide();
		}
	}
}

void EditorScriptEditor::_update_history_pos(int p_new_pos) {
	Node *n = tab_container->get_current_tab_control();

	if (Object::cast_to<EditorScriptEditorBase>(n)) {
		history.write[history_pos].state = Object::cast_to<EditorScriptEditorBase>(n)->get_edit_state();
	}
	if (Object::cast_to<EditorHelp>(n)) {
		history.write[history_pos].state = Object::cast_to<EditorHelp>(n)->get_scroll();
	}

	history_pos = p_new_pos;
	tab_container->set_current_tab(history[history_pos].control->get_index());

	n = history[history_pos].control;

	if (Object::cast_to<EditorScriptEditorBase>(n)) {
		Object::cast_to<EditorScriptEditorBase>(n)->set_edit_state(history[history_pos].state);
		Object::cast_to<EditorScriptEditorBase>(n)->ensure_focus();

		Ref<Script> script = Object::cast_to<EditorScriptEditorBase>(n)->get_edited_resource();
		if (script != nullptr) {
			notify_script_changed(script);
		}
	}

	if (Object::cast_to<EditorHelp>(n)) {
		Object::cast_to<EditorHelp>(n)->set_scroll(history[history_pos].state);
		Object::cast_to<EditorHelp>(n)->set_focused();
	}

	n->set_meta("__editor_pass", ++edit_pass);
	_update_script_names();
	_update_history_arrows();
	_update_selected_editor_menu();
}

void EditorScriptEditor::_history_forward() {
	if (history_pos < history.size() - 1) {
		_update_history_pos(history_pos + 1);
	}
}

void EditorScriptEditor::_history_back() {
	if (history_pos > 0) {
		_update_history_pos(history_pos - 1);
	}
}

Vector<Ref<Script>> EditorScriptEditor::get_open_scripts() const {
	Vector<Ref<Script>> out_scripts = Vector<Ref<Script>>();

	for (int i = 0; i < tab_container->get_child_count(); i++) {
		EditorScriptEditorBase *se = Object::cast_to<EditorScriptEditorBase>(tab_container->get_child(i));
		if (!se) {
			continue;
		}

		Ref<Script> script = se->get_edited_resource();
		if (script != nullptr) {
			out_scripts.push_back(script);
		}
	}

	return out_scripts;
}

void EditorScriptEditor::set_scene_root_script(Ref<Script> p_script) {
	// Don't open dominant script if using an external editor.
	bool use_external_editor =
			EditorSettings::get_singleton()->get("text_editor/external/use_external_editor") ||
			(p_script.is_valid() && p_script->get_language()->overrides_external_editor());
	use_external_editor = use_external_editor && !(p_script.is_valid() && (p_script->get_path().empty() || p_script->get_path().find("::") != -1)); // Ignore external editor for built-in scripts.
	const bool open_dominant = EditorSettings::get_singleton()->get("text_editor/files/open_dominant_script_on_scene_change");

	if (open_dominant && !use_external_editor && p_script.is_valid()) {
		edit(p_script);
	}
}

bool EditorScriptEditor::script_goto_method(Ref<Script> p_script, const String &p_method) {
	int line = p_script->get_member_line(p_method);

	if (line == -1) {
		return false;
	}

	return edit(p_script, line, 0);
}

void EditorScriptEditor::set_live_auto_reload_running_scripts(bool p_enabled) {
	auto_reload_running_scripts = p_enabled;
}

void EditorScriptEditor::_help_search(String p_text) {
	help_search_dialog->popup_dialog(p_text);
}

void EditorScriptEditor::_open_script_request(const String &p_path) {
	Ref<Script> script = ResourceLoader::load(p_path);
	if (script.is_valid()) {
		script_editor->edit(script, false);
		return;
	}

	Error err;
	Ref<TextFile> text_file = script_editor->_load_text_file(p_path, &err);
	if (text_file.is_valid()) {
		script_editor->edit(text_file, false);
		return;
	}
}

void EditorScriptEditor::register_syntax_highlighter(const Ref<EditorSyntaxHighlighter> &p_syntax_highlighter) {
	ERR_FAIL_COND(p_syntax_highlighter.is_null());

	if (syntax_highlighters.find(p_syntax_highlighter) == -1) {
		syntax_highlighters.push_back(p_syntax_highlighter);
	}
}

void EditorScriptEditor::unregister_syntax_highlighter(const Ref<EditorSyntaxHighlighter> &p_syntax_highlighter) {
	ERR_FAIL_COND(p_syntax_highlighter.is_null());

	syntax_highlighters.erase(p_syntax_highlighter);
}

int EditorScriptEditor::script_editor_func_count = 0;
CreateEditorScriptEditorFunc EditorScriptEditor::script_editor_funcs[EditorScriptEditor::SCRIPT_EDITOR_FUNC_MAX];

void EditorScriptEditor::register_create_script_editor_function(CreateEditorScriptEditorFunc p_func) {
	ERR_FAIL_COND(script_editor_func_count == SCRIPT_EDITOR_FUNC_MAX);
	script_editor_funcs[script_editor_func_count++] = p_func;
}

void EditorScriptEditor::_script_changed() {
	NodeDock::singleton->update_lists();
}

void EditorScriptEditor::_on_find_in_files_requested(String text) {
	find_in_files_dialog->set_find_in_files_mode(FindInFilesDialog::SEARCH_MODE);
	find_in_files_dialog->set_search_text(text);
	find_in_files_dialog->popup_centered_minsize();
}

void EditorScriptEditor::_on_replace_in_files_requested(String text) {
	find_in_files_dialog->set_find_in_files_mode(FindInFilesDialog::REPLACE_MODE);
	find_in_files_dialog->set_search_text(text);
	find_in_files_dialog->set_replace_text("");
	find_in_files_dialog->popup_centered_minsize();
}

void EditorScriptEditor::_on_find_in_files_result_selected(String fpath, int line_number, int begin, int end) {
	if (ResourceLoader::exists(fpath)) {
		RES res = ResourceLoader::load(fpath);

#ifdef MODULE_SHADER_EDITOR_ENABLED
		if (fpath.get_extension() == "gdshader" || fpath.get_extension() == "shader") {
			ShaderEditorPlugin *shader_editor = Object::cast_to<ShaderEditorPlugin>(EditorNode::get_singleton()->get_editor_data().get_editor("Shader"));
			shader_editor->edit(res.ptr());
			shader_editor->make_visible(true);
			shader_editor->get_shader_editor()->goto_line_selection(line_number - 1, begin, end);
			return;
		} else if (fpath.get_extension() == "tscn") {
#else
		if (fpath.get_extension() == "tscn") {
#endif
			editor->load_scene(fpath);
			return;
		} else {
			Ref<Script> script = res;
			if (script.is_valid()) {
				edit(script);

				EditorScriptTextEditor *ste = Object::cast_to<EditorScriptTextEditor>(_get_current_editor());
				if (ste) {
					ste->goto_line_selection(line_number - 1, begin, end);
				}
				return;
			}
		}
	}

	// If the file is not a valid resource/script, load it as a text file.
	Error err;
	Ref<TextFile> text_file = _load_text_file(fpath, &err);
	if (text_file.is_valid()) {
		edit(text_file);

		EditorTextEditor *te = Object::cast_to<EditorTextEditor>(_get_current_editor());
		if (te) {
			te->goto_line_selection(line_number - 1, begin, end);
		}
	}
}

void EditorScriptEditor::_start_find_in_files(bool with_replace) {
	FindInFiles *f = find_in_files->get_finder();

	f->set_search_text(find_in_files_dialog->get_search_text());
	f->set_match_case(find_in_files_dialog->is_match_case());
	f->set_whole_words(find_in_files_dialog->is_whole_words());
	f->set_folder(find_in_files_dialog->get_folder());
	f->set_filter(find_in_files_dialog->get_filter());

	find_in_files->set_with_replace(with_replace);
	find_in_files->set_replace_text(find_in_files_dialog->get_replace_text());
	find_in_files->start_search();

	editor->make_bottom_panel_item_visible(find_in_files);
}

void EditorScriptEditor::_on_find_in_files_modified_files(PoolStringArray paths) {
	_test_script_times_on_disk();
	_update_modified_scripts_for_external_editor();
}

void EditorScriptEditor::_filter_scripts_text_changed(const String &p_newtext) {
	_update_script_names();
}

void EditorScriptEditor::_filter_methods_text_changed(const String &p_newtext) {
	_update_members_overview();
}

void EditorScriptEditor::_bind_methods() {
	ClassDB::bind_method("_file_dialog_action", &EditorScriptEditor::_file_dialog_action);
	ClassDB::bind_method("_tab_changed", &EditorScriptEditor::_tab_changed);
	ClassDB::bind_method("_menu_option", &EditorScriptEditor::_menu_option);
	ClassDB::bind_method("_close_current_tab", &EditorScriptEditor::_close_current_tab);
	ClassDB::bind_method("_close_discard_current_tab", &EditorScriptEditor::_close_discard_current_tab);
	ClassDB::bind_method("_close_docs_tab", &EditorScriptEditor::_close_docs_tab);
	ClassDB::bind_method("_close_all_tabs", &EditorScriptEditor::_close_all_tabs);
	ClassDB::bind_method("_close_other_tabs", &EditorScriptEditor::_close_other_tabs);
	ClassDB::bind_method("_queue_close_tabs", &EditorScriptEditor::_queue_close_tabs);
	ClassDB::bind_method("_open_recent_script", &EditorScriptEditor::_open_recent_script);
	ClassDB::bind_method("_theme_option", &EditorScriptEditor::_theme_option);
	ClassDB::bind_method("_editor_play", &EditorScriptEditor::_editor_play);
	ClassDB::bind_method("_editor_pause", &EditorScriptEditor::_editor_pause);
	ClassDB::bind_method("_editor_stop", &EditorScriptEditor::_editor_stop);
	ClassDB::bind_method("_add_callback", &EditorScriptEditor::_add_callback);
	ClassDB::bind_method("_resave_scripts", &EditorScriptEditor::_resave_scripts);
	ClassDB::bind_method("_res_saved_callback", &EditorScriptEditor::_res_saved_callback);
	ClassDB::bind_method("_scene_saved_callback", &EditorScriptEditor::_scene_saved_callback);
	ClassDB::bind_method("_goto_script_line", &EditorScriptEditor::_goto_script_line);
	ClassDB::bind_method("_goto_script_line2", &EditorScriptEditor::_goto_script_line2);
	ClassDB::bind_method("_set_execution", &EditorScriptEditor::_set_execution);
	ClassDB::bind_method("_clear_execution", &EditorScriptEditor::_clear_execution);
	ClassDB::bind_method("_help_search", &EditorScriptEditor::_help_search);
	ClassDB::bind_method("_save_history", &EditorScriptEditor::_save_history);
	ClassDB::bind_method("_copy_script_path", &EditorScriptEditor::_copy_script_path);
	ClassDB::bind_method("_prepare_file_menu", &EditorScriptEditor::_prepare_file_menu);

	ClassDB::bind_method("_breaked", &EditorScriptEditor::_breaked);
	ClassDB::bind_method("_show_debugger", &EditorScriptEditor::_show_debugger);
	ClassDB::bind_method("_get_debug_tooltip", &EditorScriptEditor::_get_debug_tooltip);
	ClassDB::bind_method("_autosave_scripts", &EditorScriptEditor::_autosave_scripts);
	ClassDB::bind_method("_update_autosave_timer", &EditorScriptEditor::_update_autosave_timer);
	ClassDB::bind_method("_editor_settings_changed", &EditorScriptEditor::_editor_settings_changed);
	ClassDB::bind_method("_update_script_names", &EditorScriptEditor::_update_script_names);
	ClassDB::bind_method("_update_script_connections", &EditorScriptEditor::_update_script_connections);
	ClassDB::bind_method("_tree_changed", &EditorScriptEditor::_tree_changed);
	ClassDB::bind_method("_members_overview_selected", &EditorScriptEditor::_members_overview_selected);
	ClassDB::bind_method("_help_overview_selected", &EditorScriptEditor::_help_overview_selected);
	ClassDB::bind_method("_script_selected", &EditorScriptEditor::_script_selected);
	ClassDB::bind_method("_script_created", &EditorScriptEditor::_script_created);
	ClassDB::bind_method("_script_split_dragged", &EditorScriptEditor::_script_split_dragged);
	ClassDB::bind_method("_help_class_open", &EditorScriptEditor::_help_class_open);
	ClassDB::bind_method("_help_class_goto", &EditorScriptEditor::_help_class_goto);
	ClassDB::bind_method("_request_help", &EditorScriptEditor::_help_class_open);
	ClassDB::bind_method("_history_forward", &EditorScriptEditor::_history_forward);
	ClassDB::bind_method("_history_back", &EditorScriptEditor::_history_back);
	ClassDB::bind_method("_live_auto_reload_running_scripts", &EditorScriptEditor::_live_auto_reload_running_scripts);
	ClassDB::bind_method("_input", &EditorScriptEditor::_input);
	ClassDB::bind_method("_shortcut_input", &EditorScriptEditor::_shortcut_input);
	ClassDB::bind_method("_script_list_gui_input", &EditorScriptEditor::_script_list_gui_input);
	ClassDB::bind_method("_toggle_members_overview_alpha_sort", &EditorScriptEditor::_toggle_members_overview_alpha_sort);
	ClassDB::bind_method("_update_members_overview", &EditorScriptEditor::_update_members_overview);
	ClassDB::bind_method("_script_changed", &EditorScriptEditor::_script_changed);
	ClassDB::bind_method("_filter_scripts_text_changed", &EditorScriptEditor::_filter_scripts_text_changed);
	ClassDB::bind_method("_filter_methods_text_changed", &EditorScriptEditor::_filter_methods_text_changed);
	ClassDB::bind_method("_update_recent_scripts", &EditorScriptEditor::_update_recent_scripts);
	ClassDB::bind_method("_on_find_in_files_requested", &EditorScriptEditor::_on_find_in_files_requested);
	ClassDB::bind_method("_on_replace_in_files_requested", &EditorScriptEditor::_on_replace_in_files_requested);
	ClassDB::bind_method("_start_find_in_files", &EditorScriptEditor::_start_find_in_files);
	ClassDB::bind_method("_on_find_in_files_result_selected", &EditorScriptEditor::_on_find_in_files_result_selected);
	ClassDB::bind_method("_on_find_in_files_modified_files", &EditorScriptEditor::_on_find_in_files_modified_files);

	ClassDB::bind_method(D_METHOD("get_drag_data_fw", "point", "from"), &EditorScriptEditor::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw", "point", "data", "from"), &EditorScriptEditor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw", "point", "data", "from"), &EditorScriptEditor::drop_data_fw);

	ClassDB::bind_method(D_METHOD("goto_line", "line_number"), &EditorScriptEditor::_goto_script_line2);
	ClassDB::bind_method(D_METHOD("get_current_script"), &EditorScriptEditor::_get_current_script);
	ClassDB::bind_method(D_METHOD("get_open_scripts"), &EditorScriptEditor::_get_open_scripts);
	ClassDB::bind_method(D_METHOD("open_script_create_dialog", "base_name", "base_path"), &EditorScriptEditor::open_script_create_dialog);
	ClassDB::bind_method(D_METHOD("reload_scripts"), &EditorScriptEditor::reload_scripts);
	ClassDB::bind_method(D_METHOD("get_base_editor"), &EditorScriptEditor::_get_base_editor);

	ADD_SIGNAL(MethodInfo("editor_script_changed", PropertyInfo(Variant::OBJECT, "script", PROPERTY_HINT_RESOURCE_TYPE, "Script")));
	ADD_SIGNAL(MethodInfo("script_close", PropertyInfo(Variant::OBJECT, "script", PROPERTY_HINT_RESOURCE_TYPE, "Script")));
}

EditorScriptEditor::EditorScriptEditor(EditorNode *p_editor) {
	current_theme = "";

	completion_cache = memnew(EditorScriptCodeCompletionCache);
	restoring_layout = false;
	waiting_update_names = false;
	pending_auto_reload = false;
	auto_reload_running_scripts = true;
	members_overview_enabled = EditorSettings::get_singleton()->get("text_editor/script_list/show_members_overview");
	help_overview_enabled = EditorSettings::get_singleton()->get("text_editor/help/show_help_index");
	editor = p_editor;

	VBoxContainer *main_container = memnew(VBoxContainer);
	add_child(main_container);

	menu_hb = memnew(HBoxContainer);
	main_container->add_child(menu_hb);

	script_split = memnew(HSplitContainer);
	main_container->add_child(script_split);
	script_split->set_v_size_flags(SIZE_EXPAND_FILL);

	list_split = memnew(VSplitContainer);
	script_split->add_child(list_split);
	list_split->set_v_size_flags(SIZE_EXPAND_FILL);

	scripts_vbox = memnew(VBoxContainer);
	scripts_vbox->set_v_size_flags(SIZE_EXPAND_FILL);
	list_split->add_child(scripts_vbox);

	filter_scripts = memnew(LineEdit);
	filter_scripts->set_placeholder(TTR("Filter scripts"));
	filter_scripts->set_clear_button_enabled(true);
	filter_scripts->connect("text_changed", this, "_filter_scripts_text_changed");
	scripts_vbox->add_child(filter_scripts);

	script_list = memnew(ItemList);
	scripts_vbox->add_child(script_list);
	script_list->set_custom_minimum_size(Size2(150, 60) * EDSCALE); //need to give a bit of limit to avoid it from disappearing
	script_list->set_v_size_flags(SIZE_EXPAND_FILL);
	script_split->set_split_offset(70 * EDSCALE);
	_sort_list_on_update = true;
	script_list->connect("gui_input", this, "_script_list_gui_input", varray(), CONNECT_DEFERRED);
	script_list->set_allow_rmb_select(true);
	script_list->set_drag_forwarding(this);

	context_menu = memnew(PopupMenu);
	add_child(context_menu);
	context_menu->connect("id_pressed", this, "_menu_option");
	context_menu->set_hide_on_window_lose_focus(true);

	overview_vbox = memnew(VBoxContainer);
	overview_vbox->set_custom_minimum_size(Size2(0, 90));
	overview_vbox->set_v_size_flags(SIZE_EXPAND_FILL);

	list_split->add_child(overview_vbox);
	list_split->set_visible(EditorSettings::get_singleton()->get_project_metadata("scripts_panel", "show_scripts_panel", true));
	buttons_hbox = memnew(HBoxContainer);
	overview_vbox->add_child(buttons_hbox);

	filename = memnew(Label);
	filename->set_clip_text(true);
	filename->set_h_size_flags(SIZE_EXPAND_FILL);
	filename->add_theme_style_override("normal", EditorNode::get_singleton()->get_gui_base()->get_theme_stylebox("normal", "LineEdit"));
	buttons_hbox->add_child(filename);

	members_overview_alphabeta_sort_button = memnew(ToolButton);
	members_overview_alphabeta_sort_button->set_tooltip(TTR("Toggle alphabetical sorting of the method list."));
	members_overview_alphabeta_sort_button->set_toggle_mode(true);
	members_overview_alphabeta_sort_button->set_pressed(EditorSettings::get_singleton()->get("text_editor/tools/sort_members_outline_alphabetically"));
	members_overview_alphabeta_sort_button->connect("toggled", this, "_toggle_members_overview_alpha_sort");

	buttons_hbox->add_child(members_overview_alphabeta_sort_button);

	filter_methods = memnew(LineEdit);
	filter_methods->set_placeholder(TTR("Filter methods"));
	filter_methods->set_clear_button_enabled(true);
	filter_methods->connect("text_changed", this, "_filter_methods_text_changed");
	overview_vbox->add_child(filter_methods);

	members_overview = memnew(ItemList);
	overview_vbox->add_child(members_overview);

	members_overview->set_allow_reselect(true);
	members_overview->set_custom_minimum_size(Size2(0, 60) * EDSCALE); //need to give a bit of limit to avoid it from disappearing
	members_overview->set_v_size_flags(SIZE_EXPAND_FILL);
	members_overview->set_allow_rmb_select(true);

	help_overview = memnew(ItemList);
	overview_vbox->add_child(help_overview);
	help_overview->set_allow_reselect(true);
	help_overview->set_custom_minimum_size(Size2(0, 60) * EDSCALE); //need to give a bit of limit to avoid it from disappearing
	help_overview->set_v_size_flags(SIZE_EXPAND_FILL);

	tab_container = memnew(TabContainer);
	tab_container->set_tabs_visible(false);
	tab_container->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
	script_split->add_child(tab_container);
	tab_container->set_h_size_flags(SIZE_EXPAND_FILL);

	ED_SHORTCUT("script_editor/window_sort", TTR("Sort"));
	ED_SHORTCUT("script_editor/window_move_up", TTR("Move Up"), KEY_MASK_SHIFT | KEY_MASK_ALT | KEY_UP);
	ED_SHORTCUT("script_editor/window_move_down", TTR("Move Down"), KEY_MASK_SHIFT | KEY_MASK_ALT | KEY_DOWN);
	// FIXME: These should be `KEY_GREATER` and `KEY_LESS` but those don't work.
	ED_SHORTCUT("script_editor/next_script", TTR("Next Script"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_PERIOD);
	ED_SHORTCUT("script_editor/prev_script", TTR("Previous Script"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_COMMA);
	set_process_input(true);
	set_process_shortcut_input(true);

	file_menu = memnew(MenuButton);
	menu_hb->add_child(file_menu);
	file_menu->set_text(TTR("File"));
	file_menu->set_switch_on_hover(true);
	file_menu->get_popup()->set_hide_on_window_lose_focus(true);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/new", TTR("New Script...")), FILE_NEW);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/new_textfile", TTR("New Text File...")), FILE_NEW_TEXTFILE);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/open", TTR("Open...")), FILE_OPEN);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/reopen_closed_script", TTR("Reopen Closed Script"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_T), FILE_REOPEN_CLOSED);
	file_menu->get_popup()->add_submenu_item(TTR("Open Recent"), "RecentScripts", FILE_OPEN_RECENT);

	recent_scripts = memnew(PopupMenu);
	recent_scripts->set_name("RecentScripts");
	file_menu->get_popup()->add_child(recent_scripts);
	recent_scripts->connect("id_pressed", this, "_open_recent_script");
	_update_recent_scripts();

	file_menu->get_popup()->add_separator();
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/save", TTR("Save"), KEY_MASK_ALT | KEY_MASK_CMD | KEY_S), FILE_SAVE);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/save_as", TTR("Save As...")), FILE_SAVE_AS);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/save_all", TTR("Save All"), KEY_MASK_SHIFT | KEY_MASK_ALT | KEY_S), FILE_SAVE_ALL);
	file_menu->get_popup()->add_separator();
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/reload_script_soft", TTR("Soft Reload Script"), KEY_MASK_CMD | KEY_MASK_ALT | KEY_R), FILE_TOOL_RELOAD_SOFT);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/copy_path", TTR("Copy Script Path")), FILE_COPY_PATH);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/show_in_file_system", TTR("Show in FileSystem")), SHOW_IN_FILE_SYSTEM);
	file_menu->get_popup()->add_separator();

	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/history_previous", TTR("History Previous"), KEY_MASK_ALT | KEY_LEFT), WINDOW_PREV);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/history_next", TTR("History Next"), KEY_MASK_ALT | KEY_RIGHT), WINDOW_NEXT);
	file_menu->get_popup()->add_separator();

	file_menu->get_popup()->add_submenu_item(TTR("Theme"), "Theme", FILE_THEME);

	theme_submenu = memnew(PopupMenu);
	theme_submenu->set_name("Theme");
	file_menu->get_popup()->add_child(theme_submenu);
	theme_submenu->connect("id_pressed", this, "_theme_option");
	theme_submenu->add_shortcut(ED_SHORTCUT("script_editor/import_theme", TTR("Import Theme...")), THEME_IMPORT);
	theme_submenu->add_shortcut(ED_SHORTCUT("script_editor/reload_theme", TTR("Reload Theme")), THEME_RELOAD);

	theme_submenu->add_separator();
	theme_submenu->add_shortcut(ED_SHORTCUT("script_editor/save_theme", TTR("Save Theme")), THEME_SAVE);
	theme_submenu->add_shortcut(ED_SHORTCUT("script_editor/save_theme_as", TTR("Save Theme As...")), THEME_SAVE_AS);

	file_menu->get_popup()->add_separator();
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/close_file", TTR("Close"), KEY_MASK_CMD | KEY_W), FILE_CLOSE);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/close_all", TTR("Close All")), CLOSE_ALL);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/close_other_tabs", TTR("Close Other Tabs")), CLOSE_OTHER_TABS);
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/close_docs", TTR("Close Docs")), CLOSE_DOCS);

	file_menu->get_popup()->add_separator();
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/run_file", TTR("Run"), KEY_MASK_CMD | KEY_MASK_SHIFT | KEY_X), FILE_RUN);

	file_menu->get_popup()->add_separator();
	file_menu->get_popup()->add_shortcut(ED_SHORTCUT("script_editor/toggle_scripts_panel", TTR("Toggle Scripts Panel"), KEY_MASK_CMD | KEY_BACKSLASH), TOGGLE_SCRIPTS_PANEL);
	file_menu->get_popup()->connect("id_pressed", this, "_menu_option");
	file_menu->get_popup()->connect("about_to_show", this, "_prepare_file_menu");

	script_search_menu = memnew(MenuButton);
	menu_hb->add_child(script_search_menu);
	script_search_menu->set_text(TTR("Search"));
	script_search_menu->set_switch_on_hover(true);
	script_search_menu->get_popup()->set_hide_on_window_lose_focus(true);
	script_search_menu->get_popup()->connect("id_pressed", this, "_menu_option");

	debug_menu = memnew(MenuButton);
	menu_hb->add_child(debug_menu);
	debug_menu->set_text(TTR("Debug"));
	debug_menu->set_switch_on_hover(true);
	debug_menu->get_popup()->set_hide_on_window_lose_focus(true);
	debug_menu->get_popup()->add_shortcut(ED_SHORTCUT("debugger/step_into", TTR("Step Into"), KEY_F11), DEBUG_STEP);
	debug_menu->get_popup()->add_shortcut(ED_SHORTCUT("debugger/step_over", TTR("Step Over"), KEY_F10), DEBUG_NEXT);
	debug_menu->get_popup()->add_separator();
	debug_menu->get_popup()->add_shortcut(ED_SHORTCUT("debugger/break", TTR("Break")), DEBUG_BREAK);
	debug_menu->get_popup()->add_shortcut(ED_SHORTCUT("debugger/continue", TTR("Continue"), KEY_F12), DEBUG_CONTINUE);
	debug_menu->get_popup()->add_separator();
	debug_menu->get_popup()->add_check_shortcut(ED_SHORTCUT("debugger/keep_debugger_open", TTR("Keep Debugger Open")), DEBUG_KEEP_DEBUGGER_OPEN);
	debug_menu->get_popup()->add_check_shortcut(ED_SHORTCUT("debugger/debug_with_external_editor", TTR("Debug with External Editor")), DEBUG_WITH_EXTERNAL_EDITOR);
	debug_menu->get_popup()->connect("id_pressed", this, "_menu_option");

	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_NEXT), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_STEP), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_BREAK), true);
	debug_menu->get_popup()->set_item_disabled(debug_menu->get_popup()->get_item_index(DEBUG_CONTINUE), true);

	menu_hb->add_spacer();

	script_icon = memnew(TextureRect);
	menu_hb->add_child(script_icon);
	script_name_label = memnew(Label);
	menu_hb->add_child(script_name_label);

	script_icon->hide();
	script_name_label->hide();

	menu_hb->add_spacer();

	help_search = memnew(ToolButton);
	help_search->set_text(TTR("Search Help"));
	help_search->connect("pressed", this, "_menu_option", varray(SEARCH_HELP));
	menu_hb->add_child(help_search);
	help_search->set_tooltip(TTR("Search the reference documentation."));

	menu_hb->add_child(memnew(VSeparator));

	script_back = memnew(ToolButton);
	script_back->connect("pressed", this, "_history_back");
	menu_hb->add_child(script_back);
	script_back->set_disabled(true);
	script_back->set_tooltip(TTR("Go to previous edited document."));

	script_forward = memnew(ToolButton);
	script_forward->connect("pressed", this, "_history_forward");
	menu_hb->add_child(script_forward);
	script_forward->set_disabled(true);
	script_forward->set_tooltip(TTR("Go to next edited document."));

	tab_container->connect("tab_changed", this, "_tab_changed");

	erase_tab_confirm = memnew(ConfirmationDialog);
	erase_tab_confirm->get_ok()->set_text(TTR("Save"));
	erase_tab_confirm->add_button(TTR("Discard"), OS::get_singleton()->get_swap_ok_cancel(), "discard");
	erase_tab_confirm->connect("confirmed", this, "_close_current_tab", varray(true));
	erase_tab_confirm->connect("custom_action", this, "_close_discard_current_tab");
	add_child(erase_tab_confirm);

	script_create_dialog = memnew(ScriptCreateDialog);
	script_create_dialog->set_title(TTR("Create Script"));
	add_child(script_create_dialog);
	script_create_dialog->connect("script_created", this, "_script_created");

	file_dialog_option = -1;
	file_dialog = memnew(EditorFileDialog);
	add_child(file_dialog);
	file_dialog->connect("file_selected", this, "_file_dialog_action");

	error_dialog = memnew(AcceptDialog);
	add_child(error_dialog);

	debugger = memnew(EditorScriptEditorDebugger(editor));
	debugger->connect("goto_script_line", this, "_goto_script_line");
	debugger->connect("set_execution", this, "_set_execution");
	debugger->connect("clear_execution", this, "_clear_execution");
	debugger->connect("show_debugger", this, "_show_debugger");

	disk_changed = memnew(ConfirmationDialog);
	{
		VBoxContainer *vbc = memnew(VBoxContainer);
		disk_changed->add_child(vbc);

		Label *dl = memnew(Label);
		dl->set_text(TTR("The following files are newer on disk.\nWhat action should be taken?:"));
		vbc->add_child(dl);

		disk_changed_list = memnew(Tree);
		vbc->add_child(disk_changed_list);
		disk_changed_list->set_v_size_flags(SIZE_EXPAND_FILL);

		disk_changed->connect("confirmed", this, "reload_scripts");
		disk_changed->get_ok()->set_text(TTR("Reload"));

		disk_changed->add_button(TTR("Resave"), !OS::get_singleton()->get_swap_ok_cancel(), "resave");
		disk_changed->connect("custom_action", this, "_resave_scripts");
	}

	add_child(disk_changed);

	script_editor = this;

	Button *db = EditorNode::get_singleton()->add_bottom_panel_item(TTR("Debugger"), debugger);
	db->set_shortcut(ED_SHORTCUT("debugger/toggle_debugger_panel", TTR("Toggle Debugger Panel"), KEY_MASK_CMD | KEY_MASK_ALT | KEY_4));
	// Add separation for the warning/error icon that is displayed later.
	db->add_theme_constant_override("hseparation", 6 * EDSCALE);
	debugger->set_tool_button(db);

	debugger->connect("breaked", this, "_breaked");

	autosave_timer = memnew(Timer);
	autosave_timer->set_one_shot(false);
	autosave_timer->connect(SceneStringNames::get_singleton()->tree_entered, this, "_update_autosave_timer");
	autosave_timer->connect("timeout", this, "_autosave_scripts");
	add_child(autosave_timer);

	grab_focus_block = false;

	help_search_dialog = memnew(EditorHelpSearch);
	add_child(help_search_dialog);
	help_search_dialog->connect("go_to_help", this, "_help_class_goto");

	find_in_files_dialog = memnew(FindInFilesDialog);
	find_in_files_dialog->connect(FindInFilesDialog::SIGNAL_FIND_REQUESTED, this, "_start_find_in_files", varray(false));
	find_in_files_dialog->connect(FindInFilesDialog::SIGNAL_REPLACE_REQUESTED, this, "_start_find_in_files", varray(true));
	add_child(find_in_files_dialog);
	find_in_files = memnew(FindInFilesPanel);
	find_in_files_button = editor->add_bottom_panel_item(TTR("Search Results"), find_in_files);
	find_in_files_button->set_shortcut(ED_SHORTCUT("script_editor/toggle_search_results_panel", TTR("Toggle Search Results Panel"), KEY_MASK_CMD | KEY_MASK_ALT | KEY_5));
	find_in_files->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
	find_in_files->connect(FindInFilesPanel::SIGNAL_RESULT_SELECTED, this, "_on_find_in_files_result_selected");
	find_in_files->connect(FindInFilesPanel::SIGNAL_FILES_MODIFIED, this, "_on_find_in_files_modified_files");
	find_in_files->hide();
	find_in_files_button->hide();

	history_pos = -1;
	//debugger_gui->hide();

	edit_pass = 0;
	trim_trailing_whitespace_on_save = EditorSettings::get_singleton()->get("text_editor/files/trim_trailing_whitespace_on_save");
	convert_indent_on_save = EditorSettings::get_singleton()->get("text_editor/indent/convert_indent_on_save");
	use_space_indentation = EditorSettings::get_singleton()->get("text_editor/indent/type");

	ScriptServer::edit_request_func = _open_script_request;

	add_theme_style_override("panel", editor->get_gui_base()->get_theme_stylebox("EditorScriptEditorPanel", "EditorStyles"));
	tab_container->add_theme_style_override("panel", editor->get_gui_base()->get_theme_stylebox("EditorScriptEditor", "EditorStyles"));
}

EditorScriptEditor::~EditorScriptEditor() {
	memdelete(completion_cache);
}
