
#include "slope_point.h"

float SlopePoint::get_distance() const {
	return distance;
}

void SlopePoint::set_distance(const float val) {
	distance = val;
}

Variant SlopePoint::get_moving() {
	return moving;
}

void SlopePoint::set_moving(const bool val) {
	moving = val;
}

void SlopePoint::_on_ControlPoint_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventMouseButton> iemb = event;
	Ref<InputEventMouseMotion> iemm = event;

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT) {
			if (iemb->is_pressed()) {
				if (iemb->is_doubleclick()) {
					Control *parent = get_parent_control();
					Vector2 vector;

					if (get_index() == 0) {
						vector = parent->get_position() - Object::cast_to<Control>(parent->get_parent()->get_child(parent->get_index() - 1))->get_position();
					} else {
						vector = Object::cast_to<Control>(parent->get_parent()->get_child(parent->get_index() + 1))->get_position() - parent->get_position();
					}

					vector = distance * vector.normalized();
					set_position(vector - OFFSET);

					if (iemb->get_control()) {
						Object::cast_to<Control>(get_parent()->get_child(1 - get_index()))->get_position() = -vector - OFFSET;
					}

					get_parent()->call("update_tangents");
				} else {
					moving = true;
				}
			} else {
				moving = false;
			}
		}
	} else if (moving && iemm.is_valid()) {
		Vector2 vector = get_global_mouse_position() - get_parent_control()->get_global_rect().position + OFFSET;
		vector *= SGN(vector.x);
		vector = distance * vector.normalized();
		set_position(vector - OFFSET);

		if (iemm->get_control()) {
			Object::cast_to<Control>(get_parent()->get_child(1 - get_index()))->get_position() = -vector - OFFSET;
		}

		get_parent()->call("update_tangents");
	}
}

SlopePoint::SlopePoint() {
	distance = 0;
	moving = false;
}

SlopePoint::~SlopePoint() {
}

void SlopePoint::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("gui_input", this, "_on_ControlPoint_gui_input");
	} else if (p_what == NOTIFICATION_DRAW) {
		//	var current_theme : Theme = get_node("/root/MainWindow").theme;
		//	var color : Color = current_theme.get_color("font_color", "Label");
		Color color = Color(1, 1, 1, 1);
		draw_circle(Vector2(3.0, 3.0), 3.0, color);
	}
}

void SlopePoint::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_distance"), &SlopePoint::get_distance);
	ClassDB::bind_method(D_METHOD("set_distance", "value"), &SlopePoint::set_distance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "distance"), "set_distance", "get_distance");

	ClassDB::bind_method(D_METHOD("get_moving"), &SlopePoint::get_moving);
	ClassDB::bind_method(D_METHOD("set_moving", "value"), &SlopePoint::set_moving);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &SlopePoint::_on_ControlPoint_gui_input);
}
