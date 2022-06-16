
#include "sdf3d_shape_cone.h"


Ref<Resource> Sdf3dShapeCone::get_output() {
 return output;
}

void Sdf3dShapeCone::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dShapeCone::get_axis() const {
 return axis;
}

void Sdf3dShapeCone::set_axis(const int val) {
axis = val;
}


float Sdf3dShapeCone::get_angle() const {
 return angle;
}

void Sdf3dShapeCone::set_angle(const float val) {
angle = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "+X,-X,+Y,-Y,+Z,-Z") ;
  int axis = 2;
 //export(float) ;
  float angle = 30;

 void Sdf3dShapeCone::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeCone::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_axis", "set_axis", "Axis", [ "+X", "-X", "+Y", "-Y", "+Z", "-Z" ]);
  mm_graph_node.add_slot_float("get_angle", "set_angle", "Angle", 1);
}


 Vector2 Sdf3dShapeCone::_get_property_value_sdf3d(const Vector3 &uv3) {

  if (axis == 0) {
   return MMAlgos.sdf3d_cone_px(uv3, angle);
}


  else if (axis == 1) {
   return MMAlgos.sdf3d_cone_nx(uv3, angle);
}


  else if (axis == 2) {
   return MMAlgos.sdf3d_cone_py(uv3, angle);
}


  else if (axis == 3) {
   return MMAlgos.sdf3d_cone_ny(uv3, angle);
}


  else if (axis == 4) {
   return MMAlgos.sdf3d_cone_pz(uv3, angle);
}


  else if (axis == 5) {
   return MMAlgos.sdf3d_cone_nz(uv3, angle);
}

  return Vector2();
}

 //axis;

 int Sdf3dShapeCone::get_axis() {
  return axis;
}


 void Sdf3dShapeCone::set_axis(const int val) {
  axis = val;
  emit_changed();
  output.emit_changed();
}

 //angle;

 float Sdf3dShapeCone::get_angle() {
  return angle;
}


 void Sdf3dShapeCone::set_angle(const float val) {
  angle = val;
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeCone::Sdf3dShapeCone() {
  output;
  axis = 2;
  angle = 30;
 }

 Sdf3dShapeCone::~Sdf3dShapeCone() {
 }


 static void Sdf3dShapeCone::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeCone::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeCone::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCone::get_axis);
   ClassDB::bind_method(D_METHOD("set_axis", "value"), &Sdf3dShapeCone::set_axis);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");


   ClassDB::bind_method(D_METHOD("get_angle"), &Sdf3dShapeCone::get_angle);
   ClassDB::bind_method(D_METHOD("set_angle", "value"), &Sdf3dShapeCone::set_angle);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeCone::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeCone::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeCone::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_axis"), &Sdf3dShapeCone::get_axis);
  ClassDB::bind_method(D_METHOD("set_axis", "val"), &Sdf3dShapeCone::set_axis);
  ClassDB::bind_method(D_METHOD("get_angle"), &Sdf3dShapeCone::get_angle);
  ClassDB::bind_method(D_METHOD("set_angle", "val"), &Sdf3dShapeCone::set_angle);

 }



