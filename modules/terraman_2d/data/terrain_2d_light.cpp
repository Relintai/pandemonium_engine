/*************************************************************************/
/*  terrain_2d_light.cpp                                                 */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
