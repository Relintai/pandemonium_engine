
#include "sd_tf_translate.h"


Ref<Resource> SdTfTranslate::get_output() {
 return output;
}

void SdTfTranslate::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 SdTfTranslate::get_translation() {
 return translation;
}

void SdTfTranslate::set_translation(const Vector2 &val) {
translation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 translation = Vector2(0, 0);

 void SdTfTranslate::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>    Apply    >>>";
  output.get_value_from_owner = true;
  register_input_property(output);
  register_output_property(output);
}


 void SdTfTranslate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_translation", "set_translation", "Translation", 0.01);
}


 void SdTfTranslate::_get_property_value(const Vector2 &uv) {
  return output.get_value(uv - translation, true);
}

 //a;

 Vector2 SdTfTranslate::get_translation() {
  return translation;
}


 void SdTfTranslate::set_translation(const Vector2 &val) {
  translation = val;
  emit_changed();
  output.emit_changed();
}

}

 SdTfTranslate::SdTfTranslate() {
  output;
  translation = Vector2(0, 0);
 }

 SdTfTranslate::~SdTfTranslate() {
 }


 static void SdTfTranslate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdTfTranslate::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdTfTranslate::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_translation"), &SdTfTranslate::get_translation);
   ClassDB::bind_method(D_METHOD("set_translation", "value"), &SdTfTranslate::set_translation);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "translation"), "set_translation", "get_translation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdTfTranslate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdTfTranslate::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdTfTranslate::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_translation"), &SdTfTranslate::get_translation);
  ClassDB::bind_method(D_METHOD("set_translation", "val"), &SdTfTranslate::set_translation);

 }



