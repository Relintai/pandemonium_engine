#ifndef MM_FILL_TO_COLOR_H
#define MM_FILL_TO_COLOR_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMFillToColor : public MMNode {
	GDCLASS(MMFillToColor, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_color_map();
	void set_color_map(const Ref<MMNodeUniversalProperty> &val);

	Color get_edge_color();
	void set_edge_color(const Color &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMFillToColor();
	~MMFillToColor();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> color_map;
	Color edge_color;
};

#endif
