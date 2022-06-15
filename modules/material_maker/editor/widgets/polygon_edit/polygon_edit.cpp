
#include "polygon_edit.h"

#include "polygon_dialog.h"
#include "polygon_view.h"
#include "scene/gui/button.h"

#include "../../../nodes/bases/polygon_base.h"

bool PolygonEdit::get_closed() const {
	return closed;
}

void PolygonEdit::set_closed(const bool c) {
	closed = c;
	_polygon_view->set_closed(c);
}

Ref<PolygonBase> PolygonEdit::get_value() {
	return value;
}

void PolygonEdit::set_value(const Ref<PolygonBase> &v) {
	value = v;
	_polygon_view->set_polygon(value);
	_polygon_view->update();
}

void PolygonEdit::_on_PolygonEdit_pressed() {
	PolygonDialog *dialog = memnew(PolygonDialog);
	dialog->set_closed(closed);
	add_child(dialog);
	dialog->connect("polygon_changed", this, "on_value_changed");
	dialog->edit_polygon(value);
}

void PolygonEdit::on_value_changed(const Variant &v) {
	//set_value(v);
	emit_signal("updated", v);
	_polygon_view->update();
}

PolygonEdit::PolygonEdit() {
	closed = true;

	set_custom_minimum_size(Vector2(32, 32));
	set_focus_mode(FOCUS_CLICK);

	_polygon_view = memnew(PolygonView);
	add_child(_polygon_view);
	_polygon_view->set_anchors_and_margins_preset(PRESET_WIDE);
	_polygon_view->set_mouse_filter(MOUSE_FILTER_IGNORE);
}

PolygonEdit::~PolygonEdit() {
}

void PolygonEdit::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "polygon", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase")));

	ClassDB::bind_method(D_METHOD("get_closed"), &PolygonEdit::get_closed);
	ClassDB::bind_method(D_METHOD("set_closed", "value"), &PolygonEdit::set_closed, true);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");

	ClassDB::bind_method(D_METHOD("get_value"), &PolygonEdit::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PolygonEdit::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("_on_PolygonEdit_pressed"), &PolygonEdit::_on_PolygonEdit_pressed);
	ClassDB::bind_method(D_METHOD("on_value_changed", "v"), &PolygonEdit::on_value_changed);
}
