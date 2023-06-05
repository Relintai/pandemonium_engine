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

#ifdef DEBUG_ENABLED
#include "core/config/project_settings.h"
#include "scene/resources/material.h"
#endif

NavigationServer *NavigationServer::singleton = nullptr;

void NavigationServer::init() {
}

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

#ifdef DEBUG_ENABLED
	ADD_SIGNAL(MethodInfo("navigation_debug_changed"));
#endif // DEBUG_ENABLED
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

#ifdef DEBUG_ENABLED
	_debug_enabled = true;
	_debug_dirty = true;

	_debug_navigation_edge_connection_color = GLOBAL_DEF("debug/shapes/navigation/edge_connection_color", Color(1.0, 0.0, 1.0, 1.0));
	_debug_navigation_geometry_edge_color = GLOBAL_DEF("debug/shapes/navigation/geometry_edge_color", Color(0.5, 1.0, 1.0, 1.0));
	_debug_navigation_geometry_face_color = GLOBAL_DEF("debug/shapes/navigation/geometry_face_color", Color(0.5, 1.0, 1.0, 0.4));
	_debug_navigation_geometry_edge_disabled_color = GLOBAL_DEF("debug/shapes/navigation/geometry_edge_disabled_color", Color(0.5, 0.5, 0.5, 1.0));
	_debug_navigation_geometry_face_disabled_color = GLOBAL_DEF("debug/shapes/navigation/geometry_face_disabled_color", Color(0.5, 0.5, 0.5, 0.4));
	_debug_navigation_enable_edge_connections = GLOBAL_DEF("debug/shapes/navigation/enable_edge_connections", true);
	_debug_navigation_enable_edge_connections_xray = GLOBAL_DEF("debug/shapes/navigation/enable_edge_connections_xray", true);
	_debug_navigation_enable_edge_lines = GLOBAL_DEF("debug/shapes/navigation/enable_edge_lines", true);
	_debug_navigation_enable_edge_lines_xray = GLOBAL_DEF("debug/shapes/navigation/enable_edge_lines_xray", true);
	_debug_navigation_enable_geometry_face_random_color = GLOBAL_DEF("debug/shapes/navigation/enable_geometry_face_random_color", true);
#endif // DEBUG_ENABLED
}

NavigationServer::~NavigationServer() {
	singleton = nullptr;
}

#ifdef DEBUG_ENABLED
void NavigationServer::_emit_navigation_debug_changed_signal() {
	if (_debug_dirty) {
		_debug_dirty = false;
		emit_signal("navigation_debug_changed");
	}
}
#endif // DEBUG_ENABLED

#ifdef DEBUG_ENABLED
Ref<SpatialMaterial> NavigationServer::get_debug_navigation_geometry_face_material() {
	if (_debug_navigation_geometry_face_material.is_valid()) {
		return _debug_navigation_geometry_face_material;
	}

	bool enabled_geometry_face_random_color = get_debug_navigation_enable_geometry_face_random_color();

	Color debug_navigation_geometry_face_color = get_debug_navigation_geometry_face_color();

	Ref<SpatialMaterial> face_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	face_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	face_material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	face_material->set_albedo(debug_navigation_geometry_face_color);
	if (enabled_geometry_face_random_color) {
		face_material->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
		face_material->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	}

	_debug_navigation_geometry_face_material = face_material;

	return _debug_navigation_geometry_face_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_geometry_edge_material() {
	if (_debug_navigation_geometry_edge_material.is_valid()) {
		return _debug_navigation_geometry_edge_material;
	}

	bool enabled_edge_lines_xray = get_debug_navigation_enable_edge_lines_xray();

	Color debug_navigation_geometry_edge_color = get_debug_navigation_geometry_edge_color();

	Ref<SpatialMaterial> line_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	line_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	line_material->set_albedo(debug_navigation_geometry_edge_color);
	if (enabled_edge_lines_xray) {
		line_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}

	_debug_navigation_geometry_edge_material = line_material;

	return _debug_navigation_geometry_edge_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_geometry_face_disabled_material() {
	if (_debug_navigation_geometry_face_disabled_material.is_valid()) {
		return _debug_navigation_geometry_face_disabled_material;
	}

	Color debug_navigation_geometry_face_disabled_color = get_debug_navigation_geometry_face_disabled_color();

	Ref<SpatialMaterial> face_disabled_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	face_disabled_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	face_disabled_material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	face_disabled_material->set_albedo(debug_navigation_geometry_face_disabled_color);

	_debug_navigation_geometry_face_disabled_material = face_disabled_material;

	return _debug_navigation_geometry_face_disabled_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_geometry_edge_disabled_material() {
	if (_debug_navigation_geometry_edge_disabled_material.is_valid()) {
		return _debug_navigation_geometry_edge_disabled_material;
	}

	bool enabled_edge_lines_xray = get_debug_navigation_enable_edge_lines_xray();

	Color debug_navigation_geometry_edge_disabled_color = get_debug_navigation_geometry_edge_disabled_color();

	Ref<SpatialMaterial> line_disabled_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	line_disabled_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	line_disabled_material->set_albedo(debug_navigation_geometry_edge_disabled_color);
	if (enabled_edge_lines_xray) {
		line_disabled_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}

	_debug_navigation_geometry_edge_disabled_material = line_disabled_material;

	return _debug_navigation_geometry_edge_disabled_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_edge_connections_material() {
	if (_debug_navigation_edge_connections_material.is_valid()) {
		return _debug_navigation_edge_connections_material;
	}

	bool enabled_edge_connections_xray = get_debug_navigation_enable_edge_connections_xray();

	Color debug_navigation_edge_connection_color = get_debug_navigation_edge_connection_color();

	Ref<SpatialMaterial> edge_connections_material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	edge_connections_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	edge_connections_material->set_albedo(debug_navigation_edge_connection_color);
	if (enabled_edge_connections_xray) {
		edge_connections_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}
	edge_connections_material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MAX - 2);

	_debug_navigation_edge_connections_material = edge_connections_material;

	return _debug_navigation_edge_connections_material;
}

void NavigationServer::set_debug_navigation_edge_connection_color(const Color &p_color) {
	_debug_navigation_edge_connection_color = p_color;
	if (_debug_navigation_edge_connections_material.is_valid()) {
		_debug_navigation_edge_connections_material->set_albedo(_debug_navigation_edge_connection_color);
	}
}

Color NavigationServer::get_debug_navigation_edge_connection_color() const {
	return _debug_navigation_edge_connection_color;
}

void NavigationServer::set_debug_navigation_geometry_edge_color(const Color &p_color) {
	_debug_navigation_geometry_edge_color = p_color;
	if (_debug_navigation_geometry_edge_material.is_valid()) {
		_debug_navigation_geometry_edge_material->set_albedo(_debug_navigation_geometry_edge_color);
	}
}

Color NavigationServer::get_debug_navigation_geometry_edge_color() const {
	return _debug_navigation_geometry_edge_color;
}

void NavigationServer::set_debug_navigation_geometry_face_color(const Color &p_color) {
	_debug_navigation_geometry_face_color = p_color;
	if (_debug_navigation_geometry_face_material.is_valid()) {
		_debug_navigation_geometry_face_material->set_albedo(_debug_navigation_geometry_face_color);
	}
}

Color NavigationServer::get_debug_navigation_geometry_face_color() const {
	return _debug_navigation_geometry_face_color;
}

void NavigationServer::set_debug_navigation_geometry_edge_disabled_color(const Color &p_color) {
	_debug_navigation_geometry_edge_disabled_color = p_color;
	if (_debug_navigation_geometry_edge_disabled_material.is_valid()) {
		_debug_navigation_geometry_edge_disabled_material->set_albedo(_debug_navigation_geometry_edge_disabled_color);
	}
}

Color NavigationServer::get_debug_navigation_geometry_edge_disabled_color() const {
	return _debug_navigation_geometry_edge_disabled_color;
}

void NavigationServer::set_debug_navigation_geometry_face_disabled_color(const Color &p_color) {
	_debug_navigation_geometry_face_disabled_color = p_color;
	if (_debug_navigation_geometry_face_disabled_material.is_valid()) {
		_debug_navigation_geometry_face_disabled_material->set_albedo(_debug_navigation_geometry_face_disabled_color);
	}
}

Color NavigationServer::get_debug_navigation_geometry_face_disabled_color() const {
	return _debug_navigation_geometry_face_disabled_color;
}

void NavigationServer::set_debug_navigation_enable_edge_connections(const bool p_value) {
	_debug_navigation_enable_edge_connections = p_value;
	_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enable_edge_connections() const {
	return _debug_navigation_enable_edge_connections;
}

void NavigationServer::set_debug_navigation_enable_edge_connections_xray(const bool p_value) {
	_debug_navigation_enable_edge_connections_xray = p_value;
	if (_debug_navigation_edge_connections_material.is_valid()) {
		_debug_navigation_edge_connections_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, _debug_navigation_enable_edge_connections_xray);
	}
}

bool NavigationServer::get_debug_navigation_enable_edge_connections_xray() const {
	return _debug_navigation_enable_edge_connections_xray;
}

void NavigationServer::set_debug_navigation_enable_edge_lines(const bool p_value) {
	_debug_navigation_enable_edge_lines = p_value;
	_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enable_edge_lines() const {
	return _debug_navigation_enable_edge_lines;
}

void NavigationServer::set_debug_navigation_enable_edge_lines_xray(const bool p_value) {
	_debug_navigation_enable_edge_lines_xray = p_value;
	if (_debug_navigation_geometry_edge_material.is_valid()) {
		_debug_navigation_geometry_edge_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, _debug_navigation_enable_edge_lines_xray);
	}
}

bool NavigationServer::get_debug_navigation_enable_edge_lines_xray() const {
	return _debug_navigation_enable_edge_lines_xray;
}

void NavigationServer::set_debug_navigation_enable_geometry_face_random_color(const bool p_value) {
	_debug_navigation_enable_geometry_face_random_color = p_value;
	_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enable_geometry_face_random_color() const {
	return _debug_navigation_enable_geometry_face_random_color;
}

void NavigationServer::set_debug_enabled(bool p_enabled) {
	if (_debug_enabled != p_enabled) {
		_debug_dirty = true;
	}

	_debug_enabled = p_enabled;

	if (_debug_dirty) {
		call_deferred("_emit_navigation_debug_changed_signal");
	}
}

bool NavigationServer::get_debug_enabled() const {
	return _debug_enabled;
}
#endif // DEBUG_ENABLED

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
