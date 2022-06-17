#ifndef MM_SD_OP_ROUNDED_SHAPE_H
#define MM_SD_OP_ROUNDED_SHAPE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdOpRoundedShape : public MMNode {
	GDCLASS(MMSdOpRoundedShape, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdOpRoundedShape();
	~MMSdOpRoundedShape();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	float radius;
};

#endif
