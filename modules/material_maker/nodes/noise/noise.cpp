
#include "noise.h"


Ref<Resource> Noise::get_image() {
 return image;
}

void Noise::set_image(const Ref<Resource> &val) {
image = val;
}


int Noise::get_grid_size() const {
 return grid_size;
}

void Noise::set_grid_size(const int val) {
grid_size = val;
}


float Noise::get_density() const {
 return density;
}

void Noise::set_density(const float val) {
density = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(int) ;
  int grid_size = 16;
 //export(float) ;
  float density = 0.5;

 void Noise::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Noise::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_int("get_grid_size", "set_grid_size", "Grid Size");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_float("get_density", "set_density", "Density", 0.01);
}


 Color Noise::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //return dots(uv, 1.0/$(size), $(density), $(seed));
   float f = MMAlgos.dots(uv, 1.0 / float(grid_size), density, ps);
  return Color(f, f, f, 1);
}


 void Noise::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 int Noise::get_grid_size() {
  return grid_size;
}


 void Noise::set_grid_size(const int val) {
  grid_size = val;
  set_dirty(true);
}


 float Noise::get_density() {
  return density;
}


 void Noise::set_density(const float val) {
  density = val;
  set_dirty(true);
}

}

 Noise::Noise() {
  image;
  grid_size = 16;
  density = 0.5;
 }

 Noise::~Noise() {
 }


 static void Noise::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Noise::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Noise::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_grid_size"), &Noise::get_grid_size);
   ClassDB::bind_method(D_METHOD("set_grid_size", "value"), &Noise::set_grid_size);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_size"), "set_grid_size", "get_grid_size");


   ClassDB::bind_method(D_METHOD("get_density"), &Noise::get_density);
   ClassDB::bind_method(D_METHOD("set_density", "value"), &Noise::set_density);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "density"), "set_density", "get_density");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Noise::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Noise::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Noise::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Noise::_render);
  ClassDB::bind_method(D_METHOD("get_grid_size"), &Noise::get_grid_size);
  ClassDB::bind_method(D_METHOD("set_grid_size", "val"), &Noise::set_grid_size);
  ClassDB::bind_method(D_METHOD("get_density"), &Noise::get_density);
  ClassDB::bind_method(D_METHOD("set_density", "val"), &Noise::set_density);

 }



