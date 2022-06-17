#ifndef MM_IMAGE_H
#define MM_IMAGE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMImage : public MMNode {
	GDCLASS(MMImage, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	String get_image_path();
	void set_image_path(const String &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMImage();
	~MMImage();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	String image_path;
};

#endif
