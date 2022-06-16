
#include "rotate.h"


Ref<Resource> Rotate::get_image() {
 return image;
}

void Rotate::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Rotate::get_input() {
 return input;
}

void Rotate::set_input(const Ref<Resource> &val) {
input = val;
}


Vector2 Rotate::get_center() {
 return center;
}

void Rotate::set_center(const Vector2 &val) {
center = val;
}


float Rotate::get_rotate() const {
 return rotate;
}

void Rotate::set_rotate(const float val) {
rotate = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Vector2) ;
  Vector2 center = Vector2();
 //export(float) ;
  float rotate = 0;

 void Rotate::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";

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


 void Rotate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_vector2("get_center", "set_center", "Center", 0.01);
  mm_graph_node.add_slot_float("get_rotate", "set_rotate", "Rotate", 0.1);
}


 void Rotate::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Rotate::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i(rotate($uv, vec2(0.5+$cx, 0.5+$cy), $rotate*0.01745329251));
  return input.get_value(MMAlgos.rotate(uv, center + Vector2(0.5, 0.5), rotate*0.01745329251));
}

 //center;

 Vector2 Rotate::get_center() {
  return center;
}


 void Rotate::set_center(const Vector2 &val) {
  center = val;
  set_dirty(true);
}

 //rotate;

 float Rotate::get_rotate() {
  return rotate;
}


 void Rotate::set_rotate(const float val) {
  rotate = val;
  set_dirty(true);
}

}

 Rotate::Rotate() {
  image;
  input;
  center = Vector2();
  rotate = 0;
 }

 Rotate::~Rotate() {
 }


 static void Rotate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Rotate::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Rotate::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Rotate::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Rotate::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_center"), &Rotate::get_center);
   ClassDB::bind_method(D_METHOD("set_center", "value"), &Rotate::set_center);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");


   ClassDB::bind_method(D_METHOD("get_rotate"), &Rotate::get_rotate);
   ClassDB::bind_method(D_METHOD("set_rotate", "value"), &Rotate::set_rotate);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotate"), "set_rotate", "get_rotate");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Rotate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Rotate::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Rotate::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Rotate::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_center"), &Rotate::get_center);
  ClassDB::bind_method(D_METHOD("set_center", "val"), &Rotate::set_center);
  ClassDB::bind_method(D_METHOD("get_rotate"), &Rotate::get_rotate);
  ClassDB::bind_method(D_METHOD("set_rotate", "val"), &Rotate::set_rotate);

 }



