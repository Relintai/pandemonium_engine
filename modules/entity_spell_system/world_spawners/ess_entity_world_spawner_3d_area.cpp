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

String ESSEntityWorldSpawner3DArea::get_entity_name() const {
	return _entity_name;
}
void ESSEntityWorldSpawner3DArea::set_entity_name(const String &p_name) {
	_entity_name = p_name;
}

Ref<EntityData> ESSEntityWorldSpawner3DArea::get_entity_data() const {
	return _entity_data;
}
void ESSEntityWorldSpawner3DArea::set_entity_data(const Ref<EntityData> &p_data) {
	_entity_data = p_data;
}

int ESSEntityWorldSpawner3DArea::get_level() const {
	return _level;
}
void ESSEntityWorldSpawner3DArea::set_level(const int p_level) {
	_level = p_level;
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

void ESSEntityWorldSpawner3DArea::_spawn() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (!_entity_data.is_valid()) {
		return;
	}

	if (!is_inside_tree()) {
		return;
	}

	Ref<EntityCreateInfo> info;
	info.instance();

	if (!_entity_name.empty()) {
		info->set_entity_name(_entity_name);
	} else {
		info->set_entity_name(get_name());
	}

	info->set_entity_controller(EntityEnums::ENITIY_CONTROLLER_AI);
	info->set_level(_level);
	info->set_transform(get_global_transform());
	info->set_entity_data(_entity_data);
	info->set_species_instance(_entity_data->get_species_instance());
	ESS::get_singleton()->request_entity_spawn(info);

	Entity *created_entity = info->get_created_entity();

	if (created_entity != NULL) {
		created_entity->sets_spawner_object_id(get_instance_id());
		_entity = created_entity->get_instance_id();
		created_entity->connect("tree_exited", this, "_on_entity_tree_exited");
	}

	emit_on_entity_spawned(info);
}

ESSEntityWorldSpawner3DArea::ESSEntityWorldSpawner3DArea() {
	_level = 1;
	_entity = ObjectID();
	_respawn_time_min = 0;
	_respawn_time_max = 0;
	_respawn_timer = 0;
}

ESSEntityWorldSpawner3DArea::~ESSEntityWorldSpawner3DArea() {
}

void ESSEntityWorldSpawner3DArea::_on_entity_tree_exited() {
	_entity = ObjectID();

	if (_respawn_time_min > CMP_EPSILON) {
		if (_respawn_time_max > CMP_EPSILON) {
			_respawn_timer = Math::random(_respawn_time_min, _respawn_time_max);
		} else {
			_respawn_timer = _respawn_time_min;
		}

		set_process_internal(true);
	}
}

void ESSEntityWorldSpawner3DArea::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			call_deferred("spawn");
			break;
		case NOTIFICATION_EXIT_TREE:
			if (_entity != ObjectID()) {
				Entity *ent = ObjectDB::get_instance<Entity>(_entity);

				if (ent) {
					ent->sets_spawner_object_id(ObjectID());
				}
			}
			break;
		case NOTIFICATION_INTERNAL_PROCESS:
			_respawn_timer -= get_tree()->get_idle_process_time();

			if (_respawn_timer <= 0) {
				_respawn_timer = 0;
				set_process_internal(false);
				call_deferred("spawn");
			}

			break;
		default:
			break;
	}
}

void ESSEntityWorldSpawner3DArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_name"), &ESSEntityWorldSpawner3DArea::get_entity_name);
	ClassDB::bind_method(D_METHOD("set_entity_name", "value"), &ESSEntityWorldSpawner3DArea::set_entity_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "entity_name"), "set_entity_name", "get_entity_name");

	ClassDB::bind_method(D_METHOD("get_entity_data"), &ESSEntityWorldSpawner3DArea::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "value"), &ESSEntityWorldSpawner3DArea::set_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "set_entity_data", "get_entity_data");

	ClassDB::bind_method(D_METHOD("get_level"), &ESSEntityWorldSpawner3DArea::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &ESSEntityWorldSpawner3DArea::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");

	ClassDB::bind_method(D_METHOD("get_respawn_time_min"), &ESSEntityWorldSpawner3DArea::get_respawn_time_min);
	ClassDB::bind_method(D_METHOD("set_respawn_time_min", "respawn_time"), &ESSEntityWorldSpawner3DArea::set_respawn_time_min);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "respawn_time_min"), "set_respawn_time_min", "get_respawn_time_min");

	ClassDB::bind_method(D_METHOD("get_respawn_time_max"), &ESSEntityWorldSpawner3DArea::get_respawn_time_max);
	ClassDB::bind_method(D_METHOD("set_respawn_time_max", "respawn_time"), &ESSEntityWorldSpawner3DArea::set_respawn_time_max);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "respawn_time_max"), "set_respawn_time_max", "get_respawn_time_max");

	ClassDB::bind_method(D_METHOD("_on_entity_tree_exited"), &ESSEntityWorldSpawner3DArea::_on_entity_tree_exited);
}
