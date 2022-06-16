
#include "circle_map.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMCircleMap::get_image() {
	return image;
}

void MMCircleMap::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMCircleMap::get_input() {
	return input;
}

void MMCircleMap::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMCircleMap::get_radius() const {
	return radius;
}

void MMCircleMap::set_radius(const float val) {
	radius = val;

	if (radius == 0) {
		radius = 0.000000001;
	}

	set_dirty(true);
}

int MMCircleMap::get_repeat() const {
	return repeat;
}

void MMCircleMap::set_repeat(const int val) {
	repeat = val;
	set_dirty(true);
}

void MMCircleMap::_init_properties() {
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

void MMCircleMap::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
	mm_graph_node->add_slot_int("get_repeat", "set_repeat", "Repeat");
}

void MMCircleMap::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMCircleMap::_get_value_for(const Vector2 &uv, const int pseed) {
	//$in(vec2(fract($repeat*atan($uv.y-0.5, $uv.x-0.5)*0.15915494309), min(0.99999, 2.0/$radius*length($uv-vec2(0.5)))))",;
	Vector2 nuv = Vector2(MMAlgos::fractf(repeat * Math::atan2(uv.y - 0.5, uv.x - 0.5) * 0.15915494309), MIN(0.99999, 2.0 / radius * (uv - Vector2(0.5, 0.5)).length()));
	return input->get_value(nuv);
}

MMCircleMap::MMCircleMap() {
	radius = 1;
	repeat = 1;
}

MMCircleMap::~MMCircleMap() {
}

void MMCircleMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMCircleMap::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMCircleMap::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMCircleMap::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMCircleMap::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMCircleMap::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMCircleMap::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");

	ClassDB::bind_method(D_METHOD("get_repeat"), &MMCircleMap::get_repeat);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &MMCircleMap::set_repeat);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");
}
