#ifndef SDF3D_OP_EXTRUSION_H
#define SDF3D_OP_EXTRUSION_H


class Sdf3dOpExtrusion : public MMNode {
 GDCLASS(Sdf3dOpExtrusion, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_length() const;
 void set_length(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_length();
 void set_length(const float val);
 void on_input_changed();

 Sdf3dOpExtrusion();
 ~Sdf3dOpExtrusion();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float length = 0.25;
 //length
};


#endif
