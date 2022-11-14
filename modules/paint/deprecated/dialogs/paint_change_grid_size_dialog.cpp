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

#include "paint_change_grid_size_dialog.h"

#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/spin_box.h"

int PaintChangeGridSizeDialog::get_grid_value() const {
	return static_cast<int>(grid_spin_box->get_value());
}
void PaintChangeGridSizeDialog::set_grid_value(const int val) {
	grid_spin_box->set_value(val);
}

int PaintChangeGridSizeDialog::get_big_grid_value() const {
	return static_cast<int>(big_grid_spin_box->get_value());
}
void PaintChangeGridSizeDialog::set_big_grid_value(const int val) {
	big_grid_spin_box->set_value(val);
}

PaintChangeGridSizeDialog::PaintChangeGridSizeDialog() {
	set_resizable(true);
	set_title("Change Grid Size");

	VBoxContainer *main_box_container = memnew(VBoxContainer);
	add_child(main_box_container);

	Label *main_label = memnew(Label);
	main_label->set_text("Change Grid Size");
	main_label->set_align(Label::ALIGN_CENTER);
	main_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	main_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(main_label);

	HBoxContainer *grid_spin_container = memnew(HBoxContainer);
	grid_spin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(grid_spin_container);

	Label *grid_label = memnew(Label);
	grid_label->set_text("Grid 1");
	grid_label->set_align(Label::ALIGN_CENTER);
	grid_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->add_child(grid_label);

	grid_spin_box = memnew(SpinBox);
	grid_spin_box->set_value(1);
	grid_spin_box->set_max(2500);
	grid_spin_box->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid_spin_container->add_child(grid_spin_box);

	HBoxContainer *big_grid_spin_container = memnew(HBoxContainer);
	big_grid_spin_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_box_container->add_child(big_grid_spin_container);

	grid_label = memnew(Label);
	grid_label->set_text("Grid 2");
	grid_label->set_align(Label::ALIGN_CENTER);
	grid_label->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	grid_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->add_child(grid_label);

	big_grid_spin_box = memnew(SpinBox);
	big_grid_spin_box->set_value(8);
	big_grid_spin_box->set_max(2500);
	big_grid_spin_box->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	big_grid_spin_container->add_child(big_grid_spin_box);
}

PaintChangeGridSizeDialog::~PaintChangeGridSizeDialog() {
}

void PaintChangeGridSizeDialog::_bind_methods() {
}
