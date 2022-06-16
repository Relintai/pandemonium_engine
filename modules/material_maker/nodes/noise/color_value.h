#ifndef COLOR_VALUE_H
#define COLOR_VALUE_H


class ColorValue : public MMNode {
 GDCLASS(ColorValue, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 int get_iterations() const;
 void set_iterations(const int val);

 float get_persistence() const;
 void set_persistence(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);
 int get_iterations();
 void set_iterations(const int val);
 float get_persistence();
 void set_persistence(const float val);

 ColorValue();
 ~ColorValue();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Vector2) 
 Vector2 scale = Vector2(4, 4);
 //export(int) 
 int iterations = 3;
 //export(float) 
 float persistence = 0.5;
};


#endif
