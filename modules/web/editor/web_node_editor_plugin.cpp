

#include "web_node_editor_plugin.h"
#include "core/config/engine.h"

#include "../http/web_node.h"
#include "web_node_editor.h"

#include "editor/editor_node.h"

void WebNodeEditorPlugin::make_visible(bool visible) {
	if (visible) {
		if (is_inside_tree()) {
			_scene_has_webnode = true;
			editor->editor_set_visible_by_name("Web", true);
		}
	}

	window->set_visible(visible);
}

String WebNodeEditorPlugin::get_name() const {
	return "Web";
}

const Ref<Texture> WebNodeEditorPlugin::get_icon() const {
	return _icon;
}

bool WebNodeEditorPlugin::has_main_screen() const {
	return true;
}

void WebNodeEditorPlugin::edit(Object *p_object) {
	WebNode *wn = Object::cast_to<WebNode>(p_object);

	if (wn) {
		window->edit(wn);

		get_editor_interface()->set_main_screen_editor("Web");
	}
}

bool WebNodeEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("WebNode");
}

void WebNodeEditorPlugin::edited_scene_changed() {
	Node *edited_scene = editor->get_edited_scene();

	_scene_has_webnode = scene_has_webnode(edited_scene);
	editor->editor_set_visible_by_name("Web", _scene_has_webnode);
}

bool WebNodeEditorPlugin::scene_has_webnode(Node *p_node) {
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

bool WebNodeEditorPlugin::scene_has_webnode_skip(Node *p_node, Node *skip) {
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

void WebNodeEditorPlugin::on_node_removed(Node *p_node) {
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

WebNodeEditorPlugin::WebNodeEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	window = memnew(WebNodeEditor);

	editor->get_viewport()->add_child(window);
	_icon = get_editor_interface()->get_base_control()->get_theme_icon("WebNodeEditor", "EditorIcons");

	make_visible(false);

	_scene_has_webnode = false;

	Engine::get_singleton()->add_global("WebNodeEditor", window);

	editor->get_scene_tree_dock()->add_custom_scene_root_class("Web", "WebNode");
}

WebNodeEditorPlugin::~WebNodeEditorPlugin() {
	Engine::get_singleton()->remove_global("WebNodeEditor");
}

void WebNodeEditorPlugin::_notification(int p_what) {
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

void WebNodeEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_node_removed", "node"), &WebNodeEditorPlugin::on_node_removed);
}
