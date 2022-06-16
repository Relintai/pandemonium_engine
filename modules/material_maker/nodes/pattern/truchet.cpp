
#include "truchet.h"


Ref<Resource> Truchet::get_image() {
 return image;
}

void Truchet::set_image(const Ref<Resource> &val) {
image = val;
}


int Truchet::get_shape() const {
 return shape;
}

void Truchet::set_shape(const int val) {
shape = val;
}


float Truchet::get_size() const {
 return size;
}

void Truchet::set_size(const float val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(int, "Line,Circle") ;
  int shape = 0;
 //export(float) ;
  float size = 4;

 void Truchet::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Truchet::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_shape", "set_shape", "Shape", [ "Line", "Circle" ]);
  mm_graph_node.add_slot_float("get_size", "set_size", "Size", 1);
}


 void Truchet::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Truchet::_get_value_for(const Vector2 &uv, const int pseed) {

  if (shape == 0) {
   return MMAlgos.truchet1c(uv, size, pseed);
}


  else if (shape == 1) {
   return MMAlgos.truchet2c(uv, size, pseed);
}

  return Color();
}

 //shape;

 int Truchet::get_shape() {
  return shape;
}


 void Truchet::set_shape(const int val) {
  shape = val;
  set_dirty(true);
}

 //size;

 float Truchet::get_size() {
  return size;
}


 void Truchet::set_size(const float val) {
  size = val;
  set_dirty(true);
}

}

 Truchet::Truchet() {
  image;
  shape = 0;
  size = 4;
 }

 Truchet::~Truchet() {
 }


 static void Truchet::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Truchet::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Truchet::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_shape"), &Truchet::get_shape);
   ClassDB::bind_method(D_METHOD("set_shape", "value"), &Truchet::set_shape);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "shape"), "set_shape", "get_shape");


   ClassDB::bind_method(D_METHOD("get_size"), &Truchet::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Truchet::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Truchet::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Truchet::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Truchet::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Truchet::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_shape"), &Truchet::get_shape);
  ClassDB::bind_method(D_METHOD("set_shape", "val"), &Truchet::set_shape);
  ClassDB::bind_method(D_METHOD("get_size"), &Truchet::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Truchet::set_size);

 }



