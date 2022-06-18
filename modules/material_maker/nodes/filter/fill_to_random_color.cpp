
#include "fill_to_random_color.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToRandomColor::get_image() {
	return image;
}

void MMFillToRandomColor::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToRandomColor::get_input() {
	return input;
}

void MMFillToRandomColor::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Color MMFillToRandomColor::get_edge_color() {
	return edge_color;
}

void MMFillToRandomColor::set_edge_color(const Color &val) {
	edge_color = val;
	set_dirty(true);
}

void MMFillToRandomColor::_init_properties() {
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

void MMFillToRandomColor::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_color("get_edge_color", "set_edge_color");
}

void MMFillToRandomColor::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToRandomColor::_get_value_for(const Vector2 &uv, const int pseed) {
	//vec4 $(name_uv)_bb = $in($uv);
	Color c = input->get_value(uv);
	//mix($edgecolor.rgb, rand3(vec2(float($seed), rand(vec2(rand($(name_uv)_bb.xy), rand($(name_uv)_bb.zw))))), step(0.0000001, dot($(name_uv)_bb.zw, vec2(1.0))));
	float r1 = MMAlgos::rand(Vector2(c.r, c.g));
	float r2 = MMAlgos::rand(Vector2(c.b, c.a));
	float s = MMAlgos::step(0.0000001, Vector2(c.b, c.a).dot(Vector2(1, 1)));
	Vector3 f = Vector3(edge_color.r, edge_color.g, edge_color.b).linear_interpolate(MMAlgos::rand3(Vector2(1.0 / float(pseed), MMAlgos::rand(Vector2(r1, r2)))), s);
	return Color(f.x, f.y, f.z, 1);
}

MMFillToRandomColor::MMFillToRandomColor() {
	edge_color = Color(1, 1, 1, 1);
}

MMFillToRandomColor::~MMFillToRandomColor() {
}

void MMFillToRandomColor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToRandomColor::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToRandomColor::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToRandomColor::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToRandomColor::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_edge_color"), &MMFillToRandomColor::get_edge_color);
	ClassDB::bind_method(D_METHOD("set_edge_color", "value"), &MMFillToRandomColor::set_edge_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "edge_color"), "set_edge_color", "get_edge_color");
}
