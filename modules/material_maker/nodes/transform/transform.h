#ifndef TRANSFORM_H
#define TRANSFORM_H


class Transform : public MMNode {
 GDCLASS(Transform, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_translate_x();
 void set_translate_x(const Ref<Resource> &val);

 Ref<Resource> get_translate_y();
 void set_translate_y(const Ref<Resource> &val);

 Ref<Resource> get_rotate();
 void set_rotate(const Ref<Resource> &val);

 Ref<Resource> get_scale_x();
 void set_scale_x(const Ref<Resource> &val);

 Ref<Resource> get_scale_y();
 void set_scale_y(const Ref<Resource> &val);

 int get_mode() const;
 void set_mode(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_mode();
 void set_mode(const int val);

 Transform();
 ~Transform();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> translate_x;
 //export(Resource) 
 Ref<Resource> translate_y;
 //export(Resource) 
 Ref<Resource> rotate;
 //export(Resource) 
 Ref<Resource> scale_x;
 //export(Resource) 
 Ref<Resource> scale_y;
 //export(int, "Clamp,Repeat,Extend") 
 int mode = 0;
 //mode
};


#endif
