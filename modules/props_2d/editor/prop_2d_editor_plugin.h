#ifndef PROP_2D_EDITOR_PLUGIN_H
#define PROP_2D_EDITOR_PLUGIN_H


#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

class Prop2DEditorPlugin : public EditorPlugin {
	GDCLASS(Prop2DEditorPlugin, EditorPlugin);
	EditorNode *editor;

protected:
	static void _bind_methods();

public:
	virtual String get_name() const { return "Prop2DEditorPlugin"; }
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

	Prop2DEditorPlugin(EditorNode *p_node);
	~Prop2DEditorPlugin();
};

#endif
