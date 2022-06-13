
#include "control_point.h"

bool ControlPoint::get_moving() const {
	return moving;
}

void ControlPoint::set_moving(const bool val) {
	moving = val;
}

void ControlPoint::_draw() {
	//	var current_theme : Theme = get_node("/root/MainWindow").theme;
	//	var color : Color = current_theme.get_color("font_color", "Label");
	Color color = Color(1, 1, 1, 1);
	draw_rect(Rect2(0, 0, 7, 7), color);
}

void ControlPoint::initialize(const Vector2 &p) {
	set_rect_position(get_parent().transform_point(p) - OFFSET);
}

void ControlPoint::_on_ControlPoint_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventMouseButton> iemb = event;
	Ref<InputEventMouseMotion> iemm = event;

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT) {
			if (iemb->is_pressed()) {
				moving = true;
			} else {
				moving = false;
				get_parent().update_controls();
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

ControlPoint::ControlPoint() {
	moving = false;

	set_custom_minimum_size(Vector2(7, 7));
}

ControlPoint::~ControlPoint() {
}

void ControlPoint::_bind_methods() {
	ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::INT, "index")));
	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::INT, "index")));

	ClassDB::bind_method(D_METHOD("get_moving"), &ControlPoint::get_moving);
	ClassDB::bind_method(D_METHOD("set_moving", "value"), &ControlPoint::set_moving);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");

	//ClassDB::bind_method(D_METHOD("_draw"), &ControlPoint::_draw);
	ClassDB::bind_method(D_METHOD("initialize", "p"), &ControlPoint::initialize);

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &ControlPoint::_on_ControlPoint_gui_input);
}
