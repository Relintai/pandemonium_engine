/*************************************************************************/
/*  polygon_control_point.cpp                                            */
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

#include "polygon_control_point.h"

#include "polygon_editor.h"

bool PolygonControlPoint::get_moving() const {
	return moving;
}

void PolygonControlPoint::set_moving(const bool val) {
	moving = val;
}

void PolygonControlPoint::_draw() {
	//	var current_theme : Theme = get_node("/root/MainWindow").theme;
	//	var color : Color = current_theme.get_theme_color("font_color", "Label");
	Color color = Color(1, 1, 1, 1);
	draw_rect(Rect2(0, 0, 7, 7), color);
}

void PolygonControlPoint::initialize(const Vector2 &p) {
	set_position(polygon_editor->transform_point(p) - OFFSET);
}

void PolygonControlPoint::_on_ControlPoint_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventMouseButton> iemb = event;
	Ref<InputEventMouseMotion> iemm = event;

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT) {
			if (iemb->is_pressed()) {
				moving = true;
			} else {
				moving = false;
				polygon_editor->update_controls();
			}
		} else if (iemb->get_button_index() == BUTTON_RIGHT && event->is_pressed()) {
			emit_signal("removed", get_index());
		}
	} else if (moving && iemm.is_valid()) {
		Vector2 rect_position = get_position();
		rect_position += iemm->get_relative();
		set_position(rect_position);
		emit_signal("moved", get_index());
	}
}

PolygonControlPoint::PolygonControlPoint() {
	polygon_editor = nullptr;
	moving = false;

	set_custom_minimum_size(Vector2(7, 7));
}

PolygonControlPoint::~PolygonControlPoint() {
}

void PolygonControlPoint::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("gui_input", this, "_on_ControlPoint_gui_input");
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
		default: {
		} break;
	}
}

void PolygonControlPoint::_bind_methods() {
	ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::INT, "index")));
	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::INT, "index")));

	ClassDB::bind_method(D_METHOD("get_moving"), &PolygonControlPoint::get_moving);
	ClassDB::bind_method(D_METHOD("set_moving", "value"), &PolygonControlPoint::set_moving);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");

	//ClassDB::bind_method(D_METHOD("_draw"), &PolygonControlPoint::_draw);
	ClassDB::bind_method(D_METHOD("initialize", "p"), &PolygonControlPoint::initialize);

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &PolygonControlPoint::_on_ControlPoint_gui_input);
}
