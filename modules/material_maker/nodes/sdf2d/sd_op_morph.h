#ifndef MM_SD_OP_MORPH_H
#define MM_SD_OP_MORPH_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdOpMorph : public MMNode {
	GDCLASS(MMSdOpMorph, MMNode);

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
	Variant _get_property_value(const Vector2 &uv);

	void on_input_changed();

	MMSdOpMorph();
	~MMSdOpMorph();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input1;
	Ref<MMNodeUniversalProperty> input2;
	Ref<MMNodeUniversalProperty> output;
	float amount;
};

#endif
