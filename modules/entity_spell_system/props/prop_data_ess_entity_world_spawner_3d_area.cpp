/*************************************************************************/
/*  prop_data_ess_entity_world_spawner_3d_area.cpp                       */
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

#include "prop_data_ess_entity_world_spawner_3d_area.h"

#include "../../props/props/prop_data.h"
#include "../entities/data/entity_data.h"
#include "../world_spawners/ess_entity_world_spawner_3d_area.h"

Vector3 PropDataESSEntityWorldSpawner3DArea::get_spawn_area_extents() const {
	return _spawn_area_extents;
}
void PropDataESSEntityWorldSpawner3DArea::set_spawn_area_extents(const Vector3 &p_extents) {
	_spawn_area_extents = p_extents;

	emit_changed();
}

int PropDataESSEntityWorldSpawner3DArea::get_spawn_slot_count() const {
	return _spawn_slot_count;
}
void PropDataESSEntityWorldSpawner3DArea::set_spawn_slot_count(const int p_spawn_slot_count) {
	_spawn_slot_count = p_spawn_slot_count;

	emit_changed();
}

float PropDataESSEntityWorldSpawner3DArea::get_respawn_time_min() const {
	return _respawn_time_min;
}
void PropDataESSEntityWorldSpawner3DArea::set_respawn_time_min(const float p_respawn_time) {
	_respawn_time_min = p_respawn_time;

	emit_changed();
}

int PropDataESSEntityWorldSpawner3DArea::get_spawn_entry_count() const {
	return _spawn_entries.size();
}
void PropDataESSEntityWorldSpawner3DArea::set_spawn_entry_count(const int p_spawn_entry_count) {
	_spawn_entries.resize(p_spawn_entry_count);

	property_list_changed_notify();
	emit_changed();
}

float PropDataESSEntityWorldSpawner3DArea::get_respawn_time_max() const {
	return _respawn_time_max;
}
void PropDataESSEntityWorldSpawner3DArea::set_respawn_time_max(const float p_respawn_time) {
	_respawn_time_max = p_respawn_time;

	emit_changed();
}

String PropDataESSEntityWorldSpawner3DArea::get_entity_name(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), String());

	return _spawn_entries[p_index].entity_name;
}
void PropDataESSEntityWorldSpawner3DArea::set_entity_name(const int p_index, const String &p_name) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].entity_name = p_name;

	emit_changed();
}

Ref<EntityData> PropDataESSEntityWorldSpawner3DArea::get_entity_data(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), Ref<EntityData>());

	return _spawn_entries[p_index].entity_data;
}
void PropDataESSEntityWorldSpawner3DArea::set_entity_data(const int p_index, const Ref<EntityData> &p_data) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].entity_data = p_data;

	emit_changed();
}

Vector2i PropDataESSEntityWorldSpawner3DArea::get_entity_level_range(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), Vector2i());

	return _spawn_entries[p_index].level_range;
}
void PropDataESSEntityWorldSpawner3DArea::set_entity_level_range(const int p_index, const Vector2i &p_level_range) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].level_range = p_level_range;

	emit_changed();
}

float PropDataESSEntityWorldSpawner3DArea::get_entity_spawn_chance(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), 0);

	return _spawn_entries[p_index].spawn_chance;
}
void PropDataESSEntityWorldSpawner3DArea::set_entity_spawn_chance(const int p_index, const float p_spawn_chance) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].spawn_chance = p_spawn_chance;

	emit_changed();
}

void PropDataESSEntityWorldSpawner3DArea::clear_spawn_entries() {
	_spawn_entries.clear();
	emit_changed();
}
void PropDataESSEntityWorldSpawner3DArea::add_spawn_entry(const String &p_name, const Ref<EntityData> &p_data, const Vector2i &p_level_range, const float p_spawn_chance) {
	SpawnEntry e;

	e.entity_name = p_name;
	e.entity_data = p_data;
	e.level_range = p_level_range;
	e.spawn_chance = p_spawn_chance;

	_spawn_entries.push_back(e);

	emit_changed();
}
void PropDataESSEntityWorldSpawner3DArea::remove_spawn_entry(const int p_index) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.remove(p_index);

	emit_changed();
}

bool PropDataESSEntityWorldSpawner3DArea::_processor_handles(Node *node) {
	ESSEntityWorldSpawner3DArea *i = Object::cast_to<ESSEntityWorldSpawner3DArea>(node);

	return i;
}

void PropDataESSEntityWorldSpawner3DArea::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	ESSEntityWorldSpawner3DArea *i = Object::cast_to<ESSEntityWorldSpawner3DArea>(node);

	ERR_FAIL_COND(!i);

	if (i->get_spawn_entry_count() == 0) {
		// skip
		return;
	}

	Ref<PropDataESSEntityWorldSpawner3DArea> l;
	l.instance();

	l->set_spawn_area_extents(i->get_spawn_area_extents());
	l->set_spawn_slot_count(i->get_spawn_slot_count());
	l->set_respawn_time_min(i->get_respawn_time_min());
	l->set_respawn_time_max(i->get_respawn_time_max());

	int sec = i->get_spawn_entry_count();

	l->set_spawn_entry_count(sec);

	for (int j = 0; j < sec; ++j) {
		l->set_entity_name(j, i->get_entity_name(j));
		l->set_entity_data(j, i->get_entity_data(j));
		l->set_entity_level_range(j, i->get_entity_level_range(j));
		l->set_entity_spawn_chance(j, i->get_entity_spawn_chance(j));
	}

	l->set_transform(transform * i->get_transform());

	prop_data->add_prop(l);
}

Node *PropDataESSEntityWorldSpawner3DArea::_processor_get_node_for(const Transform &transform) {
	ESSEntityWorldSpawner3DArea *i = memnew(ESSEntityWorldSpawner3DArea);

	i->set_spawn_area_extents(get_spawn_area_extents());
	i->set_spawn_slot_count(get_spawn_slot_count());
	i->set_respawn_time_min(get_respawn_time_min());
	i->set_respawn_time_max(get_respawn_time_max());

	int sec = get_spawn_entry_count();

	i->set_spawn_entry_count(sec);

	for (int j = 0; j < sec; ++j) {
		i->set_entity_name(j, get_entity_name(j));
		i->set_entity_data(j, get_entity_data(j));
		i->set_entity_level_range(j, get_entity_level_range(j));
		i->set_entity_spawn_chance(j, get_entity_spawn_chance(j));
	}

	i->set_transform(get_transform());

	return i;
}

PropDataESSEntityWorldSpawner3DArea::PropDataESSEntityWorldSpawner3DArea() {
	_spawn_slot_count = 0;
	_respawn_time_min = 0;
	_respawn_time_max = 0;
	_spawn_area_extents = Vector3(2, 2, 2);
}
PropDataESSEntityWorldSpawner3DArea::~PropDataESSEntityWorldSpawner3DArea() {
}

bool PropDataESSEntityWorldSpawner3DArea::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("spawn_entry_")) {
		int index = name.get_slicec('_', 2).to_int();

		if (index >= _spawn_entries.size()) {
			_spawn_entries.resize(index + 1);
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "entity_name") {
			_spawn_entries.write[index].entity_name = p_value;

			return true;
		} else if (p == "entity_data") {
			_spawn_entries.write[index].entity_data = p_value;

			return true;
		} else if (p == "level_range") {
			_spawn_entries.write[index].level_range = p_value;

			return true;
		} else if (p == "spawn_chance") {
			_spawn_entries.write[index].spawn_chance = p_value;

			return true;
		}
	}

	return false;
}

bool PropDataESSEntityWorldSpawner3DArea::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("spawn_entry_")) {
		int index = name.get_slicec('_', 2).to_int();

		if (index >= _spawn_entries.size()) {
			return false;
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "entity_name") {
			r_ret = _spawn_entries[index].entity_name;

			return true;
		} else if (p == "entity_data") {
			r_ret = _spawn_entries[index].entity_data;

			return true;
		} else if (p == "level_range") {
			r_ret = _spawn_entries[index].level_range;

			return true;
		} else if (p == "spawn_chance") {
			r_ret = _spawn_entries[index].spawn_chance;

			return true;
		}
	}

	return false;
}

void PropDataESSEntityWorldSpawner3DArea::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _spawn_entries.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::STRING, "spawn_entry_" + itos(i) + "/entity_name"));
		p_list->push_back(PropertyInfo(Variant::OBJECT, "spawn_entry_" + itos(i) + "/entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::VECTOR2I, "spawn_entry_" + itos(i) + "/level_range"));
		p_list->push_back(PropertyInfo(Variant::REAL, "spawn_entry_" + itos(i) + "/spawn_chance"));
	}
}

void PropDataESSEntityWorldSpawner3DArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_spawn_area_extents"), &PropDataESSEntityWorldSpawner3DArea::get_spawn_area_extents);
	ClassDB::bind_method(D_METHOD("set_spawn_area_extents", "value"), &PropDataESSEntityWorldSpawner3DArea::set_spawn_area_extents);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "spawn_area_extents"), "set_spawn_area_extents", "get_spawn_area_extents");

	ClassDB::bind_method(D_METHOD("get_spawn_slot_count"), &PropDataESSEntityWorldSpawner3DArea::get_spawn_slot_count);
	ClassDB::bind_method(D_METHOD("set_spawn_slot_count", "respawn_time"), &PropDataESSEntityWorldSpawner3DArea::set_spawn_slot_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "spawn_slot_count"), "set_spawn_slot_count", "get_spawn_slot_count");

	ClassDB::bind_method(D_METHOD("get_respawn_time_min"), &PropDataESSEntityWorldSpawner3DArea::get_respawn_time_min);
	ClassDB::bind_method(D_METHOD("set_respawn_time_min", "respawn_time"), &PropDataESSEntityWorldSpawner3DArea::set_respawn_time_min);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_min"), "set_respawn_time_min", "get_respawn_time_min");

	ClassDB::bind_method(D_METHOD("get_respawn_time_max"), &PropDataESSEntityWorldSpawner3DArea::get_respawn_time_max);
	ClassDB::bind_method(D_METHOD("set_respawn_time_max", "respawn_time"), &PropDataESSEntityWorldSpawner3DArea::set_respawn_time_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_max"), "set_respawn_time_max", "get_respawn_time_max");

	ClassDB::bind_method(D_METHOD("get_spawn_entry_count"), &PropDataESSEntityWorldSpawner3DArea::get_spawn_entry_count);
	ClassDB::bind_method(D_METHOD("set_spawn_entry_count", "spawn_entry_count"), &PropDataESSEntityWorldSpawner3DArea::set_spawn_entry_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "spawn_entry_count"), "set_spawn_entry_count", "get_spawn_entry_count");

	ClassDB::bind_method(D_METHOD("get_entity_name", "index"), &PropDataESSEntityWorldSpawner3DArea::get_entity_name);
	ClassDB::bind_method(D_METHOD("set_entity_name", "index", "name"), &PropDataESSEntityWorldSpawner3DArea::set_entity_name);

	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &PropDataESSEntityWorldSpawner3DArea::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &PropDataESSEntityWorldSpawner3DArea::set_entity_data);

	ClassDB::bind_method(D_METHOD("get_entity_level_range", "index"), &PropDataESSEntityWorldSpawner3DArea::get_entity_level_range);
	ClassDB::bind_method(D_METHOD("set_entity_level_range", "index", "level_range"), &PropDataESSEntityWorldSpawner3DArea::set_entity_level_range);

	ClassDB::bind_method(D_METHOD("get_entity_spawn_chance", "index"), &PropDataESSEntityWorldSpawner3DArea::get_entity_spawn_chance);
	ClassDB::bind_method(D_METHOD("set_entity_spawn_chance", "index", "spawn_chance"), &PropDataESSEntityWorldSpawner3DArea::set_entity_spawn_chance);

	ClassDB::bind_method(D_METHOD("clear_spawn_entries"), &PropDataESSEntityWorldSpawner3DArea::clear_spawn_entries);
	ClassDB::bind_method(D_METHOD("add_spawn_entry", "name", "data", "level_range", "spawn_chance"), &PropDataESSEntityWorldSpawner3DArea::add_spawn_entry);
	ClassDB::bind_method(D_METHOD("remove_spawn_entry", "index"), &PropDataESSEntityWorldSpawner3DArea::remove_spawn_entry);
}

PropDataESSEntityWorldSpawner3DArea::SpawnEntry::SpawnEntry() {
	spawn_chance = 1;
}
PropDataESSEntityWorldSpawner3DArea::SpawnEntry::~SpawnEntry() {
}
