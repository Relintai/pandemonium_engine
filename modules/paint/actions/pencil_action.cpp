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

#include "pencil_action.h"

#include "../deprecated/paint_canvas.h"
#include "../deprecated/paint_canvas_layer.h"
#include "../paint_utilities.h"
#include "core/string/print_string.h"

void PencilAction::do_action_old(PaintCanvasOld *canvas, const Array &data) {
	PaintAction::do_action_old(canvas, data);

	Color c = data[2];

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		PoolVector2iArray points = get_points_old(canvas, pixel);

		for (int j = 0; j < points.size(); ++j) {
			Vector2i p = points[j];

			_set_pixel(canvas, p, c);
		}
	}
}

void PencilAction::_set_pixel(PaintCanvasOld *canvas, Vector2i pixel, Color color) {
	undo_colors.append(canvas->get_pixel_v(pixel));
	undo_cells.append(pixel);

	canvas->set_pixel_v(pixel, color);

	redo_cells.append(pixel);
	redo_colors.append(color);
}

PencilAction::PencilAction() {
}

PencilAction::~PencilAction() {
}

void PencilAction::_bind_methods() {
}
