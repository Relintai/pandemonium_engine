#ifndef FILE-EDITOR_H
#define FILE-EDITOR_H


class File-editor : public EditorPlugin {
 GDCLASS(File-editor, EditorPlugin);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _enter_tree();
 void _exit_tree();
 void has_main_screen();
 void get_plugin_name();
 void get_plugin_icon();
 void make_visible(const Variant &visible);

 File-editor();
 ~File-editor();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/file-editor/scripts/IconLoader.gd").new();
 Variant  = preload("res://addons/file-editor/scripts/LastOpenedFiles.gd").new();
 Variant ;
};


#endif
