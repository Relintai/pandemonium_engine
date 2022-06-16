
#include "shear.h"


Ref<Resource> Shear::get_image() {
 return image;
}

void Shear::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Shear::get_input() {
 return input;
}

void Shear::set_input(const Ref<Resource> &val) {
input = val;
}


int Shear::get_direction() const {
 return direction;
}

void Shear::set_direction(const int val) {
direction = val;
}


float Shear::get_amount() const {
 return amount;
}

void Shear::set_amount(const float val) {
amount = val;
}


float Shear::get_center() const {
 return center;
}

void Shear::set_center(const float val) {
center = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "Horizontal,Vertical") ;
  int direction = 0;
 //export(float) ;
  float amount = 1;
 //export(float) ;
  float center = 0;

 void Shear::_init_properties() {

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


 void Shear::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_direction", "set_direction", "Direction", [ "Horizontal", "Vertical" ]);
  mm_graph_node.add_slot_float("get_amount", "set_amount", "Amount", 0.01);
  mm_graph_node.add_slot_float("get_center", "set_center", "Center", 0.01);
}


 void Shear::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Shear::_get_value_for(const Vector2 &uv, const int pseed) {
  //$in($uv+$amount*($uv.yx-vec2($center))*vec2($direction));

  if (direction == 0) {
   return input.get_value(uv + amount * (Vector2(uv.y, uv.x) - Vector2(center, center)) * Vector2(1, 0));
}


  else if (direction == 1) {
   return input.get_value(uv + amount * (Vector2(uv.y, uv.x) - Vector2(center, center)) * Vector2(0, 1));
}

  return Color(0, 0, 0, 1);
}

 //direction;

 int Shear::get_direction() {
  return direction;
}


 void Shear::set_direction(const int val) {
  direction = val;
  set_dirty(true);
}

 //amount;

 float Shear::get_amount() {
  return amount;
}


 void Shear::set_amount(const float val) {
  amount = val;
  set_dirty(true);
}

 //center;

 float Shear::get_center() {
  return center;
}


 void Shear::set_center(const float val) {
  center = val;
  set_dirty(true);
}

}

 Shear::Shear() {
  image;
  input;
  direction = 0;
  amount = 1;
  center = 0;
 }

 Shear::~Shear() {
 }


 static void Shear::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Shear::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Shear::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Shear::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Shear::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_direction"), &Shear::get_direction);
   ClassDB::bind_method(D_METHOD("set_direction", "value"), &Shear::set_direction);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");


   ClassDB::bind_method(D_METHOD("get_amount"), &Shear::get_amount);
   ClassDB::bind_method(D_METHOD("set_amount", "value"), &Shear::set_amount);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");


   ClassDB::bind_method(D_METHOD("get_center"), &Shear::get_center);
   ClassDB::bind_method(D_METHOD("set_center", "value"), &Shear::set_center);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "center"), "set_center", "get_center");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Shear::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Shear::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Shear::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Shear::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_direction"), &Shear::get_direction);
  ClassDB::bind_method(D_METHOD("set_direction", "val"), &Shear::set_direction);
  ClassDB::bind_method(D_METHOD("get_amount"), &Shear::get_amount);
  ClassDB::bind_method(D_METHOD("set_amount", "val"), &Shear::set_amount);
  ClassDB::bind_method(D_METHOD("get_center"), &Shear::get_center);
  ClassDB::bind_method(D_METHOD("set_center", "val"), &Shear::set_center);

 }



