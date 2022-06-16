
#include "rotate.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMRotate::get_image() {
	return image;
}

void MMRotate::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMRotate::get_input() {
	return input;
}

void MMRotate::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Vector2 MMRotate::get_center() {
	return center;
}

void MMRotate::set_center(const Vector2 &val) {
	center = val;
	set_dirty(true);
}

float MMRotate::get_rotate() const {
	return rotate;
}

void MMRotate::set_rotate(const float val) {
	rotate = val;
	set_dirty(true);
}

void MMRotate::_init_properties() {
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

void MMRotate::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_center", "set_center", "Center", 0.01);
	mm_graph_node->add_slot_float("get_rotate", "set_rotate", "MMRotate", 0.1);
}

void MMRotate::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMRotate::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i(rotate($uv, vec2(0.5+$cx, 0.5+$cy), $rotate*0.01745329251));
	return input->get_value(MMAlgos::rotate(uv, center + Vector2(0.5, 0.5), rotate * 0.01745329251));
}

MMRotate::MMRotate() {
	rotate = 0;
}

MMRotate::~MMRotate() {
}

void MMRotate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMRotate::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMRotate::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMRotate::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMRotate::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_center"), &MMRotate::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMRotate::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");

	ClassDB::bind_method(D_METHOD("get_rotate"), &MMRotate::get_rotate);
	ClassDB::bind_method(D_METHOD("set_rotate", "value"), &MMRotate::set_rotate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotate"), "set_rotate", "get_rotate");
}
