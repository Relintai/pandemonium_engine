#ifndef MM_OUTPUT_IMAGE_H
#define MM_OUTPUT_IMAGE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMOutputImage : public MMNode {
	GDCLASS(MMOutputImage, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	String get_postfix();
	void set_postfix(const String &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	void save();

	MMOutputImage();
	~MMOutputImage();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> image;
	String postfix;
	String _material_path;
};

#endif
