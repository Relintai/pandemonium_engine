#ifndef MM_EMBOSS_H
#define MM_EMBOSS_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMEmboss : public MMNode {
	GDCLASS(MMEmboss, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_angle() const;
	void set_angle(const float val);

	float get_amount() const;
	void set_amount(const float val);

	float get_width() const;
	void set_width(const float val);

	int get_size() const;
	void set_size(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	float emboss(const Vector2 &uv, const float psize, const float pangle, const float pamount, const float pwidth);

	MMEmboss();
	~MMEmboss();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float angle;
	float amount;
	float width;
	int size;
};

#endif
