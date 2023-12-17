/*************************************************************************/
/*  curve_edit.cpp                                                       */
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

#include "curve_edit.h"

#include "curve_dialog.h"
#include "curve_view.h"

#include "../../../nodes/bases/curve_base.h"

Ref<CurveBase> CurveEdit::get_value() {
	return value;
}

void CurveEdit::set_value(const Ref<CurveBase> &v) {
	value = v;
	_curve_view->set_curve(value);
}

void CurveEdit::_on_CurveEdit_pressed() {
	CurveDialog *dialog = memnew(CurveDialog);
	add_child(dialog);
	dialog->connect("curve_changed", this, "on_value_changed");
	dialog->edit_curve(value);
}

void CurveEdit::on_value_changed(const Ref<CurveBase> &v) {
	//set_value(v);
	emit_signal("updated", v);
	_curve_view->update();
}

CurveEdit::CurveEdit() {
	set_custom_minimum_size(Vector2(60, 20));
	set_focus_mode(FOCUS_CLICK);

	_curve_view = memnew(CurveView);
	add_child(_curve_view);
	_curve_view->set_mouse_filter(MOUSE_FILTER_IGNORE);
}

CurveEdit::~CurveEdit() {
}

void CurveEdit::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("pressed", this, "_on_CurveEdit_pressed");
		} break;
		default: {
		} break;
	}
}

void CurveEdit::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase")));

	ClassDB::bind_method(D_METHOD("get_value"), &CurveEdit::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &CurveEdit::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("_on_CurveEdit_pressed"), &CurveEdit::_on_CurveEdit_pressed);
	ClassDB::bind_method(D_METHOD("on_value_changed", "v"), &CurveEdit::on_value_changed);
}
