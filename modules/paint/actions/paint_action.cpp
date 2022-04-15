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

void PaintAction::do_action(PaintCanvas *canvas, Array data) {
	/*
	if not "cells" in action_data.redo:
		action_data.redo["cells"] = []
		action_data.redo["colors"] = []

	if not "cells" in action_data.undo:
		action_data.undo["cells"] = []
		action_data.undo["colors"] = []

	if not "cells" in action_data.preview:
		action_data.preview["cells"] = []
		action_data.preview["colors"] = []

	if not "layer" in action_data:
		action_data["layer"] = canvas.active_layer
	*/
}
void PaintAction::commit_action(PaintCanvas *canvas) {
	/*
	print("NO IMPL commit_action ")
	return []
	*/
}

void PaintAction::undo_action(PaintCanvas *canvas) {
	/*
	print("NO IMPL undo_action ")
	*/
}
void PaintAction::redo_action(PaintCanvas *canvas) {
	/*
	print("NO IMPL redo_action ")
	*/
}
bool PaintAction::can_commit() {
	/*
	return not action_data.redo.empty()
	*/
}

Array PaintAction::get_x_sym_points(int canvas_width, Vector2i pixel) {
	/*
	var p = int(canvas_width - pixel.x)
	var all_points = [pixel, Vector2(p-1, pixel.y)]

	var points :Array = []
	for point in all_points:
		if point in points:
			continue
		points.append(point)
	return points
	*/
}
Array PaintAction::get_y_sym_points(int canvas_height, Vector2i pixel) {
	/*
	var p = int(canvas_height - pixel.y)
	var all_points = [pixel, Vector2(pixel.x, p-1)]

	var points :Array = []
	for point in all_points:
		if point in points:
			continue
		points.append(point)
	return points
	*/
}
Array PaintAction::get_xy_sym_points(int canvas_width, int canvas_height, Vector2i pixel) {
	/*
	var all_points = []
	var xpoints = get_x_sym_points(canvas_width, pixel)

	all_points += get_y_sym_points(canvas_height, xpoints[0])
	all_points += get_y_sym_points(canvas_height, xpoints[1])

	var points :Array = []
	for point in all_points:
		if point in points:
			continue
		points.append(point)

	return points
	*/
}
Array PaintAction::get_points(PaintCanvas *canvas, Vector2i pixel) {
	/*
	var points = []
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

	return points
	*/
}

PaintAction::PaintAction() {
	/*

	action_data["redo"] = {}
	action_data["undo"] = {}
	action_data["preview"] = {}

	*/
}

PaintAction::~PaintAction() {
}

void PaintAction::_bind_methods() {
}
