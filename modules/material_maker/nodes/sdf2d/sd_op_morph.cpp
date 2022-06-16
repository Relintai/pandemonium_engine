
#include "sd_op_morph.h"


Ref<Resource> SdOpMorph::get_input1() {
 return input1;
}

void SdOpMorph::set_input1(const Ref<Resource> &val) {
input1 = val;
}


Ref<Resource> SdOpMorph::get_input2() {
 return input2;
}

void SdOpMorph::set_input2(const Ref<Resource> &val) {
input2 = val;
}


Ref<Resource> SdOpMorph::get_output() {
 return output;
}

void SdOpMorph::set_output(const Ref<Resource> &val) {
output = val;
}


float SdOpMorph::get_amount() const {
 return amount;
}

void SdOpMorph::set_amount(const float val) {
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

 void SdOpMorph::_init_properties() {

  if (!input1) {
   input1 = MMNodeUniversalProperty.new();
   input1.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  input1.slot_name = ">>>   Input 1       ";

  if (!input1.is_connected("changed", self, "on_input_changed")) {
   input1.connect("changed", self, "on_input_changed");
}


  if (!input2) {
   input2 = MMNodeUniversalProperty.new();
   input2.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  input2.slot_name = ">>>   Input 2       ";

  if (!input2.is_connected("changed", self, "on_input_changed")) {
   input2.connect("changed", self, "on_input_changed");
}


  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = "       Output    >>>";
  output.get_value_from_owner = true;
  register_input_property(input1);
  register_input_property(input2);
  register_output_property(output);
}


 void SdOpMorph::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input1);
  mm_graph_node.add_slot_label_universal(input2);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_amount", "set_amount", "Amount", 0.01);
}


 float SdOpMorph::_get_property_value(const Vector2 &uv) {
  return MMAlgos.sdf_morph(input1.get_value(uv), input2.get_value(uv), amount);
}

 //amount;

 float SdOpMorph::get_amount() {
  return amount;
}


 void SdOpMorph::set_amount(const float val) {
  amount = val;
  emit_changed();
  output.emit_changed();
}


 void SdOpMorph::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 SdOpMorph::SdOpMorph() {
  input1;
  input2;
  output;
  amount = 0.5;
 }

 SdOpMorph::~SdOpMorph() {
 }


 static void SdOpMorph::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input1"), &SdOpMorph::get_input1);
   ClassDB::bind_method(D_METHOD("set_input1", "value"), &SdOpMorph::set_input1);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input1", "get_input1");


   ClassDB::bind_method(D_METHOD("get_input2"), &SdOpMorph::get_input2);
   ClassDB::bind_method(D_METHOD("set_input2", "value"), &SdOpMorph::set_input2);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input2", "get_input2");


   ClassDB::bind_method(D_METHOD("get_output"), &SdOpMorph::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpMorph::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_amount"), &SdOpMorph::get_amount);
   ClassDB::bind_method(D_METHOD("set_amount", "value"), &SdOpMorph::set_amount);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpMorph::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpMorph::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpMorph::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_amount"), &SdOpMorph::get_amount);
  ClassDB::bind_method(D_METHOD("set_amount", "val"), &SdOpMorph::set_amount);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &SdOpMorph::on_input_changed);

 }



