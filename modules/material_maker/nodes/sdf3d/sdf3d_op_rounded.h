#ifndef SDF3D_OP_ROUNDED_H
#define SDF3D_OP_ROUNDED_H


class Sdf3dOpRounded : public MMNode {
 GDCLASS(Sdf3dOpRounded, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_radius();
 void set_radius(const float val);
 void on_input_changed();

 Sdf3dOpRounded();
 ~Sdf3dOpRounded();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float radius = 0.15;
 //radius
};


#endif
