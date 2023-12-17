/*************************************************************************/
/*  terrain_environment_data.cpp                                         */
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

#include "terrain_environment_data.h"

#include "scene/resources/environment_3d.h"

Ref<Environment3D> TerrainEnvironmentData::get_environment() {
	return _environment;
}
void TerrainEnvironmentData::set_environment(const Ref<Environment3D> &value) {
	_environment = value;
}

Color TerrainEnvironmentData::get_color(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, Color());

	return _colors[index];
}
void TerrainEnvironmentData::set_color(const int index, const Color &value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_colors[index] = value;
}
float TerrainEnvironmentData::get_energy(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, 0);

	return _energies[index];
}
void TerrainEnvironmentData::set_energy(const int index, const float value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_energies[index] = value;
}
float TerrainEnvironmentData::get_indirect_energy(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, 0);

	return _indirect_energies[index];
}
void TerrainEnvironmentData::set_indirect_energy(const int index, const float value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_indirect_energies[index] = value;
}

void TerrainEnvironmentData::setup(WorldEnvironment3D *world_environment, DirectionalLight *primary_light, DirectionalLight *secondary_light) {
	if (has_method("_setup")) {
		call("_setup", world_environment, primary_light, secondary_light);
	}
}
void TerrainEnvironmentData::setup_bind(Node *world_environment, Node *primary_light, Node *secondary_light) {
	setup(Object::cast_to<WorldEnvironment3D>(world_environment), Object::cast_to<DirectionalLight>(primary_light), Object::cast_to<DirectionalLight>(secondary_light));
}

TerrainEnvironmentData::TerrainEnvironmentData() {
	_colors[0] = Color(1, 1, 1, 1);
	_colors[1] = Color(1, 1, 1, 1);

	_energies[0] = 1;
	_energies[1] = 1;

	_indirect_energies[0] = 1;
	_indirect_energies[1] = 1;
}

TerrainEnvironmentData::~TerrainEnvironmentData() {
	_environment.unref();
}

void TerrainEnvironmentData::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_setup", PropertyInfo(Variant::OBJECT, "world_environment", PROPERTY_HINT_RESOURCE_TYPE, "WorldEnvironment3D"), PropertyInfo(Variant::OBJECT, "primary_light", PROPERTY_HINT_RESOURCE_TYPE, "DirectionalLight"), PropertyInfo(Variant::OBJECT, "secondary_light", PROPERTY_HINT_RESOURCE_TYPE, "DirectionalLight")));

	ClassDB::bind_method(D_METHOD("get_environment"), &TerrainEnvironmentData::get_environment);
	ClassDB::bind_method(D_METHOD("set_environment", "value"), &TerrainEnvironmentData::set_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment3D"), "set_environment", "get_environment");

	ClassDB::bind_method(D_METHOD("get_color", "index"), &TerrainEnvironmentData::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "index", "value"), &TerrainEnvironmentData::set_color);

	ClassDB::bind_method(D_METHOD("get_energy", "index"), &TerrainEnvironmentData::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "index", "value"), &TerrainEnvironmentData::set_energy);

	ClassDB::bind_method(D_METHOD("get_indirect_energy", "index"), &TerrainEnvironmentData::get_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_indirect_energy", "index", "value"), &TerrainEnvironmentData::set_indirect_energy);

	ADD_PROPERTYI(PropertyInfo(Variant::COLOR, "primary_light_color"), "set_color", "get_color", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "primary_light_energy"), "set_energy", "get_energy", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "primary_light_indirect_energy"), "set_indirect_energy", "get_indirect_energy", 0);

	ADD_PROPERTYI(PropertyInfo(Variant::COLOR, "secondary_light_color"), "set_color", "get_color", 1);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "secondary_light_energy"), "set_energy", "get_energy", 1);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "secondary_light_indirect_energy"), "set_indirect_energy", "get_indirect_energy", 1);

	ClassDB::bind_method(D_METHOD("setup", "world_environment", "primary_light", "secondary_light"), &TerrainEnvironmentData::setup_bind);

	BIND_CONSTANT(LIGHT_COUNT);
}
