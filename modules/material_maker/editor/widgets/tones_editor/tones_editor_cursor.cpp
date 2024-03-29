/*************************************************************************/
/*  tones_editor_cursor.cpp                                              */
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

#include "tones_editor_cursor.h"

void MMTonesEditorCursor::set_value(float val) {
	_position = val;

	Control *parent = get_parent_control();

	if (!parent) {
		return;
	}

	Vector2 pos = get_position();
	set_position(Vector2(_position * parent->get_size().x - 0.5 * CURSOR_WIDTH, pos.y));
}

void MMTonesEditorCursor::update_value(float val) {
	if (!Math::is_equal_approx(val, _position)) {
		set_value(val);
		emit_signal("cursor_value_changed", this, _position);
		update();
	}
}

void MMTonesEditorCursor::initialize(const Color &color, float position, bool top) {
	_color = color;
	_position = position;
	_top = top;
}

void MMTonesEditorCursor::resize() {
	Control *parent = get_parent_control();

	if (!parent) {
		return;
	}

	Vector2 position = get_position();

	if (_top) {
		set_position(Vector2(position.x, -2));
	} else {
		set_position(Vector2(position.x, parent->get_size().y + 2 - CURSOR_HEIGHT));
	}

	set_size(Vector2(CURSOR_WIDTH, CURSOR_HEIGHT));
	set_value(_position);
}

void MMTonesEditorCursor::_gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseButton> iemb = p_event;

	if (iemb.is_valid()) {
		if (iemb->is_pressed()) {
			if (_pointer_index != -1) {
				return;
			}

			if (iemb->get_button_index() != BUTTON_LEFT) {
				return;
			}

			_pointer_index = 1;
		} else {
			if (_pointer_index != 1) {
				return;
			}

			_pointer_index = -1;

			Control *parent = get_parent_control();

			if (!parent) {
				return;
			}

			Vector2 parent_size = parent->get_size();

			Vector2 pos = get_position();

			update_value(pos.x / parent_size.x);
		}

		return;
	}

	if (_pointer_index != 1) {
		return;
	}

	Ref<InputEventMouseMotion> iemm = p_event;

	if (iemm.is_valid()) {
		Control *parent = get_parent_control();

		if (!parent) {
			return;
		}

		Vector2 parent_size = parent->get_size();

		Vector2 pos = get_position();

		pos.x += iemm->get_relative().x;
		pos.x = MIN(MAX(-0.5 * CURSOR_WIDTH, pos.x), parent_size.x - 0.5 * CURSOR_WIDTH);

		set_value((pos.x + 0.5 * CURSOR_WIDTH) / parent_size.x);
	}
}

MMTonesEditorCursor::MMTonesEditorCursor() {
	_pointer_index = -1;
	set_mouse_filter(MOUSE_FILTER_STOP);
}

MMTonesEditorCursor::~MMTonesEditorCursor() {
}

void MMTonesEditorCursor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			Vector<Point2> polygon;
			polygon.resize(4);

			if (_top) {
				polygon.write[0] = Vector2(0, 0);
				polygon.write[1] = Vector2(CURSOR_WIDTH / 2.0, CURSOR_HEIGHT);
				polygon.write[2] = Vector2(CURSOR_WIDTH, 0);
				polygon.write[3] = Vector2(0, 0);
			} else {
				polygon.write[0] = Vector2(0, CURSOR_HEIGHT);
				polygon.write[1] = Vector2(CURSOR_WIDTH / 2.0, 0);
				polygon.write[2] = Vector2(CURSOR_WIDTH, CURSOR_HEIGHT);
				polygon.write[3] = Vector2(0, CURSOR_HEIGHT);
			}

			Color c = _color;
			c.a = 1.0;
			draw_colored_polygon(polygon, c);
			float outline_color = 0;

			if (_position < 0.5) {
				outline_color = 1.0;
			}

			draw_polyline(polygon, Color(outline_color, outline_color, outline_color), 1.0, true);

		} break;
		case NOTIFICATION_RESIZED:
		case NOTIFICATION_READY: {
			resize();
		} break;
		default:
			break;
	}
}

void MMTonesEditorCursor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("cursor_value_changed", PropertyInfo(Variant::OBJECT, "cursor", PROPERTY_HINT_RESOURCE_TYPE, "Control"), PropertyInfo(Variant::REAL, "position")));

	ClassDB::bind_method(D_METHOD("_gui_input"), &MMTonesEditorCursor::_gui_input);
}
