#ifndef MM_SDF3D_OP_REPEAT_H
#define MM_SDF3D_OP_REPEAT_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dOpRepeat : public MMNode {
	GDCLASS(MMSdf3dOpRepeat, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_col_row();
	void set_col_row(const Vector2 &val);

	float get_rotation() const;
	void set_rotation(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	void on_input_changed();

	MMSdf3dOpRepeat();
	~MMSdf3dOpRepeat();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> output;
	Vector2 col_row;
	float rotation;
};

#endif
