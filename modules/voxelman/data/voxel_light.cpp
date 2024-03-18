/*************************************************************************/
/*  voxel_light.cpp                                                      */
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

#include "voxel_light.h"

_FORCE_INLINE_ int VoxelLight::get_world_position_x() const {
	return _world_position_x;
}
_FORCE_INLINE_ int VoxelLight::get_world_position_y() const {
	return _world_position_y;
}
_FORCE_INLINE_ int VoxelLight::get_world_position_z() const {
	return _world_position_z;
}
Vector3 VoxelLight::get_world_position() {
	return Vector3(_world_position_x, _world_position_y, _world_position_z);
}
void VoxelLight::set_world_position(const int x, const int y, const int z) {
	_world_position_x = x;
	_world_position_y = y;
	_world_position_z = z;
}

real_t VoxelLight::get_range() const {
	return _range;
}
void VoxelLight::set_range(const real_t value) {
	_range = value;
}

real_t VoxelLight::get_attenuation() const {
	return _attenuation;
}
void VoxelLight::set_attenuation(const real_t value) {
	_attenuation = value;
}

Color VoxelLight::get_color() const {
	return _color;
}
void VoxelLight::set_color(const Color value) {
	_color = value;
}

real_t VoxelLight::get_energy() const {
	return _energy;
}
void VoxelLight::set_energy(const real_t value) {
	_energy = value;
}

real_t VoxelLight::get_indirect_energy() const {
	return _indirect_energy;
}
void VoxelLight::set_indirect_energy(const real_t value) {
	_indirect_energy = value;
}

bool VoxelLight::get_negative() const {
	return _negative;
}
void VoxelLight::set_negative(const bool value) {
	_negative = value;
}

real_t VoxelLight::get_specular() const {
	return _specular;
}
void VoxelLight::set_specular(const real_t value) {
	_specular = value;
}

#ifndef DISABLE_DEPRECATED
bool VoxelLight::_set(const StringName &p_name, const Variant &p_value) {
	// Convert to range
	if (p_name == "light_size") {
		set_range(p_value);
	}

	return false;
}
#endif

VoxelLight::VoxelLight() {
	_range = 0;
	_attenuation = 0;
	_energy = 0;
	_indirect_energy = 0;
	_negative = false;
	_specular = 0;
}

VoxelLight::~VoxelLight() {
}

void VoxelLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_world_position_x"), &VoxelLight::get_world_position_x);
	ClassDB::bind_method(D_METHOD("get_world_position_y"), &VoxelLight::get_world_position_y);
	ClassDB::bind_method(D_METHOD("get_world_position_z"), &VoxelLight::get_world_position_z);
	ClassDB::bind_method(D_METHOD("set_world_position", "x", "y", "z"), &VoxelLight::set_world_position);

	ClassDB::bind_method(D_METHOD("get_range"), &VoxelLight::get_range);
	ClassDB::bind_method(D_METHOD("set_range", "value"), &VoxelLight::set_range);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_range"), "set_range", "get_range");

	ClassDB::bind_method(D_METHOD("get_attenuation"), &VoxelLight::get_attenuation);
	ClassDB::bind_method(D_METHOD("set_attenuation", "value"), &VoxelLight::set_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_attenuation"), "set_attenuation", "get_attenuation");

	ClassDB::bind_method(D_METHOD("get_color"), &VoxelLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "value"), &VoxelLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_energy"), &VoxelLight::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "value"), &VoxelLight::set_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_energy"), "set_energy", "get_energy");

	ClassDB::bind_method(D_METHOD("get_indirect_energy"), &VoxelLight::get_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_indirect_energy", "value"), &VoxelLight::set_indirect_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_indirect_energy"), "set_indirect_energy", "get_indirect_energy");

	ClassDB::bind_method(D_METHOD("get_negative"), &VoxelLight::get_negative);
	ClassDB::bind_method(D_METHOD("set_negative", "value"), &VoxelLight::set_negative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "light_negative"), "set_negative", "get_negative");

	ClassDB::bind_method(D_METHOD("get_specular"), &VoxelLight::get_specular);
	ClassDB::bind_method(D_METHOD("set_specular", "value"), &VoxelLight::set_specular);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_specular"), "set_specular", "get_specular");
}
