
#include "sdf3d_op_revolution.h"


Ref<Resource> Sdf3dOpRevolution::get_input() {
 return input;
}

void Sdf3dOpRevolution::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpRevolution::get_output() {
 return output;
}

void Sdf3dOpRevolution::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dOpRevolution::get_offset() const {
 return offset;
}

void Sdf3dOpRevolution::set_offset(const float val) {
offset = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float offset = 0.25;

 void Sdf3dOpRevolution::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
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


 void Sdf3dOpRevolution::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_offset", "set_offset", "Offset", 0.01);
}


 Vector2 Sdf3dOpRevolution::_get_property_value_sdf3d(const Vector3 &uv3) {
  //vec2 $(name_uv)_q = vec2(length($uv.xy) - $d + 0.5, $uv.z + 0.5);
   Vector2 uv = Vector2(Vector2(uv3.x, uv3.y).length() - offset + 0.5, uv3.z + 0.5);
   float f = input.get_value(uv);
  return Vector2(f, 0);
}

 //offset;

 float Sdf3dOpRevolution::get_offset() {
  return offset;
}


 void Sdf3dOpRevolution::set_offset(const float val) {
  offset = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpRevolution::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpRevolution::Sdf3dOpRevolution() {
  input;
  output;
  offset = 0.25;
 }

 Sdf3dOpRevolution::~Sdf3dOpRevolution() {
 }


 static void Sdf3dOpRevolution::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpRevolution::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpRevolution::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpRevolution::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpRevolution::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_offset"), &Sdf3dOpRevolution::get_offset);
   ClassDB::bind_method(D_METHOD("set_offset", "value"), &Sdf3dOpRevolution::set_offset);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpRevolution::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpRevolution::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpRevolution::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_offset"), &Sdf3dOpRevolution::get_offset);
  ClassDB::bind_method(D_METHOD("set_offset", "val"), &Sdf3dOpRevolution::set_offset);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpRevolution::on_input_changed);

 }



