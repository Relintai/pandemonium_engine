#ifndef PLUGIN_REFRESHER_EDITOR_PLUGIN_H
#define PLUGIN_REFRESHER_EDITOR_PLUGIN_H



#include "core/object/reference.h"
#include "editor/editor_plugin.h"

class CheckBox;
class PluginRefresher;

class PluginRefresherEditorPlugin : public EditorPlugin {
	GDCLASS(PluginRefresherEditorPlugin, EditorPlugin);

public:
	String get_name() const;

	PluginRefresherEditorPlugin(EditorNode *p_node);
	~PluginRefresherEditorPlugin();

	EditorNode *editor;

protected:
	static void _bind_methods();

	CheckBox *_enable_check_box;
	PluginRefresher *_plugin_refresher;
};

#endif
