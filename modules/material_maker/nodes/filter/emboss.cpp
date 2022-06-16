
#include "emboss.h"


Ref<Resource> Emboss::get_image() {
 return image;
}

void Emboss::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Emboss::get_input() {
 return input;
}

void Emboss::set_input(const Ref<Resource> &val) {
input = val;
}


float Emboss::get_angle() const {
 return angle;
}

void Emboss::set_angle(const float val) {
angle = val;
}


float Emboss::get_amount() const {
 return amount;
}

void Emboss::set_amount(const float val) {
amount = val;
}


float Emboss::get_width() const {
 return width;
}

void Emboss::set_width(const float val) {
width = val;
}


int Emboss::get_size() const {
 return size;
}

void Emboss::set_size(const int val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float angle = 0;
 //export(float) ;
  float amount = 5;
 //export(float) ;
  float width = 1;
  int size = 0;

 void Emboss::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input.set_default_value(1);
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";
  register_input_property(input);
  register_output_property(image);
}


 void Emboss::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_float("get_angle", "set_angle", "Angle", 0.1);
  mm_graph_node.add_slot_float("get_amount", "set_amount", "Amount", 0.1);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 1);
}


 void Emboss::_render(const Variant &material) {
  size = max(material.image_size.x, material.image_size.y);
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Emboss::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = 0;
  f = emboss(uv, size, angle, amount, width);
  return Color(f, f, f, 1);
}


 float Emboss::get_angle() {
  return angle;
}


 void Emboss::set_angle(const float val) {
  angle = val;
  set_dirty(true);
}


 float Emboss::get_amount() {
  return amount;
}


 void Emboss::set_amount(const float val) {
  amount = val;
  set_dirty(true);
}


 float Emboss::get_width() {
  return width;
}


 void Emboss::set_width(const float val) {
  width = val;
  set_dirty(true);
}

 //float $(name)_fct(vec2 uv) {;
 //	float pixels = max(1.0, $width);
 //	float e = 1.0/$size;
 //	float rv = 0.0;
 //;
 //	for (float dx = -pixels; dx <= pixels; dx += 1.0) {;
 //		for (float dy = -pixels; dy <= pixels; dy += 1.0) {;
 //			if (abs(dx) > 0.5 || abs(dy) > 0.5) {;
 //				rv += $in(uv+e*vec2(dx, dy))*cos(atan(dy, dx)-$angle*3.14159265359/180.0)/length(vec2(dx, dy));
 //			};
 //		};
 //	};
 //;
 //	return $amount*rv/pixels+0.5;
 //};

 float Emboss::emboss(const Vector2 &uv, const float psize, const float pangle, const float pamount, const float pwidth) {
   float pixels = max(1.0, pwidth);
   float e = 1.0 / psize;
   float rv = 0.0;
   float dx = -pixels;
   float dy = -pixels;
  //for (float dx = -pixels; dx <= pixels; dx += 1.0) {;

  while (dx <= pixels) {
   //for (float dy = -pixels; dy <= pixels; dy += 1.0) {;

   while (dy <= pixels) {

    if ((abs(dx) > 0.5 || abs(dy) > 0.5)) {
     rv += input.get_value(uv + e * Vector2(dx, dy)) * cos(atan2(dy, dx) - pangle * 3.14159265359 / 180.0) / Vector2(dx, dy).length();
}

    dx += 1;
    dy += 1;
}

}

  return pamount * rv / pixels + 0.5;
}

}

 Emboss::Emboss() {
  image;
  input;
  angle = 0;
  amount = 5;
  width = 1;
  size = 0;
 }

 Emboss::~Emboss() {
 }


 static void Emboss::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Emboss::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Emboss::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Emboss::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Emboss::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_angle"), &Emboss::get_angle);
   ClassDB::bind_method(D_METHOD("set_angle", "value"), &Emboss::set_angle);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");


   ClassDB::bind_method(D_METHOD("get_amount"), &Emboss::get_amount);
   ClassDB::bind_method(D_METHOD("set_amount", "value"), &Emboss::set_amount);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");


   ClassDB::bind_method(D_METHOD("get_width"), &Emboss::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &Emboss::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


   ClassDB::bind_method(D_METHOD("get_size"), &Emboss::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Emboss::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Emboss::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Emboss::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Emboss::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Emboss::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_angle"), &Emboss::get_angle);
  ClassDB::bind_method(D_METHOD("set_angle", "val"), &Emboss::set_angle);
  ClassDB::bind_method(D_METHOD("get_amount"), &Emboss::get_amount);
  ClassDB::bind_method(D_METHOD("set_amount", "val"), &Emboss::set_amount);
  ClassDB::bind_method(D_METHOD("get_width"), &Emboss::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &Emboss::set_width);
  ClassDB::bind_method(D_METHOD("emboss", "uv", "psize", "pangle", "pamount", "pwidth"), &Emboss::emboss);

 }



