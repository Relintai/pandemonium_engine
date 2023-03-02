#ifndef MM_TONES_MAP_H
#define MM_TONES_MAP_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTonesMap : public MMNode {
	GDCLASS(MMTonesMap, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_input_range() const;
	void set_input_range(const Vector2 &val);

	Vector2 get_output_range() const;
	void set_output_range(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTonesMap();
	~MMTonesMap();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;

	Vector2 _input_range;
	Vector2 _output_range;
};

#endif
