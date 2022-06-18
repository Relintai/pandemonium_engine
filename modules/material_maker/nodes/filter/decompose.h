#ifndef MM_DECOMPOSE_H
#define MM_DECOMPOSE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMDecompose : public MMNode {
	GDCLASS(MMDecompose, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_r();
	void set_out_r(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_g();
	void set_out_g(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_b();
	void set_out_b(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_a();
	void set_out_a(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMDecompose();
	~MMDecompose();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> out_r;
	Ref<MMNodeUniversalProperty> out_g;
	Ref<MMNodeUniversalProperty> out_b;
	Ref<MMNodeUniversalProperty> out_a;
};

#endif
