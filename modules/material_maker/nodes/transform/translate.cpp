
#include "translate.h"


Ref<Resource> Translate::get_image() {
 return image;
}

void Translate::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Translate::get_input() {
 return input;
}

void Translate::set_input(const Ref<Resource> &val) {
input = val;
}


Vector2 Translate::get_translation() {
 return translation;
}

void Translate::set_translation(const Vector2 &val) {
translation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Vector2) ;
  Vector2 translation = Vector2();

 void Translate::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";

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


 void Translate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_vector2("get_translation", "set_translation", "Translation", 0.01);
}


 void Translate::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Translate::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i($uv-vec2($translate_x, $translate_y));
  return input.get_value(uv - translation);
}

 //translation;

 Vector2 Translate::get_translation() {
  return translation;
}


 void Translate::set_translation(const Vector2 &val) {
  translation = val;
  set_dirty(true);
}

}

 Translate::Translate() {
  image;
  input;
  translation = Vector2();
 }

 Translate::~Translate() {
 }


 static void Translate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Translate::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Translate::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Translate::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Translate::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_translation"), &Translate::get_translation);
   ClassDB::bind_method(D_METHOD("set_translation", "value"), &Translate::set_translation);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "translation"), "set_translation", "get_translation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Translate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Translate::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Translate::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Translate::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_translation"), &Translate::get_translation);
  ClassDB::bind_method(D_METHOD("set_translation", "val"), &Translate::set_translation);

 }



