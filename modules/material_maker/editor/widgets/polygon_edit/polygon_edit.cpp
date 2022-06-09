
#include "polygon_edit.h"


Variant PolygonEdit::get_Variant() {
 return Variant;
}

void PolygonEdit::set_Variant(const Variant &val) {
Variant = val;
}


bool PolygonEdit::get_closed() const {
 return closed;
}

void PolygonEdit::set_closed(const bool val) {
closed = val;
}


Variant PolygonEdit::get_Variant() {
 return Variant;
}

void PolygonEdit::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //var MMPolygon = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
 //export ;
 // setget set_closed;
  bool closed = true;
 // setget set_value;
  Variant  = null;
 signal updated(polygon);

 void PolygonEdit::set_closed(const bool c) {
  closed = c;
  $PolygonView.set_closed(c);
}


 void PolygonEdit::set_value(const Variant &v) {
  value = v;
  $PolygonView.set_polygon(value);
  $PolygonView.update();
}


 void PolygonEdit::_on_PolygonEdit_pressed() {
  //var dialog = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_dialog.tscn").instance();
  dialog.set_closed(closed);
  add_child(dialog);
  dialog.connect("polygon_changed", self, "on_value_changed");
  dialog.edit_polygon(value);
}


 void PolygonEdit::on_value_changed(const Variant &v) {
  //set_value(v);
  emit_signal("updated", v);
  $PolygonView.update();
}

}

 PolygonEdit::PolygonEdit() {
 //var MMPolygon = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
  closed = true;
   = null;
 }

 PolygonEdit::~PolygonEdit() {
 }


 static void PolygonEdit::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &PolygonEdit::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &PolygonEdit::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_closed"), &PolygonEdit::get_closed);
   ClassDB::bind_method(D_METHOD("set_closed", "value"), &PolygonEdit::set_closed);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");


   ClassDB::bind_method(D_METHOD("get_Variant"), &PolygonEdit::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &PolygonEdit::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("set_closed", "c"), &PolygonEdit::set_closed, true);
  ClassDB::bind_method(D_METHOD("set_value", "v"), &PolygonEdit::set_value);
  ClassDB::bind_method(D_METHOD("_on_PolygonEdit_pressed"), &PolygonEdit::_on_PolygonEdit_pressed);
  ClassDB::bind_method(D_METHOD("on_value_changed", "v"), &PolygonEdit::on_value_changed);

 }



