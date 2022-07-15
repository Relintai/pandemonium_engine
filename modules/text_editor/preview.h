#ifndef PREVIEW_H
#define PREVIEW_H


class Preview : public WindowDialog {
 GDCLASS(Preview, WindowDialog);

 public:

 RichTextLabel get_*text_preview();
 void set_*text_preview(const RichTextLabel &val);

 GridContainer get_*table_preview();
 void set_*table_preview(const GridContainer &val);

 void _init();
 void print_preview(const String &content);
 void print_bb(const String &content);
 void print_markdown(const String &content);
 void print_html(const String &content);
 void print_csv(const Array &rows);
 void _on_Preview_popup_hide();

 Preview();
 ~Preview();

 protected:
 static void _bind_methods();

 //tool
 RichTextLabel *text_preview = null;
 GridContainer *table_preview = null;
 signal image_downloaded();
 signal image_loaded();
};


#endif
