#ifndef MM_TONES_H
#define MM_TONES_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTones : public MMNode {
	GDCLASS(MMTones, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Color get_in_max() const;
	void set_in_max(const Color &val);

	Color get_in_mid() const;
	void set_in_mid(const Color &val);

	Color get_in_min() const;
	void set_in_min(const Color &val);

	Color get_out_max() const;
	void set_out_max(const Color &val);

	Color get_out_min() const;
	void set_out_min(const Color &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTones();
	~MMTones();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;

	Color _in_max;
	Color _in_mid;
	Color _in_min;
	Color _out_max;
	Color _out_min;
};

#endif
