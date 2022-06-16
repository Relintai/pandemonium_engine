#ifndef SDF3D_SHAPE_CONE_H
#define SDF3D_SHAPE_CONE_H


class Sdf3dShapeCone : public MMNode {
 GDCLASS(Sdf3dShapeCone, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_axis() const;
 void set_axis(const int val);

 float get_angle() const;
 void set_angle(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 int get_axis();
 void set_axis(const int val);
 float get_angle();
 void set_angle(const float val);

 Sdf3dShapeCone();
 ~Sdf3dShapeCone();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "+X,-X,+Y,-Y,+Z,-Z") 
 int axis = 2;
 //export(float) 
 float angle = 30;
 //axis
 //angle
};


#endif
