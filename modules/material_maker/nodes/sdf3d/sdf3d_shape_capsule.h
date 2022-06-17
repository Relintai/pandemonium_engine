#ifndef MM_SDF3D_SHAPE_CAPSULE_H
#define MM_SDF3D_SHAPE_CAPSULE_H

#include "../bases/curve_base.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeCapsule : public CurveBase {
	GDCLASS(MMSdf3dShapeCapsule, CurveBase);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_axis() const;
	void set_axis(const int val);

	float get_length() const;
	void set_length(const float val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	Vector2 sdf3d_capsule_y(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_x(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_z(const Vector3 &p, const float r, const float l);

	void _curve_changed();

	MMSdf3dShapeCapsule();
	~MMSdf3dShapeCapsule();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	//export(int, "X,Y,Z")
	int axis;
	float length;
	float radius;
};

#endif
