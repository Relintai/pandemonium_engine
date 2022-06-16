
#include "combine.h"


Ref<Resource> Combine::get_image() {
 return image;
}

void Combine::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Combine::get_input_r() {
 return input_r;
}

void Combine::set_input_r(const Ref<Resource> &val) {
input_r = val;
}


Ref<Resource> Combine::get_input_g() {
 return input_g;
}

void Combine::set_input_g(const Ref<Resource> &val) {
input_g = val;
}


Ref<Resource> Combine::get_input_b() {
 return input_b;
}

void Combine::set_input_b(const Ref<Resource> &val) {
input_b = val;
}


Ref<Resource> Combine::get_input_a() {
 return input_a;
}

void Combine::set_input_a(const Ref<Resource> &val) {
input_a = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input_r;
 //export(Resource) ;
  Ref<Resource> input_g;
 //export(Resource) ;
  Ref<Resource> input_b;
 //export(Resource) ;
  Ref<Resource> input_a;

 void Combine::_init_properties() {

  if (!input_r) {
   input_r = MMNodeUniversalProperty.new();
   input_r.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input_r.set_default_value(0);
}

  input_r.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input_r.slot_name = ">>>    R    ";

  if (!input_g) {
   input_g = MMNodeUniversalProperty.new();
   input_g.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input_g.set_default_value(0);
}

  input_g.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input_g.slot_name = ">>>    G    ";

  if (!input_b) {
   input_b = MMNodeUniversalProperty.new();
   input_b.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input_b.set_default_value(0);
}

  input_b.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input_b.slot_name = ">>>    B    ";

  if (!input_a) {
   input_a = MMNodeUniversalProperty.new();
   input_a.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input_a.set_default_value(1);
}

  input_a.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input_a.slot_name = ">>>    A    ";

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  //image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  //image.force_override = true;
  register_input_property(input_r);
  register_input_property(input_g);
  register_input_property(input_b);
  register_input_property(input_a);
  register_output_property(image);
}


 void Combine::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input_r);
  mm_graph_node.add_slot_label_universal(input_g);
  mm_graph_node.add_slot_label_universal(input_b);
  mm_graph_node.add_slot_label_universal(input_a);
  mm_graph_node.add_slot_texture_universal(image);
}


 void Combine::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Combine::_get_value_for(const Vector2 &uv, const int pseed) {
   float r = input_r.get_value(uv);
   float g = input_g.get_value(uv);
   float b = input_b.get_value(uv);
   float a = input_a.get_value(uv);
  return Color(r, g, b, a);
}

}

 Combine::Combine() {
  image;
  input_r;
  input_g;
  input_b;
  input_a;
 }

 Combine::~Combine() {
 }


 static void Combine::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Combine::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Combine::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input_r"), &Combine::get_input_r);
   ClassDB::bind_method(D_METHOD("set_input_r", "value"), &Combine::set_input_r);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_r", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input_r", "get_input_r");


   ClassDB::bind_method(D_METHOD("get_input_g"), &Combine::get_input_g);
   ClassDB::bind_method(D_METHOD("set_input_g", "value"), &Combine::set_input_g);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_g", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input_g", "get_input_g");


   ClassDB::bind_method(D_METHOD("get_input_b"), &Combine::get_input_b);
   ClassDB::bind_method(D_METHOD("set_input_b", "value"), &Combine::set_input_b);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_b", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input_b", "get_input_b");


   ClassDB::bind_method(D_METHOD("get_input_a"), &Combine::get_input_a);
   ClassDB::bind_method(D_METHOD("set_input_a", "value"), &Combine::set_input_a);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_a", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input_a", "get_input_a");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Combine::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Combine::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Combine::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Combine::_get_value_for);

 }



