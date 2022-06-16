#ifndef MIRROR_H
#define MIRROR_H


class Mirror : public MMNode {
 GDCLASS(Mirror, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_direction() const;
 void set_direction(const int val);

 float get_offset() const;
 void set_offset(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_direction();
 void set_direction(const int val);
 float get_offset();
 void set_offset(const float val);

 Mirror();
 ~Mirror();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "Horizontal,Vertical") 
 int direction = 0;
 //export(float) 
 float offset = 0;
 //direction
 //offset
};


#endif
