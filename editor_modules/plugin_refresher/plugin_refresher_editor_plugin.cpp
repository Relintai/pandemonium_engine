/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "plugin_refresher_editor_plugin.h"

#include "editor/editor_node.h"
#include "editor/editor_plugin_settings.h"
#include "editor/editor_settings.h"
#include "editor/project_settings_editor.h"
#include "plugin_refresher.h"
#include "scene/gui/check_box.h"

String PluginRefresherEditorPlugin::get_name() const {
	return "PluginRefresherEditorPlugin";
}

PluginRefresherEditorPlugin::PluginRefresherEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	_enable_check_box = memnew(CheckBox);
	_enable_check_box->set_text(TTR("Show Refresher"));

	_plugin_refresher = memnew(PluginRefresher);
	_plugin_refresher->set_icon(get_editor_interface()->get_base_control()->get_theme_icon("Reload", "EditorIcons"));

	_plugin_refresher->set_enabler_check_box(_enable_check_box);

	add_control_to_container(CONTAINER_TOOLBAR, _plugin_refresher);
	ProjectSettingsEditor::get_singleton()->get_plugin_settings()->add_control_to_top_bar(_enable_check_box);

	bool enabled = EditorSettings::get_singleton()->get_project_metadata("plugin_refresher", "enabled", false);
	_plugin_refresher->set_enabled(enabled);
}

PluginRefresherEditorPlugin::~PluginRefresherEditorPlugin() {
}

void PluginRefresherEditorPlugin::_bind_methods() {
}
