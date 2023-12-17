

#include "terrain_light.h"

_FORCE_INLINE_ int TerrainLight::get_world_position_x() const {
	return _world_position_x;
}
_FORCE_INLINE_ int TerrainLight::get_world_position_y() const {
	return _world_position_y;
}
_FORCE_INLINE_ int TerrainLight::get_world_position_z() const {
	return _world_position_z;
}
Vector3 TerrainLight::get_world_position() {
	return Vector3(_world_position_x, _world_position_y, _world_position_z);
}
void TerrainLight::set_world_position(const int x, const int y, const int z) {
	_world_position_x = x;
	_world_position_y = y;
	_world_position_z = z;
}

_FORCE_INLINE_ Color TerrainLight::get_color() const {
	return _color;
}
void TerrainLight::set_color(const Color &color) {
	_color = color;
}

_FORCE_INLINE_ float TerrainLight::get_size() const {
	return _size;
}
void TerrainLight::set_size(const float size) {
	_size = size;
}

TerrainLight::TerrainLight() {
	_size = 0;
}

TerrainLight::~TerrainLight() {
}

void TerrainLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_world_position_x"), &TerrainLight::get_world_position_x);
	ClassDB::bind_method(D_METHOD("get_world_position_y"), &TerrainLight::get_world_position_y);
	ClassDB::bind_method(D_METHOD("get_world_position_z"), &TerrainLight::get_world_position_z);
	ClassDB::bind_method(D_METHOD("set_world_position", "x", "y", "z"), &TerrainLight::set_world_position);

	ClassDB::bind_method(D_METHOD("get_color"), &TerrainLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color"), &TerrainLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_size"), &TerrainLight::get_size);
	ClassDB::bind_method(D_METHOD("set_size"), &TerrainLight::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
