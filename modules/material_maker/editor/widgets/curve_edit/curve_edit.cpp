
#include "curve_edit.h"


Variant CurveEdit::get_Variant() {
 return Variant;
}

void CurveEdit::set_Variant(const Variant &val) {
Variant = val;
}


Variant CurveEdit::get_Variant() {
 return Variant;
}

void CurveEdit::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 // setget set_value;
  Variant  = null;
 signal updated(curve);

 void CurveEdit::set_value(const Variant &v) {
  value = v;
  $CurveView.set_curve(value);
  $CurveView.update();
}


 void CurveEdit::_on_CurveEdit_pressed() {
  //var dialog = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_dialog.tscn").instance();
  add_child(dialog);
  dialog.connect("curve_changed", self, "on_value_changed");
  dialog.edit_curve(value);
}


 void CurveEdit::on_value_changed(const Variant &v) {
  //set_value(v);
  emit_signal("updated", v);
  $CurveView.update();
}

}

 CurveEdit::CurveEdit() {
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
   = null;
 }

 CurveEdit::~CurveEdit() {
 }


 static void CurveEdit::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveEdit::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveEdit::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveEdit::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveEdit::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("set_value", "v"), &CurveEdit::set_value);
  ClassDB::bind_method(D_METHOD("_on_CurveEdit_pressed"), &CurveEdit::_on_CurveEdit_pressed);
  ClassDB::bind_method(D_METHOD("on_value_changed", "v"), &CurveEdit::on_value_changed);

 }



