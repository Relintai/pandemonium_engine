
#include "scale.h"


Ref<Resource> Scale::get_image() {
 return image;
}

void Scale::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Scale::get_input() {
 return input;
}

void Scale::set_input(const Ref<Resource> &val) {
input = val;
}


Vector2 Scale::get_center() {
 return center;
}

void Scale::set_center(const Vector2 &val) {
center = val;
}


Vector2 Scale::get_scale() {
 return scale;
}

void Scale::set_scale(const Vector2 &val) {
scale = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Vector2) ;
  Vector2 center = Vector2();
 //export(Vector2) ;
  Vector2 scale = Vector2(1, 1);

 void Scale::_init_properties() {

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


 void Scale::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_vector2("get_center", "set_center", "Center", 0.01);
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale", 0.01);
}


 void Scale::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Scale::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i(scale($uv, vec2(0.5+$cx, 0.5+$cy), vec2($scale_x, $scale_y)));
  return input.get_value(MMAlgos.scale(uv, center + Vector2(0.5, 0.5), scale));
}

 //center;

 Vector2 Scale::get_center() {
  return center;
}


 void Scale::set_center(const Vector2 &val) {
  center = val;
  set_dirty(true);
}

 //scale;

 Vector2 Scale::get_scale() {
  return scale;
}


 void Scale::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}

}

 Scale::Scale() {
  image;
  input;
  center = Vector2();
  scale = Vector2(1, 1);
 }

 Scale::~Scale() {
 }


 static void Scale::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Scale::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Scale::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Scale::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Scale::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_center"), &Scale::get_center);
   ClassDB::bind_method(D_METHOD("set_center", "value"), &Scale::set_center);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");


   ClassDB::bind_method(D_METHOD("get_scale"), &Scale::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &Scale::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Scale::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Scale::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Scale::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Scale::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_center"), &Scale::get_center);
  ClassDB::bind_method(D_METHOD("set_center", "val"), &Scale::set_center);
  ClassDB::bind_method(D_METHOD("get_scale"), &Scale::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &Scale::set_scale);

 }



