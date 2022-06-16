#ifndef CURVE_H
#define CURVE_H


class Curve : public MMNode {
 GDCLASS(Curve, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Vector2 get_a();
 void set_a(const Vector2 &val);

 Vector2 get_b();
 void set_b(const Vector2 &val);

 Vector2 get_c();
 void set_c(const Vector2 &val);

 float get_width() const;
 void set_width(const float val);

 int get_repeat() const;
 void set_repeat(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 Vector2 transform_uv(const Vector2 &uv);
 Vector2 get_a();
 void set_a(const Vector2 &val);
 Vector2 get_b();
 void set_b(const Vector2 &val);
 Vector2 get_c();
 void set_c(const Vector2 &val);
 float get_width();
 void set_width(const float val);
 int get_repeat();
 void set_repeat(const int val);

 Curve();
 ~Curve();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Vector2) 
 Vector2 a = Vector2(-0.35, -0.2);
 //export(Vector2) 
 Vector2 b = Vector2(0, 0.5);
 //export(Vector2) 
 Vector2 c = Vector2(0.35, -0.2);
 //export(float) 
 float width = 0.05;
 //export(int) 
 int repeat = 1;
 //b
 //b
 //c
 //width
 //repeat
};


#endif
