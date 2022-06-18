
#include "brightness_contrast.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMBrightnessContrast::get_image() {
	return image;
}

void MMBrightnessContrast::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMBrightnessContrast::get_input() {
	return input;
}

void MMBrightnessContrast::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMBrightnessContrast::get_brightness() const {
	return brightness;
}

void MMBrightnessContrast::set_brightness(const float val) {
	brightness = val;
	set_dirty(true);
}

float MMBrightnessContrast::get_contrast() const {
	return contrast;
}

void MMBrightnessContrast::set_contrast(const float val) {
	contrast = val;
	set_dirty(true);
}

void MMBrightnessContrast::_init_properties() {
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

void MMBrightnessContrast::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_brightness", "set_brightness", "Brightness", 0.01);
	mm_graph_node->add_slot_float("get_contrast", "set_contrast", "Contrast", 0.01);
}

void MMBrightnessContrast::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMBrightnessContrast::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return MMAlgos::brightness_contrast(c, brightness, contrast);
}

MMBrightnessContrast::MMBrightnessContrast() {
	brightness = 0;
	contrast = 1;
}

MMBrightnessContrast::~MMBrightnessContrast() {
}

void MMBrightnessContrast::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMBrightnessContrast::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMBrightnessContrast::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMBrightnessContrast::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMBrightnessContrast::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_brightness"), &MMBrightnessContrast::get_brightness);
	ClassDB::bind_method(D_METHOD("set_brightness", "value"), &MMBrightnessContrast::set_brightness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "brightness"), "set_brightness", "get_brightness");

	ClassDB::bind_method(D_METHOD("get_contrast"), &MMBrightnessContrast::get_contrast);
	ClassDB::bind_method(D_METHOD("set_contrast", "value"), &MMBrightnessContrast::set_contrast);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "contrast"), "set_contrast", "get_contrast");
}
