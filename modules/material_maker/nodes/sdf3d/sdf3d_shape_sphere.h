#ifndef MM_SDF3D_SHAPE_SPHERE_H
#define MM_SDF3D_SHAPE_SPHERE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeSphere : public MMNode {
	GDCLASS(MMSdf3dShapeSphere, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	MMSdf3dShapeSphere();
	~MMSdf3dShapeSphere();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	float radius;
};

#endif
