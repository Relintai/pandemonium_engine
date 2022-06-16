
#include "fill_to_size.h"


Ref<Resource> FillToSize::get_image() {
 return image;
}

void FillToSize::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillToSize::get_input() {
 return input;
}

void FillToSize::set_input(const Ref<Resource> &val) {
input = val;
}


int FillToSize::get_formula() const {
 return formula;
}

void FillToSize::set_formula(const int val) {
formula = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "Area,Width,Height,Max(W,H)") ;
  int formula = 0;

 void FillToSize::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
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


 void FillToSize::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_formula", "set_formula", "Formula", [ "Area", "Width", "Height", "Max(W,H)" ]);
}


 void FillToSize::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillToSize::_get_value_for(const Vector2 &uv, const int pseed) {
  //vec4 $(name_uv)_bb = $in($uv);
   Color c = input.get_value(uv);
   float f = 0;
  //"Area" sqrt($(name_uv)_bb.z*$(name_uv)_bb.w);
  //"Width" $(name_uv)_bb.z;
  //"Height" $(name_uv)_bb.w;
  //"max(W, H)" max($(name_uv)_bb.z, $(name_uv)_bb.w);

  if (formula == 0) {
   f = sqrt(c.b * c.a);
}


  else if (formula == 1) {
   f = c.b;
}


  else if (formula == 2) {
   f = c.a;
}


  else if (formula == 3) {
   f = max(c.b, c.a);
}

  return Color(f, f, f, 1);
}

 //formula;

 int FillToSize::get_formula() {
  return formula;
}


 void FillToSize::set_formula(const int val) {
  formula = val;
  set_dirty(true);
}

}

 FillToSize::FillToSize() {
  image;
  input;
  formula = 0;
 }

 FillToSize::~FillToSize() {
 }


 static void FillToSize::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillToSize::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillToSize::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillToSize::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillToSize::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_formula"), &FillToSize::get_formula);
   ClassDB::bind_method(D_METHOD("set_formula", "value"), &FillToSize::set_formula);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "formula"), "set_formula", "get_formula");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillToSize::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillToSize::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillToSize::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillToSize::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_formula"), &FillToSize::get_formula);
  ClassDB::bind_method(D_METHOD("set_formula", "val"), &FillToSize::set_formula);

 }



