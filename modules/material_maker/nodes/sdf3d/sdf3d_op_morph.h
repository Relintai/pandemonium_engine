#ifndef MM_SDF3D_OP_MORPH_H
#define MM_SDF3D_OP_MORPH_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dOpMorph : public MMNode {
	GDCLASS(MMSdf3dOpMorph, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input1();
	void set_input1(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input2();
	void set_input2(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_amount() const;
	void set_amount(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	void on_input_changed();

	MMSdf3dOpMorph();
	~MMSdf3dOpMorph();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input1;
	Ref<MMNodeUniversalProperty> input2;
	Ref<MMNodeUniversalProperty> output;
	float amount;
};

#endif
