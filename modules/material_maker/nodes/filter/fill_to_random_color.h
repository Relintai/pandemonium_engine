#ifndef MM_FILL_TO_RANDOM_COLOR_H
#define MM_FILL_TO_RANDOM_COLOR_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMFillToRandomColor : public MMNode {
	GDCLASS(MMFillToRandomColor, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Color get_edge_color();
	void set_edge_color(const Color &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMFillToRandomColor();
	~MMFillToRandomColor();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Color edge_color;
};

#endif
