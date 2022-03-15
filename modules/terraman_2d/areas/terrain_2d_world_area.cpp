/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "terrain_2d_world_area.h"

AABB Terrain2DWorldArea::get_aabb() const {
	return _aabb;
}
void Terrain2DWorldArea::set_aabb(const AABB value) {
	_aabb = value;
}

Ref<Texture> Terrain2DWorldArea::get_map_texture() const {
	return _map_texture;
}
void Terrain2DWorldArea::set_map_texture(const Ref<Texture> &value) {
	_map_texture = value;
}

Ref<Texture> Terrain2DWorldArea::get_fov_texture() const {
	return _fov_texture;
}
void Terrain2DWorldArea::set_fov_texture(const Ref<Texture> &value) {
	_fov_texture = value;
}

String Terrain2DWorldArea::get_name() const {
	return _name;
}
void Terrain2DWorldArea::set_name(const String &value) {
	_name = value;
}

int Terrain2DWorldArea::get_level() const {
	return _level;
}
void Terrain2DWorldArea::set_level(const int level) {
	_level = level;
}

Terrain2DWorldArea::Terrain2DWorldArea() {
	_level = 0;
}

Terrain2DWorldArea::~Terrain2DWorldArea() {
}

void Terrain2DWorldArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_aabb"), &Terrain2DWorldArea::get_aabb);
	ClassDB::bind_method(D_METHOD("set_aabb"), &Terrain2DWorldArea::set_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "aabb"), "set_aabb", "get_aabb");

	ClassDB::bind_method(D_METHOD("get_map_texture"), &Terrain2DWorldArea::get_map_texture);
	ClassDB::bind_method(D_METHOD("set_map_texture"), &Terrain2DWorldArea::set_map_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "map_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_map_texture", "get_map_texture");

	ClassDB::bind_method(D_METHOD("get_fov_texture"), &Terrain2DWorldArea::get_fov_texture);
	ClassDB::bind_method(D_METHOD("set_fov_texture"), &Terrain2DWorldArea::set_fov_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fov_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_fov_texture", "get_fov_texture");

	ClassDB::bind_method(D_METHOD("get_name"), &Terrain2DWorldArea::get_name);
	ClassDB::bind_method(D_METHOD("set_name"), &Terrain2DWorldArea::set_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("get_level"), &Terrain2DWorldArea::get_level);
	ClassDB::bind_method(D_METHOD("set_level"), &Terrain2DWorldArea::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
