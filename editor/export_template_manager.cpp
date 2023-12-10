/*************************************************************************/
/*  export_template_manager.cpp                                          */
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

#include "export_template_manager.h"

#include "core/variant/array.h"
#include "core/object/class_db.h"
#include "core/variant/dictionary.h"
#include "core/error/error_macros.h"
#include "core/io/http_client.h"
#include "core/io/json.h"
#include "core/io/zip_io.h"
#include "core/math/vector2.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/memory.h"
#include "core/os/os.h"
#include "core/containers/pool_vector.h"
#include "core/containers/rb_set.h"
#include "core/containers/vector.h"
#include "core/version.h"
#include "core/version_generated.gen.h"
#include "editor/editor_settings.h"
#include "editor_node.h"
#include "editor_scale.h"
#include "progress_dialog.h"
#include "scene/main/canvas_item.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/main/control.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/separator.h"
#include "scene/gui/tree.h"
#include "scene/main/http_request.h"
#include "scene/main/node.h"
#include "thirdparty/minizip/ioapi.h"
#include "thirdparty/minizip/unzip.h"

void ExportTemplateManager::_update_template_status() {
	// Fetch installed templates from the file system.
	DirAccess *da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	const String &templates_dir = EditorSettings::get_singleton()->get_templates_dir();

	Error err = da->change_dir(templates_dir);
	ERR_FAIL_COND_MSG(err != OK, "Could not access templates directory at '" + templates_dir + "'.");

	RBSet<String> templates;
	da->list_dir_begin();
	if (err == OK) {
		String c = da->get_next();
		while (c != String()) {
			if (da->current_is_dir() && !c.begins_with(".")) {
				templates.insert(c);
			}
			c = da->get_next();
		}
	}
	da->list_dir_end();
	memdelete(da);

	// Update the state of the current version.
	String current_version = VERSION_FULL_CONFIG;
	current_value->set_text(current_version);

	if (templates.has(current_version)) {
		current_missing_label->hide();
		current_installed_label->show();

		current_installed_hb->show();
		current_version_exists = true;
	} else {
		current_installed_label->hide();
		current_missing_label->show();

		current_installed_hb->hide();
		current_version_exists = false;
	}

	install_options_vb->show();

	if (templates.has(current_version)) {
		current_installed_path->set_text(templates_dir.plus_file(current_version));
	}

	// Update the list of other installed versions.
	installed_table->clear();
	TreeItem *installed_root = installed_table->create_item();

	for (RBSet<String>::Element *E = templates.back(); E; E = E->prev()) {
		String version_string = E->get();
		if (version_string == current_version) {
			continue;
		}

		TreeItem *ti = installed_table->create_item(installed_root);
		ti->set_text(0, version_string);

		ti->add_button(0, get_theme_icon("Folder", "EditorIcons"), OPEN_TEMPLATE_FOLDER, false, TTR("Open the folder containing these templates."));
		ti->add_button(0, get_theme_icon("Remove", "EditorIcons"), UNINSTALL_TEMPLATE, false, TTR("Uninstall these templates."));
	}

	minimum_size_changed();
	update();
}

void ExportTemplateManager::_install_file() {
	install_file_dialog->popup_centered_ratio();
}

bool ExportTemplateManager::_install_file_selected(const String &p_file, bool p_skip_progress) {
	// unzClose() will take care of closing the file stored in the unzFile,
	// so we don't need to `memdelete(fa)` in this method.
	FileAccess *fa = nullptr;
	zlib_filefunc_def io = zipio_create_io_from_file(&fa);

	unzFile pkg = unzOpen2(p_file.utf8().get_data(), &io);
	if (!pkg) {
		EditorNode::get_singleton()->show_warning(TTR("Can't open the export templates file."));
		return false;
	}
	int ret = unzGoToFirstFile(pkg);

	// Count them and find version.
	int fc = 0;
	String version;
	String contents_dir;

	while (ret == UNZ_OK) {
		unz_file_info info;
		char fname[16384];
		ret = unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);

		String file = String::utf8(fname);
		if (file.ends_with("version.txt")) {
			Vector<uint8_t> data;
			data.resize(info.uncompressed_size);

			// Read.
			unzOpenCurrentFile(pkg);
			ret = unzReadCurrentFile(pkg, data.ptrw(), data.size());
			unzCloseCurrentFile(pkg);

			String data_str;
			data_str.parse_utf8((const char *)data.ptr(), data.size());
			data_str = data_str.strip_edges();

			// Version number should be of the form major.minor[.patch].status[.module_config]
			// so it can in theory have 3 or more slices.
			if (data_str.get_slice_count(".") < 3) {
				EditorNode::get_singleton()->show_warning(vformat(TTR("Invalid version.txt format inside the export templates file: %s."), data_str));
				unzClose(pkg);
				return false;
			}

			version = data_str;
			contents_dir = file.get_base_dir().trim_suffix("/").trim_suffix("\\");
		}

		if (file.get_file().size() != 0) {
			fc++;
		}

		ret = unzGoToNextFile(pkg);
	}

	if (version == String()) {
		EditorNode::get_singleton()->show_warning(TTR("No version.txt found inside the export templates file."));
		unzClose(pkg);
		return false;
	}

	DirAccessRef d = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	String template_path = EditorSettings::get_singleton()->get_templates_dir().plus_file(version);
	Error err = d->make_dir_recursive(template_path);
	if (err != OK) {
		EditorNode::get_singleton()->show_warning(TTR("Error creating path for extracting templates:") + "\n" + template_path);
		unzClose(pkg);
		return false;
	}

	EditorProgress *p = nullptr;
	if (!p_skip_progress) {
		p = memnew(EditorProgress("ltask", TTR("Extracting Export Templates"), fc));
	}

	fc = 0;
	ret = unzGoToFirstFile(pkg);
	while (ret == UNZ_OK) {
		// Get filename.
		unz_file_info info;
		char fname[16384];
		unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);

		String file_path(String::utf8(fname).simplify_path());

		String file = file_path.get_file();

		if (file.size() == 0) {
			ret = unzGoToNextFile(pkg);
			continue;
		}

		Vector<uint8_t> data;
		data.resize(info.uncompressed_size);

		// Read
		unzOpenCurrentFile(pkg);
		unzReadCurrentFile(pkg, data.ptrw(), data.size());
		unzCloseCurrentFile(pkg);

		String base_dir = file_path.get_base_dir().trim_suffix("/");

		if (base_dir != contents_dir && base_dir.begins_with(contents_dir)) {
			base_dir = base_dir.substr(contents_dir.length(), file_path.length()).trim_prefix("/");
			file = base_dir.plus_file(file);

			DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
			ERR_CONTINUE(!da);

			String output_dir = template_path.plus_file(base_dir);

			if (!DirAccess::exists(output_dir)) {
				Error mkdir_err = da->make_dir_recursive(output_dir);
				ERR_CONTINUE(mkdir_err != OK);
			}
		}

		if (p) {
			p->step(TTR("Importing:") + " " + file, fc);
		}

		String to_write = template_path.plus_file(file);
		FileAccessRef f = FileAccess::open(to_write, FileAccess::WRITE);

		if (!f) {
			ret = unzGoToNextFile(pkg);
			fc++;
			ERR_CONTINUE_MSG(true, "Can't open file from path '" + String(to_write) + "'.");
		}

		f->store_buffer(data.ptr(), data.size());

#ifndef WINDOWS_ENABLED
		FileAccess::set_unix_permissions(to_write, (info.external_fa >> 16) & 0x01FF);
#endif

		ret = unzGoToNextFile(pkg);
		fc++;
	}

	if (p) {
		memdelete(p);
	}
	unzClose(pkg);

	_update_template_status();
	return true;
}

void ExportTemplateManager::_uninstall_template(const String &p_version) {
	uninstall_confirm->set_text(vformat(TTR("Remove templates for the version '%s'?"), p_version));
	uninstall_confirm->popup_centered();
	uninstall_version = p_version;
}

void ExportTemplateManager::_uninstall_template_confirmed() {
	DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	const String &templates_dir = EditorSettings::get_singleton()->get_templates_dir();

	Error err = da->change_dir(templates_dir);
	ERR_FAIL_COND_MSG(err != OK, "Could not access templates directory at '" + templates_dir + "'.");
	err = da->change_dir(uninstall_version);
	ERR_FAIL_COND_MSG(err != OK, "Could not access templates directory at '" + templates_dir.plus_file(uninstall_version) + "'.");

	err = da->erase_contents_recursive();
	ERR_FAIL_COND_MSG(err != OK, "Could not remove all templates in '" + templates_dir.plus_file(uninstall_version) + "'.");

	da->change_dir("..");
	err = da->remove(uninstall_version);
	ERR_FAIL_COND_MSG(err != OK, "Could not remove templates directory at '" + templates_dir.plus_file(uninstall_version) + "'.");

	_update_template_status();
}

void ExportTemplateManager::_installed_table_button_cbk(Object *p_item, int p_column, int p_id) {
	TreeItem *ti = Object::cast_to<TreeItem>(p_item);
	if (!ti) {
		return;
	}

	switch (p_id) {
		case OPEN_TEMPLATE_FOLDER: {
			String version_string = ti->get_text(0);
			_open_template_folder(version_string);
		} break;

		case UNINSTALL_TEMPLATE: {
			String version_string = ti->get_text(0);
			_uninstall_template(version_string);
		} break;
	}
}

void ExportTemplateManager::_open_template_folder(const String &p_version) {
	const String &templates_dir = EditorSettings::get_singleton()->get_templates_dir();
	OS::get_singleton()->shell_open("file://" + templates_dir.plus_file(p_version));
}

void ExportTemplateManager::popup_manager() {
	_update_template_status();
	popup_centered(Size2(720, 280) * EDSCALE);
}

void ExportTemplateManager::ok_pressed() {
	hide();
}

void ExportTemplateManager::cancel_pressed() {
	// This won't stop the window from closing, but will show the alert if the download is active.
	ok_pressed();
}

void ExportTemplateManager::_hide_dialog() {
	hide();
}

bool ExportTemplateManager::can_install_android_template() {
	const String templates_dir = EditorSettings::get_singleton()->get_templates_dir().plus_file(VERSION_FULL_CONFIG);
	return FileAccess::exists(templates_dir.plus_file("android_source.zip"));
}

Error ExportTemplateManager::install_android_template() {
	const String &templates_path = EditorSettings::get_singleton()->get_templates_dir().plus_file(VERSION_FULL_CONFIG);
	const String &source_zip = templates_path.plus_file("android_source.zip");
	ERR_FAIL_COND_V(!FileAccess::exists(source_zip), ERR_CANT_OPEN);
	return install_android_template_from_file(source_zip);
}
Error ExportTemplateManager::install_android_template_from_file(const String &p_file) {
	// To support custom Android builds, we install the Java source code and buildsystem
	// from android_source.zip to the project's res://android folder.

	DirAccessRef da = DirAccess::open("res://");
	ERR_FAIL_COND_V(!da, ERR_CANT_CREATE);

	// Make res://android dir (if it does not exist).
	da->make_dir("android");
	{
		// Add version, to ensure building won't work if template and Godot version don't match.
		FileAccessRef f = FileAccess::open("res://android/.build_version", FileAccess::WRITE);
		ERR_FAIL_COND_V(!f, ERR_CANT_CREATE);
		f->store_line(VERSION_FULL_CONFIG);
		f->close();
	}

	// Create the android plugins directory.
	Error err = da->make_dir_recursive("android/plugins");
	ERR_FAIL_COND_V(err != OK, err);

	err = da->make_dir_recursive("android/build");
	ERR_FAIL_COND_V(err != OK, err);
	{
		// Add an empty .gdignore file to avoid scan.
		FileAccessRef f = FileAccess::open("res://android/build/.gdignore", FileAccess::WRITE);
		ERR_FAIL_COND_V(!f, ERR_CANT_CREATE);
		f->store_line("");
		f->close();
	}

	// Uncompress source template.

	FileAccess *src_f = nullptr;
	zlib_filefunc_def io = zipio_create_io_from_file(&src_f);

	unzFile pkg = unzOpen2(p_file.utf8().get_data(), &io);
	ERR_FAIL_COND_V_MSG(!pkg, ERR_CANT_OPEN, "Android sources not in ZIP format.");

	int ret = unzGoToFirstFile(pkg);
	int total_files = 0;
	// Count files to unzip.
	while (ret == UNZ_OK) {
		total_files++;
		ret = unzGoToNextFile(pkg);
	}
	ret = unzGoToFirstFile(pkg);

	ProgressDialog::get_singleton()->add_task("uncompress_src", TTR("Uncompressing Android Build Sources"), total_files);

	RBSet<String> dirs_tested;
	int idx = 0;
	while (ret == UNZ_OK) {
		// Get file path.
		unz_file_info info;
		char fpath[16384];
		ret = unzGetCurrentFileInfo(pkg, &info, fpath, 16384, nullptr, 0, nullptr, 0);

		String path = String::utf8(fpath);
		String base_dir = path.get_base_dir();

		if (!path.ends_with("/")) {
			Vector<uint8_t> data;
			data.resize(info.uncompressed_size);

			// Read.
			unzOpenCurrentFile(pkg);
			unzReadCurrentFile(pkg, data.ptrw(), data.size());
			unzCloseCurrentFile(pkg);

			if (!dirs_tested.has(base_dir)) {
				da->make_dir_recursive(String("android/build").plus_file(base_dir));
				dirs_tested.insert(base_dir);
			}

			String to_write = String("res://android/build").plus_file(path);
			FileAccess *f = FileAccess::open(to_write, FileAccess::WRITE);
			if (f) {
				f->store_buffer(data.ptr(), data.size());
				memdelete(f);
#ifndef WINDOWS_ENABLED
				FileAccess::set_unix_permissions(to_write, (info.external_fa >> 16) & 0x01FF);
#endif
			} else {
				ERR_PRINT("Can't uncompress file: " + to_write);
			}
		}

		ProgressDialog::get_singleton()->task_step("uncompress_src", path, idx);

		idx++;
		ret = unzGoToNextFile(pkg);
	}

	ProgressDialog::get_singleton()->end_task("uncompress_src");
	unzClose(pkg);

	return OK;
}

void ExportTemplateManager::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			current_value->add_theme_font_override("font", get_theme_font("bold", "EditorFonts"));
			current_missing_label->add_theme_color_override("font_color", get_theme_color("error_color", "Editor"));
			current_installed_label->add_theme_color_override("font_color", get_theme_color("disabled_font_color", "Editor"));
		} break;
	}
}

void ExportTemplateManager::_bind_methods() {
	ClassDB::bind_method("_hide_dialog", &ExportTemplateManager::_hide_dialog);
	ClassDB::bind_method("_open_template_folder", &ExportTemplateManager::_open_template_folder);

	ClassDB::bind_method("_uninstall_template", &ExportTemplateManager::_uninstall_template);
	ClassDB::bind_method("_uninstall_template_confirmed", &ExportTemplateManager::_uninstall_template_confirmed);

	ClassDB::bind_method("_install_file", &ExportTemplateManager::_install_file);
	ClassDB::bind_method("_installed_table_button_cbk", &ExportTemplateManager::_installed_table_button_cbk);
	ClassDB::bind_method("_install_file_selected", &ExportTemplateManager::_install_file_selected);
}

ExportTemplateManager::ExportTemplateManager() {
	set_title(TTR("Export Template Manager"));
	set_hide_on_ok(false);
	get_ok()->set_text(TTR("Close"));

	VBoxContainer *main_vb = memnew(VBoxContainer);
	add_child(main_vb);

	// Current version controls.
	HBoxContainer *current_hb = memnew(HBoxContainer);
	main_vb->add_child(current_hb);

	Label *current_label = memnew(Label);
	current_label->set_text(TTR("Current Version:"));
	current_hb->add_child(current_label);

	current_value = memnew(Label);
	current_hb->add_child(current_value);

	// Current version statuses.
	// Status: Current version is missing.
	current_missing_label = memnew(Label);
	current_missing_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	current_missing_label->set_align(Label::ALIGN_RIGHT);
	current_missing_label->set_text(TTR("Export templates are missing. Download them or install from a file."));
	current_hb->add_child(current_missing_label);

	// Status: Current version is installed.
	current_installed_label = memnew(Label);
	current_installed_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	current_installed_label->set_align(Label::ALIGN_RIGHT);
	current_installed_label->set_text(TTR("Export templates are installed and ready to be used."));
	current_hb->add_child(current_installed_label);
	current_installed_label->hide();

	// Currently installed template.
	current_installed_hb = memnew(HBoxContainer);
	current_hb->add_child(current_installed_hb);

	current_installed_path = memnew(LineEdit);
	current_installed_path->set_editable(false);
	current_installed_path->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	current_installed_hb->add_child(current_installed_path);

	current_open_button = memnew(Button);
	current_open_button->set_text(TTR("Open Folder"));
	current_open_button->set_tooltip(TTR("Open the folder containing installed templates for the current version."));
	current_installed_hb->add_child(current_open_button);
	current_open_button->connect("pressed", this, "_open_template_folder", varray(VERSION_FULL_CONFIG));

	current_uninstall_button = memnew(Button);
	current_uninstall_button->set_text(TTR("Uninstall"));
	current_uninstall_button->set_tooltip(TTR("Uninstall templates for the current version."));
	current_installed_hb->add_child(current_uninstall_button);
	current_uninstall_button->connect("pressed", this, "_uninstall_template", varray(VERSION_FULL_CONFIG));

	// Download and install section.
	HBoxContainer *install_templates_hb = memnew(HBoxContainer);
	current_hb->add_child(install_templates_hb);

	// Download and install buttons are available.
	install_options_vb = memnew(VBoxContainer);
	install_options_vb->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	install_templates_hb->add_child(install_options_vb);

	HBoxContainer *install_file_hb = memnew(HBoxContainer);
	install_file_hb->set_alignment(BoxContainer::ALIGN_END);
	install_options_vb->add_child(install_file_hb);

	install_file_button = memnew(Button);
	install_file_button->set_text(TTR("Install from File"));
	install_file_button->set_tooltip(TTR("Install templates from a local file."));
	install_file_hb->add_child(install_file_button);
	install_file_button->connect("pressed", this, "_install_file");

	main_vb->add_child(memnew(HSeparator));

	// Other installed templates table.
	HBoxContainer *installed_versions_hb = memnew(HBoxContainer);
	main_vb->add_child(installed_versions_hb);
	Label *installed_label = memnew(Label);
	installed_label->set_text(TTR("Other Installed Versions:"));
	installed_versions_hb->add_child(installed_label);

	installed_table = memnew(Tree);
	installed_table->set_hide_root(true);
	installed_table->set_custom_minimum_size(Size2(0, 100) * EDSCALE);
	installed_table->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	main_vb->add_child(installed_table);
	installed_table->connect("button_pressed", this, "_installed_table_button_cbk");

	// Dialogs.
	uninstall_confirm = memnew(ConfirmationDialog);
	uninstall_confirm->set_title(TTR("Uninstall Template"));
	add_child(uninstall_confirm);
	uninstall_confirm->connect("confirmed", this, "_uninstall_template_confirmed");

	install_file_dialog = memnew(FileDialog);
	install_file_dialog->set_title(TTR("Select Template File"));
	install_file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
	install_file_dialog->set_mode(FileDialog::MODE_OPEN_FILE);
	install_file_dialog->add_filter("*.tpz ; " + TTR("Pandemonium Export Templates"));
	install_file_dialog->connect("file_selected", this, "_install_file_selected", varray(false));
	add_child(install_file_dialog);
}
