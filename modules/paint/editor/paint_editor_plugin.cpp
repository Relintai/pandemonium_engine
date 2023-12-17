

#include "paint_editor_plugin.h"

#include "core/config/engine.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/plugins/canvas_item_editor_plugin.h"

#include "paint_inspector_plugin.h"

#include "../nodes/paint_node.h"

void PaintEditorPlugin::make_visible(const bool visible) {
}

String PaintEditorPlugin::get_name() const {
	return "PaintNodeEditor";
}

void PaintEditorPlugin::edit(Object *p_object) {
	_active_node = Object::cast_to<PaintNode>(p_object);
}
bool PaintEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("PaintNode");
}
void PaintEditorPlugin::edited_scene_changed() {
}

bool PaintEditorPlugin::forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	if (!_active_node) {
		return false;
	}

	if (!ObjectDB::instance_validate(_active_node)) {
		_active_node = NULL;
		return false;
	}

	if (CanvasItemEditor::get_singleton() && CanvasItemEditor::get_singleton()->get_current_tool() != CanvasItemEditor::TOOL_SELECT) {
		return false;
	}

	return _active_node->forward_canvas_gui_input(p_event);
}
void PaintEditorPlugin::forward_canvas_draw_over_viewport(Control *p_overlay) {
}
void PaintEditorPlugin::forward_canvas_force_draw_over_viewport(Control *p_overlay) {
}

PaintEditorPlugin::PaintEditorPlugin(EditorNode *p_node) {
	_active_node = NULL;

	editor = p_node;

	_inspector_plugin = memnew(PaintInspectorPlugin);
	add_inspector_plugin(_inspector_plugin);

	editor->get_scene_tree_dock()->add_custom_scene_root_class("Paint", "PaintProject");
}

PaintEditorPlugin::~PaintEditorPlugin() {
}

void PaintEditorPlugin::on_node_removed(Node *node) {
	if (_active_node == node) {
		_active_node = NULL;
	}
}

void PaintEditorPlugin::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		if (!get_tree()->is_connected("node_removed", this, "on_node_removed")) {
			get_tree()->connect("node_removed", this, "on_node_removed");
		}
	}
}

void PaintEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_node_removed", "node"), &PaintEditorPlugin::on_node_removed);
}
