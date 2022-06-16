
#include "kaleidoscope.h"


Ref<Resource> Kaleidoscope::get_image() {
 return image;
}

void Kaleidoscope::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Kaleidoscope::get_input() {
 return input;
}

void Kaleidoscope::set_input(const Ref<Resource> &val) {
input = val;
}


int Kaleidoscope::get_count() const {
 return count;
}

void Kaleidoscope::set_count(const int val) {
count = val;
}


float Kaleidoscope::get_offset() const {
 return offset;
}

void Kaleidoscope::set_offset(const float val) {
offset = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int) ;
  int count = 5;
 //export(float) ;
  float offset = 0;

 void Kaleidoscope::_init_properties() {

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


 void Kaleidoscope::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_int("get_count", "set_count", "Count");
  mm_graph_node.add_slot_float("get_offset", "set_offset", "Offset", 0.5);
}


 void Kaleidoscope::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Kaleidoscope::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i(kal_rotate($uv, $count, $offset));
  return input.get_value(MMAlgos.kal_rotate(uv, count, offset));
}

 //count;

 int Kaleidoscope::get_count() {
  return count;
}


 void Kaleidoscope::set_count(const int val) {
  count = val;
  set_dirty(true);
}

 //offset;

 float Kaleidoscope::get_offset() {
  return offset;
}


 void Kaleidoscope::set_offset(const float val) {
  offset = val;
  set_dirty(true);
}

}

 Kaleidoscope::Kaleidoscope() {
  image;
  input;
  count = 5;
  offset = 0;
 }

 Kaleidoscope::~Kaleidoscope() {
 }


 static void Kaleidoscope::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Kaleidoscope::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Kaleidoscope::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Kaleidoscope::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Kaleidoscope::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_count"), &Kaleidoscope::get_count);
   ClassDB::bind_method(D_METHOD("set_count", "value"), &Kaleidoscope::set_count);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");


   ClassDB::bind_method(D_METHOD("get_offset"), &Kaleidoscope::get_offset);
   ClassDB::bind_method(D_METHOD("set_offset", "value"), &Kaleidoscope::set_offset);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Kaleidoscope::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Kaleidoscope::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Kaleidoscope::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Kaleidoscope::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_count"), &Kaleidoscope::get_count);
  ClassDB::bind_method(D_METHOD("set_count", "val"), &Kaleidoscope::set_count);
  ClassDB::bind_method(D_METHOD("get_offset"), &Kaleidoscope::get_offset);
  ClassDB::bind_method(D_METHOD("set_offset", "val"), &Kaleidoscope::set_offset);

 }



