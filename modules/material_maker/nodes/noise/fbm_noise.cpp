
#include "fbm_noise.h"


Ref<Resource> FbmNoise::get_image() {
 return image;
}

void FbmNoise::set_image(const Ref<Resource> &val) {
image = val;
}


int FbmNoise::get_type() const {
 return type;
}

void FbmNoise::set_type(const int val) {
type = val;
}


Vector2 FbmNoise::get_scale() {
 return scale;
}

void FbmNoise::set_scale(const Vector2 &val) {
scale = val;
}


int FbmNoise::get_folds() const {
 return folds;
}

void FbmNoise::set_folds(const int val) {
folds = val;
}


int FbmNoise::get_iterations() const {
 return iterations;
}

void FbmNoise::set_iterations(const int val) {
iterations = val;
}


float FbmNoise::get_persistence() const {
 return persistence;
}

void FbmNoise::set_persistence(const float val) {
persistence = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(int, "Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6") ;
  int type = 0;
 //export(Vector2) ;
  Vector2 scale = Vector2(2, 2);
 //export(int) ;
  int folds = 0;
 //export(int) ;
  int iterations = 5;
 //export(float) ;
  float persistence = 0.5;

 void FbmNoise::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void FbmNoise::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  //, Vector2(0, 1));
  mm_graph_node.add_slot_enum("get_type", "set_type", "Type", [ "Value", "Perlin", "Simplex", "Cellular1", "Cellular2", "Cellular3", "Cellular4", "Cellular5", "Cellular6" ]);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_int("get_folds", "set_folds", "folds");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_int("get_iterations", "set_iterations", "Iterations");
  //, Vector2(0, 1));
  mm_graph_node.add_slot_float("get_persistence", "set_persistence", "Persistence", 0.01);
}


 Color FbmNoise::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //"Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6";

  if (type == 0) {
   return MMAlgos.fbmval(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 1) {
   return MMAlgos.perlin(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 2) {
   return MMAlgos.simplex(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 3) {
   return MMAlgos.cellular(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 4) {
   return MMAlgos.cellular2(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 5) {
   return MMAlgos.cellular3(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 6) {
   return MMAlgos.cellular4(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 7) {
   return MMAlgos.cellular5(uv, scale, folds, iterations, persistence, ps);
}


  else if (type == 8) {
   return MMAlgos.cellular6(uv, scale, folds, iterations, persistence, ps);
}

  return Color();
}


 void FbmNoise::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 int FbmNoise::get_type() {
  return type;
}


 void FbmNoise::set_type(const int val) {
  type = val;
  set_dirty(true);
}


 Vector2 FbmNoise::get_scale() {
  return scale;
}


 void FbmNoise::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}


 int FbmNoise::get_folds() {
  return folds;
}


 void FbmNoise::set_folds(const int val) {
  folds = val;
  set_dirty(true);
}


 int FbmNoise::get_iterations() {
  return iterations;
}


 void FbmNoise::set_iterations(const int val) {
  iterations = val;
  set_dirty(true);
}


 float FbmNoise::get_persistence() {
  return persistence;
}


 void FbmNoise::set_persistence(const float val) {
  persistence = val;
  set_dirty(true);
}

}

 FbmNoise::FbmNoise() {
  image;
  type = 0;
  scale = Vector2(2, 2);
  folds = 0;
  iterations = 5;
  persistence = 0.5;
 }

 FbmNoise::~FbmNoise() {
 }


 static void FbmNoise::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FbmNoise::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FbmNoise::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_type"), &FbmNoise::get_type);
   ClassDB::bind_method(D_METHOD("set_type", "value"), &FbmNoise::set_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");


   ClassDB::bind_method(D_METHOD("get_scale"), &FbmNoise::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &FbmNoise::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


   ClassDB::bind_method(D_METHOD("get_folds"), &FbmNoise::get_folds);
   ClassDB::bind_method(D_METHOD("set_folds", "value"), &FbmNoise::set_folds);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "folds"), "set_folds", "get_folds");


   ClassDB::bind_method(D_METHOD("get_iterations"), &FbmNoise::get_iterations);
   ClassDB::bind_method(D_METHOD("set_iterations", "value"), &FbmNoise::set_iterations);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "iterations"), "set_iterations", "get_iterations");


   ClassDB::bind_method(D_METHOD("get_persistence"), &FbmNoise::get_persistence);
   ClassDB::bind_method(D_METHOD("set_persistence", "value"), &FbmNoise::set_persistence);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "persistence"), "set_persistence", "get_persistence");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FbmNoise::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FbmNoise::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FbmNoise::_get_value_for);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FbmNoise::_render);
  ClassDB::bind_method(D_METHOD("get_type"), &FbmNoise::get_type);
  ClassDB::bind_method(D_METHOD("set_type", "val"), &FbmNoise::set_type);
  ClassDB::bind_method(D_METHOD("get_scale"), &FbmNoise::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &FbmNoise::set_scale);
  ClassDB::bind_method(D_METHOD("get_folds"), &FbmNoise::get_folds);
  ClassDB::bind_method(D_METHOD("set_folds", "val"), &FbmNoise::set_folds);
  ClassDB::bind_method(D_METHOD("get_iterations"), &FbmNoise::get_iterations);
  ClassDB::bind_method(D_METHOD("set_iterations", "val"), &FbmNoise::set_iterations);
  ClassDB::bind_method(D_METHOD("get_persistence"), &FbmNoise::get_persistence);
  ClassDB::bind_method(D_METHOD("set_persistence", "val"), &FbmNoise::set_persistence);

 }



