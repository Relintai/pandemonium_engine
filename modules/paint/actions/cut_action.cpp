/*************************************************************************/
/*  cut_action.cpp                                                       */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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

#include "cut_action.h"

#include "../paint_utilities.h"

#include "../nodes/paint_canvas.h"

Color CutAction::get_selection_color() {
	return selection_color;
}
void CutAction::set_selection_color(const Color &val) {
	selection_color = val;
}

Vector2i CutAction::get_mouse_start_pos() {
	return mouse_start_pos;
}
void CutAction::set_mouse_start_pos(const Vector2i &val) {
	mouse_start_pos = val;
}

Vector2i CutAction::get_mouse_end_pos() {
	return mouse_end_pos;
}
void CutAction::set_mouse_end_pos(const Vector2i &val) {
	mouse_end_pos = val;
}

bool CutAction::get_mouse_start_pos_set() {
	return mouse_start_pos_set;
}
void CutAction::set_mouse_start_pos_set(const bool val) {
	mouse_start_pos_set = val;
}

bool CutAction::_can_commit() {
	return false; //ugly way of handling a cut
}

void CutAction::_do_action(const Array &data) {
	if (!mouse_start_pos_set) {
		mouse_start_pos = data[0];
		mouse_start_pos_set = true;
	}

	mouse_end_pos = data[0];

	preview_cells.clear();
	preview_colors.clear();
	_paint_canvas->clear_preview();

	Vector2i p = mouse_start_pos;
	Vector2i s = mouse_end_pos - mouse_start_pos;

	PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(p, p + Vector2i(s.x, 0));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p, p + Vector2i(0, s.y)));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p + s, p + s + Vector2i(0, -s.y)));
	pixels.append_array(PaintUtilities::get_pixels_in_line(p + s, p + s + Vector2i(-s.x, 0)));

	for (int i = 0; i < pixels.size(); ++i) {
		Vector2i pixel = pixels[i];

		_paint_canvas->set_preview_pixel_v(pixel, selection_color);
		preview_cells.append(pixel);
		preview_colors.append(selection_color);
	}
}

void CutAction::_commit_action() {
	_paint_canvas->clear_preview();
	Vector2i p = mouse_start_pos;
	Vector2i s = mouse_end_pos - mouse_start_pos;

	int ex = ABS(s.x) + 1;
	int ey = ABS(s.y) + 1;

	for (int x = 0; x < ex; ++x) {
		for (int y = 0; y < ey; ++y) {
			int px = x;
			int py = y;

			if (s.x < 0) {
				px *= -1;
			}

			if (s.y < 0) {
				py *= -1;
			}

			Vector2i pos = p + Vector2i(px, py);

			if (!_paint_canvas->validate_pixel_v(pos)) {
				continue;
			}

			Color color = _paint_canvas->get_pixel_v(pos);

			if (color.a < 0.0001) {
				continue;
			}

			redo_cells.append(pos);
			redo_colors.append(color);

			_paint_canvas->set_pixel_v(pos, Color(0, 0, 0, 0));

			undo_cells.append(pos);
			undo_colors.append(Color(0, 0, 0, 0));
		}
	}
}

CutAction::CutAction() {
	selection_color = Color(0.8, 0.8, 0.8, 0.5);
	mouse_start_pos_set = false;
}

CutAction::~CutAction() {
}

void CutAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_selection_color"), &CutAction::get_selection_color);
	ClassDB::bind_method(D_METHOD("set_selection_color", "value"), &CutAction::set_selection_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "selection_color"), "set_selection_color", "get_selection_color");

	ClassDB::bind_method(D_METHOD("get_mouse_start_pos"), &CutAction::get_mouse_start_pos);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos", "value"), &CutAction::set_mouse_start_pos);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "mouse_start_pos"), "set_mouse_start_pos", "get_mouse_start_pos");

	ClassDB::bind_method(D_METHOD("get_mouse_end_pos"), &CutAction::get_mouse_end_pos);
	ClassDB::bind_method(D_METHOD("set_mouse_end_pos", "value"), &CutAction::set_mouse_end_pos);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "mouse_end_pos"), "set_mouse_end_pos", "get_mouse_end_pos");

	ClassDB::bind_method(D_METHOD("get_mouse_start_pos_set"), &CutAction::get_mouse_start_pos_set);
	ClassDB::bind_method(D_METHOD("set_mouse_start_pos_set", "value"), &CutAction::set_mouse_start_pos_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mouse_start_pos_set"), "set_mouse_start_pos_set", "get_mouse_start_pos_set");
}
