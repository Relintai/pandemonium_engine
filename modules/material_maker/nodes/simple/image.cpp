
#include "image.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"
#include "core/io/image_loader.h"
#include "core/io/resource_loader.h"

Ref<MMNodeUniversalProperty> MMImage::get_image() {
	return image;
}

void MMImage::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

String MMImage::get_image_path() {
	return image_path;
}

void MMImage::set_image_path(const String &val) {
	image_path = val;
	Ref<MMImage> img;
	img.instance();

	if (image_path != "") {
		ImageLoader::load_image(image_path, img);
	}

	image->set_value(img);
	set_dirty(true);
}

void MMImage::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_output_property(image);
}

void MMImage::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_image_path_universal(image, "get_image_path", "set_image_path");
}

//func _render(material) -> void:;
//	var img : MMImage = render_image(material);
//;
//	image.set_value(img);

Color MMImage::_get_value_for(const Vector2 &uv, const int pseed) {
	return image->get_value(uv);
}

MMImage::MMImage() {
}

MMImage::~MMImage() {
}

void MMImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMImage::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMImage::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_image_path"), &MMImage::get_image_path);
	ClassDB::bind_method(D_METHOD("set_image_path", "value"), &MMImage::set_image_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_path"), "set_image_path", "get_image_path");
}
