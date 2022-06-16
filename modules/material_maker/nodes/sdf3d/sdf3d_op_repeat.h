#ifndef SDF3D_OP_REPEAT_H
#define SDF3D_OP_REPEAT_H


class Sdf3dOpRepeat : public MMNode {
 GDCLASS(Sdf3dOpRepeat, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_col_row();
 void set_col_row(const Vector2 &val);

 float get_rotation() const;
 void set_rotation(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 Vector2 get_col_row();
 void set_col_row(const Vector2 &val);
 float get_rotation();
 void set_rotation(const float val);
 void on_input_changed();

 Sdf3dOpRepeat();
 ~Sdf3dOpRepeat();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 col_row = Vector2(3, 3);
 //export(float) 
 float rotation = 0.3;
 //col_row
 //rotation
};


#endif
