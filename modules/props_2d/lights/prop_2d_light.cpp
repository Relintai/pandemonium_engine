

#include "prop_2d_light.h"

Vector2 Prop2DLight::get_position() {
	return _position;
}
void Prop2DLight::set_position(const Vector2 &pos) {
	_position = pos;
}

Color Prop2DLight::get_color() const {
	return _color;
}
void Prop2DLight::set_color(const Color &color) {
	_color = color;
}

float Prop2DLight::get_size() const {
	return _size;
}
void Prop2DLight::set_size(const float size) {
	_size = size;
}

Prop2DLight::Prop2DLight() {
	_size = 0;
}

Prop2DLight::~Prop2DLight() {
}

void Prop2DLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &Prop2DLight::get_position);
	ClassDB::bind_method(D_METHOD("set_position"), &Prop2DLight::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "world_position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_color"), &Prop2DLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color"), &Prop2DLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_size"), &Prop2DLight::get_size);
	ClassDB::bind_method(D_METHOD("set_size"), &Prop2DLight::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
