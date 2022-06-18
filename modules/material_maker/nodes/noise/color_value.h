#ifndef MM_COLOR_VALUE_H
#define MM_COLOR_VALUE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMColorValue : public MMNode {
	GDCLASS(MMColorValue, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	int get_iterations() const;
	void set_iterations(const int val);

	float get_persistence() const;
	void set_persistence(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMColorValue();
	~MMColorValue();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Vector2 scale;
	int iterations;
	float persistence;
};

#endif
