
#include "circle_map.h"


Ref<Resource> CircleMap::get_image() {
 return image;
}

void CircleMap::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> CircleMap::get_input() {
 return input;
}

void CircleMap::set_input(const Ref<Resource> &val) {
input = val;
}


float CircleMap::get_radius() const {
 return radius;
}

void CircleMap::set_radius(const float val) {
radius = val;
}


int CircleMap::get_repeat() const {
 return repeat;
}

void CircleMap::set_repeat(const int val) {
repeat = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float radius = 1;
 //export(int) ;
  int repeat = 1;

 void CircleMap::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";

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


 void CircleMap::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
  mm_graph_node.add_slot_int("get_repeat", "set_repeat", "Repeat");
}


 void CircleMap::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color CircleMap::_get_value_for(const Vector2 &uv, const int pseed) {
  //$in(vec2(fract($repeat*atan($uv.y-0.5, $uv.x-0.5)*0.15915494309), min(0.99999, 2.0/$radius*length($uv-vec2(0.5)))))",;
   Vector2 nuv = Vector2(MMAlgos.fractf(repeat*atan2(uv.y - 0.5, uv.x - 0.5) * 0.15915494309), min(0.99999, 2.0 / radius * (uv - Vector2(0.5, 0.5)).length()));
  return input.get_value(nuv);
}

 //radius;

 float CircleMap::get_radius() {
  return radius;
}


 void CircleMap::set_radius(const float val) {
  radius = val;

  if (radius == 0) {
   radius = 0.000000001;
}

  set_dirty(true);
}

 //repeat;

 int CircleMap::get_repeat() {
  return repeat;
}


 void CircleMap::set_repeat(const int val) {
  repeat = val;
  set_dirty(true);
}

}

 CircleMap::CircleMap() {
  image;
  input;
  radius = 1;
  repeat = 1;
 }

 CircleMap::~CircleMap() {
 }


 static void CircleMap::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &CircleMap::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &CircleMap::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &CircleMap::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &CircleMap::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_radius"), &CircleMap::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &CircleMap::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


   ClassDB::bind_method(D_METHOD("get_repeat"), &CircleMap::get_repeat);
   ClassDB::bind_method(D_METHOD("set_repeat", "value"), &CircleMap::set_repeat);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");


  ClassDB::bind_method(D_METHOD("_init_properties"), &CircleMap::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &CircleMap::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &CircleMap::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &CircleMap::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_radius"), &CircleMap::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &CircleMap::set_radius);
  ClassDB::bind_method(D_METHOD("get_repeat"), &CircleMap::get_repeat);
  ClassDB::bind_method(D_METHOD("set_repeat", "val"), &CircleMap::set_repeat);

 }



