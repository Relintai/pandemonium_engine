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

#include "rect_action.h"

void RectAction::do_action(PaintCanvas *canvas, const Array &data) {
	/*
	.do_action(canvas, data)

	if mouse_start_pos == null:
		mouse_start_pos = data[0]
		#print("init:", mouse_start_pos)


	action_data.undo.cells.clear()
	action_data.undo.colors.clear()
	action_data.preview.cells.clear()
	action_data.preview.colors.clear()
	canvas.clear_preview_layer()

	var p = mouse_start_pos
	var s = data[0] - mouse_start_pos
	var pixels = GEUtils.get_pixels_in_line(p, p + Vector2(s.x, 0))
	pixels += GEUtils.get_pixels_in_line(p, p + Vector2(0, s.y))
	pixels += GEUtils.get_pixels_in_line(p + s, p + s + Vector2(0, -s.y))
	pixels += GEUtils.get_pixels_in_line(p + s, p + s  + Vector2(-s.x, 0))

	for pixel in pixels:
		if canvas.get_pixel_v(pixel) == null:
			continue

		if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
			continue

		canvas.set_preview_pixel_v(pixel, data[2])
		action_data.undo.cells.append(pixel)
		action_data.undo.colors.append(canvas.get_pixel_v(pixel))
		action_data.preview.cells.append(pixel)
		action_data.preview.colors.append(data[2])
	*/
}
void RectAction::commit_action(PaintCanvas *canvas) {
	/*
	canvas.clear_preview_layer()
	var cells = action_data.preview.cells
	var colors = action_data.preview.colors
	for idx in range(cells.size()):
		canvas.set_pixel_v(cells[idx], colors[idx])

		action_data.redo.cells.append(cells[idx])
		action_data.redo.colors.append(colors[idx])
	mouse_start_pos = null
	*/
}

void RectAction::undo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.undo.cells
	var colors = action_data.undo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}
void RectAction::redo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.redo.cells
	var colors = action_data.redo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}

RectAction::RectAction() {
}

RectAction::~RectAction() {
}

void RectAction::_bind_methods() {
}
