#ifndef MM_INVERT_H
#define MM_INVERT_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMInvert : public MMNode {
	GDCLASS(MMInvert, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMInvert();
	~MMInvert();

protected:
	static void _bind_methods();

	//tool
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> image;
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> input;
};

#endif
