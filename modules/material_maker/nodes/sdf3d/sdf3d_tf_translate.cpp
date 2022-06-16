
#include "sdf3d_tf_translate.h"


Ref<Resource> Sdf3dTfTranslate::get_input() {
 return input;
}

void Sdf3dTfTranslate::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dTfTranslate::get_output() {
 return output;
}

void Sdf3dTfTranslate::set_output(const Ref<Resource> &val) {
output = val;
}


Vector3 Sdf3dTfTranslate::get_translation() {
 return translation;
}

void Sdf3dTfTranslate::set_translation(const Vector3 &val) {
translation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector3) ;
  Vector3 translation = Vector3();

 void Sdf3dTfTranslate::_init_properties() {

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


 void Sdf3dTfTranslate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector3("get_translation", "set_translation", "Translation", 0.01);
}


 Vector2 Sdf3dTfTranslate::_get_property_value_sdf3d(const Vector3 &uv3) {
  //$in($uv-vec3($x, $y, $z));
  return input.get_value_sdf3d(uv3 - translation);
}

 //translation;

 Vector3 Sdf3dTfTranslate::get_translation() {
  return translation;
}


 void Sdf3dTfTranslate::set_translation(const Vector3 &val) {
  translation = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dTfTranslate::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dTfTranslate::Sdf3dTfTranslate() {
  input;
  output;
  translation = Vector3();
 }

 Sdf3dTfTranslate::~Sdf3dTfTranslate() {
 }


 static void Sdf3dTfTranslate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dTfTranslate::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dTfTranslate::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dTfTranslate::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dTfTranslate::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_translation"), &Sdf3dTfTranslate::get_translation);
   ClassDB::bind_method(D_METHOD("set_translation", "value"), &Sdf3dTfTranslate::set_translation);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "translation"), "set_translation", "get_translation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dTfTranslate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dTfTranslate::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dTfTranslate::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_translation"), &Sdf3dTfTranslate::get_translation);
  ClassDB::bind_method(D_METHOD("set_translation", "val"), &Sdf3dTfTranslate::set_translation);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dTfTranslate::on_input_changed);

 }



