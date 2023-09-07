/*************************************************************************/
/*  text_editor.cpp                                                      */
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

#include "editor_text_editor.h"

#include "core/error/error_macros.h"
#include "core/input/input_event.h"
#include "core/math/transform_2d.h"
#include "core/math/vector2.h"
#include "core/object/class_db.h"
#include "core/os/keyboard.h"
#include "core/os/memory.h"
#include "core/typedefs.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "scene/gui/box_container.h"
#include "scene/gui/control.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/text_edit.h"
#include "scene/resources/text_file.h"
#include "scene/resources/texture.h"

#include "editor_find_replace_bar.h"
#include "editor_goto_line_dialog.h"
#include "editor_script_editor.h"
#include "editor_syntax_highlighter.h"

void EditorTextEditor::add_syntax_highlighter(Ref<EditorSyntaxHighlighter> p_highlighter) {
	highlighters[p_highlighter->_get_name()] = p_highlighter;
	highlighter_menu->add_radio_check_item(p_highlighter->_get_name());
}

void EditorTextEditor::set_syntax_highlighter(Ref<EditorSyntaxHighlighter> p_highlighter) {
	TextEdit *te = code_editor->get_text_edit();
	te->set_syntax_highlighter(p_highlighter);
	highlighter_menu->set_item_checked(highlighter_menu->get_item_idx_from_text(p_highlighter->_get_name()), true);
}

void EditorTextEditor::_change_syntax_highlighter(int p_idx) {
	RBMap<String, Ref<EditorSyntaxHighlighter>>::Element *el = highlighters.front();
	while (el != nullptr) {
		highlighter_menu->set_item_checked(highlighter_menu->get_item_idx_from_text(el->key()), false);
		el = el->next();
	}
	set_syntax_highlighter(highlighters[highlighter_menu->get_item_text(p_idx)]);
}

void EditorTextEditor::_load_theme_settings() {
	TextEdit *text_edit = code_editor->get_text_edit();
	text_edit->get_syntax_highlighter()->update_cache();

	Color background_color = EDITOR_GET("text_editor/highlighting/background_color");
	Color completion_background_color = EDITOR_GET("text_editor/highlighting/completion_background_color");
	Color completion_selected_color = EDITOR_GET("text_editor/highlighting/completion_selected_color");
	Color completion_existing_color = EDITOR_GET("text_editor/highlighting/completion_existing_color");
	Color completion_scroll_color = EDITOR_GET("text_editor/highlighting/completion_scroll_color");
	Color completion_font_color = EDITOR_GET("text_editor/highlighting/completion_font_color");
	Color text_color = EDITOR_GET("text_editor/highlighting/text_color");
	Color line_number_color = EDITOR_GET("text_editor/highlighting/line_number_color");
	Color caret_color = EDITOR_GET("text_editor/highlighting/caret_color");
	Color caret_background_color = EDITOR_GET("text_editor/highlighting/caret_background_color");
	Color text_selected_color = EDITOR_GET("text_editor/highlighting/text_selected_color");
	Color selection_color = EDITOR_GET("text_editor/highlighting/selection_color");
	Color brace_mismatch_color = EDITOR_GET("text_editor/highlighting/brace_mismatch_color");
	Color current_line_color = EDITOR_GET("text_editor/highlighting/current_line_color");
	Color line_length_guideline_color = EDITOR_GET("text_editor/highlighting/line_length_guideline_color");
	Color word_highlighted_color = EDITOR_GET("text_editor/highlighting/word_highlighted_color");
	Color mark_color = EDITOR_GET("text_editor/highlighting/mark_color");
	Color bookmark_color = EDITOR_GET("text_editor/highlighting/bookmark_color");
	Color breakpoint_color = EDITOR_GET("text_editor/highlighting/breakpoint_color");
	Color executing_line_color = EDITOR_GET("text_editor/highlighting/executing_line_color");
	Color code_folding_color = EDITOR_GET("text_editor/highlighting/code_folding_color");
	Color search_result_color = EDITOR_GET("text_editor/highlighting/search_result_color");
	Color search_result_border_color = EDITOR_GET("text_editor/highlighting/search_result_border_color");

	text_edit->add_theme_color_override("background_color", background_color);
	text_edit->add_theme_color_override("completion_background_color", completion_background_color);
	text_edit->add_theme_color_override("completion_selected_color", completion_selected_color);
	text_edit->add_theme_color_override("completion_existing_color", completion_existing_color);
	text_edit->add_theme_color_override("completion_scroll_color", completion_scroll_color);
	text_edit->add_theme_color_override("completion_font_color", completion_font_color);
	text_edit->add_theme_color_override("font_color", text_color);
	text_edit->add_theme_color_override("line_number_color", line_number_color);
	text_edit->add_theme_color_override("caret_color", caret_color);
	text_edit->add_theme_color_override("caret_background_color", caret_background_color);
	text_edit->add_theme_color_override("font_color_selected", text_selected_color);
	text_edit->add_theme_color_override("selection_color", selection_color);
	text_edit->add_theme_color_override("brace_mismatch_color", brace_mismatch_color);
	text_edit->add_theme_color_override("current_line_color", current_line_color);
	text_edit->add_theme_color_override("line_length_guideline_color", line_length_guideline_color);
	text_edit->add_theme_color_override("word_highlighted_color", word_highlighted_color);
	text_edit->add_theme_color_override("breakpoint_color", breakpoint_color);
	text_edit->add_theme_color_override("executing_line_color", executing_line_color);
	text_edit->add_theme_color_override("mark_color", mark_color);
	text_edit->add_theme_color_override("bookmark_color", bookmark_color);
	text_edit->add_theme_color_override("code_folding_color", code_folding_color);
	text_edit->add_theme_color_override("search_result_color", search_result_color);
	text_edit->add_theme_color_override("search_result_border_color", search_result_border_color);

	text_edit->add_theme_constant_override("line_spacing", EDITOR_DEF("text_editor/theme/line_spacing", 6));
}

String EditorTextEditor::get_name() {
	String name;

	name = text_file->get_path().get_file();
	if (name.empty()) {
		// This appears for newly created built-in text_files before saving the scene.
		name = TTR("[unsaved]");
	} else if (text_file->get_path().find("local://") == -1 || text_file->get_path().find("::") == -1) {
		const String &text_file_name = text_file->get_name();
		if (text_file_name != "") {
			// If the built-in text_file has a custom resource name defined,
			// display the built-in text_file name as follows: `ResourceName (scene_file.tscn)`
			name = vformat("%s (%s)", text_file_name, name.get_slice("::", 0));
		}
	}

	if (is_unsaved()) {
		name += "(*)";
	}

	return name;
}

Ref<Texture> EditorTextEditor::get_icon() {
	return EditorNode::get_singleton()->get_object_icon(text_file.ptr(), "");
}

RES EditorTextEditor::get_edited_resource() const {
	return text_file;
}

void EditorTextEditor::set_edited_resource(const RES &p_res) {
	ERR_FAIL_COND(text_file.is_valid());
	ERR_FAIL_COND(p_res.is_null());

	text_file = p_res;

	code_editor->get_text_edit()->set_text(text_file->get_text());
	code_editor->get_text_edit()->clear_undo_history();
	code_editor->get_text_edit()->tag_saved_version();

	emit_signal("name_changed");
	code_editor->update_line_and_column();
}

void EditorTextEditor::enable_editor() {
	if (editor_enabled) {
		return;
	}

	editor_enabled = true;

	_load_theme_settings();
}

void EditorTextEditor::add_callback(const String &p_function, PoolStringArray p_args) {
}

void EditorTextEditor::set_debugger_active(bool p_active) {
}

void EditorTextEditor::get_breakpoints(List<int> *p_breakpoints) {
}

void EditorTextEditor::reload_text() {
	ERR_FAIL_COND(text_file.is_null());

	TextEdit *te = code_editor->get_text_edit();
	int column = te->cursor_get_column();
	int row = te->cursor_get_line();
	int h = te->get_h_scroll();
	int v = te->get_v_scroll();

	te->set_text(text_file->get_text());
	te->cursor_set_line(row);
	te->cursor_set_column(column);
	te->set_h_scroll(h);
	te->set_v_scroll(v);

	te->tag_saved_version();

	code_editor->update_line_and_column();
}

void EditorTextEditor::_validate_script() {
	emit_signal("name_changed");
	emit_signal("edited_script_changed");
}

void EditorTextEditor::_update_bookmark_list() {
	bookmarks_menu->clear();

	bookmarks_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/toggle_bookmark"), BOOKMARK_TOGGLE);
	bookmarks_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/remove_all_bookmarks"), BOOKMARK_REMOVE_ALL);
	bookmarks_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/goto_next_bookmark"), BOOKMARK_GOTO_NEXT);
	bookmarks_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/goto_previous_bookmark"), BOOKMARK_GOTO_PREV);

	Array bookmark_list = code_editor->get_text_edit()->get_bookmarks_array();
	if (bookmark_list.size() == 0) {
		return;
	}

	bookmarks_menu->add_separator();

	for (int i = 0; i < bookmark_list.size(); i++) {
		String line = code_editor->get_text_edit()->get_line(bookmark_list[i]).strip_edges();
		// Limit the size of the line if too big.
		if (line.length() > 50) {
			line = line.substr(0, 50);
		}

		bookmarks_menu->add_item(String::num((int)bookmark_list[i] + 1) + " - \"" + line + "\"");
		bookmarks_menu->set_item_metadata(bookmarks_menu->get_item_count() - 1, bookmark_list[i]);
	}
}

void EditorTextEditor::_bookmark_item_pressed(int p_idx) {
	if (p_idx < 4) { // Any item before the separator.
		_edit_option(bookmarks_menu->get_item_id(p_idx));
	} else {
		code_editor->goto_line(bookmarks_menu->get_item_metadata(p_idx));
	}
}

void EditorTextEditor::apply_code() {
	text_file->set_text(code_editor->get_text_edit()->get_text());
}

bool EditorTextEditor::is_unsaved() {
	return code_editor->get_text_edit()->get_version() != code_editor->get_text_edit()->get_saved_version();
}

Variant EditorTextEditor::get_edit_state() {
	return code_editor->get_edit_state();
}

void EditorTextEditor::set_edit_state(const Variant &p_state) {
	code_editor->set_edit_state(p_state);

	Dictionary state = p_state;
	if (state.has("syntax_highlighter")) {
		int idx = highlighter_menu->get_item_idx_from_text(state["syntax_highlighter"]);
		if (idx >= 0) {
			_change_syntax_highlighter(idx);
		}
	}

	ensure_focus();
}

void EditorTextEditor::trim_trailing_whitespace() {
	code_editor->trim_trailing_whitespace();
}

void EditorTextEditor::insert_final_newline() {
	code_editor->insert_final_newline();
}

void EditorTextEditor::convert_indent_to_spaces() {
	code_editor->convert_indent_to_spaces();
}

void EditorTextEditor::convert_indent_to_tabs() {
	code_editor->convert_indent_to_tabs();
}

void EditorTextEditor::tag_saved_version() {
	code_editor->get_text_edit()->tag_saved_version();
}

void EditorTextEditor::goto_line(int p_line, bool p_with_error) {
	code_editor->goto_line(p_line);
}

void EditorTextEditor::goto_line_selection(int p_line, int p_begin, int p_end) {
	code_editor->goto_line_selection(p_line, p_begin, p_end);
}

void EditorTextEditor::set_executing_line(int p_line) {
	code_editor->set_executing_line(p_line);
}

void EditorTextEditor::clear_executing_line() {
	code_editor->clear_executing_line();
}

void EditorTextEditor::ensure_focus() {
	code_editor->get_text_edit()->grab_focus();
}

Vector<String> EditorTextEditor::get_functions() {
	return Vector<String>();
}

bool EditorTextEditor::show_members_overview() {
	return true;
}

void EditorTextEditor::update_settings() {
	code_editor->update_editor_settings();
}

void EditorTextEditor::set_tooltip_request_func(String p_method, Object *p_obj) {
	code_editor->get_text_edit()->set_tooltip_request_func(p_obj, p_method, this);
}

Control *EditorTextEditor::get_edit_menu() {
	return edit_hb;
}

void EditorTextEditor::clear_edit_menu() {
	memdelete(edit_hb);
}

void EditorTextEditor::_edit_option(int p_op) {
	TextEdit *tx = code_editor->get_text_edit();

	switch (p_op) {
		case EDIT_UNDO: {
			tx->undo();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_REDO: {
			tx->redo();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_CUT: {
			tx->cut();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_COPY: {
			tx->copy();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_PASTE: {
			tx->paste();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_SELECT_ALL: {
			tx->select_all();
			tx->call_deferred("grab_focus");
		} break;
		case EDIT_MOVE_LINE_UP: {
			code_editor->move_lines_up();
		} break;
		case EDIT_MOVE_LINE_DOWN: {
			code_editor->move_lines_down();
		} break;
		case EDIT_INDENT_LEFT: {
			tx->indent_left();
		} break;
		case EDIT_INDENT_RIGHT: {
			tx->indent_right();
		} break;
		case EDIT_DELETE_LINE: {
			code_editor->delete_lines();
		} break;
		case EDIT_DUPLICATE_SELECTION: {
			code_editor->duplicate_selection();
		} break;
		case EDIT_TOGGLE_FOLD_LINE: {
			tx->toggle_fold_line(tx->cursor_get_line());
			tx->update();
		} break;
		case EDIT_FOLD_ALL_LINES: {
			tx->fold_all_lines();
			tx->update();
		} break;
		case EDIT_UNFOLD_ALL_LINES: {
			tx->unhide_all_lines();
			tx->update();
		} break;
		case EDIT_TRIM_TRAILING_WHITESAPCE: {
			trim_trailing_whitespace();
		} break;
		case EDIT_CONVERT_INDENT_TO_SPACES: {
			convert_indent_to_spaces();
		} break;
		case EDIT_CONVERT_INDENT_TO_TABS: {
			convert_indent_to_tabs();
		} break;
		case EDIT_TO_UPPERCASE: {
			_convert_case(EditorCodeTextEditor::UPPER);
		} break;
		case EDIT_TO_LOWERCASE: {
			_convert_case(EditorCodeTextEditor::LOWER);
		} break;
		case EDIT_CAPITALIZE: {
			_convert_case(EditorCodeTextEditor::CAPITALIZE);
		} break;
		case SEARCH_FIND: {
			code_editor->get_find_replace_bar()->popup_search();
		} break;
		case SEARCH_FIND_NEXT: {
			code_editor->get_find_replace_bar()->search_next();
		} break;
		case SEARCH_FIND_PREV: {
			code_editor->get_find_replace_bar()->search_prev();
		} break;
		case SEARCH_REPLACE: {
			code_editor->get_find_replace_bar()->popup_replace();
		} break;
		case SEARCH_IN_FILES: {
			String selected_text = code_editor->get_text_edit()->get_selection_text();

			// Yep, because it doesn't make sense to instance this dialog for every single script open...
			// So this will be delegated to the EditorScriptEditor.
			emit_signal("search_in_files_requested", selected_text);
		} break;
		case REPLACE_IN_FILES: {
			String selected_text = code_editor->get_text_edit()->get_selection_text();

			emit_signal("replace_in_files_requested", selected_text);
		} break;
		case SEARCH_GOTO_LINE: {
			goto_line_dialog->popup_find_line(tx);
		} break;
		case BOOKMARK_TOGGLE: {
			code_editor->toggle_bookmark();
		} break;
		case BOOKMARK_GOTO_NEXT: {
			code_editor->goto_next_bookmark();
		} break;
		case BOOKMARK_GOTO_PREV: {
			code_editor->goto_prev_bookmark();
		} break;
		case BOOKMARK_REMOVE_ALL: {
			code_editor->remove_all_bookmarks();
		} break;
	}
}

void EditorTextEditor::_convert_case(EditorCodeTextEditor::CaseStyle p_case) {
	code_editor->convert_case(p_case);
}

void EditorTextEditor::_bind_methods() {
	ClassDB::bind_method("_validate_script", &EditorTextEditor::_validate_script);
	ClassDB::bind_method("_update_bookmark_list", &EditorTextEditor::_update_bookmark_list);
	ClassDB::bind_method("_bookmark_item_pressed", &EditorTextEditor::_bookmark_item_pressed);
	ClassDB::bind_method("_load_theme_settings", &EditorTextEditor::_load_theme_settings);
	ClassDB::bind_method("_edit_option", &EditorTextEditor::_edit_option);
	ClassDB::bind_method("_change_syntax_highlighter", &EditorTextEditor::_change_syntax_highlighter);
	ClassDB::bind_method("_text_edit_gui_input", &EditorTextEditor::_text_edit_gui_input);
	ClassDB::bind_method("_prepare_edit_menu", &EditorTextEditor::_prepare_edit_menu);
}

EditorScriptEditorBase *EditorTextEditor::create_editor(const RES &p_resource) {
	if (Object::cast_to<TextFile>(*p_resource)) {
		return memnew(EditorTextEditor);
	}
	return nullptr;
}

void EditorTextEditor::register_editor() {
	EditorScriptEditor::register_create_script_editor_function(create_editor);
}

void EditorTextEditor::_text_edit_gui_input(const Ref<InputEvent> &ev) {
	Ref<InputEventMouseButton> mb = ev;

	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_RIGHT) {
			int col, row;
			TextEdit *tx = code_editor->get_text_edit();
			tx->_get_mouse_pos(mb->get_global_position() - tx->get_global_position(), row, col);

			tx->set_right_click_moves_caret(EditorSettings::get_singleton()->get("text_editor/cursor/right_click_moves_caret"));
			bool can_fold = tx->can_fold(row);
			bool is_folded = tx->is_folded(row);

			if (tx->is_right_click_moving_caret()) {
				if (tx->is_selection_active()) {
					int from_line = tx->get_selection_from_line();
					int to_line = tx->get_selection_to_line();
					int from_column = tx->get_selection_from_column();
					int to_column = tx->get_selection_to_column();

					if (row < from_line || row > to_line || (row == from_line && col < from_column) || (row == to_line && col > to_column)) {
						// Right click is outside the selected text.
						tx->deselect();
					}
				}
				if (!tx->is_selection_active()) {
					tx->cursor_set_line(row, true, false);
					tx->cursor_set_column(col);
				}
			}

			if (!mb->is_pressed()) {
				_make_context_menu(tx->is_selection_active(), can_fold, is_folded, get_local_mouse_position());
			}
		}
	}

	Ref<InputEventKey> k = ev;
	if (k.is_valid() && k->is_pressed() && k->get_scancode() == KEY_MENU) {
		TextEdit *tx = code_editor->get_text_edit();
		int line = tx->cursor_get_line();
		_make_context_menu(tx->is_selection_active(), tx->can_fold(line), tx->is_folded(line), (get_global_transform().inverse() * tx->get_global_transform()).xform(tx->_get_cursor_pixel_pos()));
		context_menu->grab_focus();
	}
}

void EditorTextEditor::_prepare_edit_menu() {
	const TextEdit *tx = code_editor->get_text_edit();
	PopupMenu *popup = edit_menu->get_popup();
	popup->set_item_disabled(popup->get_item_index(EDIT_UNDO), !tx->has_undo());
	popup->set_item_disabled(popup->get_item_index(EDIT_REDO), !tx->has_redo());
}

void EditorTextEditor::_make_context_menu(bool p_selection, bool p_can_fold, bool p_is_folded, Vector2 p_position) {
	context_menu->clear();
	if (p_selection) {
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/cut"), EDIT_CUT);
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/copy"), EDIT_COPY);
	}

	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/paste"), EDIT_PASTE);
	context_menu->add_separator();
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/select_all"), EDIT_SELECT_ALL);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/undo"), EDIT_UNDO);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/redo"), EDIT_REDO);
	context_menu->add_separator();
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/indent_left"), EDIT_INDENT_LEFT);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/indent_right"), EDIT_INDENT_RIGHT);
	context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/toggle_bookmark"), BOOKMARK_TOGGLE);

	if (p_selection) {
		context_menu->add_separator();
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/convert_to_uppercase"), EDIT_TO_UPPERCASE);
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/convert_to_lowercase"), EDIT_TO_LOWERCASE);
	}
	if (p_can_fold || p_is_folded) {
		context_menu->add_shortcut(ED_GET_SHORTCUT("script_text_editor/toggle_fold_line"), EDIT_TOGGLE_FOLD_LINE);
	}

	const TextEdit *tx = code_editor->get_text_edit();
	context_menu->set_item_disabled(context_menu->get_item_index(EDIT_UNDO), !tx->has_undo());
	context_menu->set_item_disabled(context_menu->get_item_index(EDIT_REDO), !tx->has_redo());

	context_menu->set_position(get_global_transform().xform(p_position));
	context_menu->set_size(Vector2(1, 1));
	context_menu->popup();
}

EditorTextEditor::EditorTextEditor() {
	editor_enabled = false;

	code_editor = memnew(EditorCodeTextEditor);
	add_child(code_editor);
	code_editor->add_theme_constant_override("separation", 0);
	code_editor->connect("load_theme_settings", this, "_load_theme_settings");
	code_editor->connect("validate_script", this, "_validate_script");
	code_editor->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	code_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	update_settings();

	code_editor->get_text_edit()->set_context_menu_enabled(false);
	code_editor->get_text_edit()->connect("gui_input", this, "_text_edit_gui_input");

	context_menu = memnew(PopupMenu);
	add_child(context_menu);
	context_menu->connect("id_pressed", this, "_edit_option");

	edit_hb = memnew(HBoxContainer);

	search_menu = memnew(MenuButton);
	search_menu->set_shortcut_context(this);
	edit_hb->add_child(search_menu);
	search_menu->set_text(TTR("Search"));
	search_menu->set_switch_on_hover(true);
	search_menu->get_popup()->connect("id_pressed", this, "_edit_option");

	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/find"), SEARCH_FIND);
	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/find_next"), SEARCH_FIND_NEXT);
	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/find_previous"), SEARCH_FIND_PREV);
	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/replace"), SEARCH_REPLACE);
	search_menu->get_popup()->add_separator();
	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/find_in_files"), SEARCH_IN_FILES);
	search_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/replace_in_files"), REPLACE_IN_FILES);

	edit_menu = memnew(MenuButton);
	edit_menu->set_shortcut_context(this);
	edit_hb->add_child(edit_menu);
	edit_menu->set_text(TTR("Edit"));
	edit_menu->set_switch_on_hover(true);
	edit_menu->connect("about_to_show", this, "_prepare_edit_menu");
	edit_menu->get_popup()->connect("id_pressed", this, "_edit_option");

	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/undo"), EDIT_UNDO);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/redo"), EDIT_REDO);
	edit_menu->get_popup()->add_separator();
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/cut"), EDIT_CUT);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/copy"), EDIT_COPY);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/paste"), EDIT_PASTE);
	edit_menu->get_popup()->add_separator();
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/select_all"), EDIT_SELECT_ALL);
	edit_menu->get_popup()->add_separator();
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/move_up"), EDIT_MOVE_LINE_UP);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/move_down"), EDIT_MOVE_LINE_DOWN);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/indent_left"), EDIT_INDENT_LEFT);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/indent_right"), EDIT_INDENT_RIGHT);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/delete_line"), EDIT_DELETE_LINE);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/toggle_fold_line"), EDIT_TOGGLE_FOLD_LINE);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/fold_all_lines"), EDIT_FOLD_ALL_LINES);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/unfold_all_lines"), EDIT_UNFOLD_ALL_LINES);
	edit_menu->get_popup()->add_separator();
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/duplicate_selection"), EDIT_DUPLICATE_SELECTION);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/trim_trailing_whitespace"), EDIT_TRIM_TRAILING_WHITESAPCE);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/convert_indent_to_spaces"), EDIT_CONVERT_INDENT_TO_SPACES);
	edit_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/convert_indent_to_tabs"), EDIT_CONVERT_INDENT_TO_TABS);

	edit_menu->get_popup()->add_separator();
	PopupMenu *convert_case = memnew(PopupMenu);
	convert_case->set_name("convert_case");
	edit_menu->get_popup()->add_child(convert_case);
	edit_menu->get_popup()->add_submenu_item(TTR("Convert Case"), "convert_case");
	convert_case->add_shortcut(ED_SHORTCUT("script_text_editor/convert_to_uppercase", TTR("Uppercase")), EDIT_TO_UPPERCASE);
	convert_case->add_shortcut(ED_SHORTCUT("script_text_editor/convert_to_lowercase", TTR("Lowercase")), EDIT_TO_LOWERCASE);
	convert_case->add_shortcut(ED_SHORTCUT("script_text_editor/capitalize", TTR("Capitalize")), EDIT_CAPITALIZE);
	convert_case->connect("id_pressed", this, "_edit_option");

	highlighter_menu = memnew(PopupMenu);
	highlighter_menu->set_name("highlighter_menu");
	edit_menu->get_popup()->add_child(highlighter_menu);
	edit_menu->get_popup()->add_submenu_item(TTR("Syntax Highlighter"), "highlighter_menu");
	highlighter_menu->connect("id_pressed", this, "_change_syntax_highlighter");

	Ref<EditorPlainTextSyntaxHighlighter> plain_highlighter;
	plain_highlighter.instance();
	add_syntax_highlighter(plain_highlighter);

	Ref<EditorStandardSyntaxHighlighter> highlighter;
	highlighter.instance();
	add_syntax_highlighter(highlighter);
	set_syntax_highlighter(plain_highlighter);

	MenuButton *goto_menu = memnew(MenuButton);
	goto_menu->set_shortcut_context(this);
	edit_hb->add_child(goto_menu);
	goto_menu->set_text(TTR("Go To"));
	goto_menu->set_switch_on_hover(true);
	goto_menu->get_popup()->connect("id_pressed", this, "_edit_option");

	goto_menu->get_popup()->add_shortcut(ED_GET_SHORTCUT("script_text_editor/goto_line"), SEARCH_GOTO_LINE);
	goto_menu->get_popup()->add_separator();

	bookmarks_menu = memnew(PopupMenu);
	bookmarks_menu->set_name("Bookmarks");
	goto_menu->get_popup()->add_child(bookmarks_menu);
	goto_menu->get_popup()->add_submenu_item(TTR("Bookmarks"), "Bookmarks");
	_update_bookmark_list();
	bookmarks_menu->connect("about_to_show", this, "_update_bookmark_list");
	bookmarks_menu->connect("index_pressed", this, "_bookmark_item_pressed");

	goto_line_dialog = memnew(EditorGotoLineDialog);
	add_child(goto_line_dialog);

	code_editor->get_text_edit()->set_drag_forwarding(this);
}

EditorTextEditor::~EditorTextEditor() {
	highlighters.clear();
}

void EditorTextEditor::validate() {
}
