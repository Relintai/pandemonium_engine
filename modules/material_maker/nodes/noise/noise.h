#ifndef MM_NOISE_H
#define MM_NOISE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMNoise : public MMNode {
	GDCLASS(MMNoise, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_grid_size() const;
	void set_grid_size(const int val);

	float get_density() const;
	void set_density(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMNoise();
	~MMNoise();

protected:
	static void _bind_methods();

	//tool
	//export(MMNodeUniversalProperty)
	Ref<MMNodeUniversalProperty> image;
	//export(int)
	int grid_size = 16;
	//export(float)
	float density = 0.5;
};

#endif
