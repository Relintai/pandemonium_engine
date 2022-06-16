
#include "sdf3d_shape_torus.h"


Ref<Resource> Sdf3dShapeTorus::get_output() {
 return output;
}

void Sdf3dShapeTorus::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dShapeTorus::get_axis() const {
 return axis;
}

void Sdf3dShapeTorus::set_axis(const int val) {
axis = val;
}


float Sdf3dShapeTorus::get_major_radius() const {
 return major_radius;
}

void Sdf3dShapeTorus::set_major_radius(const float val) {
major_radius = val;
}


float Sdf3dShapeTorus::get_minor_radius() const {
 return minor_radius;
}

void Sdf3dShapeTorus::set_minor_radius(const float val) {
minor_radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "X,Y,Z") ;
  int axis = 2;
 //export(float) ;
  float major_radius = 0.3;
 //export(float) ;
  float minor_radius = 0.15;

 void Sdf3dShapeTorus::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeTorus::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_axis", "set_axis", "Axis", [ "X", "Y", "Z" ]);
  mm_graph_node.add_slot_float("get_major_radius", "set_major_radius", "Major_radius", 0.01);
  mm_graph_node.add_slot_float("get_minor_radius", "set_minor_radius", "Minor_radius", 0.01);
}


 Vector2 Sdf3dShapeTorus::_get_property_value_sdf3d(const Vector3 &uv3) {

  if (axis == 0) {
   return MMAlgos.sdf3d_torus_x(uv3, major_radius, minor_radius);
}


  else if (axis == 1) {
   return MMAlgos.sdf3d_torus_y(uv3, major_radius, minor_radius);
}


  else if (axis == 2) {
   return MMAlgos.sdf3d_torus_z(uv3, major_radius, minor_radius);
}

  return Vector2();
}

 //axis;

 int Sdf3dShapeTorus::get_axis() {
  return axis;
}


 void Sdf3dShapeTorus::set_axis(const int val) {
  axis = val;
  emit_changed();
  output.emit_changed();
}

 //major_radius;

 float Sdf3dShapeTorus::get_major_radius() {
  return major_radius;
}


 void Sdf3dShapeTorus::set_major_radius(const float val) {
  major_radius = val;
  emit_changed();
  output.emit_changed();
}

 //minor_radius;

 float Sdf3dShapeTorus::get_minor_radius() {
  return minor_radius;
}


 void Sdf3dShapeTorus::set_minor_radius(const float val) {
  minor_radius = val;
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeTorus::Sdf3dShapeTorus() {
  output;
  axis = 2;
  major_radius = 0.3;
  minor_radius = 0.15;
 }

 Sdf3dShapeTorus::~Sdf3dShapeTorus() {
 }


 static void Sdf3dShapeTorus::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeTorus::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeTorus::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeTorus::get_axis);
   ClassDB::bind_method(D_METHOD("set_axis", "value"), &Sdf3dShapeTorus::set_axis);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");


   ClassDB::bind_method(D_METHOD("get_major_radius"), &Sdf3dShapeTorus::get_major_radius);
   ClassDB::bind_method(D_METHOD("set_major_radius", "value"), &Sdf3dShapeTorus::set_major_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "major_radius"), "set_major_radius", "get_major_radius");


   ClassDB::bind_method(D_METHOD("get_minor_radius"), &Sdf3dShapeTorus::get_minor_radius);
   ClassDB::bind_method(D_METHOD("set_minor_radius", "value"), &Sdf3dShapeTorus::set_minor_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "minor_radius"), "set_minor_radius", "get_minor_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeTorus::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeTorus::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeTorus::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeTorus::get_axis);
  ClassDB::bind_method(D_METHOD("set_axis", "val"), &Sdf3dShapeTorus::set_axis);
  ClassDB::bind_method(D_METHOD("get_major_radius"), &Sdf3dShapeTorus::get_major_radius);
  ClassDB::bind_method(D_METHOD("set_major_radius", "val"), &Sdf3dShapeTorus::set_major_radius);
  ClassDB::bind_method(D_METHOD("get_minor_radius"), &Sdf3dShapeTorus::get_minor_radius);
  ClassDB::bind_method(D_METHOD("set_minor_radius", "val"), &Sdf3dShapeTorus::set_minor_radius);

 }



