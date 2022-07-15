
#include "fileeditor.h"


String FileEditor::get_DIRECTORY() {
 return DIRECTORY;
}

void FileEditor::set_DIRECTORY(const String &val) {
DIRECTORY = val;
}


String FileEditor::get_EXCEPTIONS() {
 return EXCEPTIONS;
}

void FileEditor::set_EXCEPTIONS(const String &val) {
EXCEPTIONS = val;
}


PoolStringArray FileEditor::get_EXTENSIONS() {
 return EXTENSIONS;
}

void FileEditor::set_EXTENSIONS(const PoolStringArray &val) {
EXTENSIONS = val;
}


MenuButton FileEditor::get_*file_btn() {
 return *file_btn;
}

void FileEditor::set_*file_btn(const MenuButton &val) {
*file_btn = val;
}


MenuButton FileEditor::get_*preview_btn() {
 return *preview_btn;
}

void FileEditor::set_*preview_btn(const MenuButton &val) {
*preview_btn = val;
}


MenuButton FileEditor::get_*settings_btn() {
 return *settings_btn;
}

void FileEditor::set_*settings_btn(const MenuButton &val) {
*settings_btn = val;
}


PopupMenu FileEditor::get_*file_btn_popup() {
 return *file_btn_popup;
}

void FileEditor::set_*file_btn_popup(const PopupMenu &val) {
*file_btn_popup = val;
}


PopupMenu FileEditor::get_*preview_btn_popup() {
 return *preview_btn_popup;
}

void FileEditor::set_*preview_btn_popup(const PopupMenu &val) {
*preview_btn_popup = val;
}


PopupMenu FileEditor::get_*settings_btn_popup() {
 return *settings_btn_popup;
}

void FileEditor::set_*settings_btn_popup(const PopupMenu &val) {
*settings_btn_popup = val;
}


HSplitContainer FileEditor::get_*editor_container() {
 return *editor_container;
}

void FileEditor::set_*editor_container(const HSplitContainer &val) {
*editor_container = val;
}


VBoxContainer FileEditor::get_*file_container() {
 return *file_container;
}

void FileEditor::set_*file_container(const VBoxContainer &val) {
*file_container = val;
}


ItemList FileEditor::get_*open_file_list() {
 return *open_file_list;
}

void FileEditor::set_*open_file_list(const ItemList &val) {
*open_file_list = val;
}


VBoxContainer FileEditor::get_*split_editor_container() {
 return *split_editor_container;
}

void FileEditor::set_*split_editor_container(const VBoxContainer &val) {
*split_editor_container = val;
}


LineEdit FileEditor::get_*open_file_name() {
 return *open_file_name;
}

void FileEditor::set_*open_file_name(const LineEdit &val) {
*open_file_name = val;
}


OptionButton FileEditor::get_*wrap_btn() {
 return *wrap_btn;
}

void FileEditor::set_*wrap_btn(const OptionButton &val) {
*wrap_btn = val;
}


OptionButton FileEditor::get_*map_btn() {
 return *map_btn;
}

void FileEditor::set_*map_btn(const OptionButton &val) {
*map_btn = val;
}


FileDialog FileEditor::get_*file_list() {
 return *file_list;
}

void FileEditor::set_*file_list(const FileDialog &val) {
*file_list = val;
}


AcceptDialog FileEditor::get_*new_file_dialogue() {
 return *new_file_dialogue;
}

void FileEditor::set_*new_file_dialogue(const AcceptDialog &val) {
*new_file_dialogue = val;
}


LineEdit FileEditor::get_*new_file_dialogue_name() {
 return *new_file_dialogue_name;
}

void FileEditor::set_*new_file_dialogue_name(const LineEdit &val) {
*new_file_dialogue_name = val;
}


ConfirmationDialog FileEditor::get_*confirmation_close() {
 return *confirmation_close;
}

void FileEditor::set_*confirmation_close(const ConfirmationDialog &val) {
*confirmation_close = val;
}


FileDialog FileEditor::get_*select_font_dialog() {
 return *select_font_dialog;
}

void FileEditor::set_*select_font_dialog(const FileDialog &val) {
*select_font_dialog = val;
}


Reference FileEditor::get_*LastOpenedFiles() {
 return *LastOpenedFiles;
}

void FileEditor::set_*LastOpenedFiles(const Reference &val) {
*LastOpenedFiles = val;
}


Variant FileEditor::get_Variant() {
 return Variant;
}

void FileEditor::set_Variant(const Variant &val) {
Variant = val;
}


Variant FileEditor::get_Variant() {
 return Variant;
}

void FileEditor::set_Variant(const Variant &val) {
Variant = val;
}


Array FileEditor::get_directories() {
 return directories;
}

void FileEditor::set_directories(const Array &val) {
directories = val;
}


Array FileEditor::get_files() {
 return files;
}

void FileEditor::set_files(const Array &val) {
files = val;
}


int FileEditor::get_current_file_index() const {
 return current_file_index;
}

void FileEditor::set_current_file_index(const int val) {
current_file_index = val;
}


String FileEditor::get_current_file_path() {
 return current_file_path;
}

void FileEditor::set_current_file_path(const String &val) {
current_file_path = val;
}


bool FileEditor::get_save_as() const {
 return save_as;
}

void FileEditor::set_save_as(const bool val) {
save_as = val;
}


Control FileEditor::get_*current_editor() {
 return *current_editor;
}

void FileEditor::set_*current_editor(const Control &val) {
*current_editor = val;
}


Ref<DynamicFont> FileEditor::get_current_font() {
 return current_font;
}

void FileEditor::set_current_font(const Ref<DynamicFont> &val) {
current_font = val;
}


bool FileEditor::get_editing_file() const {
 return editing_file;
}

void FileEditor::set_editing_file(const bool val) {
editing_file = val;
}



 //tool;
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
 //var Preview = preload("res://addons/file-editor/scripts/Preview.gd");
 //var VanillaEditor = preload("res://addons/file-editor/scripts/VanillaEditor.gd");
  Array directories = ;
  Array files = ;
  int current_file_index = -1;
  String current_file_path = ;
  bool save_as = false;
  Control *current_editor;
  Ref<DynamicFont> current_font;
  bool editing_file = false;

 void FileEditor::_init() {
  set_anchors_and_margins_preset(Control.PRESET_WIDE);
  size_flags_vertical = SIZE_EXPAND_FILL;
  size_flags_horizontal = SIZE_EXPAND_FILL;
   VBoxContainer *vbc = VBoxContainer.new();
  add_child(vbc);
  vbc.set_anchors_and_margins_preset(Control.PRESET_WIDE);
   HBoxContainer *tob_bar = HBoxContainer.new();
  vbc.add_child(tob_bar);
  file_btn = MenuButton.new();
  tob_bar.add_child(file_btn);
  file_btn.text = "File";
  file_btn_popup = file_btn.get_popup();
   Ref<InputEventKey> hotkey = InputEventKey.new();
  hotkey.scancode = KEY_N;
  hotkey.control = true;
  file_btn_popup.add_item("New File", FileMenuOptions.FILE_MENU_OPTION_NEW, hotkey.get_scancode_with_modifiers());
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_O;
  hotkey.control = true;
  file_btn_popup.add_item("Open File", FileMenuOptions.FILE_MENU_OPTION_OPEN, hotkey.get_scancode_with_modifiers());
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_C;
  hotkey.control = true;
  hotkey.alt = true;
  file_btn_popup.add_item("Close File", FileMenuOptions.FILE_MENU_OPTION_CLOSE, hotkey.get_scancode_with_modifiers());
  file_btn_popup.add_separator();
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_S;
  hotkey.control = true;
  file_btn_popup.add_item("Save File", FileMenuOptions.FILE_MENU_OPTION_SAVE, hotkey.get_scancode_with_modifiers());
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_S;
  hotkey.control = true;
  hotkey.alt = true;
  file_btn_popup.add_item("Save File as...", FileMenuOptions.FILE_MENU_OPTION_SAVE_AS, hotkey.get_scancode_with_modifiers());
  //hotkey = InputEventKey.new();
  //hotkey.scancode = KEY_D;
  //hotkey.control = true;
  //file_btn_popup.add_item("Delete File", FileMenuOptions.FILE_MENU_OPTION_DELETE, hotkey.get_scancode_with_modifiers());
  file_btn_popup.add_item("Delete File", FileMenuOptions.FILE_MENU_OPTION_DELETE);
  file_btn_popup.add_separator();
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_F;
  hotkey.control = true;
  file_btn_popup.add_item("Search in file...", FileMenuOptions.FILE_MENU_OPTION_SEARCH, hotkey.get_scancode_with_modifiers());
  hotkey = InputEventKey.new();
  hotkey.scancode = KEY_R;
  hotkey.control = true;
  file_btn_popup.add_item("Replace occurencies", FileMenuOptions.FILE_MENU_OPTION_REPLACE, hotkey.get_scancode_with_modifiers());
  //Preview;
  preview_btn = MenuButton.new();
  tob_bar.add_child(preview_btn);
  preview_btn.text = "Preview";
  preview_btn_popup = preview_btn.get_popup();
  preview_btn_popup.add_item("BBCode Preview");
  preview_btn_popup.add_item("Markdown Preview");
  preview_btn_popup.add_item("HTML Preview");
  preview_btn_popup.add_item("CSV Preview");
  //Settings;
  settings_btn = MenuButton.new();
  tob_bar.add_child(settings_btn);
  settings_btn.text = "Settings";
  settings_btn_popup = settings_btn.get_popup();
  settings_btn_popup.add_item("Change Font");
  //SplitContainer;
  editor_container = HSplitContainer.new();
  vbc.add_child(editor_container);
  editor_container.split_offset = 150;
  editor_container.size_flags_horizontal = SIZE_EXPAND_FILL;
  editor_container.size_flags_vertical = SIZE_EXPAND_FILL;
  //Files;
  file_container = VBoxContainer.new();
  editor_container.add_child(file_container);
  open_file_list = ItemList.new();
  file_container.add_child(open_file_list);
  open_file_list.allow_reselect = true;
  open_file_list.size_flags_vertical = SIZE_EXPAND_FILL;
  file_container.add_child(HSeparator.new());
  //Editor;
  split_editor_container = VBoxContainer.new();
  editor_container.add_child(split_editor_container);
   HBoxContainer *editor_top_bar = HBoxContainer.new();
  split_editor_container.add_child(editor_top_bar);
   Label *edtopbar_label = Label.new();
  editor_top_bar.add_child(edtopbar_label);
  edtopbar_label.text = "Editing file:";
  open_file_name = LineEdit.new();
  editor_top_bar.add_child(open_file_name);
  open_file_name.editable = false;
  open_file_name.mouse_filter = Control.MOUSE_FILTER_PASS;
  open_file_name.size_flags_horizontal = SIZE_EXPAND_FILL;
  wrap_btn = OptionButton.new();
  editor_top_bar.add_child(wrap_btn);
  wrap_btn.add_item("No Wrap");
  wrap_btn.add_item("Soft Wrap");
  map_btn = OptionButton.new();
  editor_top_bar.add_child(map_btn);
  map_btn.add_item("Hide Map");
  map_btn.add_item("Show Map");
  map_btn.selected = 1;
  //dialogs;
  file_list = FileDialog.new();
  add_child(file_list);
  file_list.show_hidden_files = true;
  file_list.dialog_hide_on_ok = true;
  file_list.window_title = "Save file";
  file_list.popup_exclusive = true;
  file_list.set_anchors_and_margins_preset(Control.PRESET_WIDE);
  file_list.margin_left = 222;
  file_list.margin_top = 132;
  file_list.margin_right = -221;
  file_list.margin_bottom = -131;
  file_list.rect_min_size = Vector2(200, 70);
  new_file_dialogue = AcceptDialog.new();
  add_child(new_file_dialogue);
  new_file_dialogue.window_title = "Create new File";
   VBoxContainer *nfd_vbc = VBoxContainer.new();
  new_file_dialogue.add_child(nfd_vbc);
   Label *nfd_name = Label.new();
  nfd_vbc.add_child(nfd_name);
  nfd_name.text = "Insert file name (no extension needed)";
  nfd_name.align = Label.ALIGN_CENTER;
  nfd_name.valign = Label.VALIGN_CENTER;
  nfd_name.size_flags_vertical = SIZE_EXPAND_FILL;
  new_file_dialogue_name = LineEdit.new();
  nfd_vbc.add_child(new_file_dialogue_name);
  new_file_dialogue_name.clear_button_enabled = true;
  new_file_dialogue_name.text = "example";
  new_file_dialogue_name.rect_min_size = Vector2(200, 0);
  new_file_dialogue_name.size_flags_horizontal = SIZE_EXPAND | SIZE_SHRINK_CENTER;
  new_file_dialogue_name.size_flags_vertical = SIZE_EXPAND_FILL;
  confirmation_close = ConfirmationDialog.new();
  add_child(confirmation_close);
  confirmation_close.dialog_text = "There are some unsaved changes.\nPress \"OK\" if you want to close this tab anyway, or \"cancel\" if you want to keep on editing your file.";
  confirmation_close.window_title = "Unsaved changes";
  confirmation_close.set_anchors_and_margins_preset(Control.PRESET_CENTER);
  select_font_dialog = FileDialog.new();
  add_child(select_font_dialog);
  select_font_dialog.mode = FileDialog.MODE_OPEN_FILE;
  select_font_dialog.access = FileDialog.ACCESS_FILESYSTEM;
  select_font_dialog.show_hidden_files = true;
  select_font_dialog.window_title = "Open a File";
  select_font_dialog.resizable = true;
  select_font_dialog.set_anchors_and_margins_preset(Control.PRESET_WIDE);
  select_font_dialog.margin_left = 222;
  select_font_dialog.margin_top = 132;
  select_font_dialog.margin_right = -221;
  select_font_dialog.margin_bottom = -131;
  select_font_dialog.rect_min_size = Vector2(200, 70);
   PoolStringArray farr = PoolStringArray();
  farr.push_back("*.TTF");
  farr.push_back("*.ttf");
  select_font_dialog.filters = farr;
}


 void FileEditor::_ready() {

  if (not Engine.is_editor_hint()) {
   return;
}

  clean_editor();
  connect_signals();

  if (LastOpenedFiles) {
    Array opened_files = LastOpenedFiles.load_opened_files();

   for (opened_file in opened_files) {
    open_file(opened_file[1], opened_file[2]);
}

}

  file_list.set_filters(EXTENSIONS);
}


 void FileEditor::connect_signals() {
  file_list.connect("confirmed",self,"update_list");
  file_btn_popup.connect("id_pressed",self,"_on_file_btn_pressed");
  preview_btn_popup.connect("id_pressed",self,"_on_preview_btn_pressed");
  settings_btn_popup.connect("id_pressed",self,"_on_settings_btn_pressed");
  open_file_list.connect("item_selected",self,"_on_fileitem_pressed");
  wrap_btn.connect("item_selected",self,"on_wrap_button");
  map_btn.connect("item_selected",self,"on_minimap_button");
  select_font_dialog.connect("file_selected",self,"_on_font_selected");
}


 void FileEditor::create_selected_file() {
  update_list();
  file_list.mode = FileDialog.MODE_SAVE_FILE;
  file_list.set_title("Create a new File");

  if (file_list.is_connected("file_selected",self,"delete_file")) {
   file_list.disconnect("file_selected",self,"delete_file");
}


  if (file_list.is_connected("file_selected",self,"open_file")) {
   file_list.disconnect("file_selected",self,"open_file");
}


  if (not file_list.is_connected("file_selected",self,"create_new_file")) {
   file_list.connect("file_selected",self,"create_new_file");
}

  open_file_list();
}


 void FileEditor::open_selected_file() {
  update_list();
  file_list.mode = FileDialog.MODE_OPEN_FILE;
  file_list.set_title("Select a File you want to edit");

  if (file_list.is_connected("file_selected",self,"delete_file")) {
   file_list.disconnect("file_selected",self,"delete_file");
}


  if (file_list.is_connected("file_selected",self,"create_new_file")) {
   file_list.disconnect("file_selected",self,"create_new_file");
}


  if (not file_list.is_connected("file_selected",self,"open_file")) {
   file_list.connect("file_selected",self,"open_file");
}

  open_file_list();
}


 void FileEditor::delete_selected_file() {
  update_list();
  file_list.mode = FileDialog.MODE_OPEN_FILES;
  file_list.set_title("Select one or more Files you want to delete");

  if (file_list.is_connected("file_selected",self,"open_file")) {
   file_list.disconnect("file_selected",self,"open_file");
}


  if (file_list.is_connected("file_selected",self,"create_new_file")) {
   file_list.disconnect("file_selected",self,"create_new_file");
}


  if (not file_list.is_connected("files_selected",self,"delete_file")) {
   file_list.connect("files_selected",self,"delete_file");
}

  open_file_list();
}


 void FileEditor::save_current_file_as() {
  update_list();
  file_list.mode = FileDialog.MODE_SAVE_FILE;
  file_list.set_title("Save this File as...");

  if (file_list.is_connected("file_selected",self,"delete_file")) {
   file_list.disconnect("file_selected",self,"delete_file");
}


  if (file_list.is_connected("file_selected",self,"open_file")) {
   file_list.disconnect("file_selected",self,"open_file");
}


  if (not file_list.is_connected("file_selected",self,"create_new_file")) {
   file_list.connect("file_selected",self,"create_new_file");
}

  open_file_list();
}


 void FileEditor::_on_file_btn_pressed(const int index) {

   {

    {
    create_selected_file();
}


    {
    open_selected_file();
}


    {

    if (current_file_index!=-1 && current_file_path != "") {
     close_file(current_file_index);
}

}


    {

    if (current_file_index!=-1 && current_file_path != "") {
     save_as = false;
     save_file(current_file_path);
}

}


    {

    if (current_file_index!=-1 && current_file_path != "") {
     save_as = true;
     save_file(current_file_path);
     save_current_file_as();
}

}


    {
    delete_selected_file();
}


    {
    current_editor.open_search_box();
}


    {
    current_editor.open_replace_box();
}

}

}


 void FileEditor::_on_preview_btn_pressed(const int id) {

  if (id == 0) {
   bbcode_preview();
}


  else if (id == 1) {
   markdown_preview();
}


  else if (id == 2) {
   html_preview();
}


  else if (id == 3) {
   csv_preview();
}

}


 void FileEditor::_on_settings_btn_pressed(const int index) {

   {

    {
    select_font_dialog.popup();
}

}

}


 void FileEditor::_on_font_selected(const String &font_path) {
  current_editor.set_font(font_path);
  LastOpenedFiles.store_editor_fonts(current_file_path.get_file(), font_path);
}


 void FileEditor::_on_fileitem_pressed(const int index) {
  current_file_index = index;
   Array selected_item_metadata = open_file_list.get_item_metadata(current_file_index);
   String extension = selected_item_metadata[0].current_path.get_file().get_extension();

  if (open_file_list.get_item_text(current_file_index).begins_with("(*)")) {
   editing_file = true;
}


  else {
   editing_file = false;
}

  current_file_path = selected_item_metadata[0].current_path;

  if (current_editor.visible || current_editor == null) {

   if (current_editor != null) {
    current_editor.hide();
}

   current_editor = selected_item_metadata[0];
   current_editor.show();
   open_file_name.set_text(current_editor.current_path);

   if (wrap_btn.get_selected_id() == 1) {
    current_editor.set_wrap_enabled(true);
}


   else {
    current_editor.set_wrap_enabled(false);
}


   if (map_btn.get_selected_id() == 1) {
    current_editor.draw_minimap(true);
}


   else {
    current_editor.draw_minimap(false);
}

}

}


 void FileEditor::open_file(const String &path, const String &font) {

  if (current_file_path != path) {
   current_file_path = path;
    Control *vanilla_editor = open_in_vanillaeditor(path);

   if (font != "null" && vanilla_editor.get("custom_fonts/font")!=null) {
    vanilla_editor.set_font(font);
}

   generate_file_item(path, vanilla_editor);
   LastOpenedFiles.store_opened_files(open_file_list);
}

  current_editor.show();
}


 void FileEditor::generate_file_item(const String &path, const Control &veditor) {
  open_file_name.set_text(path);
  open_file_list.add_item(path.get_file(), null,true);
  current_file_index = open_file_list.get_item_count()-1;
  open_file_list.set_item_metadata(current_file_index,[veditor]);
  open_file_list.select(open_file_list.get_item_count()-1);
}


 Control FileEditor::open_in_vanillaeditor(const String &path) {
   Control *editor = VanillaEditor.new();
  editor.LastOpenedFiles = LastOpenedFiles;
  editor.file_list = file_list;
  split_editor_container.add_child(editor,true);

  if (current_editor && current_editor!=editor) {
   editor.show();
   current_editor.hide();
}

  current_editor = editor;
  editor.connect("text_changed",self,"_on_vanillaeditor_text_changed");
   File *current_file = File.new();
  current_file.open(path,File.READ);
   String current_content = "";
  current_content = current_file.get_as_text();
   Variant  = OS.get_datetime_from_unix_time(current_file.get_modified_time(path));
  current_file.close();
  editor.new_file_open(current_content,last_modified,current_file_path);
  update_list();

  if (wrap_btn.get_selected_id() == 1) {
   current_editor.set_wrap_enabled(true);
}

  return editor;
}


 void FileEditor::close_file(const int index) {

  if (editing_file) {
   confirmation_close.popup();
}


  else {
   confirm_close(index);
}

}


 void FileEditor::confirm_close(const int index) {
  LastOpenedFiles.remove_opened_file(index,open_file_list);
  open_file_list.remove_item(index);
  open_file_name.clear();
  current_editor.queue_free();

  if (index > 0) {
   open_file_list.select(index-1);
   _on_fileitem_pressed(index-1);
}

}


 void FileEditor::_on_update_file() {
   File *current_file = File.new();
  current_file.open(current_file_path,File.READ);
   String current_content = current_file.get_as_text();
   Dictionary last_modified = OS.get_datetime_from_unix_time(current_file.get_modified_time(current_file_path));
  current_file.close();
  current_editor.new_file_open(current_content,last_modified,current_file_path);
}


 void FileEditor::delete_file(const PoolStringArray &files_selected) {
   Variant  = Directory.new();

  for (file in files_selected) {
   dir.remove(file);
}

  update_list();
}


 void FileEditor::open_new_file_dialogue() {
  new_file_dialogue.popup();
  new_file_dialogue.set_position(OS.get_screen_size()/2 - new_file_dialogue.get_size()/2);
}


 void FileEditor::open_file_list() {
  update_list();
  file_list.popup();
  file_list.set_position(OS.get_screen_size()/2 - file_list.get_size()/2);
}


 void FileEditor::create_new_file(const String &given_path) {
   File *current_file = File.new();
  current_file.open(given_path,File.WRITE);

  if (save_as) {
   current_file.store_line(current_editor.text_editor.get_text());
}

  current_file.close();
  open_file(given_path);
}


 void FileEditor::save_file(const String &current_path) {
  print("Saving file: ",current_path);
   File *current_file = File.new();
  current_file.open(current_path,File.WRITE);
   String current_content = "";
   int lines = current_editor.text_editor.get_line_count();

  for (int line = 0; line < lines; ++line) { //line in range(lines)
   //if current_editor.text_editor.get_line(line) == "":;
   //	continue;
   current_content = current_editor.text_editor.get_text();
   current_file.store_line(current_editor.text_editor.get_line(line));
}

  current_file.close();
  current_file_path = current_path;
   Dictionary last_modified = OS.get_datetime_from_unix_time(current_file.get_modified_time(current_file_path));
  current_editor.update_lastmodified(last_modified,"save");
  open_file_list.set_item_metadata(current_file_index,[current_editor]);

  if (open_file_list.get_item_text(current_file_index).begins_with("(*)")) {
   open_file_list.set_item_text(current_file_index,open_file_list.get_item_text(current_file_index).lstrip("(*)"));
   editing_file = false;
}

  update_list();
}


 void FileEditor::clean_editor() {
   Array nodes = get_tree().get_nodes_in_group("vanilla_editor");

  for (int i = 0; i < nodes.size(); ++i) { //i in range(nodes.size())
    Node *vanillaeditor = nodes[i];
   vanillaeditor.queue_free();
}

  open_file_name.clear();
  open_file_list.clear();
}


 void FileEditor::csv_preview() {
   Control *preview = Preview.new();
  get_parent().get_parent().get_parent().add_child(preview);
  preview.popup();
  preview.window_title += " ("+current_file_path.get_file()+")";
   int lines = current_editor.text_editor.get_line_count();
   Array rows = Array();

  for (int i = 0; i <  lines-1; ++i) { //i in range(0, lines-1)
   rows.append(current_editor.text_editor.get_line(i).rsplit(",",false));
}

  preview.print_csv(rows);
}


 void FileEditor::bbcode_preview() {
   Control *preview = Preview.new();
  get_parent().get_parent().get_parent().add_child(preview);
  preview.popup();
  preview.window_title += " ("+current_file_path.get_file()+")";
  preview.print_bb(current_editor.text_editor.get_text());
}


 void FileEditor::markdown_preview() {
   Control *preview = Preview.new();
  get_parent().get_parent().get_parent().add_child(preview);
  preview.popup();
  preview.window_title += " ("+current_file_path.get_file()+")";
  preview.print_markdown(current_editor.text_editor.get_text());
}


 void FileEditor::html_preview() {
   Control *preview = Preview.new();
  get_parent().get_parent().get_parent().add_child(preview);
  preview.popup();
  preview.window_title += " ("+current_file_path.get_file()+")";
  preview.print_html(current_editor.text_editor.get_text());
}


 void FileEditor::_on_vanillaeditor_text_changed() {

  if (not open_file_list.get_item_text(current_file_index).begins_with("(*)")) {
   open_file_list.set_item_text(current_file_index,"(*)"+open_file_list.get_item_text(current_file_index));
   editing_file = true;
}

}


 void FileEditor::update_list() {
  file_list.invalidate();
}


 void FileEditor::on_wrap_button(const int index) {

   {

    {
    current_editor.set_wrap_enabled(false);
}


    {
    current_editor.set_wrap_enabled(true);
}

}

}


 void FileEditor::on_minimap_button(const int index) {

   {

    {
    current_editor.draw_minimap(false);
}


    {
    current_editor.draw_minimap(true);
}

}

}


 void FileEditor::check_file_preview(const String &file) {
  // check whether the opened file has a corresponding preview session for its extension;
  pass;
}


 void FileEditor::_on_ConfirmationDialog_confirmed() {
  confirm_close(current_file_index);
}

}

 FileEditor::FileEditor() {
  DIRECTORY = "res://";
  EXCEPTIONS = "addons";
  EXTENSIONS = [;
  *file_btn = null;
  *preview_btn = null;
  *settings_btn = null;
  *file_btn_popup = null;
  *preview_btn_popup = null;
  *settings_btn_popup = null;
  *editor_container = null;
  *file_container = null;
  *open_file_list = null;
  *split_editor_container = null;
  *open_file_name = null;
  *wrap_btn = null;
  *map_btn = null;
  *file_list = null;
  *new_file_dialogue = null;
  *new_file_dialogue_name = null;
  *confirmation_close = null;
  *select_font_dialog = null;
  *LastOpenedFiles = null;
 //var Preview = preload("res://addons/file-editor/scripts/Preview.gd");
 //var VanillaEditor = preload("res://addons/file-editor/scripts/VanillaEditor.gd");
  directories = ;
  files = ;
  current_file_index = -1;
  current_file_path = ;
  save_as = false;
  *current_editor;
  current_font;
  editing_file = false;
 }

 FileEditor::~FileEditor() {
 }


 static void FileEditor::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_DIRECTORY"), &FileEditor::get_DIRECTORY);
   ClassDB::bind_method(D_METHOD("set_DIRECTORY", "value"), &FileEditor::set_DIRECTORY);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "DIRECTORY"), "set_DIRECTORY", "get_DIRECTORY");


   ClassDB::bind_method(D_METHOD("get_EXCEPTIONS"), &FileEditor::get_EXCEPTIONS);
   ClassDB::bind_method(D_METHOD("set_EXCEPTIONS", "value"), &FileEditor::set_EXCEPTIONS);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "EXCEPTIONS"), "set_EXCEPTIONS", "get_EXCEPTIONS");


   ClassDB::bind_method(D_METHOD("get_EXTENSIONS"), &FileEditor::get_EXTENSIONS);
   ClassDB::bind_method(D_METHOD("set_EXTENSIONS", "value"), &FileEditor::set_EXTENSIONS);
   ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "EXTENSIONS"), "set_EXTENSIONS", "get_EXTENSIONS");


   ClassDB::bind_method(D_METHOD("get_*file_btn"), &FileEditor::get_*file_btn);
   ClassDB::bind_method(D_METHOD("set_*file_btn", "value"), &FileEditor::set_*file_btn);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_btn", PROPERTY_HINT_RESOURCE_TYPE, "MenuButton"), "set_*file_btn", "get_*file_btn");


   ClassDB::bind_method(D_METHOD("get_*preview_btn"), &FileEditor::get_*preview_btn);
   ClassDB::bind_method(D_METHOD("set_*preview_btn", "value"), &FileEditor::set_*preview_btn);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*preview_btn", PROPERTY_HINT_RESOURCE_TYPE, "MenuButton"), "set_*preview_btn", "get_*preview_btn");


   ClassDB::bind_method(D_METHOD("get_*settings_btn"), &FileEditor::get_*settings_btn);
   ClassDB::bind_method(D_METHOD("set_*settings_btn", "value"), &FileEditor::set_*settings_btn);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*settings_btn", PROPERTY_HINT_RESOURCE_TYPE, "MenuButton"), "set_*settings_btn", "get_*settings_btn");


   ClassDB::bind_method(D_METHOD("get_*file_btn_popup"), &FileEditor::get_*file_btn_popup);
   ClassDB::bind_method(D_METHOD("set_*file_btn_popup", "value"), &FileEditor::set_*file_btn_popup);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_btn_popup", PROPERTY_HINT_RESOURCE_TYPE, "PopupMenu"), "set_*file_btn_popup", "get_*file_btn_popup");


   ClassDB::bind_method(D_METHOD("get_*preview_btn_popup"), &FileEditor::get_*preview_btn_popup);
   ClassDB::bind_method(D_METHOD("set_*preview_btn_popup", "value"), &FileEditor::set_*preview_btn_popup);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*preview_btn_popup", PROPERTY_HINT_RESOURCE_TYPE, "PopupMenu"), "set_*preview_btn_popup", "get_*preview_btn_popup");


   ClassDB::bind_method(D_METHOD("get_*settings_btn_popup"), &FileEditor::get_*settings_btn_popup);
   ClassDB::bind_method(D_METHOD("set_*settings_btn_popup", "value"), &FileEditor::set_*settings_btn_popup);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*settings_btn_popup", PROPERTY_HINT_RESOURCE_TYPE, "PopupMenu"), "set_*settings_btn_popup", "get_*settings_btn_popup");


   ClassDB::bind_method(D_METHOD("get_*editor_container"), &FileEditor::get_*editor_container);
   ClassDB::bind_method(D_METHOD("set_*editor_container", "value"), &FileEditor::set_*editor_container);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*editor_container", PROPERTY_HINT_RESOURCE_TYPE, "HSplitContainer"), "set_*editor_container", "get_*editor_container");


   ClassDB::bind_method(D_METHOD("get_*file_container"), &FileEditor::get_*file_container);
   ClassDB::bind_method(D_METHOD("set_*file_container", "value"), &FileEditor::set_*file_container);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_container", PROPERTY_HINT_RESOURCE_TYPE, "VBoxContainer"), "set_*file_container", "get_*file_container");


   ClassDB::bind_method(D_METHOD("get_*open_file_list"), &FileEditor::get_*open_file_list);
   ClassDB::bind_method(D_METHOD("set_*open_file_list", "value"), &FileEditor::set_*open_file_list);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*open_file_list", PROPERTY_HINT_RESOURCE_TYPE, "ItemList"), "set_*open_file_list", "get_*open_file_list");


   ClassDB::bind_method(D_METHOD("get_*split_editor_container"), &FileEditor::get_*split_editor_container);
   ClassDB::bind_method(D_METHOD("set_*split_editor_container", "value"), &FileEditor::set_*split_editor_container);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*split_editor_container", PROPERTY_HINT_RESOURCE_TYPE, "VBoxContainer"), "set_*split_editor_container", "get_*split_editor_container");


   ClassDB::bind_method(D_METHOD("get_*open_file_name"), &FileEditor::get_*open_file_name);
   ClassDB::bind_method(D_METHOD("set_*open_file_name", "value"), &FileEditor::set_*open_file_name);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*open_file_name", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*open_file_name", "get_*open_file_name");


   ClassDB::bind_method(D_METHOD("get_*wrap_btn"), &FileEditor::get_*wrap_btn);
   ClassDB::bind_method(D_METHOD("set_*wrap_btn", "value"), &FileEditor::set_*wrap_btn);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*wrap_btn", PROPERTY_HINT_RESOURCE_TYPE, "OptionButton"), "set_*wrap_btn", "get_*wrap_btn");


   ClassDB::bind_method(D_METHOD("get_*map_btn"), &FileEditor::get_*map_btn);
   ClassDB::bind_method(D_METHOD("set_*map_btn", "value"), &FileEditor::set_*map_btn);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*map_btn", PROPERTY_HINT_RESOURCE_TYPE, "OptionButton"), "set_*map_btn", "get_*map_btn");


   ClassDB::bind_method(D_METHOD("get_*file_list"), &FileEditor::get_*file_list);
   ClassDB::bind_method(D_METHOD("set_*file_list", "value"), &FileEditor::set_*file_list);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*file_list", PROPERTY_HINT_RESOURCE_TYPE, "FileDialog"), "set_*file_list", "get_*file_list");


   ClassDB::bind_method(D_METHOD("get_*new_file_dialogue"), &FileEditor::get_*new_file_dialogue);
   ClassDB::bind_method(D_METHOD("set_*new_file_dialogue", "value"), &FileEditor::set_*new_file_dialogue);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*new_file_dialogue", PROPERTY_HINT_RESOURCE_TYPE, "AcceptDialog"), "set_*new_file_dialogue", "get_*new_file_dialogue");


   ClassDB::bind_method(D_METHOD("get_*new_file_dialogue_name"), &FileEditor::get_*new_file_dialogue_name);
   ClassDB::bind_method(D_METHOD("set_*new_file_dialogue_name", "value"), &FileEditor::set_*new_file_dialogue_name);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*new_file_dialogue_name", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*new_file_dialogue_name", "get_*new_file_dialogue_name");


   ClassDB::bind_method(D_METHOD("get_*confirmation_close"), &FileEditor::get_*confirmation_close);
   ClassDB::bind_method(D_METHOD("set_*confirmation_close", "value"), &FileEditor::set_*confirmation_close);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*confirmation_close", PROPERTY_HINT_RESOURCE_TYPE, "ConfirmationDialog"), "set_*confirmation_close", "get_*confirmation_close");


   ClassDB::bind_method(D_METHOD("get_*select_font_dialog"), &FileEditor::get_*select_font_dialog);
   ClassDB::bind_method(D_METHOD("set_*select_font_dialog", "value"), &FileEditor::set_*select_font_dialog);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*select_font_dialog", PROPERTY_HINT_RESOURCE_TYPE, "FileDialog"), "set_*select_font_dialog", "get_*select_font_dialog");


   ClassDB::bind_method(D_METHOD("get_*LastOpenedFiles"), &FileEditor::get_*LastOpenedFiles);
   ClassDB::bind_method(D_METHOD("set_*LastOpenedFiles", "value"), &FileEditor::set_*LastOpenedFiles);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*LastOpenedFiles", PROPERTY_HINT_RESOURCE_TYPE, "Reference"), "set_*LastOpenedFiles", "get_*LastOpenedFiles");


   ClassDB::bind_method(D_METHOD("get_Variant"), &FileEditor::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &FileEditor::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &FileEditor::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &FileEditor::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_directories"), &FileEditor::get_directories);
   ClassDB::bind_method(D_METHOD("set_directories", "value"), &FileEditor::set_directories);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "directories"), "set_directories", "get_directories");


   ClassDB::bind_method(D_METHOD("get_files"), &FileEditor::get_files);
   ClassDB::bind_method(D_METHOD("set_files", "value"), &FileEditor::set_files);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "files"), "set_files", "get_files");


   ClassDB::bind_method(D_METHOD("get_current_file_index"), &FileEditor::get_current_file_index);
   ClassDB::bind_method(D_METHOD("set_current_file_index", "value"), &FileEditor::set_current_file_index);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "current_file_index"), "set_current_file_index", "get_current_file_index");


   ClassDB::bind_method(D_METHOD("get_current_file_path"), &FileEditor::get_current_file_path);
   ClassDB::bind_method(D_METHOD("set_current_file_path", "value"), &FileEditor::set_current_file_path);
   ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_file_path"), "set_current_file_path", "get_current_file_path");


   ClassDB::bind_method(D_METHOD("get_save_as"), &FileEditor::get_save_as);
   ClassDB::bind_method(D_METHOD("set_save_as", "value"), &FileEditor::set_save_as);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "save_as"), "set_save_as", "get_save_as");


   ClassDB::bind_method(D_METHOD("get_*current_editor"), &FileEditor::get_*current_editor);
   ClassDB::bind_method(D_METHOD("set_*current_editor", "value"), &FileEditor::set_*current_editor);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*current_editor", PROPERTY_HINT_RESOURCE_TYPE, "Control"), "set_*current_editor", "get_*current_editor");


   ClassDB::bind_method(D_METHOD("get_current_font"), &FileEditor::get_current_font);
   ClassDB::bind_method(D_METHOD("set_current_font", "value"), &FileEditor::set_current_font);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_font", PROPERTY_HINT_RESOURCE_TYPE, "Ref<DynamicFont>"), "set_current_font", "get_current_font");


   ClassDB::bind_method(D_METHOD("get_editing_file"), &FileEditor::get_editing_file);
   ClassDB::bind_method(D_METHOD("set_editing_file", "value"), &FileEditor::set_editing_file);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editing_file"), "set_editing_file", "get_editing_file");


  ClassDB::bind_method(D_METHOD("_init"), &FileEditor::_init);
  ClassDB::bind_method(D_METHOD("_ready"), &FileEditor::_ready);
  ClassDB::bind_method(D_METHOD("connect_signals"), &FileEditor::connect_signals);
  ClassDB::bind_method(D_METHOD("create_selected_file"), &FileEditor::create_selected_file);
  ClassDB::bind_method(D_METHOD("open_selected_file"), &FileEditor::open_selected_file);
  ClassDB::bind_method(D_METHOD("delete_selected_file"), &FileEditor::delete_selected_file);
  ClassDB::bind_method(D_METHOD("save_current_file_as"), &FileEditor::save_current_file_as);
  ClassDB::bind_method(D_METHOD("_on_file_btn_pressed", "index"), &FileEditor::_on_file_btn_pressed);
  ClassDB::bind_method(D_METHOD("_on_preview_btn_pressed", "id"), &FileEditor::_on_preview_btn_pressed);
  ClassDB::bind_method(D_METHOD("_on_settings_btn_pressed", "index"), &FileEditor::_on_settings_btn_pressed);
  ClassDB::bind_method(D_METHOD("_on_font_selected", "font_path"), &FileEditor::_on_font_selected);
  ClassDB::bind_method(D_METHOD("_on_fileitem_pressed", "index"), &FileEditor::_on_fileitem_pressed);
  ClassDB::bind_method(D_METHOD("open_file", "path", "font"), &FileEditor::open_file, "null");
  ClassDB::bind_method(D_METHOD("generate_file_item", "path", "veditor"), &FileEditor::generate_file_item);
  ClassDB::bind_method(D_METHOD("open_in_vanillaeditor", "path"), &FileEditor::open_in_vanillaeditor);
  ClassDB::bind_method(D_METHOD("close_file", "index"), &FileEditor::close_file);
  ClassDB::bind_method(D_METHOD("confirm_close", "index"), &FileEditor::confirm_close);
  ClassDB::bind_method(D_METHOD("_on_update_file"), &FileEditor::_on_update_file);
  ClassDB::bind_method(D_METHOD("delete_file", "files_selected"), &FileEditor::delete_file);
  ClassDB::bind_method(D_METHOD("open_new_file_dialogue"), &FileEditor::open_new_file_dialogue);
  ClassDB::bind_method(D_METHOD("open_file_list"), &FileEditor::open_file_list);
  ClassDB::bind_method(D_METHOD("create_new_file", "given_path"), &FileEditor::create_new_file);
  ClassDB::bind_method(D_METHOD("save_file", "current_path"), &FileEditor::save_file);
  ClassDB::bind_method(D_METHOD("clean_editor"), &FileEditor::clean_editor);
  ClassDB::bind_method(D_METHOD("csv_preview"), &FileEditor::csv_preview);
  ClassDB::bind_method(D_METHOD("bbcode_preview"), &FileEditor::bbcode_preview);
  ClassDB::bind_method(D_METHOD("markdown_preview"), &FileEditor::markdown_preview);
  ClassDB::bind_method(D_METHOD("html_preview"), &FileEditor::html_preview);
  ClassDB::bind_method(D_METHOD("_on_vanillaeditor_text_changed"), &FileEditor::_on_vanillaeditor_text_changed);
  ClassDB::bind_method(D_METHOD("update_list"), &FileEditor::update_list);
  ClassDB::bind_method(D_METHOD("on_wrap_button", "index"), &FileEditor::on_wrap_button);
  ClassDB::bind_method(D_METHOD("on_minimap_button", "index"), &FileEditor::on_minimap_button);
  ClassDB::bind_method(D_METHOD("check_file_preview", "file"), &FileEditor::check_file_preview);
  ClassDB::bind_method(D_METHOD("_on_ConfirmationDialog_confirmed"), &FileEditor::_on_ConfirmationDialog_confirmed);

 }



