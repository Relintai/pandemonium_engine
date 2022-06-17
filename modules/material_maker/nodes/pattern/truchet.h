#ifndef MM_TRUCHET_H
#define MM_TRUCHET_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTruchet : public MMNode {
	GDCLASS(MMTruchet, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_shape() const;
	void set_shape(const int val);

	float get_size() const;
	void set_size(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTruchet();
	~MMTruchet();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Line,Circle")
	int shape;
	float size;
};

#endif
