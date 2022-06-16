
#include "sdf3d_op_smooth_bool.h"


Ref<Resource> Sdf3dOpSmoothBool::get_input1() {
 return input1;
}

void Sdf3dOpSmoothBool::set_input1(const Ref<Resource> &val) {
input1 = val;
}


Ref<Resource> Sdf3dOpSmoothBool::get_input2() {
 return input2;
}

void Sdf3dOpSmoothBool::set_input2(const Ref<Resource> &val) {
input2 = val;
}


Ref<Resource> Sdf3dOpSmoothBool::get_output() {
 return output;
}

void Sdf3dOpSmoothBool::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dOpSmoothBool::get_operation() const {
 return operation;
}

void Sdf3dOpSmoothBool::set_operation(const int val) {
operation = val;
}


float Sdf3dOpSmoothBool::get_smoothness() const {
 return smoothness;
}

void Sdf3dOpSmoothBool::set_smoothness(const float val) {
smoothness = val;
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
 //export(float) ;
  float smoothness = 0.15;

 void Sdf3dOpSmoothBool::_init_properties() {

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


 void Sdf3dOpSmoothBool::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input1);
  mm_graph_node.add_slot_label_universal(input2);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_enum("get_operation", "set_operation", "Operation", [ "Union", "Substraction", "Intersection" ]);
  mm_graph_node.add_slot_float("get_smoothness", "set_smoothness", "Smoothness", 0.01);
}


 Vector2 Sdf3dOpSmoothBool::_get_property_value_sdf3d(const Vector3 &uv3) {
   Vector2 s1 = input1.get_value_sdf3d(uv3);
   Vector2 s2 = input2.get_value_sdf3d(uv3);

  if (operation == 0) {
   return MMAlgos.sdf3d_smooth_union(s1, s2, smoothness);
}


  else if (operation == 1) {
   return MMAlgos.sdf3d_smooth_subtraction(s1, s2, smoothness);
}


  else if (operation == 2) {
   return MMAlgos.sdf3d_smooth_intersection(s1, s2, smoothness);
}

  return Vector2();
}

 //operation;

 int Sdf3dOpSmoothBool::get_operation() {
  return operation;
}


 void Sdf3dOpSmoothBool::set_operation(const int val) {
  operation = val;
  emit_changed();
  output.emit_changed();
}

 //smoothness;

 float Sdf3dOpSmoothBool::get_smoothness() {
  return smoothness;
}


 void Sdf3dOpSmoothBool::set_smoothness(const float val) {
  smoothness = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpSmoothBool::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpSmoothBool::Sdf3dOpSmoothBool() {
  input1;
  input2;
  output;
  operation = 0;
  smoothness = 0.15;
 }

 Sdf3dOpSmoothBool::~Sdf3dOpSmoothBool() {
 }


 static void Sdf3dOpSmoothBool::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input1"), &Sdf3dOpSmoothBool::get_input1);
   ClassDB::bind_method(D_METHOD("set_input1", "value"), &Sdf3dOpSmoothBool::set_input1);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input1", "get_input1");


   ClassDB::bind_method(D_METHOD("get_input2"), &Sdf3dOpSmoothBool::get_input2);
   ClassDB::bind_method(D_METHOD("set_input2", "value"), &Sdf3dOpSmoothBool::set_input2);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input2", "get_input2");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpSmoothBool::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpSmoothBool::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_operation"), &Sdf3dOpSmoothBool::get_operation);
   ClassDB::bind_method(D_METHOD("set_operation", "value"), &Sdf3dOpSmoothBool::set_operation);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "operation"), "set_operation", "get_operation");


   ClassDB::bind_method(D_METHOD("get_smoothness"), &Sdf3dOpSmoothBool::get_smoothness);
   ClassDB::bind_method(D_METHOD("set_smoothness", "value"), &Sdf3dOpSmoothBool::set_smoothness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "smoothness"), "set_smoothness", "get_smoothness");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpSmoothBool::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpSmoothBool::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpSmoothBool::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_operation"), &Sdf3dOpSmoothBool::get_operation);
  ClassDB::bind_method(D_METHOD("set_operation", "val"), &Sdf3dOpSmoothBool::set_operation);
  ClassDB::bind_method(D_METHOD("get_smoothness"), &Sdf3dOpSmoothBool::get_smoothness);
  ClassDB::bind_method(D_METHOD("set_smoothness", "val"), &Sdf3dOpSmoothBool::set_smoothness);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpSmoothBool::on_input_changed);

 }



