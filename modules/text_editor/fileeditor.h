#ifndef FILEEDITOR_H
#define FILEEDITOR_H


class FileEditor : public Control {
 GDCLASS(FileEditor, Control);

 public:

 String get_DIRECTORY();
 void set_DIRECTORY(const String &val);

 String get_EXCEPTIONS();
 void set_EXCEPTIONS(const String &val);

 PoolStringArray get_EXTENSIONS();
 void set_EXTENSIONS(const PoolStringArray &val);

 MenuButton get_*file_btn();
 void set_*file_btn(const MenuButton &val);

 MenuButton get_*preview_btn();
 void set_*preview_btn(const MenuButton &val);

 MenuButton get_*settings_btn();
 void set_*settings_btn(const MenuButton &val);

 PopupMenu get_*file_btn_popup();
 void set_*file_btn_popup(const PopupMenu &val);

 PopupMenu get_*preview_btn_popup();
 void set_*preview_btn_popup(const PopupMenu &val);

 PopupMenu get_*settings_btn_popup();
 void set_*settings_btn_popup(const PopupMenu &val);

 HSplitContainer get_*editor_container();
 void set_*editor_container(const HSplitContainer &val);

 VBoxContainer get_*file_container();
 void set_*file_container(const VBoxContainer &val);

 ItemList get_*open_file_list();
 void set_*open_file_list(const ItemList &val);

 VBoxContainer get_*split_editor_container();
 void set_*split_editor_container(const VBoxContainer &val);

 LineEdit get_*open_file_name();
 void set_*open_file_name(const LineEdit &val);

 OptionButton get_*wrap_btn();
 void set_*wrap_btn(const OptionButton &val);

 OptionButton get_*map_btn();
 void set_*map_btn(const OptionButton &val);

 FileDialog get_*file_list();
 void set_*file_list(const FileDialog &val);

 AcceptDialog get_*new_file_dialogue();
 void set_*new_file_dialogue(const AcceptDialog &val);

 LineEdit get_*new_file_dialogue_name();
 void set_*new_file_dialogue_name(const LineEdit &val);

 ConfirmationDialog get_*confirmation_close();
 void set_*confirmation_close(const ConfirmationDialog &val);

 FileDialog get_*select_font_dialog();
 void set_*select_font_dialog(const FileDialog &val);

 Reference get_*LastOpenedFiles();
 void set_*LastOpenedFiles(const Reference &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Array get_directories();
 void set_directories(const Array &val);

 Array get_files();
 void set_files(const Array &val);

 int get_current_file_index() const;
 void set_current_file_index(const int val);

 String get_current_file_path();
 void set_current_file_path(const String &val);

 bool get_save_as() const;
 void set_save_as(const bool val);

 Control get_*current_editor();
 void set_*current_editor(const Control &val);

 Ref<DynamicFont> get_current_font();
 void set_current_font(const Ref<DynamicFont> &val);

 bool get_editing_file() const;
 void set_editing_file(const bool val);

 enum FileMenuOptions  {

  FILE_MENU_OPTION_NEW = 0,
  FILE_MENU_OPTION_OPEN = 1,
  FILE_MENU_OPTION_CLOSE = 2,
  FILE_MENU_OPTION_SAVE = 3,
  FILE_MENU_OPTION_SAVE_AS = 4,
  FILE_MENU_OPTION_DELETE = 5,
  FILE_MENU_OPTION_SEARCH = 6,
  FILE_MENU_OPTION_REPLACE = 7,
};

 void _init();
 void _ready();
 void connect_signals();
 void create_selected_file();
 void open_selected_file();
 void delete_selected_file();
 void save_current_file_as();
 void _on_file_btn_pressed(const int index);
 void _on_preview_btn_pressed(const int id);
 void _on_settings_btn_pressed(const int index);
 void _on_font_selected(const String &font_path);
 void _on_fileitem_pressed(const int index);
 void open_file(const String &path, const String &font = "null");
 void generate_file_item(const String &path, const Control &veditor);
 Control open_in_vanillaeditor(const String &path);
 void close_file(const int index);
 void confirm_close(const int index);
 void _on_update_file();
 void delete_file(const PoolStringArray &files_selected);
 void open_new_file_dialogue();
 void open_file_list();
 void create_new_file(const String &given_path);
 void save_file(const String &current_path);
 void clean_editor();
 void csv_preview();
 void bbcode_preview();
 void markdown_preview();
 void html_preview();
 void _on_vanillaeditor_text_changed();
 void update_list();
 void on_wrap_button(const int index);
 void on_minimap_button(const int index);
 void check_file_preview(const String &file);
 void _on_ConfirmationDialog_confirmed();

 FileEditor();
 ~FileEditor();

 protected:
 static void _bind_methods();

 //tool
 };
 String DIRECTORY = "res://";
 String EXCEPTIONS = "addons";
 PoolStringArray EXTENSIONS = [;
 "*.txt ; Plain Text File",;
 "*.rtf ; Rich Text Format File",;
 "*.log ; Log File",;
 "*.md ; MD File",;
 "*.doc ; WordPad Document",;
 "*.doc ; Microsoft Word Document",;
 "*.docm ; Word Open XML Macro-Enabled Document",;
 "*.docx ; Microsoft Word Open XML Document",;
 "*.bbs ; Bulletin Board System Text",;
 "*.dat ; Data File",;
 "*.xml ; XML File",;
 "*.sql ; SQL database file",;
 "*.json ; JavaScript Object Notation File",;
 "*.html ; HyperText Markup Language",;
 "*.csv ; Comma-separated values",;
 "*.cfg ; Configuration File",;
 "*.ini ; Initialization File (same as .cfg Configuration File)",;
 "*.csv ; Comma-separated values File",;
 "*.res ; Resource File",;
 ];
 MenuButton *file_btn = null;
 MenuButton *preview_btn = null;
 MenuButton *settings_btn = null;
 PopupMenu *file_btn_popup = null;
 PopupMenu *preview_btn_popup = null;
 PopupMenu *settings_btn_popup = null;
 HSplitContainer *editor_container = null;
 VBoxContainer *file_container = null;
 ItemList *open_file_list = null;
 VBoxContainer *split_editor_container = null;
 LineEdit *open_file_name = null;
 OptionButton *wrap_btn = null;
 OptionButton *map_btn = null;
 FileDialog *file_list = null;
 AcceptDialog *new_file_dialogue = null;
 LineEdit *new_file_dialogue_name = null;
 ConfirmationDialog *confirmation_close = null;
 FileDialog *select_font_dialog = null;
 Reference *LastOpenedFiles = null;
 Variant  = preload("res://addons/file-editor/scripts/Preview.gd");
 Variant  = preload("res://addons/file-editor/scripts/VanillaEditor.gd");
 Array directories = ;
 Array files = ;
 int current_file_index = -1;
 String current_file_path = ;
 bool save_as = false;
 Control *current_editor;
 Ref<DynamicFont> current_font;
 bool editing_file = false;
};


#endif
