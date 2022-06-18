
#include "fill_to_random_grey.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToRandomGrey::get_image() {
	return image;
}

void MMFillToRandomGrey::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToRandomGrey::get_input() {
	return input;
}

void MMFillToRandomGrey::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMFillToRandomGrey::get_edge_color() const {
	return edge_color;
}

void MMFillToRandomGrey::set_edge_color(const float val) {
	edge_color = val;
	set_dirty(true);
}

void MMFillToRandomGrey::_init_properties() {
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

void MMFillToRandomGrey::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_edge_color", "set_edge_color", "Edge color", 0.01);
}

void MMFillToRandomGrey::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToRandomGrey::_get_value_for(const Vector2 &uv, const int pseed) {
	//vec4 $(name_uv)_bb = $in($uv);
	Color c = input->get_value(uv);
	//mix($edgecolor, rand(vec2(float($seed), rand(vec2(rand($(name_uv)_bb.xy), rand($(name_uv)_bb.zw))))), step(0.0000001, dot($(name_uv)_bb.zw, vec2(1.0))));
	float r1 = MMAlgos::rand(Vector2(c.r, c.g));
	float r2 = MMAlgos::rand(Vector2(c.b, c.a));
	float s = MMAlgos::step(0.0000001, Vector2(c.b, c.a).dot(Vector2(1, 1)));
	float f = Math::lerp(edge_color, MMAlgos::rand(Vector2(1.0 / float(pseed), MMAlgos::rand(Vector2(r1, r2)))), s);
	return Color(f, f, f, 1);
}

MMFillToRandomGrey::MMFillToRandomGrey() {
	edge_color = 1;
}

MMFillToRandomGrey::~MMFillToRandomGrey() {
}

void MMFillToRandomGrey::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToRandomGrey::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToRandomGrey::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToRandomGrey::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToRandomGrey::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_edge_color"), &MMFillToRandomGrey::get_edge_color);
	ClassDB::bind_method(D_METHOD("set_edge_color", "value"), &MMFillToRandomGrey::set_edge_color);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "edge_color"), "set_edge_color", "get_edge_color");
}
