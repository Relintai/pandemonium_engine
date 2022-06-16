
#include "swap_channels.h"


Ref<Resource> SwapChannels::get_image() {
 return image;
}

void SwapChannels::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> SwapChannels::get_input() {
 return input;
}

void SwapChannels::set_input(const Ref<Resource> &val) {
input = val;
}


int SwapChannels::get_op_r() const {
 return op_r;
}

void SwapChannels::set_op_r(const int val) {
op_r = val;
}


int SwapChannels::get_op_g() const {
 return op_g;
}

void SwapChannels::set_op_g(const int val) {
op_g = val;
}


int SwapChannels::get_op_b() const {
 return op_b;
}

void SwapChannels::set_op_b(const int val) {
op_b = val;
}


int SwapChannels::get_op_a() const {
 return op_a;
}

void SwapChannels::set_op_a(const int val) {
op_a = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") ;
  int op_r = 2;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") ;
  int op_g = 4;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") ;
  int op_b = 6;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") ;
  int op_a = 8;

 void SwapChannels::_init_properties() {

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


 void SwapChannels::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_op_r", "set_op_r", "R", [ "0", "1", "R", "-R", "G", "-G", "B", "-B", "A","-A" ]);
  mm_graph_node.add_slot_enum("get_op_g", "set_op_g", "G", [ "0", "1", "R", "-R", "G", "-G", "B", "-B", "A","-A" ]);
  mm_graph_node.add_slot_enum("get_op_b", "set_op_b", "B", [ "0", "1", "R", "-R", "G", "-G", "B", "-B", "A","-A" ]);
  mm_graph_node.add_slot_enum("get_op_a", "set_op_a", "A", [ "0", "1", "R", "-R", "G", "-G", "B", "-B", "A","-A" ]);
}


 void SwapChannels::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 float SwapChannels::apply(const int op, const Color &val) {

  if (op == 0) {
   return 0.0;
}


  else if (op == 1) {
   return 1.0;
}


  else if (op == 2) {
   return val.r;
}


  else if (op == 3) {
   return 1.0 - val.r;
}


  else if (op == 4) {
   return val.g;
}


  else if (op == 5) {
   return 1.0 - val.g;
}


  else if (op == 6) {
   return val.b;
}


  else if (op == 7) {
   return 1.0 - val.b;
}


  else if (op == 8) {
   return val.a;
}


  else if (op == 9) {
   return 1.0 - val.a;
}

  return 0.0;
}


 Color SwapChannels::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  return Color(apply(op_r, c), apply(op_g, c), apply(op_b, c), apply(op_a, c));
}

 //op_r;

 int SwapChannels::get_op_r() {
  return op_r;
}


 void SwapChannels::set_op_r(const int val) {
  op_r = val;
  set_dirty(true);
}

 //op_g;

 int SwapChannels::get_op_g() {
  return op_g;
}


 void SwapChannels::set_op_g(const int val) {
  op_g = val;
  set_dirty(true);
}

 //op_b;

 int SwapChannels::get_op_b() {
  return op_b;
}


 void SwapChannels::set_op_b(const int val) {
  op_b = val;
  set_dirty(true);
}

 //op_a;

 int SwapChannels::get_op_a() {
  return op_a;
}


 void SwapChannels::set_op_a(const int val) {
  op_a = val;
  set_dirty(true);
}

}

 SwapChannels::SwapChannels() {
  image;
  input;
  op_r = 2;
  op_g = 4;
  op_b = 6;
  op_a = 8;
 }

 SwapChannels::~SwapChannels() {
 }


 static void SwapChannels::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &SwapChannels::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &SwapChannels::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &SwapChannels::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &SwapChannels::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_op_r"), &SwapChannels::get_op_r);
   ClassDB::bind_method(D_METHOD("set_op_r", "value"), &SwapChannels::set_op_r);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "op_r"), "set_op_r", "get_op_r");


   ClassDB::bind_method(D_METHOD("get_op_g"), &SwapChannels::get_op_g);
   ClassDB::bind_method(D_METHOD("set_op_g", "value"), &SwapChannels::set_op_g);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "op_g"), "set_op_g", "get_op_g");


   ClassDB::bind_method(D_METHOD("get_op_b"), &SwapChannels::get_op_b);
   ClassDB::bind_method(D_METHOD("set_op_b", "value"), &SwapChannels::set_op_b);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "op_b"), "set_op_b", "get_op_b");


   ClassDB::bind_method(D_METHOD("get_op_a"), &SwapChannels::get_op_a);
   ClassDB::bind_method(D_METHOD("set_op_a", "value"), &SwapChannels::set_op_a);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "op_a"), "set_op_a", "get_op_a");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SwapChannels::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SwapChannels::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &SwapChannels::_render);
  ClassDB::bind_method(D_METHOD("apply", "op", "val"), &SwapChannels::apply);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &SwapChannels::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_op_r"), &SwapChannels::get_op_r);
  ClassDB::bind_method(D_METHOD("set_op_r", "val"), &SwapChannels::set_op_r);
  ClassDB::bind_method(D_METHOD("get_op_g"), &SwapChannels::get_op_g);
  ClassDB::bind_method(D_METHOD("set_op_g", "val"), &SwapChannels::set_op_g);
  ClassDB::bind_method(D_METHOD("get_op_b"), &SwapChannels::get_op_b);
  ClassDB::bind_method(D_METHOD("set_op_b", "val"), &SwapChannels::set_op_b);
  ClassDB::bind_method(D_METHOD("get_op_a"), &SwapChannels::get_op_a);
  ClassDB::bind_method(D_METHOD("set_op_a", "val"), &SwapChannels::set_op_a);

 }



