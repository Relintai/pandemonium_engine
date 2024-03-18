/*************************************************************************/
/*  prop.cpp                                                       */
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

#include "prop_light.h"

Vector3 PropLight::get_position() {
	return _position;
}
void PropLight::set_position(const Vector3 &pos) {
	_position = pos;
}

real_t PropLight::get_range() const {
	return _range;
}
void PropLight::set_range(const real_t value) {
	_range = value;
}

real_t PropLight::get_attenuation() const {
	return _attenuation;
}
void PropLight::set_attenuation(const real_t value) {
	_attenuation = value;
}

Color PropLight::get_color() const {
	return _color;
}
void PropLight::set_color(const Color value) {
	_color = value;
}

real_t PropLight::get_energy() const {
	return _energy;
}
void PropLight::set_energy(const real_t value) {
	_energy = value;
}

real_t PropLight::get_indirect_energy() const {
	return _indirect_energy;
}
void PropLight::set_indirect_energy(const real_t value) {
	_indirect_energy = value;
}

bool PropLight::get_negative() const {
	return _negative;
}
void PropLight::set_negative(const bool value) {
	_negative = value;
}

real_t PropLight::get_specular() const {
	return _specular;
}
void PropLight::set_specular(const real_t value) {
	_specular = value;
}

PropLight::PropLight() {
	_range = 0;
	_attenuation = 0;
	_energy = 0;
	_indirect_energy = 0;
	_negative = false;
	_specular = 0;
}

PropLight::~PropLight() {
}

#ifndef DISABLE_DEPRECATED
bool PropLight::_set(const StringName &p_name, const Variant &p_value) {
	// Convert to range
	if (p_name == "light_size") {
		set_range(p_value);
	}

	return false;
}
#endif

void PropLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &PropLight::get_position);
	ClassDB::bind_method(D_METHOD("set_position"), &PropLight::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "world_position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_range"), &PropLight::get_range);
	ClassDB::bind_method(D_METHOD("set_range", "value"), &PropLight::set_range);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_range"), "set_range", "get_range");

	ClassDB::bind_method(D_METHOD("get_attenuation"), &PropLight::get_attenuation);
	ClassDB::bind_method(D_METHOD("set_attenuation", "value"), &PropLight::set_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_attenuation"), "set_attenuation", "get_attenuation");

	ClassDB::bind_method(D_METHOD("get_color"), &PropLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "value"), &PropLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_energy"), &PropLight::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "value"), &PropLight::set_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_energy"), "set_energy", "get_energy");

	ClassDB::bind_method(D_METHOD("get_indirect_energy"), &PropLight::get_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_indirect_energy", "value"), &PropLight::set_indirect_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_indirect_energy"), "set_indirect_energy", "get_indirect_energy");

	ClassDB::bind_method(D_METHOD("get_negative"), &PropLight::get_negative);
	ClassDB::bind_method(D_METHOD("set_negative", "value"), &PropLight::set_negative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "light_negative"), "set_negative", "get_negative");

	ClassDB::bind_method(D_METHOD("get_specular"), &PropLight::get_specular);
	ClassDB::bind_method(D_METHOD("set_specular", "value"), &PropLight::set_specular);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_specular"), "set_specular", "get_specular");
}
