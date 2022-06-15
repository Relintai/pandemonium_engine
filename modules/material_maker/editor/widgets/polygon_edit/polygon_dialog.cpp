
#include "polygon_dialog.h"

#include "../../../nodes/bases/polygon_base.h"

#include "polygon_editor.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/separator.h"

bool PolygonDialog::get_closed() const {
	return closed;
}

void PolygonDialog::set_closed(const bool c) {
	closed = c;
	if (closed) {
		set_title("Edit polygon");
	} else {
		set_title("Edit polyline");
	}

	_polygon_editor->set_closed(closed);
}

PoolVector2Array PolygonDialog::get_previous_points() {
	return previous_points;
}

void PolygonDialog::set_previous_points(const PoolVector2Array &val) {
	previous_points = val;
}

Ref<PolygonBase> PolygonDialog::get_polygon() {
	return polygon;
}

void PolygonDialog::set_polygon(const Ref<PolygonBase> &val) {
	edit_polygon(val);
}

void PolygonDialog::edit_polygon(const Ref<PolygonBase> &poly) {
	polygon = poly;
	previous_points = polygon->get_points();
	_polygon_editor->set_polygon(polygon);
	popup_centered();
	//var result = yield(self, "return_polygon");
	//queue_free();
	//return result;
}

void PolygonDialog::_on_CurveDialog_popup_hide() {
	//	emit_signal("return_polygon", null);
	queue_delete();
}

void PolygonDialog::_on_OK_pressed() {
	emit_signal("polygon_changed", polygon);
	polygon->polygon_changed();
	queue_delete();
}

void PolygonDialog::_on_Cancel_pressed() {
	polygon->set_points(previous_points);
	emit_signal("polygon_changed", polygon);
	queue_delete();
}

void PolygonDialog::_on_PolygonEditor_value_changed(const Variant &value) {
	emit_signal("polygon_changed", value);
}

PolygonDialog::PolygonDialog() {
	closed = true;

	set_title("Edit polygon");

	set_size(Vector2(300, 300));
	//set_resizable(true);

	MarginContainer *main_mc = memnew(MarginContainer);
	main_mc->set_anchors_and_margins_preset(PRESET_WIDE);
	main_mc->set("custom_constants/margin_right", 4);
	main_mc->set("custom_constants/margin_top", 4);
	main_mc->set("custom_constants/margin_left", 4);
	main_mc->set("custom_constants/margin_bottom", 4);
	add_child(main_mc);

	VBoxContainer *mvbc = memnew(VBoxContainer);
	main_mc->add_child(mvbc);

	MarginContainer *mc = memnew(MarginContainer);
	mc->set_clip_contents(true);
	mc->set_h_size_flags(SIZE_EXPAND_FILL);
	mc->set_v_size_flags(SIZE_EXPAND_FILL);

	mc->set("custom_constants/margin_right", 4);
	mc->set("custom_constants/margin_top", 4);
	mc->set("custom_constants/margin_left", 4);
	mc->set("custom_constants/margin_bottom", 4);

	mvbc->add_child(mc);

	_polygon_editor = memnew(PolygonEditor);
	_polygon_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	mc->add_child(_polygon_editor);

	HSeparator *hsep = memnew(HSeparator);
	mvbc->add_child(hsep);

	HBoxContainer *bhbc = memnew(HBoxContainer);
	mvbc->add_child(bhbc);

	Control *spacer = memnew(Control);
	spacer->set_h_size_flags(SIZE_EXPAND_FILL);
	bhbc->add_child(spacer);

	_ok_button = memnew(Button);
	_ok_button->set_text("OK");
	bhbc->add_child(_ok_button);

	_cancel_button = memnew(Button);
	_cancel_button->set_text("Cancel");
	bhbc->add_child(_cancel_button);
}

PolygonDialog::~PolygonDialog() {
}

void PolygonDialog::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		_ok_button->connect("pressed", this, "_on_OK_pressed");
		_cancel_button->connect("pressed", this, "_on_Cancel_pressed");
		connect("popup_hide", this, "_on_CurveDialog_popup_hide");
		_polygon_editor->connect("value_changed", this, "_on_PolygonEditor_value_changed");
	}
}

void PolygonDialog::_bind_methods() {
	ADD_SIGNAL(MethodInfo("polygon_changed", PropertyInfo(Variant::OBJECT, "polygon", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase")));

	ClassDB::bind_method(D_METHOD("get_closed"), &PolygonDialog::get_closed);
	ClassDB::bind_method(D_METHOD("set_closed", "c"), &PolygonDialog::set_closed, true);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");

	ClassDB::bind_method(D_METHOD("get_previous_points"), &PolygonDialog::get_previous_points);
	ClassDB::bind_method(D_METHOD("set_previous_points", "value"), &PolygonDialog::set_previous_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "previous_points"), "set_previous_points", "get_previous_points");

	ClassDB::bind_method(D_METHOD("get_polygon"), &PolygonDialog::get_polygon);
	ClassDB::bind_method(D_METHOD("set_polygon", "value"), &PolygonDialog::set_polygon);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "polygon", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase"), "set_polygon", "get_polygon");

	ClassDB::bind_method(D_METHOD("edit_polygon", "poly"), &PolygonDialog::edit_polygon);

	ClassDB::bind_method(D_METHOD("_on_CurveDialog_popup_hide"), &PolygonDialog::_on_CurveDialog_popup_hide);
	ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &PolygonDialog::_on_OK_pressed);
	ClassDB::bind_method(D_METHOD("_on_Cancel_pressed"), &PolygonDialog::_on_Cancel_pressed);
	ClassDB::bind_method(D_METHOD("_on_PolygonEditor_value_changed", "value"), &PolygonDialog::_on_PolygonEditor_value_changed);
}
