#ifndef MM_SHEAR_H
#define MM_SHEAR_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMShear : public MMNode {
	GDCLASS(MMShear, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_direction() const;
	void set_direction(const int val);

	float get_amount() const;
	void set_amount(const float val);

	float get_center() const;
	void set_center(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMShear();
	~MMShear();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	//export(int, "Horizontal,Vertical")
	int direction;
	float amount;
	float center;
};

#endif
