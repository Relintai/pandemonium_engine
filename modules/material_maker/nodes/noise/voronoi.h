#ifndef MM_VORONOI_H
#define MM_VORONOI_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMVoronoi : public MMNode {
	GDCLASS(MMVoronoi, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_out_nodes();
	void set_out_nodes(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_borders();
	void set_out_borders(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_random_color();
	void set_out_random_color(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_fill();
	void set_out_fill(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	Vector2 get_stretch();
	void set_stretch(const Vector2 &val);

	float get_intensity() const;
	void set_intensity(const float val);

	float get_randomness() const;
	void set_randomness(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMVoronoi();
	~MMVoronoi();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> out_nodes;
	Ref<MMNodeUniversalProperty> out_borders;
	Ref<MMNodeUniversalProperty> out_random_color;
	Ref<MMNodeUniversalProperty> out_fill;
	Vector2 scale;
	Vector2 stretch;
	float intensity;
	float randomness;
};

#endif
