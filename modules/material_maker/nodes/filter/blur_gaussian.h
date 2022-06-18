#ifndef MM_BLUR_GAUSSIAN_H
#define MM_BLUR_GAUSSIAN_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMBlurGaussian : public MMNode {
	GDCLASS(MMBlurGaussian, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_sigma();
	void set_sigma(const Ref<MMNodeUniversalProperty> &val);

	int get_direction() const;
	void set_direction(const int val);

	int get_size() const;
	void set_size(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Ref<Image> _render_image(const Ref<MMMaterial> &material);

	Color get_value_x(const Vector2 &uv, const int pseed);
	Color get_value_y(const Vector2 &uv, const int pseed);
	Color get_value_y_img(const Vector2 &uv, const int pseed);

	Color gaussian_blur_x(const Vector2 &uv, const float psize, const float psigma, const float pamount);
	Color gaussian_blur_y(const Vector2 &uv, const float psize, const float psigma, const float pamount);
	Color gaussian_blur_y_img(const Vector2 &uv, const float psize, const float psigma, const float pamount);

	MMBlurGaussian();
	~MMBlurGaussian();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> sigma;
	//export(int, "Both,X,Y")
	int direction;
	int size;
};

#endif
