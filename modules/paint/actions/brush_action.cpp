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

#include "brush_action.h"

void BrushAction::do_action(PaintCanvas *canvas, Array data) {
	/*
	.do_action(canvas, data)

	for pixel in GEUtils.get_pixels_in_line(data[0], data[1]):
		for off in BrushPrefabs.get_brush(data[3], data[4]):
			var p = pixel + off

			if p in action_data.undo.cells or canvas.get_pixel_v(p) == null:
				continue

			if canvas.is_alpha_locked() and canvas.get_pixel_v(p) == Color.transparent:
				continue

			action_data.undo.colors.append(canvas.get_pixel_v(p))
			action_data.undo.cells.append(p)

			canvas.set_pixel_v(p, data[2])

			action_data.redo.cells.append(p)
			action_data.redo.colors.append(data[2])
	*/
}
void BrushAction::commit_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.redo.cells
	var colors = action_data.redo.colors
	*/
}

void BrushAction::undo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.undo.cells
	var colors = action_data.undo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}
void BrushAction::redo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.redo.cells
	var colors = action_data.redo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}

BrushAction::BrushAction() {
}

BrushAction::~BrushAction() {
}

void BrushAction::_bind_methods() {
}
