/*************************************************************************/
/*  ess_entity_world_spawner_3d_single.cpp                               */
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

#include "ess_entity_world_spawner_3d_single.h"

#include "../data/species/entity_species_data.h"
#include "../data/species/species_instance.h"
#include "../entities/data/entity_data.h"
#include "../entity_enums.h"
#include "../utility/entity_create_info.h"

#include "core/config/engine.h"

#include "../singletons/ess.h"

String ESSEntityWorldSpawner3DSingle::get_entity_name() const {
	return _entity_name;
}
void ESSEntityWorldSpawner3DSingle::set_entity_name(const String &p_name) {
	_entity_name = p_name;
}

Ref<EntityData> ESSEntityWorldSpawner3DSingle::get_entity_data() const {
	return _entity_data;
}
void ESSEntityWorldSpawner3DSingle::set_entity_data(const Ref<EntityData> &p_data) {
	_entity_data = p_data;
}

int ESSEntityWorldSpawner3DSingle::get_level() const {
	return _level;
}
void ESSEntityWorldSpawner3DSingle::set_level(const int p_level) {
	_level = p_level;
}

void ESSEntityWorldSpawner3DSingle::_spawn() {
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

	emit_on_entity_spawned(info);
}

ESSEntityWorldSpawner3DSingle::ESSEntityWorldSpawner3DSingle() {
	_level = 1;
}

ESSEntityWorldSpawner3DSingle::~ESSEntityWorldSpawner3DSingle() {
}

void ESSEntityWorldSpawner3DSingle::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			call_deferred("spawn");
			break;

		default:
			break;
	}
}

void ESSEntityWorldSpawner3DSingle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_name"), &ESSEntityWorldSpawner3DSingle::get_entity_name);
	ClassDB::bind_method(D_METHOD("set_entity_name", "value"), &ESSEntityWorldSpawner3DSingle::set_entity_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "entity_name"), "set_entity_name", "get_entity_name");

	ClassDB::bind_method(D_METHOD("get_entity_data"), &ESSEntityWorldSpawner3DSingle::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "value"), &ESSEntityWorldSpawner3DSingle::set_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "set_entity_data", "get_entity_data");

	ClassDB::bind_method(D_METHOD("get_level"), &ESSEntityWorldSpawner3DSingle::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &ESSEntityWorldSpawner3DSingle::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
