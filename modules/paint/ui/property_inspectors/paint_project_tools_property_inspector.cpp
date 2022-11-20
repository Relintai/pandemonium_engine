/*
Copyright (c) 2022 Péter Magyar

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

#include "paint_project_tools_property_inspector.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/scroll_container.h"

#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

void PaintProjectToolsPropertyInspector::_on_paint_node_selected(Node *p_paint_node) {
	PaintNode *paint_node = Object::cast_to<PaintNode>(p_paint_node);

	_current_paint_node = 0;
	_current_paint_project = 0;

	if (!paint_node) {
		return;
	}

	_current_paint_node = paint_node->get_instance_id();

	PaintProject *proj = paint_node->get_paint_project();

	if (!proj) {
		return;
	}

	proj->connect("current_color_changed", this, "_on_project_color_changed");
	proj->connect("color_presets_changed", this, "_on_project_color_preset_changed");

	_current_paint_project = proj->get_instance_id();

}

PaintProjectToolsPropertyInspector::PaintProjectToolsPropertyInspector() {
	_current_paint_node = 0;
	_current_paint_project = 0;
	_ignore_preset_changed_event = false;
	_ignore_color_event = false;

	VBoxContainer *main_container = memnew(VBoxContainer);
	add_child(main_container);

	_button_contianer = memnew(HFlowContainer);
	main_container->add_child(_button_contianer);
	//_button_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	_button_contianer->set_v_size_flags(SIZE_EXPAND_FILL);

	//Button *button = memnew(Button);
	//button->set_tooltip("");
	//button->connect("pressed", this, "");
	//_button_contianer->add_child(button);

}

PaintProjectToolsPropertyInspector::~PaintProjectToolsPropertyInspector() {
}

void PaintProjectToolsPropertyInspector::_notification(int p_what) {
	if (p_what == NOTIFICATION_THEME_CHANGED || p_what == NOTIFICATION_ENTER_TREE) {
		//_add_color_button->set_icon(get_theme_icon("Add", "EditorIcons"));
	}
}

void PaintProjectToolsPropertyInspector::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("_on_grid_color_button_changed"), &PaintProjectToolsPropertyInspector::_on_grid_color_button_changed);
}
