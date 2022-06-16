
#include "sd_op_rounded_shape.h"


Ref<Resource> SdOpRoundedShape::get_output() {
 return output;
}

void SdOpRoundedShape::set_output(const Ref<Resource> &val) {
output = val;
}


float SdOpRoundedShape::get_radius() const {
 return radius;
}

void SdOpRoundedShape::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float radius = 0;

 void SdOpRoundedShape::_init_properties() {

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


 void SdOpRoundedShape::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 void SdOpRoundedShape::_get_property_value(const Vector2 &uv) {
   float val = output.get_value(uv, true);
  return MMAlgos.sdf_rounded_shape(val, radius);
}

 //radius;

 float SdOpRoundedShape::get_radius() {
  return radius;
}


 void SdOpRoundedShape::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

}

 SdOpRoundedShape::SdOpRoundedShape() {
  output;
  radius = 0;
 }

 SdOpRoundedShape::~SdOpRoundedShape() {
 }


 static void SdOpRoundedShape::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdOpRoundedShape::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpRoundedShape::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_radius"), &SdOpRoundedShape::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &SdOpRoundedShape::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpRoundedShape::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpRoundedShape::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpRoundedShape::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_radius"), &SdOpRoundedShape::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &SdOpRoundedShape::set_radius);

 }



