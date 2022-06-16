#ifndef SCALE_H
#define SCALE_H


class Scale : public MMNode {
 GDCLASS(Scale, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Vector2 get_center();
 void set_center(const Vector2 &val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_center();
 void set_center(const Vector2 &val);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 Scale();
 ~Scale();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Vector2) 
 Vector2 center = Vector2();
 //export(Vector2) 
 Vector2 scale = Vector2(1, 1);
 //center
 //scale
};


#endif
