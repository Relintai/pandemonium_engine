#ifndef UNIFORM_H
#define UNIFORM_H


class Uniform : public MMNode {
 GDCLASS(Uniform, MMNode);

 public:

 Ref<Resource> get_uniform();
 void set_uniform(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);

 Uniform();
 ~Uniform();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> uniform;
};


#endif
