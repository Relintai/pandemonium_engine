
#include "sd_shape_arc.h"


Ref<Resource> SdShapeArc::get_output() {
 return output;
}

void SdShapeArc::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 SdShapeArc::get_angle() {
 return angle;
}

void SdShapeArc::set_angle(const Vector2 &val) {
angle = val;
}


float SdShapeArc::get_radius() const {
 return radius;
}

void SdShapeArc::set_radius(const float val) {
radius = val;
}


float SdShapeArc::get_width() const {
 return width;
}

void SdShapeArc::set_width(const float val) {
width = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 angle = Vector2(30, 150);
 //export(float) ;
  float radius = 0.3;
 //export(float) ;
  float width = 0.1;

 void SdShapeArc::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void SdShapeArc::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_angle", "set_angle", "Angle", 1);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 0.01);
}


 float SdShapeArc::_get_property_value(const Vector2 &uv) {
  return MMAlgos.sdf_arc(uv, angle, Vector2(radius, width));
}

 //angle;

 Vector2 SdShapeArc::get_angle() {
  return angle;
}


 void SdShapeArc::set_angle(const Vector2 &val) {
  angle = val;
  emit_changed();
  output.emit_changed();
}

 //radius;

 float SdShapeArc::get_radius() {
  return radius;
}


 void SdShapeArc::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

 //width;

 float SdShapeArc::get_width() {
  return width;
}


 void SdShapeArc::set_width(const float val) {
  width = val;
  emit_changed();
  output.emit_changed();
}

}

 SdShapeArc::SdShapeArc() {
  output;
  angle = Vector2(30, 150);
  radius = 0.3;
  width = 0.1;
 }

 SdShapeArc::~SdShapeArc() {
 }


 static void SdShapeArc::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdShapeArc::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdShapeArc::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_angle"), &SdShapeArc::get_angle);
   ClassDB::bind_method(D_METHOD("set_angle", "value"), &SdShapeArc::set_angle);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "angle"), "set_angle", "get_angle");


   ClassDB::bind_method(D_METHOD("get_radius"), &SdShapeArc::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &SdShapeArc::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


   ClassDB::bind_method(D_METHOD("get_width"), &SdShapeArc::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &SdShapeArc::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShapeArc::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShapeArc::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdShapeArc::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_angle"), &SdShapeArc::get_angle);
  ClassDB::bind_method(D_METHOD("set_angle", "val"), &SdShapeArc::set_angle);
  ClassDB::bind_method(D_METHOD("get_radius"), &SdShapeArc::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &SdShapeArc::set_radius);
  ClassDB::bind_method(D_METHOD("get_width"), &SdShapeArc::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &SdShapeArc::set_width);

 }



