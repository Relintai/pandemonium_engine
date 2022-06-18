
#include "make_tileable.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMMakeTileable::get_image() {
	return image;
}

void MMMakeTileable::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMMakeTileable::get_input() {
	return input;
}

void MMMakeTileable::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMMakeTileable::get_width() const {
	return width;
}

void MMMakeTileable::set_width(const float val) {
	width = val;
	set_dirty(true);
}

int MMMakeTileable::get_size() const {
	return size;
}

void MMMakeTileable::set_size(const int val) {
	size = val;
	set_dirty(true);
}

void MMMakeTileable::_init_properties() {
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
	register_input_property(input);
	register_output_property(image);
}

void MMMakeTileable::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.01);
}

void MMMakeTileable::_render(const Ref<MMMaterial> &material) {
	size = MAX(material->image_size.x, material->image_size.y);
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMMakeTileable::_get_value_for(const Vector2 &uv, const int pseed) {
	//make_tileable_$(name)($uv, 0.5*$w);
	return make_tileable(uv, 0.5 * width);
}

//----------------------;
//make_tileable.mmg;
//vec4 make_tileable_$(name)(vec2 uv, float w) {;
//	vec4 a = $in(uv);
//	vec4 b = $in(fract(uv+vec2(0.5)));
//	float coef_ab = sin(1.57079632679*clamp((length(uv-vec2(0.5))-0.5+w)/w, 0.0, 1.0));
//	vec4 c = $in(fract(uv+vec2(0.25)));
//	float coef_abc = sin(1.57079632679*clamp((min(min(length(uv-vec2(0.0, 0.5)), length(uv-vec2(0.5, 0.0))), min(length(uv-vec2(1.0, 0.5)), length(uv-vec2(0.5, 1.0))))-w)/w, 0.0, 1.0));
//	return mix(c, mix(a, b, coef_ab), coef_abc);
//};

Color MMMakeTileable::make_tileable(const Vector2 &uv, const float w) {
	Color a = input->get_value(uv);
	Color b = input->get_value(MMAlgos::fractv2(uv + Vector2(0.5, 0.5)));
	float coef_ab = Math::sin(1.57079632679 * CLAMP(((uv - Vector2(0.5, 0.5)).length() - 0.5 + w) / w, 0.0, 1.0));
	Color c = input->get_value(MMAlgos::fractv2(uv + Vector2(0.25, 0.25)));
	float coef_abc = sin(1.57079632679 * CLAMP((MIN(MIN((uv - Vector2(0.0, 0.5)).length(), (uv - Vector2(0.5, 0.0)).length()), MIN((uv - Vector2(1.0, 0.5)).length(), (uv - Vector2(0.5, 1.0)).length())) - w) / w, 0.0, 1.0));
	return c.linear_interpolate(a.linear_interpolate(b, coef_ab), coef_abc);
}

MMMakeTileable::MMMakeTileable() {
	width = 0.1;
	size = 0;
}

MMMakeTileable::~MMMakeTileable() {
}

void MMMakeTileable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMMakeTileable::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMMakeTileable::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMMakeTileable::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMMakeTileable::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_width"), &MMMakeTileable::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMMakeTileable::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_size"), &MMMakeTileable::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMMakeTileable::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("make_tileable", "uv", "w"), &MMMakeTileable::make_tileable);
}
