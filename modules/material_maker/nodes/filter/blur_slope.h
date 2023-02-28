#ifndef MM_BLUR_SLOPE_H
#define MM_BLUR_SLOPE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMBlurSlope : public MMNode {
	GDCLASS(MMBlurSlope, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_sigma();
	void set_sigma(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Ref<Image> _render_image(const Ref<MMMaterial> &material);

	Color slope_blur(const Vector2 &uv, const float psize);

	MMBlurSlope();
	~MMBlurSlope();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> sigma;

	float size;
};

#endif
