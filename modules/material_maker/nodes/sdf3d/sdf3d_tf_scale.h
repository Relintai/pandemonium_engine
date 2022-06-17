#ifndef MM_SDF3D_TF_SCALE_H
#define MM_SDF3D_TF_SCALE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dTfScale : public MMNode {
	GDCLASS(MMSdf3dTfScale, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	float get_scale() const;
	void set_scale(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	void on_input_changed();

	MMSdf3dTfScale();
	~MMSdf3dTfScale();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> output;
	float scale;
};

#endif
