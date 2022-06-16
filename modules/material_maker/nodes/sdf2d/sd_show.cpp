
#include "sd_show.h"


Ref<Resource> SdShow::get_image() {
 return image;
}

void SdShow::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> SdShow::get_input() {
 return input;
}

void SdShow::set_input(const Ref<Resource> &val) {
input = val;
}


float SdShow::get_bevel() const {
 return bevel;
}

void SdShow::set_bevel(const float val) {
bevel = val;
}


float SdShow::get_base() const {
 return base;
}

void SdShow::set_base(const float val) {
base = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(float) ;
  float bevel = 0;
 //export(float) ;
  float base = 0;

 void SdShow::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  //for some reason this doesn't work, todo check;
  //	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = "Input";
  register_output_property(image);
  register_input_property(input);
}


 void SdShow::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_float("get_bevel", "set_bevel", "Bevel", 0.01);
  mm_graph_node.add_slot_float("get_base", "set_base", "Base", 0.01);
}


 void SdShow::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color SdShow::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = input.get_value(uv);
  //clamp($base-$in($uv)/max($bevel, 0.00001), 0.0, 1.0);
   float cf = clamp(base - f / max(bevel, 0.00001), 0.0, 1.0);
  return Color(cf, cf, cf, 1);
}

 //bevel;

 float SdShow::get_bevel() {
  return bevel;
}


 void SdShow::set_bevel(const float val) {
  bevel = val;
  set_dirty(true);
}

 //base;

 float SdShow::get_base() {
  return base;
}


 void SdShow::set_base(const float val) {
  base = val;
  set_dirty(true);
}

}

 SdShow::SdShow() {
  image;
  input;
  bevel = 0;
  base = 0;
 }

 SdShow::~SdShow() {
 }


 static void SdShow::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &SdShow::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &SdShow::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &SdShow::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &SdShow::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_bevel"), &SdShow::get_bevel);
   ClassDB::bind_method(D_METHOD("set_bevel", "value"), &SdShow::set_bevel);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "bevel"), "set_bevel", "get_bevel");


   ClassDB::bind_method(D_METHOD("get_base"), &SdShow::get_base);
   ClassDB::bind_method(D_METHOD("set_base", "value"), &SdShow::set_base);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "base"), "set_base", "get_base");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShow::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShow::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &SdShow::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &SdShow::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_bevel"), &SdShow::get_bevel);
  ClassDB::bind_method(D_METHOD("set_bevel", "val"), &SdShow::set_bevel);
  ClassDB::bind_method(D_METHOD("get_base"), &SdShow::get_base);
  ClassDB::bind_method(D_METHOD("set_base", "val"), &SdShow::set_base);

 }



