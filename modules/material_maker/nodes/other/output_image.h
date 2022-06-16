#ifndef OUTPUT_IMAGE_H
#define OUTPUT_IMAGE_H


class OutputImage : public MMNode {
 GDCLASS(OutputImage, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 String get_postfix();
 void set_postfix(const String &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 String get_postfix();
 void set_postfix(const String &pf);

 OutputImage();
 ~OutputImage();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(String) 
 String postfix = "";
};


#endif
