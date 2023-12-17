

#include "terrain_world_area.h"

AABB TerrainWorldArea::get_aabb() const {
	return _aabb;
}
void TerrainWorldArea::set_aabb(const AABB value) {
	_aabb = value;
}

Ref<Texture> TerrainWorldArea::get_map_texture() const {
	return _map_texture;
}
void TerrainWorldArea::set_map_texture(const Ref<Texture> &value) {
	_map_texture = value;
}

Ref<Texture> TerrainWorldArea::get_fov_texture() const {
	return _fov_texture;
}
void TerrainWorldArea::set_fov_texture(const Ref<Texture> &value) {
	_fov_texture = value;
}

String TerrainWorldArea::get_name() const {
	return _name;
}
void TerrainWorldArea::set_name(const String &value) {
	_name = value;
}

int TerrainWorldArea::get_level() const {
	return _level;
}
void TerrainWorldArea::set_level(const int level) {
	_level = level;
}

TerrainWorldArea::TerrainWorldArea() {
	_level = 0;
}

TerrainWorldArea::~TerrainWorldArea() {
}

void TerrainWorldArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_aabb"), &TerrainWorldArea::get_aabb);
	ClassDB::bind_method(D_METHOD("set_aabb"), &TerrainWorldArea::set_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "aabb"), "set_aabb", "get_aabb");

	ClassDB::bind_method(D_METHOD("get_map_texture"), &TerrainWorldArea::get_map_texture);
	ClassDB::bind_method(D_METHOD("set_map_texture"), &TerrainWorldArea::set_map_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "map_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_map_texture", "get_map_texture");

	ClassDB::bind_method(D_METHOD("get_fov_texture"), &TerrainWorldArea::get_fov_texture);
	ClassDB::bind_method(D_METHOD("set_fov_texture"), &TerrainWorldArea::set_fov_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fov_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_fov_texture", "get_fov_texture");

	ClassDB::bind_method(D_METHOD("get_name"), &TerrainWorldArea::get_name);
	ClassDB::bind_method(D_METHOD("set_name"), &TerrainWorldArea::set_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("get_level"), &TerrainWorldArea::get_level);
	ClassDB::bind_method(D_METHOD("set_level"), &TerrainWorldArea::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
