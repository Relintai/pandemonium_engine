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

#include "terrain_2d_environment_data.h"

Ref<Environment> Terrain2DEnvironmentData::get_environment() {
	return _environment;
}
void Terrain2DEnvironmentData::set_environment(const Ref<Environment> &value) {
	_environment = value;
}

Color Terrain2DEnvironmentData::get_color(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, Color());

	return _colors[index];
}
void Terrain2DEnvironmentData::set_color(const int index, const Color &value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_colors[index] = value;
}
float Terrain2DEnvironmentData::get_energy(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, 0);

	return _energies[index];
}
void Terrain2DEnvironmentData::set_energy(const int index, const float value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_energies[index] = value;
}
float Terrain2DEnvironmentData::get_indirect_energy(const int index) {
	ERR_FAIL_INDEX_V(index, LIGHT_COUNT, 0);

	return _indirect_energies[index];
}
void Terrain2DEnvironmentData::set_indirect_energy(const int index, const float value) {
	ERR_FAIL_INDEX(index, LIGHT_COUNT);

	_indirect_energies[index] = value;
}

void Terrain2DEnvironmentData::setup(WorldEnvironment *world_environment, DirectionalLight *primary_light, DirectionalLight *secondary_light) {
	if (has_method("_setup")) {
		CALL(_setup, world_environment, primary_light, secondary_light);
	}
}
void Terrain2DEnvironmentData::setup_bind(Node *world_environment, Node *primary_light, Node *secondary_light) {
	setup(Object::cast_to<WorldEnvironment>(world_environment), Object::cast_to<DirectionalLight>(primary_light), Object::cast_to<DirectionalLight>(secondary_light));
}

Terrain2DEnvironmentData::Terrain2DEnvironmentData() {
	_colors[0] = Color(1, 1, 1, 1);
	_colors[1] = Color(1, 1, 1, 1);

	_energies[0] = 1;
	_energies[1] = 1;

	_indirect_energies[0] = 1;
	_indirect_energies[1] = 1;
}

Terrain2DEnvironmentData::~Terrain2DEnvironmentData() {
	_environment.unref();
}

void Terrain2DEnvironmentData::_bind_methods() {
#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup", PropertyInfo(Variant::OBJECT, "world_environment", PROPERTY_HINT_RESOURCE_TYPE, "WorldEnvironment"), PropertyInfo(Variant::OBJECT, "primary_light", PROPERTY_HINT_RESOURCE_TYPE, "DirectionalLight"), PropertyInfo(Variant::OBJECT, "secondary_light", PROPERTY_HINT_RESOURCE_TYPE, "DirectionalLight")));
#else
	GDVIRTUAL_BIND(_setup, "world_environment", "primary_light", "secondary_light");
#endif

	ClassDB::bind_method(D_METHOD("get_environment"), &Terrain2DEnvironmentData::get_environment);
	ClassDB::bind_method(D_METHOD("set_environment", "value"), &Terrain2DEnvironmentData::set_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");

	ClassDB::bind_method(D_METHOD("get_color", "index"), &Terrain2DEnvironmentData::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "index", "value"), &Terrain2DEnvironmentData::set_color);

	ClassDB::bind_method(D_METHOD("get_energy", "index"), &Terrain2DEnvironmentData::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "index", "value"), &Terrain2DEnvironmentData::set_energy);

	ClassDB::bind_method(D_METHOD("get_indirect_energy", "index"), &Terrain2DEnvironmentData::get_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_indirect_energy", "index", "value"), &Terrain2DEnvironmentData::set_indirect_energy);

	ADD_PROPERTYI(PropertyInfo(Variant::COLOR, "primary_light_color"), "set_color", "get_color", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "primary_light_energy"), "set_energy", "get_energy", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "primary_light_indirect_energy"), "set_indirect_energy", "get_indirect_energy", 0);

	ADD_PROPERTYI(PropertyInfo(Variant::COLOR, "secondary_light_color"), "set_color", "get_color", 1);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "secondary_light_energy"), "set_energy", "get_energy", 1);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "secondary_light_indirect_energy"), "set_indirect_energy", "get_indirect_energy", 1);

	ClassDB::bind_method(D_METHOD("setup", "world_environment", "primary_light", "secondary_light"), &Terrain2DEnvironmentData::setup_bind);

	BIND_CONSTANT(LIGHT_COUNT);
}
