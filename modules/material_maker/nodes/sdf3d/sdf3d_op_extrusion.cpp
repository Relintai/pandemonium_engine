
#include "sdf3d_op_extrusion.h"


Ref<Resource> Sdf3dOpExtrusion::get_input() {
 return input;
}

void Sdf3dOpExtrusion::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dOpExtrusion::get_output() {
 return output;
}

void Sdf3dOpExtrusion::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dOpExtrusion::get_length() const {
 return length;
}

void Sdf3dOpExtrusion::set_length(const float val) {
length = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float length = 0.25;

 void Sdf3dOpExtrusion::_init_properties() {

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


 void Sdf3dOpExtrusion::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_length", "set_length", "Length", 0.01);
}


 Vector2 Sdf3dOpExtrusion::_get_property_value_sdf3d(const Vector3 &uv3) {
  //vec2 $(name_uv)_w = vec2($in($uv.xz+vec2(0.5)),abs($uv.y)-$d);
  //ret min(max($(name_uv)_w.x,$(name_uv)_w.y),0.0)+length(max($(name_uv)_w,0.0));
   float f = input.get_value(Vector2(uv3.x, uv3.z) + Vector2(0.5, 0.5));
   Vector2 w = Vector2(f, abs(uv3.y) - length);
   float ff = min(max(w.x,w.y),0.0) + MMAlgos.maxv2(w, Vector2()).length();
  return Vector2(ff, 0);
}

 //length;

 float Sdf3dOpExtrusion::get_length() {
  return length;
}


 void Sdf3dOpExtrusion::set_length(const float val) {
  length = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dOpExtrusion::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dOpExtrusion::Sdf3dOpExtrusion() {
  input;
  output;
  length = 0.25;
 }

 Sdf3dOpExtrusion::~Sdf3dOpExtrusion() {
 }


 static void Sdf3dOpExtrusion::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpExtrusion::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpExtrusion::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpExtrusion::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpExtrusion::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dOpExtrusion::get_length);
   ClassDB::bind_method(D_METHOD("set_length", "value"), &Sdf3dOpExtrusion::set_length);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dOpExtrusion::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dOpExtrusion::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dOpExtrusion::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dOpExtrusion::get_length);
  ClassDB::bind_method(D_METHOD("set_length", "val"), &Sdf3dOpExtrusion::set_length);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpExtrusion::on_input_changed);

 }



