#ifndef WEB_NODE_EDITOR_PLUGIN_H
#define WEB_NODE_EDITOR_PLUGIN_H

/*
Copyright (c) 2019-2023 Péter Magyar

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
