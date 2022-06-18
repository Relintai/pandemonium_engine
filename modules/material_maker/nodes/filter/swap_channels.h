#ifndef MM_SWAP_CHANNELS_H
#define MM_SWAP_CHANNELS_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSwapChannels : public MMNode {
	GDCLASS(MMSwapChannels, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	int get_op_r() const;
	void set_op_r(const int val);

	int get_op_g() const;
	void set_op_g(const int val);

	int get_op_b() const;
	void set_op_b(const int val);

	int get_op_a() const;
	void set_op_a(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	float apply(const int op, const Color &val);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMSwapChannels();
	~MMSwapChannels();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	//export(int, "0,1,R,-R,G,-G,B,-B,A,-A")
	int op_r;
	//export(int, "0,1,R,-R,G,-G,B,-B,A,-A")
	int op_g;
	//export(int, "0,1,R,-R,G,-G,B,-B,A,-A")
	int op_b;
	//export(int, "0,1,R,-R,G,-G,B,-B,A,-A")
	int op_a;
};

#endif
