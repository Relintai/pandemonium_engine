
#include "brightness_contrast.h"


Ref<Resource> BrightnessContrast::get_image() {
 return image;
}

void BrightnessContrast::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> BrightnessContrast::get_input() {
 return input;
}

void BrightnessContrast::set_input(const Ref<Resource> &val) {
input = val;
}


float BrightnessContrast::get_brightness() const {
 return brightness;
}

void BrightnessContrast::set_brightness(const float val) {
brightness = val;
}


float BrightnessContrast::get_contrast() const {
 return contrast;
}

void BrightnessContrast::set_contrast(const float val) {
contrast = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float brightness = 0;
 //export(float) ;
  float contrast = 1;

 void BrightnessContrast::_init_properties() {

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


 void BrightnessContrast::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_brightness", "set_brightness", "Brightness", 0.01);
  mm_graph_node.add_slot_float("get_contrast", "set_contrast", "Contrast", 0.01);
}


 void BrightnessContrast::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color BrightnessContrast::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  return MMAlgos.brightness_contrast(c, brightness, contrast);
}

 //brightness;

 float BrightnessContrast::get_brightness() {
  return brightness;
}


 void BrightnessContrast::set_brightness(const float val) {
  brightness = val;
  set_dirty(true);
}

 //contrast;

 float BrightnessContrast::get_contrast() {
  return contrast;
}


 void BrightnessContrast::set_contrast(const float val) {
  contrast = val;
  set_dirty(true);
}

}

 BrightnessContrast::BrightnessContrast() {
  image;
  input;
  brightness = 0;
  contrast = 1;
 }

 BrightnessContrast::~BrightnessContrast() {
 }


 static void BrightnessContrast::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &BrightnessContrast::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &BrightnessContrast::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &BrightnessContrast::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &BrightnessContrast::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_brightness"), &BrightnessContrast::get_brightness);
   ClassDB::bind_method(D_METHOD("set_brightness", "value"), &BrightnessContrast::set_brightness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "brightness"), "set_brightness", "get_brightness");


   ClassDB::bind_method(D_METHOD("get_contrast"), &BrightnessContrast::get_contrast);
   ClassDB::bind_method(D_METHOD("set_contrast", "value"), &BrightnessContrast::set_contrast);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "contrast"), "set_contrast", "get_contrast");


  ClassDB::bind_method(D_METHOD("_init_properties"), &BrightnessContrast::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &BrightnessContrast::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &BrightnessContrast::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &BrightnessContrast::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_brightness"), &BrightnessContrast::get_brightness);
  ClassDB::bind_method(D_METHOD("set_brightness", "val"), &BrightnessContrast::set_brightness);
  ClassDB::bind_method(D_METHOD("get_contrast"), &BrightnessContrast::get_contrast);
  ClassDB::bind_method(D_METHOD("set_contrast", "val"), &BrightnessContrast::set_contrast);

 }



