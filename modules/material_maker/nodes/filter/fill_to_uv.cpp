
#include "fill_to_uv.h"


Ref<Resource> FillToUv::get_image() {
 return image;
}

void FillToUv::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> FillToUv::get_input() {
 return input;
}

void FillToUv::set_input(const Ref<Resource> &val) {
input = val;
}


int FillToUv::get_mode() const {
 return mode;
}

void FillToUv::set_mode(const int val) {
mode = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int, "Stretch,Square") ;
  int mode = 0;

 void FillToUv::_init_properties() {

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


 void FillToUv::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_mode", "set_mode", "Mode", [ "Stretch", "Square" ]);
}


 void FillToUv::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color FillToUv::_get_value_for(const Vector2 &uv, const int pseed) {
  //vec4 $(name_uv)_bb = $in($uv);
   Color c = input.get_value(uv);
  //fill_to_uv_$mode($uv, $(name_uv)_bb, float($seed));
   Vector3 r = Vector3();

  if (mode == 0) {
   r = MMAlgos.fill_to_uv_stretch(uv, c, float(pseed));
}


  else if (mode == 1) {
   r = MMAlgos.fill_to_uv_square(uv, c, float(pseed));
}

  return Color(r.x, r.y, r.z, 1);
}

 //mode;

 int FillToUv::get_mode() {
  return mode;
}


 void FillToUv::set_mode(const int val) {
  mode = val;
  set_dirty(true);
}

}

 FillToUv::FillToUv() {
  image;
  input;
  mode = 0;
 }

 FillToUv::~FillToUv() {
 }


 static void FillToUv::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &FillToUv::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &FillToUv::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &FillToUv::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &FillToUv::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_mode"), &FillToUv::get_mode);
   ClassDB::bind_method(D_METHOD("set_mode", "value"), &FillToUv::set_mode);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "mode"), "set_mode", "get_mode");


  ClassDB::bind_method(D_METHOD("_init_properties"), &FillToUv::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &FillToUv::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &FillToUv::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &FillToUv::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_mode"), &FillToUv::get_mode);
  ClassDB::bind_method(D_METHOD("set_mode", "val"), &FillToUv::set_mode);

 }



