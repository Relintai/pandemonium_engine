#ifndef MM_SCRATCHES_H
#define MM_SCRATCHES_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMScratches : public MMNode {
	GDCLASS(MMScratches, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	int get_layers() const;
	void set_layers(const int val);

	float get_waviness() const;
	void set_waviness(const float val);

	int get_angle() const;
	void set_angle(const int val);

	float get_randomness() const;
	void set_randomness(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMScratches();
	~MMScratches();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Vector2 size;
	int layers;
	float waviness;
	int angle;
	float randomness;
};

#endif
