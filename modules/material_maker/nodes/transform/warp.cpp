
#include "warp.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMWarp::get_image() {
	return _image;
}

void MMWarp::set_image(const Ref<MMNodeUniversalProperty> &val) {
	_image = val;
}

Ref<MMNodeUniversalProperty> MMWarp::get_input() {
	return _input;
}

void MMWarp::set_input(const Ref<MMNodeUniversalProperty> &val) {
	_input = val;
}

Ref<MMNodeUniversalProperty> MMWarp::get_height_map() {
	return _height_map;
}

void MMWarp::set_height_map(const Ref<MMNodeUniversalProperty> &val) {
	_height_map = val;
}

int MMWarp::get_mode() const {
	return _mode;
}

void MMWarp::set_mode(const int val) {
	_mode = val;
	set_dirty(true);
}

float MMWarp::get_amount() const {
	return _amount;
}

void MMWarp::set_amount(const float val) {
	_amount = val;
	set_dirty(true);
}

float MMWarp::get_epsilon() const {
	return _epsilon;
}

void MMWarp::set_epsilon(const float val) {
	_epsilon = val;
	set_dirty(true);
}

void MMWarp::_init_properties() {
	if (!_image.is_valid()) {
		_image.instance();
		_image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	_image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!_input.is_valid()) {
		_input.instance();
		_input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		_input->set_default_value(Color());
	}

	_input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	_input->set_slot_name(">>>    Input1    ");

	if (!_height_map.is_valid()) {
		_height_map.instance();
		_height_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		_height_map->set_default_value(0);
	}

	_height_map->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	_height_map->set_slot_name(">>>  Height Map  ");

	register_input_property(_input);
	register_output_property(_image);
	register_input_property(_height_map);
}

void MMWarp::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(_image);
	mm_graph_node->add_slot_label_universal(_input);
	mm_graph_node->add_slot_label_universal(_height_map);

	Array arr;
	arr.push_back("Slope");
	arr.push_back("Distance to top");
	mm_graph_node->add_slot_enum("get_mode", "set_mode", "Mode", arr);

	mm_graph_node->add_slot_float("get_amount", "set_amount", "Amount", 0.0001, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_epsilon", "set_epsilon", "Epsilon", 0.0001, Vector2(0.005, 0.2));
}

void MMWarp::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	_image->set_value(img);
}

// Warp.mmg

//input default -> vec4(sin($uv.x*20.0)*0.5+0.5, sin($uv.y*20.0)*0.5+0.5, 0, 1)

//vec2 $(name)_slope(vec2 uv, float epsilon) {
//	return vec2($d(fract(uv+vec2(epsilon, 0.0)))-$d(fract(uv-vec2(epsilon, 0.0))), $d(fract(uv+vec2(0.0, epsilon)))-$d(fract(uv-vec2(0.0, epsilon))));
//}

//vec2 $(name_uv)_slope = $(name)_slope($uv, $eps);

//Slope
//mode -> $(name_uv)_slope"

//Distance to top
//mode -> $(name_uv)_slope*(1.0-$d($uv))

//vec2 $(name_uv)_warp = $mode

//ret -> $in($uv+$amount*$(name_uv)_warp)

Color MMWarp::_get_value_for(const Vector2 &uv, const int pseed) {
	float sx1 = _height_map->get_value(MMAlgos::fractv2(uv + Vector2(_epsilon, 0.0)));
	float sx2 = _height_map->get_value(MMAlgos::fractv2(uv - Vector2(_epsilon, 0.0)));
	float sy1 = _height_map->get_value(MMAlgos::fractv2(uv + Vector2(0.0, _epsilon)));
	float sy2 = _height_map->get_value(MMAlgos::fractv2(uv - Vector2(0.0, _epsilon)));

	Vector2 slope = Vector2(sx1 - sx2, sy1 - sy2);
	Vector2 mode = slope;

	if (_mode == 1) {
		float m = _height_map->get_value(uv);
		m = 1.0 - m;
		mode = slope * Vector2(m, m);
	}

	Vector2 warp = mode;

	return _input->get_value(uv + Vector2(_amount, _amount) * warp);
}

MMWarp::MMWarp() {
	_mode = 0;
	_amount = 0.1;
	_epsilon = 0.1;
}

MMWarp::~MMWarp() {
}

void MMWarp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMWarp::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMWarp::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMWarp::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMWarp::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_height_map"), &MMWarp::get_height_map);
	ClassDB::bind_method(D_METHOD("set_height_map", "value"), &MMWarp::set_height_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "height_map", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_height_map", "get_height_map");

	ClassDB::bind_method(D_METHOD("get_mode"), &MMWarp::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "value"), &MMWarp::set_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode"), "set_mode", "get_mode");

	ClassDB::bind_method(D_METHOD("get_amount"), &MMWarp::get_amount);
	ClassDB::bind_method(D_METHOD("set_amount", "value"), &MMWarp::set_amount);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");

	ClassDB::bind_method(D_METHOD("get_epsilon"), &MMWarp::get_epsilon);
	ClassDB::bind_method(D_METHOD("set_epsilon", "value"), &MMWarp::set_epsilon);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "epsilon"), "set_epsilon", "get_epsilon");
}
