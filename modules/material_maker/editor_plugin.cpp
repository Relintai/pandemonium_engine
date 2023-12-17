/*************************************************************************/
/*  editor_plugin.cpp                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
