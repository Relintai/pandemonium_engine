#ifndef MM_COLOR_TILER_H
#define MM_COLOR_TILER_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMColorTiler : public MMNode {
	GDCLASS(MMColorTiler, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_in_mask();
	void set_in_mask(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_instance_map();
	void set_instance_map(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_tile();
	void set_tile(const Vector2 &val);

	float get_overlap() const;
	void set_overlap(const float val);

	int get_select_inputs() const;
	void set_select_inputs(const int val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	float get_fixed_offset() const;
	void set_fixed_offset(const float val);

	float get_rnd_offset() const;
	void set_rnd_offset(const float val);

	float get_rnd_rotate() const;
	void set_rnd_rotate(const float val);

	float get_rnd_scale() const;
	void set_rnd_scale(const float val);

	float get_rnd_opacity() const;
	void set_rnd_opacity(const float val);

	bool get_variations() const;
	void set_variations(const bool val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	PoolColorArray tiler_calc(const Vector2 &uv, const Vector2 &tile, const int overlap, const Vector2 &_seed);

	MMColorTiler();
	~MMColorTiler();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> in_mask;
	Ref<MMNodeUniversalProperty> output;
	Ref<MMNodeUniversalProperty> instance_map;

	Vector2 tile = Vector2(4, 4);
	float overlap = 1;
	//export(int, "1,4,16")
	int select_inputs = 0;
	Vector2 scale = Vector2(0.5, 0.5);
	float fixed_offset = 0;
	float rnd_offset = 0.25;
	float rnd_rotate = 45;
	float rnd_scale = 0.2;
	float rnd_opacity = 0;
	bool variations = false;
};

#endif
