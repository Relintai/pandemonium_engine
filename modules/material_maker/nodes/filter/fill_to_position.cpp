
#include "fill_to_position.h"


Ref<Resource> FillToPosition::get_image() {
 return image;
}

void FillToPosition::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillToPosition::get_input() {
 return input;
}

void FillToPosition::set_input(const Ref<Resource> &val) {
input = val;
}


int FillToPosition::get_axis() const {
 return axis;
}

void FillToPosition::set_axis(const int val) {
axis = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "X,Y,Radial") ;
  int axis = 2;

 void FillToPosition::_init_properties() {

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


 void FillToPosition::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_axis", "set_axis", "Axis", [ "X", "Y", "Radial" ]);
}


 void FillToPosition::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillToPosition::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  //vec2 $(name_uv)_c = fract($in($uv).xy+0.5*$in($uv).zw);
   Vector2 cnv = MMAlgos.fractv2(Vector2(c.r, c.g) + 0.5 * Vector2(c.b, c.a));
  //X, $(name_uv)_c.x;
  //Y, $(name_uv)_c.y;
  //Radial, length($(name_uv)_c-vec2(0.5));

  if (axis == 0) {
   return Color(cnv.x, cnv.x, cnv.x, 1);
}


  else if (axis == 1) {
   return Color(cnv.y, cnv.y, cnv.y, 1);
}


  else if (axis == 2) {
    float f = (cnv - Vector2(0.5, 0.5)).length();
   return Color(f, f, f, 1);
}

  return Color(0, 0, 0, 1);
}

 //axis;

 int FillToPosition::get_axis() {
  return axis;
}


 void FillToPosition::set_axis(const int val) {
  axis = val;
  set_dirty(true);
}

}

 FillToPosition::FillToPosition() {
  image;
  input;
  axis = 2;
 }

 FillToPosition::~FillToPosition() {
 }


 static void FillToPosition::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillToPosition::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillToPosition::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillToPosition::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillToPosition::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_axis"), &FillToPosition::get_axis);
   ClassDB::bind_method(D_METHOD("set_axis", "value"), &FillToPosition::set_axis);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillToPosition::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillToPosition::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillToPosition::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillToPosition::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_axis"), &FillToPosition::get_axis);
  ClassDB::bind_method(D_METHOD("set_axis", "val"), &FillToPosition::set_axis);

 }



