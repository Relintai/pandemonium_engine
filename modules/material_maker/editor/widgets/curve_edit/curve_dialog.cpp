
#include "curve_dialog.h"

#include "curve_editor.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/separator.h"

Ref<CurveBase> CurveDialog::get_curve() {
	return curve;
}

void CurveDialog::set_curve(const Ref<CurveBase> &val) {
	curve = val;
}

Vector<CurveBase::Point> CurveDialog::get_previous_points() {
	return previous_points;
}

void CurveDialog::set_previous_points(const Vector<CurveBase::Point> &val) {
	previous_points = val;
}

void CurveDialog::edit_curve(const Ref<CurveBase> &c) {
	curve = c;
	previous_points = curve->get_points();
	_curve_editor->set_curve(curve);
	popup_centered();
}

void CurveDialog::_on_CurveDialog_popup_hide() {
	queue_delete();
}

void CurveDialog::_on_OK_pressed() {
	emit_signal("curve_changed", curve);
	curve->curve_changed();
	queue_delete();
}

void CurveDialog::_on_Cancel_pressed() {
	curve->set_points(previous_points);
	emit_signal("curve_changed", curve);
	queue_delete();
}

void CurveDialog::_on_MMCurveEditor_value_changed(const Ref<CurveBase> &value) {
	emit_signal("curve_changed", value);
}

CurveDialog::CurveDialog() {
	set_title("Edit curve");

	set_size(Vector2(300, 300));

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

	_curve_editor = memnew(MMCurveEditor);
	_curve_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	mc->add_child(_curve_editor);

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

CurveDialog::~CurveDialog() {
}

void CurveDialog::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("popup_hide", this, "_on_CurveDialog_popup_hide");
		_curve_editor->connect("value_changed", this, "_on_MMCurveEditor_value_changed");
		_ok_button->connect("pressed", this, "_on_OK_pressed");
		_cancel_button->connect("pressed", this, "_on_Cancel_pressed");
	}
}

void CurveDialog::_bind_methods() {
	ADD_SIGNAL(MethodInfo("curve_changed", PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase")));

	ClassDB::bind_method(D_METHOD("get_curve"), &CurveDialog::get_curve);
	ClassDB::bind_method(D_METHOD("set_curve", "value"), &CurveDialog::set_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase"), "set_curve", "get_curve");

	//ClassDB::bind_method(D_METHOD("get_previous_points"), &CurveDialog::get_previous_points);
	//ClassDB::bind_method(D_METHOD("set_previous_points", "value"), &CurveDialog::set_previous_points);
	//ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "previous_points"), "set_previous_points", "get_previous_points");

	ClassDB::bind_method(D_METHOD("edit_curve", "c"), &CurveDialog::edit_curve);

	ClassDB::bind_method(D_METHOD("_on_CurveDialog_popup_hide"), &CurveDialog::_on_CurveDialog_popup_hide);
	ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &CurveDialog::_on_OK_pressed);
	ClassDB::bind_method(D_METHOD("_on_Cancel_pressed"), &CurveDialog::_on_Cancel_pressed);
	ClassDB::bind_method(D_METHOD("_on_MMCurveEditor_value_changed", "value"), &CurveDialog::_on_MMCurveEditor_value_changed);
}
