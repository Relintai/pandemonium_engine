#ifndef SD_OP_REPEAT_H
#define SD_OP_REPEAT_H


class SdOpRepeat : public MMNode {
 GDCLASS(SdOpRepeat, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_x() const;
 void set_x(const int val);

 int get_y() const;
 void set_y(const int val);

 float get_random_rotation() const;
 void set_random_rotation(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 int get_x();
 void set_x(const int val);
 int get_y();
 void set_y(const int val);
 float get_random_rotation();
 void set_random_rotation(const float val);

 SdOpRepeat();
 ~SdOpRepeat();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int) 
 int x = 3;
 //export(int) 
 int y = 3;
 //export(float) 
 float random_rotation = 0.5;
 //x
 //y
 //random_rotation
};


#endif
