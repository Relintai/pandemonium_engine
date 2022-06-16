
#include "sdf3d_op_circle_repeat.h"


Ref<Resource> Sdf3dOpCircleRepeat::get_input() {
 return input;
}

void Sdf3dOpCircleRepeat::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpCircleRepeat::get_output() {
 return output;
}

void Sdf3dOpCircleRepeat::set_output(const Ref<Resource> &val) {
output = val;
}


int Sdf3dOpCircleRepeat::get_count() const {
 return count;
}

void Sdf3dOpCircleRepeat::set_count(const int val) {
count = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int) ;
  int count = 5;

 void Sdf3dOpCircleRepeat::_init_properties() {

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


 void Sdf3dOpCircleRepeat::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_int("get_count", "set_count", "Count");
}


 Vector2 Sdf3dOpCircleRepeat::_get_property_value_sdf3d(const Vector3 &uv3) {
  //todo make seed a class variable probably into MMNode;
   Vector3 new_uv = MMAlgos.circle_repeat_transform(uv3, count);
  return input.get_value_sdf3d(new_uv);
}

 //count;

 int Sdf3dOpCircleRepeat::get_count() {
  return count;
}


 void Sdf3dOpCircleRepeat::set_count(const int val) {
  count = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpCircleRepeat::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpCircleRepeat::Sdf3dOpCircleRepeat() {
  input;
  output;
  count = 5;
 }

 Sdf3dOpCircleRepeat::~Sdf3dOpCircleRepeat() {
 }


 static void Sdf3dOpCircleRepeat::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpCircleRepeat::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpCircleRepeat::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpCircleRepeat::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpCircleRepeat::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_count"), &Sdf3dOpCircleRepeat::get_count);
   ClassDB::bind_method(D_METHOD("set_count", "value"), &Sdf3dOpCircleRepeat::set_count);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpCircleRepeat::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpCircleRepeat::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpCircleRepeat::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_count"), &Sdf3dOpCircleRepeat::get_count);
  ClassDB::bind_method(D_METHOD("set_count", "val"), &Sdf3dOpCircleRepeat::set_count);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpCircleRepeat::on_input_changed);

 }



