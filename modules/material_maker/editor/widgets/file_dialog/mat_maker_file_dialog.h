#ifndef MAT_MAKER_FILE_DIALOG_H
#define MAT_MAKER_FILE_DIALOG_H

/*************************************************************************/
/*  mat_maker_file_dialog.h                                              */
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

#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include "scene/gui/file_dialog.h"

class LeftPanel;
class OptionButton;

class MatMakerFileDialog : public FileDialog {
	GDCLASS(MatMakerFileDialog, FileDialog);

public:
	String get_full_current_dir();

	void _on_FileDialog_file_selected(const String &path);
	void _on_FileDialog_files_selected(const PoolStringArray &paths);
	void _on_FileDialog_dir_selected(const String &dir);
	void _on_FileDialog_popup_hide();

	void select_files();
	void add_favorite();

	MatMakerFileDialog();
	~MatMakerFileDialog();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	LeftPanel *_left_panel;
	OptionButton *_volume_option;
};

#endif
