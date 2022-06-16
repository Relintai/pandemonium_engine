
#include "sdf3d_shape_capsule.h"


Ref<Resource> Sdf3dShapeCapsule::get_output() {
 return output;
}

void Sdf3dShapeCapsule::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dShapeCapsule::get_axis() const {
 return axis;
}

void Sdf3dShapeCapsule::set_axis(const int val) {
axis = val;
}


float Sdf3dShapeCapsule::get_length() const {
 return length;
}

void Sdf3dShapeCapsule::set_length(const float val) {
length = val;
}


float Sdf3dShapeCapsule::get_radius() const {
 return radius;
}

void Sdf3dShapeCapsule::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "X,Y,Z") ;
  int axis = 1;
 //export(float) ;
  float length = 0.3;
 //export(float) ;
  float radius = 0.2;

 void Sdf3dShapeCapsule::_init() {
  init_points_11();
}


 void Sdf3dShapeCapsule::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeCapsule::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_axis", "set_axis", "Axis", [ "X", "Y", "Z" ]);
  mm_graph_node.add_slot_float("get_length", "set_length", "Length", 0.01);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
  mm_graph_node.add_slot_curve();
}


 Vector2 Sdf3dShapeCapsule::_get_property_value_sdf3d(const Vector3 &uv3) {

  if (axis == 0) {
   return sdf3d_capsule_x(uv3, radius, length);
}


  else if (axis == 1) {
   return sdf3d_capsule_y(uv3, radius, length);
}


  else if (axis == 2) {
   return sdf3d_capsule_z(uv3, radius, length);
}

  return Vector2();
}

 //vec3 $(name_uv)_p = $uv;
 //$(name_uv)_p.$axis -= clamp($(name_uv)_p.$axis, -$l, $l);
 //return length($(name_uv)_p) - $r * $profile(clamp(0.5+0.5*($uv).$axis/$l, 0.0, 1.0));

 Vector2 Sdf3dShapeCapsule::sdf3d_capsule_y(const Vector3 &p, const float r, const float l) {
   Vector3 v = p;
  v.y -= clamp(v.y, -l, l);
   float cx = clamp(0.5 + 0.5 * p.y / l, 0.0, 1.0);
   float cp = MMAlgos.curve(cx, points_array);
   float f = v.length() - r * cp;
  return Vector2(f, 0.0);
}


 Vector2 Sdf3dShapeCapsule::sdf3d_capsule_x(const Vector3 &p, const float r, const float l) {
   Vector3 v = p;
  v.x -= clamp(v.x, -l, l);
   float cx = clamp(0.5 + 0.5 * p.x / l, 0.0, 1.0);
   float cp = MMAlgos.curve(cx, points_array);
   float f = v.length() - r * cp;
  return Vector2(f, 0.0);
}


 Vector2 Sdf3dShapeCapsule::sdf3d_capsule_z(const Vector3 &p, const float r, const float l) {
   Vector3 v = p;
  v.z -= clamp(v.z, -l, l);
   float cx = clamp(0.5 + 0.5 * p.z / l, 0.0, 1.0);
   float cp = MMAlgos.curve(cx, points_array);
   float f = v.length() - r * cp;
  return Vector2(f, 0.0);
}

 //axis;

 int Sdf3dShapeCapsule::get_axis() {
  return axis;
}


 void Sdf3dShapeCapsule::set_axis(const int val) {
  axis = val;
  emit_changed();
  output.emit_changed();
}

 //length;

 float Sdf3dShapeCapsule::get_length() {
  return length;
}


 void Sdf3dShapeCapsule::set_length(const float val) {
  length = val;
  emit_changed();
  output.emit_changed();
}

 //radius;

 float Sdf3dShapeCapsule::get_radius() {
  return radius;
}


 void Sdf3dShapeCapsule::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dShapeCapsule::_curve_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeCapsule::Sdf3dShapeCapsule() {
  output;
  axis = 1;
  length = 0.3;
  radius = 0.2;
 }

 Sdf3dShapeCapsule::~Sdf3dShapeCapsule() {
 }


 static void Sdf3dShapeCapsule::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeCapsule::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeCapsule::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCapsule::get_axis);
   ClassDB::bind_method(D_METHOD("set_axis", "value"), &Sdf3dShapeCapsule::set_axis);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");


   ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dShapeCapsule::get_length);
   ClassDB::bind_method(D_METHOD("set_length", "value"), &Sdf3dShapeCapsule::set_length);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");


   ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeCapsule::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Sdf3dShapeCapsule::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init"), &Sdf3dShapeCapsule::_init);
  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeCapsule::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeCapsule::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeCapsule::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("sdf3d_capsule_y", "p", "r", "l"), &Sdf3dShapeCapsule::sdf3d_capsule_y);
  ClassDB::bind_method(D_METHOD("sdf3d_capsule_x", "p", "r", "l"), &Sdf3dShapeCapsule::sdf3d_capsule_x);
  ClassDB::bind_method(D_METHOD("sdf3d_capsule_z", "p", "r", "l"), &Sdf3dShapeCapsule::sdf3d_capsule_z);
  ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCapsule::get_axis);
  ClassDB::bind_method(D_METHOD("set_axis", "val"), &Sdf3dShapeCapsule::set_axis);
  ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dShapeCapsule::get_length);
  ClassDB::bind_method(D_METHOD("set_length", "val"), &Sdf3dShapeCapsule::set_length);
  ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeCapsule::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &Sdf3dShapeCapsule::set_radius);
  ClassDB::bind_method(D_METHOD("_curve_changed"), &Sdf3dShapeCapsule::_curve_changed);

 }



