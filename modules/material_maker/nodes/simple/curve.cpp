
#include "curve.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMCurve::get_image() {
	return image;
}

void MMCurve::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMCurve::get_input() {
	return input;
}

void MMCurve::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Vector2 MMCurve::get_a() {
	return a;
}

void MMCurve::set_a(const Vector2 &val) {
	a = val;
	set_dirty(true);
}

Vector2 MMCurve::get_b() {
	return b;
}

void MMCurve::set_b(const Vector2 &val) {
	b = val;
	set_dirty(true);
}

Vector2 MMCurve::get_c() {
	return c;
}

void MMCurve::set_c(const Vector2 &val) {
	c = val;
	set_dirty(true);
}

float MMCurve::get_width() const {
	return width;
}

void MMCurve::set_width(const float val) {
	width = val;
	set_dirty(true);
}

int MMCurve::get_repeat() const {
	return repeat;
}

void MMCurve::set_repeat(const int val) {
	repeat = val;
	set_dirty(true);
}

void MMCurve::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input->set_default_value(1.0);
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	register_input_property(input);
	register_output_property(image);
}

void MMCurve::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_vector2("get_a", "set_a", "A", 0.01);
	mm_graph_node->add_slot_vector2("get_b", "set_b", "B", 0.01);
	mm_graph_node->add_slot_vector2("get_c", "set_c", "C", 0.01);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.01);
	mm_graph_node->add_slot_int("get_repeat", "set_repeat", "Repeat");
}

Color MMCurve::_get_value_for(const Vector2 &uv, const int pseed) {
	Vector2 nuv = transform_uv(uv);
	float f = 0;

	if (nuv.x != 0 && nuv.y != 0) {
		f = input->get_value(nuv);
	}

	return Color(f, f, f, 1);
}

void MMCurve::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Vector2 MMCurve::transform_uv(const Vector2 &uv) {
	//vec2 $(name_uv)_bezier = sdBezier($uv, vec2($ax+0.5, $ay+0.5), vec2($bx+0.5, $by+0.5), vec2($cx+0.5, $cy+0.5));
	Vector2 bezier = MMAlgos::sdBezier(uv, Vector2(a.x + 0.5, a.y + 0.5), Vector2(b.x + 0.5, b.y + 0.5), Vector2(c.x + 0.5, c.y + 0.5));
	//vec2 $(name_uv)_uv = vec2($(name_uv)_bezier.x, $(name_uv)_bezier.y / $width+0.5);
	Vector2 new_uv = Vector2(bezier.x, bezier.y / width + 0.5);
	//vec2 $(name_uv)_uvtest = step(vec2(0.5), abs($(name_uv)_uv-vec2(0.5)));
	Vector2 uv_test = MMAlgos::stepv2(Vector2(0.5, 0.5), MMAlgos::absv2(new_uv - Vector2(0.5, 0.5)));
	//$(name_uv)_uv = mix(vec2(fract($repeat*$(name_uv)_uv.x), $(name_uv)_uv.y), vec2(0.0), max($(name_uv)_uvtest.x, $(name_uv)_uvtest.y));
	Vector2 final_uv = Vector2(MMAlgos::fract(repeat * new_uv.x), new_uv.y).linear_interpolate(Vector2(), MAX(uv_test.x, uv_test.y));

	return final_uv;
}

MMCurve::MMCurve() {
	a = Vector2(-0.35, -0.2);
	b = Vector2(0, 0.5);
	c = Vector2(0.35, -0.2);
	width = 0.05;
	repeat = 1;
}

MMCurve::~MMCurve() {
}

void MMCurve::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMCurve::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMCurve::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMCurve::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMCurve::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_a"), &MMCurve::get_a);
	ClassDB::bind_method(D_METHOD("set_a", "value"), &MMCurve::set_a);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "a"), "set_a", "get_a");

	ClassDB::bind_method(D_METHOD("get_b"), &MMCurve::get_b);
	ClassDB::bind_method(D_METHOD("set_b", "value"), &MMCurve::set_b);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "b"), "set_b", "get_b");

	ClassDB::bind_method(D_METHOD("get_c"), &MMCurve::get_c);
	ClassDB::bind_method(D_METHOD("set_c", "value"), &MMCurve::set_c);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "c"), "set_c", "get_c");

	ClassDB::bind_method(D_METHOD("get_width"), &MMCurve::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMCurve::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_repeat"), &MMCurve::get_repeat);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &MMCurve::set_repeat);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");

	ClassDB::bind_method(D_METHOD("transform_uv", "uv"), &MMCurve::transform_uv);
}
