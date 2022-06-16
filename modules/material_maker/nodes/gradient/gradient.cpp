
#include "gradient.h"


Ref<Resource> Gradient::get_image() {
 return image;
}

void Gradient::set_image(const Ref<Resource> &val) {
image = val;
}


float Gradient::get_repeat() const {
 return repeat;
}

void Gradient::set_repeat(const float val) {
repeat = val;
}


float Gradient::get_rotate() const {
 return rotate;
}

void Gradient::set_rotate(const float val) {
rotate = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(float) ;
  float repeat = 1;
 //export(float) ;
  float rotate = 0;

 void Gradient::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Gradient::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_repeat", "set_repeat", "repeat");
  mm_graph_node.add_slot_float("get_rotate", "set_rotate", "rotate");
  mm_graph_node.add_slot_gradient();
}


 void Gradient::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Gradient::_get_value_for(const Vector2 &uv, const int pseed) {

  if (interpolation_type == 0) {
   return MMAlgos.normal_gradient_type_1(uv, repeat, rotate, points);
}


  else if (interpolation_type == 1) {
   return MMAlgos.normal_gradient_type_2(uv, repeat, rotate, points);
}


  else if (interpolation_type == 2) {
   return MMAlgos.normal_gradient_type_3(uv, repeat, rotate, points);
}


  else if (interpolation_type == 3) {
   return MMAlgos.normal_gradient_type_4(uv, repeat, rotate, points);
}

  return Color(1, 1, 1, 1);
}


 Color Gradient::_get_gradient_color(const float x) {

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


 float Gradient::get_repeat() {
  return repeat;
}


 void Gradient::set_repeat(const float val) {
  repeat = val;
  set_dirty(true);
}


 float Gradient::get_rotate() {
  return rotate;
}


 void Gradient::set_rotate(const float val) {
  rotate = val;
  set_dirty(true);
}

}

 Gradient::Gradient() {
  image;
  repeat = 1;
  rotate = 0;
 }

 Gradient::~Gradient() {
 }


 static void Gradient::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Gradient::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Gradient::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_repeat"), &Gradient::get_repeat);
   ClassDB::bind_method(D_METHOD("set_repeat", "value"), &Gradient::set_repeat);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "repeat"), "set_repeat", "get_repeat");


   ClassDB::bind_method(D_METHOD("get_rotate"), &Gradient::get_rotate);
   ClassDB::bind_method(D_METHOD("set_rotate", "value"), &Gradient::set_rotate);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotate"), "set_rotate", "get_rotate");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Gradient::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Gradient::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Gradient::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Gradient::_get_value_for);
  ClassDB::bind_method(D_METHOD("_get_gradient_color", "x"), &Gradient::_get_gradient_color);
  ClassDB::bind_method(D_METHOD("get_repeat"), &Gradient::get_repeat);
  ClassDB::bind_method(D_METHOD("set_repeat", "val"), &Gradient::set_repeat);
  ClassDB::bind_method(D_METHOD("get_rotate"), &Gradient::get_rotate);
  ClassDB::bind_method(D_METHOD("set_rotate", "val"), &Gradient::set_rotate);

 }



