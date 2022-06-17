#ifndef MM_SDF3D_SHAPE_CYLINDER_H
#define MM_SDF3D_SHAPE_CYLINDER_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeCylinder : public MMNode {
	GDCLASS(MMSdf3dShapeCylinder, MMNode);

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

	MMSdf3dShapeCylinder();
	~MMSdf3dShapeCylinder();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	//export(int, "X,Y,Z")
	int axis;
	float length;
	float radius;
};

#endif
