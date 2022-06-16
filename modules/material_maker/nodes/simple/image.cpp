
#include "image.h"


Ref<Resource> Image::get_image() {
 return image;
}

void Image::set_image(const Ref<Resource> &val) {
image = val;
}


String Image::get_image_path() {
 return image_path;
}

void Image::set_image_path(const String &val) {
image_path = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(String) ;
  String image_path = ;

 void Image::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Image::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_image_path_universal(image, "get_image_path", "set_image_path");
}

 //func _render(material) -> void:;
 //	var img : Image = render_image(material);
 //;
 //	image.set_value(img);

 Color Image::_get_value_for(const Vector2 &uv, const int pseed) {
  return image.get_value(uv);
}


 String Image::get_image_path() {
  return image_path;
}


 void Image::set_image_path(const String &val) {
  image_path = val;
   Ref<Image> img = Image.new();

  if (image_path && image_path != "") {
   img.load(image_path);
}

  image.set_value(img);
  set_dirty(true);
}

}

 Image::Image() {
  image;
  image_path = ;
 }

 Image::~Image() {
 }


 static void Image::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Image::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Image::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_image_path"), &Image::get_image_path);
   ClassDB::bind_method(D_METHOD("set_image_path", "value"), &Image::set_image_path);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_path"), "set_image_path", "get_image_path");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Image::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Image::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Image::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_image_path"), &Image::get_image_path);
  ClassDB::bind_method(D_METHOD("set_image_path", "val"), &Image::set_image_path);

 }



