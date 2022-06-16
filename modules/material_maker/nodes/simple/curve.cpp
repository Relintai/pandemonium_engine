
#include "curve.h"


Ref<Resource> Curve::get_image() {
 return image;
}

void Curve::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Curve::get_input() {
 return input;
}

void Curve::set_input(const Ref<Resource> &val) {
input = val;
}


Vector2 Curve::get_a() {
 return a;
}

void Curve::set_a(const Vector2 &val) {
a = val;
}


Vector2 Curve::get_b() {
 return b;
}

void Curve::set_b(const Vector2 &val) {
b = val;
}


Vector2 Curve::get_c() {
 return c;
}

void Curve::set_c(const Vector2 &val) {
c = val;
}


float Curve::get_width() const {
 return width;
}

void Curve::set_width(const float val) {
width = val;
}


int Curve::get_repeat() const {
 return repeat;
}

void Curve::set_repeat(const int val) {
repeat = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Vector2) ;
  Vector2 a = Vector2(-0.35, -0.2);
 //export(Vector2) ;
  Vector2 b = Vector2(0, 0.5);
 //export(Vector2) ;
  Vector2 c = Vector2(0.35, -0.2);
 //export(float) ;
  float width = 0.05;
 //export(int) ;
  int repeat = 1;

 void Curve::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input.set_default_value(1.0);
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";
  register_input_property(input);
  register_output_property(image);
}


 void Curve::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_vector2("get_a", "set_a", "A", 0.01);
  mm_graph_node.add_slot_vector2("get_b", "set_b", "B", 0.01);
  mm_graph_node.add_slot_vector2("get_c", "set_c", "C", 0.01);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 0.01);
  mm_graph_node.add_slot_int("get_repeat", "set_repeat", "Repeat");
}


 Color Curve::_get_value_for(const Vector2 &uv, const int pseed) {
   Vector2 nuv = transform_uv(uv);
   float f = 0;

  if (nuv.x != 0 && nuv.y != 0) {
   f = input.get_value(nuv);
}

  return Color(f, f, f, 1);
}


 void Curve::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Vector2 Curve::transform_uv(const Vector2 &uv) {
  //vec2 $(name_uv)_bezier = sdBezier($uv, vec2($ax+0.5, $ay+0.5), vec2($bx+0.5, $by+0.5), vec2($cx+0.5, $cy+0.5));
   Vector2 bezier = MMAlgos.sdBezier(uv, Vector2(a.x + 0.5, a.y + 0.5), Vector2(b.x + 0.5, b.y + 0.5), Vector2(c.x + 0.5, c.y + 0.5));
  //vec2 $(name_uv)_uv = vec2($(name_uv)_bezier.x, $(name_uv)_bezier.y / $width+0.5);
   Vector2 new_uv = Vector2(bezier.x, bezier.y / width + 0.5);
  //vec2 $(name_uv)_uvtest = step(vec2(0.5), abs($(name_uv)_uv-vec2(0.5)));
   Vector2 uv_test = MMAlgos.stepv2(Vector2(0.5, 0.5), MMAlgos.absv2(new_uv - Vector2(0.5, 0.5)));
  //$(name_uv)_uv = mix(vec2(fract($repeat*$(name_uv)_uv.x), $(name_uv)_uv.y), vec2(0.0), max($(name_uv)_uvtest.x, $(name_uv)_uvtest.y));
   Vector2 final_uv = lerp(Vector2(MMAlgos.fract(repeat * new_uv.x), new_uv.y), Vector2(), max(uv_test.x, uv_test.y));
  return final_uv;
}

 //b;

 Vector2 Curve::get_a() {
  return a;
}


 void Curve::set_a(const Vector2 &val) {
  a = val;
  set_dirty(true);
}

 //b;

 Vector2 Curve::get_b() {
  return b;
}


 void Curve::set_b(const Vector2 &val) {
  b = val;
  set_dirty(true);
}

 //c;

 Vector2 Curve::get_c() {
  return c;
}


 void Curve::set_c(const Vector2 &val) {
  c = val;
  set_dirty(true);
}

 //width;

 float Curve::get_width() {
  return width;
}


 void Curve::set_width(const float val) {
  width = val;
  set_dirty(true);
}

 //repeat;

 int Curve::get_repeat() {
  return repeat;
}


 void Curve::set_repeat(const int val) {
  repeat = val;
  set_dirty(true);
}

}

 Curve::Curve() {
  image;
  input;
  a = Vector2(-0.35, -0.2);
  b = Vector2(0, 0.5);
  c = Vector2(0.35, -0.2);
  width = 0.05;
  repeat = 1;
 }

 Curve::~Curve() {
 }


 static void Curve::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Curve::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Curve::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Curve::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Curve::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_a"), &Curve::get_a);
   ClassDB::bind_method(D_METHOD("set_a", "value"), &Curve::set_a);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "a"), "set_a", "get_a");


   ClassDB::bind_method(D_METHOD("get_b"), &Curve::get_b);
   ClassDB::bind_method(D_METHOD("set_b", "value"), &Curve::set_b);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "b"), "set_b", "get_b");


   ClassDB::bind_method(D_METHOD("get_c"), &Curve::get_c);
   ClassDB::bind_method(D_METHOD("set_c", "value"), &Curve::set_c);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "c"), "set_c", "get_c");


   ClassDB::bind_method(D_METHOD("get_width"), &Curve::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &Curve::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


   ClassDB::bind_method(D_METHOD("get_repeat"), &Curve::get_repeat);
   ClassDB::bind_method(D_METHOD("set_repeat", "value"), &Curve::set_repeat);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Curve::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Curve::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Curve::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Curve::_render);
  ClassDB::bind_method(D_METHOD("transform_uv", "uv"), &Curve::transform_uv);
  ClassDB::bind_method(D_METHOD("get_a"), &Curve::get_a);
  ClassDB::bind_method(D_METHOD("set_a", "val"), &Curve::set_a);
  ClassDB::bind_method(D_METHOD("get_b"), &Curve::get_b);
  ClassDB::bind_method(D_METHOD("set_b", "val"), &Curve::set_b);
  ClassDB::bind_method(D_METHOD("get_c"), &Curve::get_c);
  ClassDB::bind_method(D_METHOD("set_c", "val"), &Curve::set_c);
  ClassDB::bind_method(D_METHOD("get_width"), &Curve::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &Curve::set_width);
  ClassDB::bind_method(D_METHOD("get_repeat"), &Curve::get_repeat);
  ClassDB::bind_method(D_METHOD("set_repeat", "val"), &Curve::set_repeat);

 }



