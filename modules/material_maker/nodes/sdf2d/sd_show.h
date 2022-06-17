#ifndef MM_SD_SHOW_H
#define MM_SD_SHOW_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class SSSdShow : public MMNode {
	GDCLASS(SSSdShow, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_bevel() const;
	void set_bevel(const float val);

	float get_base() const;
	void set_base(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	SSSdShow();
	~SSSdShow();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float bevel;
	float base;
};

#endif
