
#include "sdf3d_color.h"


Ref<Resource> Sdf3dColor::get_input() {
 return input;
}

void Sdf3dColor::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dColor::get_output() {
 return output;
}

void Sdf3dColor::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dColor::get_color() const {
 return color;
}

void Sdf3dColor::set_color(const float val) {
color = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float color = 0.5;

 void Sdf3dColor::_init_properties() {

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


 void Sdf3dColor::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_color", "set_color", "Color", 0.01);
}


 Vector2 Sdf3dColor::_get_property_value_sdf3d(const Vector3 &uv3) {
   Vector2 v = input.get_value_sdf3d(uv3);
  v.y = color;
  return v;
}

 //color;

 float Sdf3dColor::get_color() {
  return color;
}


 void Sdf3dColor::set_color(const float val) {
  color = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dColor::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dColor::Sdf3dColor() {
  input;
  output;
  color = 0.5;
 }

 Sdf3dColor::~Sdf3dColor() {
 }


 static void Sdf3dColor::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dColor::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dColor::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dColor::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dColor::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_color"), &Sdf3dColor::get_color);
   ClassDB::bind_method(D_METHOD("set_color", "value"), &Sdf3dColor::set_color);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "color"), "set_color", "get_color");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dColor::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dColor::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dColor::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_color"), &Sdf3dColor::get_color);
  ClassDB::bind_method(D_METHOD("set_color", "val"), &Sdf3dColor::set_color);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dColor::on_input_changed);

 }



