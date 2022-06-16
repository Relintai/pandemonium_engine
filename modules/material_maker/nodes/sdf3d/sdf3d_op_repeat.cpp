
#include "sdf3d_op_repeat.h"


Ref<Resource> Sdf3dOpRepeat::get_input() {
 return input;
}

void Sdf3dOpRepeat::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpRepeat::get_output() {
 return output;
}

void Sdf3dOpRepeat::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 Sdf3dOpRepeat::get_col_row() {
 return col_row;
}

void Sdf3dOpRepeat::set_col_row(const Vector2 &val) {
col_row = val;
}


float Sdf3dOpRepeat::get_rotation() const {
 return rotation;
}

void Sdf3dOpRepeat::set_rotation(const float val) {
rotation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 col_row = Vector2(3, 3);
 //export(float) ;
  float rotation = 0.3;

 void Sdf3dOpRepeat::_init_properties() {

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


 void Sdf3dOpRepeat::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_col_row", "set_col_row", "Col,Row", 1);
  mm_graph_node.add_slot_float("get_rotation", "set_rotation", "Rotation", 0.01);
}


 Vector2 Sdf3dOpRepeat::_get_property_value_sdf3d(const Vector3 &uv3) {
  //todo make seed a class variable probably into MMNode;
   Vector3 new_uv = MMAlgos.sdf3d_repeat(uv3, col_row, rotation, 1);
  return input.get_value_sdf3d(new_uv);
}

 //col_row;

 Vector2 Sdf3dOpRepeat::get_col_row() {
  return col_row;
}


 void Sdf3dOpRepeat::set_col_row(const Vector2 &val) {
  col_row = val;
  emit_changed();
  output.emit_changed();
}

 //rotation;

 float Sdf3dOpRepeat::get_rotation() {
  return rotation;
}


 void Sdf3dOpRepeat::set_rotation(const float val) {
  rotation = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpRepeat::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpRepeat::Sdf3dOpRepeat() {
  input;
  output;
  col_row = Vector2(3, 3);
  rotation = 0.3;
 }

 Sdf3dOpRepeat::~Sdf3dOpRepeat() {
 }


 static void Sdf3dOpRepeat::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpRepeat::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpRepeat::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpRepeat::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpRepeat::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_col_row"), &Sdf3dOpRepeat::get_col_row);
   ClassDB::bind_method(D_METHOD("set_col_row", "value"), &Sdf3dOpRepeat::set_col_row);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "col_row"), "set_col_row", "get_col_row");


   ClassDB::bind_method(D_METHOD("get_rotation"), &Sdf3dOpRepeat::get_rotation);
   ClassDB::bind_method(D_METHOD("set_rotation", "value"), &Sdf3dOpRepeat::set_rotation);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation"), "set_rotation", "get_rotation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpRepeat::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpRepeat::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpRepeat::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_col_row"), &Sdf3dOpRepeat::get_col_row);
  ClassDB::bind_method(D_METHOD("set_col_row", "val"), &Sdf3dOpRepeat::set_col_row);
  ClassDB::bind_method(D_METHOD("get_rotation"), &Sdf3dOpRepeat::get_rotation);
  ClassDB::bind_method(D_METHOD("set_rotation", "val"), &Sdf3dOpRepeat::set_rotation);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpRepeat::on_input_changed);

 }



