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

#include "../bush_prefabs.h"
#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"
#include "../paint_utilities.h"

void BrushAction::do_action(PaintCanvas *canvas, const Array &data) {
	PaintAction::do_action(canvas, data);

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);
	int brush_type = data[3];
	PoolVector2iArray brush = BrushPrefabs::get_brush(static_cast<BrushPrefabs::Type>(brush_type), data[4]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		for (int j = 0; j < brush.size(); ++j) {
			Vector2i off = brush[j];
			Vector2i p = pixel + off;

			if (undo_cells.contains(p)) {
				continue;
			}

			Color col = canvas->get_pixel_v(p);

			if (canvas->is_alpha_locked() && col.a < 0.00001) {
				continue;
			}

			undo_colors.append(col);
			undo_cells.append(p);

			canvas->set_pixel_v(pixel, data[2]);

			redo_cells.append(p);
			redo_colors.append(data[2]);
		}
	}
}

BrushAction::BrushAction() {
}

BrushAction::~BrushAction() {
}

void BrushAction::_bind_methods() {
}
