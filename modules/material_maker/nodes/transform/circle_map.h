#ifndef CIRCLE_MAP_H
#define CIRCLE_MAP_H


class CircleMap : public MMNode {
 GDCLASS(CircleMap, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_radius() const;
 void set_radius(const float val);

 int get_repeat() const;
 void set_repeat(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_radius();
 void set_radius(const float val);
 int get_repeat();
 void set_repeat(const int val);

 CircleMap();
 ~CircleMap();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float radius = 1;
 //export(int) 
 int repeat = 1;
 //radius
 //repeat
};


#endif
