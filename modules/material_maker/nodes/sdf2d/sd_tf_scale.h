#ifndef MM_SD_TF_SCALE_H
#define MM_SD_TF_SCALE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdTfScale : public MMNode {
	GDCLASS(MMSdTfScale, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_scale() const;
	void set_scale(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdTfScale();
	~MMSdTfScale();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	float scale;
};

#endif
