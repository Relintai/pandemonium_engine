#ifndef EXPORT_TEMPLATE_MANAGER_H
#define EXPORT_TEMPLATE_MANAGER_H

/*************************************************************************/
/*  export_template_manager.h                                            */
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

#include "core/error/error_list.h"
#include "core/object/object.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class ExportTemplateVersion;
class HBoxContainer;
class Button;
class FileDialog;
class HTTPRequest;
class Label;
class LineEdit;
class MenuButton;
class OptionButton;
class ProgressBar;
class Tree;
class VBoxContainer;

class ExportTemplateManager : public AcceptDialog {
	GDCLASS(ExportTemplateManager, AcceptDialog);

	bool current_version_exists = false;

	Label *current_value;
	Label *current_missing_label;
	Label *current_installed_label;

	HBoxContainer *current_installed_hb;
	LineEdit *current_installed_path;
	Button *current_open_button;
	Button *current_uninstall_button;

	VBoxContainer *install_options_vb;

	HTTPRequest *download_templates;
	Button *install_file_button;

	enum TemplatesAction {
		OPEN_TEMPLATE_FOLDER,
		UNINSTALL_TEMPLATE,
	};

	Tree *installed_table;

	ConfirmationDialog *uninstall_confirm;
	String uninstall_version;
	FileDialog *install_file_dialog;

	void _update_template_status();

	void _install_file();
	bool _install_file_selected(const String &p_file, bool p_skip_progress = false);

	void _uninstall_template(const String &p_version);
	void _uninstall_template_confirmed();

	void _installed_table_button_cbk(Object *p_item, int p_column, int p_id);

	void _open_template_folder(const String &p_version);

	virtual void ok_pressed();
	virtual void cancel_pressed();
	void _hide_dialog();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	bool can_install_android_template();
	Error install_android_template();

	Error install_android_template_from_file(const String &p_file);

	void popup_manager();

	ExportTemplateManager();
};

#endif // EXPORT_TEMPLATE_MANAGER_H
