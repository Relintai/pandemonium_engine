#ifndef MM_TRANSFORM_H
#define MM_TRANSFORM_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTransform : public MMNode {
	GDCLASS(MMTransform, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_translate_x();
	void set_translate_x(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_translate_y();
	void set_translate_y(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_rotate();
	void set_rotate(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_scale_x();
	void set_scale_x(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_scale_y();
	void set_scale_y(const Ref<MMNodeUniversalProperty> &val);

	int get_mode() const;
	void set_mode(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTransform();
	~MMTransform();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> translate_x;
	Ref<MMNodeUniversalProperty> translate_y;
	Ref<MMNodeUniversalProperty> rotate;
	Ref<MMNodeUniversalProperty> scale_x;
	Ref<MMNodeUniversalProperty> scale_y;
	//export(int, "Clamp,Repeat,Extend")
	int mode;
};

#endif
