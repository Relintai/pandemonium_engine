
#include "fill_channel.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillChannel::get_image() {
	return image;
}

void MMFillChannel::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillChannel::get_input() {
	return input;
}

void MMFillChannel::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMFillChannel::get_value() {
	return value;
}

void MMFillChannel::set_value(const Ref<MMNodeUniversalProperty> &val) {
	value = val;
}

int MMFillChannel::get_channel() const {
	return channel;
}

void MMFillChannel::set_channel(const int val) {
	channel = val;
	set_dirty(true);
}

void MMFillChannel::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color());
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

	if (!value.is_valid()) {
		value.instance();
		value->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		value->set_default_value(1);
	}

	value->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	value->set_value_step(0.01);
	value->set_value_range(Vector2(0, 1));

	register_input_property(input);
	register_output_property(image);
	register_input_property(value);
}

void MMFillChannel::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float_universal(value);

	Array arr;
	arr.push_back("R");
	arr.push_back("G");
	arr.push_back("B");
	arr.push_back("A");

	mm_graph_node->add_slot_enum("get_channel", "set_channel", "Channel", arr);
}

void MMFillChannel::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillChannel::_get_value_for(const Vector2 &uv, const int pseed) {
	Color col = input->get_value(uv);

	if (channel == 0) {
		col.r = value->get_value(uv);
	}

	if (channel == 1) {
		col.g = value->get_value(uv);
	}

	if (channel == 2) {
		col.b = value->get_value(uv);
	}

	if (channel == 3) {
		col.a = value->get_value(uv);
	}

	return col;
}

MMFillChannel::MMFillChannel() {
	channel = 3;
}

MMFillChannel::~MMFillChannel() {
}

void MMFillChannel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillChannel::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillChannel::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillChannel::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillChannel::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_value"), &MMFillChannel::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &MMFillChannel::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_channel"), &MMFillChannel::get_channel);
	ClassDB::bind_method(D_METHOD("set_channel", "value"), &MMFillChannel::set_channel);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel"), "set_channel", "get_channel");
}
