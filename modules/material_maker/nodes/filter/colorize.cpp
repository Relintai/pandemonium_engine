
#include "colorize.h"


Ref<Resource> Colorize::get_image() {
 return image;
}

void Colorize::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Colorize::get_input() {
 return input;
}

void Colorize::set_input(const Ref<Resource> &val) {
input = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;

 void Colorize::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   input.set_default_value(1);
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


 void Colorize::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_gradient();
}


 void Colorize::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Colorize::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = input.get_value(uv);
  return get_gradient_color(f);
}

 //	return Color(0.5, 0.5, 0.5, 1);

 Color Colorize::get_gradient_color(const float x) {

  if (interpolation_type == 0) {
   return MMAlgos.gradient_type_1(x, points);
}


  else if (interpolation_type == 1) {
   return MMAlgos.gradient_type_2(x, points);
}


  else if (interpolation_type == 2) {
   return MMAlgos.gradient_type_3(x, points);
}


  else if (interpolation_type == 3) {
   return MMAlgos.gradient_type_4(x, points);
}

  return Color(1, 1, 1, 1);
}

}

 Colorize::Colorize() {
  image;
  input;
 }

 Colorize::~Colorize() {
 }


 static void Colorize::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Colorize::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Colorize::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Colorize::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Colorize::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Colorize::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Colorize::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Colorize::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Colorize::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_gradient_color", "x"), &Colorize::get_gradient_color);

 }



