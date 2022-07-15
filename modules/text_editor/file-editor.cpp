
#include "file-editor.h"


Variant File-editor::get_Variant() {
 return Variant;
}

void File-editor::set_Variant(const Variant &val) {
Variant = val;
}


Variant File-editor::get_Variant() {
 return Variant;
}

void File-editor::set_Variant(const Variant &val) {
Variant = val;
}


Variant File-editor::get_Variant() {
 return Variant;
}

void File-editor::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //var IconLoader = preload("res://addons/file-editor/scripts/IconLoader.gd").new();
 //var LastOpenedFiles = preload("res://addons/file-editor/scripts/LastOpenedFiles.gd").new();
  Variant ;

 void File-editor::_enter_tree() {
  LastOpenedFiles.editor_plugin = self;
  LastOpenedFiles.editor_settings = get_editor_interface().get_editor_settings();
  FileEditor = preload("res://addons/file-editor/scripts/FileEditor.gd").new();
  FileEditor.LastOpenedFiles = LastOpenedFiles;
  get_editor_interface().get_editor_viewport().add_child(FileEditor);
  FileEditor.hide();
}


 void File-editor::_exit_tree() {
  get_editor_interface().get_editor_viewport().remove_child(FileEditor);
}


 void File-editor::has_main_screen() {
  return true;
}


 void File-editor::get_plugin_name() {
  return "File";
}


 void File-editor::get_plugin_icon() {
  return IconLoader.load_icon_from_name("file");
}


 void File-editor::make_visible(const Variant &visible) {
  FileEditor.visible = visible;
}

}

 File-editor::File-editor() {
 //var IconLoader = preload("res://addons/file-editor/scripts/IconLoader.gd").new();
 //var LastOpenedFiles = preload("res://addons/file-editor/scripts/LastOpenedFiles.gd").new();
  ;
 }

 File-editor::~File-editor() {
 }


 static void File-editor::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &File-editor::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &File-editor::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &File-editor::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &File-editor::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &File-editor::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &File-editor::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_enter_tree"), &File-editor::_enter_tree);
  ClassDB::bind_method(D_METHOD("_exit_tree"), &File-editor::_exit_tree);
  ClassDB::bind_method(D_METHOD("has_main_screen"), &File-editor::has_main_screen);
  ClassDB::bind_method(D_METHOD("get_plugin_name"), &File-editor::get_plugin_name);
  ClassDB::bind_method(D_METHOD("get_plugin_icon"), &File-editor::get_plugin_icon);
  ClassDB::bind_method(D_METHOD("make_visible", "visible"), &File-editor::make_visible);

 }



