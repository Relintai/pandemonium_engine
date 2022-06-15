
#include "polygon_editor.h"

#include "../../../nodes/bases/polygon_base.h"
#include "polygon_control_point.h"

void PolygonEditor::set_polygon(const Ref<PolygonBase> &p) {
	polygon = p;
	update();
	update_controls();
}

void PolygonEditor::update_controls() {
	for (int i = 0; i < get_child_count(); ++i) {
		Node *c = get_child(i);
		c->queue_delete();
	}

	if (!polygon.is_valid()) {
		return;
	}

	PoolVector2Array points = polygon->get_points();

	for (int i = 0; i < points.size(); ++i) {
		Vector2 p = points[i];

		PolygonControlPoint *control_point = memnew(PolygonControlPoint);
		control_point->polygon_editor = this;

		add_child(control_point);
		control_point->initialize(p);
		control_point->set_position(transform_point(p) - control_point->OFFSET);
		control_point->connect("moved", this, "_on_ControlPoint_moved");
		control_point->connect("removed", this, "_on_ControlPoint_removed");
	}

	emit_signal("value_changed", polygon);
}

void PolygonEditor::_on_ControlPoint_moved(const int index) {
	PolygonControlPoint *control_point = Object::cast_to<PolygonControlPoint>(get_child(index));

	PoolVector2Array points = polygon->get_points();
	points.set(index, reverse_transform_point(control_point->get_position() + control_point->OFFSET));
	polygon->set_points(points);

	update();
	emit_signal("value_changed", polygon);
}

void PolygonEditor::_on_ControlPoint_removed(const int index) {
	if (polygon->remove_point(index)) {
		update();
		update_controls();
	}
}

void PolygonEditor::_on_PolygonEditor_gui_input(const Ref<InputEvent> &event) {
	if (!polygon.is_valid()) {
		return;
	}

	Ref<InputEventMouseButton> iemb = event;

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT && iemb->is_doubleclick()) {
			Vector2 new_point_position = reverse_transform_point(get_local_mouse_position());
			polygon->add_point(new_point_position.x, new_point_position.y, closed);
			update_controls();
		}
	}
}

void PolygonEditor::_on_resize() {
	PolygonView::_on_resize();
	update_controls();
}

PolygonEditor::PolygonEditor() {
	set_anchors_and_margins_preset(PRESET_WIDE, PRESET_MODE_MINSIZE, 10);
}

PolygonEditor::~PolygonEditor() {
}

void PolygonEditor::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("gui_input", this, "_on_PolygonEditor_gui_input");
		update_controls();
	}
}

void PolygonEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase")));

	ClassDB::bind_method(D_METHOD("update_controls"), &PolygonEditor::update_controls);

	ClassDB::bind_method(D_METHOD("_on_ControlPoint_moved", "index"), &PolygonEditor::_on_ControlPoint_moved);
	ClassDB::bind_method(D_METHOD("_on_ControlPoint_removed", "index"), &PolygonEditor::_on_ControlPoint_removed);
	ClassDB::bind_method(D_METHOD("_on_PolygonEditor_gui_input", "event"), &PolygonEditor::_on_PolygonEditor_gui_input);
}
