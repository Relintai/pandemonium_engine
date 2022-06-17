#ifndef MM_CURVE_H
#define MM_CURVE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMCurve : public MMNode {
	GDCLASS(MMCurve, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_a();
	void set_a(const Vector2 &val);

	Vector2 get_b();
	void set_b(const Vector2 &val);

	Vector2 get_c();
	void set_c(const Vector2 &val);

	float get_width() const;
	void set_width(const float val);

	int get_repeat() const;
	void set_repeat(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);
	Vector2 transform_uv(const Vector2 &uv);

	MMCurve();
	~MMCurve();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Vector2 a;
	Vector2 b;
	Vector2 c;
	float width;
	int repeat;
};

#endif
