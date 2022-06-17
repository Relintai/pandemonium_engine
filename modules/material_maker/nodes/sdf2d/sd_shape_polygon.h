#ifndef MM_SD_SHAPE_POLYGON_H
#define MM_SD_SHAPE_POLYGON_H

#include "../bases/polygon_base.h"
#include "../mm_node_universal_property.h"

class MMSdShapePolygon : public PolygonBase {
	GDCLASS(MMSdShapePolygon, PolygonBase);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	void _polygon_changed();

	MMSdShapePolygon();
	~MMSdShapePolygon();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
};

#endif
