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

#include "paint_project_property_inspector.h"

#include "scene/gui/button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/scroll_container.h"

#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

void PaintProjectPropertyInspector::add_grid_button(const Color &color) {
	ColorSelectorButton *color_selector_button = memnew(ColorSelectorButton);

	color_selector_button->set_custom_minimum_size(Size2(35, 30));

	_color_grid->add_child(color_selector_button);

	color_selector_button->set_pick_color(color);
	color_selector_button->connect("color_changed", this, "_on_grid_color_changed", varray(_color_grid->get_child_count() - 1));
	color_selector_button->connect("pressed", this, "_on_grid_color_selected", varray(_color_grid->get_child_count() - 1));
}

void PaintProjectPropertyInspector::_on_paint_node_selected(Node *p_paint_node) {
	PaintNode *paint_node = Object::cast_to<PaintNode>(p_paint_node);

	_current_paint_node = 0;
	_current_paint_project = 0;

	if (!paint_node) {
		return;
	}

	_current_paint_node = paint_node->get_instance_id();

	PaintProject *proj = paint_node->get_paint_project();

	if (proj) {
		_current_paint_project = proj->get_instance_id();
	}
}

void PaintProjectPropertyInspector::_on_grid_color_changed(const Color &color, const int index) {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		//proj->set_current_color(color);
		//store
	}
}

void PaintProjectPropertyInspector::_on_grid_color_selected(const int index) {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ColorSelectorButton *button = Object::cast_to<ColorSelectorButton>(_color_grid->get_child(index));
	ERR_FAIL_COND(!button);

	if (proj) {
		proj->set_current_color(button->get_pick_color());
	}
}

PaintProjectPropertyInspector::PaintProjectPropertyInspector() {
	_current_paint_node = 0;
	_current_paint_project = 0;

	ScrollContainer *scroll_container = memnew(ScrollContainer);
	scroll_container->set_custom_minimum_size(Size2(0, 145));
	scroll_container->set_enable_h_scroll(false);
	add_child(scroll_container);

	_color_grid = memnew(HFlowContainer);
	scroll_container->add_child(_color_grid);

	_color_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	_color_grid->set_v_size_flags(SIZE_EXPAND_FILL);

	//TODO add button
	for (int i = 0; i < 30; ++i) {
		add_grid_button(Color(Math::randf(), Math::randf(), Math::randf()));
	}
}

PaintProjectPropertyInspector::~PaintProjectPropertyInspector() {
}

void PaintProjectPropertyInspector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_grid_color_changed"), &PaintProjectPropertyInspector::_on_grid_color_changed);
	ClassDB::bind_method(D_METHOD("_on_grid_color_selected"), &PaintProjectPropertyInspector::_on_grid_color_selected);
}
