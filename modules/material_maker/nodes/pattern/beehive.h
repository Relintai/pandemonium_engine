#ifndef BEEHIVE_H
#define BEEHIVE_H


class Beehive : public MMNode {
 GDCLASS(Beehive, MMNode);

 public:

 Ref<Resource> get_out_main();
 void set_out_main(const Ref<Resource> &val);

 Ref<Resource> get_out_random_color();
 void set_out_random_color(const Ref<Resource> &val);

 Ref<Resource> get_out_uv_map();
 void set_out_uv_map(const Ref<Resource> &val);

 Vector2 get_size();
 void set_size(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_size();
 void set_size(const Vector2 &val);

 Beehive();
 ~Beehive();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> out_main;
 //export(Resource) 
 Ref<Resource> out_random_color;
 //export(Resource) 
 Ref<Resource> out_uv_map;
 //export(Vector2) 
 Vector2 size = Vector2(4, 4);
 //size
};


#endif
