
#include "shear.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMShear::get_image() {
	return image;
}

void MMShear::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMShear::get_input() {
	return input;
}

void MMShear::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMShear::get_direction() const {
	return direction;
}

void MMShear::set_direction(const int val) {
	direction = val;
	set_dirty(true);
}

float MMShear::get_amount() const {
	return amount;
}

void MMShear::set_amount(const float val) {
	amount = val;
	set_dirty(true);
}

float MMShear::get_center() const {
	return center;
}

void MMShear::set_center(const float val) {
	center = val;
	set_dirty(true);
}

void MMShear::_init_properties() {
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

void MMShear::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Horizontal");
	arr.push_back("Vertical");

	mm_graph_node->add_slot_enum("get_direction", "set_direction", "Direction", arr);
	mm_graph_node->add_slot_float("get_amount", "set_amount", "Amount", 0.01);
	mm_graph_node->add_slot_float("get_center", "set_center", "Center", 0.01);
}

void MMShear::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMShear::_get_value_for(const Vector2 &uv, const int pseed) {
	//$in($uv+$amount*($uv.yx-vec2($center))*vec2($direction));

	if (direction == 0) {
		return input->get_value(uv + amount * (Vector2(uv.y, uv.x) - Vector2(center, center)) * Vector2(1, 0));
	} else if (direction == 1) {
		return input->get_value(uv + amount * (Vector2(uv.y, uv.x) - Vector2(center, center)) * Vector2(0, 1));
	}

	return Color(0, 0, 0, 1);
}

MMShear::MMShear() {
	direction = 0;
	amount = 1;
	center = 0;
}

MMShear::~MMShear() {
}

void MMShear::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMShear::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMShear::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMShear::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMShear::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_direction"), &MMShear::get_direction);
	ClassDB::bind_method(D_METHOD("set_direction", "value"), &MMShear::set_direction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");

	ClassDB::bind_method(D_METHOD("get_amount"), &MMShear::get_amount);
	ClassDB::bind_method(D_METHOD("set_amount", "value"), &MMShear::set_amount);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");

	ClassDB::bind_method(D_METHOD("get_center"), &MMShear::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMShear::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "center"), "set_center", "get_center");
}
