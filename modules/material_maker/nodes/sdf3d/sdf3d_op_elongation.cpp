
#include "sdf3d_op_elongation.h"


Ref<Resource> Sdf3dOpElongation::get_input() {
 return input;
}

void Sdf3dOpElongation::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpElongation::get_output() {
 return output;
}

void Sdf3dOpElongation::set_output(const Ref<Resource> &val) {
output = val;
}


Vector3 Sdf3dOpElongation::get_length() {
 return length;
}

void Sdf3dOpElongation::set_length(const Vector3 &val) {
length = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector3) ;
  Vector3 length = Vector3(0.2, 0, 0);

 void Sdf3dOpElongation::_init_properties() {

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


 void Sdf3dOpElongation::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector3("get_length", "set_length", "Length", 0.01);
}


 Vector2 Sdf3dOpElongation::_get_property_value_sdf3d(const Vector3 &uv3) {
  //$in($uv - clamp($uv, -abs(vec3($x, $y, $z)), abs(vec3($x, $y, $z))));
   Vector3 new_uv = uv3 - MMAlgos.clampv3(uv3, -MMAlgos.absv3(length), MMAlgos.absv3(length));
  return input.get_value_sdf3d(new_uv);
}

 //length;

 Vector3 Sdf3dOpElongation::get_length() {
  return length;
}


 void Sdf3dOpElongation::set_length(const Vector3 &val) {
  length = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpElongation::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpElongation::Sdf3dOpElongation() {
  input;
  output;
  length = Vector3(0.2, 0, 0);
 }

 Sdf3dOpElongation::~Sdf3dOpElongation() {
 }


 static void Sdf3dOpElongation::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpElongation::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpElongation::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpElongation::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpElongation::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dOpElongation::get_length);
   ClassDB::bind_method(D_METHOD("set_length", "value"), &Sdf3dOpElongation::set_length);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "length"), "set_length", "get_length");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpElongation::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpElongation::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpElongation::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dOpElongation::get_length);
  ClassDB::bind_method(D_METHOD("set_length", "val"), &Sdf3dOpElongation::set_length);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpElongation::on_input_changed);

 }



