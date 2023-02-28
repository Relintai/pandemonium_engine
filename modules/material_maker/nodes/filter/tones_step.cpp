
#include "tones_step.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTonesStep::get_image() {
	return image;
}

void MMTonesStep::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMTonesStep::get_input() {
	return input;
}

void MMTonesStep::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMTonesStep::get_value() const {
	return _value;
}
void MMTonesStep::set_value(const float val) {
	_value = val;
	set_dirty(true);
}

float MMTonesStep::get_width() const {
	return _width;
}
void MMTonesStep::set_width(const float val) {
	_width = val;
	set_dirty(true);
}

bool MMTonesStep::get_invert() const {
	return _invert;
}
void MMTonesStep::set_invert(const bool val) {
	_invert = val;
	set_dirty(true);
}

void MMTonesStep::_init_properties() {
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

void MMTonesStep::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_value", "set_value", "Value", 0.001, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.001, Vector2(0, 1));
	mm_graph_node->add_slot_bool("get_invert", "set_invert", "Invert");
}

void MMTonesStep::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

// tones_step.mmg
//"vec3 $(name_uv)_false = clamp(($in($uv).rgb-vec3($value))/max(0.0001, $width)+vec3(0.5), vec3(0.0), vec3(1.0));",
//"vec3 $(name_uv)_true = vec3(1.0)-$(name_uv)_false;"
//"rgba": "vec4($(name_uv)_$invert, $in($uv).a)",

Color MMTonesStep::_get_value_for(const Vector2 &uv, const int pseed) {
	Color input_color = input->get_value(uv);
	Vector3 input_value = Vector3(input_color.r, input_color.g, input_color.b);
	Vector3 val_v3 = Vector3(_value, _value, _value);

	Vector3 value_false = (input_value - val_v3);

	float mv = MAX(0.0001, _width);
	Vector3 mvv3 = Vector3(mv, mv, mv);

	value_false /= mvv3;
	value_false += Vector3(0.5, 0.5, 0.5);
	value_false.clamp(Vector3(), Vector3(1, 1, 1));

	if (_invert) {
		value_false = Vector3(1, 1, 1) - value_false;
	}

	return Color(value_false.x, value_false.y, value_false.z, input_color.a);
}

MMTonesStep::MMTonesStep() {
	_value = 0.5;
	_width = 0.1;
	_invert = false;
}

MMTonesStep::~MMTonesStep() {
}

void MMTonesStep::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTonesStep::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTonesStep::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMTonesStep::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTonesStep::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_value"), &MMTonesStep::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &MMTonesStep::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "value"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_width"), &MMTonesStep::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMTonesStep::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_invert"), &MMTonesStep::get_invert);
	ClassDB::bind_method(D_METHOD("set_invert", "value"), &MMTonesStep::set_invert);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "invert"), "set_invert", "get_invert");
}
