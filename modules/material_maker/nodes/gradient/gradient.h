#ifndef MM_GRADIENT_H
#define MM_GRADIENT_H

#include "../bases/gradient_base.h"
#include "../mm_node_universal_property.h"

class MMGradient : public GradientBase {
	GDCLASS(MMGradient, GradientBase);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	float get_repeat() const;
	void set_repeat(const float val);

	float get_rotate() const;
	void set_rotate(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	Color _get_gradient_color(const float x);

	MMGradient();
	~MMGradient();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	float repeat;
	float rotate;
};

#endif
