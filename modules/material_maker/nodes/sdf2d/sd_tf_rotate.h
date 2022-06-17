#ifndef SD_TF_ROTATE_H
#define SD_TF_ROTATE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdTfRotate : public MMNode {
	GDCLASS(MMSdTfRotate, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_angle() const;
	void set_angle(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdTfRotate();
	~MMSdTfRotate();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	float angle;
};

#endif
