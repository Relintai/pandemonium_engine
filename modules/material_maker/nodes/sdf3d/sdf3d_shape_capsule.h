#ifndef SDF3D_SHAPE_CAPSULE_H
#define SDF3D_SHAPE_CAPSULE_H


class Sdf3dShapeCapsule : public CurveBase {
 GDCLASS(Sdf3dShapeCapsule, CurveBase);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_axis() const;
 void set_axis(const int val);

 float get_length() const;
 void set_length(const float val);

 float get_radius() const;
 void set_radius(const float val);

 void _init();
 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 Vector2 sdf3d_capsule_y(const Vector3 &p, const float r, const float l);
 Vector2 sdf3d_capsule_x(const Vector3 &p, const float r, const float l);
 Vector2 sdf3d_capsule_z(const Vector3 &p, const float r, const float l);
 int get_axis();
 void set_axis(const int val);
 float get_length();
 void set_length(const float val);
 float get_radius();
 void set_radius(const float val);
 void _curve_changed();

 Sdf3dShapeCapsule();
 ~Sdf3dShapeCapsule();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "X,Y,Z") 
 int axis = 1;
 //export(float) 
 float length = 0.3;
 //export(float) 
 float radius = 0.2;
 //vec3 $(name_uv)_p = $uv;
 //$(name_uv)_p.$axis -= clamp($(name_uv)_p.$axis, -$l, $l);
 //return length($(name_uv)_p) - $r * $profile(clamp(0.5+0.5*($uv).$axis/$l, 0.0, 1.0))
 //axis
 //length
 //radius
};


#endif
