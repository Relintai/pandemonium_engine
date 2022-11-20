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

#include "rect_action.h"

#include "../paint_utilities.h"

#include "../nodes/paint_canvas.h"

Vector2i RectAction::get_mouse_start_pos() {
	return mouse_start_pos;
}
void RectAction::set_mouse_start_pos(const Vector2i &val) {
	mouse_start_pos = val;
}

bool RectAction::get_mouse_start_pos_set() {
	return mouse_start_pos_set;
}
void RectAction::set_mouse_start_pos_set(const bool val) {
	mouse_start_pos_set = val;
}

void RectAction::_do_action(const Array &data) {
	if (!mouse_start_pos_set) {
		mouse_start_pos = data[0];
		//print("init:", mouse_start_pos)
		mouse_start_pos_set = true;
	}

	undo_cells.clear();
	undo_colors.clear();
	preview_cells.clear();
	preview_colors.clear();

	_paint_canvas->clear_preview();

	Vector2i p = mouse_start_pos;
	Vector2i current_mouse_pos = data[0];
	Vector2i s = current_mouse_pos - mouse_start_pos;

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(p, p + Vector2i(s.x, 0));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p, p + Vector2(0, s.y)));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p + s, p + s + Vector2(0, -s.y)));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p + s, p + s + Vector2(-s.x, 0)));

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		if (!_paint_canvas->validate_pixel_v(pixel)) {
			continue;
		}

		Color col = _paint_canvas->get_pixel_v(pixel);

		if (_paint_canvas->get_alpha_locked() && col.a < 0.00001) {
			continue;
		}

		Color tc = data[2];

		_paint_canvas->set_preview_pixel_v(pixel, tc);
		undo_cells.append(pixel);
		undo_colors.append(col);
		preview_cells.append(pixel);
		preview_colors.append(tc);
	}
}
void RectAction::_commit_action() {
	_paint_canvas->clear_preview();

	for (int idx = 0; idx < preview_cells.size(); ++idx) {
		Vector2i pcell = preview_cells[idx];
		Color pcolor = preview_colors[idx];

		_paint_canvas->set_pixel_v(pcell, pcolor);

		redo_cells.append(pcell);
		redo_colors.append(pcolor);
	}

	mouse_start_pos_set = false;
}

bool RectAction::_can_commit() {
	return true;
}

RectAction::RectAction() {
	mouse_start_pos_set = false;
}

RectAction::~RectAction() {
}

void RectAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mouse_start_pos"), &RectAction::get_mouse_start_pos);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos", "value"), &RectAction::set_mouse_start_pos);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "mouse_start_pos"), "set_mouse_start_pos", "get_mouse_start_pos");

	ClassDB::bind_method(D_METHOD("get_mouse_start_pos_set"), &RectAction::get_mouse_start_pos_set);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos_set", "value"), &RectAction::set_mouse_start_pos_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mouse_start_pos_set"), "set_mouse_start_pos_set", "get_mouse_start_pos_set");
}
