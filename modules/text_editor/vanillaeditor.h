#ifndef VANILLAEDITOR_H
#define VANILLAEDITOR_H


class VanillaEditor : public VBoxContainer {
 GDCLASS(VanillaEditor, VBoxContainer);

 public:

 Reference get_*LastOpenedFiles();
 void set_*LastOpenedFiles(const Reference &val);

 TextEdit get_*text_editor();
 void set_*text_editor(const TextEdit &val);

 FileDialog get_*file_list();
 void set_*file_list(const FileDialog &val);

 HBoxContainer get_*search_box();
 void set_*search_box(const HBoxContainer &val);

 LineEdit get_*search_box_line_edit();
 void set_*search_box_line_edit(const LineEdit &val);

 CheckBox get_*search_box_match_case_cb();
 void set_*search_box_match_case_cb(const CheckBox &val);

 CheckBox get_*search_box_whole_words_cb();
 void set_*search_box_whole_words_cb(const CheckBox &val);

 Button get_*search_box_close_button();
 void set_*search_box_close_button(const Button &val);

 HBoxContainer get_*replace_box();
 void set_*replace_box(const HBoxContainer &val);

 LineEdit get_*replace_box_replace_le();
 void set_*replace_box_replace_le(const LineEdit &val);

 LineEdit get_*replace_box_with();
 void set_*replace_box_with(const LineEdit &val);

 Button get_*replace_box_button();
 void set_*replace_box_button(const Button &val);

 Button get_*replace_box_close();
 void set_*replace_box_close(const Button &val);

 TextureRect get_*file_info_last_modified_icon();
 void set_*file_info_last_modified_icon(const TextureRect &val);

 Label get_*file_info_last_modified();
 void set_*file_info_last_modified(const Label &val);

 Label get_*file_info_c_counter();
 void set_*file_info_c_counter(const Label &val);

 CheckBox get_*file_info_read_only();
 void set_*file_info_read_only(const CheckBox &val);

 String get_current_path();
 void set_current_path(const String &val);

 String get_current_filename();
 void set_current_filename(const String &val);

 int get_search_flag() const;
 void set_search_flag(const int val);

 void _init();
 void _ready();
 void set_font(const String &font_path);
 void load_default_font();
 void set_wrap_enabled(const bool enabled);
 void draw_minimap(const bool value);
 void color_region(const String &filextension);
 void clean_editor();
 void new_file_open(const String &file_content, const Dictionary &last_modified, const String &current_file_path);
 void update_lastmodified(const Dictionary &last_modified, const String &icon);
 void new_file_create(const String &file_name);
 void _on_Readonly_toggled(const bool button_pressed);
 void _on_text_editor_text_changed();
 void count_characters();
 void _on_LineEdit_text_changed(const String &new_text);
 void _on_matchcase_toggled(const bool button_pressed);
 void _on_wholewords_toggled(const bool button_pressed);
 void _on_close_pressed();
 void open_search_box();
 void _on_Button_pressed();
 void open_replace_box();
 void _on_close2_pressed();
 void _on_LineEdit_focus_entered();

 VanillaEditor();
 ~VanillaEditor();

 protected:
 static void _bind_methods();

 //tool
 Reference *LastOpenedFiles = null;
 TextEdit *text_editor = null;
 FileDialog *file_list = null;
 HBoxContainer *search_box = null;
 LineEdit *search_box_line_edit = null;
 CheckBox *search_box_match_case_cb = null;
 CheckBox *search_box_whole_words_cb = null;
 Button *search_box_close_button = null;
 HBoxContainer *replace_box = null;
 LineEdit *replace_box_replace_le = null;
 LineEdit *replace_box_with = null;
 Button *replace_box_button = null;
 Button *replace_box_close = null;
 TextureRect *file_info_last_modified_icon = null;
 Label *file_info_last_modified = null;
 Label *file_info_c_counter = null;
 CheckBox *file_info_read_only = null;
 String current_path = "";
 String current_filename = "";
 int search_flag = 0;
 signal text_changed();
 //file_info_last_modified_icon.texture = IconLoader.load_icon_from_name(icon)
};


#endif
