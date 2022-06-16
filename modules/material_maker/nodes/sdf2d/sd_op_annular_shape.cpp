
#include "sd_op_annular_shape.h"


Ref<Resource> SdOpAnnularShape::get_output() {
 return output;
}

void SdOpAnnularShape::set_output(const Ref<Resource> &val) {
output = val;
}


float SdOpAnnularShape::get_width() const {
 return width;
}

void SdOpAnnularShape::set_width(const float val) {
width = val;
}


int SdOpAnnularShape::get_ripples() const {
 return ripples;
}

void SdOpAnnularShape::set_ripples(const int val) {
ripples = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float width = 0.1;
 //export(int) ;
  int ripples = 1;

 void SdOpAnnularShape::_init_properties() {

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


 void SdOpAnnularShape::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 0.01);
  mm_graph_node.add_slot_int("get_ripples", "set_ripples", "Ripples");
}


 void SdOpAnnularShape::_get_property_value(const Vector2 &uv) {
   float val = output.get_value(uv, true);
  return MMAlgos.sdRipples(val, width, ripples);
}

 //width;

 float SdOpAnnularShape::get_width() {
  return width;
}


 void SdOpAnnularShape::set_width(const float val) {
  width = val;
  emit_changed();
  output.emit_changed();
}

 //ripples;

 int SdOpAnnularShape::get_ripples() {
  return ripples;
}


 void SdOpAnnularShape::set_ripples(const int val) {
  ripples = val;
  emit_changed();
  output.emit_changed();
}

}

 SdOpAnnularShape::SdOpAnnularShape() {
  output;
  width = 0.1;
  ripples = 1;
 }

 SdOpAnnularShape::~SdOpAnnularShape() {
 }


 static void SdOpAnnularShape::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdOpAnnularShape::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpAnnularShape::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_width"), &SdOpAnnularShape::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &SdOpAnnularShape::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


   ClassDB::bind_method(D_METHOD("get_ripples"), &SdOpAnnularShape::get_ripples);
   ClassDB::bind_method(D_METHOD("set_ripples", "value"), &SdOpAnnularShape::set_ripples);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "ripples"), "set_ripples", "get_ripples");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpAnnularShape::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpAnnularShape::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpAnnularShape::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_width"), &SdOpAnnularShape::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &SdOpAnnularShape::set_width);
  ClassDB::bind_method(D_METHOD("get_ripples"), &SdOpAnnularShape::get_ripples);
  ClassDB::bind_method(D_METHOD("set_ripples", "val"), &SdOpAnnularShape::set_ripples);

 }



