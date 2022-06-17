#ifndef MM_PATTERN_H
#define MM_PATTERN_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMPattern : public MMNode {
	GDCLASS(MMPattern, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_combiner_type() const;
	void set_combiner_type(const int val);

	int get_combiner_axis_type_x() const;
	void set_combiner_axis_type_x(const int val);

	int get_combiner_axis_type_y() const;
	void set_combiner_axis_type_y(const int val);

	Vector2 get_repeat();
	void set_repeat(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMPattern();
	~MMPattern();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Multiply,Add,Max,Min,Xor,Pow")
	int combiner_type = 0;
	//export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce")
	int combiner_axis_type_x = 0;
	//export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce")
	int combiner_axis_type_y = 0;
	//export(Vector2)
	Vector2 repeat = Vector2(4, 4);
};

#endif
