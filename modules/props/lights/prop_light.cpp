

#include "prop_light.h"

Vector3 PropLight::get_position() {
	return _position;
}
void PropLight::set_position(const Vector3 &pos) {
	_position = pos;
}

Color PropLight::get_color() const {
	return _color;
}
void PropLight::set_color(const Color &color) {
	_color = color;
}

float PropLight::get_size() const {
	return _size;
}
void PropLight::set_size(const float size) {
	_size = size;
}

PropLight::PropLight() {
	_size = 0;
}

PropLight::~PropLight() {
}

void PropLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &PropLight::get_position);
	ClassDB::bind_method(D_METHOD("set_position"), &PropLight::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "world_position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_color"), &PropLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color"), &PropLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_size"), &PropLight::get_size);
	ClassDB::bind_method(D_METHOD("set_size"), &PropLight::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
