#ifndef MM_FBM_NOISE_H
#define MM_FBM_NOISE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMFbmNoise : public MMNode {
	GDCLASS(MMFbmNoise, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_type() const;
	void set_type(const int val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	int get_folds() const;
	void set_folds(const int val);

	int get_iterations() const;
	void set_iterations(const int val);

	float get_persistence() const;
	void set_persistence(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMFbmNoise();
	~MMFbmNoise();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6")
	int type;
	Vector2 scale;
	int folds;
	int iterations;
	float persistence;
};

#endif
