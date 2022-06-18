#ifndef MM_COMBINE_H
#define MM_COMBINE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMCombine : public MMNode {
	GDCLASS(MMCombine, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input_r();
	void set_input_r(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input_g();
	void set_input_g(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input_b();
	void set_input_b(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input_a();
	void set_input_a(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMCombine();
	~MMCombine();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input_r;
	Ref<MMNodeUniversalProperty> input_g;
	Ref<MMNodeUniversalProperty> input_b;
	Ref<MMNodeUniversalProperty> input_a;
};

#endif
