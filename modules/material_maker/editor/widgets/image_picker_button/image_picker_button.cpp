
#include "image_picker_button.h"


Variant ImagePickerButton::get_Variant() {
 return Variant;
}

void ImagePickerButton::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
  Variant  = "";
 signal on_file_selected(f);

 void ImagePickerButton::_ready() {
  texture_normal = ImageTexture.new();
}


 void ImagePickerButton::do_set_image_path(const Variant &path) {

  if (path == null) {
   return;
}

  image_path = path;
  texture_normal.load(image_path);
}


 void ImagePickerButton::set_image_path(const Variant &path) {
  do_set_image_path(path);
  emit_signal("on_file_selected", path);
}


 void ImagePickerButton::_on_ImagePicker_pressed() {
  //var dialog = preload("res://addons/mat_maker_gd/windows/file_dialog/file_dialog.tscn").instance();
  add_child(dialog);
  dialog.rect_min_size = Vector2(500, 500);
  dialog.access = FileDialog.ACCESS_FILESYSTEM;
  dialog.mode = FileDialog.MODE_OPEN_FILE;
  dialog.add_filter("*.bmp;BMP Image");
  dialog.add_filter("*.exr;EXR Image");
  dialog.add_filter("*.hdr;Radiance HDR Image");
  dialog.add_filter("*.jpg,*.jpeg;JPEG Image");
  dialog.add_filter("*.png;PNG Image");
  dialog.add_filter("*.svg;SVG Image");
  dialog.add_filter("*.tga;TGA Image");
  dialog.add_filter("*.webp;WebP Image");
   Variant  = dialog.select_files();

  while (files is GDScriptFunctionState) {
   files = yield(files, "completed");
}


  if (files.size() > 0) {
   set_image_path(files[0]);
}

}


 void ImagePickerButton::on_drop_image_file(const String &file_name) {
  set_image_path(file_name);
}

}

 ImagePickerButton::ImagePickerButton() {
   = "";
 }

 ImagePickerButton::~ImagePickerButton() {
 }


 static void ImagePickerButton::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &ImagePickerButton::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &ImagePickerButton::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_ready"), &ImagePickerButton::_ready);
  ClassDB::bind_method(D_METHOD("do_set_image_path", "path"), &ImagePickerButton::do_set_image_path);
  ClassDB::bind_method(D_METHOD("set_image_path", "path"), &ImagePickerButton::set_image_path);
  ClassDB::bind_method(D_METHOD("_on_ImagePicker_pressed"), &ImagePickerButton::_on_ImagePicker_pressed);
  ClassDB::bind_method(D_METHOD("on_drop_image_file", "file_name"), &ImagePickerButton::on_drop_image_file);

 }



