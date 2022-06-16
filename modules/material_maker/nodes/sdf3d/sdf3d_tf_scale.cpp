
#include "sdf3d_tf_scale.h"


Ref<Resource> Sdf3dTfScale::get_input() {
 return input;
}

void Sdf3dTfScale::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dTfScale::get_output() {
 return output;
}

void Sdf3dTfScale::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dTfScale::get_scale() const {
 return scale;
}

void Sdf3dTfScale::set_scale(const float val) {
scale = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float scale = 1;

 void Sdf3dTfScale::_init_properties() {

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


 void Sdf3dTfScale::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_scale", "set_scale", "Scale", 0.01);
}


 Vector2 Sdf3dTfScale::_get_property_value_sdf3d(const Vector3 &uv3) {
  //vec2 $(name_uv)_in = $in(($uv)/$s);
  return input.get_value_sdf3d(uv3 / scale);
}

 //scale;

 float Sdf3dTfScale::get_scale() {
  return scale;
}


 void Sdf3dTfScale::set_scale(const float val) {
  scale = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dTfScale::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dTfScale::Sdf3dTfScale() {
  input;
  output;
  scale = 1;
 }

 Sdf3dTfScale::~Sdf3dTfScale() {
 }


 static void Sdf3dTfScale::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dTfScale::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dTfScale::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dTfScale::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dTfScale::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_scale"), &Sdf3dTfScale::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &Sdf3dTfScale::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dTfScale::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dTfScale::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dTfScale::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_scale"), &Sdf3dTfScale::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &Sdf3dTfScale::set_scale);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dTfScale::on_input_changed);

 }



