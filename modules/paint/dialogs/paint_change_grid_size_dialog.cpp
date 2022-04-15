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

void PaintChangeGridSizeDialog::_ready() {
	/*
	yield(owner, "ready")
	find_node("GridValue").value = owner.paint_canvas.grid_size
	find_node("BigGridValue").value = owner.paint_canvas.big_grid_size
	*/
}

void PaintChangeGridSizeDialog::_on_ChangeGridSizeDialog_confirmed() {
	/*
	var grid_size = find_node("GridValue").value
	var big_grid_size = find_node("BigGridValue").value
	owner.paint_canvas.grid_size = grid_size
	owner.paint_canvas.big_grid_size = big_grid_size
	*/
}
void PaintChangeGridSizeDialog::_on_GridValue_value_changed(float value) {
	/*
	var grid_size = value
	owner.paint_canvas.grid_size = grid_size
	*/
}
void PaintChangeGridSizeDialog::_on_BigGridValue_value_changed(float value) {
	/*
	var big_grid_size = value
	owner.paint_canvas.big_grid_size = big_grid_size
	*/
}

PaintChangeGridSizeDialog::PaintChangeGridSizeDialog() {
}

PaintChangeGridSizeDialog::~PaintChangeGridSizeDialog() {
}

void PaintChangeGridSizeDialog::_bind_methods() {
}
