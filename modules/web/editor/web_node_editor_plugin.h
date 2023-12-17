#ifndef WEB_NODE_EDITOR_PLUGIN_H
#define WEB_NODE_EDITOR_PLUGIN_H



#include "core/object/reference.h"

#include "editor/editor_plugin.h"

class WebNodeEditor;
class Texture;

class WebNodeEditorPlugin : public EditorPlugin {
	GDCLASS(WebNodeEditorPlugin, EditorPlugin);

public:
	void make_visible(bool visible);
	const Ref<Texture> get_icon() const;
	bool has_main_screen() const;
	String get_name() const;
	void edit(Object *p_object);
	bool handles(Object *p_object) const;
	void edited_scene_changed();

	bool scene_has_webnode(Node *p_node);
	bool scene_has_webnode_skip(Node *p_node, Node *skip);

	WebNodeEditorPlugin(EditorNode *p_node);
	~WebNodeEditorPlugin();

	EditorNode *editor;

	WebNodeEditor *window;

protected:
	void on_node_removed(Node *p_child);

	void _notification(int p_what);

	static void _bind_methods();

	Ref<Texture> _icon;
	bool _scene_has_webnode;
};

#endif
