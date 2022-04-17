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

#include "paint_settings.h"

#include "paint_canvas_outline.h"

#include "scene/gui/box_container.h"
#include "scene/gui/check_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/label.h"
#include "scene/gui/spin_box.h"

void PaintSettings::_on_ColorPickerButton_color_changed(const Color &color) {
	canvas_outline->color = color;
	canvas_outline->update();
}
void PaintSettings::_on_CheckButton_toggled(const bool button_pressed) {
	canvas_outline->set_visible(button_pressed);
}

PaintSettings::PaintSettings() {
	set_resizable(true);
	set_title("Settings");

	VBoxContainer *main_box_container = memnew(VBoxContainer);
	add_child(main_box_container);

	HBoxContainer *outline_container = memnew(HBoxContainer);
	outline_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	outline_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(outline_container);

	Label *outline_label = memnew(Label);
	outline_label->set_text("Canvas Outline:");
	outline_label->set_valign(Label::VALIGN_CENTER);
	outline_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	outline_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	outline_container->add_child(outline_label);

	_check_button = memnew(CheckButton);
	_check_button->set_pressed(true);
	_check_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	_check_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	_check_button->connect("toggled", this, "_on_CheckButton_toggled");
	outline_container->add_child(_check_button);

	HBoxContainer *color_picker_container = memnew(HBoxContainer);
	color_picker_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	color_picker_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(color_picker_container);

	Label *grid_label = memnew(Label);
	grid_label->set_text("Canvas Outline Color:");
	grid_label->set_valign(Label::VALIGN_CENTER);
	grid_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	color_picker_container->add_child(grid_label);

	_color_picker_button = memnew(ColorPickerButton);
	_color_picker_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	_color_picker_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	_color_picker_button->set_custom_minimum_size(Size2(100, 0));
	_color_picker_button->connect("color_changed", this, "_on_ColorPickerButton_color_changed");
	color_picker_container->add_child(_color_picker_button);
}

PaintSettings::~PaintSettings() {
}

void PaintSettings::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_ColorPickerButton_color_changed"), &PaintSettings::_on_ColorPickerButton_color_changed);
	ClassDB::bind_method(D_METHOD("_on_CheckButton_toggled"), &PaintSettings::_on_CheckButton_toggled);
}
