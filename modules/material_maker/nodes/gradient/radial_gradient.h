#ifndef RADIAL_GRADIENT_H
#define RADIAL_GRADIENT_H


class RadialGradient : public GradientBase {
 GDCLASS(RadialGradient, GradientBase);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 float get_repeat() const;
 void set_repeat(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Color _get_gradient_color(const float x);
 float get_repeat();
 void set_repeat(const float val);

 RadialGradient();
 ~RadialGradient();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(float) 
 float repeat = 1;
};


#endif
