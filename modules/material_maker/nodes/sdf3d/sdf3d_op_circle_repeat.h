#ifndef SDF3D_OP_CIRCLE_REPEAT_H
#define SDF3D_OP_CIRCLE_REPEAT_H


class Sdf3dOpCircleRepeat : public MMNode {
 GDCLASS(Sdf3dOpCircleRepeat, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_count() const;
 void set_count(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 int get_count();
 void set_count(const int val);
 void on_input_changed();

 Sdf3dOpCircleRepeat();
 ~Sdf3dOpCircleRepeat();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(int) 
 int count = 5;
 //count
};


#endif
