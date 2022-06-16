
#include "sd_tf_rotate.h"


Ref<Resource> SdTfRotate::get_output() {
 return output;
}

void SdTfRotate::set_output(const Ref<Resource> &val) {
output = val;
}


float SdTfRotate::get_angle() const {
 return angle;
}

void SdTfRotate::set_angle(const float val) {
angle = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float angle = 0;

 void SdTfRotate::_init_properties() {

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


 void SdTfRotate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_angle", "set_angle", "Angle", 1);
}


 void SdTfRotate::_get_property_value(const Vector2 &uv) {
  //$in(sdf2d_rotate($uv, $a*0.01745329251))",;
  return output.get_value(MMAlgos.sdf2d_rotate(uv, angle * 0.01745329251), true);
}

 //angle;

 float SdTfRotate::get_angle() {
  return angle;
}


 void SdTfRotate::set_angle(const float val) {
  angle = val;
  emit_changed();
  output.emit_changed();
}

}

 SdTfRotate::SdTfRotate() {
  output;
  angle = 0;
 }

 SdTfRotate::~SdTfRotate() {
 }


 static void SdTfRotate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdTfRotate::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdTfRotate::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_angle"), &SdTfRotate::get_angle);
   ClassDB::bind_method(D_METHOD("set_angle", "value"), &SdTfRotate::set_angle);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdTfRotate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdTfRotate::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdTfRotate::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_angle"), &SdTfRotate::get_angle);
  ClassDB::bind_method(D_METHOD("set_angle", "val"), &SdTfRotate::set_angle);

 }



