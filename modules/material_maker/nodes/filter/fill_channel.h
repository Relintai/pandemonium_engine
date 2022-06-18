#ifndef MM_FILL_CHANNEL_H
#define MM_FILL_CHANNEL_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMFillChannel : public MMNode {
	GDCLASS(MMFillChannel, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_value();
	void set_value(const Ref<MMNodeUniversalProperty> &val);

	int get_channel() const;
	void set_channel(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMFillChannel();
	~MMFillChannel();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> value;
	//export(int, "R,G,B,A")
	int channel;
};

#endif
