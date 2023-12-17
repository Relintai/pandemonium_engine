

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
