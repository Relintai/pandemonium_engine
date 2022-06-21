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

#include "line_action.h"

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"
#include "../paint_utilities.h"

void LineAction::do_action(PaintCanvas *canvas, const Array &data) {
	PaintAction::do_action(canvas, data);

	if (!mouse_start_pos_set) {
		mouse_start_pos = data[0];
		mouse_start_pos_set = true;
	}

	preview_cells.resize(0);
	preview_colors.resize(0);
	canvas->clear_preview_layer();

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], mouse_start_pos);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		Color col = canvas->get_pixel_v(pixel);

		if (canvas->is_alpha_locked() && col.a < 0.00001) {
			continue;
		}

		Color nc = data[2];

		canvas->set_preview_pixel_v(pixel, nc);

		preview_cells.append(pixel);
		preview_colors.append(nc);
	}
}

void LineAction::commit_action(PaintCanvas *canvas) {
	canvas->clear_preview_layer();

	for (int i = 0; i < preview_cells.size(); ++i) {
		Vector2i pc = preview_cells[i];

		if (!canvas->validate_pixel_v(pc)) {
			continue;
		}

		Color pcol = preview_colors[i];

		undo_cells.append(pc);
		undo_colors.append(canvas->get_pixel_v(pc));

		canvas->set_pixel_v(pc, pcol);

		redo_cells.append(pc);
		redo_colors.append(pcol);
	}

	mouse_start_pos_set = false;
}

bool LineAction::can_commit() {
	return true;
}

LineAction::LineAction() {
	mouse_start_pos_set = false;
}

LineAction::~LineAction() {
}

void LineAction::_bind_methods() {
}
