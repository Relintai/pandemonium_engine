
#include "sd_shape_circle.h"


Ref<Resource> SdShapeCircle::get_output() {
 return output;
}

void SdShapeCircle::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 SdShapeCircle::get_center() {
 return center;
}

void SdShapeCircle::set_center(const Vector2 &val) {
center = val;
}


float SdShapeCircle::get_radius() const {
 return radius;
}

void SdShapeCircle::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 center = Vector2(0, 0);
 //export(float) ;
  float radius = 0.4;

 void SdShapeCircle::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void SdShapeCircle::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_center", "set_center", "Center", 0.01);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 float SdShapeCircle::_get_property_value(const Vector2 &uv) {
  return MMAlgos.sdf_circle(uv, center, radius);
}

 //center;

 Vector2 SdShapeCircle::get_center() {
  return center;
}


 void SdShapeCircle::set_center(const Vector2 &val) {
  center = val;
  emit_changed();
  output.emit_changed();
}

 //radius;

 float SdShapeCircle::get_radius() {
  return radius;
}


 void SdShapeCircle::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

}

 SdShapeCircle::SdShapeCircle() {
  output;
  center = Vector2(0, 0);
  radius = 0.4;
 }

 SdShapeCircle::~SdShapeCircle() {
 }


 static void SdShapeCircle::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdShapeCircle::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdShapeCircle::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_center"), &SdShapeCircle::get_center);
   ClassDB::bind_method(D_METHOD("set_center", "value"), &SdShapeCircle::set_center);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");


   ClassDB::bind_method(D_METHOD("get_radius"), &SdShapeCircle::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &SdShapeCircle::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShapeCircle::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShapeCircle::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdShapeCircle::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_center"), &SdShapeCircle::get_center);
  ClassDB::bind_method(D_METHOD("set_center", "val"), &SdShapeCircle::set_center);
  ClassDB::bind_method(D_METHOD("get_radius"), &SdShapeCircle::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &SdShapeCircle::set_radius);

 }



