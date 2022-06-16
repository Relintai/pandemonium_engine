#ifndef IMAGE_H
#define IMAGE_H


class Image : public MMNode {
 GDCLASS(Image, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 String get_image_path();
 void set_image_path(const String &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 String get_image_path();
 void set_image_path(const String &val);

 Image();
 ~Image();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(String) 
 String image_path = ;
 //func _render(material) -> void:
 //	var img : Image = render_image(material)
 //
 //	image.set_value(img)
};


#endif
