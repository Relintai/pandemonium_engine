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

#include "paint_action.h"

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"

void PaintAction::do_action(PaintCanvas *canvas, const Array &data) {
	layer = canvas->get_active_layer();
}
void PaintAction::commit_action(PaintCanvas *canvas) {
	ERR_PRINT("NO IMPL commit_action");
}

void PaintAction::undo_action(PaintCanvas *canvas) {
	for (int idx = 0; idx < undo_cells.size(); ++idx) {
		canvas->_set_pixel_v(layer, undo_cells[idx], undo_colors[idx]);
	}
}
void PaintAction::redo_action(PaintCanvas *canvas) {
	for (int idx = 0; idx < redo_cells.size(); ++idx) {
		canvas->_set_pixel_v(layer, redo_cells[idx], redo_colors[idx]);
	}
}

bool PaintAction::can_commit() {
	return !redo_cells.empty();
}

PoolVector2iArray PaintAction::get_x_sym_points(const int canvas_width, const Vector2i &pixel) {
	int p = canvas_width - pixel.x;

	PoolVector2iArray points;
	points.append(pixel);
	points.append(Vector2i(p - 1, pixel.y));

	return points;
}
PoolVector2iArray PaintAction::get_y_sym_points(const int canvas_height, const Vector2i &pixel) {
	int p = canvas_height - pixel.y;

	PoolVector2iArray points;
	points.append(pixel);
	points.append(Vector2i(pixel.x, p - 1));

	return points;
}
PoolVector2iArray PaintAction::get_xy_sym_points(const int canvas_width, const int canvas_height, const Vector2i &pixel) {
	PoolVector2iArray all_points;
	PoolVector2iArray xpoints = get_x_sym_points(canvas_width, pixel);

	all_points.append_array(get_y_sym_points(canvas_height, xpoints[0]));
	all_points.append_array(get_y_sym_points(canvas_height, xpoints[1]));

	PoolVector2iArray points;
	for (int i = 0; i < all_points.size(); ++i) { //point in all_points:
		Vector2i point = all_points[i];
		bool found = false;

		for (int j = 0; j < points.size(); ++j) {
			if (points[j] == point) {
				found = true;
				break;
			}
		}

		if (!found) {
			points.append(point);
		}
	}

	return points;
}
PoolVector2iArray PaintAction::get_points(PaintCanvas *canvas, const Vector2i &pixel) {
	/*
	PoolVector2iArray points;
	if canvas.symmetry_x and canvas.symmetry_y:
		var sym_points = get_xy_sym_points(canvas.canvas_width, canvas.canvas_height, pixel)
		for point in sym_points:
			if point in action_data.undo.cells or canvas.get_pixel_v(point) == null:
				continue
			if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
				continue
			points.append(point)
	elif canvas.symmetry_y:
		var sym_points = get_y_sym_points(canvas.canvas_height, pixel)
		for point in sym_points:
			if point in action_data.undo.cells or canvas.get_pixel_v(point) == null:
				continue
			if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
				continue
			points.append(point)
	elif canvas.symmetry_x:
		var sym_points = get_x_sym_points(canvas.canvas_width, pixel)
		for point in sym_points:
			if point in action_data.undo.cells or canvas.get_pixel_v(point) == null:
				continue
			if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
				continue
			points.append(point)
	else:
		if pixel in action_data.undo.cells or canvas.get_pixel_v(pixel) == null:
			return []
		if canvas.is_alpha_locked() and canvas.get_pixel_v(pixel) == Color.transparent:
			return []
		points.append(pixel)

	return points;
	*/

	return PoolVector2iArray();
}

bool PaintAction::pv2ia_contains(const PoolVector2iArray &arr, const Vector2i &v) const {
	PoolVector2iArray::Read r = arr.read();
	int s = arr.size();

	for (int i = 0; i < s; ++s) {
		if (r[i] == v) {
			return true;
		}
	}

	return false;
}

void PaintAction::draw_points(PaintCanvas *canvas, const PoolVector2iArray &point_arr, const PoolColorArray &color_arr) {
	for (int i = 0; i < point_arr.size(); ++i) {
		Vector2i pixel = point_arr[i];

		Color col = color_arr[i];

		if (canvas->is_alpha_locked() && col.a < 0.00001) {
			continue;
		}

		undo_cells.append(pixel);
		undo_colors.append(col);

		canvas->set_pixel_v(pixel, col);

		redo_cells.append(pixel);
		redo_colors.append(col);
	}
}

PaintAction::PaintAction() {
}

PaintAction::~PaintAction() {
}

void PaintAction::_bind_methods() {
}
