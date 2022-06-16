
#include "fill_to_random_grey.h"


Ref<Resource> FillToRandomGrey::get_image() {
 return image;
}

void FillToRandomGrey::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillToRandomGrey::get_input() {
 return input;
}

void FillToRandomGrey::set_input(const Ref<Resource> &val) {
input = val;
}


float FillToRandomGrey::get_edge_color() const {
 return edge_color;
}

void FillToRandomGrey::set_edge_color(const float val) {
edge_color = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float edge_color = 1;

 void FillToRandomGrey::_init_properties() {

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


 void FillToRandomGrey::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_edge_color", "set_edge_color", "Edge color", 0.01);
}


 void FillToRandomGrey::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillToRandomGrey::_get_value_for(const Vector2 &uv, const int pseed) {
  //vec4 $(name_uv)_bb = $in($uv);
   Color c = input.get_value(uv);
  //mix($edgecolor, rand(vec2(float($seed), rand(vec2(rand($(name_uv)_bb.xy), rand($(name_uv)_bb.zw))))), step(0.0000001, dot($(name_uv)_bb.zw, vec2(1.0))));
   float r1 = MMAlgos.rand(Vector2(c.r, c.g));
   float r2 = MMAlgos.rand(Vector2(c.b, c.a));
   float s = MMAlgos.step(0.0000001, Vector2(c.b, c.a).dot(Vector2(1, 1)));
   float f = lerp(edge_color, MMAlgos.rand(Vector2(1.0 / float(pseed), MMAlgos.rand(Vector2(r1, r2)))), s);
  return Color(f, f, f, 1);
}

 //edge_color;

 float FillToRandomGrey::get_edge_color() {
  return edge_color;
}


 void FillToRandomGrey::set_edge_color(const float val) {
  edge_color = val;
  set_dirty(true);
}

}

 FillToRandomGrey::FillToRandomGrey() {
  image;
  input;
  edge_color = 1;
 }

 FillToRandomGrey::~FillToRandomGrey() {
 }


 static void FillToRandomGrey::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillToRandomGrey::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillToRandomGrey::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillToRandomGrey::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillToRandomGrey::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_edge_color"), &FillToRandomGrey::get_edge_color);
   ClassDB::bind_method(D_METHOD("set_edge_color", "value"), &FillToRandomGrey::set_edge_color);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "edge_color"), "set_edge_color", "get_edge_color");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillToRandomGrey::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillToRandomGrey::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillToRandomGrey::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillToRandomGrey::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_edge_color"), &FillToRandomGrey::get_edge_color);
  ClassDB::bind_method(D_METHOD("set_edge_color", "val"), &FillToRandomGrey::set_edge_color);

 }



