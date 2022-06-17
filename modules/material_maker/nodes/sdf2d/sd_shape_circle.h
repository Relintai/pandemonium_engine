#ifndef MM_SD_SHAPE_CIRCLE_H
#define MM_SD_SHAPE_CIRCLE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdShapeCircle : public MMNode {
	GDCLASS(MMSdShapeCircle, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_center();
	void set_center(const Vector2 &val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdShapeCircle();
	~MMSdShapeCircle();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector2 center;
	float radius;
};

#endif
