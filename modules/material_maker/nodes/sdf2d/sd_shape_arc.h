#ifndef MM_SD_SHAPE_ARC_H
#define MM_SD_SHAPE_ARC_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdShapeArc : public MMNode {
	GDCLASS(MMSdShapeArc, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_angle();
	void set_angle(const Vector2 &val);

	float get_radius() const;
	void set_radius(const float val);

	float get_width() const;
	void set_width(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdShapeArc();
	~MMSdShapeArc();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector2 angle;
	float radius;
	float width;
};

#endif
