#ifndef MM_FILL_TO_SIZE_H
#define MM_FILL_TO_SIZE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMFillToSize : public MMNode {
	GDCLASS(MMFillToSize, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_formula() const;
	void set_formula(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMFillToSize();
	~MMFillToSize();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	//export(int, "Area,Width,Height,Max(W,H)")
	int formula;
};

#endif
