
#include "sd_tf_scale.h"


Ref<Resource> SdTfScale::get_output() {
 return output;
}

void SdTfScale::set_output(const Ref<Resource> &val) {
output = val;
}


float SdTfScale::get_scale() const {
 return scale;
}

void SdTfScale::set_scale(const float val) {
scale = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float scale = 1;

 void SdTfScale::_init_properties() {

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


 void SdTfScale::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_scale", "set_scale", "Scale", 0.01);
}


 void SdTfScale::_get_property_value(const Vector2 &uv) {
  //$in(($uv-vec2(0.5))/$s+vec2(0.5))*$s;
  return output.get_value(((uv - Vector2(0.5, 0.5)) / scale + Vector2(0.5, 0.5)), true);
}

 //scale;

 float SdTfScale::get_scale() {
  return scale;
}


 void SdTfScale::set_scale(const float val) {
  scale = val;
  emit_changed();
  output.emit_changed();
}

}

 SdTfScale::SdTfScale() {
  output;
  scale = 1;
 }

 SdTfScale::~SdTfScale() {
 }


 static void SdTfScale::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdTfScale::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdTfScale::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_scale"), &SdTfScale::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &SdTfScale::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdTfScale::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdTfScale::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdTfScale::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_scale"), &SdTfScale::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &SdTfScale::set_scale);

 }



