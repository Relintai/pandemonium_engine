
#include "mirror.h"


Ref<Resource> Mirror::get_image() {
 return image;
}

void Mirror::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Mirror::get_input() {
 return input;
}

void Mirror::set_input(const Ref<Resource> &val) {
input = val;
}


int Mirror::get_direction() const {
 return direction;
}

void Mirror::set_direction(const int val) {
direction = val;
}


float Mirror::get_offset() const {
 return offset;
}

void Mirror::set_offset(const float val) {
offset = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "Horizontal,Vertical") ;
  int direction = 0;
 //export(float) ;
  float offset = 0;

 void Mirror::_init_properties() {

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


 void Mirror::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_direction", "set_direction", "Direction", [ "Horizontal", "Vertical" ]);
  mm_graph_node.add_slot_float("get_offset", "set_offset", "offset", 0.01);
}


 void Mirror::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Mirror::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i(uvmirror_$direction($uv, $offset));

  if (direction == 0) {
   return input.get_value(MMAlgos.uvmirror_h(uv, offset));
}


  else if (direction == 1) {
   return input.get_value(MMAlgos.uvmirror_v(uv, offset));
}

  return Color(0, 0, 0, 1);
}

 //direction;

 int Mirror::get_direction() {
  return direction;
}


 void Mirror::set_direction(const int val) {
  direction = val;
  set_dirty(true);
}

 //offset;

 float Mirror::get_offset() {
  return offset;
}


 void Mirror::set_offset(const float val) {
  offset = val;
  set_dirty(true);
}

}

 Mirror::Mirror() {
  image;
  input;
  direction = 0;
  offset = 0;
 }

 Mirror::~Mirror() {
 }


 static void Mirror::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Mirror::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Mirror::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Mirror::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Mirror::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_direction"), &Mirror::get_direction);
   ClassDB::bind_method(D_METHOD("set_direction", "value"), &Mirror::set_direction);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");


   ClassDB::bind_method(D_METHOD("get_offset"), &Mirror::get_offset);
   ClassDB::bind_method(D_METHOD("set_offset", "value"), &Mirror::set_offset);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Mirror::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Mirror::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Mirror::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Mirror::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_direction"), &Mirror::get_direction);
  ClassDB::bind_method(D_METHOD("set_direction", "val"), &Mirror::set_direction);
  ClassDB::bind_method(D_METHOD("get_offset"), &Mirror::get_offset);
  ClassDB::bind_method(D_METHOD("set_offset", "val"), &Mirror::set_offset);

 }



