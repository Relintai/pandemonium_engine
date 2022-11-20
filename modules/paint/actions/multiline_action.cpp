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

#include "multiline_action.h"

#include "../paint_utilities.h"

#include "../nodes/paint_canvas.h"

bool MultiLineAction::_can_commit() {
	return false;
}

void MultiLineAction::_do_action(const Array &data) {
	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		Color col = _paint_canvas->get_pixel_v(pixel);

		if (undo_cells.contains(pixel) || !_paint_canvas->validate_pixel_v(pixel) || (_paint_canvas->get_alpha_locked() && col.a < 0.0001)) {
			continue;
		}

		undo_colors.append(col);
		undo_cells.append(pixel);

		Color tpx = data[2];

		_paint_canvas->set_pixel_v(pixel, tpx);

		redo_cells.append(pixel);
		redo_colors.append(tpx);
	}
}

MultiLineAction::MultiLineAction() {
}

MultiLineAction::~MultiLineAction() {
}

void MultiLineAction::_bind_methods() {
}
