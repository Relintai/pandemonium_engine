/*************************************************************************/
/*  ess_entity_world_spawner_2d.cpp                                      */
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

#include "ess_entity_world_spawner_2d.h"

#include "../utility/entity_create_info.h"

void ESSEntityWorldSpawner2D::spawn() {
	call("_spawn");
}
void ESSEntityWorldSpawner2D::_spawn() {
}

void ESSEntityWorldSpawner2D::emit_on_entity_spawned(const Ref<EntityCreateInfo> &p_info) {
	emit_signal("on_entity_spawned", p_info);
}

ESSEntityWorldSpawner2D::ESSEntityWorldSpawner2D() {
}

ESSEntityWorldSpawner2D::~ESSEntityWorldSpawner2D() {
}

void ESSEntityWorldSpawner2D::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_spawn"));
	ADD_SIGNAL(MethodInfo("emit_on_entity_spawned", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "EntityCreateInfo")));

	ClassDB::bind_method(D_METHOD("spawn"), &ESSEntityWorldSpawner2D::spawn);
	ClassDB::bind_method(D_METHOD("_spawn"), &ESSEntityWorldSpawner2D::_spawn);
	ClassDB::bind_method(D_METHOD("emit_on_entity_spawned", "info"), &ESSEntityWorldSpawner2D::emit_on_entity_spawned);
}
