
#include "color_value.h"


Ref<Resource> ColorValue::get_image() {
 return image;
}

void ColorValue::set_image(const Ref<Resource> &val) {
image = val;
}


Vector2 ColorValue::get_scale() {
 return scale;
}

void ColorValue::set_scale(const Vector2 &val) {
scale = val;
}


int ColorValue::get_iterations() const {
 return iterations;
}

void ColorValue::set_iterations(const int val) {
iterations = val;
}


float ColorValue::get_persistence() const {
 return persistence;
}

void ColorValue::set_persistence(const float val) {
persistence = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Vector2) ;
  Vector2 scale = Vector2(4, 4);
 //export(int) ;
  int iterations = 3;
 //export(float) ;
  float persistence = 0.5;

 void ColorValue::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void ColorValue::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_int("get_iterations", "set_iterations", "Iterations");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_float("get_persistence", "set_persistence", "Persistence", 0.01);
}


 Color ColorValue::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //perlin_color($(uv), vec2($(scale_x), $(scale_y)), int($(iterations)), $(persistence), $(seed));
  return MMAlgos.perlin_colorc(uv, scale, iterations, persistence, ps);
}


 void ColorValue::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Vector2 ColorValue::get_scale() {
  return scale;
}


 void ColorValue::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}


 int ColorValue::get_iterations() {
  return iterations;
}


 void ColorValue::set_iterations(const int val) {
  iterations = val;
  set_dirty(true);
}


 float ColorValue::get_persistence() {
  return persistence;
}


 void ColorValue::set_persistence(const float val) {
  persistence = val;
  set_dirty(true);
}

}

 ColorValue::ColorValue() {
  image;
  scale = Vector2(4, 4);
  iterations = 3;
  persistence = 0.5;
 }

 ColorValue::~ColorValue() {
 }


 static void ColorValue::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &ColorValue::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &ColorValue::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_scale"), &ColorValue::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &ColorValue::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


   ClassDB::bind_method(D_METHOD("get_iterations"), &ColorValue::get_iterations);
   ClassDB::bind_method(D_METHOD("set_iterations", "value"), &ColorValue::set_iterations);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "iterations"), "set_iterations", "get_iterations");


   ClassDB::bind_method(D_METHOD("get_persistence"), &ColorValue::get_persistence);
   ClassDB::bind_method(D_METHOD("set_persistence", "value"), &ColorValue::set_persistence);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "persistence"), "set_persistence", "get_persistence");


  ClassDB::bind_method(D_METHOD("_init_properties"), &ColorValue::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &ColorValue::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &ColorValue::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &ColorValue::_render);
  ClassDB::bind_method(D_METHOD("get_scale"), &ColorValue::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &ColorValue::set_scale);
  ClassDB::bind_method(D_METHOD("get_iterations"), &ColorValue::get_iterations);
  ClassDB::bind_method(D_METHOD("set_iterations", "val"), &ColorValue::set_iterations);
  ClassDB::bind_method(D_METHOD("get_persistence"), &ColorValue::get_persistence);
  ClassDB::bind_method(D_METHOD("set_persistence", "val"), &ColorValue::set_persistence);

 }



