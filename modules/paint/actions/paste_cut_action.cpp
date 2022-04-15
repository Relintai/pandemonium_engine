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

#include "paste_cut_action.h"

//data[2] = selection_pos
//data[3] = selection_color
//data[4] = cut pos
//data[5] = cut size
void PasteCutAction::do_action(PaintCanvas *canvas, Array data) {
	/*
	.do_action(canvas, data)

	for pixel_pos in GEUtils.get_pixels_in_line(data[0], data[1]):
		for idx in range(data[2].size()):
			var pixel = data[2][idx]
			var color = data[3][idx]
			pixel -= data[4] + data[5] / 2
			pixel += pixel_pos

			if canvas.get_pixel_v(pixel) == null:
				continue

			if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
				continue

			var found = action_data.redo.cells.find(pixel)
			if found == -1:
				action_data.redo.cells.append(pixel)
				action_data.redo.colors.append(color)
			else:
				action_data.redo.colors[found] = color

			found = action_data.undo.cells.find(pixel)
			if found == -1:
				action_data.undo.colors.append(canvas.get_pixel_v(pixel))
				action_data.undo.cells.append(pixel)

			canvas.set_pixel_v(pixel, color)
	*/
}
void PasteCutAction::commit_action(PaintCanvas *canvas) {
	/*
	canvas.clear_preview_layer()
	*/
}

void PasteCutAction::undo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.undo.cells
	var colors = action_data.undo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}
void PasteCutAction::redo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.redo.cells
	var colors = action_data.redo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}

PasteCutAction::PasteCutAction() {
}

PasteCutAction::~PasteCutAction() {
}

void PasteCutAction::_bind_methods() {
}
