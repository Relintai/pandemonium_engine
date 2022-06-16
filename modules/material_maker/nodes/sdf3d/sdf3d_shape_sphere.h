#ifndef SDF3D_SHAPE_SPHERE_H
#define SDF3D_SHAPE_SPHERE_H


class Sdf3dShapeSphere : public MMNode {
 GDCLASS(Sdf3dShapeSphere, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 float get_radius();
 void set_radius(const float val);

 Sdf3dShapeSphere();
 ~Sdf3dShapeSphere();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float radius = 0.5;
 //radius
};


#endif
