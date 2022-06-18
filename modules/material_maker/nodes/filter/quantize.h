#ifndef MM_QUANTIZE_H
#define MM_QUANTIZE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMQuantize : public MMNode {
	GDCLASS(MMQuantize, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_steps() const;
	void set_steps(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMQuantize();
	~MMQuantize();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	int steps;
};

#endif
