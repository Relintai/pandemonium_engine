
#include "lastopenedfiles.h"


EditorPlugin LastOpenedFiles::get_*editor_plugin() {
 return *editor_plugin;
}

void LastOpenedFiles::set_*editor_plugin(const EditorPlugin &val) {
*editor_plugin = val;
}


Ref<EditorSettings> LastOpenedFiles::get_editor_settings() {
 return editor_settings;
}

void LastOpenedFiles::set_editor_settings(const Ref<EditorSettings> &val) {
editor_settings = val;
}



 //tool;
  EditorPlugin *editor_plugin = null;
  Ref<EditorSettings> editor_settings = null;

 void LastOpenedFiles::store_opened_files(const Control &filecontainer) {
   Array arr = Array();

  for (int child = 0; child < filecontainer.get_item_count(); ++child) { //child in range(filecontainer.get_item_count())
    String filepath = filecontainer.get_item_metadata(child)[0].current_path;
    Array a = Array();
   a.push_back(filepath.get_file());
   a.push_back(filepath);
   arr.push_back(a);
}

  editor_settings.set_project_metadata("file_editor", "files", arr);
}


 void LastOpenedFiles::remove_opened_file(const int index, const Control &filecontainer) {
   String filepath = filecontainer.get_item_metadata(index)[0].current_path;
   String f = filepath.get_file();
   Array arr = editor_settings.get_project_metadata("file_editor", "files", Array());

  for (int i = 0; i < arr.size(); ++i) { //i in range(arr.size())
    Array a = arr[i];

   if (a[0] == f) {
    arr.remove(i);
    break;
}

}

  editor_settings.set_project_metadata("file_editor", "files", arr);
   Dictionary fonts_dict = editor_settings.get_project_metadata("file_editor", "file_fonts", Dictionary());

  if (fonts_dict.has(f)) {
   fonts_dict.erase(f);
   editor_settings.set_project_metadata("file_editor", "file_fonts", fonts_dict);
}

}


 Array LastOpenedFiles::load_opened_files() {
   Array arr = editor_settings.get_project_metadata("file_editor", "files", Array());
   Dictionary fonts_dict = editor_settings.get_project_metadata("file_editor", "file_fonts", Dictionary());
   Array keys = Array();

  for (int i = 0; i < arr.size(); ++i) { //i in range(arr.size())
    Array a = arr[i];
   // creating and returning an Array with this format [1:file name, 2:file path, 3:file font];
    Array k = ;
   k.push_back(a[0]);
   k.push_back(a[1]);

   if (fonts_dict.has(a[0])) {
    k.push_back(fonts_dict[a[0]]);
}


   else {
    k.push_back("null");
}

   keys.append(k);
}

  return keys;
}


 void LastOpenedFiles::store_editor_fonts(const String &file_name, const String &font_path) {
   Dictionary fonts_dict = editor_settings.get_project_metadata("file_editor", "file_fonts", Dictionary());
  fonts_dict[file_name] = font_path;
  editor_settings.set_project_metadata("file_editor", "file_fonts", fonts_dict);
}


 String LastOpenedFiles::get_editor_font() {
  //var editor_plugin : EditorPlugin = EditorPlugin.new();
  return editor_plugin.get_editor_interface().get_editor_settings().get_setting("interface/editor/code_font");
}

}

 LastOpenedFiles::LastOpenedFiles() {
  *editor_plugin = null;
  editor_settings = null;
 }

 LastOpenedFiles::~LastOpenedFiles() {
 }


 static void LastOpenedFiles::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*editor_plugin"), &LastOpenedFiles::get_*editor_plugin);
   ClassDB::bind_method(D_METHOD("set_*editor_plugin", "value"), &LastOpenedFiles::set_*editor_plugin);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*editor_plugin", PROPERTY_HINT_RESOURCE_TYPE, "EditorPlugin"), "set_*editor_plugin", "get_*editor_plugin");


   ClassDB::bind_method(D_METHOD("get_editor_settings"), &LastOpenedFiles::get_editor_settings);
   ClassDB::bind_method(D_METHOD("set_editor_settings", "value"), &LastOpenedFiles::set_editor_settings);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "editor_settings", PROPERTY_HINT_RESOURCE_TYPE, "Ref<EditorSettings>"), "set_editor_settings", "get_editor_settings");


  ClassDB::bind_method(D_METHOD("store_opened_files", "filecontainer"), &LastOpenedFiles::store_opened_files);
  ClassDB::bind_method(D_METHOD("remove_opened_file", "index", "filecontainer"), &LastOpenedFiles::remove_opened_file);
  ClassDB::bind_method(D_METHOD("load_opened_files"), &LastOpenedFiles::load_opened_files);
  ClassDB::bind_method(D_METHOD("store_editor_fonts", "file_name", "font_path"), &LastOpenedFiles::store_editor_fonts);
  ClassDB::bind_method(D_METHOD("get_editor_font"), &LastOpenedFiles::get_editor_font);

 }



