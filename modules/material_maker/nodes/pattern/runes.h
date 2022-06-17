#ifndef MM_RUNES_H
#define MM_RUNES_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMRunes : public MMNode {
	GDCLASS(MMRunes, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMRunes();
	~MMRunes();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Vector2 size;
};

#endif
