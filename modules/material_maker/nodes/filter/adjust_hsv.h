#ifndef MM_ADJUST_HSV_H
#define MM_ADJUST_HSV_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMAdjustHsv : public MMNode {
	GDCLASS(MMAdjustHsv, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_hue() const;
	void set_hue(const float val);

	float get_saturation() const;
	void set_saturation(const float val);

	float get_value() const;
	void set_value(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMAdjustHsv();
	~MMAdjustHsv();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float hue;
	float saturation;
	float value;
};

#endif
