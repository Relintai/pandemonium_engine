
#include "invert.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMInvert::get_image() {
	return image;
}

void MMInvert::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMInvert::get_input() {
	return input;
}

void MMInvert::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

void MMInvert::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;
	register_input_property(input);
	register_output_property(image);
}

void MMInvert::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
}

void MMInvert::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMInvert::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return MMAlgos::invert(c);
}

MMInvert::MMInvert() {
}

MMInvert::~MMInvert() {
}

void MMInvert::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMInvert::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMInvert::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMInvert::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMInvert::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");
}
