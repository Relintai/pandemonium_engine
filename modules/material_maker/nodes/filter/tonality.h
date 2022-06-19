#ifndef MM_TONALITY_H
#define MM_TONALITY_H

#include "core/math/vector2.h"

#include "../bases/curve_base.h"
#include "../mm_node_universal_property.h"

class MMTonality : public CurveBase {
	GDCLASS(MMTonality, CurveBase);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _curve_changed();

	MMTonality();
	~MMTonality();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
};

#endif
