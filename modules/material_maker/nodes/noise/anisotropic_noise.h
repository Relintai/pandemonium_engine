#ifndef MM_ANISOTROPIC_NOISE_H
#define MM_ANISOTROPIC_NOISE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMAnisotropicNoise : public MMNode {
	GDCLASS(MMAnisotropicNoise, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	float get_smoothness() const;
	void set_smoothness(const float val);

	float get_interpolation() const;
	void set_interpolation(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMAnisotropicNoise();
	~MMAnisotropicNoise();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Vector2 scale;
	float smoothness;
	float interpolation;
};

#endif
