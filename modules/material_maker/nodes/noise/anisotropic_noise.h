#ifndef ANISOTROPIC_NOISE_H
#define ANISOTROPIC_NOISE_H


class AnisotropicNoise : public MMNode {
 GDCLASS(AnisotropicNoise, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 float get_smoothness() const;
 void set_smoothness(const float val);

 float get_interpolation() const;
 void set_interpolation(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);
 float get_smoothness();
 void set_smoothness(const float val);
 float get_interpolation();
 void set_interpolation(const float val);

 AnisotropicNoise();
 ~AnisotropicNoise();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Vector2) 
 Vector2 scale = Vector2(4, 256);
 //export(float) 
 float smoothness = 1;
 //export(float) 
 float interpolation = 1;
};


#endif
