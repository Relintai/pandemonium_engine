#ifndef MM_SD_OP_REPEAT_H
#define MM_SD_OP_REPEAT_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdOpRepeat : public MMNode {
	GDCLASS(MMSdOpRepeat, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_x() const;
	void set_x(const int val);

	int get_y() const;
	void set_y(const int val);

	float get_random_rotation() const;
	void set_random_rotation(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Variant _get_property_value(const Vector2 &uv);

	MMSdOpRepeat();
	~MMSdOpRepeat();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	int x;
	int y;
	float random_rotation;
};

#endif
