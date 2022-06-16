
#include "voronoi.h"


Ref<Resource> Voronoi::get_out_nodes() {
 return out_nodes;
}

void Voronoi::set_out_nodes(const Ref<Resource> &val) {
out_nodes = val;
}


Ref<Resource> Voronoi::get_out_borders() {
 return out_borders;
}

void Voronoi::set_out_borders(const Ref<Resource> &val) {
out_borders = val;
}


Ref<Resource> Voronoi::get_out_random_color() {
 return out_random_color;
}

void Voronoi::set_out_random_color(const Ref<Resource> &val) {
out_random_color = val;
}


Ref<Resource> Voronoi::get_out_fill() {
 return out_fill;
}

void Voronoi::set_out_fill(const Ref<Resource> &val) {
out_fill = val;
}


Vector2 Voronoi::get_scale() {
 return scale;
}

void Voronoi::set_scale(const Vector2 &val) {
scale = val;
}


Vector2 Voronoi::get_stretch() {
 return stretch;
}

void Voronoi::set_stretch(const Vector2 &val) {
stretch = val;
}


float Voronoi::get_intensity() const {
 return intensity;
}

void Voronoi::set_intensity(const float val) {
intensity = val;
}


float Voronoi::get_randomness() const {
 return randomness;
}

void Voronoi::set_randomness(const float val) {
randomness = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> out_nodes;
 //export(Resource) ;
  Ref<Resource> out_borders;
 //export(Resource) ;
  Ref<Resource> out_random_color;
 //export(Resource) ;
  Ref<Resource> out_fill;
 //export(Vector2) ;
  Vector2 scale = Vector2(4, 4);
 //export(Vector2) ;
  Vector2 stretch = Vector2(1, 1);
 //export(float) ;
  float intensity = 1;
 //export(float) ;
  float randomness = 0.85;

 void Voronoi::_init_properties() {

  if (!out_nodes) {
   out_nodes = MMNodeUniversalProperty.new();
   out_nodes.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_nodes.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_borders) {
   out_borders = MMNodeUniversalProperty.new();
   out_borders.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_borders.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_random_color) {
   out_random_color = MMNodeUniversalProperty.new();
   out_random_color.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_random_color.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_fill) {
   out_fill = MMNodeUniversalProperty.new();
   out_fill.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_fill.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(out_nodes);
  register_output_property(out_borders);
  register_output_property(out_random_color);
  register_output_property(out_fill);
}


 void Voronoi::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(out_nodes);
  mm_graph_node.add_slot_texture_universal(out_borders);
  mm_graph_node.add_slot_texture_universal(out_random_color);
  mm_graph_node.add_slot_texture_universal(out_fill);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale", 0.1);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_vector2("get_stretch", "set_stretch", "stretch", 0.01);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_float("get_intensity", "set_intensity", "Intensity", 0.01);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_float("get_randomness", "set_randomness", "Randomness", 0.01);
}


 void Voronoi::_render(const Variant &material) {
   Ref<Image> nodes = Image.new();
   Ref<Image> borders = Image.new();
   Ref<Image> random_color = Image.new();
   Ref<Image> fill = Image.new();
  nodes.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  borders.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  random_color.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  fill.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  nodes.lock();
  borders.lock();
  random_color.lock();
  fill.lock();
   float w = material.image_size.x;
   float h = material.image_size.y;
   float pseed = randf() + randi();

  for (int x = 0; x < material.image_size.x; ++x) { //x in range(material.image_size.x)

   for (int y = 0; y < material.image_size.y; ++y) { //y in range(material.image_size.y)
     Vector2 uv = Vector2(x / w, y / h);
     float ps = 1.0 / float(pseed);
    //vec4 $(name_uv)_xyzw = voronoi($uv, vec2($scale_x, $scale_y), vec2($stretch_y, $stretch_x), $intensity, $randomness, $seed);
     Color voronoi = MMAlgos.voronoi(uv, scale, stretch, intensity, randomness, ps);
    //Nodes - float - A greyscale pattern based on the distance to cell centers;
    //$(name_uv)_xyzw.z;
     Color nodes_col = Color(voronoi.b, voronoi.b, voronoi.b, 1);
    //Borders - float - A greyscale pattern based on the distance to borders;
    //$(name_uv)_xyzw.w;
     Color borders_col = Color(voronoi.a, voronoi.a, voronoi.a, 1);
    //Random color - rgb - A color pattern that assigns a random color to each cell;
    //rand3(fract(floor($(name_uv)_xyzw.xy)/vec2($scale_x, $scale_y)));
     Vector3 rv3 = MMAlgos.rand3(MMAlgos.fractv2(Vector2(voronoi.r, voronoi.g) / scale));
     Color random_color_col = Color(rv3.x, rv3.y, rv3.z, 1);
    //Fill - rgba - An output that should be plugged into a Fill companion node;
    //vec4(fract(($(name_uv)_xyzw.xy-1.0)/vec2($scale_x, $scale_y)), vec2(2.0)/vec2($scale_x, $scale_y));
     Vector2 fv21 = MMAlgos.fractv2((Vector2(voronoi.r, voronoi.g) - Vector2(1, 1)) / scale);
     Vector2 fv22 = Vector2(2, 2) / scale;
     Color fill_col = Color(fv21.x, fv21.y, fv22.x, fv22.y);
    nodes.set_pixel(x, y, nodes_col);
    borders.set_pixel(x, y, borders_col);
    random_color.set_pixel(x, y, random_color_col);
    fill.set_pixel(x, y, fill_col);
}

}

  nodes.unlock();
  borders.unlock();
  random_color.unlock();
  fill.unlock();
  out_nodes.set_value(nodes);
  out_borders.set_value(borders);
  out_random_color.set_value(random_color);
  out_fill.set_value(fill);
}


 Color Voronoi::_get_value_for(const Vector2 &uv, const int pseed) {
  return Color();
}

 //scale;

 Vector2 Voronoi::get_scale() {
  return scale;
}


 void Voronoi::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}

 //stretch;

 Vector2 Voronoi::get_stretch() {
  return stretch;
}


 void Voronoi::set_stretch(const Vector2 &val) {
  stretch = val;
  set_dirty(true);
}

 //intensity;

 float Voronoi::get_intensity() {
  return intensity;
}


 void Voronoi::set_intensity(const float val) {
  intensity = val;
  set_dirty(true);
}

 //randomness;

 float Voronoi::get_randomness() {
  return randomness;
}


 void Voronoi::set_randomness(const float val) {
  randomness = val;
  set_dirty(true);
}

}

 Voronoi::Voronoi() {
  out_nodes;
  out_borders;
  out_random_color;
  out_fill;
  scale = Vector2(4, 4);
  stretch = Vector2(1, 1);
  intensity = 1;
  randomness = 0.85;
 }

 Voronoi::~Voronoi() {
 }


 static void Voronoi::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_out_nodes"), &Voronoi::get_out_nodes);
   ClassDB::bind_method(D_METHOD("set_out_nodes", "value"), &Voronoi::set_out_nodes);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_nodes", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_nodes", "get_out_nodes");


   ClassDB::bind_method(D_METHOD("get_out_borders"), &Voronoi::get_out_borders);
   ClassDB::bind_method(D_METHOD("set_out_borders", "value"), &Voronoi::set_out_borders);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_borders", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_borders", "get_out_borders");


   ClassDB::bind_method(D_METHOD("get_out_random_color"), &Voronoi::get_out_random_color);
   ClassDB::bind_method(D_METHOD("set_out_random_color", "value"), &Voronoi::set_out_random_color);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_random_color", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_random_color", "get_out_random_color");


   ClassDB::bind_method(D_METHOD("get_out_fill"), &Voronoi::get_out_fill);
   ClassDB::bind_method(D_METHOD("set_out_fill", "value"), &Voronoi::set_out_fill);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_fill", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_fill", "get_out_fill");


   ClassDB::bind_method(D_METHOD("get_scale"), &Voronoi::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &Voronoi::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


   ClassDB::bind_method(D_METHOD("get_stretch"), &Voronoi::get_stretch);
   ClassDB::bind_method(D_METHOD("set_stretch", "value"), &Voronoi::set_stretch);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "stretch"), "set_stretch", "get_stretch");


   ClassDB::bind_method(D_METHOD("get_intensity"), &Voronoi::get_intensity);
   ClassDB::bind_method(D_METHOD("set_intensity", "value"), &Voronoi::set_intensity);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "intensity"), "set_intensity", "get_intensity");


   ClassDB::bind_method(D_METHOD("get_randomness"), &Voronoi::get_randomness);
   ClassDB::bind_method(D_METHOD("set_randomness", "value"), &Voronoi::set_randomness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "randomness"), "set_randomness", "get_randomness");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Voronoi::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Voronoi::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Voronoi::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Voronoi::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_scale"), &Voronoi::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &Voronoi::set_scale);
  ClassDB::bind_method(D_METHOD("get_stretch"), &Voronoi::get_stretch);
  ClassDB::bind_method(D_METHOD("set_stretch", "val"), &Voronoi::set_stretch);
  ClassDB::bind_method(D_METHOD("get_intensity"), &Voronoi::get_intensity);
  ClassDB::bind_method(D_METHOD("set_intensity", "val"), &Voronoi::set_intensity);
  ClassDB::bind_method(D_METHOD("get_randomness"), &Voronoi::get_randomness);
  ClassDB::bind_method(D_METHOD("set_randomness", "val"), &Voronoi::set_randomness);

 }



