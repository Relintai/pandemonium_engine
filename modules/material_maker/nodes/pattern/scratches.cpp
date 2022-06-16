
#include "scratches.h"


Ref<Resource> Scratches::get_image() {
 return image;
}

void Scratches::set_image(const Ref<Resource> &val) {
image = val;
}


Vector2 Scratches::get_size() {
 return size;
}

void Scratches::set_size(const Vector2 &val) {
size = val;
}


int Scratches::get_layers() const {
 return layers;
}

void Scratches::set_layers(const int val) {
layers = val;
}


float Scratches::get_waviness() const {
 return waviness;
}

void Scratches::set_waviness(const float val) {
waviness = val;
}


int Scratches::get_angle() const {
 return angle;
}

void Scratches::set_angle(const int val) {
angle = val;
}


float Scratches::get_randomness() const {
 return randomness;
}

void Scratches::set_randomness(const float val) {
randomness = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Vector2) ;
  Vector2 size = Vector2(0.25, 0.4);
 //export(int) ;
  int layers = 4;
 //export(float) ;
  float waviness = 0.51;
 //export(int) ;
  int angle = 0;
 //export(float) ;
  float randomness = 0.44;

 void Scratches::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Scratches::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_vector2("get_size", "set_size", "Size", 0.01);
  mm_graph_node.add_slot_int("get_layers", "set_layers", "Layers");
  mm_graph_node.add_slot_float("get_waviness", "set_waviness", "Waviness", 0.01);
  mm_graph_node.add_slot_int("get_angle", "set_angle", "Angle");
  mm_graph_node.add_slot_float("get_randomness", "set_randomness", "Randomness", 0.01);
}


 void Scratches::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Scratches::_get_value_for(const Vector2 &uv, const int pseed) {
  //scratches($uv, int($layers), vec2($length, $width), $waviness, $angle, $randomness, vec2(float($seed), 0.0));
  return MMAlgos.scratchesc(uv, layers, size, waviness, angle, randomness, Vector2(pseed, 0.0));
}

 //size;

 Vector2 Scratches::get_size() {
  return size;
}


 void Scratches::set_size(const Vector2 &val) {
  size = val;
  set_dirty(true);
}

 //layers;

 int Scratches::get_layers() {
  return layers;
}


 void Scratches::set_layers(const int val) {
  layers = val;
  set_dirty(true);
}

 //waviness;

 float Scratches::get_waviness() {
  return waviness;
}


 void Scratches::set_waviness(const float val) {
  waviness = val;
  set_dirty(true);
}

 //angle;

 int Scratches::get_angle() {
  return angle;
}


 void Scratches::set_angle(const int val) {
  angle = val;
  set_dirty(true);
}

 //randomness;

 float Scratches::get_randomness() {
  return randomness;
}


 void Scratches::set_randomness(const float val) {
  randomness = val;
  set_dirty(true);
}

}

 Scratches::Scratches() {
  image;
  size = Vector2(0.25, 0.4);
  layers = 4;
  waviness = 0.51;
  angle = 0;
  randomness = 0.44;
 }

 Scratches::~Scratches() {
 }


 static void Scratches::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Scratches::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Scratches::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_size"), &Scratches::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Scratches::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");


   ClassDB::bind_method(D_METHOD("get_layers"), &Scratches::get_layers);
   ClassDB::bind_method(D_METHOD("set_layers", "value"), &Scratches::set_layers);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "layers"), "set_layers", "get_layers");


   ClassDB::bind_method(D_METHOD("get_waviness"), &Scratches::get_waviness);
   ClassDB::bind_method(D_METHOD("set_waviness", "value"), &Scratches::set_waviness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "waviness"), "set_waviness", "get_waviness");


   ClassDB::bind_method(D_METHOD("get_angle"), &Scratches::get_angle);
   ClassDB::bind_method(D_METHOD("set_angle", "value"), &Scratches::set_angle);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "angle"), "set_angle", "get_angle");


   ClassDB::bind_method(D_METHOD("get_randomness"), &Scratches::get_randomness);
   ClassDB::bind_method(D_METHOD("set_randomness", "value"), &Scratches::set_randomness);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "randomness"), "set_randomness", "get_randomness");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Scratches::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Scratches::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Scratches::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Scratches::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_size"), &Scratches::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Scratches::set_size);
  ClassDB::bind_method(D_METHOD("get_layers"), &Scratches::get_layers);
  ClassDB::bind_method(D_METHOD("set_layers", "val"), &Scratches::set_layers);
  ClassDB::bind_method(D_METHOD("get_waviness"), &Scratches::get_waviness);
  ClassDB::bind_method(D_METHOD("set_waviness", "val"), &Scratches::set_waviness);
  ClassDB::bind_method(D_METHOD("get_angle"), &Scratches::get_angle);
  ClassDB::bind_method(D_METHOD("set_angle", "val"), &Scratches::set_angle);
  ClassDB::bind_method(D_METHOD("get_randomness"), &Scratches::get_randomness);
  ClassDB::bind_method(D_METHOD("set_randomness", "val"), &Scratches::set_randomness);

 }



