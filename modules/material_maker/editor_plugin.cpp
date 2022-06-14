#include "editor_plugin.h"

#include "editor/mat_maker_gd_editor.h"
#include "nodes/mm_material.h"

void MaterialMakerEditorPlugin::make_visible(bool p_visible) {
	if (tool_button) {
		if (p_visible) {
			tool_button->show();
		} else {
			//if tool_button.pressed:
			//	tool_button.pressed = false;

			if (!tool_button->is_pressed()) {
				tool_button->hide();
			}
		}
	}
}
void MaterialMakerEditorPlugin::edit(Object *p_object) {
	//if editor_scene:
	//	make_bottom_panel_item_visible(editor_scene)

	if (p_object->is_class("MMMaterial")) {
		editor_scene->set_mm_material(Ref<MMMaterial>(p_object));
	}
}
bool MaterialMakerEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("MMMaterial");
}

MaterialMakerEditorPlugin::MaterialMakerEditorPlugin(EditorNode *p_node) {
	_editor_node = p_node;
	editor_scene = nullptr;
	tool_button = nullptr;
}

MaterialMakerEditorPlugin::~MaterialMakerEditorPlugin() {
}

void MaterialMakerEditorPlugin::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		editor_scene = memnew(MatMakerGDEditor);
		editor_scene->set_undo_redo(&get_undo_redo());

		tool_button = add_control_to_bottom_panel(editor_scene, "MMGD");
		tool_button->hide();
	} else if (p_what == NOTIFICATION_EXIT_TREE) {
		remove_control_from_bottom_panel(editor_scene);

		if (editor_scene) {
			editor_scene->queue_delete();
		}

		editor_scene = nullptr;
		tool_button = nullptr;
	}
}
