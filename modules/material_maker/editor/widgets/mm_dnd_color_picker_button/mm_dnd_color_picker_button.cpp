/*************************************************************************/
/*  mm_dnd_color_picker_button.cpp                                       */
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

#include "mm_dnd_color_picker_button.h"

#include "scene/gui/color_rect.h"

Variant MMDNDColorPickerButton::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	ColorRect *preview = memnew(ColorRect);
	preview->set_frame_color(get_pick_color());
	preview->set_custom_minimum_size(Vector2(32, 32));
	set_drag_preview(preview);
	return get_pick_color();
}

bool MMDNDColorPickerButton::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	return p_data.get_type() == Variant::COLOR;
}

void MMDNDColorPickerButton::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	set_pick_color(p_data);
	emit_signal("color_changed", get_pick_color());
}

MMDNDColorPickerButton::MMDNDColorPickerButton() {
}

MMDNDColorPickerButton::~MMDNDColorPickerButton() {
}

void MMDNDColorPickerButton::_bind_methods() {
	ADD_SIGNAL(MethodInfo("color_changed", PropertyInfo(Variant::COLOR, "color")));

	ClassDB::bind_method(D_METHOD("get_drag_data_fw", "point", "from"), &MMDNDColorPickerButton::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw", "point", "data", "from"), &MMDNDColorPickerButton::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw", "point", "data", "from"), &MMDNDColorPickerButton::drop_data_fw);
}
