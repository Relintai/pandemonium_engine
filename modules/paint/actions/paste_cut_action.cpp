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

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"
#include "../paint_utilities.h"

//data[2] = selection_pos
//data[3] = selection_color
//data[4] = cut pos
//data[5] = cut size
void PasteCutAction::do_action(PaintCanvas *canvas, const Array &data) {
	PaintAction::do_action(canvas, data);

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);
	Vector2i cut_pos = data[4];
	Vector2i cut_size = data[5];

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel_pos = pixels[i];

		PoolVector2iArray cells = data[2];
		PoolColorArray colors = data[3];

		for (int idx = 0; idx < cells.size(); ++idx) {
			Vector2i pixel = cells[idx];
			Color color = colors[idx];
			pixel -= cut_pos + cut_size / 2;
			pixel += pixel_pos;

			if (canvas->validate_pixel_v(pixel)) {
				continue;
			}

			Color col = canvas->get_pixel_v(pixel);

			if (canvas->is_alpha_locked() && col.a < 0.0001) {
				continue;
			}

			int found = redo_cells.find(pixel);
			if (found == -1) {
				redo_cells.push_back(pixel);
				redo_colors.push_back(color);
			} else {
				redo_colors[found] = color;
			}

			found = undo_cells.find(pixel);

			if (found == -1) {
				undo_colors.append(col);
				undo_cells.append(pixel);
			}

			canvas->set_pixel_v(pixel, color);
		}
	}
}

PasteCutAction::PasteCutAction() {
}

PasteCutAction::~PasteCutAction() {
}

void PasteCutAction::_bind_methods() {
}
