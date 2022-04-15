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

#include "cut_action.h"

bool CutAction::can_commit() {
	return false; //ugly way of handling a cut
}

void CutAction::do_action(PaintCanvas *canvas, Array data) {
	/*
	.do_action(canvas, data)

	if mouse_start_pos == null:
		mouse_start_pos = data[0]
	mouse_end_pos = data[0]

	action_data.preview.cells.clear()
	action_data.preview.colors.clear()
	canvas.clear_preview_layer()

	var p = mouse_start_pos
	var s = mouse_end_pos - mouse_start_pos

	var pixels = GEUtils.get_pixels_in_line(p, p + Vector2(s.x, 0))
	pixels += GEUtils.get_pixels_in_line(p, p + Vector2(0, s.y))
	pixels += GEUtils.get_pixels_in_line(p + s, p + s + Vector2(0, -s.y))
	pixels += GEUtils.get_pixels_in_line(p + s, p + s  + Vector2(-s.x, 0))

	for pixel in pixels:
		canvas.set_preview_pixel_v(pixel, selection_color)
		action_data.preview.cells.append(pixel)
		action_data.preview.colors.append(selection_color)
	*/
}
void CutAction::commit_action(PaintCanvas *canvas) {
	/*
	canvas.clear_preview_layer()
	var p = mouse_start_pos
	var s = mouse_end_pos - mouse_start_pos

	for x in range(abs(s.x)+1):
		for y in range(abs(s.y)+1):
			var px = x
			var py = y
			if s.x < 0:
				px *= -1
			if s.y < 0:
				py *= -1

			var pos = p + Vector2(px, py)
			var color = canvas.get_pixel(pos.x, pos.y)

			if color == null or color.a == 0.0:
				continue

			action_data.redo.cells.append(pos)
			action_data.redo.colors.append(canvas.get_pixel_v(pos))

			canvas.set_pixel_v(pos, Color.transparent)

			action_data.undo.cells.append(pos)
			action_data.undo.colors.append(Color.transparent)
	*/
}

void CutAction::undo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.undo.cells
	var colors = action_data.undo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}
void CutAction::redo_action(PaintCanvas *canvas) {
	/*
	var cells = action_data.redo.cells
	var colors = action_data.redo.colors
	for idx in range(cells.size()):
		canvas._set_pixel_v(action_data.layer, cells[idx], colors[idx])
	*/
}

CutAction::CutAction() {
}

CutAction::~CutAction() {
}

void CutAction::_bind_methods() {
}
