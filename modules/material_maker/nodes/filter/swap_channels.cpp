
#include "swap_channels.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSwapChannels::get_image() {
	return image;
}

void MMSwapChannels::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMSwapChannels::get_input() {
	return input;
}

void MMSwapChannels::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMSwapChannels::get_op_r() const {
	return op_r;
}

void MMSwapChannels::set_op_r(const int val) {
	op_r = val;
	set_dirty(true);
}

int MMSwapChannels::get_op_g() const {
	return op_g;
}

void MMSwapChannels::set_op_g(const int val) {
	op_g = val;
	set_dirty(true);
}

int MMSwapChannels::get_op_b() const {
	return op_b;
}

void MMSwapChannels::set_op_b(const int val) {
	op_b = val;
	set_dirty(true);
}

int MMSwapChannels::get_op_a() const {
	return op_a;
}

void MMSwapChannels::set_op_a(const int val) {
	op_a = val;
	set_dirty(true);
}

void MMSwapChannels::_init_properties() {
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

void MMSwapChannels::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("0");
	arr.push_back("1");
	arr.push_back("R");
	arr.push_back("-R");
	arr.push_back("G");
	arr.push_back("-G");
	arr.push_back("B");
	arr.push_back("-B");
	arr.push_back("A");
	arr.push_back("-A");

	mm_graph_node->add_slot_enum("get_op_r", "set_op_r", "R", arr);
	mm_graph_node->add_slot_enum("get_op_g", "set_op_g", "G", arr);
	mm_graph_node->add_slot_enum("get_op_b", "set_op_b", "B", arr);
	mm_graph_node->add_slot_enum("get_op_a", "set_op_a", "A", arr);
}

void MMSwapChannels::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

float MMSwapChannels::apply(const int op, const Color &val) {
	if (op == 0) {
		return 0.0;
	} else if (op == 1) {
		return 1.0;
	} else if (op == 2) {
		return val.r;
	} else if (op == 3) {
		return 1.0 - val.r;
	} else if (op == 4) {
		return val.g;
	} else if (op == 5) {
		return 1.0 - val.g;
	} else if (op == 6) {
		return val.b;
	} else if (op == 7) {
		return 1.0 - val.b;
	} else if (op == 8) {
		return val.a;
	} else if (op == 9) {
		return 1.0 - val.a;
	}

	return 0.0;
}

Color MMSwapChannels::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return Color(apply(op_r, c), apply(op_g, c), apply(op_b, c), apply(op_a, c));
}

MMSwapChannels::MMSwapChannels() {
	op_r = 2;
	op_g = 4;
	op_b = 6;
	op_a = 8;
}

MMSwapChannels::~MMSwapChannels() {
}

void MMSwapChannels::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMSwapChannels::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMSwapChannels::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMSwapChannels::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSwapChannels::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_op_r"), &MMSwapChannels::get_op_r);
	ClassDB::bind_method(D_METHOD("set_op_r", "value"), &MMSwapChannels::set_op_r);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "op_r"), "set_op_r", "get_op_r");

	ClassDB::bind_method(D_METHOD("get_op_g"), &MMSwapChannels::get_op_g);
	ClassDB::bind_method(D_METHOD("set_op_g", "value"), &MMSwapChannels::set_op_g);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "op_g"), "set_op_g", "get_op_g");

	ClassDB::bind_method(D_METHOD("get_op_b"), &MMSwapChannels::get_op_b);
	ClassDB::bind_method(D_METHOD("set_op_b", "value"), &MMSwapChannels::set_op_b);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "op_b"), "set_op_b", "get_op_b");

	ClassDB::bind_method(D_METHOD("get_op_a"), &MMSwapChannels::get_op_a);
	ClassDB::bind_method(D_METHOD("set_op_a", "value"), &MMSwapChannels::set_op_a);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "op_a"), "set_op_a", "get_op_a");

	ClassDB::bind_method(D_METHOD("apply", "op", "val"), &MMSwapChannels::apply);
}
