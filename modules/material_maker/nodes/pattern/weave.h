#ifndef WEAVE_H
#define WEAVE_H


class Weave : public MMNode {
 GDCLASS(Weave, MMNode);

 public:

 Ref<Resource> get_out_main();
 void set_out_main(const Ref<Resource> &val);

 Ref<Resource> get_out_horizontal_map();
 void set_out_horizontal_map(const Ref<Resource> &val);

 Ref<Resource> get_out_vertical_map();
 void set_out_vertical_map(const Ref<Resource> &val);

 Vector2 get_size();
 void set_size(const Vector2 &val);

 Ref<Resource> get_width();
 void set_width(const Ref<Resource> &val);

 int get_stitch() const;
 void set_stitch(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_size();
 void set_size(const Vector2 &val);
 int get_stitch();
 void set_stitch(const int val);

 Weave();
 ~Weave();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> out_main;
 //export(Resource) 
 Ref<Resource> out_horizontal_map;
 //export(Resource) 
 Ref<Resource> out_vertical_map;
 //export(Vector2) 
 Vector2 size = Vector2(4, 4);
 //export(Resource) 
 Ref<Resource> width;
 //export(int) 
 int stitch = 1;
 //size
 //stitch
};


#endif
