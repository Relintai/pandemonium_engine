
#include "sdf3d_op_rounded.h"


Ref<Resource> Sdf3dOpRounded::get_input() {
 return input;
}

void Sdf3dOpRounded::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpRounded::get_output() {
 return output;
}

void Sdf3dOpRounded::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dOpRounded::get_radius() const {
 return radius;
}

void Sdf3dOpRounded::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float radius = 0.15;

 void Sdf3dOpRounded::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_VECTOR2;
  input.slot_name = ">>>   Input        ";

  if (!input.is_connected("changed", self, "on_input_changed")) {
   input.connect("changed", self, "on_input_changed");
}


  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_input_property(input);
  register_output_property(output);
}


 void Sdf3dOpRounded::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 Vector2 Sdf3dOpRounded::_get_property_value_sdf3d(const Vector3 &uv3) {
   Vector2 v = input.get_value_sdf3d(uv3);
  //vec2($(name_uv)_v.x-$r, $(name_uv)_v.y);
  v.x -= radius;
  return v;
}

 //radius;

 float Sdf3dOpRounded::get_radius() {
  return radius;
}


 void Sdf3dOpRounded::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpRounded::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpRounded::Sdf3dOpRounded() {
  input;
  output;
  radius = 0.15;
 }

 Sdf3dOpRounded::~Sdf3dOpRounded() {
 }


 static void Sdf3dOpRounded::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpRounded::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpRounded::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpRounded::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpRounded::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dOpRounded::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Sdf3dOpRounded::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpRounded::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpRounded::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpRounded::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dOpRounded::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &Sdf3dOpRounded::set_radius);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpRounded::on_input_changed);

 }



