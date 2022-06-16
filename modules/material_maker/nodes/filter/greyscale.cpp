
#include "greyscale.h"


Ref<Resource> Greyscale::get_image() {
 return image;
}

void Greyscale::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Greyscale::get_input() {
 return input;
}

void Greyscale::set_input(const Ref<Resource> &val) {
input = val;
}


int Greyscale::get_type() const {
 return type;
}

void Greyscale::set_type(const int val) {
type = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "Lightness,Average,Luminosity,Min,Max") ;
  int type = 2;

 void Greyscale::_init_properties() {

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


 void Greyscale::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_type", "set_type", "Type", [ "Lightness", "Average", "Luminosity", "Min", "Max" ]);
}


 void Greyscale::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Greyscale::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
   float f = 0;

  if (type == 0) {
   f = MMAlgos.grayscale_lightness(Vector3(c.r, c.g, c.b));
}


  else if (type == 1) {
   f = MMAlgos.grayscale_average(Vector3(c.r, c.g, c.b));
}


  else if (type == 2) {
   f = MMAlgos.grayscale_luminosity(Vector3(c.r, c.g, c.b));
}


  else if (type == 3) {
   f = MMAlgos.grayscale_min(Vector3(c.r, c.g, c.b));
}


  else if (type == 4) {
   f = MMAlgos.grayscale_max(Vector3(c.r, c.g, c.b));
}

  return Color(f, f, f, c.a);
}

 //type;

 int Greyscale::get_type() {
  return type;
}


 void Greyscale::set_type(const int val) {
  type = val;
  set_dirty(true);
}

}

 Greyscale::Greyscale() {
  image;
  input;
  type = 2;
 }

 Greyscale::~Greyscale() {
 }


 static void Greyscale::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Greyscale::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Greyscale::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Greyscale::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Greyscale::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_type"), &Greyscale::get_type);
   ClassDB::bind_method(D_METHOD("set_type", "value"), &Greyscale::set_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Greyscale::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Greyscale::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Greyscale::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Greyscale::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_type"), &Greyscale::get_type);
  ClassDB::bind_method(D_METHOD("set_type", "val"), &Greyscale::set_type);

 }



