
#include "sdf3d_op_morph.h"


Ref<Resource> Sdf3dOpMorph::get_input1() {
 return input1;
}

void Sdf3dOpMorph::set_input1(const Ref<Resource> &val) {
input1 = val;
}


Ref<Resource> Sdf3dOpMorph::get_input2() {
 return input2;
}

void Sdf3dOpMorph::set_input2(const Ref<Resource> &val) {
input2 = val;
}


Ref<Resource> Sdf3dOpMorph::get_output() {
 return output;
}

void Sdf3dOpMorph::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dOpMorph::get_amount() const {
 return amount;
}

void Sdf3dOpMorph::set_amount(const float val) {
amount = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input1;
 //export(Resource) ;
  Ref<Resource> input2;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float amount = 0.5;

 void Sdf3dOpMorph::_init_properties() {

  if (!input1) {
   input1 = MMNodeUniversalProperty.new();
   input1.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_VECTOR2;
  input1.slot_name = ">>>   Input 1        ";

  if (!input1.is_connected("changed", self, "on_input_changed")) {
   input1.connect("changed", self, "on_input_changed");
}


  if (!input2) {
   input2 = MMNodeUniversalProperty.new();
   input2.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_VECTOR2;
  input2.slot_name = ">>>   Input 2        ";

  if (!input2.is_connected("changed", self, "on_input_changed")) {
   input2.connect("changed", self, "on_input_changed");
}


  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_input_property(input1);
  register_input_property(input2);
  register_output_property(output);
}


 void Sdf3dOpMorph::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input1);
  mm_graph_node.add_slot_label_universal(input2);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_amount", "set_amount", "Amount", 0.01);
}


 Vector2 Sdf3dOpMorph::_get_property_value_sdf3d(const Vector3 &uv3) {
   Vector2 s1 = input1.get_value_sdf3d(uv3);
   Vector2 s2 = input2.get_value_sdf3d(uv3);
  //mix($in1($uv), $in2($uv), $amount);
  return lerp(s1, s2, amount);
}

 //amount;

 float Sdf3dOpMorph::get_amount() {
  return amount;
}


 void Sdf3dOpMorph::set_amount(const float val) {
  amount = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpMorph::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpMorph::Sdf3dOpMorph() {
  input1;
  input2;
  output;
  amount = 0.5;
 }

 Sdf3dOpMorph::~Sdf3dOpMorph() {
 }


 static void Sdf3dOpMorph::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input1"), &Sdf3dOpMorph::get_input1);
   ClassDB::bind_method(D_METHOD("set_input1", "value"), &Sdf3dOpMorph::set_input1);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input1", "get_input1");


   ClassDB::bind_method(D_METHOD("get_input2"), &Sdf3dOpMorph::get_input2);
   ClassDB::bind_method(D_METHOD("set_input2", "value"), &Sdf3dOpMorph::set_input2);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input2", "get_input2");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpMorph::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpMorph::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_amount"), &Sdf3dOpMorph::get_amount);
   ClassDB::bind_method(D_METHOD("set_amount", "value"), &Sdf3dOpMorph::set_amount);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpMorph::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpMorph::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpMorph::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_amount"), &Sdf3dOpMorph::get_amount);
  ClassDB::bind_method(D_METHOD("set_amount", "val"), &Sdf3dOpMorph::set_amount);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpMorph::on_input_changed);

 }



