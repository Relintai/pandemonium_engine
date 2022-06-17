#ifndef MM_SDF3D_SHAPE_TORUS_H
#define MM_SDF3D_SHAPE_TORUS_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeTorus : public MMNode {
	GDCLASS(MMSdf3dShapeTorus, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_axis() const;
	void set_axis(const int val);

	float get_major_radius() const;
	void set_major_radius(const float val);

	float get_minor_radius() const;
	void set_minor_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	MMSdf3dShapeTorus();
	~MMSdf3dShapeTorus();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	//export(int, "X,Y,Z")
	int axis;
	float major_radius;
	float minor_radius;
};

#endif
