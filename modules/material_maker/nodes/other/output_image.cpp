
#include "output_image.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMOutputImage::get_image() {
	return image;
}

void MMOutputImage::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

String MMOutputImage::get_postfix() {
	return postfix;
}

void MMOutputImage::set_postfix(const String &val) {
	postfix = val;
	set_dirty(true);
}

void MMOutputImage::_init_properties() {
	image.instance();
	image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	image->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	image->set_slot_name("image");
	register_input_property(image);
}

void MMOutputImage::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_line_edit("get_postfix", "set_postfix", "postfix");
}

void MMOutputImage::_render(const Ref<MMMaterial> &material) {
	if (!image.is_valid()) {
		return;
	}

	Ref<Image> img = image->get_active_image();

	if (!img.is_valid()) {
		return;
	}

	String matpath = material->get_path();

	if (matpath == "") {
		return;
	}

	String matbn = matpath.get_basename();
	String final_file_name = matbn + postfix + ".png";
	img->save_png(final_file_name);
}

MMOutputImage::MMOutputImage() {
}

MMOutputImage::~MMOutputImage() {
}

void MMOutputImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMOutputImage::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMOutputImage::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_postfix"), &MMOutputImage::get_postfix);
	ClassDB::bind_method(D_METHOD("set_postfix", "value"), &MMOutputImage::set_postfix);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "postfix"), "set_postfix", "get_postfix");
}
