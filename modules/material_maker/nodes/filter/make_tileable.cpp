
#include "make_tileable.h"


Ref<Resource> MakeTileable::get_image() {
 return image;
}

void MakeTileable::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> MakeTileable::get_input() {
 return input;
}

void MakeTileable::set_input(const Ref<Resource> &val) {
input = val;
}


float MakeTileable::get_width() const {
 return width;
}

void MakeTileable::set_width(const float val) {
width = val;
}


int MakeTileable::get_size() const {
 return size;
}

void MakeTileable::set_size(const int val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float width = 0.1;
  int size = 0;

 void MakeTileable::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color());
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";
  register_input_property(input);
  register_output_property(image);
}


 void MakeTileable::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 0.01);
}


 void MakeTileable::_render(const Variant &material) {
  size = max(material.image_size.x, material.image_size.y);
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color MakeTileable::_get_value_for(const Vector2 &uv, const int pseed) {
  //make_tileable_$(name)($uv, 0.5*$w);
  return make_tileable(uv, 0.5 * width);
}


 float MakeTileable::get_width() {
  return width;
}


 void MakeTileable::set_width(const float val) {
  width = val;
  set_dirty(true);
}

 //----------------------;
 //make_tileable.mmg;
 //vec4 make_tileable_$(name)(vec2 uv, float w) {;
 //	vec4 a = $in(uv);
 //	vec4 b = $in(fract(uv+vec2(0.5)));
 //	float coef_ab = sin(1.57079632679*clamp((length(uv-vec2(0.5))-0.5+w)/w, 0.0, 1.0));
 //	vec4 c = $in(fract(uv+vec2(0.25)));
 //	float coef_abc = sin(1.57079632679*clamp((min(min(length(uv-vec2(0.0, 0.5)), length(uv-vec2(0.5, 0.0))), min(length(uv-vec2(1.0, 0.5)), length(uv-vec2(0.5, 1.0))))-w)/w, 0.0, 1.0));
 //	return mix(c, mix(a, b, coef_ab), coef_abc);
 //};

 Color MakeTileable::make_tileable(const Vector2 &uv, const float w) {
   Color a = input.get_value(uv);
   Color b = input.get_value(MMAlgos.fractv2(uv + Vector2(0.5, 0.5)));
   float coef_ab = sin(1.57079632679 * clamp(((uv - Vector2(0.5, 0.5)).length() - 0.5 + w) / w, 0.0, 1.0));
   Color c = input.get_value(MMAlgos.fractv2(uv + Vector2(0.25, 0.25)));
   float coef_abc = sin(1.57079632679 * clamp((min(min((uv - Vector2(0.0, 0.5)).length(), (uv - Vector2(0.5, 0.0)).length()), min((uv- Vector2(1.0, 0.5)).length(), (uv - Vector2(0.5, 1.0)).length())) - w) / w, 0.0, 1.0));
  return lerp(c, lerp(a, b, coef_ab), coef_abc);
}

}

 MakeTileable::MakeTileable() {
  image;
  input;
  width = 0.1;
  size = 0;
 }

 MakeTileable::~MakeTileable() {
 }


 static void MakeTileable::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &MakeTileable::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &MakeTileable::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &MakeTileable::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &MakeTileable::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_width"), &MakeTileable::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &MakeTileable::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


   ClassDB::bind_method(D_METHOD("get_size"), &MakeTileable::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &MakeTileable::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &MakeTileable::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &MakeTileable::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &MakeTileable::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &MakeTileable::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_width"), &MakeTileable::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &MakeTileable::set_width);
  ClassDB::bind_method(D_METHOD("make_tileable", "uv", "w"), &MakeTileable::make_tileable);

 }



