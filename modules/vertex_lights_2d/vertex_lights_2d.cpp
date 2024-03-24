/*************************************************************************/
/*  vertex_lights_2d.cpp                                                 */
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

#include "vertex_lights_2d.h"

#include "core/config/project_settings.h"

#include "vertex_light_data.h"

/*
Transform VertexLights2D::get_transform() const {
	return _transform;
}
void VertexLights2D::set_transform(const Transform &p_transform) {
	_transform = p_transform;
}

real_t VertexLights2D::get_range() const {
	return _range;
}
void VertexLights2D::set_range(const real_t value) {
	_range = value;
}

real_t VertexLights2D::get_attenuation() const {
	return _attenuation;
}
void VertexLights2D::set_attenuation(const real_t value) {
	_attenuation = value;
}

Color VertexLights2D::get_color() const {
	return _color;
}
void VertexLights2D::set_color(const Color value) {
	_color = value;
}

real_t VertexLights2D::get_energy() const {
	return _energy;
}
void VertexLights2D::set_energy(const real_t value) {
	_energy = value;
}

real_t VertexLights2D::get_indirect_energy() const {
	return _indirect_energy;
}
void VertexLights2D::set_indirect_energy(const real_t value) {
	_indirect_energy = value;
}

bool VertexLights2D::get_negative() const {
	return _negative;
}
void VertexLights2D::set_negative(const bool value) {
	_negative = value;
}

real_t VertexLights2D::get_specular() const {
	return _specular;
}
void VertexLights2D::set_specular(const real_t value) {
	_specular = value;
}
*/

VertexLights2D::VertexLights2D() {
	ERR_FAIL_COND(_self);

	_self = this;
	
	GLOBAL_DEF("vertex_lights_2d/default_quadrant_size", Vector2i(256, 256));
	_default_quadrant_size = GLOBAL_GET("vertex_lights_2d/default_quadrant_size");
}

VertexLights2D::~VertexLights2D() {
	_self = NULL;
}

void VertexLights2D::_bind_methods() {
	/*
	ClassDB::bind_method(D_METHOD("get_transform"), &VertexLights2D::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "transform"), &VertexLights2D::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_range"), &VertexLights2D::get_range);
	ClassDB::bind_method(D_METHOD("set_range", "value"), &VertexLights2D::set_range);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_range"), "set_range", "get_range");

	ClassDB::bind_method(D_METHOD("get_attenuation"), &VertexLights2D::get_attenuation);
	ClassDB::bind_method(D_METHOD("set_attenuation", "value"), &VertexLights2D::set_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_attenuation"), "set_attenuation", "get_attenuation");

	ClassDB::bind_method(D_METHOD("get_color"), &VertexLights2D::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "value"), &VertexLights2D::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_energy"), &VertexLights2D::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "value"), &VertexLights2D::set_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_energy"), "set_energy", "get_energy");

	ClassDB::bind_method(D_METHOD("get_indirect_energy"), &VertexLights2D::get_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_indirect_energy", "value"), &VertexLights2D::set_indirect_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_indirect_energy"), "set_indirect_energy", "get_indirect_energy");

	ClassDB::bind_method(D_METHOD("get_negative"), &VertexLights2D::get_negative);
	ClassDB::bind_method(D_METHOD("set_negative", "value"), &VertexLights2D::set_negative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "light_negative"), "set_negative", "get_negative");

	ClassDB::bind_method(D_METHOD("get_specular"), &VertexLights2D::get_specular);
	ClassDB::bind_method(D_METHOD("set_specular", "value"), &VertexLights2D::set_specular);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_specular"), "set_specular", "get_specular");
	*/
}

VertexLights2D *VertexLights2D::_self = NULL;
