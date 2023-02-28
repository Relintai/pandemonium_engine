#ifndef MM_TONES_STEP_H
#define MM_TONES_STEP_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTonesStep : public MMNode {
	GDCLASS(MMTonesStep, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_value() const;
	void set_value(const float val);

	float get_width() const;
	void set_width(const float val);

	bool get_invert() const;
	void set_invert(const bool val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTonesStep();
	~MMTonesStep();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;

	float _value;
	float _width;
	bool _invert;
};

#endif
