#ifndef PAINT_EDITOR__PLUGIN_H
#define PAINT_EDITOR__PLUGIN_H



#include "core/object/reference.h"
#include "editor/editor_plugin.h"

class PaintWindow;
class Texture;
class PaintNode;
class PaintInspectorPlugin;

class PaintEditorPlugin : public EditorPlugin {
	GDCLASS(PaintEditorPlugin, EditorPlugin);

public:
	void make_visible(const bool visible);
	String get_name() const;
	void edit(Object *p_object);
	bool handles(Object *p_object) const;
	void edited_scene_changed();

	bool forward_canvas_gui_input(const Ref<InputEvent> &p_event);
	void forward_canvas_draw_over_viewport(Control *p_overlay);
	void forward_canvas_force_draw_over_viewport(Control *p_overlay);

	PaintEditorPlugin(EditorNode *p_node);
	~PaintEditorPlugin();

	EditorNode *editor;

protected:
	void on_node_removed(Node *node);

	void _notification(int p_what);

	static void _bind_methods();

	PaintNode *_active_node;
	PaintInspectorPlugin *_inspector_plugin;
};

#endif
