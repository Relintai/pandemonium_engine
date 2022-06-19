#ifndef MM_MATH_H
#define MM_MATH_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMMath : public MMNode {
	GDCLASS(MMMath, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_a();
	void set_a(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_b();
	void set_b(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_operation() const;
	void set_operation(const int val);

	bool get_clamp_result() const;
	void set_clamp_result(const bool val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMMath();
	~MMMath();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> a;
	Ref<MMNodeUniversalProperty> b;
	Ref<MMNodeUniversalProperty> output;
	//export(int, "A+B,A-B,A*B,A/B,log(A),log2(A),pow(A; B),abs(A),round(A),floor(A),ceil(A),trunc(A),fract(A),min(A; B),max(A; B),A<B,cos(A*B),sin(A*B),tan(A*B),sqrt(1-A*A)")
	int operation;
	bool clamp_result;
};

#endif
