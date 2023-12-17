

#include "touch_button.h"

void TouchButton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_process_input(is_visible_in_tree());
			}

		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (Engine::get_singleton()->is_editor_hint()) {
				break;
			}

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
	if (!get_tree()) {
		return;
	}

	if (p_event->get_device() == -1) {
		return;
	}

	ERR_FAIL_COND(!is_visible_in_tree());

	if (is_disabled()) { // no interaction with disabled button
		return;
	}

	Ref<InputEventScreenTouch> b = p_event;

	if (b.is_valid() && _is_point_inside(b->get_position())) {
		get_viewport()->set_input_as_handled();

		Ref<InputEventMouseButton> ev;
		ev.instance();

		ev->set_button_index(BUTTON_LEFT);
		ev->set_pressed(b->is_pressed());
		ev->set_position(b->get_position());
		ev->set_global_position(b->get_position());

		BaseButton::_gui_input(ev);
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

	BaseButton::_gui_input(p_event);
}

void TouchButton::_shortcut_input(Ref<InputEvent> p_event) {
	if (p_event->get_device() == -1) {
		//accept_event();
		return;
	}

	Button::_shortcut_input(p_event);
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
