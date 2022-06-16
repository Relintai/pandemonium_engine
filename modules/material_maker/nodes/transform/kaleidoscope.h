#ifndef MM_KALEIDOSCOPE_H
#define MM_KALEIDOSCOPE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMKaleidoscope : public MMNode {
	GDCLASS(MMKaleidoscope, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_count() const;
	void set_count(const int val);

	float get_offset() const;
	void set_offset(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMKaleidoscope();
	~MMKaleidoscope();

protected:
	static void _bind_methods();

	//tool
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> image;
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> input;
	//export(int)
	int count = 5;
	//export(float)
	float offset = 0;
	//count
	//offset
};

#endif
