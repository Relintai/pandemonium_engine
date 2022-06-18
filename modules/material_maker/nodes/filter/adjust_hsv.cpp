
#include "adjust_hsv.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMAdjustHsv::get_image() {
	return image;
}

void MMAdjustHsv::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMAdjustHsv::get_input() {
	return input;
}

void MMAdjustHsv::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMAdjustHsv::get_hue() const {
	return hue;
}

void MMAdjustHsv::set_hue(const float val) {
	hue = val;
	set_dirty(true);
}

float MMAdjustHsv::get_saturation() const {
	return saturation;
}

void MMAdjustHsv::set_saturation(const float val) {
	saturation = val;
	set_dirty(true);
}

float MMAdjustHsv::get_value() const {
	return value;
}

void MMAdjustHsv::set_value(const float val) {
	value = val;
	set_dirty(true);
}

void MMAdjustHsv::_init_properties() {
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

void MMAdjustHsv::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_hue", "set_hue", "Hue", 0.01);
	mm_graph_node->add_slot_float("get_saturation", "set_saturation", "Saturation", 0.01);
	mm_graph_node->add_slot_float("get_value", "set_value", "Value", 0.01);
}

void MMAdjustHsv::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMAdjustHsv::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return MMAlgos::adjust_hsv(c, hue, saturation, value);
}

MMAdjustHsv::MMAdjustHsv() {
	hue = 0;
	saturation = 1;
	value = 1;
}

MMAdjustHsv::~MMAdjustHsv() {
}

void MMAdjustHsv::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMAdjustHsv::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMAdjustHsv::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMAdjustHsv::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMAdjustHsv::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_hue"), &MMAdjustHsv::get_hue);
	ClassDB::bind_method(D_METHOD("set_hue", "value"), &MMAdjustHsv::set_hue);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "hue"), "set_hue", "get_hue");

	ClassDB::bind_method(D_METHOD("get_saturation"), &MMAdjustHsv::get_saturation);
	ClassDB::bind_method(D_METHOD("set_saturation", "value"), &MMAdjustHsv::set_saturation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "saturation"), "set_saturation", "get_saturation");

	ClassDB::bind_method(D_METHOD("get_value"), &MMAdjustHsv::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &MMAdjustHsv::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "value"), "set_value", "get_value");
}
