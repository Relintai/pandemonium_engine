#ifndef QUANTIZE_H
#define QUANTIZE_H


class Quantize : public MMNode {
 GDCLASS(Quantize, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_steps() const;
 void set_steps(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_steps();
 void set_steps(const int val);

 Quantize();
 ~Quantize();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int) 
 int steps = 4;
 //steps
};


#endif
