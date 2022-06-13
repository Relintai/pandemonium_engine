
#include "control_point.h"

#include "slope_point.h"

#include "core/math/math_defs.h"

bool ControlPoint::get_moving() const {
	return moving;
}

void ControlPoint::set_moving(const bool val) {
	moving = val;
}

float ControlPoint::get_min_x() const {
	return min_x;
}

void ControlPoint::set_min_x(const float val) {
	min_x = val;
}

float ControlPoint::get_max_x() const {
	return max_x;
}

void ControlPoint::set_max_x(const float val) {
	max_x = val;
}

float ControlPoint::get_min_y() const {
	return min_y;
}

void ControlPoint::set_min_y(const float val) {
	min_y = val;
}

float ControlPoint::get_max_y() const {
	return max_y;
}

void ControlPoint::set_max_y(const float val) {
	max_y = val;
}

void ControlPoint::_draw() {
	//	var current_theme : Theme = get_node("/root/MainWindow").theme;
	//	var color : Color = current_theme.get_color("font_color", "Label");
	Color color = Color(1, 1, 1, 1);

	for (int i = 0; i < get_child_count(); ++i) {
		Control *c = Object::cast_to<Control>(get_child(i));

		if (!c) {
			continue;
		}

		if (c->is_visible()) {
			draw_line(OFFSET, c->get_position() + OFFSET, color);
		}
	}

	draw_rect(Rect2(0, 0, 7, 7), color);
}

void ControlPoint::initialize(const CurveBase::Point &p) {
	Vector2 parc = get_parent_control()->call("transform_point", p.p);
	set_position(parc - OFFSET);

	if (p.ls != Math_INF) {
		left_slope_point->set_position(left_slope_point->get_distance() * (get_parent_control()->get_size() * Vector2(1.0, -p.ls)).normalized());
	}

	if (p.rs != Math_INF) {
		right_slope_point->set_position(right_slope_point->get_distance() * (get_parent_control()->get_size() * Vector2(1.0, -p.rs)).normalized());
	}
}

void ControlPoint::set_constraint(const float x, const float X, const float y, const float Y) {
	min_x = x;
	max_x = X;
	min_y = y;
	max_y = Y;
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
				get_parent()->call("update_controls");
			}
		} else if (iemb->get_button_index() == BUTTON_RIGHT && iemb->is_pressed()) {
			emit_signal("removed", get_index());
		}
	} else if (moving && iemm.is_valid()) {
		Vector2 rect_position = get_position();

		rect_position += iemm->get_relative();

		if (rect_position.x < min_x) {
			rect_position.x = min_x;
		} else if (rect_position.x > max_x) {
			rect_position.x = max_x;
		}

		if (rect_position.y < min_y) {
			rect_position.y = min_y;
		} else if (rect_position.y > max_y) {
			rect_position.y = max_y;
		}

		set_position(rect_position);

		emit_signal("moved", get_index());
	}
}

void ControlPoint::update_tangents() {
	update();
	emit_signal("moved", get_index());
}

ControlPoint::ControlPoint() {
	moving = false;
	min_x = 0;
	max_x = 0;
	min_y = 0;
	max_y = 0;

	set_custom_minimum_size(Vector2(7, 7));

	left_slope_point = memnew(SlopePoint);
	left_slope_point->set_name("LeftSlope");
	add_child(left_slope_point);
	left_slope_point->set_custom_minimum_size(Vector2(7, 7));
	left_slope_point->set_distance(-30);

	right_slope_point = memnew(SlopePoint);
	right_slope_point->set_name("RightSlope");
	add_child(right_slope_point);
	right_slope_point->set_custom_minimum_size(Vector2(7, 7));
	right_slope_point->set_distance(30);
}

ControlPoint::~ControlPoint() {
}

void ControlPoint::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("gui_input", this, "_on_ControlPoint_gui_input");
	} else if (p_what == NOTIFICATION_DRAW) {
		_draw();
	}
}

void ControlPoint::_bind_methods() {
	ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::INT, "index")));
	ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::INT, "index")));

	ClassDB::bind_method(D_METHOD("get_moving"), &ControlPoint::get_moving);
	ClassDB::bind_method(D_METHOD("set_moving", "value"), &ControlPoint::set_moving);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");

	ClassDB::bind_method(D_METHOD("get_min_x"), &ControlPoint::get_min_x);
	ClassDB::bind_method(D_METHOD("set_min_x", "value"), &ControlPoint::set_min_x);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_x"), "set_min_x", "get_min_x");

	ClassDB::bind_method(D_METHOD("get_max_x"), &ControlPoint::get_max_x);
	ClassDB::bind_method(D_METHOD("set_max_x", "value"), &ControlPoint::set_max_x);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_x"), "set_max_x", "get_max_x");

	ClassDB::bind_method(D_METHOD("get_min_y"), &ControlPoint::get_min_y);
	ClassDB::bind_method(D_METHOD("set_min_y", "value"), &ControlPoint::set_min_y);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_y"), "set_min_y", "get_min_y");

	ClassDB::bind_method(D_METHOD("get_max_y"), &ControlPoint::get_max_y);
	ClassDB::bind_method(D_METHOD("set_max_y", "value"), &ControlPoint::set_max_y);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_y"), "set_max_y", "get_max_y");

	//ClassDB::bind_method(D_METHOD("_draw"), &ControlPoint::_draw);
	//ClassDB::bind_method(D_METHOD("initialize", "p"), &ControlPoint::initialize);
	ClassDB::bind_method(D_METHOD("set_constraint", "x", "X", "y", "Y"), &ControlPoint::set_constraint);
	ClassDB::bind_method(D_METHOD("update_tangents"), &ControlPoint::update_tangents);

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &ControlPoint::_on_ControlPoint_gui_input);
}
