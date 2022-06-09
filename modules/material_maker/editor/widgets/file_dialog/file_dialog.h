#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H


class FileDialog : public FileDialog {
 GDCLASS(FileDialog, FileDialog);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _ready();
 String get_full_current_dir();
 void _on_FileDialog_file_selected(const Variant &path);
 void _on_FileDialog_files_selected(const Variant &paths);
 void _on_FileDialog_dir_selected(const Variant &dir);
 void _on_FileDialog_popup_hide();
 Array select_files();
 void add_favorite();

 FileDialog();
 ~FileDialog();

 protected:
 static void _bind_methods();

 //tool
 Variant  = null;
 Variant  = null;
 signal return_paths(path_list);
};


#endif
