/*
Copyright (c) 2020-2023 Péter Magyar

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

#include "prop_2d_editor_plugin.h"

#include "../props/prop_2d_data.h"
#include "../singleton/prop_2d_utils.h"
#include "core/input/input.h"
#include "core/os/keyboard.h"
#include "editor/editor_settings.h"
#include "scene/gui/box_container.h"
#include "scene/gui/separator.h"

void Prop2DEditorPlugin::convert_active_scene_to_prop_data() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		Node *scene = st->get_edited_scene_root();

		if (scene) {
			EditorData &ed = EditorNode::get_editor_data();
			String path = ed.get_scene_path(ed.get_edited_scene());

			convert_scene(scene, path.get_basename() + ".tres");
		}
	}
}
void Prop2DEditorPlugin::convert_selected_scene_to_prop_data() {
}

void Prop2DEditorPlugin::convert_scene(Node *root, const String &path) {
	Ref<Prop2DData> data = Prop2DUtils::get_singleton()->convert_tree(root);

	ERR_FAIL_COND(!data.is_valid());

	ResourceLoader l;
	if (l.exists(path)) {
		Ref<Prop2DData> res = l.load(path, "Prop2DData");

		ERR_FAIL_COND(!res.is_valid());

		res->copy_from(data);

		ResourceSaver s;
		s.save(path, res);

		res.unref();
	} else {
		ResourceSaver s;
		s.save(path, data);
	}
}

void Prop2DEditorPlugin::find_room_points(Variant param) {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		Node *scene = st->get_edited_scene_root();

		if (scene) {
			//Prop2DUtils::get_singleton()->generate_room_points_node(scene);
		}
	}
}

void Prop2DEditorPlugin::_quick_convert_button_pressed() {
	convert_active_scene_to_prop_data();
}

void Prop2DEditorPlugin::_convert_active_scene_to_prop_data(Variant param) {
	convert_active_scene_to_prop_data();
}
void Prop2DEditorPlugin::_convert_selected_scene_to_prop_data(Variant param) {
	convert_selected_scene_to_prop_data();
}

Prop2DEditorPlugin::Prop2DEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	editor->add_tool_menu_item("Convert active scene to Prop2DData", this, "convert_active_scene_to_prop_data");
	editor->add_tool_menu_item("Convert selected scene(s) to Prop2DData", this, "convert_selected_scene_to_prop_data");
	editor->add_tool_menu_item("(Prop2D) Find room points.", this, "find_room_points");

	HBoxContainer *bc = memnew(HBoxContainer);
	bc->add_child(memnew(VSeparator));

	Button *b = memnew(Button);
	bc->add_child(b);
	b->set_flat(true);

	b->connect("pressed", this, "_quick_convert_button_pressed");
	b->set_text("To Prop2D");
	b->set_shortcut(ED_SHORTCUT("spatial_editor/quick_prop_convert", "Quick convert scene to Prop2DData.", KEY_MASK_ALT + KEY_U));

	add_control_to_container(EditorPlugin::CONTAINER_CANVAS_EDITOR_MENU, bc);
}

Prop2DEditorPlugin::~Prop2DEditorPlugin() {
}

void Prop2DEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("convert_active_scene_to_prop_data"), &Prop2DEditorPlugin::_convert_active_scene_to_prop_data);
	ClassDB::bind_method(D_METHOD("convert_selected_scene_to_prop_data"), &Prop2DEditorPlugin::_convert_active_scene_to_prop_data);

	ClassDB::bind_method(D_METHOD("find_room_points"), &Prop2DEditorPlugin::find_room_points);

	ClassDB::bind_method(D_METHOD("_quick_convert_button_pressed"), &Prop2DEditorPlugin::_quick_convert_button_pressed);
}
