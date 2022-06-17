#ifndef MM_SDF3D_SHAPE_BOX_H
#define MM_SDF3D_SHAPE_BOX_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeBox : public MMNode {
	GDCLASS(MMSdf3dShapeBox, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector3 get_size();
	void set_size(const Vector3 &val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	MMSdf3dShapeBox();
	~MMSdf3dShapeBox();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector3 size;
	float radius;
};

#endif
