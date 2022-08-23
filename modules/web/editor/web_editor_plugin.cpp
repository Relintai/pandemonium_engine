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

#include "web_editor_plugin.h"
#include "core/config/engine.h"

#include "../http/web_node.h"
#include "web_editor.h"

#include "editor/editor_node.h"

void WebEditorPlugin::make_visible(bool visible) {
	if (visible) {
		if (is_inside_tree()) {
			_scene_has_webnode = true;
			editor->editor_set_visible_by_name("Web", true);
		}
	}

	window->set_visible(visible);
}

String WebEditorPlugin::get_name() const {
	return "Web";
}

const Ref<Texture> WebEditorPlugin::get_icon() const {
	return _icon;
}

bool WebEditorPlugin::has_main_screen() const {
	return true;
}

void WebEditorPlugin::edit(Object *p_object) {
	WebNode *wn = Object::cast_to<WebNode>(p_object);

	if (wn) {
		window->edit(wn);

		get_editor_interface()->set_main_screen_editor("Web");
	}
}

bool WebEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("WebNode");
}

void WebEditorPlugin::edited_scene_changed() {
	Node *edited_scene = editor->get_edited_scene();

	_scene_has_webnode = scene_has_webnode(edited_scene);
	editor->editor_set_visible_by_name("Web", _scene_has_webnode);
}

bool WebEditorPlugin::scene_has_webnode(Node *p_node) {
	if (!p_node) {
		return false;
	}

	if (Object::cast_to<WebNode>(p_node)) {
		return true;
	}

	for (int i = 0; i < p_node->get_child_count(); ++i) {
		Node *n = p_node->get_child(i);

		if (scene_has_webnode(n)) {
			return true;
		}
	}

	return false;
}

bool WebEditorPlugin::scene_has_webnode_skip(Node *p_node, Node *skip) {
	if (!p_node) {
		return false;
	}

	if (p_node == skip) {
		return false;
	}

	if (Object::cast_to<WebNode>(p_node)) {
		return true;
	}

	for (int i = 0; i < p_node->get_child_count(); ++i) {
		Node *n = p_node->get_child(i);

		if (scene_has_webnode_skip(n, skip)) {
			return true;
		}
	}

	return false;
}

void WebEditorPlugin::on_node_removed(Node *p_node) {
	if (!_scene_has_webnode) {
		return;
	}

	if (Object::cast_to<WebNode>(p_node)) {
		if (!scene_has_webnode_skip(editor->get_edited_scene(), p_node)) {
			editor->editor_set_visible_by_name("Web", false);
			_scene_has_webnode = false;
		}
	}
}

WebEditorPlugin::WebEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	window = memnew(WebEditor);

	editor->get_viewport()->add_child(window);
	_icon = get_editor_interface()->get_base_control()->get_theme_icon("WebNodeEditor", "EditorIcons");

	make_visible(false);

	_scene_has_webnode = false;

	Engine::get_singleton()->add_global("WebNodeEditor", window);
}

WebEditorPlugin::~WebEditorPlugin() {
	Engine::get_singleton()->remove_global("WebNodeEditor");
}

void WebEditorPlugin::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POST_ENTER_TREE: {
			editor->editor_set_visible_by_name("Web", false);
			get_tree()->connect("node_removed", this, "on_node_removed");
		} break;
		case NOTIFICATION_EXIT_TREE: {
			get_tree()->disconnect("node_removed", this, "on_node_removed");
		} break;
	}
}

void WebEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_node_removed", "node"), &WebEditorPlugin::on_node_removed);
}
