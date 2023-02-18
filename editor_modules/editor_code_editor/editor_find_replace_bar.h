#ifndef EDITOR_FIND_REPLACE_BAR_H
#define EDITOR_FIND_REPLACE_BAR_H

/*************************************************************************/
/*  code_editor.h                                                        */
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

#include "scene/gui/dialogs.h"
#include "scene/gui/box_container.h"

#include "core/containers/list.h"
#include "core/math/math_defs.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class Button;
class CheckBox;
class InputEvent;
class Label;
class LineEdit;
class TextEdit;
class Texture;
class TextureButton;
class ToolButton;

class FindReplaceBar : public HBoxContainer {
	GDCLASS(FindReplaceBar, HBoxContainer);

	LineEdit *search_text;
	Label *matches_label;
	ToolButton *find_prev;
	ToolButton *find_next;
	CheckBox *case_sensitive;
	CheckBox *whole_words;
	TextureButton *hide_button;

	LineEdit *replace_text;
	Button *replace;
	Button *replace_all;
	CheckBox *selection_only;

	VBoxContainer *vbc_lineedit;
	HBoxContainer *hbc_button_replace;
	HBoxContainer *hbc_option_replace;

	TextEdit *text_edit;

	int result_line;
	int result_col;
	int results_count;

	bool replace_all_mode;
	bool preserve_cursor;

	void _get_search_from(int &r_line, int &r_col);
	void _update_results_count();
	void _update_matches_label();

	void _show_search(bool p_focus_replace = false, bool p_show_only = false);
	void _hide_bar();

	void _editor_text_changed();
	void _search_options_changed(bool p_pressed);
	void _search_text_changed(const String &p_text);
	void _search_text_entered(const String &p_text);
	void _replace_text_entered(const String &p_text);

protected:
	void _notification(int p_what);
	void _unhandled_input(const Ref<InputEvent> &p_event);

	bool _search(uint32_t p_flags, int p_from_line, int p_from_col);

	void _replace();
	void _replace_all();

	static void _bind_methods();

public:
	String get_search_text() const;
	String get_replace_text() const;

	bool is_case_sensitive() const;
	bool is_whole_words() const;
	bool is_selection_only() const;
	void set_error(const String &p_label);

	void set_text_edit(TextEdit *p_text_edit);

	void popup_search(bool p_show_only = false);
	void popup_replace();

	bool search_current();
	bool search_prev();
	bool search_next();

	FindReplaceBar();
};

#endif // CODE_EDITOR_H
