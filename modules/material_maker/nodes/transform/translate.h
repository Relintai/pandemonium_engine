#ifndef TRANSLATE_H
#define TRANSLATE_H


class Translate : public MMNode {
 GDCLASS(Translate, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Vector2 get_translation();
 void set_translation(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_translation();
 void set_translation(const Vector2 &val);

 Translate();
 ~Translate();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Vector2) 
 Vector2 translation = Vector2();
 //translation
};


#endif
