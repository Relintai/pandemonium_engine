
#include "preview.h"


RichTextLabel Preview::get_*text_preview() {
 return *text_preview;
}

void Preview::set_*text_preview(const RichTextLabel &val) {
*text_preview = val;
}


GridContainer Preview::get_*table_preview() {
 return *table_preview;
}

void Preview::set_*table_preview(const GridContainer &val) {
*table_preview = val;
}



 //tool;
  RichTextLabel *text_preview = null;
  GridContainer *table_preview = null;
 signal image_downloaded();
 signal image_loaded();

 void Preview::_init() {
  window_title = "File preview";
  resizable = true;
  set_anchors_and_margins_preset(Control.PRESET_WIDE);
  margin_left = 81;
  margin_top = 47;
  margin_right = -80;
  margin_bottom = -48;
   VBoxContainer *vbc = VBoxContainer.new();
  vbc.set_anchors_and_margins_preset(Control.PRESET_WIDE);
  add_child(vbc);
  text_preview = RichTextLabel.new();
  vbc.add_child(text_preview);
  text_preview.scroll_following = true;
  text_preview.bbcode_enabled = true;
  text_preview.size_flags_vertical = SIZE_EXPAND_FILL;
  text_preview.hide();
  table_preview = GridContainer.new();
  vbc.add_child(table_preview);
  table_preview.columns = 3;
  table_preview.size_flags_horizontal = SIZE_EXPAND_FILL;
  table_preview.size_flags_vertical = SIZE_EXPAND_FILL;
  table_preview.hide();
  connect("popup_hide", self, "_on_Preview_popup_hide");
}


 void Preview::print_preview(const String &content) {
  text_preview.append_bbcode(content);
  text_preview.show();
}


 void Preview::print_bb(const String &content) {
  text_preview.append_bbcode(content);
  text_preview.show();
}


 void Preview::print_markdown(const String &content) {
   Array result = Array();
   Array bolded = Array();
   Array italics = Array();
   Array striked = Array();
   Array coded = Array();
   Array linknames = Array();
   Array images = Array();
   Array links = Array();
   Array lists = Array();
   Array underlined = Array();
   Ref<RegEx> regex = RegEx.new();
  regex.compile('\\*\\*(?<boldtext>.*)\\*\\*');
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   bolded.append(res.get_string("boldtext"));
}

  regex.compile('\\_\\_(?<underlinetext>.*)\\_\\_');
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   underlined.append(res.get_string("underlinetext"));
}

  regex.compile("\\*(?<italictext>.*)\\*");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   italics.append(res.get_string("italictext"));
}

  regex.compile("~~(?<strikedtext>.*)~~");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   striked.append(res.get_string("strikedtext"));
}

  regex.compile("`(?<coded>.*)`");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   coded.append(res.get_string("coded"));
}

  regex.compile("[+-*](?<element>\\s.*)");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   lists.append(res.get_string("element"));
}

  regex.compile("(?<img>!\\[.*?\\))");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];
   images.append(res.get_string("img"));
}

  regex.compile("\\[(?<linkname>.*?)\\]|\\((?<link>[h\\.]\\S*?)\\)");
  result = regex.search_all(content);

  for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
    Ref<RegExMatch> res = result[i];

   if (res.get_string("link")!="") {
    links.append(res.get_string("link"));
}


   if (res.get_string("linkname")!="") {
    linknames.append(res.get_string("linkname"));
}

}


  for (int i = 0; i < bolded.size(); ++i) { //i in range(bolded.size())
    String bold = bolded[i];
   content = content.replace("**"+bold+"**","[b]"+bold+"[/b]");
}


  for (int i = 0; i < italics.size(); ++i) { //i in range(italics.size())
    String italic = italics[i];
   content = content.replace("*"+italic+"*","[i]"+italic+"[/i]");
}


  for (int i = 0; i < striked.size(); ++i) { //i in range(striked.size())
    String strik = striked[i];
   content = content.replace("~~"+strik+"~~","[s]"+strik+"[/s]");
}


  for (int i = 0; i < underlined.size(); ++i) { //i in range(underlined.size())
    String underline = underlined[i];
   content = content.replace("__"+underline+"__","[u]"+underline+"[/u]");
}


  for (int i = 0; i < coded.size(); ++i) { //i in range(coded.size())
    String code = coded[i];
   content = content.replace("`"+code+"`","[code]"+code+"[/code]");
}


  for (int i = 0; i < images.size(); ++i) { //i in range(images.size())
    String image = images[i];
    Variant  = image.split("(");
    Variant  = substr[1].rstrip(")");
   content = content.replace(image,"[img]"+imglink+"[/img]");
}


  for (i in links.size()) {
   content = content.replace("["+linknames[i]+"]("+links[i]+")","[url="+links[i]+"]"+linknames[i]+"[/url]");
}


  for (int i = 0; i < lists.size(); ++i) { //i in range(lists.size())
    String element = lists[i];

   if (content.find("- "+element)) {
    content = content.replace("-"+element,"[indent]-"+element+"[/indent]");
}


   if (content.find("+ "+element)) {
    content = content.replace("+"+element,"[indent]-"+element+"[/indent]");
}


   if (content.find("* "+element)) {
    content = content.replace("+"+element,"[indent]-"+element+"[/indent]");
}

}

  text_preview.append_bbcode(content);
  text_preview.show();
}


 void Preview::print_html(const String &content) {
  content = content.replace("<i>","[i]");
  content = content.replace("</i>","[/i]");
  content = content.replace("<b>","[b]");
  content = content.replace("</b>","[/b]");
  content = content.replace("<u>","[u]");
  content = content.replace("</u>","[/u]");
  content = content.replace("<ins>","[u]");
  content = content.replace("</ins>","[/u]");
  content = content.replace("<del>","[s]");
  content = content.replace("</del>","[/s]");
  content = content.replace('<a href="',"[url=");
  content = content.replace('">',"]");
  content = content.replace("</a>","[/url]");
  content = content.replace('<img src="',"[img]");
  content = content.replace('" />',"[/img]");
  content = content.replace('"/>',"[/img]");
  content = content.replace("<pre>","[code]");
  content = content.replace("</pre>","[/code]");
  content = content.replace("<center>","[center]");
  content = content.replace("</center>","[/center]");
  content = content.replace("<right>","[right]");
  content = content.replace("</right>","[/right]");
  text_preview.append_bbcode(content);
  text_preview.show();
}


 void Preview::print_csv(const Array &rows) {
  table_preview.columns = rows[0].size();

  for (item in rows) {

   for (string in item) {
     Variant  = Label.new();
    label.text = str(string);
    label.set_h_size_flags(SIZE_EXPAND);
    label.set_align(1);
    label.set_valign(1);
    table_preview.add_child(label);
}

}

  table_preview.show();
}


 void Preview::_on_Preview_popup_hide() {
  queue_free();
}

}

 Preview::Preview() {
  *text_preview = null;
  *table_preview = null;
 }

 Preview::~Preview() {
 }


 static void Preview::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*text_preview"), &Preview::get_*text_preview);
   ClassDB::bind_method(D_METHOD("set_*text_preview", "value"), &Preview::set_*text_preview);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*text_preview", PROPERTY_HINT_RESOURCE_TYPE, "RichTextLabel"), "set_*text_preview", "get_*text_preview");


   ClassDB::bind_method(D_METHOD("get_*table_preview"), &Preview::get_*table_preview);
   ClassDB::bind_method(D_METHOD("set_*table_preview", "value"), &Preview::set_*table_preview);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*table_preview", PROPERTY_HINT_RESOURCE_TYPE, "GridContainer"), "set_*table_preview", "get_*table_preview");


  ClassDB::bind_method(D_METHOD("_init"), &Preview::_init);
  ClassDB::bind_method(D_METHOD("print_preview", "content"), &Preview::print_preview);
  ClassDB::bind_method(D_METHOD("print_bb", "content"), &Preview::print_bb);
  ClassDB::bind_method(D_METHOD("print_markdown", "content"), &Preview::print_markdown);
  ClassDB::bind_method(D_METHOD("print_html", "content"), &Preview::print_html);
  ClassDB::bind_method(D_METHOD("print_csv", "rows"), &Preview::print_csv);
  ClassDB::bind_method(D_METHOD("_on_Preview_popup_hide"), &Preview::_on_Preview_popup_hide);

 }



