/*************************************************************************/
/*  navigation_server.cpp                                                */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "navigation_server.h"

#include "core/config/project_settings.h"
#include "scene/3d/navigation_mesh_instance.h"
#include "scene/resources/navigation_mesh.h"

NavigationServer *NavigationServer::singleton = nullptr;

void NavigationServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_maps"), &NavigationServer::get_maps);

	ClassDB::bind_method(D_METHOD("map_create"), &NavigationServer::map_create);
	ClassDB::bind_method(D_METHOD("map_set_active", "map", "active"), &NavigationServer::map_set_active);
	ClassDB::bind_method(D_METHOD("map_is_active", "map"), &NavigationServer::map_is_active);
	ClassDB::bind_method(D_METHOD("map_set_up", "map", "up"), &NavigationServer::map_set_up);
	ClassDB::bind_method(D_METHOD("map_get_up", "map"), &NavigationServer::map_get_up);
	ClassDB::bind_method(D_METHOD("map_set_cell_size", "map", "cell_size"), &NavigationServer::map_set_cell_size);
	ClassDB::bind_method(D_METHOD("map_get_cell_size", "map"), &NavigationServer::map_get_cell_size);
	ClassDB::bind_method(D_METHOD("map_set_cell_height", "map", "cell_height"), &NavigationServer::map_set_cell_height);
	ClassDB::bind_method(D_METHOD("map_get_cell_height", "map"), &NavigationServer::map_get_cell_height);
	ClassDB::bind_method(D_METHOD("map_set_edge_connection_margin", "map", "margin"), &NavigationServer::map_set_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_get_edge_connection_margin", "map"), &NavigationServer::map_get_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_get_path", "map", "origin", "destination", "optimize", "navigation_layers"), &NavigationServer::map_get_path, 1);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_to_segment", "map", "start", "end", "use_collision"), &NavigationServer::map_get_closest_point_to_segment, false);
	ClassDB::bind_method(D_METHOD("map_get_closest_point", "map", "to_point"), &NavigationServer::map_get_closest_point);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_normal", "map", "to_point"), &NavigationServer::map_get_closest_point_normal);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_owner", "map", "to_point"), &NavigationServer::map_get_closest_point_owner);

	ClassDB::bind_method(D_METHOD("map_get_regions", "map"), &NavigationServer::map_get_regions);
	ClassDB::bind_method(D_METHOD("map_get_agents", "map"), &NavigationServer::map_get_agents);
	ClassDB::bind_method(D_METHOD("map_force_update", "map"), &NavigationServer::map_force_update);

	ClassDB::bind_method(D_METHOD("region_create"), &NavigationServer::region_create);
	ClassDB::bind_method(D_METHOD("region_set_enter_cost", "region", "enter_cost"), &NavigationServer::region_set_enter_cost);
	ClassDB::bind_method(D_METHOD("region_get_enter_cost", "region"), &NavigationServer::region_get_enter_cost);
	ClassDB::bind_method(D_METHOD("region_set_travel_cost", "region", "travel_cost"), &NavigationServer::region_set_travel_cost);
	ClassDB::bind_method(D_METHOD("region_get_travel_cost", "region"), &NavigationServer::region_get_travel_cost);
	ClassDB::bind_method(D_METHOD("region_owns_point", "region", "point"), &NavigationServer::region_owns_point);
	ClassDB::bind_method(D_METHOD("region_set_map", "region", "map"), &NavigationServer::region_set_map);
	ClassDB::bind_method(D_METHOD("region_get_map", "region"), &NavigationServer::region_get_map);

	ClassDB::bind_method(D_METHOD("region_set_navigation_layers", "region", "navigation_layers"), &NavigationServer::region_set_navigation_layers);
	ClassDB::bind_method(D_METHOD("region_get_navigation_layers", "region"), &NavigationServer::region_get_navigation_layers);

	ClassDB::bind_method(D_METHOD("region_set_transform", "region", "transform"), &NavigationServer::region_set_transform);
	ClassDB::bind_method(D_METHOD("region_set_navmesh", "region", "nav_mesh"), &NavigationServer::region_set_navmesh);
	ClassDB::bind_method(D_METHOD("region_bake_navmesh", "mesh", "node"), &NavigationServer::region_bake_navmesh);

	ClassDB::bind_method(D_METHOD("region_get_connections_count", "region"), &NavigationServer::region_get_connections_count);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_start", "region", "connection"), &NavigationServer::region_get_connection_pathway_start);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_end", "region", "connection"), &NavigationServer::region_get_connection_pathway_end);

	ClassDB::bind_method(D_METHOD("agent_create"), &NavigationServer::agent_create);
	ClassDB::bind_method(D_METHOD("agent_set_map", "agent", "map"), &NavigationServer::agent_set_map);
	ClassDB::bind_method(D_METHOD("agent_get_map", "agent"), &NavigationServer::agent_get_map);
	ClassDB::bind_method(D_METHOD("agent_set_neighbor_dist", "agent", "dist"), &NavigationServer::agent_set_neighbor_dist);
	ClassDB::bind_method(D_METHOD("agent_set_max_neighbors", "agent", "count"), &NavigationServer::agent_set_max_neighbors);
	ClassDB::bind_method(D_METHOD("agent_set_time_horizon", "agent", "time"), &NavigationServer::agent_set_time_horizon);
	ClassDB::bind_method(D_METHOD("agent_set_radius", "agent", "radius"), &NavigationServer::agent_set_radius);
	ClassDB::bind_method(D_METHOD("agent_set_max_speed", "agent", "max_speed"), &NavigationServer::agent_set_max_speed);
	ClassDB::bind_method(D_METHOD("agent_set_velocity", "agent", "velocity"), &NavigationServer::agent_set_velocity);
	ClassDB::bind_method(D_METHOD("agent_set_target_velocity", "agent", "target_velocity"), &NavigationServer::agent_set_target_velocity);
	ClassDB::bind_method(D_METHOD("agent_set_position", "agent", "position"), &NavigationServer::agent_set_position);
	ClassDB::bind_method(D_METHOD("agent_is_map_changed", "agent"), &NavigationServer::agent_is_map_changed);
	ClassDB::bind_method(D_METHOD("agent_set_callback", "agent", "object_id", "method", "userdata"), &NavigationServer::agent_set_callback, DEFVAL(Variant()));

	ClassDB::bind_method(D_METHOD("free_rid", "rid"), &NavigationServer::free);

	ClassDB::bind_method(D_METHOD("set_active", "active"), &NavigationServer::set_active);
	ClassDB::bind_method(D_METHOD("process", "delta_time"), &NavigationServer::process);

	ADD_SIGNAL(MethodInfo("map_changed", PropertyInfo(Variant::RID, "map")));
}

const NavigationServer *NavigationServer::get_singleton() {
	return singleton;
}

NavigationServer *NavigationServer::get_singleton_mut() {
	return singleton;
}

NavigationServer::NavigationServer() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

NavigationServer::~NavigationServer() {
	singleton = nullptr;
}

Vector<NavigationServerManager::ClassInfo> NavigationServerManager::navigation_servers;
int NavigationServerManager::default_server_id = -1;
int NavigationServerManager::default_server_priority = -1;
const String NavigationServerManager::setting_property_name("navigation/3d/navigation_engine");

void NavigationServerManager::on_servers_changed() {
	String navigation_servers2("DEFAULT");
	for (int i = get_servers_count() - 1; 0 <= i; --i) {
		navigation_servers2 += "," + get_server_name(i);
	}
	ProjectSettings::get_singleton()->set_custom_property_info(setting_property_name, PropertyInfo(Variant::STRING, setting_property_name, PROPERTY_HINT_ENUM, navigation_servers2));
}

void NavigationServerManager::register_server(const String &p_name, CreateNavigationServerCallback p_creat_callback) {
	ERR_FAIL_COND(!p_creat_callback);
	ERR_FAIL_COND(find_server_id(p_name) != -1);
	navigation_servers.push_back(ClassInfo(p_name, p_creat_callback));
	on_servers_changed();
}

void NavigationServerManager::set_default_server(const String &p_name, int p_priority) {
	const int id = find_server_id(p_name);
	ERR_FAIL_COND(id == -1); // Not found
	if (default_server_priority < p_priority) {
		default_server_id = id;
		default_server_priority = p_priority;
	}
}

int NavigationServerManager::find_server_id(const String &p_name) {
	for (int i = navigation_servers.size() - 1; 0 <= i; --i) {
		if (p_name == navigation_servers[i].name) {
			return i;
		}
	}
	return -1;
}

int NavigationServerManager::get_servers_count() {
	return navigation_servers.size();
}

String NavigationServerManager::get_server_name(int p_id) {
	ERR_FAIL_INDEX_V(p_id, get_servers_count(), "");
	return navigation_servers[p_id].name;
}

NavigationServer *NavigationServerManager::new_default_server() {
	ERR_FAIL_COND_V(default_server_id == -1, nullptr);
	return navigation_servers[default_server_id].create_callback();
}

NavigationServer *NavigationServerManager::new_server(const String &p_name) {
	int id = find_server_id(p_name);
	if (id == -1) {
		return nullptr;
	} else {
		return navigation_servers[id].create_callback();
	}
}