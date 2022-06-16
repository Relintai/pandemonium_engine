#ifndef SDF3D_COLOR_H
#define SDF3D_COLOR_H


class Sdf3dColor : public MMNode {
 GDCLASS(Sdf3dColor, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_color() const;
 void set_color(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_color();
 void set_color(const float val);
 void on_input_changed();

 Sdf3dColor();
 ~Sdf3dColor();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float color = 0.5;
 //color
};


#endif
