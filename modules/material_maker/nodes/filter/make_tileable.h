#ifndef MM_MAKE_TILEABLE_H
#define MM_MAKE_TILEABLE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMMakeTileable : public MMNode {
	GDCLASS(MMMakeTileable, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_width() const;
	void set_width(const float val);

	int get_size() const;
	void set_size(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	Color make_tileable(const Vector2 &uv, const float w);

	MMMakeTileable();
	~MMMakeTileable();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float width;
	int size;
};

#endif
