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

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/scroll_container.h"

#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

void PaintProjectPropertyInspector::add_grid_button(const Color &color, const int color_index) {
	ColorSelectorButton *color_selector_button = memnew(ColorSelectorButton);

	color_selector_button->set_custom_minimum_size(Size2(35, 30));

	_color_grid->add_child(color_selector_button);

	color_selector_button->set_pick_color(color);
	color_selector_button->set_meta("color_index", color_index);
	color_selector_button->connect("color_changed", this, "_on_grid_color_button_changed", varray(color_selector_button));
	color_selector_button->connect("pressed", this, "_on_grid_color_button_pressed", varray(color_selector_button));
}

void PaintProjectPropertyInspector::create_grid_buttons() {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (!proj) {
		return;
	}

	PoolColorArray colors = proj->get_color_presets();

	for (int i = 0; i < colors.size(); ++i) {
		add_grid_button(colors[i], i);
	}
}

void PaintProjectPropertyInspector::clear_grid_buttons() {
	for (int i = 0; i < _color_grid->get_child_count(); ++i) {
		_color_grid->get_child(i)->queue_delete();
	}
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

	if (!proj) {
		return;
	}

	_current_paint_project = proj->get_instance_id();
	_main_color_button->set_pick_color(proj->get_current_color());

	create_grid_buttons();
}

void PaintProjectPropertyInspector::_on_grid_color_button_changed(const Color &color, Node *p_button) {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ColorSelectorButton *button = Object::cast_to<ColorSelectorButton>(p_button);
	ERR_FAIL_COND(!button);

	if (proj) {
		_ignore_preset_changed_event = true;

		int color_index = button->get_meta("color_index");

		proj->set_preset_color(color_index, color);
		
		_ignore_preset_changed_event = false;
	}
}

void PaintProjectPropertyInspector::_on_grid_color_button_pressed(Node *p_button) {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ColorSelectorButton *button = Object::cast_to<ColorSelectorButton>(p_button);
	ERR_FAIL_COND(!button);

	if (proj) {
		_main_color_button->set_pick_color(button->get_pick_color());
		_on_main_color_changed(button->get_pick_color());
	}
}

void PaintProjectPropertyInspector::_on_main_color_changed(const Color &color) {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		proj->set_current_color(_main_color_button->get_pick_color());
	}
}
void PaintProjectPropertyInspector::_on_main_color_selected() {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		proj->set_current_color(_main_color_button->get_pick_color());
	}
}

void PaintProjectPropertyInspector::_on_add_color_button_pressed() {
	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		_ignore_preset_changed_event = true;
		Color c = _main_color_button->get_pick_color();
		proj->add_preset_color(c);
		add_grid_button(c, proj->get_preset_color_count() - 1);
		_ignore_preset_changed_event = false;
	}
}

void PaintProjectPropertyInspector::_on_project_color_preset_changed() {
	if (_ignore_preset_changed_event) {
		return;
	}

	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		clear_grid_buttons();
		create_grid_buttons();
	}
}

PaintProjectPropertyInspector::PaintProjectPropertyInspector() {
	_current_paint_node = 0;
	_current_paint_project = 0;
	_ignore_preset_changed_event = false;

	VBoxContainer *main_container = memnew(VBoxContainer);
	add_child(main_container);

	ScrollContainer *scroll_container = memnew(ScrollContainer);
	scroll_container->set_custom_minimum_size(Size2(0, 145));
	scroll_container->set_enable_h_scroll(false);
	main_container->add_child(scroll_container);

	_color_grid = memnew(HFlowContainer);
	scroll_container->add_child(_color_grid);
	_color_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	_color_grid->set_v_size_flags(SIZE_EXPAND_FILL);

	HBoxContainer *row_container = memnew(HBoxContainer);
	main_container->add_child(row_container);
	_color_grid->set_h_size_flags(SIZE_EXPAND_FILL);

	_main_color_button = memnew(ColorSelectorButton);
	_main_color_button->set_custom_minimum_size(Size2(35, 30));
	_main_color_button->set_h_size_flags(SIZE_EXPAND_FILL);
	row_container->add_child(_main_color_button);
	_main_color_button->connect("color_changed", this, "_on_main_color_changed");
	_main_color_button->connect("pressed", this, "_on_main_color_selected");

	_add_color_button = memnew(Button);
	row_container->add_child(_add_color_button);
	_add_color_button->set_tooltip("Save color.");
	_add_color_button->connect("pressed", this, "_on_add_color_button_pressed");
}

PaintProjectPropertyInspector::~PaintProjectPropertyInspector() {
}

void PaintProjectPropertyInspector::_notification(int p_what) {
	if (p_what == NOTIFICATION_THEME_CHANGED || p_what == NOTIFICATION_ENTER_TREE) {
		_add_color_button->set_icon(get_theme_icon("Add", "EditorIcons"));
	}
}

void PaintProjectPropertyInspector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_grid_color_button_changed"), &PaintProjectPropertyInspector::_on_grid_color_button_changed);
	ClassDB::bind_method(D_METHOD("_on_grid_color_button_pressed"), &PaintProjectPropertyInspector::_on_grid_color_button_pressed);

	ClassDB::bind_method(D_METHOD("_on_main_color_changed"), &PaintProjectPropertyInspector::_on_main_color_changed);
	ClassDB::bind_method(D_METHOD("_on_main_color_selected"), &PaintProjectPropertyInspector::_on_main_color_selected);

	ClassDB::bind_method(D_METHOD("_on_add_color_button_pressed"), &PaintProjectPropertyInspector::_on_add_color_button_pressed);
}
