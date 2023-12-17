/*************************************************************************/
/*  plugin_refresher.cpp                                                 */
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

#include "plugin_refresher.h"
#include "core/input/input_event.h"
#include "core/io/config_file.h"
#include "core/log/logger.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "editor/editor_plugin.h"
#include "editor/project_settings_editor.h"

#include "editor/editor_settings.h"
#include "scene/gui/box_container.h"
#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"

void PluginRefresher::set_enabled(const bool p_enabled) {
	if (_enabler_check_box) {
		_enabler_check_box->set_pressed(p_enabled);
	}

	_set_enabled(p_enabled);
	_refresh();
}

void PluginRefresher::set_enabler_check_box(CheckBox *cb) {
	_enabler_check_box = cb;

	if (_enabler_check_box) {
		_enabler_check_box->connect("toggled", this, "_set_enabled");
	}
}

void PluginRefresher::_popup() {
	_refresh();
	_selector_dialog->popup_centered();
}
void PluginRefresher::_refresh() {
	String selected_plugin;

	if (_currently_selected_addon != -1) {
		selected_plugin = _plugins[_currently_selected_addon].folder;
	} else {
		selected_plugin = EditorSettings::get_singleton()->get_project_metadata("plugin_refresher", "plugin", "");
	}

	_currently_selected_addon = -1;

	_option_button->clear();
	_plugins.clear();

	DirAccess *dir = DirAccess::open("res://addons/");

	if (!dir) {
		return;
	}

	Vector<String> addon_dirs;

	dir->list_dir_begin();

	String file = dir->get_next();
	while (!file.empty()) {
		if (file == "." || file == "..") {
			file = dir->get_next();
			continue;
		}

		if (dir->current_is_dir()) {
			addon_dirs.push_back(file);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);

	addon_dirs.sort();

	for (int i = 0; i < addon_dirs.size(); ++i) {
		String addon_dir = addon_dirs[i];

		String cp = "res://addons/";
		cp += addon_dir;
		cp += "/plugin.cfg";

		Ref<ConfigFile> cf;
		cf.instance();
		if (cf->load(cp) != OK) {
			continue;
		}

		if (!cf->has_section("plugin")) {
			continue;
		}

		String addon_name = cf->get_value("plugin", "name", addon_dir);

		PluginEntry p;
		p.name = addon_name;
		p.folder = addon_dir;

		_plugins.push_back(p);
		_option_button->add_item(addon_name, i);

		if (addon_dir == selected_plugin) {
			_currently_selected_addon = i;
		}
	}

	if (_currently_selected_addon != -1) {
		_option_button->select(_currently_selected_addon);
		_on_option_selected(_currently_selected_addon);
	}
}

void PluginRefresher::_set_enabled(const bool p_enabled) {
	set_visible(p_enabled);

	bool set_enabled = EditorSettings::get_singleton()->get_project_metadata("plugin_refresher", "enabled", false);

	if (set_enabled != p_enabled) {
		EditorSettings::get_singleton()->set_project_metadata("plugin_refresher", "enabled", p_enabled);
	}
}

void PluginRefresher::_on_plugins_button_pressed() {
	_selector_dialog->hide();
	ProjectSettingsEditor::get_singleton()->set_plugins_page();
	ProjectSettingsEditor::get_singleton()->popup_project_settings();
}
void PluginRefresher::_on_disable_button_pressed() {
	_selector_dialog->hide();
	set_enabled(false);
}

void PluginRefresher::_on_popup_confirmed() {
	if (_currently_selected_addon == -1 && _plugins.size() > 0) {
		_option_button->select(0);
		_on_option_selected(0);
	}
}

void PluginRefresher::_on_option_selected(const int id) {
	_currently_selected_addon = id;

	String selected_plugin = _plugins[_currently_selected_addon].folder;
	EditorSettings::get_singleton()->set_project_metadata("plugin_refresher", "plugin", selected_plugin);
}

void PluginRefresher::_refresh_selected() {
	if (_currently_selected_addon == -1) {
		_popup();
		return;
	}

	String folder = _plugins[_currently_selected_addon].folder;

	String cp = "res://addons/";
	cp += folder;
	cp += "/plugin.cfg";

	if (!FileAccess::exists(cp)) {
		ERR_PRINT("Selected addon couldn't be found.");
		_popup();
		return;
	}

	String name = _plugins[_currently_selected_addon].name;

	PLOG_MSG("Refreshing addon: " + name);

	if (EditorInterface::get_singleton()->is_plugin_enabled(folder)) {
		EditorInterface::get_singleton()->set_plugin_enabled(folder, false);
	} else {
		PLOG_WARN("Warning! Addon was disabled. Enabling!");
	}

	EditorInterface::get_singleton()->set_plugin_enabled(folder, true);
}

void PluginRefresher::pressed() {
	if (_button == BUTTON_LEFT) {
		_refresh_selected();
	} else if (_button == BUTTON_RIGHT) {
		_popup();
	}
}
void PluginRefresher::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> mouse_button = p_event;

	if (mouse_button.is_valid()) {
		_button = mouse_button->get_button_index();
	}

	BaseButton::_gui_input(p_event);
}

PluginRefresher::PluginRefresher() {
	_currently_selected_addon = -1;
	_button = 0;
	set_button_mask(BUTTON_MASK_LEFT | BUTTON_MASK_RIGHT);
	set_enabled_focus_mode(FOCUS_NONE);
	set_tooltip("Left click: Refresh selected plugin.\nRight click: Settings.");

	_selector_dialog = memnew(AcceptDialog);
	add_child(_selector_dialog);
	_selector_dialog->set_title("Plugin refresher");
	_selector_dialog->set_custom_minimum_size(Size2(300, 0));
	_selector_dialog->connect("confirmed", this, "_on_popup_confirmed");

	VBoxContainer *main_container = memnew(VBoxContainer);
	_selector_dialog->add_child(main_container);

	HBoxContainer *top_container = memnew(HBoxContainer);
	main_container->add_child(top_container);
	top_container->set_alignment(BoxContainer::ALIGN_END);

	Button *plugins_button = memnew(Button);
	top_container->add_child(plugins_button);
	plugins_button->set_text("Plugins");
	plugins_button->set_tooltip("Open the plugins tab.");
	plugins_button->connect("pressed", this, "_on_plugins_button_pressed");

	Button *disable_button = memnew(Button);
	top_container->add_child(disable_button);
	disable_button->set_text("Disable");
	disable_button->set_tooltip("Disables the refresher widget.");
	disable_button->connect("pressed", this, "_on_disable_button_pressed");

	Label *ml = memnew(Label);
	main_container->add_child(ml);
	ml->set_text("Select an addon:");

	_option_button = memnew(OptionButton);
	main_container->add_child(_option_button);
	_option_button->connect("item_selected", this, "_on_option_selected");
}

PluginRefresher::~PluginRefresher() {
}

void PluginRefresher::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_set_enabled", "enabled"), &PluginRefresher::_set_enabled);
	ClassDB::bind_method(D_METHOD("_on_plugins_button_pressed"), &PluginRefresher::_on_plugins_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_disable_button_pressed"), &PluginRefresher::_on_disable_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_popup_confirmed"), &PluginRefresher::_on_popup_confirmed);
	ClassDB::bind_method(D_METHOD("_on_option_selected", "id"), &PluginRefresher::_on_option_selected);
}
