#ifndef MM_SD_SHAPE_RHOMBUS_H
#define MM_SD_SHAPE_RHOMBUS_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdShapeRhombus : public MMNode {
	GDCLASS(MMSdShapeRhombus, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_center();
	void set_center(const Vector2 &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdShapeRhombus();
	~MMSdShapeRhombus();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector2 center;
	Vector2 size;
};

#endif
