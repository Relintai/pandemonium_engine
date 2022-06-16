
#include "output_image.h"


Ref<Resource> OutputImage::get_image() {
 return image;
}

void OutputImage::set_image(const Ref<Resource> &val) {
image = val;
}


String OutputImage::get_postfix() {
 return postfix;
}

void OutputImage::set_postfix(const String &val) {
postfix = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(String) ;
  String postfix = "";

 void OutputImage::_init_properties() {
  image = MMNodeUniversalProperty.new();
  image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
  image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  image.slot_name = "image";
  register_input_property(image);
}


 void OutputImage::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_line_edit("get_postfix", "set_postfix", "postfix");
}


 void OutputImage::_render(const Variant &material) {

  if (!image) {
   return;
}

   Ref<Image> img = image.get_active_image();

  if (!img) {
   return;
}

   String matpath = material.get_path();

  if (matpath == "") {
   return;
}

   String matbn = matpath.get_basename();
   String final_file_name = matbn + postfix + ".png";
  img.save_png(final_file_name);
}


 String OutputImage::get_postfix() {
  return postfix;
}


 void OutputImage::set_postfix(const String &pf) {
  postfix = pf;
  set_dirty(true);
}

}

 OutputImage::OutputImage() {
  image;
  postfix = "";
 }

 OutputImage::~OutputImage() {
 }


 static void OutputImage::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &OutputImage::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &OutputImage::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_postfix"), &OutputImage::get_postfix);
   ClassDB::bind_method(D_METHOD("set_postfix", "value"), &OutputImage::set_postfix);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "postfix"), "set_postfix", "get_postfix");


  ClassDB::bind_method(D_METHOD("_init_properties"), &OutputImage::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &OutputImage::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &OutputImage::_render);
  ClassDB::bind_method(D_METHOD("get_postfix"), &OutputImage::get_postfix);
  ClassDB::bind_method(D_METHOD("set_postfix", "pf"), &OutputImage::set_postfix);

 }



