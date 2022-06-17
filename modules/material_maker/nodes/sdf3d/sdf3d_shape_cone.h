#ifndef MM_SDF3D_SHAPE_CONE_H
#define MM_SDF3D_SHAPE_CONE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeCone : public MMNode {
	GDCLASS(MMSdf3dShapeCone, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_axis() const;
	void set_axis(const int val);

	float get_angle() const;
	void set_angle(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	MMSdf3dShapeCone();
	~MMSdf3dShapeCone();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	//export(int, "+X,-X,+Y,-Y,+Z,-Z")
	int axis;
	float angle;
};

#endif
