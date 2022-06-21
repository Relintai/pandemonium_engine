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

#include "bucket_action.h"

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"
#include "../paint_utilities.h"

void BucketAction::do_action(PaintCanvas *canvas, const Array &data) {
	PaintAction::do_action(canvas, data);

	Vector2i pos = data[0];

	Color col = canvas->get_pixel_v(pos);
	Color col2 = data[2];

	if (col == col2) {
		return;
	}

	PoolVector2iArray pixels = canvas->select_same_color(pos.x, pos.y);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		if (undo_cells.contains(pixel)) {
			continue;
		}

		if (canvas->is_alpha_locked() && col.a < 0.0001) {
			continue;
		}

		undo_cells.append(pixel);
		undo_colors.append(col);

		canvas->set_pixel_v(pixel, col2);

		redo_cells.append(pixel);
		redo_colors.append(col2);
	}
}

BucketAction::BucketAction() {
}

BucketAction::~BucketAction() {
}

void BucketAction::_bind_methods() {
}
