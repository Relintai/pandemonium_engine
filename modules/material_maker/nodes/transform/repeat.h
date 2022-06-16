#ifndef MM_REPEAT_H
#define MM_REPEAT_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMRepeat : public MMNode {
	GDCLASS(MMRepeat, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMRepeat();
	~MMRepeat();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
};

#endif
