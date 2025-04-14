/*************************************************************************/
/*  ess_entity_world_spawner_3d_area.cpp                                 */
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

#include "ess_entity_world_spawner_3d_area.h"

#include "../data/species/entity_species_data.h"
#include "../data/species/species_instance.h"
#include "../entities/data/entity_data.h"
#include "../entities/entity.h"
#include "../entity_enums.h"
#include "../utility/entity_create_info.h"
#include "scene/main/scene_tree.h"

#include "core/config/engine.h"

#include "../singletons/ess.h"

Vector3 ESSEntityWorldSpawner3DArea::get_spawn_area_extents() const {
	return _spawn_area_extents;
}
void ESSEntityWorldSpawner3DArea::set_spawn_area_extents(const Vector3 &p_extents) {
	_spawn_area_extents = p_extents;

	_spawn_slots_dirty = true;
}

int ESSEntityWorldSpawner3DArea::get_spawn_slot_count() const {
	return _spawn_slot_count;
}
void ESSEntityWorldSpawner3DArea::set_spawn_slot_count(const int p_spawn_slot_count) {
	_spawn_slot_count = p_spawn_slot_count;

	_spawn_slots_dirty = true;
}

float ESSEntityWorldSpawner3DArea::get_respawn_time_min() const {
	return _respawn_time_min;
}
void ESSEntityWorldSpawner3DArea::set_respawn_time_min(const float p_respawn_time) {
	_respawn_time_min = p_respawn_time;
}

float ESSEntityWorldSpawner3DArea::get_respawn_time_max() const {
	return _respawn_time_max;
}
void ESSEntityWorldSpawner3DArea::set_respawn_time_max(const float p_respawn_time) {
	_respawn_time_max = p_respawn_time;
}

String ESSEntityWorldSpawner3DArea::get_entity_name(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), String());

	return _spawn_entries[p_index].entity_name;
}
void ESSEntityWorldSpawner3DArea::set_entity_name(const int p_index, const String &p_name) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].entity_name = p_name;

	_spawn_entries_dirty = true;
}

Ref<EntityData> ESSEntityWorldSpawner3DArea::get_entity_data(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), Ref<EntityData>());

	return _spawn_entries[p_index].entity_data;
}
void ESSEntityWorldSpawner3DArea::set_entity_data(const int p_index, const Ref<EntityData> &p_data) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].entity_data = p_data;

	_spawn_entries_dirty = true;
}

Vector2i ESSEntityWorldSpawner3DArea::get_entity_level_range(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), Vector2i());

	return _spawn_entries[p_index].level_range;
}
void ESSEntityWorldSpawner3DArea::set_entity_level_range(const int p_index, const Vector2i &p_level_range) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].level_range = p_level_range;

	_spawn_entries_dirty = true;
}

float ESSEntityWorldSpawner3DArea::get_entity_spawn_chance(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _spawn_entries.size(), 0);

	return _spawn_entries[p_index].spawn_chance;
}
void ESSEntityWorldSpawner3DArea::set_entity_spawn_chance(const int p_index, const float p_spawn_chance) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.write[p_index].spawn_chance = p_spawn_chance;

	_spawn_entries_dirty = true;
}

int ESSEntityWorldSpawner3DArea::get_spawn_entry_count() const {
	return _spawn_entries.size();
}
void ESSEntityWorldSpawner3DArea::clear_spawn_entries() {
	_spawn_entries.clear();
	_spawn_entries_dirty = true;
}
void ESSEntityWorldSpawner3DArea::add_spawn_entry(const String &p_name, const Ref<EntityData> &p_data, const Vector2i &p_level_range, const float p_spawn_chance) {
	SpawnEntry e;

	e.entity_name = p_name;
	e.entity_data = p_data;
	e.level_range = p_level_range;
	e.spawn_chance = p_spawn_chance;

	_spawn_entries.push_back(e);

	_spawn_entries_dirty = true;
}
void ESSEntityWorldSpawner3DArea::remove_spawn_entry(const int p_index) {
	ERR_FAIL_INDEX(p_index, _spawn_entries.size());

	_spawn_entries.remove(p_index);

	_spawn_entries_dirty = true;
}

void ESSEntityWorldSpawner3DArea::_spawn() {
	// force spawns in all empty slots

	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (!is_inside_tree()) {
		return;
	}

	if (_spawn_entries_dirty) {
		_recalculate_valid_spawn_entries();
	}

	if (_spawn_slots_dirty) {
		_recalculate_slots();
	}

	for (int i = 0; i < _spawn_slots.size(); ++i) {
		const SpawnSlot &e = _spawn_slots.write[i];

		if (e.entity == ObjectID()) {
			_spawn_slot(i);
		}
	}
}

ESSEntityWorldSpawner3DArea::ESSEntityWorldSpawner3DArea() {
	_spawn_slot_count = 0;
	_respawn_time_min = 0;
	_respawn_time_max = 0;
	_spawn_area_extents = Vector3(2, 2, 2);
	_spawn_slots_dirty = true;
	_spawn_entries_dirty = true;
	_spawn_entries_max_chance = 0;
}

ESSEntityWorldSpawner3DArea::~ESSEntityWorldSpawner3DArea() {
}

Vector<Vector2> ESSEntityWorldSpawner3DArea::get_spawn_positions() const {
	Vector<Vector2> pos;

	for (int i = 0; i < _spawn_slots.size(); ++i) {
		const SpawnSlot &e = _spawn_slots[i];

		pos.push_back(e.position);
	}

	return pos;
}

void ESSEntityWorldSpawner3DArea::_on_entity_tree_exited(const ObjectID p_entity_object_id) {
	for (int i = 0; i < _spawn_slots.size(); ++i) {
		SpawnSlot &e = _spawn_slots.write[i];

		if (e.entity == p_entity_object_id) {
			e.entity = ObjectID();

			if (_respawn_time_min > CMP_EPSILON) {
				if (_respawn_time_max > CMP_EPSILON) {
					e.respawn_timer = Math::random(_respawn_time_min, _respawn_time_max);
				} else {
					e.respawn_timer = _respawn_time_min;
				}

				set_process_internal(true);
			}

			return;
		}
	}
}

void ESSEntityWorldSpawner3DArea::_recalculate_slots() {
	_spawn_slots.resize(_spawn_slot_count);

	for (int i = 0; i < _spawn_slots.size(); ++i) {
		SpawnSlot &e = _spawn_slots.write[i];

		e.position = Vector2(Math::random(-_spawn_area_extents.x, _spawn_area_extents.x), Math::random(-_spawn_area_extents.z, _spawn_area_extents.z));
	}

	_spawn_slots_dirty = false;
}

void ESSEntityWorldSpawner3DArea::_recalculate_valid_spawn_entries() {
	_spawn_entries_max_chance = 0;
	_valid_spawn_entries.clear();

	for (int i = 0; i < _spawn_entries.size(); ++i) {
		const SpawnEntry &e = _spawn_entries[i];

		if (e.entity_data.is_valid() && e.spawn_chance > CMP_EPSILON) {
			_valid_spawn_entries.push_back(e);
			_spawn_entries_max_chance += e.spawn_chance;
		}
	}

	_spawn_entries_dirty = false;
}

void ESSEntityWorldSpawner3DArea::_spawn_slot(const int p_index) {
	int spawn_index = generate_spawn_index();

	if (spawn_index == -1) {
		return;
	}

	const SpawnEntry &e = _spawn_entries[spawn_index];

	SpawnSlot &s = _spawn_slots.write[p_index];

	Ref<EntityCreateInfo> info;
	info.instance();

	if (!e.entity_name.empty()) {
		info->set_entity_name(e.entity_name);
	} else {
		info->set_entity_name(get_name());
	}

	info->set_entity_controller(EntityEnums::ENITIY_CONTROLLER_AI);

	int level;

	if (e.level_range.y > 0) {
		level = Math::randomi(e.level_range.x, e.level_range.y);
	} else {
		level = e.level_range.x;
	}

	info->set_level(level);

	Transform offset;
	offset.translate_local(Vector3(s.position.x, 0, s.position.y));

	info->set_transform(get_global_transform() * offset);

	info->set_entity_data(e.entity_data);
	info->set_species_instance(e.entity_data->get_species_instance());
	ESS::get_singleton()->request_entity_spawn(info);

	s.respawn_timer = 0;

	Entity *created_entity = info->get_created_entity();

	if (created_entity != NULL) {
		created_entity->sets_spawner_object_id(get_instance_id());
		s.entity = created_entity->get_instance_id();
		created_entity->connect("tree_exited", this, "_on_entity_tree_exited", varray(s.entity));
	}

	emit_on_entity_spawned(info);
}

int ESSEntityWorldSpawner3DArea::generate_spawn_index() {
	if (_spawn_entries_dirty) {
		_recalculate_valid_spawn_entries();
	}

	float rnd = Math::random(0.0f, _spawn_entries_max_chance);
	float current_spawn_chance = 0;

	for (int i = 0; i < _spawn_entries.size(); ++i) {
		const SpawnEntry &e = _spawn_entries[i];

		current_spawn_chance += e.spawn_chance;

		if (rnd < current_spawn_chance) {
			return i;
		}
	}

	return _spawn_entries.size() - 1;
}

void ESSEntityWorldSpawner3DArea::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			call_deferred("spawn");
		} break;
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _spawn_slots.size(); ++i) {
				SpawnSlot &s = _spawn_slots.write[i];

				if (s.entity != ObjectID()) {
					Entity *ent = ObjectDB::get_instance<Entity>(s.entity);

					if (ent) {
						ent->sets_spawner_object_id(ObjectID());
					}

					s.entity = ObjectID();
				}
			}
		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			float delta = get_tree()->get_idle_process_time();

			bool has_active_timer = false;

			for (int i = 0; i < _spawn_slots.size(); ++i) {
				SpawnSlot &s = _spawn_slots.write[i];

				if (s.respawn_timer > CMP_EPSILON2) {
					s.respawn_timer -= delta;

					if (s.respawn_timer <= 0) {
						s.respawn_timer = 0;

						call_deferred("_spawn_slot", i);
					} else {
						has_active_timer = true;
					}
				}
			}

			if (!has_active_timer) {
				set_process_internal(false);
			}
		} break;
		default:
			break;
	}
}

void ESSEntityWorldSpawner3DArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_spawn_area_extents"), &ESSEntityWorldSpawner3DArea::get_spawn_area_extents);
	ClassDB::bind_method(D_METHOD("set_spawn_area_extents", "value"), &ESSEntityWorldSpawner3DArea::set_spawn_area_extents);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "spawn_area_extents"), "set_spawn_area_extents", "get_spawn_area_extents");

	ClassDB::bind_method(D_METHOD("get_spawn_slot_count"), &ESSEntityWorldSpawner3DArea::get_spawn_slot_count);
	ClassDB::bind_method(D_METHOD("set_spawn_slot_count", "respawn_time"), &ESSEntityWorldSpawner3DArea::set_spawn_slot_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "spawn_slot_count"), "set_spawn_slot_count", "get_spawn_slot_count");

	ClassDB::bind_method(D_METHOD("get_respawn_time_min"), &ESSEntityWorldSpawner3DArea::get_respawn_time_min);
	ClassDB::bind_method(D_METHOD("set_respawn_time_min", "respawn_time"), &ESSEntityWorldSpawner3DArea::set_respawn_time_min);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_min"), "set_respawn_time_min", "get_respawn_time_min");

	ClassDB::bind_method(D_METHOD("get_respawn_time_max"), &ESSEntityWorldSpawner3DArea::get_respawn_time_max);
	ClassDB::bind_method(D_METHOD("set_respawn_time_max", "respawn_time"), &ESSEntityWorldSpawner3DArea::set_respawn_time_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "respawn_time_max"), "set_respawn_time_max", "get_respawn_time_max");

	ClassDB::bind_method(D_METHOD("get_entity_name", "index"), &ESSEntityWorldSpawner3DArea::get_entity_name);
	ClassDB::bind_method(D_METHOD("set_entity_name", "index", "name"), &ESSEntityWorldSpawner3DArea::set_entity_name);

	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &ESSEntityWorldSpawner3DArea::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &ESSEntityWorldSpawner3DArea::set_entity_data);

	ClassDB::bind_method(D_METHOD("get_entity_level_range", "index"), &ESSEntityWorldSpawner3DArea::get_entity_level_range);
	ClassDB::bind_method(D_METHOD("set_entity_level_range", "index", "level_range"), &ESSEntityWorldSpawner3DArea::set_entity_level_range);

	ClassDB::bind_method(D_METHOD("get_entity_spawn_chance", "index"), &ESSEntityWorldSpawner3DArea::get_entity_spawn_chance);
	ClassDB::bind_method(D_METHOD("set_entity_spawn_chance", "index", "spawn_chance"), &ESSEntityWorldSpawner3DArea::set_entity_spawn_chance);

	ClassDB::bind_method(D_METHOD("get_spawn_entry_count"), &ESSEntityWorldSpawner3DArea::get_spawn_entry_count);
	ClassDB::bind_method(D_METHOD("clear_spawn_entries"), &ESSEntityWorldSpawner3DArea::clear_spawn_entries);
	ClassDB::bind_method(D_METHOD("add_spawn_entry", "name", "data", "level_range", "spawn_chance"), &ESSEntityWorldSpawner3DArea::add_spawn_entry);
	ClassDB::bind_method(D_METHOD("remove_spawn_entry", "index"), &ESSEntityWorldSpawner3DArea::remove_spawn_entry);

	ClassDB::bind_method(D_METHOD("_on_entity_tree_exited"), &ESSEntityWorldSpawner3DArea::_on_entity_tree_exited);
	ClassDB::bind_method(D_METHOD("_spawn_slot"), &ESSEntityWorldSpawner3DArea::_spawn_slot);
}

ESSEntityWorldSpawner3DArea::SpawnEntry::SpawnEntry() {
	spawn_chance = 1;
}
ESSEntityWorldSpawner3DArea::SpawnEntry::~SpawnEntry() {
}
