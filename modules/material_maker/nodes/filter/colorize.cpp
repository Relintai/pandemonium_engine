
#include "colorize.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMColorize::get_image() {
	return image;
}

void MMColorize::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMColorize::get_input() {
	return input;
}

void MMColorize::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

void MMColorize::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input->set_default_value(1);
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

void MMColorize::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_gradient();
}

void MMColorize::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMColorize::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = input->get_value(uv);
	return get_gradient_color(f);
}

//	return Color(0.5, 0.5, 0.5, 1);

Color MMColorize::_get_gradient_color(const float x) {
	if (interpolation_type == 0) {
		return MMAlgos::gradient_type_1(x, points);
	} else if (interpolation_type == 1) {
		return MMAlgos::gradient_type_2(x, points);
	} else if (interpolation_type == 2) {
		return MMAlgos::gradient_type_3(x, points);
	} else if (interpolation_type == 3) {
		return MMAlgos::gradient_type_4(x, points);
	}

	return Color(1, 1, 1, 1);
}

MMColorize::MMColorize() {
}

MMColorize::~MMColorize() {
}

void MMColorize::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMColorize::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMColorize::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMColorize::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMColorize::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");
}
