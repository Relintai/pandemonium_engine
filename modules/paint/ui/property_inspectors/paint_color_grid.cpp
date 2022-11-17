/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
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

#include "paint_color_grid.h"

#include "scene/gui/button.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/resources/style_box.h"

#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

void PaintColorGrid::change_color_to(const Color &color) {
	emit_signal("color_change_request", color);

	PaintProject *proj = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	if (proj) {
		proj->set_current_color(color);
	}
}
void PaintColorGrid::add_color_prefab(const Color &color) {
	Button *button = memnew(Button);

	button->set_custom_minimum_size(Size2(25, 25));
	button->set_h_size_flags(SIZE_EXPAND_FILL);
	button->set_v_size_flags(SIZE_EXPAND_FILL);

	_grid->add_child(button);
	_grid->move_child(button, 0);

	Ref<StyleBoxFlat> style_box;
	style_box.instance();
	style_box->set("bg_color", color);

	button->set("custom_styles/normal", style_box);

	Vector<Variant> binds;
	binds.push_back(color);

	button->connect("pressed", this, "change_color_to", binds);
}

void PaintColorGrid::_on_paint_node_selected(Node *p_paint_node) {
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

PaintColorGrid::PaintColorGrid() {
	_current_paint_node = 0;
	_current_paint_project = 0;

	ScrollContainer *scroll_container = memnew(ScrollContainer);
	scroll_container->set_custom_minimum_size(Size2(0, 145));
	scroll_container->set_enable_h_scroll(false);
	add_child(scroll_container);

	_grid = memnew(GridContainer);
	scroll_container->add_child(_grid);

	_grid->set_columns(5);
	//_grid->set_custom_minimum_size(Size2(0, 145));
	_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	_grid->set_v_size_flags(SIZE_EXPAND_FILL);

	for (int i = 0; i < 24; ++i) {
		add_color_prefab(Color(Math::randf(), Math::randf(), Math::randf()));
	}
}

PaintColorGrid::~PaintColorGrid() {
}

void PaintColorGrid::_bind_methods() {
	ADD_SIGNAL(MethodInfo("color_change_request", PropertyInfo(Variant::COLOR, "color")));

	ClassDB::bind_method(D_METHOD("change_color_to"), &PaintColorGrid::change_color_to);
	ClassDB::bind_method(D_METHOD("add_color_prefab"), &PaintColorGrid::add_color_prefab);
}
