
#include "radial_gradient.h"


Ref<Resource> RadialGradient::get_image() {
 return image;
}

void RadialGradient::set_image(const Ref<Resource> &val) {
image = val;
}


float RadialGradient::get_repeat() const {
 return repeat;
}

void RadialGradient::set_repeat(const float val) {
repeat = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(float) ;
  float repeat = 1;

 void RadialGradient::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void RadialGradient::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_repeat", "set_repeat", "repeat");
  mm_graph_node.add_slot_gradient();
}


 void RadialGradient::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color RadialGradient::_get_value_for(const Vector2 &uv, const int pseed) {

  if (interpolation_type == 0) {
   return MMAlgos.radial_gradient_type_1(uv, repeat, points);
}


  else if (interpolation_type == 1) {
   return MMAlgos.radial_gradient_type_2(uv, repeat, points);
}


  else if (interpolation_type == 2) {
   return MMAlgos.radial_gradient_type_3(uv, repeat, points);
}


  else if (interpolation_type == 3) {
   return MMAlgos.radial_gradient_type_4(uv, repeat, points);
}

  return Color(1, 1, 1, 1);
}


 Color RadialGradient::_get_gradient_color(const float x) {

  if (interpolation_type == 0) {
   return MMAlgos.gradient_type_1(x, points);
}


  else if (interpolation_type == 1) {
   return MMAlgos.gradient_type_2(x, points);
}


  else if (interpolation_type == 2) {
   return MMAlgos.gradient_type_3(x, points);
}


  else if (interpolation_type == 3) {
   return MMAlgos.gradient_type_4(x, points);
}

  return Color(1, 1, 1, 1);
}


 float RadialGradient::get_repeat() {
  return repeat;
}


 void RadialGradient::set_repeat(const float val) {
  repeat = val;
  set_dirty(true);
}

}

 RadialGradient::RadialGradient() {
  image;
  repeat = 1;
 }

 RadialGradient::~RadialGradient() {
 }


 static void RadialGradient::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &RadialGradient::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &RadialGradient::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_repeat"), &RadialGradient::get_repeat);
   ClassDB::bind_method(D_METHOD("set_repeat", "value"), &RadialGradient::set_repeat);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "repeat"), "set_repeat", "get_repeat");


  ClassDB::bind_method(D_METHOD("_init_properties"), &RadialGradient::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &RadialGradient::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &RadialGradient::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &RadialGradient::_get_value_for);
  ClassDB::bind_method(D_METHOD("_get_gradient_color", "x"), &RadialGradient::_get_gradient_color);
  ClassDB::bind_method(D_METHOD("get_repeat"), &RadialGradient::get_repeat);
  ClassDB::bind_method(D_METHOD("set_repeat", "val"), &RadialGradient::set_repeat);

 }



