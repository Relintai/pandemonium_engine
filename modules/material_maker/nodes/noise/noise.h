#ifndef NOISE_H
#define NOISE_H


class Noise : public MMNode {
 GDCLASS(Noise, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_grid_size() const;
 void set_grid_size(const int val);

 float get_density() const;
 void set_density(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 int get_grid_size();
 void set_grid_size(const int val);
 float get_density();
 void set_density(const float val);

 Noise();
 ~Noise();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(int) 
 int grid_size = 16;
 //export(float) 
 float density = 0.5;
};


#endif
