#ifndef MM_SDF3D_OP_ELONGATION_H
#define MM_SDF3D_OP_ELONGATION_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dOpElongation : public MMNode {
	GDCLASS(MMSdf3dOpElongation, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector3 get_length();
	void set_length(const Vector3 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	void on_input_changed();

	MMSdf3dOpElongation();
	~MMSdf3dOpElongation();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> output;
	Vector3 length;
};

#endif
