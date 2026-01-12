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

VoxelLight::OwnerType VoxelLight::get_owner_type() const {
	return _owner_type;
}
void VoxelLight::set_owner_type(const OwnerType p_value) {
	_owner_type = p_value;
}

bool VoxelLight::get_has_owner_chunk() const {
	return _has_owner_chunk;
}

void VoxelLight::set_has_owner_chunk(const bool p_value) {
	_has_owner_chunk = p_value;
}

Vector3i VoxelLight::get_owner_chunk_position() const {
	return _owner_chunk_position;
}

void VoxelLight::set_owner_chunk_position(const Vector3i &p_owner_chunk_position) {
	_owner_chunk_position = p_owner_chunk_position;
}

Vector3i VoxelLight::get_world_data_position() const {
	return _world_data_position;
}
void VoxelLight::set_world_data_position(const Vector3i &p_world_data_position) {
	if (_world_data_position == p_world_data_position) {
		return;
	}

	_world_data_position = p_world_data_position;

	emit_signal("light_moved", Ref<VoxelLight>(this));
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

VoxelLight::LightMode VoxelLight::get_light_mode() const {
	return _light_mode;
}
void VoxelLight::set_light_mode(const LightMode value) {
	_light_mode = value;
}

int VoxelLight::get_item_cull_mask() {
	return _item_cull_mask;
}
void VoxelLight::set_item_cull_mask(const int p_item_cull_mask) {
	_item_cull_mask = p_item_cull_mask;
}

Dictionary VoxelLight::to_dict() {
	Dictionary data;

	data["owner_type"] = _owner_type;

	data["has_owner_chunk"] = _has_owner_chunk;
	data["owner_chunk_position"] = _owner_chunk_position;

	data["world_data_position"] = _world_data_position;

	data["range"] = _range;
	data["attenuation"] = _attenuation;
	data["color"] = _color;
	data["energy"] = _energy;
	data["indirect_energy"] = _indirect_energy;
	data["negative"] = _negative;
	data["specular"] = _specular;

	data["light_mode"] = (int)_light_mode;

	data["item_cull_mask"] = _item_cull_mask;

	return data;
}
void VoxelLight::from_dict(const Dictionary &p_data) {
	_owner_type = (OwnerType)((int)p_data["owner_type"]);

	_has_owner_chunk = p_data["has_owner_chunk"];
	_owner_chunk_position = p_data["owner_chunk_position"];

	_world_data_position = p_data["world_data_position"];

	_range = p_data["range"];
	_attenuation = p_data["attenuation"];
	_color = p_data["color"];
	_energy = p_data["energy"];
	_indirect_energy = p_data["indirect_energy"];
	_negative = p_data["negative"];
	_specular = p_data["specular"];

	_light_mode = (LightMode)((int)p_data["light_mode"]);

	_item_cull_mask = p_data["item_cull_mask"];
}

VoxelLight::VoxelLight() {
	_owner_type = OWNER_TYPE_NONE;
	_has_owner_chunk = false;
	_range = 0;
	_attenuation = 0;
	_energy = 0;
	_indirect_energy = 0;
	_negative = false;
	_specular = 0;
	_light_mode = LIGHT_MODE_ADD;
	_item_cull_mask = 1;
}

VoxelLight::~VoxelLight() {
}

void VoxelLight::_bind_methods() {
	ADD_SIGNAL(MethodInfo("light_moved", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLight")));

	ClassDB::bind_method(D_METHOD("get_owner_type"), &VoxelLight::get_owner_type);
	ClassDB::bind_method(D_METHOD("set_owner_type", "value"), &VoxelLight::set_owner_type);

	String owner_type_hint = "None";
#ifdef MODULE_PROPS_ENABLED
	owner_type_hint += ",Prop";
#endif

	ADD_PROPERTY(PropertyInfo(Variant::INT, "owner_type", PROPERTY_HINT_ENUM, owner_type_hint), "set_owner_type", "get_owner_type");

	ClassDB::bind_method(D_METHOD("get_has_owner_chunk"), &VoxelLight::get_has_owner_chunk);
	ClassDB::bind_method(D_METHOD("set_has_owner_chunk", "value"), &VoxelLight::set_has_owner_chunk);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_owner_chunk"), "set_has_owner_chunk", "get_has_owner_chunk");

	ClassDB::bind_method(D_METHOD("get_owner_chunk_position"), &VoxelLight::get_owner_chunk_position);
	ClassDB::bind_method(D_METHOD("set_owner_chunk_position", "world_data_position"), &VoxelLight::set_owner_chunk_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "owner_chunk_position"), "set_owner_chunk_position", "get_owner_chunk_position");

	ClassDB::bind_method(D_METHOD("get_world_data_position"), &VoxelLight::get_world_data_position);
	ClassDB::bind_method(D_METHOD("set_world_data_position", "world_data_position"), &VoxelLight::set_world_data_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3I, "world_data_position"), "set_world_data_position", "get_world_data_position");

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

	ClassDB::bind_method(D_METHOD("get_light_mode"), &VoxelLight::get_light_mode);
	ClassDB::bind_method(D_METHOD("set_light_mode", "value"), &VoxelLight::set_light_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_mode", PROPERTY_HINT_ENUM, "Add,Sub,Mix"), "set_light_mode", "get_light_mode");

	ClassDB::bind_method(D_METHOD("get_item_cull_mask"), &VoxelLight::get_item_cull_mask);
	ClassDB::bind_method(D_METHOD("set_item_cull_mask", "value"), &VoxelLight::set_item_cull_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "item_cull_mask", PROPERTY_HINT_LAYERS_3D_RENDER), "set_item_cull_mask", "get_item_cull_mask");

	ClassDB::bind_method(D_METHOD("to_dict"), &VoxelLight::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "data"), &VoxelLight::from_dict);

	BIND_ENUM_CONSTANT(OWNER_TYPE_NONE);
#ifdef MODULE_PROPS_ENABLED
	BIND_ENUM_CONSTANT(OWNER_TYPE_PROP);
#endif

	BIND_ENUM_CONSTANT(LIGHT_MODE_ADD);
	BIND_ENUM_CONSTANT(LIGHT_MODE_SUB);
	BIND_ENUM_CONSTANT(LIGHT_MODE_MIX);
}
