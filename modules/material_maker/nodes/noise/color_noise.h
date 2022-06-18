#ifndef MM_COLOR_NOISE_H
#define MM_COLOR_NOISE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMColorNoise : public MMNode {
	GDCLASS(MMColorNoise, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_size() const;
	void set_size(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMColorNoise();
	~MMColorNoise();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	int size;
};

#endif
