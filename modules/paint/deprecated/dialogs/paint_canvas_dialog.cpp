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

#include "paint_canvas_dialog.h"

#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/spin_box.h"

int PaintCanvasDialog::get_size_x() const {
	return static_cast<int>(size_x_spin_box->get_value());
}
void PaintCanvasDialog::set_size_x(const int val) {
	size_x_spin_box->set_value(val);
	_x_prev_val = val;
}

int PaintCanvasDialog::get_size_y() const {
	return static_cast<int>(size_y_spin_box->get_value());
}
void PaintCanvasDialog::set_size_y(const int val) {
	size_y_spin_box->set_value(val);
	_y_prev_val = val;
}

void PaintCanvasDialog::_on_confirmed() {
	_x_prev_val = get_size_x();
	_y_prev_val = get_size_y();
}

void PaintCanvasDialog::_on_about_to_show() {
	set_size_x(_x_prev_val);
	set_size_y(_y_prev_val);
}

PaintCanvasDialog::PaintCanvasDialog() {
	_x_prev_val = 0;
	_y_prev_val = 0;
	is_new = false;

	set_resizable(true);
	set_title("Please Confirm...");

	VBoxContainer *main_box_container = memnew(VBoxContainer);
	add_child(main_box_container);

	Label *main_label = memnew(Label);
	main_label->set_text("Change canvas size?");
	main_label->set_align(Label::ALIGN_CENTER);
	main_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	main_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(main_label);

	HBoxContainer *grid_spin_container = memnew(HBoxContainer);
	grid_spin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(grid_spin_container);

	Label *grid_label = memnew(Label);
	grid_label->set_text("Width (X)");
	grid_label->set_align(Label::ALIGN_CENTER);
	grid_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->add_child(grid_label);

	size_x_spin_box = memnew(SpinBox);
	size_x_spin_box->set_value(64);
	size_x_spin_box->set_min(1);
	size_x_spin_box->set_max(2500);
	size_x_spin_box->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	size_x_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->add_child(size_x_spin_box);

	HBoxContainer *big_grid_spin_container = memnew(HBoxContainer);
	big_grid_spin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(big_grid_spin_container);

	grid_label = memnew(Label);
	grid_label->set_text("Height (Y)");
	grid_label->set_align(Label::ALIGN_CENTER);
	grid_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->add_child(grid_label);

	size_y_spin_box = memnew(SpinBox);
	size_y_spin_box->set_value(64);
	size_y_spin_box->set_min(1);
	size_y_spin_box->set_max(2500);
	size_y_spin_box->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	size_y_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->add_child(size_y_spin_box);
}

PaintCanvasDialog::~PaintCanvasDialog() {
}

void PaintCanvasDialog::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("confirmed", this, "_on_confirmed");
			connect("about_to_show", this, "_on_about_to_show");
		} break;
		default: {
		} break;
	}
}

void PaintCanvasDialog::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_confirmed"), &PaintCanvasDialog::_on_confirmed);
	ClassDB::bind_method(D_METHOD("_on_about_to_show"), &PaintCanvasDialog::_on_about_to_show);
}
