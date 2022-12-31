/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022-2023 Péter Magyar

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

#include "../paint_utilities.h"

#include "../nodes/paint_canvas.h"

Vector2i LineAction::get_mouse_start_pos() {
	return mouse_start_pos;
}
void LineAction::set_mouse_start_pos(const Vector2i &val) {
	mouse_start_pos = val;
}

bool LineAction::get_mouse_start_pos_set() {
	return mouse_start_pos_set;
}
void LineAction::set_mouse_start_pos_set(const bool val) {
	mouse_start_pos_set = val;
}

void LineAction::_do_action(const Array &data) {
	if (!mouse_start_pos_set) {
		mouse_start_pos = data[0];
		mouse_start_pos_set = true;
	}

	preview_cells.resize(0);
	preview_colors.resize(0);
	_paint_canvas->clear_preview();

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(data[0], mouse_start_pos);

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		Color col = _paint_canvas->get_pixel_v(pixel);

		if (_paint_canvas->get_alpha_locked() && col.a < 0.00001) {
			continue;
		}

		Color nc = data[2];

		_paint_canvas->set_preview_pixel_v(pixel, nc);

		preview_cells.append(pixel);
		preview_colors.append(nc);
	}
}

void LineAction::_commit_action() {
	_paint_canvas->clear_preview();

	for (int i = 0; i < preview_cells.size(); ++i) {
		Vector2i pc = preview_cells[i];

		if (!_paint_canvas->validate_pixel_v(pc)) {
			continue;
		}

		Color pcol = preview_colors[i];

		undo_cells.append(pc);
		undo_colors.append(_paint_canvas->get_pixel_v(pc));

		_paint_canvas->set_pixel_v(pc, pcol);

		redo_cells.append(pc);
		redo_colors.append(pcol);
	}

	mouse_start_pos_set = false;
}

bool LineAction::_can_commit() {
	return true;
}

LineAction::LineAction() {
	mouse_start_pos_set = false;
}

LineAction::~LineAction() {
}

void LineAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mouse_start_pos"), &LineAction::get_mouse_start_pos);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos", "value"), &LineAction::set_mouse_start_pos);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "mouse_start_pos"), "set_mouse_start_pos", "get_mouse_start_pos");

	ClassDB::bind_method(D_METHOD("get_mouse_start_pos_set"), &LineAction::get_mouse_start_pos_set);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos_set", "value"), &LineAction::set_mouse_start_pos_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mouse_start_pos_set"), "set_mouse_start_pos_set", "get_mouse_start_pos_set");
}
