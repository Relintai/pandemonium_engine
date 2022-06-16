
#include "sdf3d_shape_cylinder.h"


Ref<Resource> Sdf3dShapeCylinder::get_output() {
 return output;
}

void Sdf3dShapeCylinder::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dShapeCylinder::get_axis() const {
 return axis;
}

void Sdf3dShapeCylinder::set_axis(const int val) {
axis = val;
}


float Sdf3dShapeCylinder::get_length() const {
 return length;
}

void Sdf3dShapeCylinder::set_length(const float val) {
length = val;
}


float Sdf3dShapeCylinder::get_radius() const {
 return radius;
}

void Sdf3dShapeCylinder::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "X,Y,Z") ;
  int axis = 1;
 //export(float) ;
  float length = 0.25;
 //export(float) ;
  float radius = 0.25;

 void Sdf3dShapeCylinder::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeCylinder::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_axis", "set_axis", "Axis", [ "X", "Y", "Z" ]);
  mm_graph_node.add_slot_float("get_length", "set_length", "Length", 0.01);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 Vector2 Sdf3dShapeCylinder::_get_property_value_sdf3d(const Vector3 &uv3) {

  if (axis == 0) {
   return MMAlgos.sdf3d_cylinder_x(uv3, radius, length);
}


  else if (axis == 1) {
   return MMAlgos.sdf3d_cylinder_y(uv3, radius, length);
}


  else if (axis == 2) {
   return MMAlgos.sdf3d_cylinder_z(uv3, radius, length);
}

  return Vector2();
}

 //axis;

 int Sdf3dShapeCylinder::get_axis() {
  return axis;
}


 void Sdf3dShapeCylinder::set_axis(const int val) {
  axis = val;
  emit_changed();
  output.emit_changed();
}

 //length;

 float Sdf3dShapeCylinder::get_length() {
  return length;
}


 void Sdf3dShapeCylinder::set_length(const float val) {
  length = val;
  emit_changed();
  output.emit_changed();
}

 //radius;

 float Sdf3dShapeCylinder::get_radius() {
  return radius;
}


 void Sdf3dShapeCylinder::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeCylinder::Sdf3dShapeCylinder() {
  output;
  axis = 1;
  length = 0.25;
  radius = 0.25;
 }

 Sdf3dShapeCylinder::~Sdf3dShapeCylinder() {
 }


 static void Sdf3dShapeCylinder::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeCylinder::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeCylinder::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCylinder::get_axis);
   ClassDB::bind_method(D_METHOD("set_axis", "value"), &Sdf3dShapeCylinder::set_axis);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");


   ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dShapeCylinder::get_length);
   ClassDB::bind_method(D_METHOD("set_length", "value"), &Sdf3dShapeCylinder::set_length);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");


   ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeCylinder::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Sdf3dShapeCylinder::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeCylinder::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeCylinder::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeCylinder::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCylinder::get_axis);
  ClassDB::bind_method(D_METHOD("set_axis", "val"), &Sdf3dShapeCylinder::set_axis);
  ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dShapeCylinder::get_length);
  ClassDB::bind_method(D_METHOD("set_length", "val"), &Sdf3dShapeCylinder::set_length);
  ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeCylinder::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &Sdf3dShapeCylinder::set_radius);

 }



