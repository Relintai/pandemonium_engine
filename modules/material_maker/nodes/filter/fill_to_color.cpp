
#include "fill_to_color.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToColor::get_image() {
	return image;
}

void MMFillToColor::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToColor::get_input() {
	return input;
}

void MMFillToColor::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMFillToColor::get_color_map() {
	return color_map;
}

void MMFillToColor::set_color_map(const Ref<MMNodeUniversalProperty> &val) {
	color_map = val;
}

Color MMFillToColor::get_edge_color() {
	return edge_color;
}

void MMFillToColor::set_edge_color(const Color &val) {
	edge_color = val;
	set_dirty(true);
}

void MMFillToColor::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	if (!color_map.is_valid()) {
		color_map.instance();
		color_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		color_map->set_default_value(Color(1, 1, 1, 1));
	}

	color_map->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	color_map->set_slot_name(">>>  Color Map  ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;

	register_input_property(input);
	register_input_property(color_map);
	register_output_property(image);
}

void MMFillToColor::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(color_map);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_color("get_edge_color", "set_edge_color");
}

void MMFillToColor::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToColor::_get_value_for(const Vector2 &uv, const int pseed) {
	//vec4 $(name_uv)_bb = $in($uv);
	Color c = input->get_value(uv);
	//mix($edgecolor, $map(fract($(name_uv)_bb.xy+0.5*$(name_uv)_bb.zw)), step(0.0000001, dot($(name_uv)_bb.zw, vec2(1.0))));
	Color rc = color_map->get_value(MMAlgos::fractv2(Vector2(c.r, c.g) + 0.5 * Vector2(c.b, c.a)));
	float s = MMAlgos::step(0.0000001, Vector2(c.b, c.a).dot(Vector2(1, 1)));
	return edge_color.linear_interpolate(rc, s);
}

MMFillToColor::MMFillToColor() {
	edge_color = Color(1, 1, 1, 1);
}

MMFillToColor::~MMFillToColor() {
}

void MMFillToColor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToColor::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToColor::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToColor::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToColor::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_color_map"), &MMFillToColor::get_color_map);
	ClassDB::bind_method(D_METHOD("set_color_map", "value"), &MMFillToColor::set_color_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_map", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_color_map", "get_color_map");

	ClassDB::bind_method(D_METHOD("get_edge_color"), &MMFillToColor::get_edge_color);
	ClassDB::bind_method(D_METHOD("set_edge_color", "value"), &MMFillToColor::set_edge_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "edge_color"), "set_edge_color", "get_edge_color");
}
