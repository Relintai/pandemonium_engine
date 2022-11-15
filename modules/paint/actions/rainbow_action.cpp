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

#include "rainbow_action.h"

#include "../deprecated/paint_canvas.h"
#include "../deprecated/paint_canvas_layer.h"
#include "../paint_utilities.h"

#include "../nodes/paint_canvas.h"

void RainbowAction::do_action_old(PaintCanvasOld *canvas, const Array &data) {
	PaintAction::do_action_old(canvas, data);

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		if (!canvas->validate_pixel_v(pixel)) {
			continue;
		}

		Color col = canvas->get_pixel_v(pixel);

		if (canvas->is_alpha_locked() && col.a < 0.0001) {
			continue;
		}

		if (undo_cells.contains(pixel)) {
			Color color = PaintUtilities::random_color();
			canvas->set_pixel_v(pixel, color);

			int idx = redo_cells.find(pixel);
			redo_cells.remove(idx);
			redo_colors.remove(idx);

			redo_cells.append(pixel);
			redo_colors.append(color);

			continue;
		}

		undo_colors.append(col);
		undo_cells.append(pixel);

		Color color = PaintUtilities::random_color();
		canvas->set_pixel_v(pixel, color);

		redo_cells.append(pixel);
		redo_colors.append(color);
	}
}

void RainbowAction::_do_action(const Array &data) {
	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		if (!_paint_canvas->validate_pixel_v(pixel)) {
			continue;
		}

		Color col = _paint_canvas->get_pixel_v(pixel);

		if (_paint_canvas->get_alpha_locked() && col.a < 0.0001) {
			continue;
		}

		if (undo_cells.contains(pixel)) {
			Color color = PaintUtilities::random_color();
			_paint_canvas->set_pixel_v(pixel, color);

			int idx = redo_cells.find(pixel);
			redo_cells.remove(idx);
			redo_colors.remove(idx);

			redo_cells.append(pixel);
			redo_colors.append(color);

			continue;
		}

		undo_colors.append(col);
		undo_cells.append(pixel);

		Color color = PaintUtilities::random_color();
		_paint_canvas->set_pixel_v(pixel, color);

		redo_cells.append(pixel);
		redo_colors.append(color);
	}
}

RainbowAction::RainbowAction() {
}

RainbowAction::~RainbowAction() {
}

void RainbowAction::_bind_methods() {
}
