#ifndef MM_BRIGHTNESS_CONTRAST_H
#define MM_BRIGHTNESS_CONTRAST_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMBrightnessContrast : public MMNode {
	GDCLASS(MMBrightnessContrast, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	float get_brightness() const;
	void set_brightness(const float val);

	float get_contrast() const;
	void set_contrast(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMBrightnessContrast();
	~MMBrightnessContrast();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	Ref<MMNodeUniversalProperty> input;
	float brightness;
	float contrast;
};

#endif
