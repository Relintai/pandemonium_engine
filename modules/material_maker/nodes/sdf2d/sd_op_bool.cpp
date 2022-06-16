
#include "sd_op_bool.h"


Ref<Resource> SdOpBool::get_input1() {
 return input1;
}

void SdOpBool::set_input1(const Ref<Resource> &val) {
input1 = val;
}


Ref<Resource> SdOpBool::get_input2() {
 return input2;
}

void SdOpBool::set_input2(const Ref<Resource> &val) {
input2 = val;
}


Ref<Resource> SdOpBool::get_output() {
 return output;
}

void SdOpBool::set_output(const Ref<Resource> &val) {
output = val;
}


int SdOpBool::get_operation() const {
 return operation;
}

void SdOpBool::set_operation(const int val) {
operation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input1;
 //export(Resource) ;
  Ref<Resource> input2;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "Union,Substraction,Intersection") ;
  int operation = 0;

 void SdOpBool::_init_properties() {

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


 void SdOpBool::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input1);
  mm_graph_node.add_slot_label_universal(input2);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_operation", "set_operation", "Operation", [ "Union", "Substraction", "Intersection" ]);
}


 float SdOpBool::_get_property_value(const Vector2 &uv) {

  if (operation == 0) {
   return MMAlgos.sdf_boolean_union(input1.get_value(uv), input2.get_value(uv));
}


  else if (operation == 1) {
   return MMAlgos.sdf_boolean_substraction(input1.get_value(uv), input2.get_value(uv));
}


  else if (operation == 2) {
   return MMAlgos.sdf_boolean_intersection(input1.get_value(uv), input2.get_value(uv));
}

  return 0.0;
}

 //operation;

 int SdOpBool::get_operation() {
  return operation;
}


 void SdOpBool::set_operation(const int val) {
  operation = val;
  emit_changed();
  output.emit_changed();
}


 void SdOpBool::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 SdOpBool::SdOpBool() {
  input1;
  input2;
  output;
  operation = 0;
 }

 SdOpBool::~SdOpBool() {
 }


 static void SdOpBool::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input1"), &SdOpBool::get_input1);
   ClassDB::bind_method(D_METHOD("set_input1", "value"), &SdOpBool::set_input1);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input1", "get_input1");


   ClassDB::bind_method(D_METHOD("get_input2"), &SdOpBool::get_input2);
   ClassDB::bind_method(D_METHOD("set_input2", "value"), &SdOpBool::set_input2);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input2", "get_input2");


   ClassDB::bind_method(D_METHOD("get_output"), &SdOpBool::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpBool::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_operation"), &SdOpBool::get_operation);
   ClassDB::bind_method(D_METHOD("set_operation", "value"), &SdOpBool::set_operation);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "operation"), "set_operation", "get_operation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpBool::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpBool::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpBool::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_operation"), &SdOpBool::get_operation);
  ClassDB::bind_method(D_METHOD("set_operation", "val"), &SdOpBool::set_operation);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &SdOpBool::on_input_changed);

 }



