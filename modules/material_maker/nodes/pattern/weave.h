#ifndef MM_WEAVE_H
#define MM_WEAVE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMWeave : public MMNode {
	GDCLASS(MMWeave, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_out_main();
	void set_out_main(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_horizontal_map();
	void set_out_horizontal_map(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_vertical_map();
	void set_out_vertical_map(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	Ref<MMNodeUniversalProperty> get_width();
	void set_width(const Ref<MMNodeUniversalProperty> &val);

	int get_stitch() const;
	void set_stitch(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMWeave();
	~MMWeave();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> out_main;
	Ref<MMNodeUniversalProperty> out_horizontal_map;
	Ref<MMNodeUniversalProperty> out_vertical_map;
	Vector2 size;
	Ref<MMNodeUniversalProperty> width;
	int stitch;
};

#endif
