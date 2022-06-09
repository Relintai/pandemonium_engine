
#include "polygon_dialog.h"


bool PolygonDialog::get_closed() const {
 return closed;
}

void PolygonDialog::set_closed(const bool val) {
closed = val;
}


PoolVector2Array PolygonDialog::get_previous_points() {
 return previous_points;
}

void PolygonDialog::set_previous_points(const PoolVector2Array &val) {
previous_points = val;
}


Variant PolygonDialog::get_Variant() {
 return Variant;
}

void PolygonDialog::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //export ;
 // setget set_closed;
  bool closed = true;
  PoolVector2Array previous_points = ;
  Variant ;
 signal polygon_changed(polygon);

 void PolygonDialog::set_closed(const bool c) {
  closed = c;
  window_title = "Edit polygon" if closed else "Edit polyline";
  $VBoxContainer/EditorContainer/PolygonEditor.set_closed(closed);
}


 void PolygonDialog::_on_CurveDialog_popup_hide() {
  //	emit_signal("return_polygon", null);
  queue_free();
  pass;
}


 void PolygonDialog::_on_OK_pressed() {
  emit_signal("polygon_changed", polygon);
  polygon.polygon_changed();
  queue_free();
}


 void PolygonDialog::_on_Cancel_pressed() {
  polygon.set_points(previous_points);
  emit_signal("polygon_changed", polygon);
  queue_free();
}


 void PolygonDialog::edit_polygon(const Variant &poly) {
  polygon = poly;
  previous_points = polygon.points;
  $VBoxContainer/EditorContainer/PolygonEditor.set_polygon(polygon);
  popup_centered();
}

 //var result = yield(self, "return_polygon");
 //queue_free();
 //return result;

 void PolygonDialog::_on_PolygonEditor_value_changed(const Variant &value) {
  emit_signal("polygon_changed", value);
}

}

 PolygonDialog::PolygonDialog() {
  closed = true;
  previous_points = ;
  ;
 }

 PolygonDialog::~PolygonDialog() {
 }


 static void PolygonDialog::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_closed"), &PolygonDialog::get_closed);
   ClassDB::bind_method(D_METHOD("set_closed", "value"), &PolygonDialog::set_closed);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");


   ClassDB::bind_method(D_METHOD("get_previous_points"), &PolygonDialog::get_previous_points);
   ClassDB::bind_method(D_METHOD("set_previous_points", "value"), &PolygonDialog::set_previous_points);
   ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "previous_points"), "set_previous_points", "get_previous_points");


   ClassDB::bind_method(D_METHOD("get_Variant"), &PolygonDialog::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &PolygonDialog::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("set_closed", "c"), &PolygonDialog::set_closed, true);
  ClassDB::bind_method(D_METHOD("_on_CurveDialog_popup_hide"), &PolygonDialog::_on_CurveDialog_popup_hide);
  ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &PolygonDialog::_on_OK_pressed);
  ClassDB::bind_method(D_METHOD("_on_Cancel_pressed"), &PolygonDialog::_on_Cancel_pressed);
  ClassDB::bind_method(D_METHOD("edit_polygon", "poly"), &PolygonDialog::edit_polygon);
  ClassDB::bind_method(D_METHOD("_on_PolygonEditor_value_changed", "value"), &PolygonDialog::_on_PolygonEditor_value_changed);

 }



