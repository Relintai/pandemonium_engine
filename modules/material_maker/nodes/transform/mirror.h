#ifndef MM_MIRROR_H
#define MM_MIRROR_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMMirror : public MMNode {
	GDCLASS(MMMirror, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_direction() const;
	void set_direction(const int val);

	float get_offset() const;
	void set_offset(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMMirror();
	~MMMirror();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	//export(int, "Horizontal,Vertical")
	int direction;
	float offset;
};

#endif
