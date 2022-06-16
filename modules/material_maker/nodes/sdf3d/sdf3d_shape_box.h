#ifndef SDF3D_SHAPE_BOX_H
#define SDF3D_SHAPE_BOX_H


class Sdf3dShapeBox : public MMNode {
 GDCLASS(Sdf3dShapeBox, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector3 get_size();
 void set_size(const Vector3 &val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 Vector3 get_size();
 void set_size(const Vector3 &val);
 float get_radius();
 void set_radius(const float val);

 Sdf3dShapeBox();
 ~Sdf3dShapeBox();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector3) 
 Vector3 size = Vector3(0.3, 0.25, 0.25);
 //export(float) 
 float radius = 0.01;
 //size
 //radius
};


#endif
