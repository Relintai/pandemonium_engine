#ifndef MM_OUTPUT_IMAGE_H
#define MM_OUTPUT_IMAGE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMOutputImage : public MMNode {
	GDCLASS(MMOutputImage, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	String get_postfix();
	void set_postfix(const String &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	MMOutputImage();
	~MMOutputImage();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	String postfix;
};

#endif
