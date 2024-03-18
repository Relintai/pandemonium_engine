/*************************************************************************/
/*  prop_data_light.cpp                                                  */
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

#include "prop_data_light.h"

#include "prop_data.h"

#include "scene/3d/light.h"

real_t PropDataLight::get_light_range() const {
	return _light_range;
}
void PropDataLight::set_light_range(const real_t value) {
	_light_range = value;
}

real_t PropDataLight::get_light_attenuation() const {
	return _light_attenuation;
}
void PropDataLight::set_light_attenuation(const real_t value) {
	_light_attenuation = value;
}

Color PropDataLight::get_light_color() const {
	return _light_color;
}
void PropDataLight::set_light_color(const Color value) {
	_light_color = value;
}

real_t PropDataLight::get_light_energy() const {
	return _light_energy;
}
void PropDataLight::set_light_energy(const real_t value) {
	_light_energy = value;
}

real_t PropDataLight::get_light_indirect_energy() const {
	return _light_indirect_energy;
}
void PropDataLight::set_light_indirect_energy(const real_t value) {
	_light_indirect_energy = value;
}

bool PropDataLight::get_light_negative() const {
	return _light_negative;
}
void PropDataLight::set_light_negative(const bool value) {
	_light_negative = value;
}

real_t PropDataLight::get_light_specular() const {
	return _light_specular;
}
void PropDataLight::set_light_specular(const real_t value) {
	_light_specular = value;
}

bool PropDataLight::_processor_handles(Node *node) {
	OmniLight *i = Object::cast_to<OmniLight>(node);

	return i;
}

void PropDataLight::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	OmniLight *i = Object::cast_to<OmniLight>(node);

	ERR_FAIL_COND(!i);

	if (i->is_editor_only()) {
		return;
	}

	Ref<PropDataLight> l;
	l.instance();

	l->set_light_range(i->get_param(Light::PARAM_RANGE));
	l->set_light_attenuation(i->get_param(Light::PARAM_ATTENUATION));
	l->set_light_color(i->get_color());
	l->set_light_energy(i->get_param(Light::PARAM_ENERGY));
	l->set_light_indirect_energy(i->get_param(Light::PARAM_INDIRECT_ENERGY));
	l->set_light_negative(i->is_negative());
	l->set_light_specular(i->get_param(Light::PARAM_SPECULAR));
	l->set_transform(transform * i->get_transform());

	prop_data->add_prop(l);
}

Node *PropDataLight::_processor_get_node_for(const Transform &transform) {
	OmniLight *i = memnew(OmniLight);

	i->set_param(Light::PARAM_RANGE, get_light_range());
	i->set_param(Light::PARAM_ATTENUATION, get_light_attenuation());
	i->set_color(get_light_color());
	i->set_param(Light::PARAM_ENERGY, get_light_energy());
	i->set_param(Light::PARAM_INDIRECT_ENERGY, get_light_indirect_energy());
	i->set_negative(get_light_negative());
	i->set_param(Light::PARAM_SPECULAR, get_light_specular());
	i->set_transform(get_transform());

	return i;
}

PropDataLight::PropDataLight() {
	_light_range = 0;
	_light_attenuation = 0;
	_light_energy = 0;
	_light_indirect_energy = 0;
	_light_negative = false;
	_light_specular = 0;
}
PropDataLight::~PropDataLight() {
}

#ifndef DISABLE_DEPRECATED
bool PropDataLight::_set(const StringName &p_name, const Variant &p_value) {
	// Convert to range
	if (p_name == "light_size") {
		set_light_range(p_value);
	}

	return false;
}
#endif

void PropDataLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_light_range"), &PropDataLight::get_light_range);
	ClassDB::bind_method(D_METHOD("set_light_range", "value"), &PropDataLight::set_light_range);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_range"), "set_light_range", "get_light_range");

	ClassDB::bind_method(D_METHOD("get_light_attenuation"), &PropDataLight::get_light_attenuation);
	ClassDB::bind_method(D_METHOD("set_light_attenuation", "value"), &PropDataLight::set_light_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_attenuation"), "set_light_attenuation", "get_light_attenuation");

	ClassDB::bind_method(D_METHOD("get_light_color"), &PropDataLight::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &PropDataLight::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_energy"), &PropDataLight::get_light_energy);
	ClassDB::bind_method(D_METHOD("set_light_energy", "value"), &PropDataLight::set_light_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_energy"), "set_light_energy", "get_light_energy");

	ClassDB::bind_method(D_METHOD("get_light_indirect_energy"), &PropDataLight::get_light_indirect_energy);
	ClassDB::bind_method(D_METHOD("set_light_indirect_energy", "value"), &PropDataLight::set_light_indirect_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_indirect_energy"), "set_light_indirect_energy", "get_light_indirect_energy");

	ClassDB::bind_method(D_METHOD("get_light_negative"), &PropDataLight::get_light_negative);
	ClassDB::bind_method(D_METHOD("set_light_negative", "value"), &PropDataLight::set_light_negative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "light_negative"), "set_light_negative", "get_light_negative");

	ClassDB::bind_method(D_METHOD("get_light_specular"), &PropDataLight::get_light_specular);
	ClassDB::bind_method(D_METHOD("set_light_specular", "value"), &PropDataLight::set_light_specular);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_specular"), "set_light_specular", "get_light_specular");
}
