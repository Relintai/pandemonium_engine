
#include "file_dialog.h"


Variant FileDialog::get_Variant() {
 return Variant;
}

void FileDialog::set_Variant(const Variant &val) {
Variant = val;
}


Variant FileDialog::get_Variant() {
 return Variant;
}

void FileDialog::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
  Variant  = null;
  Variant  = null;
 signal return_paths(path_list);

 void FileDialog::_ready() {
   Variant  = get_child(3);
   Variant  = HSplitContainer.new();
  add_child(hbox);
  remove_child(vbox);
  left_panel = preload("res://addons/mat_maker_gd/windows/file_dialog/left_panel.tscn").instance();
  hbox.add_child(left_panel);
  left_panel.connect("open_directory", self, "set_current_dir");
  hbox.add_child(vbox);
  vbox.size_flags_horizontal = SIZE_EXPAND_FILL;
  //var fav_button = preload("res://addons/mat_maker_gd/windows/file_dialog/fav_button.tscn").instance();
  vbox.get_child(0).add_child(fav_button);
  fav_button.connect("pressed", self, "add_favorite");

  if (OS.get_name() == "Windows") {
   volume_option = vbox.get_child(0).get_child(3);

   if (! volume_option is OptionButton) {
    volume_option = null;
}

}

}


 String FileDialog::get_full_current_dir() {
   Variant  = "";

  if (volume_option != null && volume_option.visible) {
   prefix = volume_option.get_item_text(volume_option.selected);
}

  return prefix+get_current_dir();
}


 void FileDialog::_on_FileDialog_file_selected(const Variant &path) {
  left_panel.add_recent(get_full_current_dir());
  emit_signal("return_paths", [ path ]);
}


 void FileDialog::_on_FileDialog_files_selected(const Variant &paths) {
  left_panel.add_recent(get_full_current_dir());
  emit_signal("return_paths", paths);
}


 void FileDialog::_on_FileDialog_dir_selected(const Variant &dir) {
  emit_signal("return_paths", [ dir ]);
}


 void FileDialog::_on_FileDialog_popup_hide() {
  emit_signal("return_paths", [ ]);
}


 Array FileDialog::select_files() {
  popup_centered();
   Variant  = yield(self, "return_paths");
  queue_free();
  return result;
}


 void FileDialog::add_favorite() {
  left_panel.add_favorite(get_full_current_dir());
}

}

 FileDialog::FileDialog() {
   = null;
   = null;
 }

 FileDialog::~FileDialog() {
 }


 static void FileDialog::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &FileDialog::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &FileDialog::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &FileDialog::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &FileDialog::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_ready"), &FileDialog::_ready);
  ClassDB::bind_method(D_METHOD("get_full_current_dir"), &FileDialog::get_full_current_dir);
  ClassDB::bind_method(D_METHOD("_on_FileDialog_file_selected", "path"), &FileDialog::_on_FileDialog_file_selected);
  ClassDB::bind_method(D_METHOD("_on_FileDialog_files_selected", "paths"), &FileDialog::_on_FileDialog_files_selected);
  ClassDB::bind_method(D_METHOD("_on_FileDialog_dir_selected", "dir"), &FileDialog::_on_FileDialog_dir_selected);
  ClassDB::bind_method(D_METHOD("_on_FileDialog_popup_hide"), &FileDialog::_on_FileDialog_popup_hide);
  ClassDB::bind_method(D_METHOD("select_files"), &FileDialog::select_files);
  ClassDB::bind_method(D_METHOD("add_favorite"), &FileDialog::add_favorite);

 }



