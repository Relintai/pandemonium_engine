/*************************************************************************/
/*  room_group.cpp                                                       */
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

#include "room_group.h"

#include "core/math/geometry.h"
#include "room.h"
#include "room_manager.h"
#include "scene/resources/mesh/mesh.h"
#include "scene/resources/world_3d.h"
#include "servers/rendering_server.h"

void RoomGroup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_roomgroup_priority", "p_priority"), &RoomGroup::set_roomgroup_priority);
	ClassDB::bind_method(D_METHOD("get_roomgroup_priority"), &RoomGroup::get_roomgroup_priority);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "roomgroup_priority", PROPERTY_HINT_RANGE, "-16,16,1", PROPERTY_USAGE_DEFAULT), "set_roomgroup_priority", "get_roomgroup_priority");
}

RoomGroup::RoomGroup() {
	_room_group_rid = RID_PRIME(RenderingServer::get_singleton()->roomgroup_create());
}

RoomGroup::~RoomGroup() {
	if (_room_group_rid != RID()) {
		RenderingServer::get_singleton()->free(_room_group_rid);
	}
}

String RoomGroup::get_configuration_warning() const {
	String warning = Spatial::get_configuration_warning();

	if (Room::detect_nodes_of_type<RoomManager>(this)) {
		if (!warning.empty()) {
			warning += "\n\n";
		}
		warning += TTR("The RoomManager should not be placed inside a RoomGroup.");
	}

	return warning;
}

void RoomGroup::clear() {
	_roomgroup_ID = -1;
}

void RoomGroup::add_room(Room *p_room) {
	RenderingServer::get_singleton()->roomgroup_add_room(_room_group_rid, p_room->_room_rid);
}

// extra editor links to the room manager to allow unloading
// on change, or re-converting
void RoomGroup::_changed() {
#ifdef TOOLS_ENABLED
	RoomManager *rm = RoomManager::active_room_manager;
	if (!rm) {
		return;
	}

	rm->_rooms_changed("changed RoomGroup " + get_name());
#endif
}

void RoomGroup::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_WORLD: {
			ERR_FAIL_COND(get_world_3d().is_null());
			RenderingServer::get_singleton()->roomgroup_set_scenario(_room_group_rid, get_world_3d()->get_scenario());
		} break;
		case NOTIFICATION_EXIT_WORLD: {
			RenderingServer::get_singleton()->roomgroup_set_scenario(_room_group_rid, RID());
		} break;
	}
}
