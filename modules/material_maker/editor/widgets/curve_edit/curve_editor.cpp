
#include "curve_editor.h"

#include "../../../nodes/bases/curve_base.h"
#include "control_point.h"
#include "core/object.h"
#include "slope_point.h"

void MMCurveEditor::set_curve(const Ref<CurveBase> &c) {
	curve = c;
	update();
	update_controls();
}

void MMCurveEditor::update_controls() {
	if (!curve.is_valid()) {
		return;
	}

	for (int i = 0; i < get_child_count(); ++i) {
		Node *c = get_child(i);
		c->queue_delete();
	}

	Vector<CurveBase::Point> points = curve->get_points();

	for (int i = 0; i < points.size(); ++i) {
		CurveBase::Point p = points[i];
		ControlPoint *control_point = memnew(ControlPoint);
		add_child(control_point);
		control_point->initialize(p);
		control_point->set_position(transform_point(p.p) - control_point->OFFSET);

		if (i == 0 || i == points.size() - 1) {
			control_point->set_constraint(control_point->get_position().x, control_point->get_position().x, -control_point->OFFSET.y, get_size().y - control_point->OFFSET.y);

			if (i == 0) {
				Object::cast_to<Control>(control_point->get_child(0))->set_visible(false);
			} else {
				Object::cast_to<Control>(control_point->get_child(1))->set_visible(false);
			}
		} else {
			float min_x = transform_point(points[i - 1].p).x + 1;
			float max_x = transform_point(points[i + 1].p).x - 1;
			control_point->set_constraint(min_x, max_x, -control_point->OFFSET.y, get_size().y - control_point->OFFSET.y);
		}

		control_point->connect("moved", this, "_on_ControlPoint_moved");
		control_point->connect("removed", this, "_on_ControlPoint_removed");
	}

	emit_signal("value_changed", curve);
}

void MMCurveEditor::_on_ControlPoint_moved(const int index) {
	Vector<CurveBase::Point> points = curve->get_points();
	ControlPoint *control_point = Object::cast_to<ControlPoint>(get_child(index));
	points.write[index].p = reverse_transform_point(control_point->get_position() + control_point->OFFSET);

	//if (control_point.has_node("LeftSlope")) {
	Vector2 slope_vector = control_point->left_slope_point->get_position() / get_size();

	if (slope_vector.x != 0) {
		points.write[index].ls = -slope_vector.y / slope_vector.x;
	}
	//}

	//if (control_point.has_node("RightSlope")) {
	slope_vector = control_point->right_slope_point->get_position() / get_size();

	if (slope_vector.x != 0) {
		points.write[index].rs = -slope_vector.y / slope_vector.x;
	}
	//}

	curve->set_points(points, false);
	update();
	emit_signal("value_changed", curve);
}

void MMCurveEditor::_on_ControlPoint_removed(const int index) {
	if (curve->remove_point(index)) {
		update();
		update_controls();
	}
}

void MMCurveEditor::_on_MMCurveEditor_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventMouseButton> iemb = event;

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT && iemb->is_doubleclick()) {
			Vector2 new_point_position = reverse_transform_point(get_local_mouse_position());
			curve->add_point(new_point_position.x, new_point_position.y, 0.0, 0.0);
			update_controls();
		}
	}
}

void MMCurveEditor::_on_resize() {
	CurveView::_on_resize();
	update_controls();
}

MMCurveEditor::MMCurveEditor() {
	set_mouse_filter(MOUSE_FILTER_STOP);
}

MMCurveEditor::~MMCurveEditor() {
}

void MMCurveEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("gui_input", this, "_on_MMCurveEditor_gui_input");
			update_controls();
		} break;
		default: {
		} break;
	}
}
void MMCurveEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase")));

	ClassDB::bind_method(D_METHOD("update_controls"), &MMCurveEditor::update_controls);

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_moved", "index"), &MMCurveEditor::_on_ControlPoint_moved);
	ClassDB::bind_method(D_METHOD("_on_ControlPoint_removed", "index"), &MMCurveEditor::_on_ControlPoint_removed);
	ClassDB::bind_method(D_METHOD("_on_MMCurveEditor_gui_input", "event"), &MMCurveEditor::_on_MMCurveEditor_gui_input);
}
