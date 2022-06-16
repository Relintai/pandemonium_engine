#ifndef COLOR_NOISE_H
#define COLOR_NOISE_H


class ColorNoise : public MMNode {
 GDCLASS(ColorNoise, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_size() const;
 void set_size(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 int get_size();
 void set_size(const int val);

 ColorNoise();
 ~ColorNoise();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(int) 
 int size = 8;
 //----------------------
 //color_noise.mmg
 //Outputs:
 //Output - (rgb) - Shows the noise pattern
 //color_dots($(uv), 1.0/$(size), $(seed))
 //Inputs:
 //size, float, default: 8, min: 2, max: 12, step: 1
};


#endif
