#ifndef MM_SD_OP_CIRCLE_REPEAT_H
#define MM_SD_OP_CIRCLE_REPEAT_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdOpCircleRepeat : public MMNode {
	GDCLASS(MMSdOpCircleRepeat, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_count() const;
	void set_count(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdOpCircleRepeat();
	~MMSdOpCircleRepeat();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	int count;
};

#endif
