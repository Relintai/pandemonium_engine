#ifndef ROTATE_H
#define ROTATE_H


class Rotate : public MMNode {
 GDCLASS(Rotate, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Vector2 get_center();
 void set_center(const Vector2 &val);

 float get_rotate() const;
 void set_rotate(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_center();
 void set_center(const Vector2 &val);
 float get_rotate();
 void set_rotate(const float val);

 Rotate();
 ~Rotate();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Vector2) 
 Vector2 center = Vector2();
 //export(float) 
 float rotate = 0;
 //center
 //rotate
};


#endif
