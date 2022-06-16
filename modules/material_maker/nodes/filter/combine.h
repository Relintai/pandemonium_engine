#ifndef COMBINE_H
#define COMBINE_H


class Combine : public MMNode {
 GDCLASS(Combine, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input_r();
 void set_input_r(const Ref<Resource> &val);

 Ref<Resource> get_input_g();
 void set_input_g(const Ref<Resource> &val);

 Ref<Resource> get_input_b();
 void set_input_b(const Ref<Resource> &val);

 Ref<Resource> get_input_a();
 void set_input_a(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);

 Combine();
 ~Combine();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input_r;
 //export(Resource) 
 Ref<Resource> input_g;
 //export(Resource) 
 Ref<Resource> input_b;
 //export(Resource) 
 Ref<Resource> input_a;
};


#endif
