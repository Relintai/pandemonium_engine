#ifndef GREYSCALE_UNIFORM_H
#define GREYSCALE_UNIFORM_H


class GreyscaleUniform : public MMNode {
 GDCLASS(GreyscaleUniform, MMNode);

 public:

 Ref<Resource> get_uniform();
 void set_uniform(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);

 GreyscaleUniform();
 ~GreyscaleUniform();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> uniform;
};


#endif
