#ifndef EDITOR_LOCALE_DIALOG_H
#define EDITOR_LOCALE_DIALOG_H

/*************************************************************************/
/*  editor_locale_dialog.h                                               */
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

#include "core/string/translation.h"
#include "scene/gui/dialogs.h"

class Button;
class HBoxContainer;
class VBoxContainer;
class LineEdit;
class Tree;
class OptionButton;
class UndoRedo;

class EditorLocaleDialog : public ConfirmationDialog {
	GDCLASS(EditorLocaleDialog, ConfirmationDialog);

	enum LocaleFilter {
		SHOW_ALL_LOCALES,
		SHOW_ONLY_SELECTED_LOCALES,
	};

	HBoxContainer *hb_locale = nullptr;
	VBoxContainer *vb_script_list = nullptr;
	OptionButton *filter_mode = nullptr;
	Button *edit_filters = nullptr;
	Button *advanced = nullptr;
	LineEdit *lang_code = nullptr;
	LineEdit *script_code = nullptr;
	LineEdit *country_code = nullptr;
	LineEdit *variant_code = nullptr;
	Tree *lang_list = nullptr;
	Tree *script_list = nullptr;
	Tree *cnt_list = nullptr;

	UndoRedo *undo_redo = nullptr;

	bool locale_set = false;
	bool updating_lists = false;

protected:
	static void _bind_methods();
	virtual void _post_popup();
	virtual void ok_pressed();

	void _item_selected();
	void _filter_lang_option_changed();
	void _filter_script_option_changed();
	void _filter_cnt_option_changed();
	void _filter_mode_changed(int p_mode);
	void _edit_filters(bool p_checked);
	void _toggle_advanced(bool p_checked);

	void _update_tree();

public:
	EditorLocaleDialog();

	void set_locale(const String &p_locale);
	void popup_locale_dialog();
};

#endif // EDITOR_LOCALE_DIALOG_H
