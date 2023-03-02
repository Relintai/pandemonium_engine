
#include "tones_map.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTonesMap::get_image() {
	return image;
}

void MMTonesMap::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMTonesMap::get_input() {
	return input;
}

void MMTonesMap::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Vector2 MMTonesMap::get_input_range() const {
	return _input_range;
}
void MMTonesMap::set_input_range(const Vector2 &val) {
	_input_range = val;
	set_dirty(true);
}

Vector2 MMTonesMap::get_output_range() const {
	return _output_range;
}
void MMTonesMap::set_output_range(const Vector2 &val) {
	_output_range = val;
	set_dirty(true);
}

void MMTonesMap::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

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

void MMTonesMap::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_input_range", "set_input_range", "Input Range", 0.001, Vector2(0, 1));
	mm_graph_node->add_slot_vector2("get_output_range", "set_output_range", "Output Range", 0.001, Vector2(0, 1));
}

void MMTonesMap::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTonesMap::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);

	//vec4(vec3($out_min)+($in($uv).rgb-vec3($in_min))*vec3(($out_max-($out_min))/($in_max-($in_min))), $in($uv).a)

	//vec3($out_min) + ($in($uv).rgb - vec3($in_min)) * vec3(($out_max - ($out_min)) / ($in_max - ($in_min)))

	Vector3 in_v3 = Vector3(c.r, c.g, c.b);
	Vector3 out_min_v3 = Vector3(_output_range.x, _output_range.x, _output_range.x);
	Vector3 in_min_v3 = Vector3(_input_range.x, _input_range.x, _input_range.x);

	float mm = (_output_range.y - _output_range.x) / (_input_range.y - _input_range.x);
	Vector3 min_max = Vector3(mm, mm, mm);

	Vector3 res = out_min_v3 + (in_v3 - in_min_v3) * min_max;

	return Color(res.x, res.y, res.z, c.a);
}

MMTonesMap::MMTonesMap() {
	_input_range = Vector2(0, 1);
	_output_range = Vector2(0, 1);
}

MMTonesMap::~MMTonesMap() {
}

void MMTonesMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTonesMap::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTonesMap::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMTonesMap::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTonesMap::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_input_range"), &MMTonesMap::get_input_range);
	ClassDB::bind_method(D_METHOD("set_input_range", "value"), &MMTonesMap::set_input_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "input_range"), "set_input_range", "get_input_range");

	ClassDB::bind_method(D_METHOD("get_output_range"), &MMTonesMap::get_output_range);
	ClassDB::bind_method(D_METHOD("set_output_range", "value"), &MMTonesMap::set_output_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "output_range"), "set_output_range", "get_output_range");
}
