#ifndef SDF3D_TF_SCALE_H
#define SDF3D_TF_SCALE_H


class Sdf3dTfScale : public MMNode {
 GDCLASS(Sdf3dTfScale, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_scale() const;
 void set_scale(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_scale();
 void set_scale(const float val);
 void on_input_changed();

 Sdf3dTfScale();
 ~Sdf3dTfScale();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float scale = 1;
 //scale
};


#endif
