#ifndef MM_CIRCLE_MAP_H
#define MM_CIRCLE_MAP_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMCircleMap : public MMNode {
	GDCLASS(MMCircleMap, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_radius() const;
	void set_radius(const float val);

	int get_repeat() const;
	void set_repeat(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMCircleMap();
	~MMCircleMap();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float radius = 1;
	int repeat = 1;
};

#endif
