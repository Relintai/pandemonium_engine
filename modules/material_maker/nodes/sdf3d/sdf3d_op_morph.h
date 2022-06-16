#ifndef SDF3D_OP_MORPH_H
#define SDF3D_OP_MORPH_H


class Sdf3dOpMorph : public MMNode {
 GDCLASS(Sdf3dOpMorph, MMNode);

 public:

 Ref<Resource> get_input1();
 void set_input1(const Ref<Resource> &val);

 Ref<Resource> get_input2();
 void set_input2(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_amount() const;
 void set_amount(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_amount();
 void set_amount(const float val);
 void on_input_changed();

 Sdf3dOpMorph();
 ~Sdf3dOpMorph();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input1;
 //export(Resource) 
 Ref<Resource> input2;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float amount = 0.5;
 //amount
};


#endif
