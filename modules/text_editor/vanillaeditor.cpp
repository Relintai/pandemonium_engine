
#include "vanillaeditor.h"


Reference VanillaEditor::get_*LastOpenedFiles() {
 return *LastOpenedFiles;
}

void VanillaEditor::set_*LastOpenedFiles(const Reference &val) {
*LastOpenedFiles = val;
}


TextEdit VanillaEditor::get_*text_editor() {
 return *text_editor;
}

void VanillaEditor::set_*text_editor(const TextEdit &val) {
*text_editor = val;
}


FileDialog VanillaEditor::get_*file_list() {
 return *file_list;
}

void VanillaEditor::set_*file_list(const FileDialog &val) {
*file_list = val;
}


HBoxContainer VanillaEditor::get_*search_box() {
 return *search_box;
}

void VanillaEditor::set_*search_box(const HBoxContainer &val) {
*search_box = val;
}


LineEdit VanillaEditor::get_*search_box_line_edit() {
 return *search_box_line_edit;
}

void VanillaEditor::set_*search_box_line_edit(const LineEdit &val) {
*search_box_line_edit = val;
}


CheckBox VanillaEditor::get_*search_box_match_case_cb() {
 return *search_box_match_case_cb;
}

void VanillaEditor::set_*search_box_match_case_cb(const CheckBox &val) {
*search_box_match_case_cb = val;
}


CheckBox VanillaEditor::get_*search_box_whole_words_cb() {
 return *search_box_whole_words_cb;
}

void VanillaEditor::set_*search_box_whole_words_cb(const CheckBox &val) {
*search_box_whole_words_cb = val;
}


Button VanillaEditor::get_*search_box_close_button() {
 return *search_box_close_button;
}

void VanillaEditor::set_*search_box_close_button(const Button &val) {
*search_box_close_button = val;
}


HBoxContainer VanillaEditor::get_*replace_box() {
 return *replace_box;
}

void VanillaEditor::set_*replace_box(const HBoxContainer &val) {
*replace_box = val;
}


LineEdit VanillaEditor::get_*replace_box_replace_le() {
 return *replace_box_replace_le;
}

void VanillaEditor::set_*replace_box_replace_le(const LineEdit &val) {
*replace_box_replace_le = val;
}


LineEdit VanillaEditor::get_*replace_box_with() {
 return *replace_box_with;
}

void VanillaEditor::set_*replace_box_with(const LineEdit &val) {
*replace_box_with = val;
}


Button VanillaEditor::get_*replace_box_button() {
 return *replace_box_button;
}

void VanillaEditor::set_*replace_box_button(const Button &val) {
*replace_box_button = val;
}


Button VanillaEditor::get_*replace_box_close() {
 return *replace_box_close;
}

void VanillaEditor::set_*replace_box_close(const Button &val) {
*replace_box_close = val;
}


TextureRect VanillaEditor::get_*file_info_last_modified_icon() {
 return *file_info_last_modified_icon;
}

void VanillaEditor::set_*file_info_last_modified_icon(const TextureRect &val) {
*file_info_last_modified_icon = val;
}


Label VanillaEditor::get_*file_info_last_modified() {
 return *file_info_last_modified;
}

void VanillaEditor::set_*file_info_last_modified(const Label &val) {
*file_info_last_modified = val;
}


Label VanillaEditor::get_*file_info_c_counter() {
 return *file_info_c_counter;
}

void VanillaEditor::set_*file_info_c_counter(const Label &val) {
*file_info_c_counter = val;
}


CheckBox VanillaEditor::get_*file_info_read_only() {
 return *file_info_read_only;
}

void VanillaEditor::set_*file_info_read_only(const CheckBox &val) {
*file_info_read_only = val;
}


String VanillaEditor::get_current_path() {
 return current_path;
}

void VanillaEditor::set_current_path(const String &val) {
current_path = val;
}


String VanillaEditor::get_current_filename() {
 return current_filename;
}

void VanillaEditor::set_current_filename(const String &val) {
current_filename = val;
}


int VanillaEditor::get_search_flag() const {
 return search_flag;
}

void VanillaEditor::set_search_flag(const int val) {
search_flag = val;
}



 //tool;
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

 void VanillaEditor::_init() {
  size_flags_vertical = SIZE_EXPAND_FILL;
  set_anchors_and_margins_preset(Control.PRESET_WIDE);
  text_editor = TextEdit.new();
  add_child(text_editor);
  text_editor.highlight_current_line = true;
  text_editor.syntax_highlighting = true;
  text_editor.show_line_numbers = true;
  text_editor.breakpoint_gutter = true;
  text_editor.highlight_all_occurrences = true;
  text_editor.override_selected_font_color = true;
  text_editor.smooth_scrolling = true;
  text_editor.hiding_enabled = true;
  //todo look this up from the editor settings;
  //text_editor.caret_blink = true;
  //text_editor.caret_blink_speed = 1;
  text_editor.caret_moving_by_right_click = false;
  text_editor.minimap_draw = true;
  text_editor.size_flags_vertical = SIZE_EXPAND_FILL;
  text_editor.set("custom_colors/member_variable_color", Color(0.737255, 0.882353, 1));
  text_editor.set("custom_colors/code_folding_color", Color(1, 1, 1, 0.701961));
  text_editor.set("custom_colors/function_color", Color(0.341176, 0.701961, 1));
  text_editor.set("custom_colors/safe_line_number_color", Color(0.8, 0.968627, 0.827451, 0.74902));
  text_editor.set("custom_colors/symbol_color", Color(0.670588, 0.788235, 1));
  text_editor.set("custom_colors/caret_background_color", Color(0, 0, 0));
  text_editor.set("custom_colors/selection_color", Color(0.411765, 0.611765, 0.909804, 0.34902));
  text_editor.set("custom_colors/caret_color", Color(1, 1, 1));
  text_editor.set("custom_colors/breakpoint_color", Color(1, 0.470588, 0.419608));
  text_editor.set("custom_colors/font_color_selected", Color(0, 0, 0));
  text_editor.set("custom_colors/font_color", Color(1, 1, 1));
  text_editor.set("custom_colors/completion_font_color", Color(1, 1, 1, 0.392157));
  text_editor.set("custom_colors/completion_scroll_color", Color(1, 1, 1, 0.070588));
  text_editor.set("custom_colors/background_color", Color(0.121569, 0.145098, 0.192157));
  text_editor.set("custom_colors/number_color", Color(0.631373, 1, 0.882353));
  text_editor.set("custom_colors/completion_background_color", Color(0.196078, 0.231373, 0.309804));
  text_editor.set("custom_colors/brace_mismatch_color", Color(1, 0.470588, 0.419608));
  text_editor.set("custom_colors/current_line_color", Color(1, 1, 1, 0.070588));
  text_editor.set("custom_colors/completion_selected_color", Color(1, 1, 1, 0.070588));
  text_editor.set("custom_colors/mark_color", Color(1, 0.470588, 0.419608, 0.301961));
  text_editor.set("custom_colors/word_highlighted_color", Color(1, 1, 1, 0.392157));
  text_editor.set("custom_colors/completion_existing_color", Color(1, 1, 1, 0.392157));
  text_editor.set("custom_constants/completion_lines", 20);
  text_editor.set("custom_constants/completion_max_width", 20);
  text_editor.set("custom_constants/completion_scroll_width", 20);
  //text_editor.owner = self;
  search_box = HBoxContainer.new();
  add_child(search_box);
  search_box.hide();
   Label *selabel = Label.new();
  search_box.add_child(selabel);
  selabel.text = "Search:";
  search_box_line_edit = LineEdit.new();
  search_box.add_child(search_box_line_edit);
  search_box_line_edit.size_flags_horizontal = SIZE_EXPAND_FILL;
  search_box_line_edit.connect("text_changed", self, "_on_LineEdit_text_changed");
  search_box_line_edit.connect("focus_entered", self, "_on_LineEdit_focus_entered");
  search_box_match_case_cb = CheckBox.new();
  search_box.add_child(search_box_match_case_cb);
  search_box_match_case_cb.text = "Match Case";
  search_box_match_case_cb.connect("toggled", self, "_on_matchcase_toggled");
  search_box_whole_words_cb = CheckBox.new();
  search_box.add_child(search_box_whole_words_cb);
  search_box_whole_words_cb.text = "Whole Words";
  search_box_whole_words_cb.connect("toggled", self, "_on_wholewords_toggled");
  search_box_close_button = Button.new();
  search_box.add_child(search_box_close_button);
  search_box_close_button.text = "x";
  search_box_close_button.flat = true;
  search_box_whole_words_cb.connect("pressed", self, "_on_close_pressed");
  replace_box = HBoxContainer.new();
  add_child(replace_box);
  replace_box.hide();
   Label *rblabel = Label.new();
  replace_box.add_child(rblabel);
  rblabel.text = "Replace:";
  replace_box_replace_le = LineEdit.new();
  replace_box.add_child(replace_box_replace_le);
  replace_box_replace_le.size_flags_horizontal = SIZE_EXPAND_FILL;
   Label *rb2label = Label.new();
  replace_box.add_child(rb2label);
  rb2label.text = "With:";
  replace_box_with = LineEdit.new();
  replace_box.add_child(replace_box_with);
  replace_box_with.size_flags_horizontal = SIZE_EXPAND_FILL;
  replace_box_button = Button.new();
  replace_box.add_child(replace_box_button);
  replace_box_button.text = "Replace";
  replace_box_button.connect("pressed", self, "_on_Button_pressed");
  replace_box_close = Button.new();
  replace_box.add_child(replace_box_close);
  replace_box_close.text = "x";
  replace_box_close.flat = true;
  replace_box_button.connect("pressed", self, "_on_close2_pressed");
   HBoxContainer *file_info = HBoxContainer.new();
  add_child(file_info);
  file_info_last_modified_icon = TextureRect.new();
  file_info.add_child(file_info_last_modified_icon);
  file_info_last_modified_icon.stretch_mode = TextureRect.STRETCH_KEEP_CENTERED;
   Label *filabel = Label.new();
  file_info.add_child(filabel);
  filabel.text = "Last modified time:";
  file_info_last_modified = Label.new();
  file_info.add_child(file_info_last_modified);
   Label *fi2label = Label.new();
  file_info.add_child(fi2label);
  fi2label.text = "Characters counter:";
  fi2label.align = Label.ALIGN_RIGHT;
  fi2label.size_flags_horizontal = SIZE_EXPAND | SIZE_FILL | SIZE_SHRINK_CENTER | SIZE_SHRINK_END;
  file_info_c_counter = Label.new();
  file_info.add_child(file_info_c_counter);
  file_info_c_counter.size_flags_horizontal = SIZE_EXPAND;
  file_info_read_only = CheckBox.new();
  file_info.add_child(file_info_read_only);
  file_info_read_only.text = "Can Edit";
  file_info_read_only.flat = true;
  file_info_read_only.size_flags_horizontal = SIZE_EXPAND | SIZE_SHRINK_END;
}


 void VanillaEditor::_ready() {
  text_editor.connect("text_changed", self, "_on_text_editor_text_changed");
  //FileList = get_parent().get_parent().get_parent().get_parent().get_node("FileList");
  file_info_read_only.connect("toggled",self,"_on_Readonly_toggled");
  //file_info_read_only.set("custom_icons/checked",IconLoader.load_icon_from_name("read"));
  //file_info_read_only.set("custom_icons/unchecked",IconLoader.load_icon_from_name("edit"));
  add_to_group("vanilla_editor");
  load_default_font();
}


 void VanillaEditor::set_font(const String &font_path) {
   Ref<DynamicFont> dynamic_font = DynamicFont.new();
   Ref<DynamicFontData> dynamic_font_data = DynamicFontData.new();
  dynamic_font_data.set_font_path(font_path);
  dynamic_font.set_font_data(dynamic_font_data);
  text_editor.set("custom_fonts/font",dynamic_font);
}


 void VanillaEditor::load_default_font() {
   String default_font = LastOpenedFiles.get_editor_font();

  if (default_font != "") {
   set_font(default_font);
}

}


 void VanillaEditor::set_wrap_enabled(const bool enabled) {
  text_editor.set_wrap_enabled(enabled);
  text_editor.update();
}


 void VanillaEditor::draw_minimap(const bool value) {
  text_editor.draw_minimap(value);
  text_editor.update();
}


 void VanillaEditor::color_region(const String &filextension) {

   {

    {
    text_editor.add_color_region("[b]","[/b]",Color8(153,153,255,255),false);
    text_editor.add_color_region("[i]","[/i]",Color8(153,255,153,255),false);
    text_editor.add_color_region("[s]","[/s]",Color8(255,153,153,255),false);
    text_editor.add_color_region("[u]","[/u]",Color8(255,255,102,255),false);
    text_editor.add_color_region("[url","[/url]",Color8(153,204,255,255),false);
    text_editor.add_color_region("[code]","[/code]",Color8(192,192,192,255),false);
    text_editor.add_color_region("[img]","[/img]",Color8(255,204,153,255),false);
    text_editor.add_color_region("[center]","[/center]",Color8(175,238,238,255),false);
    text_editor.add_color_region("[right]","[/right]",Color8(135,206,235,255),false);
}


    {
    text_editor.add_color_region("<b>","</b>",Color8(153,153,255,255),false);
    text_editor.add_color_region("<i>","</i>",Color8(153,255,153,255),false);
    text_editor.add_color_region("<del>","</del>",Color8(255,153,153,255),false);
    text_editor.add_color_region("<ins>","</ins>",Color8(255,255,102,255),false);
    text_editor.add_color_region("<a","</a>",Color8(153,204,255,255),false);
    text_editor.add_color_region("<img","/>",Color8(255,204,153,255),true);
    text_editor.add_color_region("<pre>","</pre>",Color8(192,192,192,255),false);
    text_editor.add_color_region("<center>","</center>",Color8(175,238,238,255),false);
    text_editor.add_color_region("<right>","</right>",Color8(135,206,235,255),false);
}


    {
    text_editor.add_color_region("***","***",Color8(126,186,181,255),false);
    text_editor.add_color_region("**","**",Color8(153,153,255,255),false);
    text_editor.add_color_region("*","*",Color8(153,255,153,255),false);
    text_editor.add_color_region("+ ","",Color8(255,178,102,255),false);
    text_editor.add_color_region("- ","",Color8(255,178,102,255),false);
    text_editor.add_color_region("~~","~~",Color8(255,153,153,255),false);
    text_editor.add_color_region("__","__",Color8(255,255,102,255),false);
    text_editor.add_color_region("[",")",Color8(153,204,255,255),false);
    text_editor.add_color_region("`","`",Color8(192,192,192,255),false);
    text_editor.add_color_region('"*.','"',Color8(255,255,255,255),true);
    // ","",Color8(105,105,105,255),true);
    text_editor.add_color_region(";
    //// ","",Color8(128,128,128,255),true);
    text_editor.add_color_region(";
    ////// ","",Color8(169,169,169,255),true);
    text_editor.add_color_region(";
    //////// ","",Color8(192,192,192,255),true);
    text_editor.add_color_region(";
    ////////// ","",Color8(211,211,211,255),true);
    text_editor.add_color_region(";
    //////////// ","",Color8(255,255,255,255),true);
    text_editor.add_color_region(";
    text_editor.add_color_region("> ","",Color8(172,138,79,255),true);
}


    {
    text_editor.add_color_region("[","]",Color8(153,204,255,255),false);
    text_editor.add_color_region('"','"',Color8(255,255,102,255),false);
    text_editor.add_color_region(';','',Color8(128,128,128,255),true);
}


    {
    text_editor.add_color_region("[","]",Color8(153,204,255,255),false);
    text_editor.add_color_region('"','"',Color8(255,255,102,255),false);
    text_editor.add_color_region(';','',Color8(128,128,128,255),true);
}


    {
    pass;
}

}

}


 void VanillaEditor::clean_editor() {
  text_editor.set_text("");
  //file_info_last_modified_icon.texture = IconLoader.load_icon_from_name("save");
  file_info_last_modified.set_text("");
  file_list.invalidate();
  current_filename = "";
  current_path = "";
}


 void VanillaEditor::new_file_open(const String &file_content, const Dictionary &last_modified, const String &current_file_path) {
  current_path = current_file_path;
  current_filename = current_file_path.get_file();
  color_region(current_filename.get_extension());
  text_editor.set_text(file_content);
  update_lastmodified(last_modified,"save");
  file_list.invalidate();
  count_characters();
}


 void VanillaEditor::update_lastmodified(const Dictionary &last_modified, const String &icon) {
  file_info_last_modified.set_text(str(last_modified.hour)+":"+str(last_modified.minute)+"  "+str(last_modified.day)+"/"+str(last_modified.month)+"/"+str(last_modified.year));
}

 //file_info_last_modified_icon.texture = IconLoader.load_icon_from_name(icon);

 void VanillaEditor::new_file_create(const String &file_name) {
  text_editor.set_text("");
  file_list.invalidate();
}


 void VanillaEditor::_on_Readonly_toggled(const bool button_pressed) {

  if (button_pressed) {
   file_info_read_only.set_text("Read Only");
   text_editor.readonly = (true);
}


  else {
   file_info_read_only.set_text("Can Edit");
   text_editor.readonly = (false);
}

}


 void VanillaEditor::_on_text_editor_text_changed() {
  //file_info_last_modified_icon.texture = IconLoader.load_icon_from_name("saveas");
  count_characters();
  emit_signal("text_changed");
}


 void VanillaEditor::count_characters() {
   int counted = 0;

  for (int line = 0; line < text_editor.get_line_count(); ++line) { //line in range(text_editor.get_line_count())
   counted += text_editor.get_line(line).length();
}

  file_info_c_counter.set_text(str(counted));
}


 void VanillaEditor::_on_LineEdit_text_changed(const String &new_text) {
   int linecount = text_editor.get_line_count();

  if (new_text != "") {
    PoolIntArray found = ;
    bool find = false;

   for (int line = 0; line < linecount; ++line) { //line in range(linecount)

    for (int column = 0; column < text_editor.get_line(line).length(); ++column) { //column in range(text_editor.get_line(line).length())
     found = text_editor.search( new_text, search_flag, line , column );

     if (found.size()) {

      if (found[1] == line) {
       //						if not find:;
       text_editor.select(line,found[0],found[1],found[0]+new_text.length());
}

      //							find = true;
}


     else {
      text_editor.select(0,0,0,0);
}

}

}

}


  else {
   text_editor.select(0,0,0,0);
}

}


 void VanillaEditor::_on_matchcase_toggled(const bool button_pressed) {

  if (button_pressed) {
   search_flag = 1;
}


  else {

   if (search_box_whole_words_cb.is_pressed()) {
    search_flag = 2;
}


   else {
    search_flag = 0;
}

}

  _on_LineEdit_text_changed(search_box_line_edit.get_text());
}


 void VanillaEditor::_on_wholewords_toggled(const bool button_pressed) {

  if (button_pressed) {
   search_flag = 2;
}


  else {

   if (search_box_match_case_cb.is_pressed()) {
    search_flag = 1;
}


   else {
    search_flag = 0;
}

}

  _on_LineEdit_text_changed(search_box_line_edit.get_text());
}


 void VanillaEditor::_on_close_pressed() {
  search_box.hide();
}


 void VanillaEditor::open_search_box() {

  if (search_box.visible) {
   search_box.hide();
}


  else {
   search_box.show();
   search_box.get_node("LineEdit").grab_focus();
}

}


 void VanillaEditor::_on_Button_pressed() {
   int linecount = text_editor.get_line_count()-1;
   String old_text = replace_box_replace_le.get_text();
   String new_text = replace_box_with.get_text();
   String text = text_editor.get_text();
  text_editor.set_text(text.replace(old_text,new_text));
}


 void VanillaEditor::open_replace_box() {

  if (replace_box.visible) {
   replace_box.hide();
}


  else {
   replace_box.show();
   replace_box.get_node("replace").grab_focus();
}

}


 void VanillaEditor::_on_close2_pressed() {
  replace_box.hide();
}


 void VanillaEditor::_on_LineEdit_focus_entered() {
  _on_LineEdit_text_changed(search_box_line_edit.get_text());
}

}

 VanillaEditor::VanillaEditor() {
  *LastOpenedFiles = null;
  *text_editor = null;
  *file_list = null;
  *search_box = null;
  *search_box_line_edit = null;
  *search_box_match_case_cb = null;
  *search_box_whole_words_cb = null;
  *search_box_close_button = null;
  *replace_box = null;
  *replace_box_replace_le = null;
  *replace_box_with = null;
  *replace_box_button = null;
  *replace_box_close = null;
  *file_info_last_modified_icon = null;
  *file_info_last_modified = null;
  *file_info_c_counter = null;
  *file_info_read_only = null;
  current_path = "";
  current_filename = "";
  search_flag = 0;
 }

 VanillaEditor::~VanillaEditor() {
 }


 static void VanillaEditor::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*LastOpenedFiles"), &VanillaEditor::get_*LastOpenedFiles);
   ClassDB::bind_method(D_METHOD("set_*LastOpenedFiles", "value"), &VanillaEditor::set_*LastOpenedFiles);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*LastOpenedFiles", PROPERTY_HINT_RESOURCE_TYPE, "Reference"), "set_*LastOpenedFiles", "get_*LastOpenedFiles");


   ClassDB::bind_method(D_METHOD("get_*text_editor"), &VanillaEditor::get_*text_editor);
   ClassDB::bind_method(D_METHOD("set_*text_editor", "value"), &VanillaEditor::set_*text_editor);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*text_editor", PROPERTY_HINT_RESOURCE_TYPE, "TextEdit"), "set_*text_editor", "get_*text_editor");


   ClassDB::bind_method(D_METHOD("get_*file_list"), &VanillaEditor::get_*file_list);
   ClassDB::bind_method(D_METHOD("set_*file_list", "value"), &VanillaEditor::set_*file_list);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_list", PROPERTY_HINT_RESOURCE_TYPE, "FileDialog"), "set_*file_list", "get_*file_list");


   ClassDB::bind_method(D_METHOD("get_*search_box"), &VanillaEditor::get_*search_box);
   ClassDB::bind_method(D_METHOD("set_*search_box", "value"), &VanillaEditor::set_*search_box);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*search_box", PROPERTY_HINT_RESOURCE_TYPE, "HBoxContainer"), "set_*search_box", "get_*search_box");


   ClassDB::bind_method(D_METHOD("get_*search_box_line_edit"), &VanillaEditor::get_*search_box_line_edit);
   ClassDB::bind_method(D_METHOD("set_*search_box_line_edit", "value"), &VanillaEditor::set_*search_box_line_edit);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*search_box_line_edit", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*search_box_line_edit", "get_*search_box_line_edit");


   ClassDB::bind_method(D_METHOD("get_*search_box_match_case_cb"), &VanillaEditor::get_*search_box_match_case_cb);
   ClassDB::bind_method(D_METHOD("set_*search_box_match_case_cb", "value"), &VanillaEditor::set_*search_box_match_case_cb);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*search_box_match_case_cb", PROPERTY_HINT_RESOURCE_TYPE, "CheckBox"), "set_*search_box_match_case_cb", "get_*search_box_match_case_cb");


   ClassDB::bind_method(D_METHOD("get_*search_box_whole_words_cb"), &VanillaEditor::get_*search_box_whole_words_cb);
   ClassDB::bind_method(D_METHOD("set_*search_box_whole_words_cb", "value"), &VanillaEditor::set_*search_box_whole_words_cb);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*search_box_whole_words_cb", PROPERTY_HINT_RESOURCE_TYPE, "CheckBox"), "set_*search_box_whole_words_cb", "get_*search_box_whole_words_cb");


   ClassDB::bind_method(D_METHOD("get_*search_box_close_button"), &VanillaEditor::get_*search_box_close_button);
   ClassDB::bind_method(D_METHOD("set_*search_box_close_button", "value"), &VanillaEditor::set_*search_box_close_button);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*search_box_close_button", PROPERTY_HINT_RESOURCE_TYPE, "Button"), "set_*search_box_close_button", "get_*search_box_close_button");


   ClassDB::bind_method(D_METHOD("get_*replace_box"), &VanillaEditor::get_*replace_box);
   ClassDB::bind_method(D_METHOD("set_*replace_box", "value"), &VanillaEditor::set_*replace_box);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*replace_box", PROPERTY_HINT_RESOURCE_TYPE, "HBoxContainer"), "set_*replace_box", "get_*replace_box");


   ClassDB::bind_method(D_METHOD("get_*replace_box_replace_le"), &VanillaEditor::get_*replace_box_replace_le);
   ClassDB::bind_method(D_METHOD("set_*replace_box_replace_le", "value"), &VanillaEditor::set_*replace_box_replace_le);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*replace_box_replace_le", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*replace_box_replace_le", "get_*replace_box_replace_le");


   ClassDB::bind_method(D_METHOD("get_*replace_box_with"), &VanillaEditor::get_*replace_box_with);
   ClassDB::bind_method(D_METHOD("set_*replace_box_with", "value"), &VanillaEditor::set_*replace_box_with);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*replace_box_with", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*replace_box_with", "get_*replace_box_with");


   ClassDB::bind_method(D_METHOD("get_*replace_box_button"), &VanillaEditor::get_*replace_box_button);
   ClassDB::bind_method(D_METHOD("set_*replace_box_button", "value"), &VanillaEditor::set_*replace_box_button);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*replace_box_button", PROPERTY_HINT_RESOURCE_TYPE, "Button"), "set_*replace_box_button", "get_*replace_box_button");


   ClassDB::bind_method(D_METHOD("get_*replace_box_close"), &VanillaEditor::get_*replace_box_close);
   ClassDB::bind_method(D_METHOD("set_*replace_box_close", "value"), &VanillaEditor::set_*replace_box_close);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*replace_box_close", PROPERTY_HINT_RESOURCE_TYPE, "Button"), "set_*replace_box_close", "get_*replace_box_close");


   ClassDB::bind_method(D_METHOD("get_*file_info_last_modified_icon"), &VanillaEditor::get_*file_info_last_modified_icon);
   ClassDB::bind_method(D_METHOD("set_*file_info_last_modified_icon", "value"), &VanillaEditor::set_*file_info_last_modified_icon);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_info_last_modified_icon", PROPERTY_HINT_RESOURCE_TYPE, "TextureRect"), "set_*file_info_last_modified_icon", "get_*file_info_last_modified_icon");


   ClassDB::bind_method(D_METHOD("get_*file_info_last_modified"), &VanillaEditor::get_*file_info_last_modified);
   ClassDB::bind_method(D_METHOD("set_*file_info_last_modified", "value"), &VanillaEditor::set_*file_info_last_modified);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_info_last_modified", PROPERTY_HINT_RESOURCE_TYPE, "Label"), "set_*file_info_last_modified", "get_*file_info_last_modified");


   ClassDB::bind_method(D_METHOD("get_*file_info_c_counter"), &VanillaEditor::get_*file_info_c_counter);
   ClassDB::bind_method(D_METHOD("set_*file_info_c_counter", "value"), &VanillaEditor::set_*file_info_c_counter);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_info_c_counter", PROPERTY_HINT_RESOURCE_TYPE, "Label"), "set_*file_info_c_counter", "get_*file_info_c_counter");


   ClassDB::bind_method(D_METHOD("get_*file_info_read_only"), &VanillaEditor::get_*file_info_read_only);
   ClassDB::bind_method(D_METHOD("set_*file_info_read_only", "value"), &VanillaEditor::set_*file_info_read_only);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_info_read_only", PROPERTY_HINT_RESOURCE_TYPE, "CheckBox"), "set_*file_info_read_only", "get_*file_info_read_only");


   ClassDB::bind_method(D_METHOD("get_current_path"), &VanillaEditor::get_current_path);
   ClassDB::bind_method(D_METHOD("set_current_path", "value"), &VanillaEditor::set_current_path);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_path"), "set_current_path", "get_current_path");


   ClassDB::bind_method(D_METHOD("get_current_filename"), &VanillaEditor::get_current_filename);
   ClassDB::bind_method(D_METHOD("set_current_filename", "value"), &VanillaEditor::set_current_filename);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_filename"), "set_current_filename", "get_current_filename");


   ClassDB::bind_method(D_METHOD("get_search_flag"), &VanillaEditor::get_search_flag);
   ClassDB::bind_method(D_METHOD("set_search_flag", "value"), &VanillaEditor::set_search_flag);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "search_flag"), "set_search_flag", "get_search_flag");


  ClassDB::bind_method(D_METHOD("_init"), &VanillaEditor::_init);
  ClassDB::bind_method(D_METHOD("_ready"), &VanillaEditor::_ready);
  ClassDB::bind_method(D_METHOD("set_font", "font_path"), &VanillaEditor::set_font);
  ClassDB::bind_method(D_METHOD("load_default_font"), &VanillaEditor::load_default_font);
  ClassDB::bind_method(D_METHOD("set_wrap_enabled", "enabled"), &VanillaEditor::set_wrap_enabled);
  ClassDB::bind_method(D_METHOD("draw_minimap", "value"), &VanillaEditor::draw_minimap);
  ClassDB::bind_method(D_METHOD("color_region", "filextension"), &VanillaEditor::color_region);
  ClassDB::bind_method(D_METHOD("clean_editor"), &VanillaEditor::clean_editor);
  ClassDB::bind_method(D_METHOD("new_file_open", "file_content", "last_modified", "current_file_path"), &VanillaEditor::new_file_open);
  ClassDB::bind_method(D_METHOD("update_lastmodified", "last_modified", "icon"), &VanillaEditor::update_lastmodified);
  ClassDB::bind_method(D_METHOD("new_file_create", "file_name"), &VanillaEditor::new_file_create);
  ClassDB::bind_method(D_METHOD("_on_Readonly_toggled", "button_pressed"), &VanillaEditor::_on_Readonly_toggled);
  ClassDB::bind_method(D_METHOD("_on_text_editor_text_changed"), &VanillaEditor::_on_text_editor_text_changed);
  ClassDB::bind_method(D_METHOD("count_characters"), &VanillaEditor::count_characters);
  ClassDB::bind_method(D_METHOD("_on_LineEdit_text_changed", "new_text"), &VanillaEditor::_on_LineEdit_text_changed);
  ClassDB::bind_method(D_METHOD("_on_matchcase_toggled", "button_pressed"), &VanillaEditor::_on_matchcase_toggled);
  ClassDB::bind_method(D_METHOD("_on_wholewords_toggled", "button_pressed"), &VanillaEditor::_on_wholewords_toggled);
  ClassDB::bind_method(D_METHOD("_on_close_pressed"), &VanillaEditor::_on_close_pressed);
  ClassDB::bind_method(D_METHOD("open_search_box"), &VanillaEditor::open_search_box);
  ClassDB::bind_method(D_METHOD("_on_Button_pressed"), &VanillaEditor::_on_Button_pressed);
  ClassDB::bind_method(D_METHOD("open_replace_box"), &VanillaEditor::open_replace_box);
  ClassDB::bind_method(D_METHOD("_on_close2_pressed"), &VanillaEditor::_on_close2_pressed);
  ClassDB::bind_method(D_METHOD("_on_LineEdit_focus_entered"), &VanillaEditor::_on_LineEdit_focus_entered);

 }



