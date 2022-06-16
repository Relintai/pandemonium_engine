
#include "decompose.h"


Ref<Resource> Decompose::get_input() {
 return input;
}

void Decompose::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Decompose::get_out_r() {
 return out_r;
}

void Decompose::set_out_r(const Ref<Resource> &val) {
out_r = val;
}


Ref<Resource> Decompose::get_out_g() {
 return out_g;
}

void Decompose::set_out_g(const Ref<Resource> &val) {
out_g = val;
}


Ref<Resource> Decompose::get_out_b() {
 return out_b;
}

void Decompose::set_out_b(const Ref<Resource> &val) {
out_b = val;
}


Ref<Resource> Decompose::get_out_a() {
 return out_a;
}

void Decompose::set_out_a(const Ref<Resource> &val) {
out_a = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> out_r;
 //export(Resource) ;
  Ref<Resource> out_g;
 //export(Resource) ;
  Ref<Resource> out_b;
 //export(Resource) ;
  Ref<Resource> out_a;

 void Decompose::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";

  if (!out_r) {
   out_r = MMNodeUniversalProperty.new();
   out_r.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_r.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_g) {
   out_g = MMNodeUniversalProperty.new();
   out_g.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_g.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_b) {
   out_b = MMNodeUniversalProperty.new();
   out_b.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_b.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_a) {
   out_a = MMNodeUniversalProperty.new();
   out_a.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_a.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_input_property(input);
  register_output_property(out_r);
  register_output_property(out_g);
  register_output_property(out_b);
  register_output_property(out_a);
}


 void Decompose::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(out_r);
  mm_graph_node.add_slot_texture_universal(out_g);
  mm_graph_node.add_slot_texture_universal(out_b);
  mm_graph_node.add_slot_texture_universal(out_a);
}


 void Decompose::_render(const Variant &material) {
   Ref<Image> img_r = Image.new();
   Ref<Image> img_g = Image.new();
   Ref<Image> img_b = Image.new();
   Ref<Image> img_a = Image.new();
  img_r.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  img_g.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  img_b.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  img_a.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  img_r.lock();
  img_g.lock();
  img_b.lock();
  img_a.lock();
   float w = material.image_size.x;
   float h = material.image_size.y;
   float pseed = randf() + randi();

  for (int x = 0; x < material.image_size.x; ++x) { //x in range(material.image_size.x)

   for (int y = 0; y < material.image_size.y; ++y) { //y in range(material.image_size.y)
     Vector2 uv = Vector2(x / w, y / h);
     Color c = input.get_value(uv);
    img_r.set_pixel(x, y, Color(c.r, c.r, c.r, 1));
    img_g.set_pixel(x, y, Color(c.g, c.g, c.g, 1));
    img_b.set_pixel(x, y, Color(c.b, c.b, c.b, 1));
    img_a.set_pixel(x, y, Color(c.a, c.a, c.a, c.a));
}

}

  img_r.unlock();
  img_g.unlock();
  img_b.unlock();
  img_a.unlock();
  out_r.set_value(img_r);
  out_g.set_value(img_g);
  out_b.set_value(img_b);
  out_a.set_value(img_a);
}


 Color Decompose::_get_value_for(const Vector2 &uv, const int pseed) {
  return Color();
}

}

 Decompose::Decompose() {
  input;
  out_r;
  out_g;
  out_b;
  out_a;
 }

 Decompose::~Decompose() {
 }


 static void Decompose::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Decompose::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Decompose::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_out_r"), &Decompose::get_out_r);
   ClassDB::bind_method(D_METHOD("set_out_r", "value"), &Decompose::set_out_r);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_r", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_r", "get_out_r");


   ClassDB::bind_method(D_METHOD("get_out_g"), &Decompose::get_out_g);
   ClassDB::bind_method(D_METHOD("set_out_g", "value"), &Decompose::set_out_g);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_g", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_g", "get_out_g");


   ClassDB::bind_method(D_METHOD("get_out_b"), &Decompose::get_out_b);
   ClassDB::bind_method(D_METHOD("set_out_b", "value"), &Decompose::set_out_b);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_b", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_b", "get_out_b");


   ClassDB::bind_method(D_METHOD("get_out_a"), &Decompose::get_out_a);
   ClassDB::bind_method(D_METHOD("set_out_a", "value"), &Decompose::set_out_a);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_a", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_a", "get_out_a");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Decompose::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Decompose::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Decompose::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Decompose::_get_value_for);

 }



