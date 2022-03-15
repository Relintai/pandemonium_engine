/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "touch_button.h"

#include "core/version.h"

void TouchButton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_process_input(is_visible_in_tree());
			}

		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (Engine::get_singleton()->is_editor_hint())
				break;
			if (is_visible_in_tree()) {
				set_process_input(true);
			} else {
				set_process_input(false);
			}
		} break;
	}

	Button::_notification(p_what);
}

void TouchButton::_input(Ref<InputEvent> p_event) {
	if (!get_tree())
		return;

	if (p_event->get_device() == -1)
		return;

	ERR_FAIL_COND(!is_visible_in_tree());

	if (is_disabled()) // no interaction with disabled button
		return;

	Ref<InputEventScreenTouch> b = p_event;

	if (b.is_valid() && _is_point_inside(b->get_position())) {
		get_viewport()->set_input_as_handled();

		Ref<InputEventMouseButton> ev;
#if VERSION_MAJOR < 4
		ev.instance();
#else
		ev.instantiate();
#endif

#if VERSION_MAJOR < 4
		ev->set_button_index(BUTTON_LEFT);
#else
		ev->set_button_index(MouseButton::LEFT);
#endif
		ev->set_pressed(b->is_pressed());
		ev->set_position(b->get_position());
		ev->set_global_position(b->get_position());

#if VERSION_MAJOR <= 3
		BaseButton::_gui_input(ev);
#endif
		update();
	}
	/*
	Ref<InputEventScreenDrag> mm = p_event;

	if (mm.is_valid() && _is_point_inside(mm->get_position())) {
		Ref<InputEventMouseMotion> ev;
		ev.instance();

		ev->set_relative(mm->get_relative());
		ev->set_speed(mm->get_speed());
		ev->set_position(b->get_position());
		ev->set_global_position(b->get_position());

		BaseButton::_gui_input(ev);
		update();
	}*/
}

void TouchButton::_gui_input(Ref<InputEvent> p_event) {
	if (p_event->get_device() == -1) {
		accept_event();
		return;
	}

#if VERSION_MAJOR <= 3
	BaseButton::_gui_input(p_event);
#endif
}

void TouchButton::_unhandled_input(Ref<InputEvent> p_event) {
	if (p_event->get_device() == -1) {
		//accept_event();
		return;
	}

#if VERSION_MAJOR <= 3
	Button::_unhandled_input(p_event);
#endif
}

void TouchButton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_input"), &TouchButton::_input);
}

bool TouchButton::_is_point_inside(const Vector2 &vec) {
	Point2 point;
	point.x = vec.x;
	point.y = vec.y;

	return get_global_rect().has_point(point);
}
