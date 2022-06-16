#ifndef REPEAT_H
#define REPEAT_H


class Repeat : public MMNode {
 GDCLASS(Repeat, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);

 Repeat();
 ~Repeat();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
};


#endif
