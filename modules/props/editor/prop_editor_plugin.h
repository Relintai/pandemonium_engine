#ifndef PROP_EDITOR_PLUGIN_H
#define PROP_EDITOR_PLUGIN_H


#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

class PropEditorPlugin : public EditorPlugin {
	GDCLASS(PropEditorPlugin, EditorPlugin);
	EditorNode *editor;

protected:
	static void _bind_methods();

public:
	virtual String get_name() const { return "PropEditorPlugin"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object) {}
	virtual bool handles(Object *p_object) const { return false; }
	virtual void make_visible(bool p_visible) {}

	void convert_active_scene_to_prop_data();
	void convert_selected_scene_to_prop_data();
	void convert_scene(Node *root, const String &path);

	void find_room_points(Variant param);

	void _convert_active_scene_to_prop_data(Variant param);
	void _convert_selected_scene_to_prop_data(Variant param);
	void _quick_convert_button_pressed();

	PropEditorPlugin(EditorNode *p_node);
	~PropEditorPlugin();
};

#endif
