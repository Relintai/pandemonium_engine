
#include "runes.h"


Ref<Resource> Runes::get_image() {
 return image;
}

void Runes::set_image(const Ref<Resource> &val) {
image = val;
}


Vector2 Runes::get_size() {
 return size;
}

void Runes::set_size(const Vector2 &val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Vector2) ;
  Vector2 size = Vector2(4, 4);

 void Runes::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Runes::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_vector2("get_size", "set_size", "Size", 1);
}


 void Runes::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Runes::_get_value_for(const Vector2 &uv, const int pseed) {
   float ps = 1.0 / float(pseed);
  //Rune(vec2($columns, $rows)*$uv, float($seed));
  return MMAlgos.runesc(uv, size, ps);
}

 //size;

 Vector2 Runes::get_size() {
  return size;
}


 void Runes::set_size(const Vector2 &val) {
  size = val;
  set_dirty(true);
}

}

 Runes::Runes() {
  image;
  size = Vector2(4, 4);
 }

 Runes::~Runes() {
 }


 static void Runes::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Runes::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Runes::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_size"), &Runes::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Runes::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Runes::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Runes::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Runes::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Runes::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_size"), &Runes::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Runes::set_size);

 }



