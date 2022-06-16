
#include "mirror.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMMirror::get_image() {
	return image;
}

void MMMirror::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMMirror::get_input() {
	return input;
}

void MMMirror::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMMirror::get_direction() const {
	return direction;
}

void MMMirror::set_direction(const int val) {
	direction = val;
	set_dirty(true);
}

float MMMirror::get_offset() const {
	return offset;
}

void MMMirror::set_offset(const float val) {
	offset = val;
	set_dirty(true);
}

void MMMirror::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

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

void MMMirror::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Horizontal");
	arr.push_back("Vertical");

	mm_graph_node->add_slot_enum("get_direction", "set_direction", "Direction", arr);
	mm_graph_node->add_slot_float("get_offset", "set_offset", "offset", 0.01);
}

void MMMirror::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMMirror::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i(uvmirror_$direction($uv, $offset));

	if (direction == 0) {
		return input->get_value(MMAlgos::uvmirror_h(uv, offset));
	}

	else if (direction == 1) {
		return input->get_value(MMAlgos::uvmirror_v(uv, offset));
	}

	return Color(0, 0, 0, 1);
}

MMMirror::MMMirror() {
	direction = 0;
	offset = 0;
}

MMMirror::~MMMirror() {
}

void MMMirror::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMMirror::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMMirror::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMMirror::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMMirror::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_direction"), &MMMirror::get_direction);
	ClassDB::bind_method(D_METHOD("set_direction", "value"), &MMMirror::set_direction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");

	ClassDB::bind_method(D_METHOD("get_offset"), &MMMirror::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "value"), &MMMirror::set_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");
}
