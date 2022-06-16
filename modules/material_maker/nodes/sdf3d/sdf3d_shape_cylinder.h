#ifndef SDF3D_SHAPE_CYLINDER_H
#define SDF3D_SHAPE_CYLINDER_H


class Sdf3dShapeCylinder : public MMNode {
 GDCLASS(Sdf3dShapeCylinder, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_axis() const;
 void set_axis(const int val);

 float get_length() const;
 void set_length(const float val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 int get_axis();
 void set_axis(const int val);
 float get_length();
 void set_length(const float val);
 float get_radius();
 void set_radius(const float val);

 Sdf3dShapeCylinder();
 ~Sdf3dShapeCylinder();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "X,Y,Z") 
 int axis = 1;
 //export(float) 
 float length = 0.25;
 //export(float) 
 float radius = 0.25;
 //axis
 //length
 //radius
};


#endif
