#ifndef VORONOI_H
#define VORONOI_H


class Voronoi : public MMNode {
 GDCLASS(Voronoi, MMNode);

 public:

 Ref<Resource> get_out_nodes();
 void set_out_nodes(const Ref<Resource> &val);

 Ref<Resource> get_out_borders();
 void set_out_borders(const Ref<Resource> &val);

 Ref<Resource> get_out_random_color();
 void set_out_random_color(const Ref<Resource> &val);

 Ref<Resource> get_out_fill();
 void set_out_fill(const Ref<Resource> &val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 Vector2 get_stretch();
 void set_stretch(const Vector2 &val);

 float get_intensity() const;
 void set_intensity(const float val);

 float get_randomness() const;
 void set_randomness(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);
 Vector2 get_stretch();
 void set_stretch(const Vector2 &val);
 float get_intensity();
 void set_intensity(const float val);
 float get_randomness();
 void set_randomness(const float val);

 Voronoi();
 ~Voronoi();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> out_nodes;
 //export(Resource) 
 Ref<Resource> out_borders;
 //export(Resource) 
 Ref<Resource> out_random_color;
 //export(Resource) 
 Ref<Resource> out_fill;
 //export(Vector2) 
 Vector2 scale = Vector2(4, 4);
 //export(Vector2) 
 Vector2 stretch = Vector2(1, 1);
 //export(float) 
 float intensity = 1;
 //export(float) 
 float randomness = 0.85;
 //scale
 //stretch
 //intensity
 //randomness
};


#endif
