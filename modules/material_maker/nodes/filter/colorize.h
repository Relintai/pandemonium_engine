#ifndef COLORIZE_H
#define COLORIZE_H


class Colorize : public GradientBase {
 GDCLASS(Colorize, GradientBase);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Color get_gradient_color(const float x);

 Colorize();
 ~Colorize();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //	return Color(0.5, 0.5, 0.5, 1)
};


#endif
