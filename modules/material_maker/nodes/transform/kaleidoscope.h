#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H


class Kaleidoscope : public MMNode {
 GDCLASS(Kaleidoscope, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_count() const;
 void set_count(const int val);

 float get_offset() const;
 void set_offset(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_count();
 void set_count(const int val);
 float get_offset();
 void set_offset(const float val);

 Kaleidoscope();
 ~Kaleidoscope();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int) 
 int count = 5;
 //export(float) 
 float offset = 0;
 //count
 //offset
};


#endif
