
#include "iconloader.h"



 //tool;

 ImageTexture IconLoader::load_icon_from_name(const String &icon_name) {
   File *file = File.new();
   Ref<Image> image = Image.new();
   Ref<ImageTexture> texture = ImageTexture.new();
  file.open("res://addons/file-editor/icons.pngs/"+icon_name+".png.buttonicon", File.READ);
   PoolByteArray buffer = file.get_buffer(file.get_len());
  file.close();
  image.load_png_from_buffer(buffer);
  texture.create_from_image(image);
  return texture;
}

}

 IconLoader::IconLoader() {
 }

 IconLoader::~IconLoader() {
 }


 static void IconLoader::_bind_methods() {
  ClassDB::bind_method(D_METHOD("load_icon_from_name", "icon_name"), &IconLoader::load_icon_from_name);

 }



