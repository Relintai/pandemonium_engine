#ifndef MM_SDF3D_OP_ROUNDED_H
#define MM_SDF3D_OP_ROUNDED_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dOpRounded : public MMNode {
	GDCLASS(MMSdf3dOpRounded, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	void on_input_changed();

	MMSdf3dOpRounded();
	~MMSdf3dOpRounded();

protected:
	static void _bind_methods();

	//tool
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> input;
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> output;
	//export(float)
	float radius = 0.15;
	//radius
};

#endif
