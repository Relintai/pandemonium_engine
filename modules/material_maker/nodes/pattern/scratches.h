#ifndef SCRATCHES_H
#define SCRATCHES_H


class Scratches : public MMNode {
 GDCLASS(Scratches, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Vector2 get_size();
 void set_size(const Vector2 &val);

 int get_layers() const;
 void set_layers(const int val);

 float get_waviness() const;
 void set_waviness(const float val);

 int get_angle() const;
 void set_angle(const int val);

 float get_randomness() const;
 void set_randomness(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_size();
 void set_size(const Vector2 &val);
 int get_layers();
 void set_layers(const int val);
 float get_waviness();
 void set_waviness(const float val);
 int get_angle();
 void set_angle(const int val);
 float get_randomness();
 void set_randomness(const float val);

 Scratches();
 ~Scratches();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Vector2) 
 Vector2 size = Vector2(0.25, 0.4);
 //export(int) 
 int layers = 4;
 //export(float) 
 float waviness = 0.51;
 //export(int) 
 int angle = 0;
 //export(float) 
 float randomness = 0.44;
 //size
 //layers
 //waviness
 //angle
 //randomness
};


#endif
