#ifndef SDF3D_OP_SMOOTH_BOOL_H
#define SDF3D_OP_SMOOTH_BOOL_H


class Sdf3dOpSmoothBool : public MMNode {
 GDCLASS(Sdf3dOpSmoothBool, MMNode);

 public:

 Ref<Resource> get_input1();
 void set_input1(const Ref<Resource> &val);

 Ref<Resource> get_input2();
 void set_input2(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_operation() const;
 void set_operation(const int val);

 float get_smoothness() const;
 void set_smoothness(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 int get_operation();
 void set_operation(const int val);
 float get_smoothness();
 void set_smoothness(const float val);
 void on_input_changed();

 Sdf3dOpSmoothBool();
 ~Sdf3dOpSmoothBool();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input1;
 //export(Resource) 
 Ref<Resource> input2;
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "Union,Substraction,Intersection") 
 int operation = 0;
 //export(float) 
 float smoothness = 0.15;
 //operation
 //smoothness
};


#endif
