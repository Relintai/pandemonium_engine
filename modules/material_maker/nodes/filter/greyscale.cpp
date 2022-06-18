
#include "greyscale.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMGreyscale::get_image() {
	return image;
}

void MMGreyscale::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMGreyscale::get_input() {
	return input;
}

void MMGreyscale::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMGreyscale::get_type() const {
	return type;
}

void MMGreyscale::set_type(const int val) {
	type = val;
	set_dirty(true);
}

void MMGreyscale::_init_properties() {
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

void MMGreyscale::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Lightness");
	arr.push_back("Average");
	arr.push_back("Luminosity");
	arr.push_back("Min");
	arr.push_back("Max");

	mm_graph_node->add_slot_enum("get_type", "set_type", "Type", arr);
}

void MMGreyscale::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMGreyscale::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	float f = 0;

	if (type == 0) {
		f = MMAlgos::grayscale_lightness(Vector3(c.r, c.g, c.b));
	} else if (type == 1) {
		f = MMAlgos::grayscale_average(Vector3(c.r, c.g, c.b));
	} else if (type == 2) {
		f = MMAlgos::grayscale_luminosity(Vector3(c.r, c.g, c.b));
	} else if (type == 3) {
		f = MMAlgos::grayscale_min(Vector3(c.r, c.g, c.b));
	} else if (type == 4) {
		f = MMAlgos::grayscale_max(Vector3(c.r, c.g, c.b));
	}

	return Color(f, f, f, c.a);
}

MMGreyscale::MMGreyscale() {
	type = 2;
}

MMGreyscale::~MMGreyscale() {
}

void MMGreyscale::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMGreyscale::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMGreyscale::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMGreyscale::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMGreyscale::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_type"), &MMGreyscale::get_type);
	ClassDB::bind_method(D_METHOD("set_type", "value"), &MMGreyscale::set_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
}
