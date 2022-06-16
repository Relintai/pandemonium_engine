
#include "invert.h"


Ref<Resource> Invert::get_image() {
 return image;
}

void Invert::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Invert::get_input() {
 return input;
}

void Invert::set_input(const Ref<Resource> &val) {
input = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;

 void Invert::_init_properties() {

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


 void Invert::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
}


 void Invert::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Invert::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  return MMAlgos.invert(c);
}

}

 Invert::Invert() {
  image;
  input;
 }

 Invert::~Invert() {
 }


 static void Invert::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Invert::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Invert::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Invert::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Invert::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Invert::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Invert::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Invert::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Invert::_get_value_for);

 }



