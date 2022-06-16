
#include "fill_channel.h"


Ref<Resource> FillChannel::get_image() {
 return image;
}

void FillChannel::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillChannel::get_input() {
 return input;
}

void FillChannel::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> FillChannel::get_value() {
 return value;
}

void FillChannel::set_value(const Ref<Resource> &val) {
value = val;
}


int FillChannel::get_channel() const {
 return channel;
}

void FillChannel::set_channel(const int val) {
channel = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> value;
 //export(int, "R,G,B,A") ;
  int channel = 3;

 void FillChannel::_init_properties() {

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

  if (!value) {
   value = MMNodeUniversalProperty.new();
   value.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   value.set_default_value(1);
}

  value.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  value.value_step = 0.01;
  value.value_range = Vector2(0, 1);
  register_input_property(input);
  register_output_property(image);
  register_input_property(value);
}


 void FillChannel::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float_universal(value);
  mm_graph_node.add_slot_enum("get_channel", "set_channel", "Channel", [ "R", "G", "B", "A" ]);
}


 void FillChannel::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillChannel::_get_value_for(const Vector2 &uv, const int pseed) {
   Color col = input.get_value(uv);

  if (channel == 0) {
   col.r = value.get_value(uv);
}


  if (channel == 1) {
   col.g = value.get_value(uv);
}


  if (channel == 2) {
   col.b = value.get_value(uv);
}


  if (channel == 3) {
   col.a = value.get_value(uv);
}

  return col;
}


 int FillChannel::get_channel() {
  return channel;
}


 void FillChannel::set_channel(const int val) {
  channel = val;
  set_dirty(true);
}

}

 FillChannel::FillChannel() {
  image;
  input;
  value;
  channel = 3;
 }

 FillChannel::~FillChannel() {
 }


 static void FillChannel::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillChannel::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillChannel::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillChannel::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillChannel::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_value"), &FillChannel::get_value);
   ClassDB::bind_method(D_METHOD("set_value", "value"), &FillChannel::set_value);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_value", "get_value");


   ClassDB::bind_method(D_METHOD("get_channel"), &FillChannel::get_channel);
   ClassDB::bind_method(D_METHOD("set_channel", "value"), &FillChannel::set_channel);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "channel"), "set_channel", "get_channel");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillChannel::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillChannel::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillChannel::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillChannel::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_channel"), &FillChannel::get_channel);
  ClassDB::bind_method(D_METHOD("set_channel", "val"), &FillChannel::set_channel);

 }



