#ifndef MM_GREYSCALE_H
#define MM_GREYSCALE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMGreyscale : public MMNode {
	GDCLASS(MMGreyscale, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_type() const;
	void set_type(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMGreyscale();
	~MMGreyscale();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	//export(int, "Lightness,Average,Luminosity,Min,Max")
	int type;
};

#endif
