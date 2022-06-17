#ifndef MM_SD_SHAPE_LINE_H
#define MM_SD_SHAPE_LINE_H

#include "../bases/curve_base.h"
#include "../mm_node_universal_property.h"

class MMSdShapeLine : public CurveBase {
	GDCLASS(MMSdShapeLine, CurveBase);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_a();
	void set_a(const Vector2 &val);

	Vector2 get_b();
	void set_b(const Vector2 &val);

	float get_width() const;
	void set_width(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	void _curve_changed();

	MMSdShapeLine();
	~MMSdShapeLine();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector2 A;
	Vector2 B;
	float width;
};

#endif
