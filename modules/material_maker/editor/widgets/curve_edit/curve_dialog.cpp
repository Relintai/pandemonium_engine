
#include "curve_dialog.h"


Variant CurveDialog::get_Variant() {
 return Variant;
}

void CurveDialog::set_Variant(const Variant &val) {
Variant = val;
}


Array CurveDialog::get_previous_points() {
 return previous_points;
}

void CurveDialog::set_previous_points(const Array &val) {
previous_points = val;
}


Variant CurveDialog::get_Variant() {
 return Variant;
}

void CurveDialog::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
  Array previous_points = ;
  Variant ;
 signal curve_changed(curve);
 // Called when the node enters the scene tree for the first time.;

 void CurveDialog::_ready() {
  // Replace with function body.;
  pass;
}


 void CurveDialog::_on_CurveDialog_popup_hide() {
  queue_free();
}


 void CurveDialog::_on_OK_pressed() {
  emit_signal("curve_changed", curve);
  curve.curve_changed();
  queue_free();
}


 void CurveDialog::_on_Cancel_pressed() {
  curve.set_points(previous_points);
  emit_signal("curve_changed", curve);
  queue_free();
}


 void CurveDialog::edit_curve(const Variant &c) {
  curve = c;
  previous_points = curve.get_points();
  $VBoxContainer/EditorContainer/CurveEditor.set_curve(curve);
  popup_centered();
}


 void CurveDialog::_on_CurveEditor_value_changed(const Variant &value) {
  emit_signal("curve_changed", value);
}

}

 CurveDialog::CurveDialog() {
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
  previous_points = ;
  ;
 }

 CurveDialog::~CurveDialog() {
 }


 static void CurveDialog::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveDialog::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveDialog::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_previous_points"), &CurveDialog::get_previous_points);
   ClassDB::bind_method(D_METHOD("set_previous_points", "value"), &CurveDialog::set_previous_points);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "previous_points"), "set_previous_points", "get_previous_points");


   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveDialog::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveDialog::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_ready"), &CurveDialog::_ready);
  ClassDB::bind_method(D_METHOD("_on_CurveDialog_popup_hide"), &CurveDialog::_on_CurveDialog_popup_hide);
  ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &CurveDialog::_on_OK_pressed);
  ClassDB::bind_method(D_METHOD("_on_Cancel_pressed"), &CurveDialog::_on_Cancel_pressed);
  ClassDB::bind_method(D_METHOD("edit_curve", "c"), &CurveDialog::edit_curve);
  ClassDB::bind_method(D_METHOD("_on_CurveEditor_value_changed", "value"), &CurveDialog::_on_CurveEditor_value_changed);

 }



