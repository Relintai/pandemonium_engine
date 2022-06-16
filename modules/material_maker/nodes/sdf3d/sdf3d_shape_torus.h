#ifndef SDF3D_SHAPE_TORUS_H
#define SDF3D_SHAPE_TORUS_H


class Sdf3dShapeTorus : public MMNode {
 GDCLASS(Sdf3dShapeTorus, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_axis() const;
 void set_axis(const int val);

 float get_major_radius() const;
 void set_major_radius(const float val);

 float get_minor_radius() const;
 void set_minor_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 int get_axis();
 void set_axis(const int val);
 float get_major_radius();
 void set_major_radius(const float val);
 float get_minor_radius();
 void set_minor_radius(const float val);

 Sdf3dShapeTorus();
 ~Sdf3dShapeTorus();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "X,Y,Z") 
 int axis = 2;
 //export(float) 
 float major_radius = 0.3;
 //export(float) 
 float minor_radius = 0.15;
 //axis
 //major_radius
 //minor_radius
};


#endif
