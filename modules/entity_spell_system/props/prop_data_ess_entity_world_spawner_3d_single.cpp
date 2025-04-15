/*************************************************************************/
/*  prop_data_ess_entity_world_spawner_3d_single.cpp                     */
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

#include "prop_data_ess_entity_world_spawner_3d_single.h"

#include "../../props/props/prop_data.h"
#include "../entities/data/entity_data.h"
#include "../world_spawners/ess_entity_world_spawner_3d_single.h"

String PropDataESSEntityWorldSpawner3DSingle::get_entity_name() const {
	return _entity_name;
}
void PropDataESSEntityWorldSpawner3DSingle::set_entity_name(const String &p_name) {
	_entity_name = p_name;
}

Ref<EntityData> PropDataESSEntityWorldSpawner3DSingle::get_entity_data() const {
	return _entity_data;
}
void PropDataESSEntityWorldSpawner3DSingle::set_entity_data(const Ref<EntityData> &p_data) {
	_entity_data = p_data;
}

int PropDataESSEntityWorldSpawner3DSingle::get_entity_level() const {
	return _entity_level;
}
void PropDataESSEntityWorldSpawner3DSingle::set_entity_level(const int p_level) {
	_entity_level = p_level;
}

float PropDataESSEntityWorldSpawner3DSingle::get_respawn_time_min() const {
	return _respawn_time_min;
}
void PropDataESSEntityWorldSpawner3DSingle::set_respawn_time_min(const float p_respawn_time) {
	_respawn_time_min = p_respawn_time;
}

float PropDataESSEntityWorldSpawner3DSingle::get_respawn_time_max() const {
	return _respawn_time_max;
}
void PropDataESSEntityWorldSpawner3DSingle::set_respawn_time_max(const float p_respawn_time) {
	_respawn_time_max = p_respawn_time;
}

bool PropDataESSEntityWorldSpawner3DSingle::_processor_handles(Node *node) {
	ESSEntityWorldSpawner3DSingle *i = Object::cast_to<ESSEntityWorldSpawner3DSingle>(node);

	return i;
}

void PropDataESSEntityWorldSpawner3DSingle::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	ESSEntityWorldSpawner3DSingle *i = Object::cast_to<ESSEntityWorldSpawner3DSingle>(node);

	ERR_FAIL_COND(!i);

	if (!i->get_entity_data().is_valid()) {
		// skip
		return;
	}

	Ref<PropDataESSEntityWorldSpawner3DSingle> l;
	l.instance();

	l->set_entity_name(i->get_entity_name());
	l->set_entity_data(i->get_entity_data());
	l->set_entity_level(i->get_entity_level());
	l->set_respawn_time_min(i->get_respawn_time_min());
	l->set_respawn_time_max(i->get_respawn_time_max());
	l->set_transform(transform * i->get_transform());

	prop_data->add_prop(l);
}

Node *PropDataESSEntityWorldSpawner3DSingle::_processor_get_node_for(const Transform &transform) {
	ESSEntityWorldSpawner3DSingle *i = memnew(ESSEntityWorldSpawner3DSingle);

	i->set_entity_name(get_entity_name());
	i->set_entity_data(get_entity_data());
	i->set_entity_level(get_entity_level());
	i->set_respawn_time_min(get_respawn_time_min());
	i->set_respawn_time_max(get_respawn_time_max());
	i->set_transform(get_transform());

	return i;
}

PropDataESSEntityWorldSpawner3DSingle::PropDataESSEntityWorldSpawner3DSingle() {
	_entity_level = 1;
	_respawn_time_min = 0;
	_respawn_time_max = 0;
	_respawn_timer = 0;
}
PropDataESSEntityWorldSpawner3DSingle::~PropDataESSEntityWorldSpawner3DSingle() {
}

void PropDataESSEntityWorldSpawner3DSingle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_name"), &PropDataESSEntityWorldSpawner3DSingle::get_entity_name);
	ClassDB::bind_method(D_METHOD("set_entity_name", "value"), &PropDataESSEntityWorldSpawner3DSingle::set_entity_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "entity_name"), "set_entity_name", "get_entity_name");

	ClassDB::bind_method(D_METHOD("get_entity_data"), &PropDataESSEntityWorldSpawner3DSingle::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "value"), &PropDataESSEntityWorldSpawner3DSingle::set_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "set_entity_data", "get_entity_data");

	ClassDB::bind_method(D_METHOD("get_entity_level"), &PropDataESSEntityWorldSpawner3DSingle::get_entity_level);
	ClassDB::bind_method(D_METHOD("set_entity_level", "value"), &PropDataESSEntityWorldSpawner3DSingle::set_entity_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entity_level"), "set_entity_level", "get_entity_level");

	ClassDB::bind_method(D_METHOD("get_respawn_time_min"), &PropDataESSEntityWorldSpawner3DSingle::get_respawn_time_min);
	ClassDB::bind_method(D_METHOD("set_respawn_time_min", "respawn_time"), &PropDataESSEntityWorldSpawner3DSingle::set_respawn_time_min);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_min"), "set_respawn_time_min", "get_respawn_time_min");

	ClassDB::bind_method(D_METHOD("get_respawn_time_max"), &PropDataESSEntityWorldSpawner3DSingle::get_respawn_time_max);
	ClassDB::bind_method(D_METHOD("set_respawn_time_max", "respawn_time"), &PropDataESSEntityWorldSpawner3DSingle::set_respawn_time_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_max"), "set_respawn_time_max", "get_respawn_time_max");
}
