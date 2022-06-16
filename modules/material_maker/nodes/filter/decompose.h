#ifndef DECOMPOSE_H
#define DECOMPOSE_H


class Decompose : public MMNode {
 GDCLASS(Decompose, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_out_r();
 void set_out_r(const Ref<Resource> &val);

 Ref<Resource> get_out_g();
 void set_out_g(const Ref<Resource> &val);

 Ref<Resource> get_out_b();
 void set_out_b(const Ref<Resource> &val);

 Ref<Resource> get_out_a();
 void set_out_a(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);

 Decompose();
 ~Decompose();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> out_r;
 //export(Resource) 
 Ref<Resource> out_g;
 //export(Resource) 
 Ref<Resource> out_b;
 //export(Resource) 
 Ref<Resource> out_a;
};


#endif
