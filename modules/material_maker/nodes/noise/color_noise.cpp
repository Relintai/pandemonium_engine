
#include "color_noise.h"


Ref<Resource> ColorNoise::get_image() {
 return image;
}

void ColorNoise::set_image(const Ref<Resource> &val) {
image = val;
}


int ColorNoise::get_size() const {
 return size;
}

void ColorNoise::set_size(const int val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(int) ;
  int size = 8;
 //----------------------;
 //color_noise.mmg;
 //Outputs:;
 //Output - (rgb) - Shows the noise pattern;
 //color_dots($(uv), 1.0/$(size), $(seed));
 //Inputs:;
 //size, float, default: 8, min: 2, max: 12, step: 1;

 void ColorNoise::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void ColorNoise::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_int("get_size", "set_size", "Size");
}


 Color ColorNoise::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //color_dots($(uv), 1.0/$(size), $(seed));
  return MMAlgos.noise_color(uv, float(size), ps);
}


 void ColorNoise::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 int ColorNoise::get_size() {
  return size;
}


 void ColorNoise::set_size(const int val) {
  size = val;
  set_dirty(true);
}

}

 ColorNoise::ColorNoise() {
  image;
  size = 8;
 }

 ColorNoise::~ColorNoise() {
 }


 static void ColorNoise::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &ColorNoise::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &ColorNoise::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_size"), &ColorNoise::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &ColorNoise::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &ColorNoise::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &ColorNoise::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &ColorNoise::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &ColorNoise::_render);
  ClassDB::bind_method(D_METHOD("get_size"), &ColorNoise::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &ColorNoise::set_size);

 }



