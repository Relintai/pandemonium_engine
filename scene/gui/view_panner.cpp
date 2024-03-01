/*************************************************************************/
/*  view_panner.cpp                                                      */
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

#include "view_panner.h"

#include "core/input/input.h"
#include "core/input/input_event.h"
#include "core/input/shortcut.h"
#include "core/os/keyboard.h"

ViewPanner::ControlScheme ViewPanner::get_control_scheme() const {
	return control_scheme;
}

void ViewPanner::set_control_scheme(const ControlScheme p_scheme) {
	control_scheme = p_scheme;
}

bool ViewPanner::get_enable_rmb() const {
	return enable_rmb;
}

void ViewPanner::set_enable_rmb(const bool p_enable) {
	enable_rmb = p_enable;
}

Ref<ShortCut> ViewPanner::get_pan_shortcut() const {
	return pan_view_shortcut;
}

void ViewPanner::set_pan_shortcut(const Ref<ShortCut> &p_shortcut) {
	pan_view_shortcut = p_shortcut;
	pan_key_pressed = false;
}

bool ViewPanner::get_simple_panning_enabled() const {
	return simple_panning_enabled;
}

void ViewPanner::set_simple_panning_enabled(const bool p_enabled) {
	simple_panning_enabled = p_enabled;
}

int ViewPanner::get_scroll_speed() const {
	return scroll_speed;
}

void ViewPanner::set_scroll_speed(const int p_scroll_speed) {
	ERR_FAIL_COND(p_scroll_speed <= 0);
	scroll_speed = p_scroll_speed;
}

float ViewPanner::get_scroll_zoom_factor() const {
	return scroll_zoom_factor;
}

void ViewPanner::set_scroll_zoom_factor(const float p_scroll_zoom_factor) {
	ERR_FAIL_COND(p_scroll_zoom_factor <= 1.0);
	scroll_zoom_factor = p_scroll_zoom_factor;
}

ViewPanner::PanAxis ViewPanner::get_pan_axis() const {
	return pan_axis;
}

void ViewPanner::set_pan_axis(PanAxis p_pan_axis) {
	pan_axis = p_pan_axis;
}

void ViewPanner::setup(const ControlScheme p_scheme, const Ref<ShortCut> &p_shortcut, const bool p_simple_panning) {
	set_control_scheme(p_scheme);
	set_pan_shortcut(p_shortcut);
	set_simple_panning_enabled(p_simple_panning);
}

bool ViewPanner::is_panning() const {
	return is_dragging || pan_key_pressed;
}

void ViewPanner::set_force_drag(bool p_force) {
	force_drag = p_force;
}

bool ViewPanner::gui_input(const Ref<InputEvent> &p_event, Rect2 p_canvas_rect) {
	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		Vector2 scroll_vec = Vector2((mb->get_button_index() == BUTTON_WHEEL_RIGHT) - (mb->get_button_index() == BUTTON_WHEEL_LEFT), (mb->get_button_index() == BUTTON_WHEEL_DOWN) - (mb->get_button_index() == BUTTON_WHEEL_UP));
		// Moving the scroll wheel sends two events: one with pressed as true,
		// and one with pressed as false. Make sure we only process one of them.
		if (scroll_vec != Vector2() && mb->is_pressed()) {
			if (control_scheme == SCROLL_PANS) {
				if (mb->get_control()) {
					if (scroll_vec.y != 0) {
						// Compute the zoom factor.
						float zoom_factor = mb->get_factor() <= 0 ? 1.0 : mb->get_factor();
						zoom_factor = ((scroll_zoom_factor - 1.0) * zoom_factor) + 1.0;
						float zoom = scroll_vec.y > 0 ? 1.0 / scroll_zoom_factor : scroll_zoom_factor;
						emit_signal("zoomed", zoom, mb->get_position(), p_event);
						return true;
					}
				} else {
					Vector2 panning = scroll_vec * mb->get_factor();
					if (pan_axis == PAN_AXIS_HORIZONTAL) {
						panning = Vector2(panning.x + panning.y, 0);
					} else if (pan_axis == PAN_AXIS_VERTICAL) {
						panning = Vector2(0, panning.x + panning.y);
					} else if (mb->get_shift()) {
						panning = Vector2(panning.y, panning.x);
					}
					emit_signal("panned", -panning * scroll_speed, p_event);
					return true;
				}
			} else {
				if (mb->get_control()) {
					Vector2 panning = scroll_vec * mb->get_factor();
					if (pan_axis == PAN_AXIS_HORIZONTAL) {
						panning = Vector2(panning.x + panning.y, 0);
					} else if (pan_axis == PAN_AXIS_VERTICAL) {
						panning = Vector2(0, panning.x + panning.y);
					} else if (mb->get_shift()) {
						panning = Vector2(panning.y, panning.x);
					}
					emit_signal("panned", -panning * scroll_speed, p_event);
					return true;
				} else if (!mb->get_shift() && scroll_vec.y != 0) {
					// Compute the zoom factor.
					float zoom_factor = mb->get_factor() <= 0 ? 1.0 : mb->get_factor();
					zoom_factor = ((scroll_zoom_factor - 1.0) * zoom_factor) + 1.0;
					float zoom = scroll_vec.y > 0 ? 1.0 / scroll_zoom_factor : scroll_zoom_factor;
					emit_signal("zoomed", zoom, mb->get_position(), p_event);
					return true;
				}
			}
		}

		// Alt is not used for button presses, so ignore it.
		if (mb->get_alt()) {
			return false;
		}

		bool is_drag_event = mb->get_button_index() == BUTTON_MIDDLE ||
				(enable_rmb && mb->get_button_index() == BUTTON_RIGHT) ||
				(!simple_panning_enabled && mb->get_button_index() == BUTTON_LEFT && is_panning()) ||
				(force_drag && mb->get_button_index() == BUTTON_LEFT);

		if (is_drag_event) {
			if (mb->is_pressed()) {
				is_dragging = true;
			} else {
				is_dragging = false;
			}
			return mb->get_button_index() != BUTTON_LEFT || mb->is_pressed(); // Don't consume LMB release events (it fixes some selection problems).
		}
	}

	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (is_dragging) {
			if (p_canvas_rect != Rect2()) {
				emit_signal("panned", Input::get_singleton()->warp_mouse_motion(mm, p_canvas_rect), p_event);
			} else {
				emit_signal("panned", mm->get_relative(), p_event);
			}
			return true;
		}
	}

	Ref<InputEventMagnifyGesture> magnify_gesture = p_event;
	if (magnify_gesture.is_valid()) {
		// Zoom gesture
		emit_signal("zoomed", magnify_gesture->get_factor(), magnify_gesture->get_position(), p_event);
		return true;
	}

	Ref<InputEventPanGesture> pan_gesture = p_event;
	if (pan_gesture.is_valid()) {
		if (pan_gesture->get_control()) {
			// Zoom gesture.
			float pan_zoom_factor = 1.02f;
			float zoom_direction = pan_gesture->get_delta().x - pan_gesture->get_delta().y;
			if (zoom_direction == 0.f) {
				return true;
			}
			float zoom = zoom_direction < 0 ? 1.0 / pan_zoom_factor : pan_zoom_factor;
			emit_signal("zoomed", zoom, pan_gesture->get_position(), p_event);
			return true;
		}
		emit_signal("panned", -pan_gesture->get_delta() * scroll_speed, p_event);
	}

	Ref<InputEventScreenDrag> screen_drag = p_event;
	if (screen_drag.is_valid()) {
		if (Input::get_singleton()->is_emulating_mouse_from_touch() || Input::get_singleton()->is_emulating_touch_from_mouse()) {
			// This set of events also generates/is generated by
			// InputEventMouseButton/InputEventMouseMotion events which will be processed instead.
		} else {
			emit_signal("panned", screen_drag->get_relative(), p_event);
		}
	}

	Ref<InputEventKey> k = p_event;
	if (k.is_valid()) {
		if (pan_view_shortcut.is_valid() && pan_view_shortcut->is_shortcut(k)) {
			pan_key_pressed = k->is_pressed();
			if (simple_panning_enabled || (Input::get_singleton()->get_mouse_button_mask() & BUTTON_MASK_LEFT)) {
				is_dragging = pan_key_pressed;
			}
			return true;
		}
	}

	return false;
}

void ViewPanner::release_pan_key() {
	pan_key_pressed = false;
	is_dragging = false;
}

ViewPanner::ViewPanner() {
	pan_view_shortcut.instance();
	pan_view_shortcut->set_shortcut(InputEventKey::create_reference(KEY_SPACE));

	scroll_speed = 32;
	scroll_zoom_factor = 1.1;
	pan_axis = PAN_AXIS_BOTH;

	is_dragging = false;
	pan_key_pressed = false;
	force_drag = false;

	enable_rmb = false;
	simple_panning_enabled = false;

	control_scheme = SCROLL_ZOOMS;
}

void ViewPanner::_bind_methods() {
	ADD_SIGNAL(MethodInfo("panned",
			PropertyInfo(Variant::VECTOR2, "position"),
			PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));

	ADD_SIGNAL(MethodInfo("zoomed",
			PropertyInfo(Variant::REAL, "amount"),
			PropertyInfo(Variant::VECTOR2, "position"),
			PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));

	ClassDB::bind_method(D_METHOD("get_control_scheme"), &ViewPanner::get_control_scheme);
	ClassDB::bind_method(D_METHOD("set_control_scheme", "scheme"), &ViewPanner::set_control_scheme);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "control_scheme", PROPERTY_HINT_ENUM, "Scroll Zooms,Scroll Pans"), "set_control_scheme", "get_control_scheme");

	ClassDB::bind_method(D_METHOD("get_enable_rmb"), &ViewPanner::get_enable_rmb);
	ClassDB::bind_method(D_METHOD("set_enable_rmb", "enable"), &ViewPanner::set_enable_rmb);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_rmb"), "set_enable_rmb", "get_enable_rmb");

	ClassDB::bind_method(D_METHOD("get_pan_shortcut"), &ViewPanner::get_pan_shortcut);
	ClassDB::bind_method(D_METHOD("set_pan_shortcut", "shortcut"), &ViewPanner::set_pan_shortcut);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pan_shortcut", PROPERTY_HINT_RESOURCE_TYPE, "ShortCut"), "set_pan_shortcut", "get_pan_shortcut");

	ClassDB::bind_method(D_METHOD("get_simple_panning_enabled"), &ViewPanner::get_simple_panning_enabled);
	ClassDB::bind_method(D_METHOD("set_simple_panning_enabled", "enabled"), &ViewPanner::set_simple_panning_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "simple_panning_enabled"), "set_simple_panning_enabled", "get_simple_panning_enabled");

	ClassDB::bind_method(D_METHOD("get_scroll_speed"), &ViewPanner::get_scroll_speed);
	ClassDB::bind_method(D_METHOD("set_scroll_speed", "scroll_speed"), &ViewPanner::set_scroll_speed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "scroll_speed"), "set_scroll_speed", "get_scroll_speed");

	ClassDB::bind_method(D_METHOD("get_scroll_zoom_factor"), &ViewPanner::get_scroll_zoom_factor);
	ClassDB::bind_method(D_METHOD("set_scroll_zoom_factor", "scroll_zoom_factor"), &ViewPanner::set_scroll_zoom_factor);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "scroll_zoom_factor"), "set_scroll_zoom_factor", "get_scroll_zoom_factor");

	ClassDB::bind_method(D_METHOD("get_pan_axis"), &ViewPanner::get_pan_axis);
	ClassDB::bind_method(D_METHOD("set_pan_axis", "pan_axis"), &ViewPanner::set_pan_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "pan_axis", PROPERTY_HINT_ENUM, "Both,Horizontal,Vertical"), "set_pan_axis", "get_pan_axis");

	ClassDB::bind_method(D_METHOD("setup", "scheme", "shortcut", "simple_panning"), &ViewPanner::setup);

	ClassDB::bind_method(D_METHOD("is_panning"), &ViewPanner::is_panning);
	ClassDB::bind_method(D_METHOD("set_force_drag", "force"), &ViewPanner::set_force_drag);

	ClassDB::bind_method(D_METHOD("gui_input", "ev", "canvas_rect"), &ViewPanner::gui_input, Rect2());
	ClassDB::bind_method(D_METHOD("release_pan_key"), &ViewPanner::release_pan_key);

	BIND_ENUM_CONSTANT(SCROLL_ZOOMS);
	BIND_ENUM_CONSTANT(SCROLL_PANS);

	BIND_ENUM_CONSTANT(PAN_AXIS_BOTH);
	BIND_ENUM_CONSTANT(PAN_AXIS_HORIZONTAL);
	BIND_ENUM_CONSTANT(PAN_AXIS_VERTICAL);
}
