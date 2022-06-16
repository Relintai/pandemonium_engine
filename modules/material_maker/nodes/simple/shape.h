#ifndef MM_SHAPE_H
#define MM_SHAPE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMShape : public MMNode {
	GDCLASS(MMShape, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_shape_type() const;
	void set_shape_type(const int val);

	int get_sides() const;
	void set_sides(const int val);

	Ref<MMNodeUniversalProperty> get_radius();
	void set_radius(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_edge();
	void set_edge(const Ref<MMNodeUniversalProperty> &val);

	enum MMShapeType {
		SHAPE_TYPE_CIRCLE = 0,
		SHAPE_TYPE_POLYGON = 1,
		SHAPE_TYPE_STAR = 2,
		SHAPE_TYPE_CURVED_STAR = 3,
		SHAPE_TYPE_RAYS = 4,
	};

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMShape();
	~MMShape();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Circle,Polygon,Star,Curved Star,Rays")
	int shape_type;
	int sides;
	Ref<MMNodeUniversalProperty> radius;
	Ref<MMNodeUniversalProperty> edge;
};

#endif
