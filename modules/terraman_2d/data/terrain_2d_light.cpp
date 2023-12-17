

#include "terrain_2d_light.h"

_FORCE_INLINE_ int Terrain2DLight::get_world_position_x() const {
	return _world_position_x;
}
_FORCE_INLINE_ int Terrain2DLight::get_world_position_y() const {
	return _world_position_y;
}
Vector2 Terrain2DLight::get_world_position() {
	return Vector2(_world_position_x, _world_position_y);
}
void Terrain2DLight::set_world_position(const int x, const int y) {
	_world_position_x = x;
	_world_position_y = y;
}

_FORCE_INLINE_ Color Terrain2DLight::get_color() const {
	return _color;
}
void Terrain2DLight::set_color(const Color &color) {
	_color = color;
}

_FORCE_INLINE_ float Terrain2DLight::get_size() const {
	return _size;
}
void Terrain2DLight::set_size(const float size) {
	_size = size;
}

Terrain2DLight::Terrain2DLight() {
	_size = 0;
	_world_position_x = 0;
	_world_position_y = 0;
}

Terrain2DLight::~Terrain2DLight() {
}

void Terrain2DLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_world_position_x"), &Terrain2DLight::get_world_position_x);
	ClassDB::bind_method(D_METHOD("get_world_position_y"), &Terrain2DLight::get_world_position_y);
	ClassDB::bind_method(D_METHOD("set_world_position", "x", "y"), &Terrain2DLight::set_world_position);

	ClassDB::bind_method(D_METHOD("get_color"), &Terrain2DLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color"), &Terrain2DLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_size"), &Terrain2DLight::get_size);
	ClassDB::bind_method(D_METHOD("set_size"), &Terrain2DLight::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
