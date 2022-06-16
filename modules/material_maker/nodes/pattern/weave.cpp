
#include "weave.h"


Ref<Resource> Weave::get_out_main() {
 return out_main;
}

void Weave::set_out_main(const Ref<Resource> &val) {
out_main = val;
}


Ref<Resource> Weave::get_out_horizontal_map() {
 return out_horizontal_map;
}

void Weave::set_out_horizontal_map(const Ref<Resource> &val) {
out_horizontal_map = val;
}


Ref<Resource> Weave::get_out_vertical_map() {
 return out_vertical_map;
}

void Weave::set_out_vertical_map(const Ref<Resource> &val) {
out_vertical_map = val;
}


Vector2 Weave::get_size() {
 return size;
}

void Weave::set_size(const Vector2 &val) {
size = val;
}


Ref<Resource> Weave::get_width() {
 return width;
}

void Weave::set_width(const Ref<Resource> &val) {
width = val;
}


int Weave::get_stitch() const {
 return stitch;
}

void Weave::set_stitch(const int val) {
stitch = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> out_main;
 //export(Resource) ;
  Ref<Resource> out_horizontal_map;
 //export(Resource) ;
  Ref<Resource> out_vertical_map;
 //export(Vector2) ;
  Vector2 size = Vector2(4, 4);
 //export(Resource) ;
  Ref<Resource> width;
 //export(int) ;
  int stitch = 1;

 void Weave::_init_properties() {

  if (!out_main) {
   out_main = MMNodeUniversalProperty.new();
   out_main.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_main.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_horizontal_map) {
   out_horizontal_map = MMNodeUniversalProperty.new();
   out_horizontal_map.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_horizontal_map.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_vertical_map) {
   out_vertical_map = MMNodeUniversalProperty.new();
   out_vertical_map.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_vertical_map.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!width) {
   width = MMNodeUniversalProperty.new();
   width.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
   width.set_default_value(Vector2(0.9, 0.9));
}

  width.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  width.slot_name = "Width";
  width.value_step = 0.01;
  width.value_range = Vector2(0, 1);
  register_output_property(out_main);
  register_output_property(out_horizontal_map);
  register_output_property(out_vertical_map);
  register_input_property(width);
}


 void Weave::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(out_main);
  mm_graph_node.add_slot_texture_universal(out_horizontal_map);
  mm_graph_node.add_slot_texture_universal(out_vertical_map);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_vector2("get_size", "set_size", "Size");
  mm_graph_node.add_slot_vector2_universal(width);
  mm_graph_node.add_slot_int("get_stitch", "set_stitch", "Stitch");
}


 void Weave::_render(const Variant &material) {
   Ref<Image> main_pattern = Image.new();
   Ref<Image> horizontal_map = Image.new();
   Ref<Image> vertical_map = Image.new();
  main_pattern.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  horizontal_map.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  vertical_map.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  main_pattern.lock();
  horizontal_map.lock();
  vertical_map.lock();
   float w = material.image_size.x;
   float h = material.image_size.y;
   float pseed = randf() + randi();

  for (int x = 0; x < material.image_size.x; ++x) { //x in range(material.image_size.x)

   for (int y = 0; y < material.image_size.y; ++y) { //y in range(material.image_size.y)
     Vector2 uv = Vector2(x / w, y / h);
     Vector2 width_val = width.get_value(uv);
    //vec3 $(name_uv) = weave2($uv, vec2($columns, $rows), $stitch, $width_x*$width_map($uv), $width_y*$width_map($uv));
     Vector3 weave = MMAlgos.weave2(uv, size, stitch, width_val.x, width_val.y);
    //Outputs:;
    //Output (float) - Shows the generated greyscale weave pattern.;
    //$(name_uv).x;
     Color main_pattern_col = Color(weave.x, weave.x, weave.x, 1);
    //Horizontal mask (float) - Horizontal mask;
    //$(name_uv).y;
     Color horizontal_map_col = Color(weave.y, weave.y, weave.y, 1);
    //Vertical mask (float) - Mask for vertical stripes;
    //$(name_uv).z;
     Color vertical_map_col = Color(weave.z, weave.z, weave.z, 1);
    main_pattern.set_pixel(x, y, main_pattern_col);
    horizontal_map.set_pixel(x, y, horizontal_map_col);
    vertical_map.set_pixel(x, y, vertical_map_col);
}

}

  main_pattern.unlock();
  horizontal_map.unlock();
  vertical_map.unlock();
  out_main.set_value(main_pattern);
  out_horizontal_map.set_value(horizontal_map);
  out_vertical_map.set_value(vertical_map);
}


 Color Weave::_get_value_for(const Vector2 &uv, const int pseed) {
  return Color();
}

 //size;

 Vector2 Weave::get_size() {
  return size;
}


 void Weave::set_size(const Vector2 &val) {
  size = val;
  set_dirty(true);
}

 //stitch;

 int Weave::get_stitch() {
  return stitch;
}


 void Weave::set_stitch(const int val) {
  stitch = val;
  set_dirty(true);
}

}

 Weave::Weave() {
  out_main;
  out_horizontal_map;
  out_vertical_map;
  size = Vector2(4, 4);
  width;
  stitch = 1;
 }

 Weave::~Weave() {
 }


 static void Weave::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_out_main"), &Weave::get_out_main);
   ClassDB::bind_method(D_METHOD("set_out_main", "value"), &Weave::set_out_main);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_main", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_main", "get_out_main");


   ClassDB::bind_method(D_METHOD("get_out_horizontal_map"), &Weave::get_out_horizontal_map);
   ClassDB::bind_method(D_METHOD("set_out_horizontal_map", "value"), &Weave::set_out_horizontal_map);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_horizontal_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_horizontal_map", "get_out_horizontal_map");


   ClassDB::bind_method(D_METHOD("get_out_vertical_map"), &Weave::get_out_vertical_map);
   ClassDB::bind_method(D_METHOD("set_out_vertical_map", "value"), &Weave::set_out_vertical_map);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_vertical_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_vertical_map", "get_out_vertical_map");


   ClassDB::bind_method(D_METHOD("get_size"), &Weave::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Weave::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");


   ClassDB::bind_method(D_METHOD("get_width"), &Weave::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &Weave::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "width", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_width", "get_width");


   ClassDB::bind_method(D_METHOD("get_stitch"), &Weave::get_stitch);
   ClassDB::bind_method(D_METHOD("set_stitch", "value"), &Weave::set_stitch);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "stitch"), "set_stitch", "get_stitch");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Weave::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Weave::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Weave::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Weave::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_size"), &Weave::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Weave::set_size);
  ClassDB::bind_method(D_METHOD("get_stitch"), &Weave::get_stitch);
  ClassDB::bind_method(D_METHOD("set_stitch", "val"), &Weave::set_stitch);

 }



