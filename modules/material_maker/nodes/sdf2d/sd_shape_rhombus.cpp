
#include "sd_shape_rhombus.h"


Ref<Resource> SdShapeRhombus::get_output() {
 return output;
}

void SdShapeRhombus::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 SdShapeRhombus::get_center() {
 return center;
}

void SdShapeRhombus::set_center(const Vector2 &val) {
center = val;
}


Vector2 SdShapeRhombus::get_size() {
 return size;
}

void SdShapeRhombus::set_size(const Vector2 &val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 center = Vector2(0, 0);
 //export(Vector2) ;
  Vector2 size = Vector2(0.3, 0.2);

 void SdShapeRhombus::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void SdShapeRhombus::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_center", "set_center", "Center", 0.01);
  mm_graph_node.add_slot_vector2("get_size", "set_size", "Size", 0.01);
}


 float SdShapeRhombus::_get_property_value(const Vector2 &uv) {
  return MMAlgos.sdf_rhombus(uv, center, size);
}

 //center;

 Vector2 SdShapeRhombus::get_center() {
  return center;
}


 void SdShapeRhombus::set_center(const Vector2 &val) {
  center = val;
  emit_changed();
  output.emit_changed();
}

 //size;

 Vector2 SdShapeRhombus::get_size() {
  return size;
}


 void SdShapeRhombus::set_size(const Vector2 &val) {
  size = val;
  emit_changed();
  output.emit_changed();
}

}

 SdShapeRhombus::SdShapeRhombus() {
  output;
  center = Vector2(0, 0);
  size = Vector2(0.3, 0.2);
 }

 SdShapeRhombus::~SdShapeRhombus() {
 }


 static void SdShapeRhombus::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdShapeRhombus::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdShapeRhombus::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_center"), &SdShapeRhombus::get_center);
   ClassDB::bind_method(D_METHOD("set_center", "value"), &SdShapeRhombus::set_center);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");


   ClassDB::bind_method(D_METHOD("get_size"), &SdShapeRhombus::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &SdShapeRhombus::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShapeRhombus::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShapeRhombus::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdShapeRhombus::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_center"), &SdShapeRhombus::get_center);
  ClassDB::bind_method(D_METHOD("set_center", "val"), &SdShapeRhombus::set_center);
  ClassDB::bind_method(D_METHOD("get_size"), &SdShapeRhombus::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &SdShapeRhombus::set_size);

 }



