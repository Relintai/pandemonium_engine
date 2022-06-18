#ifndef MM_COLORIZE_H
#define MM_COLORIZE_H

#include "../bases/gradient_base.h"
#include "../mm_node_universal_property.h"

class MMColorize : public GradientBase {
	GDCLASS(MMColorize, GradientBase);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	Color _get_gradient_color(const float x);

	MMColorize();
	~MMColorize();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
};

#endif
