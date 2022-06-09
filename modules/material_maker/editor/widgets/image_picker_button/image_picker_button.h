#ifndef IMAGE_PICKER_BUTTON_H
#define IMAGE_PICKER_BUTTON_H


class ImagePickerButton : public TextureButton {
 GDCLASS(ImagePickerButton, TextureButton);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _ready();
 void do_set_image_path(const Variant &path);
 void set_image_path(const Variant &path);
 void _on_ImagePicker_pressed();
 void on_drop_image_file(const String &file_name);

 ImagePickerButton();
 ~ImagePickerButton();

 protected:
 static void _bind_methods();

 //tool
 Variant  = "";
 signal on_file_selected(f);
};


#endif
