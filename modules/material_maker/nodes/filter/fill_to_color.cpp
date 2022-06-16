
#include "fill_to_color.h"


Ref<Resource> FillToColor::get_image() {
 return image;
}

void FillToColor::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillToColor::get_input() {
 return input;
}

void FillToColor::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> FillToColor::get_color_map() {
 return color_map;
}

void FillToColor::set_color_map(const Ref<Resource> &val) {
color_map = val;
}


Color FillToColor::get_edge_color() {
 return edge_color;
}

void FillToColor::set_edge_color(const Color &val) {
edge_color = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> color_map;
 //export(Color) ;
  Color edge_color = Color(1, 1, 1, 1);

 void FillToColor::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";

  if (!color_map) {
   color_map = MMNodeUniversalProperty.new();
   color_map.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   color_map.set_default_value(Color(1, 1, 1, 1));
}

  color_map.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  color_map.slot_name = ">>>  Color Map  ";

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  //image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  //image.force_override = true;
  register_input_property(input);
  register_input_property(color_map);
  register_output_property(image);
}


 void FillToColor::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(color_map);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_color("get_edge_color", "set_edge_color");
}


 void FillToColor::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillToColor::_get_value_for(const Vector2 &uv, const int pseed) {
  //vec4 $(name_uv)_bb = $in($uv);
   Color c = input.get_value(uv);
  //mix($edgecolor, $map(fract($(name_uv)_bb.xy+0.5*$(name_uv)_bb.zw)), step(0.0000001, dot($(name_uv)_bb.zw, vec2(1.0))));
   Color rc = color_map.get_value(MMAlgos.fractv2(Vector2(c.r, c.g) + 0.5 * Vector2(c.b, c.a)));
   float s = MMAlgos.step(0.0000001, Vector2(c.b, c.a).dot(Vector2(1, 1)));
  return lerp(edge_color, rc, s);
}

 //edge_color;

 Color FillToColor::get_edge_color() {
  return edge_color;
}


 void FillToColor::set_edge_color(const Color &val) {
  edge_color = val;
  set_dirty(true);
}

}

 FillToColor::FillToColor() {
  image;
  input;
  color_map;
  edge_color = Color(1, 1, 1, 1);
 }

 FillToColor::~FillToColor() {
 }


 static void FillToColor::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillToColor::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillToColor::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillToColor::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillToColor::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_color_map"), &FillToColor::get_color_map);
   ClassDB::bind_method(D_METHOD("set_color_map", "value"), &FillToColor::set_color_map);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_color_map", "get_color_map");


   ClassDB::bind_method(D_METHOD("get_edge_color"), &FillToColor::get_edge_color);
   ClassDB::bind_method(D_METHOD("set_edge_color", "value"), &FillToColor::set_edge_color);
   ADD_PROPERTY(PropertyInfo(Variant::COLOR, "edge_color"), "set_edge_color", "get_edge_color");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillToColor::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillToColor::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillToColor::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillToColor::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_edge_color"), &FillToColor::get_edge_color);
  ClassDB::bind_method(D_METHOD("set_edge_color", "val"), &FillToColor::set_edge_color);

 }



