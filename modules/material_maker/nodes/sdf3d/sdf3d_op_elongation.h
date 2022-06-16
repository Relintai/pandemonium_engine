#ifndef SDF3D_OP_ELONGATION_H
#define SDF3D_OP_ELONGATION_H


class Sdf3dOpElongation : public MMNode {
 GDCLASS(Sdf3dOpElongation, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector3 get_length();
 void set_length(const Vector3 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 Vector3 get_length();
 void set_length(const Vector3 &val);
 void on_input_changed();

 Sdf3dOpElongation();
 ~Sdf3dOpElongation();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector3) 
 Vector3 length = Vector3(0.2, 0, 0);
 //length
};


#endif
