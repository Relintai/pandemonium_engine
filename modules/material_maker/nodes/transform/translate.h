#ifndef MM_TRANSLATE_H
#define MM_TRANSLATE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMTranslate : public MMNode {
	GDCLASS(MMTranslate, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_translation();
	void set_translation(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMTranslate();
	~MMTranslate();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Vector2 translation;
};

#endif
