#ifndef MM_BLEND_H
#define MM_BLEND_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMBlend : public MMNode {
	GDCLASS(MMBlend, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input1();
	void set_input1(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input2();
	void set_input2(const Ref<MMNodeUniversalProperty> &val);

	int get_blend_type() const;
	void set_blend_type(const int val);

	Ref<MMNodeUniversalProperty> get_opacity();
	void set_opacity(const Ref<MMNodeUniversalProperty> &val);

	enum MMBlendType {
		BLEND_TYPE_NORMAL = 0,
		BLEND_TYPE_DISSOLVE,
		BLEND_TYPE_MULTIPLY,
		BLEND_TYPE_SCREEN,
		BLEND_TYPE_OVERLAY,
		BLEND_TYPE_HARD_LIGHT,
		BLEND_TYPE_SOFT_LIGHT,
		BLEND_TYPE_BURN,
		BLEND_TYPE_DODGE,
		BLEND_TYPE_LIGHTEN,
		BLEND_TYPE_DARKEN,
		BLEND_TYPE_DIFFRENCE
	};

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMBlend();
	~MMBlend();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input1;
	Ref<MMNodeUniversalProperty> input2;
	//export(int, "Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference")
	int blend_type;
	Ref<MMNodeUniversalProperty> opacity;
};

#endif
