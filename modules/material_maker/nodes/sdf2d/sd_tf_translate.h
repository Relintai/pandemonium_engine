#ifndef MM_SD_TF_TRANSLATE_H
#define MM_SD_TF_TRANSLATE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdTfTranslate : public MMNode {
	GDCLASS(MMSdTfTranslate, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_translation();
	void set_translation(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdTfTranslate();
	~MMSdTfTranslate();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	Vector2 translation;
};

#endif
