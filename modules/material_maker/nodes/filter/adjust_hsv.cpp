
#include "adjust_hsv.h"


Ref<Resource> AdjustHsv::get_image() {
 return image;
}

void AdjustHsv::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> AdjustHsv::get_input() {
 return input;
}

void AdjustHsv::set_input(const Ref<Resource> &val) {
input = val;
}


float AdjustHsv::get_hue() const {
 return hue;
}

void AdjustHsv::set_hue(const float val) {
hue = val;
}


float AdjustHsv::get_saturation() const {
 return saturation;
}

void AdjustHsv::set_saturation(const float val) {
saturation = val;
}


float AdjustHsv::get_value() const {
 return value;
}

void AdjustHsv::set_value(const float val) {
value = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float hue = 0;
 //export(float) ;
  float saturation = 1;
 //export(float) ;
  float value = 1;

 void AdjustHsv::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  //image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  //image.force_override = true;
  register_input_property(input);
  register_output_property(image);
}


 void AdjustHsv::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_hue", "set_hue", "Hue", 0.01);
  mm_graph_node.add_slot_float("get_saturation", "set_saturation", "Saturation", 0.01);
  mm_graph_node.add_slot_float("get_value", "set_value", "Value", 0.01);
}


 void AdjustHsv::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color AdjustHsv::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  return MMAlgos.adjust_hsv(c, hue, saturation, value);
}

 //hue;

 float AdjustHsv::get_hue() {
  return hue;
}


 void AdjustHsv::set_hue(const float val) {
  hue = val;
  set_dirty(true);
}

 //saturation;

 float AdjustHsv::get_saturation() {
  return saturation;
}


 void AdjustHsv::set_saturation(const float val) {
  saturation = val;
  set_dirty(true);
}

 //value;

 float AdjustHsv::get_value() {
  return value;
}


 void AdjustHsv::set_value(const float val) {
  value = val;
  set_dirty(true);
}

}

 AdjustHsv::AdjustHsv() {
  image;
  input;
  hue = 0;
  saturation = 1;
  value = 1;
 }

 AdjustHsv::~AdjustHsv() {
 }


 static void AdjustHsv::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &AdjustHsv::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &AdjustHsv::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &AdjustHsv::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &AdjustHsv::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_hue"), &AdjustHsv::get_hue);
   ClassDB::bind_method(D_METHOD("set_hue", "value"), &AdjustHsv::set_hue);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "hue"), "set_hue", "get_hue");


   ClassDB::bind_method(D_METHOD("get_saturation"), &AdjustHsv::get_saturation);
   ClassDB::bind_method(D_METHOD("set_saturation", "value"), &AdjustHsv::set_saturation);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "saturation"), "set_saturation", "get_saturation");


   ClassDB::bind_method(D_METHOD("get_value"), &AdjustHsv::get_value);
   ClassDB::bind_method(D_METHOD("set_value", "value"), &AdjustHsv::set_value);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "value"), "set_value", "get_value");


  ClassDB::bind_method(D_METHOD("_init_properties"), &AdjustHsv::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &AdjustHsv::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &AdjustHsv::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &AdjustHsv::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_hue"), &AdjustHsv::get_hue);
  ClassDB::bind_method(D_METHOD("set_hue", "val"), &AdjustHsv::set_hue);
  ClassDB::bind_method(D_METHOD("get_saturation"), &AdjustHsv::get_saturation);
  ClassDB::bind_method(D_METHOD("set_saturation", "val"), &AdjustHsv::set_saturation);
  ClassDB::bind_method(D_METHOD("get_value"), &AdjustHsv::get_value);
  ClassDB::bind_method(D_METHOD("set_value", "val"), &AdjustHsv::set_value);

 }



