/*************************************************************************/
/*  editor_script_editor_quick_open.cpp                                  */
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

#include "editor_script_editor_quick_open.h"

#include "core/os/keyboard.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/tree.h"

void EditorScriptEditorQuickOpen::popup_dialog(const Vector<String> &p_functions, bool p_dontclear) {
	popup_centered_ratio(0.6);
	if (p_dontclear) {
		search_box->select_all();
	} else {
		search_box->clear();
	}
	search_box->grab_focus();
	functions = p_functions;
	_update_search();
}

void EditorScriptEditorQuickOpen::_text_changed(const String &p_newtext) {
	_update_search();
}

void EditorScriptEditorQuickOpen::_sbox_input(const Ref<InputEvent> &p_ie) {
	Ref<InputEventKey> k = p_ie;

	if (k.is_valid() && (k->get_scancode() == KEY_UP || k->get_scancode() == KEY_DOWN || k->get_scancode() == KEY_PAGEUP || k->get_scancode() == KEY_PAGEDOWN)) {
		search_options->call("_gui_input", k);
		search_box->accept_event();
	}
}

void EditorScriptEditorQuickOpen::_update_search() {
	search_options->clear();
	TreeItem *root = search_options->create_item();

	for (int i = 0; i < functions.size(); i++) {
		String file = functions[i];
		if ((search_box->get_text() == "" || file.findn(search_box->get_text()) != -1)) {
			TreeItem *ti = search_options->create_item(root);
			ti->set_text(0, file);
			if (root->get_children() == ti) {
				ti->select(0);
			}
		}
	}

	get_ok()->set_disabled(root->get_children() == nullptr);
}

void EditorScriptEditorQuickOpen::_confirmed() {
	TreeItem *ti = search_options->get_selected();
	if (!ti) {
		return;
	}
	int line = ti->get_text(0).get_slice(":", 1).to_int();

	emit_signal("goto_line", line - 1);
	hide();
}

void EditorScriptEditorQuickOpen::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			connect("confirmed", this, "_confirmed");

			search_box->set_clear_button_enabled(true);
			FALLTHROUGH;
		}
		case NOTIFICATION_THEME_CHANGED: {
			search_box->set_right_icon(get_theme_icon("Search", "EditorIcons"));
		} break;
		case NOTIFICATION_EXIT_TREE: {
			disconnect("confirmed", this, "_confirmed");
		} break;
	}
}

void EditorScriptEditorQuickOpen::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_text_changed"), &EditorScriptEditorQuickOpen::_text_changed);
	ClassDB::bind_method(D_METHOD("_confirmed"), &EditorScriptEditorQuickOpen::_confirmed);
	ClassDB::bind_method(D_METHOD("_sbox_input"), &EditorScriptEditorQuickOpen::_sbox_input);

	ADD_SIGNAL(MethodInfo("goto_line", PropertyInfo(Variant::INT, "line")));
}

EditorScriptEditorQuickOpen::EditorScriptEditorQuickOpen() {
	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	search_box = memnew(LineEdit);
	vbc->add_margin_child(TTR("Search:"), search_box);
	search_box->connect("text_changed", this, "_text_changed");
	search_box->connect("gui_input", this, "_sbox_input");
	search_options = memnew(Tree);
	vbc->add_margin_child(TTR("Matches:"), search_options, true);
	get_ok()->set_text(TTR("Open"));
	get_ok()->set_disabled(true);
	register_text_enter(search_box);
	set_hide_on_ok(false);
	search_options->connect("item_activated", this, "_confirmed");
	search_options->set_hide_root(true);
	search_options->set_hide_folding(true);
	search_options->add_theme_constant_override("draw_guides", 1);
}
