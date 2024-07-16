#ifndef RENAME_DIALOG_H
#define RENAME_DIALOG_H

/*************************************************************************/
/*  rename_dialog.h                                                      */
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

#include "modules/modules_enabled.gen.h" // For regex.

#ifdef MODULE_REGEX_ENABLED

#include "core/containers/list.h"
#include "core/containers/pair.h"
#include "core/error/error_macros.h"
#include "core/object/object.h"
#include "core/string/node_path.h"
#include "core/string/ustring.h"

#include "scene/gui/dialogs.h"

class CheckButton;
class Array;
class Button;
class CheckBox;
class Label;
class LineEdit;
class Node;
class OptionButton;
class SceneTreeEditor;
class SpinBox;
class TabContainer;
class UndoRedo;

/**
@author Blazej Floch
*/

class RenameDialog : public ConfirmationDialog {
	GDCLASS(RenameDialog, ConfirmationDialog);

	virtual void ok_pressed() { rename(); };
	void _cancel_pressed() {};
	void _features_toggled(bool pressed);
	void _insert_text(String text);
	void _update_substitute();
	bool _is_main_field(LineEdit *line_edit);

	void _iterate_scene(const Node *node, const Array &selection, int *count);
	String _apply_rename(const Node *node, int count);
	String _substitute(const String &subject, const Node *node, int count);
	String _regex(const String &pattern, const String &subject, const String &replacement);
	String _postprocess(const String &subject);
	void _update_preview(String new_text = "");
	void _update_preview_int(int new_value = 0);
	static void _error_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, ErrorHandlerType p_type);

	SceneTreeEditor *scene_tree_editor;
	UndoRedo *undo_redo;
	int global_count;

	LineEdit *lne_search;
	LineEdit *lne_replace;
	LineEdit *lne_prefix;
	LineEdit *lne_suffix;

	TabContainer *tabc_features;

	CheckBox *cbut_substitute;
	CheckButton *cbut_regex;
	CheckBox *cbut_process;
	CheckBox *chk_per_level_counter;

	Button *but_insert_name;
	Button *but_insert_parent;
	Button *but_insert_type;
	Button *but_insert_scene;
	Button *but_insert_root;
	Button *but_insert_count;

	SpinBox *spn_count_start;
	SpinBox *spn_count_step;
	SpinBox *spn_count_padding;

	OptionButton *opt_style;
	OptionButton *opt_case;

	Label *lbl_preview_title;
	Label *lbl_preview;

	List<Pair<NodePath, String>> to_rename;
	Node *preview_node;
	bool lock_preview_update;
	ErrorHandlerList eh;
	bool has_errors;

protected:
	void _notification(int p_what) {};
	static void _bind_methods();
	virtual void _post_popup();

public:
	void reset();
	void rename();

	RenameDialog(SceneTreeEditor *p_scene_tree_editor, UndoRedo *p_undo_redo = nullptr);
	~RenameDialog() {};
};

#endif // MODULE_REGEX_ENABLED

#endif // RENAME_DIALOG_H
