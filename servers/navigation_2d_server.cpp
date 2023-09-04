/*************************************************************************/
/*  navigation_2d_server.cpp                                             */
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

#include "servers/navigation_2d_server.h"

#include "core/config/project_settings.h"
#include "navigation/navigation_path_query_parameters_2d.h"
#include "navigation/navigation_path_query_result_2d.h"
#include "servers/navigation_server.h"

Navigation2DServer *Navigation2DServer::singleton = nullptr;

void Navigation2DServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_maps"), &Navigation2DServer::get_maps);

	ClassDB::bind_method(D_METHOD("map_create"), &Navigation2DServer::map_create);
	ClassDB::bind_method(D_METHOD("map_set_active", "map", "active"), &Navigation2DServer::map_set_active);
	ClassDB::bind_method(D_METHOD("map_is_active", "map"), &Navigation2DServer::map_is_active);
	ClassDB::bind_method(D_METHOD("map_set_cell_size", "map", "cell_size"), &Navigation2DServer::map_set_cell_size);
	ClassDB::bind_method(D_METHOD("map_get_cell_size", "map"), &Navigation2DServer::map_get_cell_size);
	ClassDB::bind_method(D_METHOD("map_set_use_edge_connections", "map", "enabled"), &Navigation2DServer::map_set_use_edge_connections);
	ClassDB::bind_method(D_METHOD("map_get_use_edge_connections", "map"), &Navigation2DServer::map_get_use_edge_connections);
	ClassDB::bind_method(D_METHOD("map_set_cell_height", "map", "cell_height"), &Navigation2DServer::map_set_cell_size);
	ClassDB::bind_method(D_METHOD("map_get_cell_height", "map"), &Navigation2DServer::map_get_cell_size);
	ClassDB::bind_method(D_METHOD("map_set_edge_connection_margin", "map", "margin"), &Navigation2DServer::map_set_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_get_edge_connection_margin", "map"), &Navigation2DServer::map_get_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_set_link_connection_radius", "map", "radius"), &Navigation2DServer::map_set_link_connection_radius);
	ClassDB::bind_method(D_METHOD("map_get_link_connection_radius", "map"), &Navigation2DServer::map_get_link_connection_radius);
	ClassDB::bind_method(D_METHOD("map_get_path", "map", "origin", "destination", "optimize", "navigation_layers"), &Navigation2DServer::map_get_path, 1);
	ClassDB::bind_method(D_METHOD("map_get_closest_point", "map", "to_point"), &Navigation2DServer::map_get_closest_point);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_owner", "map", "to_point"), &Navigation2DServer::map_get_closest_point_owner);

	ClassDB::bind_method(D_METHOD("map_get_links", "map"), &Navigation2DServer::map_get_links);
	ClassDB::bind_method(D_METHOD("map_get_regions", "map"), &Navigation2DServer::map_get_regions);
	ClassDB::bind_method(D_METHOD("map_get_agents", "map"), &Navigation2DServer::map_get_agents);
	ClassDB::bind_method(D_METHOD("map_get_obstacles", "map"), &Navigation2DServer::map_get_obstacles);
	ClassDB::bind_method(D_METHOD("map_force_update", "map"), &Navigation2DServer::map_force_update);

	ClassDB::bind_method(D_METHOD("region_create"), &Navigation2DServer::region_create);
	ClassDB::bind_method(D_METHOD("region_set_use_edge_connections", "region", "enabled"), &Navigation2DServer::region_set_use_edge_connections);
	ClassDB::bind_method(D_METHOD("region_get_use_edge_connections", "region"), &Navigation2DServer::region_get_use_edge_connections);
	ClassDB::bind_method(D_METHOD("region_set_enter_cost", "region", "enter_cost"), &Navigation2DServer::region_set_enter_cost);
	ClassDB::bind_method(D_METHOD("region_get_enter_cost", "region"), &Navigation2DServer::region_get_enter_cost);
	ClassDB::bind_method(D_METHOD("region_set_travel_cost", "region", "travel_cost"), &Navigation2DServer::region_set_travel_cost);
	ClassDB::bind_method(D_METHOD("region_get_travel_cost", "region"), &Navigation2DServer::region_get_travel_cost);
	ClassDB::bind_method(D_METHOD("region_set_owner_id", "region", "owner_id"), &Navigation2DServer::region_set_owner_id);
	ClassDB::bind_method(D_METHOD("region_get_owner_id", "region"), &Navigation2DServer::region_get_owner_id);
	ClassDB::bind_method(D_METHOD("region_owns_point", "region", "point"), &Navigation2DServer::region_owns_point);
	ClassDB::bind_method(D_METHOD("region_set_map", "region", "map"), &Navigation2DServer::region_set_map);
	ClassDB::bind_method(D_METHOD("region_get_map", "region"), &Navigation2DServer::region_get_map);
	ClassDB::bind_method(D_METHOD("region_set_navigation_layers", "region", "navigation_layers"), &Navigation2DServer::region_set_navigation_layers);
	ClassDB::bind_method(D_METHOD("region_get_navigation_layers", "region"), &Navigation2DServer::region_get_navigation_layers);
	ClassDB::bind_method(D_METHOD("region_set_transform", "region", "transform"), &Navigation2DServer::region_set_transform);
	ClassDB::bind_method(D_METHOD("region_set_navigation_polygon", "region", "nav_poly"), &Navigation2DServer::region_set_navigation_polygon);
	ClassDB::bind_method(D_METHOD("region_get_connections_count", "region"), &Navigation2DServer::region_get_connections_count);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_start", "region", "connection"), &Navigation2DServer::region_get_connection_pathway_start);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_end", "region", "connection"), &Navigation2DServer::region_get_connection_pathway_end);

	ClassDB::bind_method(D_METHOD("link_create"), &Navigation2DServer::link_create);
	ClassDB::bind_method(D_METHOD("link_set_map", "link", "map"), &Navigation2DServer::link_set_map);
	ClassDB::bind_method(D_METHOD("link_get_map", "link"), &Navigation2DServer::link_get_map);
	ClassDB::bind_method(D_METHOD("link_set_bidirectional", "link", "bidirectional"), &Navigation2DServer::link_set_bidirectional);
	ClassDB::bind_method(D_METHOD("link_is_bidirectional", "link"), &Navigation2DServer::link_is_bidirectional);
	ClassDB::bind_method(D_METHOD("link_set_navigation_layers", "link", "navigation_layers"), &Navigation2DServer::link_set_navigation_layers);
	ClassDB::bind_method(D_METHOD("link_get_navigation_layers", "link"), &Navigation2DServer::link_get_navigation_layers);
	ClassDB::bind_method(D_METHOD("link_set_start_position", "link", "position"), &Navigation2DServer::link_set_start_position);
	ClassDB::bind_method(D_METHOD("link_get_start_position", "link"), &Navigation2DServer::link_get_start_position);
	ClassDB::bind_method(D_METHOD("link_set_end_position", "link", "position"), &Navigation2DServer::link_set_end_position);
	ClassDB::bind_method(D_METHOD("link_get_end_position", "link"), &Navigation2DServer::link_get_end_position);
	ClassDB::bind_method(D_METHOD("link_set_enter_cost", "link", "enter_cost"), &Navigation2DServer::link_set_enter_cost);
	ClassDB::bind_method(D_METHOD("link_get_enter_cost", "link"), &Navigation2DServer::link_get_enter_cost);
	ClassDB::bind_method(D_METHOD("link_set_travel_cost", "link", "travel_cost"), &Navigation2DServer::link_set_travel_cost);
	ClassDB::bind_method(D_METHOD("link_get_travel_cost", "link"), &Navigation2DServer::link_get_travel_cost);
	ClassDB::bind_method(D_METHOD("link_set_owner_id", "link", "owner_id"), &Navigation2DServer::link_set_owner_id);
	ClassDB::bind_method(D_METHOD("link_get_owner_id", "link"), &Navigation2DServer::link_get_owner_id);

	ClassDB::bind_method(D_METHOD("agent_create"), &Navigation2DServer::agent_create);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_enabled", "agent", "enabled"), &Navigation2DServer::agent_set_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("agent_get_avoidance_enabled", "agent"), &Navigation2DServer::agent_get_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("agent_set_map", "agent", "map"), &Navigation2DServer::agent_set_map);
	ClassDB::bind_method(D_METHOD("agent_get_map", "agent"), &Navigation2DServer::agent_get_map);
	ClassDB::bind_method(D_METHOD("agent_set_neighbor_distance", "agent", "dist"), &Navigation2DServer::agent_set_neighbor_distance);
	ClassDB::bind_method(D_METHOD("agent_set_max_neighbors", "agent", "count"), &Navigation2DServer::agent_set_max_neighbors);
	ClassDB::bind_method(D_METHOD("agent_set_time_horizon_agents", "agent", "time_horizon"), &Navigation2DServer::agent_set_time_horizon_agents);
	ClassDB::bind_method(D_METHOD("agent_set_time_horizon_obstacles", "agent", "time_horizon"), &Navigation2DServer::agent_set_time_horizon_obstacles);
	ClassDB::bind_method(D_METHOD("agent_set_radius", "agent", "radius"), &Navigation2DServer::agent_set_radius);
	ClassDB::bind_method(D_METHOD("agent_set_max_speed", "agent", "max_speed"), &Navigation2DServer::agent_set_max_speed);
	ClassDB::bind_method(D_METHOD("agent_set_velocity_forced", "agent", "velocity"), &Navigation2DServer::agent_set_velocity_forced);
	ClassDB::bind_method(D_METHOD("agent_set_velocity", "agent", "velocity"), &Navigation2DServer::agent_set_velocity);
	ClassDB::bind_method(D_METHOD("agent_set_position", "agent", "position"), &Navigation2DServer::agent_set_position);
	ClassDB::bind_method(D_METHOD("agent_is_map_changed", "agent"), &Navigation2DServer::agent_is_map_changed);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_callback", "agent", "object_id", "method", "userdata"), &Navigation2DServer::agent_set_avoidance_callback, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_layers", "agent", "layers"), &Navigation2DServer::agent_set_avoidance_layers);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_mask", "agent", "mask"), &Navigation2DServer::agent_set_avoidance_mask);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_priority", "agent", "priority"), &Navigation2DServer::agent_set_avoidance_priority);

	ClassDB::bind_method(D_METHOD("obstacle_create"), &Navigation2DServer::obstacle_create);
	ClassDB::bind_method(D_METHOD("obstacle_set_map", "obstacle", "map"), &Navigation2DServer::obstacle_set_map);
	ClassDB::bind_method(D_METHOD("obstacle_get_map", "obstacle"), &Navigation2DServer::obstacle_get_map);
	ClassDB::bind_method(D_METHOD("obstacle_set_position", "obstacle", "position"), &Navigation2DServer::obstacle_set_position);
	ClassDB::bind_method(D_METHOD("obstacle_set_vertices", "obstacle", "vertices"), &Navigation2DServer::obstacle_set_vertices);
	ClassDB::bind_method(D_METHOD("obstacle_set_avoidance_layers", "obstacle", "layers"), &Navigation2DServer::obstacle_set_avoidance_layers);

	ClassDB::bind_method(D_METHOD("free_rid", "rid"), &Navigation2DServer::free);

	ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &Navigation2DServer::set_debug_enabled);
	ClassDB::bind_method(D_METHOD("get_debug_enabled"), &Navigation2DServer::get_debug_enabled);

#ifdef DEBUG_ENABLED
	ClassDB::bind_method(D_METHOD("_emit_navigation_debug_changed_signal"), &Navigation2DServer::_emit_navigation_debug_changed_signal);
#endif // DEBUG_ENABLED

	ADD_SIGNAL(MethodInfo("map_changed", PropertyInfo(Variant::RID, "map")));
	ADD_SIGNAL(MethodInfo("navigation_debug_changed"));
}

void Navigation2DServer::init() {
#ifdef DEBUG_ENABLED
	NavigationServer::get_singleton()->connect("navigation_debug_changed", this, "_emit_navigation_debug_changed_signal");
#endif // DEBUG_ENABLED
}

void Navigation2DServer::finish() {
}

void Navigation2DServer::query_path(const Ref<NavigationPathQueryParameters2D> &p_query_parameters, Ref<NavigationPathQueryResult2D> p_query_result) const {
	ERR_FAIL_COND(!p_query_parameters.is_valid());
	ERR_FAIL_COND(!p_query_result.is_valid());

	const NavigationUtilities::PathQueryResult2D _query_result = _query_path(p_query_parameters->get_parameters());

	p_query_result->set_path(_query_result.path);
}

Navigation2DServer::Navigation2DServer() {
	singleton = this;
}

Navigation2DServer::~Navigation2DServer() {
	singleton = nullptr;
}

#ifdef DEBUG_ENABLED
void Navigation2DServer::_emit_navigation_debug_changed_signal() {
	emit_signal("navigation_debug_changed");
}
#endif // DEBUG_ENABLED

void Navigation2DServer::set_debug_enabled(bool p_enabled) {
	NavigationServer::get_singleton()->set_debug_enabled(p_enabled);
}
bool Navigation2DServer::get_debug_enabled() const {
	return NavigationServer::get_singleton()->get_debug_enabled();
}

#ifdef DEBUG_ENABLED
void Navigation2DServer::set_debug_navigation_enabled(bool p_enabled) {
	NavigationServer::get_singleton()->set_debug_navigation_enabled(p_enabled);
}

bool Navigation2DServer::get_debug_navigation_enabled() const {
	return NavigationServer::get_singleton()->get_debug_navigation_enabled();
}

void Navigation2DServer::set_debug_avoidance_enabled(bool p_enabled) {
	NavigationServer::get_singleton()->set_debug_avoidance_enabled(p_enabled);
}

bool Navigation2DServer::get_debug_avoidance_enabled() const {
	return NavigationServer::get_singleton()->get_debug_avoidance_enabled();
}

void Navigation2DServer::set_debug_navigation_edge_connection_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_edge_connection_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_edge_connection_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_edge_connection_color();
}

void Navigation2DServer::set_debug_navigation_geometry_face_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_geometry_face_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_geometry_face_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_geometry_face_color();
}

void Navigation2DServer::set_debug_navigation_geometry_face_disabled_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_geometry_face_disabled_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_geometry_face_disabled_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_geometry_face_disabled_color();
}

void Navigation2DServer::set_debug_navigation_geometry_edge_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_geometry_edge_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_geometry_edge_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_geometry_edge_color();
}

void Navigation2DServer::set_debug_navigation_geometry_edge_disabled_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_geometry_edge_disabled_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_geometry_edge_disabled_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_geometry_edge_disabled_color();
}

void Navigation2DServer::set_debug_navigation_enable_edge_connections(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_enable_edge_connections(p_value);
}

bool Navigation2DServer::get_debug_navigation_enable_edge_connections() const {
	return NavigationServer::get_singleton()->get_debug_navigation_enable_edge_connections();
}

void Navigation2DServer::set_debug_navigation_enable_geometry_face_random_color(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_enable_geometry_face_random_color(p_value);
}

bool Navigation2DServer::get_debug_navigation_enable_geometry_face_random_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_enable_geometry_face_random_color();
}

void Navigation2DServer::set_debug_navigation_enable_edge_lines(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_enable_edge_lines(p_value);
}

bool Navigation2DServer::get_debug_navigation_enable_edge_lines() const {
	return NavigationServer::get_singleton()->get_debug_navigation_enable_edge_lines();
}

void Navigation2DServer::set_debug_navigation_link_connection_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_link_connection_color(p_color);
}
Color Navigation2DServer::get_debug_navigation_link_connection_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_link_connection_color();
}

void Navigation2DServer::set_debug_navigation_link_connection_disabled_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_link_connection_disabled_color(p_color);
}
Color Navigation2DServer::get_debug_navigation_link_connection_disabled_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_link_connection_disabled_color();
}

void Navigation2DServer::set_debug_navigation_agent_path_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_agent_path_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_agent_path_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_agent_path_color();
}

void Navigation2DServer::set_debug_navigation_enable_agent_paths(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_enable_agent_paths(p_value);
}

bool Navigation2DServer::get_debug_navigation_enable_agent_paths() const {
	return NavigationServer::get_singleton()->get_debug_navigation_enable_agent_paths();
}

void Navigation2DServer::set_debug_navigation_agent_path_point_size(float p_point_size) {
	NavigationServer::get_singleton()->set_debug_navigation_agent_path_point_size(p_point_size);
}

float Navigation2DServer::get_debug_navigation_agent_path_point_size() const {
	return NavigationServer::get_singleton()->get_debug_navigation_agent_path_point_size();
}

void Navigation2DServer::set_debug_navigation_avoidance_enable_agents_radius(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_enable_agents_radius(p_value);
}

bool Navigation2DServer::get_debug_navigation_avoidance_enable_agents_radius() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_agents_radius();
}

void Navigation2DServer::set_debug_navigation_avoidance_enable_obstacles_radius(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_enable_obstacles_radius(p_value);
}

bool Navigation2DServer::get_debug_navigation_avoidance_enable_obstacles_radius() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_radius();
}

void Navigation2DServer::set_debug_navigation_avoidance_agents_radius_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_agents_radius_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_agents_radius_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_agents_radius_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_obstacles_radius_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_obstacles_radius_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_obstacles_radius_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_obstacles_radius_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_static_obstacle_pushin_face_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushin_face_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_static_obstacle_pushin_face_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_face_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_static_obstacle_pushout_face_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushout_face_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_static_obstacle_pushout_face_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_static_obstacle_pushin_edge_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushin_edge_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_static_obstacle_pushin_edge_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_edge_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_static_obstacle_pushout_edge_color(const Color &p_color) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushout_edge_color(p_color);
}

Color Navigation2DServer::get_debug_navigation_avoidance_static_obstacle_pushout_edge_color() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_edge_color();
}

void Navigation2DServer::set_debug_navigation_avoidance_enable_obstacles_static(const bool p_value) {
	NavigationServer::get_singleton()->set_debug_navigation_avoidance_enable_obstacles_static(p_value);
}

bool Navigation2DServer::get_debug_navigation_avoidance_enable_obstacles_static() const {
	return NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_static();
}

#endif

Vector<Navigation2DServerManager::ClassInfo> Navigation2DServerManager::navigation_servers;
int Navigation2DServerManager::default_server_id = -1;
int Navigation2DServerManager::default_server_priority = -1;
const String Navigation2DServerManager::setting_property_name("navigation/2d/navigation_engine");

void Navigation2DServerManager::on_servers_changed() {
	String navigation_servers2("DEFAULT");
	for (int i = get_servers_count() - 1; 0 <= i; --i) {
		navigation_servers2 += "," + get_server_name(i);
	}
	ProjectSettings::get_singleton()->set_custom_property_info(setting_property_name, PropertyInfo(Variant::STRING, setting_property_name, PROPERTY_HINT_ENUM, navigation_servers2));
}

void Navigation2DServerManager::register_server(const String &p_name, CreateNavigation2DServerCallback p_creat_callback) {
	ERR_FAIL_COND(!p_creat_callback);
	ERR_FAIL_COND(find_server_id(p_name) != -1);
	navigation_servers.push_back(ClassInfo(p_name, p_creat_callback));
	on_servers_changed();
}

void Navigation2DServerManager::set_default_server(const String &p_name, int p_priority) {
	const int id = find_server_id(p_name);
	ERR_FAIL_COND(id == -1); // Not found
	if (default_server_priority < p_priority) {
		default_server_id = id;
		default_server_priority = p_priority;
	}
}

int Navigation2DServerManager::find_server_id(const String &p_name) {
	for (int i = navigation_servers.size() - 1; 0 <= i; --i) {
		if (p_name == navigation_servers[i].name) {
			return i;
		}
	}
	return -1;
}

int Navigation2DServerManager::get_servers_count() {
	return navigation_servers.size();
}

String Navigation2DServerManager::get_server_name(int p_id) {
	ERR_FAIL_INDEX_V(p_id, get_servers_count(), "");
	return navigation_servers[p_id].name;
}

Navigation2DServer *Navigation2DServerManager::new_default_server() {
	ERR_FAIL_COND_V(default_server_id == -1, nullptr);
	return navigation_servers[default_server_id].create_callback();
}

Navigation2DServer *Navigation2DServerManager::new_server(const String &p_name) {
	int id = find_server_id(p_name);
	if (id == -1) {
		return nullptr;
	} else {
		return navigation_servers[id].create_callback();
	}
}