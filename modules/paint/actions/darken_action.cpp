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

#include "darken_action.h"

#include "../deprecated/paint_canvas.h"
#include "../deprecated/paint_canvas_layer.h"
#include "../paint_utilities.h"

float DarkenAction::get_dark_factor() {
	return dark_factor;
}
void DarkenAction::set_dark_factor(const float val) {
	dark_factor = val;
}

void DarkenAction::do_action(PaintCanvas *canvas, const Array &data) {
	PaintAction::do_action(canvas, data);

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], data[1]);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		if (!canvas->validate_pixel_v(pixel)) {
			continue;
		}

		Color col = canvas->get_pixel_v(pixel);

		if (canvas->is_alpha_locked() && col.a < 0.001) {
			continue;
		}

		Color darkened_color = col.darkened(dark_factor);

		canvas->set_pixel_v(pixel, darkened_color);

		redo_cells.append(pixel);
		redo_colors.append(darkened_color);

		if (undo_cells.contains(pixel)) {
			continue;
		}

		undo_colors.append(col);
		undo_cells.append(pixel);
	}
}

DarkenAction::DarkenAction() {
	dark_factor = 0.1;
}

DarkenAction::~DarkenAction() {
}

void DarkenAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_dark_factor"), &DarkenAction::get_dark_factor);
	ClassDB::bind_method(D_METHOD("set_dark_factor", "value"), &DarkenAction::set_dark_factor);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dark_factor"), "set_dark_factor", "get_dark_factor");
}
