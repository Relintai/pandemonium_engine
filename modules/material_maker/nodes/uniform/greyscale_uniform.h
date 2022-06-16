#ifndef GREYSCALE_UNIFORM_H
#define GREYSCALE_UNIFORM_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class GreyscaleUniform : public MMNode {
	GDCLASS(GreyscaleUniform, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_uniform();
	void set_uniform(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	GreyscaleUniform();
	~GreyscaleUniform();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> uniform;
};

#endif
