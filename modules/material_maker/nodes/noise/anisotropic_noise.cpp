
#include "anisotropic_noise.h"


Ref<Resource> AnisotropicNoise::get_image() {
 return image;
}

void AnisotropicNoise::set_image(const Ref<Resource> &val) {
image = val;
}


Vector2 AnisotropicNoise::get_scale() {
 return scale;
}

void AnisotropicNoise::set_scale(const Vector2 &val) {
scale = val;
}


float AnisotropicNoise::get_smoothness() const {
 return smoothness;
}

void AnisotropicNoise::set_smoothness(const float val) {
smoothness = val;
}


float AnisotropicNoise::get_interpolation() const {
 return interpolation;
}

void AnisotropicNoise::set_interpolation(const float val) {
interpolation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Vector2) ;
  Vector2 scale = Vector2(4, 256);
 //export(float) ;
  float smoothness = 1;
 //export(float) ;
  float interpolation = 1;

 void AnisotropicNoise::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void AnisotropicNoise::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale", 1);
  //, Vector2(0, 1));
  mm_graph_node.add_slot_float("get_smoothness", "set_smoothness", "Smoothness", 0.01);
  //, Vector2(0, 1));
  mm_graph_node.add_slot_float("get_interpolation", "set_interpolation", "Interpolation", 0.01);
}


 Color AnisotropicNoise::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //anisotropic($(uv), vec2($(scale_x), $(scale_y)), $(seed), $(smoothness), $(interpolation));
  return MMAlgos.anisotropicc(uv, scale, ps, smoothness, interpolation);
}


 void AnisotropicNoise::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Vector2 AnisotropicNoise::get_scale() {
  return scale;
}


 void AnisotropicNoise::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}


 float AnisotropicNoise::get_smoothness() {
  return smoothness;
}


 void AnisotropicNoise::set_smoothness(const float val) {
  smoothness = val;
  set_dirty(true);
}


 float AnisotropicNoise::get_interpolation() {
  return interpolation;
}


 void AnisotropicNoise::set_interpolation(const float val) {
  interpolation = val;
  set_dirty(true);
}

}

 AnisotropicNoise::AnisotropicNoise() {
  image;
  scale = Vector2(4, 256);
  smoothness = 1;
  interpolation = 1;
 }

 AnisotropicNoise::~AnisotropicNoise() {
 }


 static void AnisotropicNoise::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &AnisotropicNoise::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &AnisotropicNoise::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_scale"), &AnisotropicNoise::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &AnisotropicNoise::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


   ClassDB::bind_method(D_METHOD("get_smoothness"), &AnisotropicNoise::get_smoothness);
   ClassDB::bind_method(D_METHOD("set_smoothness", "value"), &AnisotropicNoise::set_smoothness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "smoothness"), "set_smoothness", "get_smoothness");


   ClassDB::bind_method(D_METHOD("get_interpolation"), &AnisotropicNoise::get_interpolation);
   ClassDB::bind_method(D_METHOD("set_interpolation", "value"), &AnisotropicNoise::set_interpolation);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "interpolation"), "set_interpolation", "get_interpolation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &AnisotropicNoise::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &AnisotropicNoise::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &AnisotropicNoise::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &AnisotropicNoise::_render);
  ClassDB::bind_method(D_METHOD("get_scale"), &AnisotropicNoise::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &AnisotropicNoise::set_scale);
  ClassDB::bind_method(D_METHOD("get_smoothness"), &AnisotropicNoise::get_smoothness);
  ClassDB::bind_method(D_METHOD("set_smoothness", "val"), &AnisotropicNoise::set_smoothness);
  ClassDB::bind_method(D_METHOD("get_interpolation"), &AnisotropicNoise::get_interpolation);
  ClassDB::bind_method(D_METHOD("set_interpolation", "val"), &AnisotropicNoise::set_interpolation);

 }



