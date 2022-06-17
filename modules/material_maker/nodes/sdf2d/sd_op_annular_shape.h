#ifndef MM_SD_OP_ANNULAR_SHAPE_H
#define MM_SD_OP_ANNULAR_SHAPE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdOpAnnularShape : public MMNode {
	GDCLASS(MMSdOpAnnularShape, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_width() const;
	void set_width(const float val);

	int get_ripples() const;
	void set_ripples(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdOpAnnularShape();
	~MMSdOpAnnularShape();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	float width;
	int ripples;
};

#endif
