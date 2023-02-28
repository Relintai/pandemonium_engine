#ifndef MM_WARP_H
#define MM_WARP_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMWarp : public MMNode {
	GDCLASS(MMWarp, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_height_map();
	void set_height_map(const Ref<MMNodeUniversalProperty> &val);

	int get_mode() const;
	void set_mode(const int val);

	float get_amount() const;
	void set_amount(const float val);

	float get_epsilon() const;
	void set_epsilon(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMWarp();
	~MMWarp();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> _image;
	Ref<MMNodeUniversalProperty> _input;
	Ref<MMNodeUniversalProperty> _height_map;

	//export(int, "Slope,Distance to top")
	int _mode;
	float _amount;
	float _epsilon;
};

#endif
