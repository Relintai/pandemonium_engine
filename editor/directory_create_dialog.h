#ifndef DIRECTORY_CREATE_DIALOG_H
#define DIRECTORY_CREATE_DIALOG_H

/*************************************************************************/
/*  directory_create_dialog.h                                            */
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

class Label;
class LineEdit;
class PanelContainer;

class DirectoryCreateDialog : public ConfirmationDialog {
	GDCLASS(DirectoryCreateDialog, ConfirmationDialog);

	String base_dir;

	Label *label = nullptr;
	LineEdit *dir_path = nullptr;

	PanelContainer *status_panel = nullptr;
	Label *status_label = nullptr;

	String _validate_path(const String &p_path) const;

	void _on_dir_path_changed(const String &p_text);

protected:
	static void _bind_methods();
	void _notification(int p_what);

	virtual void ok_pressed();
	virtual void _post_popup();

public:
	void config(const String &p_base_dir);

	DirectoryCreateDialog();
};

#endif // DIRECTORY_CREATE_DIALOG_H