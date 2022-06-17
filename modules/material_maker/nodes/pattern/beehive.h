#ifndef MM_BEEHIVE_H
#define MM_BEEHIVE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMBeehive : public MMNode {
	GDCLASS(MMBeehive, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_out_main();
	void set_out_main(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_random_color();
	void set_out_random_color(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_uv_map();
	void set_out_uv_map(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMBeehive();
	~MMBeehive();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> out_main;
	Ref<MMNodeUniversalProperty> out_random_color;
	Ref<MMNodeUniversalProperty> out_uv_map;
	Vector2 size;
};

#endif
