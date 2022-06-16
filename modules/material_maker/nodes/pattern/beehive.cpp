
#include "beehive.h"


Ref<Resource> Beehive::get_out_main() {
 return out_main;
}

void Beehive::set_out_main(const Ref<Resource> &val) {
out_main = val;
}


Ref<Resource> Beehive::get_out_random_color() {
 return out_random_color;
}

void Beehive::set_out_random_color(const Ref<Resource> &val) {
out_random_color = val;
}


Ref<Resource> Beehive::get_out_uv_map() {
 return out_uv_map;
}

void Beehive::set_out_uv_map(const Ref<Resource> &val) {
out_uv_map = val;
}


Vector2 Beehive::get_size() {
 return size;
}

void Beehive::set_size(const Vector2 &val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> out_main;
 //export(Resource) ;
  Ref<Resource> out_random_color;
 //export(Resource) ;
  Ref<Resource> out_uv_map;
 //export(Vector2) ;
  Vector2 size = Vector2(4, 4);

 void Beehive::_init_properties() {

  if (!out_main) {
   out_main = MMNodeUniversalProperty.new();
   out_main.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_main.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_random_color) {
   out_random_color = MMNodeUniversalProperty.new();
   out_random_color.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_random_color.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!out_uv_map) {
   out_uv_map = MMNodeUniversalProperty.new();
   out_uv_map.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  out_uv_map.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(out_main);
  register_output_property(out_random_color);
  register_output_property(out_uv_map);
}


 void Beehive::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(out_main);
  mm_graph_node.add_slot_texture_universal(out_random_color);
  mm_graph_node.add_slot_texture_universal(out_uv_map);
  //, Vector2(1, 32))//, Vector2(0, 32));
  mm_graph_node.add_slot_vector2("get_size", "set_size", "Size");
}


 void Beehive::_render(const Variant &material) {
   Ref<Image> main_pattern = Image.new();
   Ref<Image> random_color = Image.new();
   Ref<Image> uv_map = Image.new();
  main_pattern.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  random_color.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  uv_map.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  main_pattern.lock();
  random_color.lock();
  uv_map.lock();
   float w = material.image_size.x;
   float h = material.image_size.y;
   float pseed = randf() + randi();

  for (int x = 0; x < material.image_size.x; ++x) { //x in range(material.image_size.x)

   for (int y = 0; y < material.image_size.y; ++y) { //y in range(material.image_size.y)
     Vector2 uv = Vector2(x / w, y / h);
     float ps = 1.0 / float(pseed);
    //vec2 $(name_uv)_uv = $uv*vec2($sx, $sy*1.73205080757);
    //vec4 $(name_uv)_center = beehive_center($(name_uv)_uv);
     Vector2 beehive_uv = uv * size;
     Color beehive_uv_center = MMAlgos.beehive_center(beehive_uv);
    //Output (float) - Shows the greyscale pattern;
    //1.0-2.0*beehive_dist($(name_uv)_center.xy);
     float f = 1.0 - 2.0 * MMAlgos.beehive_dist(Vector2(beehive_uv_center.r, beehive_uv_center.g));
     Color main_pattern_col = Color(f, f, f, 1);
    //Random color (rgb) - Shows a random color for each hexagonal tile;
    //rand3(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed)));
     Vector3 rcv3 = MMAlgos.rand3(MMAlgos.fractv2(Vector2(beehive_uv_center.b, beehive_uv_center.a) / size) + Vector2(ps, ps));
     Color random_color_col = Color(rcv3.x, rcv3.y, rcv3.z, 1);
    //UV map (rgb) - Shows an UV map to be connected to the UV map port of the Custom UV node;
    //vec3(vec2(0.5)+$(name_uv)_center.xy, rand(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed))));
     Vector2 uvm1 = Vector2(0.5, 0.5) + Vector2(beehive_uv_center.r, beehive_uv_center.g);
     Vector2 uvm2 = MMAlgos.rand2(MMAlgos.fractv2(Vector2(beehive_uv_center.b, beehive_uv_center.a) / size) + Vector2(ps, ps));
     Color uv_map_col = Color(uvm1.x, uvm1.y, uvm2.x, 1);
    main_pattern.set_pixel(x, y, main_pattern_col);
    random_color.set_pixel(x, y, random_color_col);
    uv_map.set_pixel(x, y, uv_map_col);
}

}

  main_pattern.unlock();
  random_color.unlock();
  uv_map.unlock();
  out_main.set_value(main_pattern);
  out_random_color.set_value(random_color);
  out_uv_map.set_value(uv_map);
}


 Color Beehive::_get_value_for(const Vector2 &uv, const int pseed) {
  return Color();
}

 //size;

 Vector2 Beehive::get_size() {
  return size;
}


 void Beehive::set_size(const Vector2 &val) {
  size = val;
  set_dirty(true);
}

}

 Beehive::Beehive() {
  out_main;
  out_random_color;
  out_uv_map;
  size = Vector2(4, 4);
 }

 Beehive::~Beehive() {
 }


 static void Beehive::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_out_main"), &Beehive::get_out_main);
   ClassDB::bind_method(D_METHOD("set_out_main", "value"), &Beehive::set_out_main);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_main", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_main", "get_out_main");


   ClassDB::bind_method(D_METHOD("get_out_random_color"), &Beehive::get_out_random_color);
   ClassDB::bind_method(D_METHOD("set_out_random_color", "value"), &Beehive::set_out_random_color);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_random_color", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_random_color", "get_out_random_color");


   ClassDB::bind_method(D_METHOD("get_out_uv_map"), &Beehive::get_out_uv_map);
   ClassDB::bind_method(D_METHOD("set_out_uv_map", "value"), &Beehive::set_out_uv_map);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_uv_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_out_uv_map", "get_out_uv_map");


   ClassDB::bind_method(D_METHOD("get_size"), &Beehive::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Beehive::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Beehive::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Beehive::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Beehive::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Beehive::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_size"), &Beehive::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Beehive::set_size);

 }



