
#include "tonality.h"


Ref<Resource> Tonality::get_image() {
 return image;
}

void Tonality::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Tonality::get_input() {
 return input;
}

void Tonality::set_input(const Ref<Resource> &val) {
input = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;

 void Tonality::_init() {
  init_points_01();
}


 void Tonality::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input.set_default_value(0);
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


 void Tonality::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_curve();
}


 void Tonality::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Tonality::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = input.get_value(uv);
   float cf = MMAlgos.curve(f, points_array);
  return Color(cf, cf, cf, 1);
}


 void Tonality::_curve_changed() {
  set_dirty(true);
}

}

 Tonality::Tonality() {
  image;
  input;
 }

 Tonality::~Tonality() {
 }


 static void Tonality::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Tonality::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Tonality::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Tonality::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Tonality::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


  ClassDB::bind_method(D_METHOD("_init"), &Tonality::_init);
  ClassDB::bind_method(D_METHOD("_init_properties"), &Tonality::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Tonality::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Tonality::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Tonality::_get_value_for);
  ClassDB::bind_method(D_METHOD("_curve_changed"), &Tonality::_curve_changed);

 }



