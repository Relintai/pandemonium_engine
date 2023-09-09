/*************************************************************************/
/*  code_editor.cpp                                                      */
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

#include "editor_find_replace_bar.h"

#include "core/input/input.h"
#include "core/input/input_event.h"
#include "core/input/shortcut.h"
#include "core/math/math_funcs.h"
#include "core/os/keyboard.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/tool_button.h"
#include "scene/resources/font.h"
#include "scene/resources/texture.h"

void EditorFindReplaceBar::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		find_prev->set_icon(get_theme_icon("MoveUp", "EditorIcons"));
		find_next->set_icon(get_theme_icon("MoveDown", "EditorIcons"));
		hide_button->set_normal_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_hover_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_pressed_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_custom_minimum_size(hide_button->get_normal_texture()->get_size());
	} else if (p_what == NOTIFICATION_VISIBILITY_CHANGED) {
		set_process_unhandled_input(is_visible_in_tree());
	} else if (p_what == EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED) {
		find_prev->set_icon(get_theme_icon("MoveUp", "EditorIcons"));
		find_next->set_icon(get_theme_icon("MoveDown", "EditorIcons"));
		hide_button->set_normal_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_hover_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_pressed_texture(get_theme_icon("Close", "EditorIcons"));
		hide_button->set_custom_minimum_size(hide_button->get_normal_texture()->get_size());
	} else if (p_what == NOTIFICATION_THEME_CHANGED) {
		matches_label->add_theme_color_override("font_color", results_count > 0 ? get_theme_color("font_color", "Label") : get_theme_color("error_color", "Editor"));
	}
}

void EditorFindReplaceBar::_unhandled_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventKey> k = p_event;
	if (!k.is_valid() || !k->is_pressed()) {
		return;
	}

	Control *focus_owner = get_focus_owner();
	if (text_edit->has_focus() || (focus_owner && vbc_lineedit->is_a_parent_of(focus_owner))) {
		bool accepted = true;

		switch (k->get_scancode()) {
			case KEY_ESCAPE: {
				_hide_bar();
			} break;
			default: {
				accepted = false;
			} break;
		}

		if (accepted) {
			accept_event();
		}
	}
}

bool EditorFindReplaceBar::_search(uint32_t p_flags, int p_from_line, int p_from_col) {
	int line, col;
	String text = get_search_text();

	bool found = text_edit->search(text, p_flags, p_from_line, p_from_col, line, col);

	if (found) {
		if (!preserve_cursor && !is_selection_only()) {
			text_edit->unfold_line(line);
			text_edit->cursor_set_line(line, false);
			text_edit->cursor_set_column(col + text.length(), false);
			text_edit->center_viewport_to_cursor();
			text_edit->select(line, col, line, col + text.length());
		}

		text_edit->set_search_text(text);
		text_edit->set_search_flags(p_flags);
		text_edit->set_current_search_result(line, col);

		result_line = line;
		result_col = col;

		_update_results_count();
	} else {
		results_count = 0;
		result_line = -1;
		result_col = -1;
		text_edit->set_search_text("");
		text_edit->set_search_flags(p_flags);
		text_edit->set_current_search_result(line, col);
	}

	_update_matches_label();

	return found;
}

void EditorFindReplaceBar::_replace() {
	bool selection_enabled = text_edit->is_selection_active();
	Point2i selection_begin, selection_end;
	if (selection_enabled) {
		selection_begin = Point2i(text_edit->get_selection_from_line(), text_edit->get_selection_from_column());
		selection_end = Point2i(text_edit->get_selection_to_line(), text_edit->get_selection_to_column());
	}

	String replace_text = get_replace_text();
	int search_text_len = get_search_text().length();

	text_edit->begin_complex_operation();
	if (selection_enabled && is_selection_only()) { // To restrict search_current() to selected region
		text_edit->cursor_set_line(selection_begin.width);
		text_edit->cursor_set_column(selection_begin.height);
	}

	if (search_current()) {
		text_edit->unfold_line(result_line);
		text_edit->select(result_line, result_col, result_line, result_col + search_text_len);

		if (selection_enabled && is_selection_only()) {
			Point2i match_from(result_line, result_col);
			Point2i match_to(result_line, result_col + search_text_len);
			if (!(match_from < selection_begin || match_to > selection_end)) {
				text_edit->insert_text_at_cursor(replace_text);
				if (match_to.x == selection_end.x) { // Adjust selection bounds if necessary
					selection_end.y += replace_text.length() - search_text_len;
				}
			}
		} else {
			text_edit->insert_text_at_cursor(replace_text);
		}
	}
	text_edit->end_complex_operation();
	results_count = -1;

	if (selection_enabled && is_selection_only()) {
		// Reselect in order to keep 'Replace' restricted to selection
		text_edit->select(selection_begin.x, selection_begin.y, selection_end.x, selection_end.y);
	} else {
		text_edit->deselect();
	}
}

void EditorFindReplaceBar::_replace_all() {
	text_edit->disconnect("text_changed", this, "_editor_text_changed");
	// Line as x so it gets priority in comparison, column as y.
	Point2i orig_cursor(text_edit->cursor_get_line(), text_edit->cursor_get_column());
	Point2i prev_match = Point2(-1, -1);

	bool selection_enabled = text_edit->is_selection_active();
	Point2i selection_begin, selection_end;
	if (selection_enabled) {
		selection_begin = Point2i(text_edit->get_selection_from_line(), text_edit->get_selection_from_column());
		selection_end = Point2i(text_edit->get_selection_to_line(), text_edit->get_selection_to_column());
	}

	int vsval = text_edit->get_v_scroll();

	text_edit->cursor_set_line(0);
	text_edit->cursor_set_column(0);

	String replace_text = get_replace_text();
	int search_text_len = get_search_text().length();

	int rc = 0;

	replace_all_mode = true;

	text_edit->begin_complex_operation();

	if (selection_enabled && is_selection_only()) {
		text_edit->cursor_set_line(selection_begin.width);
		text_edit->cursor_set_column(selection_begin.height);
	}
	if (search_current()) {
		do {
			// replace area
			Point2i match_from(result_line, result_col);
			Point2i match_to(result_line, result_col + search_text_len);

			if (match_from < prev_match) {
				break; // Done.
			}

			prev_match = Point2i(result_line, result_col + replace_text.length());

			text_edit->unfold_line(result_line);
			text_edit->select(result_line, result_col, result_line, match_to.y);

			if (selection_enabled && is_selection_only()) {
				if (match_from < selection_begin || match_to > selection_end) {
					break; // Done.
				}

				// Replace but adjust selection bounds.
				text_edit->insert_text_at_cursor(replace_text);
				if (match_to.x == selection_end.x) {
					selection_end.y += replace_text.length() - search_text_len;
				}

			} else {
				// Just replace.
				text_edit->insert_text_at_cursor(replace_text);
			}

			rc++;
		} while (search_next());
	}

	text_edit->end_complex_operation();

	replace_all_mode = false;

	// Restore editor state (selection, cursor, scroll).
	text_edit->cursor_set_line(orig_cursor.x);
	text_edit->cursor_set_column(orig_cursor.y);

	if (selection_enabled && is_selection_only()) {
		// Reselect.
		text_edit->select(selection_begin.x, selection_begin.y, selection_end.x, selection_end.y);
	} else {
		text_edit->deselect();
	}

	text_edit->set_v_scroll(vsval);
	matches_label->add_theme_color_override("font_color", rc > 0 ? get_theme_color("font_color", "Label") : get_theme_color("error_color", "Editor"));
	matches_label->set_text(vformat(TTR("%d replaced."), rc));

	text_edit->call_deferred("connect", "text_changed", this, "_editor_text_changed");
	results_count = -1;
}

void EditorFindReplaceBar::_get_search_from(int &r_line, int &r_col) {
	r_line = text_edit->cursor_get_line();
	r_col = text_edit->cursor_get_column();

	if (text_edit->is_selection_active() && is_selection_only()) {
		return;
	}

	if (r_line == result_line && r_col >= result_col && r_col <= result_col + get_search_text().length()) {
		r_col = result_col;
	}
}

void EditorFindReplaceBar::_update_results_count() {
	if (results_count != -1) {
		return;
	}

	results_count = 0;

	String searched = get_search_text();
	if (searched.empty()) {
		return;
	}

	String full_text = text_edit->get_text();

	int from_pos = 0;

	while (true) {
		int pos = is_case_sensitive() ? full_text.find(searched, from_pos) : full_text.findn(searched, from_pos);
		if (pos == -1) {
			break;
		}

		int pos_subsequent = pos + searched.length();
		if (is_whole_words()) {
			from_pos = pos + 1; // Making sure we won't hit the same match next time, if we get out via a continue.
			if (pos > 0 && !(is_symbol(full_text[pos - 1]) || full_text[pos - 1] == '\n')) {
				continue;
			}
			if (pos_subsequent < full_text.length() && !(is_symbol(full_text[pos_subsequent]) || full_text[pos_subsequent] == '\n')) {
				continue;
			}
		}

		results_count++;
		from_pos = pos_subsequent;
	}
}

void EditorFindReplaceBar::_update_matches_label() {
	if (search_text->get_text().empty() || results_count == -1) {
		matches_label->hide();
	} else {
		matches_label->show();

		matches_label->add_theme_color_override("font_color", results_count > 0 ? get_theme_color("font_color", "Label") : get_theme_color("error_color", "Editor"));
		matches_label->set_text(vformat(results_count == 1 ? TTR("%d match.") : TTR("%d matches."), results_count));
	}
}

bool EditorFindReplaceBar::search_current() {
	uint32_t flags = 0;

	if (is_whole_words()) {
		flags |= TextEdit::SEARCH_WHOLE_WORDS;
	}
	if (is_case_sensitive()) {
		flags |= TextEdit::SEARCH_MATCH_CASE;
	}

	int line, col;
	_get_search_from(line, col);

	return _search(flags, line, col);
}

bool EditorFindReplaceBar::search_prev() {
	if (!is_visible()) {
		popup_search(true);
	}

	uint32_t flags = 0;
	String text = get_search_text();

	if (is_whole_words()) {
		flags |= TextEdit::SEARCH_WHOLE_WORDS;
	}
	if (is_case_sensitive()) {
		flags |= TextEdit::SEARCH_MATCH_CASE;
	}

	flags |= TextEdit::SEARCH_BACKWARDS;

	int line, col;
	_get_search_from(line, col);
	if (text_edit->is_selection_active()) {
		col--; // Skip currently selected word.
	}

	col -= text.length();
	if (col < 0) {
		line -= 1;
		if (line < 0) {
			line = text_edit->get_line_count() - 1;
		}
		col = text_edit->get_line(line).length();
	}

	return _search(flags, line, col);
}

bool EditorFindReplaceBar::search_next() {
	if (!is_visible()) {
		popup_search(true);
	}

	uint32_t flags = 0;
	String text;
	if (replace_all_mode) {
		text = get_replace_text();
	} else {
		text = get_search_text();
	}

	if (is_whole_words()) {
		flags |= TextEdit::SEARCH_WHOLE_WORDS;
	}
	if (is_case_sensitive()) {
		flags |= TextEdit::SEARCH_MATCH_CASE;
	}

	int line, col;
	_get_search_from(line, col);

	if (line == result_line && col == result_col) {
		col += text.length();
		if (col > text_edit->get_line(line).length()) {
			line += 1;
			if (line >= text_edit->get_line_count()) {
				line = 0;
			}
			col = 0;
		}
	}

	return _search(flags, line, col);
}

void EditorFindReplaceBar::_hide_bar() {
	if (replace_text->has_focus() || search_text->has_focus()) {
		text_edit->grab_focus();
	}

	text_edit->set_search_text("");
	result_line = -1;
	result_col = -1;
	hide();
}

void EditorFindReplaceBar::_show_search(bool p_focus_replace, bool p_show_only) {
	show();
	if (p_show_only) {
		return;
	}

	if (p_focus_replace) {
		search_text->deselect();
		replace_text->call_deferred("grab_focus");
	} else {
		replace_text->deselect();
		search_text->call_deferred("grab_focus");
	}

	if (text_edit->is_selection_active() && !selection_only->is_pressed()) {
		search_text->set_text(text_edit->get_selection_text());
	}

	if (!get_search_text().empty()) {
		if (p_focus_replace) {
			replace_text->select_all();
			replace_text->set_cursor_position(replace_text->get_text().length());
		} else {
			search_text->select_all();
			search_text->set_cursor_position(search_text->get_text().length());
		}

		results_count = -1;
		_update_results_count();
		_update_matches_label();
	}
}

void EditorFindReplaceBar::popup_search(bool p_show_only) {
	replace_text->hide();
	hbc_button_replace->hide();
	hbc_option_replace->hide();

	_show_search(false, p_show_only);
}

void EditorFindReplaceBar::popup_replace() {
	if (!replace_text->is_visible_in_tree()) {
		replace_text->show();
		hbc_button_replace->show();
		hbc_option_replace->show();
	}

	selection_only->set_pressed((text_edit->is_selection_active() && text_edit->get_selection_from_line() < text_edit->get_selection_to_line()));

	_show_search(is_visible() || text_edit->is_selection_active());
}

void EditorFindReplaceBar::_search_options_changed(bool p_pressed) {
	results_count = -1;
	search_current();
}

void EditorFindReplaceBar::_editor_text_changed() {
	results_count = -1;
	if (is_visible_in_tree()) {
		preserve_cursor = true;
		search_current();
		preserve_cursor = false;
	}
}

void EditorFindReplaceBar::_search_text_changed(const String &p_text) {
	results_count = -1;
	search_current();
}

void EditorFindReplaceBar::_search_text_entered(const String &p_text) {
	if (Input::get_singleton()->is_key_pressed(KEY_SHIFT)) {
		search_prev();
	} else {
		search_next();
	}
}

void EditorFindReplaceBar::_replace_text_entered(const String &p_text) {
	if (selection_only->is_pressed() && text_edit->is_selection_active()) {
		_replace_all();
		_hide_bar();
	}
}

String EditorFindReplaceBar::get_search_text() const {
	return search_text->get_text();
}

String EditorFindReplaceBar::get_replace_text() const {
	return replace_text->get_text();
}

bool EditorFindReplaceBar::is_case_sensitive() const {
	return case_sensitive->is_pressed();
}

bool EditorFindReplaceBar::is_whole_words() const {
	return whole_words->is_pressed();
}

bool EditorFindReplaceBar::is_selection_only() const {
	return selection_only->is_pressed();
}

void EditorFindReplaceBar::set_error(const String &p_label) {
	emit_signal("error", p_label);
}

void EditorFindReplaceBar::set_text_edit(TextEdit *p_text_edit) {
	results_count = -1;
	text_edit = p_text_edit;
	text_edit->connect("text_changed", this, "_editor_text_changed");
}

void EditorFindReplaceBar::_bind_methods() {
	ClassDB::bind_method("_unhandled_input", &EditorFindReplaceBar::_unhandled_input);

	ClassDB::bind_method("_editor_text_changed", &EditorFindReplaceBar::_editor_text_changed);
	ClassDB::bind_method("_search_text_changed", &EditorFindReplaceBar::_search_text_changed);
	ClassDB::bind_method("_search_text_entered", &EditorFindReplaceBar::_search_text_entered);
	ClassDB::bind_method("_replace_text_entered", &EditorFindReplaceBar::_replace_text_entered);
	ClassDB::bind_method("_search_current", &EditorFindReplaceBar::search_current);
	ClassDB::bind_method("_search_next", &EditorFindReplaceBar::search_next);
	ClassDB::bind_method("_search_prev", &EditorFindReplaceBar::search_prev);
	ClassDB::bind_method("_replace_pressed", &EditorFindReplaceBar::_replace);
	ClassDB::bind_method("_replace_all_pressed", &EditorFindReplaceBar::_replace_all);
	ClassDB::bind_method("_search_options_changed", &EditorFindReplaceBar::_search_options_changed);
	ClassDB::bind_method("_hide_pressed", &EditorFindReplaceBar::_hide_bar);

	ADD_SIGNAL(MethodInfo("search"));
	ADD_SIGNAL(MethodInfo("error"));
}

EditorFindReplaceBar::EditorFindReplaceBar() {
	results_count = -1;
	replace_all_mode = false;
	preserve_cursor = false;

	vbc_lineedit = memnew(VBoxContainer);
	add_child(vbc_lineedit);
	vbc_lineedit->set_alignment(ALIGN_CENTER);
	vbc_lineedit->set_h_size_flags(SIZE_EXPAND_FILL);
	VBoxContainer *vbc_button = memnew(VBoxContainer);
	add_child(vbc_button);
	VBoxContainer *vbc_option = memnew(VBoxContainer);
	add_child(vbc_option);

	HBoxContainer *hbc_button_search = memnew(HBoxContainer);
	vbc_button->add_child(hbc_button_search);
	hbc_button_search->set_alignment(ALIGN_END);
	hbc_button_replace = memnew(HBoxContainer);
	vbc_button->add_child(hbc_button_replace);
	hbc_button_replace->set_alignment(ALIGN_END);

	HBoxContainer *hbc_option_search = memnew(HBoxContainer);
	vbc_option->add_child(hbc_option_search);
	hbc_option_replace = memnew(HBoxContainer);
	vbc_option->add_child(hbc_option_replace);

	// search toolbar
	search_text = memnew(LineEdit);
	vbc_lineedit->add_child(search_text);
	search_text->set_custom_minimum_size(Size2(100 * EDSCALE, 0));
	search_text->connect("text_changed", this, "_search_text_changed");
	search_text->connect("text_entered", this, "_search_text_entered");

	matches_label = memnew(Label);
	hbc_button_search->add_child(matches_label);
	matches_label->hide();

	find_prev = memnew(ToolButton);
	hbc_button_search->add_child(find_prev);
	find_prev->set_focus_mode(FOCUS_NONE);
	find_prev->connect("pressed", this, "_search_prev");

	find_next = memnew(ToolButton);
	hbc_button_search->add_child(find_next);
	find_next->set_focus_mode(FOCUS_NONE);
	find_next->connect("pressed", this, "_search_next");

	case_sensitive = memnew(CheckBox);
	hbc_option_search->add_child(case_sensitive);
	case_sensitive->set_text(TTR("Match Case"));
	case_sensitive->set_focus_mode(FOCUS_NONE);
	case_sensitive->set_pressed(true);
	case_sensitive->connect("toggled", this, "_search_options_changed");

	whole_words = memnew(CheckBox);
	hbc_option_search->add_child(whole_words);
	whole_words->set_text(TTR("Whole Words"));
	whole_words->set_focus_mode(FOCUS_NONE);
	whole_words->connect("toggled", this, "_search_options_changed");

	// replace toolbar
	replace_text = memnew(LineEdit);
	vbc_lineedit->add_child(replace_text);
	replace_text->set_custom_minimum_size(Size2(100 * EDSCALE, 0));
	replace_text->connect("text_entered", this, "_replace_text_entered");

	replace = memnew(Button);
	hbc_button_replace->add_child(replace);
	replace->set_text(TTR("Replace"));
	replace->connect("pressed", this, "_replace_pressed");

	replace_all = memnew(Button);
	hbc_button_replace->add_child(replace_all);
	replace_all->set_text(TTR("Replace All"));
	replace_all->connect("pressed", this, "_replace_all_pressed");

	selection_only = memnew(CheckBox);
	hbc_option_replace->add_child(selection_only);
	selection_only->set_text(TTR("Selection Only"));
	selection_only->set_focus_mode(FOCUS_NONE);
	selection_only->connect("toggled", this, "_search_options_changed");

	hide_button = memnew(TextureButton);
	add_child(hide_button);
	hide_button->set_focus_mode(FOCUS_NONE);
	hide_button->connect("pressed", this, "_hide_pressed");
	hide_button->set_v_size_flags(SIZE_SHRINK_CENTER);
}
