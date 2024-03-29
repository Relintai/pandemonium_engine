#ifndef SCRIPT_CREATE_DIALOG_H
#define SCRIPT_CREATE_DIALOG_H

/*************************************************************************/
/*  script_create_dialog.h                                               */
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

#include "scene/gui/dialogs.h"

#include "core/containers/rb_map.h"
#include "core/containers/vector.h"
#include "core/object/object.h"
#include "core/string/ustring.h"

class CreateDialog;
class Button;
class CheckBox;
class Control;
class EditorFileDialog;
class Label;
class LineEdit;
class OptionButton;
class PanelContainer;
class VBoxContainer;

class ScriptCreateDialog : public ConfirmationDialog {
	GDCLASS(ScriptCreateDialog, ConfirmationDialog);

	LineEdit *class_name;
	Label *error_label;
	Label *path_error_label;
	Label *builtin_warning_label;
	Label *script_name_warning_label;
	PanelContainer *status_panel;
	LineEdit *parent_name;
	Button *parent_browse_button;
	Button *parent_search_button;
	OptionButton *language_menu;
	OptionButton *template_menu;
	LineEdit *file_path;
	LineEdit *internal_name;
	Button *path_button;
	EditorFileDialog *file_browse;
	CheckBox *internal;
	VBoxContainer *path_vb;
	AcceptDialog *alert;
	CreateDialog *select_class;
	bool path_valid;
	bool create_new;
	bool is_browsing_parent;
	String initial_bp;
	bool is_new_script_created;
	bool is_path_valid;
	bool has_named_classes;
	bool supports_built_in;
	bool can_inherit_from_file;
	bool is_parent_name_valid;
	bool is_class_name_valid;
	bool is_built_in;
	bool built_in_enabled;
	bool load_enabled;
	int current_language;
	int default_language;
	bool re_check_path;

	Control *path_controls[2];
	Control *name_controls[2];

	enum ScriptOrigin {
		SCRIPT_ORIGIN_PROJECT,
		SCRIPT_ORIGIN_EDITOR,
	};
	struct ScriptTemplateInfo {
		int id;
		ScriptOrigin origin;
		String dir;
		String name;
		String extension;
	};

	String script_template;
	Vector<ScriptTemplateInfo> template_list;
	RBMap<String, Vector<int>> template_overrides; // name : indices

	void _update_script_templates(const String &p_extension);

	String base_type;

	void _path_hbox_sorted();
	bool _can_be_built_in();
	void _path_changed(const String &p_path = String());
	void _path_entered(const String &p_path = String());
	void _lang_changed(int l = 0);
	void _built_in_pressed();
	bool _validate_parent(const String &p_string);
	bool _validate_class(const String &p_string);
	String _validate_path(const String &p_path, bool p_file_must_exist);
	String _get_class_name() const;
	void _class_name_changed(const String &p_name);
	void _parent_name_changed(const String &p_parent);
	void _template_changed(int p_template = 0);
	void _browse_path(bool browse_parent, bool p_save);
	void _file_selected(const String &p_file);
	void _create();
	void _browse_class_in_tree();
	virtual void ok_pressed();
	void _create_new();
	void _load_exist();
	void _msg_script_valid(bool valid, const String &p_msg = String());
	void _msg_path_valid(bool valid, const String &p_msg = String());
	void _update_dialog();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void config(const String &p_base_name, const String &p_base_path, bool p_built_in_enabled = true, bool p_load_enabled = true);
	void set_inheritance_base_type(const String &p_base);
	ScriptCreateDialog();
};

#endif // SCRIPT_CREATE_DIALOG_H
