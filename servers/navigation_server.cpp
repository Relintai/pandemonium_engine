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
#include "navigation/navigation_path_query_parameters_3d.h"
#include "navigation/navigation_path_query_result_3d.h"
#include "scene/3d/navigation_mesh_instance.h"
#include "scene/resources/navigation_mesh.h"

#ifdef DEBUG_ENABLED
#include "core/config/engine.h"
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
	ClassDB::bind_method(D_METHOD("map_set_use_edge_connections", "map", "enabled"), &NavigationServer::map_set_use_edge_connections);
	ClassDB::bind_method(D_METHOD("map_get_use_edge_connections", "map"), &NavigationServer::map_get_use_edge_connections);
	ClassDB::bind_method(D_METHOD("map_set_cell_height", "map", "cell_height"), &NavigationServer::map_set_cell_height);
	ClassDB::bind_method(D_METHOD("map_get_cell_height", "map"), &NavigationServer::map_get_cell_height);
	ClassDB::bind_method(D_METHOD("map_set_edge_connection_margin", "map", "margin"), &NavigationServer::map_set_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_get_edge_connection_margin", "map"), &NavigationServer::map_get_edge_connection_margin);
	ClassDB::bind_method(D_METHOD("map_set_link_connection_radius", "map", "radius"), &NavigationServer::map_set_link_connection_radius);
	ClassDB::bind_method(D_METHOD("map_get_link_connection_radius", "map"), &NavigationServer::map_get_link_connection_radius);
	ClassDB::bind_method(D_METHOD("map_get_path", "map", "origin", "destination", "optimize", "navigation_layers"), &NavigationServer::map_get_path, 1);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_to_segment", "map", "start", "end", "use_collision"), &NavigationServer::map_get_closest_point_to_segment, false);
	ClassDB::bind_method(D_METHOD("map_get_closest_point", "map", "to_point"), &NavigationServer::map_get_closest_point);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_normal", "map", "to_point"), &NavigationServer::map_get_closest_point_normal);
	ClassDB::bind_method(D_METHOD("map_get_closest_point_owner", "map", "to_point"), &NavigationServer::map_get_closest_point_owner);

	ClassDB::bind_method(D_METHOD("map_get_links", "map"), &NavigationServer::map_get_links);
	ClassDB::bind_method(D_METHOD("map_get_regions", "map"), &NavigationServer::map_get_regions);
	ClassDB::bind_method(D_METHOD("map_get_agents", "map"), &NavigationServer::map_get_agents);
	ClassDB::bind_method(D_METHOD("map_get_obstacles", "map"), &NavigationServer::map_get_obstacles);
	ClassDB::bind_method(D_METHOD("map_force_update", "map"), &NavigationServer::map_force_update);

	ClassDB::bind_method(D_METHOD("query_path", "parameters", "result"), &NavigationServer::query_path);

	ClassDB::bind_method(D_METHOD("region_create"), &NavigationServer::region_create);
	ClassDB::bind_method(D_METHOD("region_set_use_edge_connections", "region", "enabled"), &NavigationServer::region_set_use_edge_connections);
	ClassDB::bind_method(D_METHOD("region_get_use_edge_connections", "region"), &NavigationServer::region_get_use_edge_connections);
	ClassDB::bind_method(D_METHOD("region_set_enter_cost", "region", "enter_cost"), &NavigationServer::region_set_enter_cost);
	ClassDB::bind_method(D_METHOD("region_get_enter_cost", "region"), &NavigationServer::region_get_enter_cost);
	ClassDB::bind_method(D_METHOD("region_set_travel_cost", "region", "travel_cost"), &NavigationServer::region_set_travel_cost);
	ClassDB::bind_method(D_METHOD("region_get_travel_cost", "region"), &NavigationServer::region_get_travel_cost);
	ClassDB::bind_method(D_METHOD("region_set_owner_id", "region", "owner_id"), &NavigationServer::region_set_owner_id);
	ClassDB::bind_method(D_METHOD("region_get_owner_id", "region"), &NavigationServer::region_get_owner_id);
	ClassDB::bind_method(D_METHOD("region_owns_point", "region", "point"), &NavigationServer::region_owns_point);
	ClassDB::bind_method(D_METHOD("region_set_map", "region", "map"), &NavigationServer::region_set_map);
	ClassDB::bind_method(D_METHOD("region_get_map", "region"), &NavigationServer::region_get_map);

	ClassDB::bind_method(D_METHOD("region_set_navigation_layers", "region", "navigation_layers"), &NavigationServer::region_set_navigation_layers);
	ClassDB::bind_method(D_METHOD("region_get_navigation_layers", "region"), &NavigationServer::region_get_navigation_layers);

	ClassDB::bind_method(D_METHOD("region_set_transform", "region", "transform"), &NavigationServer::region_set_transform);
	ClassDB::bind_method(D_METHOD("region_set_navmesh", "region", "nav_mesh"), &NavigationServer::region_set_navmesh);

	ClassDB::bind_method(D_METHOD("region_get_connections_count", "region"), &NavigationServer::region_get_connections_count);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_start", "region", "connection"), &NavigationServer::region_get_connection_pathway_start);
	ClassDB::bind_method(D_METHOD("region_get_connection_pathway_end", "region", "connection"), &NavigationServer::region_get_connection_pathway_end);

	ClassDB::bind_method(D_METHOD("link_create"), &NavigationServer::link_create);
	ClassDB::bind_method(D_METHOD("link_set_map", "link", "map"), &NavigationServer::link_set_map);
	ClassDB::bind_method(D_METHOD("link_get_map", "link"), &NavigationServer::link_get_map);
	ClassDB::bind_method(D_METHOD("link_set_bidirectional", "link", "bidirectional"), &NavigationServer::link_set_bidirectional);
	ClassDB::bind_method(D_METHOD("link_is_bidirectional", "link"), &NavigationServer::link_is_bidirectional);
	ClassDB::bind_method(D_METHOD("link_set_navigation_layers", "link", "navigation_layers"), &NavigationServer::link_set_navigation_layers);
	ClassDB::bind_method(D_METHOD("link_get_navigation_layers", "link"), &NavigationServer::link_get_navigation_layers);
	ClassDB::bind_method(D_METHOD("link_set_start_position", "link", "position"), &NavigationServer::link_set_start_position);
	ClassDB::bind_method(D_METHOD("link_get_start_position", "link"), &NavigationServer::link_get_start_position);
	ClassDB::bind_method(D_METHOD("link_set_end_position", "link", "position"), &NavigationServer::link_set_end_position);
	ClassDB::bind_method(D_METHOD("link_get_end_position", "link"), &NavigationServer::link_get_end_position);
	ClassDB::bind_method(D_METHOD("link_set_enter_cost", "link", "enter_cost"), &NavigationServer::link_set_enter_cost);
	ClassDB::bind_method(D_METHOD("link_get_enter_cost", "link"), &NavigationServer::link_get_enter_cost);
	ClassDB::bind_method(D_METHOD("link_set_travel_cost", "link", "travel_cost"), &NavigationServer::link_set_travel_cost);
	ClassDB::bind_method(D_METHOD("link_get_travel_cost", "link"), &NavigationServer::link_get_travel_cost);
	ClassDB::bind_method(D_METHOD("link_set_owner_id", "link", "owner_id"), &NavigationServer::link_set_owner_id);
	ClassDB::bind_method(D_METHOD("link_get_owner_id", "link"), &NavigationServer::link_get_owner_id);

	ClassDB::bind_method(D_METHOD("agent_create"), &NavigationServer::agent_create);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_enabled", "agent", "enabled"), &NavigationServer::agent_set_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("agent_get_avoidance_enabled", "agent"), &NavigationServer::agent_get_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("agent_set_use_3d_avoidance", "agent", "enabled"), &NavigationServer::agent_set_use_3d_avoidance);
	ClassDB::bind_method(D_METHOD("agent_get_use_3d_avoidance", "agent"), &NavigationServer::agent_get_use_3d_avoidance);

	ClassDB::bind_method(D_METHOD("agent_set_map", "agent", "map"), &NavigationServer::agent_set_map);
	ClassDB::bind_method(D_METHOD("agent_get_map", "agent"), &NavigationServer::agent_get_map);
	ClassDB::bind_method(D_METHOD("agent_set_neighbor_dist", "agent", "dist"), &NavigationServer::agent_set_neighbor_dist);
	ClassDB::bind_method(D_METHOD("agent_set_max_neighbors", "agent", "count"), &NavigationServer::agent_set_max_neighbors);
	ClassDB::bind_method(D_METHOD("agent_set_time_horizon_agents", "agent", "time_horizon"), &NavigationServer::agent_set_time_horizon_agents);
	ClassDB::bind_method(D_METHOD("agent_set_time_horizon_obstacles", "agent", "time_horizon"), &NavigationServer::agent_set_time_horizon_obstacles);
	ClassDB::bind_method(D_METHOD("agent_set_radius", "agent", "radius"), &NavigationServer::agent_set_radius);
	ClassDB::bind_method(D_METHOD("agent_set_height", "agent", "height"), &NavigationServer::agent_set_height);
	ClassDB::bind_method(D_METHOD("agent_set_max_speed", "agent", "max_speed"), &NavigationServer::agent_set_max_speed);
	ClassDB::bind_method(D_METHOD("agent_set_velocity_forced", "agent", "velocity"), &NavigationServer::agent_set_velocity_forced);
	ClassDB::bind_method(D_METHOD("agent_set_velocity", "agent", "velocity"), &NavigationServer::agent_set_velocity);

	ClassDB::bind_method(D_METHOD("agent_set_position", "agent", "position"), &NavigationServer::agent_set_position);
	ClassDB::bind_method(D_METHOD("agent_is_map_changed", "agent"), &NavigationServer::agent_is_map_changed);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_callback", "agent", "object_id", "method", "userdata"), &NavigationServer::agent_set_avoidance_callback, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_layers", "agent", "layers"), &NavigationServer::agent_set_avoidance_layers);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_mask", "agent", "mask"), &NavigationServer::agent_set_avoidance_mask);
	ClassDB::bind_method(D_METHOD("agent_set_avoidance_priority", "agent", "priority"), &NavigationServer::agent_set_avoidance_priority);

	ClassDB::bind_method(D_METHOD("obstacle_create"), &NavigationServer::obstacle_create);
	ClassDB::bind_method(D_METHOD("obstacle_set_map", "obstacle", "map"), &NavigationServer::obstacle_set_map);
	ClassDB::bind_method(D_METHOD("obstacle_get_map", "obstacle"), &NavigationServer::obstacle_get_map);
	ClassDB::bind_method(D_METHOD("obstacle_set_height", "obstacle", "height"), &NavigationServer::obstacle_set_height);
	ClassDB::bind_method(D_METHOD("obstacle_set_position", "obstacle", "position"), &NavigationServer::obstacle_set_position);
	ClassDB::bind_method(D_METHOD("obstacle_set_vertices", "obstacle", "vertices"), &NavigationServer::obstacle_set_vertices);
	ClassDB::bind_method(D_METHOD("obstacle_set_avoidance_layers", "obstacle", "layers"), &NavigationServer::obstacle_set_avoidance_layers);

	ClassDB::bind_method(D_METHOD("free_rid", "rid"), &NavigationServer::free);

	ClassDB::bind_method(D_METHOD("set_active", "active"), &NavigationServer::set_active);
	ClassDB::bind_method(D_METHOD("process", "delta_time"), &NavigationServer::process);

	ClassDB::bind_method(D_METHOD("get_process_info", "process_info"), &NavigationServer::get_process_info);

	ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &NavigationServer::set_debug_enabled);
	ClassDB::bind_method(D_METHOD("get_debug_enabled"), &NavigationServer::get_debug_enabled);

	ADD_SIGNAL(MethodInfo("map_changed", PropertyInfo(Variant::RID, "map")));
	ADD_SIGNAL(MethodInfo("navigation_debug_changed"));
	ADD_SIGNAL(MethodInfo("avoidance_debug_changed"));

	BIND_ENUM_CONSTANT(INFO_ACTIVE_MAPS);
	BIND_ENUM_CONSTANT(INFO_REGION_COUNT);
	BIND_ENUM_CONSTANT(INFO_AGENT_COUNT);
	BIND_ENUM_CONSTANT(INFO_LINK_COUNT);
	BIND_ENUM_CONSTANT(INFO_POLYGON_COUNT);
	BIND_ENUM_CONSTANT(INFO_EDGE_COUNT);
	BIND_ENUM_CONSTANT(INFO_EDGE_MERGE_COUNT);
	BIND_ENUM_CONSTANT(INFO_EDGE_CONNECTION_COUNT);
	BIND_ENUM_CONSTANT(INFO_EDGE_FREE_COUNT);

	GLOBAL_DEF("navigation/avoidance/thread_model/avoidance_use_multiple_threads", true);
	GLOBAL_DEF("navigation/avoidance/thread_model/avoidance_use_high_priority_threads", true);

#ifdef DEBUG_ENABLED
	ClassDB::bind_method(D_METHOD("_emit_navigation_debug_changed_signal"), &NavigationServer::_emit_navigation_debug_changed_signal);
	ClassDB::bind_method(D_METHOD("_emit_avoidance_debug_changed_signal"), &NavigationServer::_emit_avoidance_debug_changed_signal);
#endif // DEBUG_ENABLED
}

NavigationServer *NavigationServer::get_singleton() {
	return singleton;
}

NavigationServer::NavigationServer() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

	GLOBAL_DEF("navigation/3d/use_edge_connections", true);

#ifdef DEBUG_ENABLED
	_debug_enabled = true;
	_debug_dirty = true;

	_debug_navigation_enabled = false;
	_navigation_debug_dirty = true;

	_debug_avoidance_enabled = false;
	_avoidance_debug_dirty = true;

	_debug_navigation_edge_connection_color = GLOBAL_DEF("debug/shapes/navigation/edge_connection_color", Color(1.0, 0.0, 1.0, 1.0));
	_debug_navigation_geometry_edge_color = GLOBAL_DEF("debug/shapes/navigation/geometry_edge_color", Color(0.5, 1.0, 1.0, 1.0));
	_debug_navigation_geometry_face_color = GLOBAL_DEF("debug/shapes/navigation/geometry_face_color", Color(0.5, 1.0, 1.0, 0.4));
	_debug_navigation_geometry_edge_disabled_color = GLOBAL_DEF("debug/shapes/navigation/geometry_edge_disabled_color", Color(0.5, 0.5, 0.5, 1.0));
	_debug_navigation_geometry_face_disabled_color = GLOBAL_DEF("debug/shapes/navigation/geometry_face_disabled_color", Color(0.5, 0.5, 0.5, 0.4));
	_debug_navigation_link_connection_color = GLOBAL_DEF("debug/shapes/navigation/link_connection_color", Color(1.0, 0.5, 1.0, 1.0));
	_debug_navigation_link_connection_disabled_color = GLOBAL_DEF("debug/shapes/navigation/link_connection_disabled_color", Color(0.5, 0.5, 0.5, 1.0));
	_debug_navigation_agent_path_color = GLOBAL_DEF("debug/shapes/navigation/agent_path_color", Color(1.0, 0.0, 0.0, 1.0));

	_debug_navigation_enable_edge_connections = GLOBAL_DEF("debug/shapes/navigation/enable_edge_connections", true);
	_debug_navigation_enable_edge_connections_xray = GLOBAL_DEF("debug/shapes/navigation/enable_edge_connections_xray", true);
	_debug_navigation_enable_edge_lines = GLOBAL_DEF("debug/shapes/navigation/enable_edge_lines", true);
	_debug_navigation_enable_edge_lines_xray = GLOBAL_DEF("debug/shapes/navigation/enable_edge_lines_xray", true);
	_debug_navigation_enable_geometry_face_random_color = GLOBAL_DEF("debug/shapes/navigation/enable_geometry_face_random_color", true);
	_debug_navigation_enable_link_connections = GLOBAL_DEF("debug/shapes/navigation/enable_link_connections", true);
	_debug_navigation_enable_link_connections_xray = GLOBAL_DEF("debug/shapes/navigation/enable_link_connections_xray", true);

	_debug_navigation_enable_agent_paths = GLOBAL_DEF("debug/shapes/navigation/enable_agent_paths", true);
	_debug_navigation_enable_agent_paths_xray = GLOBAL_DEF("debug/shapes/navigation/enable_agent_paths_xray", true);

	_debug_navigation_agent_path_point_size = GLOBAL_DEF("debug/shapes/navigation/agent_path_point_size", 4.0);

	_debug_navigation_avoidance_agents_radius_color = GLOBAL_DEF("debug/shapes/avoidance/agents_radius_color", Color(1.0, 1.0, 0.0, 0.25));
	_debug_navigation_avoidance_obstacles_radius_color = GLOBAL_DEF("debug/shapes/avoidance/obstacles_radius_color", Color(1.0, 0.5, 0.0, 0.25));
	_debug_navigation_avoidance_static_obstacle_pushin_face_color = GLOBAL_DEF("debug/shapes/avoidance/obstacles_static_face_pushin_color", Color(1.0, 0.0, 0.0, 0.0));
	_debug_navigation_avoidance_static_obstacle_pushin_edge_color = GLOBAL_DEF("debug/shapes/avoidance/obstacles_static_edge_pushin_color", Color(1.0, 0.0, 0.0, 1.0));
	_debug_navigation_avoidance_static_obstacle_pushout_face_color = GLOBAL_DEF("debug/shapes/avoidance/obstacles_static_face_pushout_color", Color(1.0, 1.0, 0.0, 0.5));
	_debug_navigation_avoidance_static_obstacle_pushout_edge_color = GLOBAL_DEF("debug/shapes/avoidance/obstacles_static_edge_pushout_color", Color(1.0, 1.0, 0.0, 1.0));
	_debug_navigation_avoidance_enable_agents_radius = GLOBAL_DEF("debug/shapes/avoidance/enable_agents_radius", true);
	_debug_navigation_avoidance_enable_obstacles_radius = GLOBAL_DEF("debug/shapes/avoidance/enable_obstacles_radius", true);
	_debug_navigation_avoidance_enable_obstacles_static = GLOBAL_DEF("debug/shapes/avoidance/enable_obstacles_static", true);

	if (Engine::get_singleton()->is_editor_hint()) {
		// enable NavigationServer when in Editor or else navmesh edge connections are invisible
		// on runtime tests SceneTree has "Visible Navigation" set and main iteration takes care of this
		set_debug_enabled(true);
		set_debug_navigation_enabled(true);
		set_debug_avoidance_enabled(true);
	}
#endif // DEBUG_ENABLED
}

NavigationServer::~NavigationServer() {
	singleton = nullptr;
}

void NavigationServer::set_debug_enabled(bool p_enabled) {
#ifdef DEBUG_ENABLED
	if (_debug_enabled != p_enabled) {
		_debug_dirty = true;
	}

	_debug_enabled = p_enabled;

	if (_debug_dirty) {
		call_deferred("_emit_navigation_debug_changed_signal");
	}
#endif
}

bool NavigationServer::get_debug_enabled() const {
#ifdef DEBUG_ENABLED
	return _debug_enabled;
#else
	return false;
#endif
}

#ifdef DEBUG_ENABLED
void NavigationServer::_emit_navigation_debug_changed_signal() {
	if (_navigation_debug_dirty) {
		_navigation_debug_dirty = false;
		emit_signal("navigation_debug_changed");
	}
}

void NavigationServer::_emit_avoidance_debug_changed_signal() {
	if (_avoidance_debug_dirty) {
		_avoidance_debug_dirty = false;
		emit_signal("avoidance_debug_changed");
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
	face_material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
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

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_link_connections_material() {
	if (_debug_navigation_link_connections_material.is_valid()) {
		return _debug_navigation_link_connections_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);

	material->set_albedo(_debug_navigation_link_connection_color);
	if (_debug_navigation_enable_link_connections_xray) {
		material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MAX - 2);

	_debug_navigation_link_connections_material = material;
	return _debug_navigation_link_connections_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_link_connections_disabled_material() {
	if (_debug_navigation_link_connections_disabled_material.is_valid()) {
		return _debug_navigation_link_connections_disabled_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);

	material->set_albedo(_debug_navigation_link_connection_disabled_color);
	if (_debug_navigation_enable_link_connections_xray) {
		material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MAX - 2);

	_debug_navigation_link_connections_disabled_material = material;
	return _debug_navigation_link_connections_disabled_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_agent_path_line_material() {
	if (_debug_navigation_agent_path_line_material.is_valid()) {
		return _debug_navigation_agent_path_line_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	material->set_albedo(_debug_navigation_agent_path_color);
	if (_debug_navigation_enable_agent_paths_xray) {
		material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MAX - 2);

	_debug_navigation_agent_path_line_material = material;
	return _debug_navigation_agent_path_line_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_agent_path_point_material() {
	if (_debug_navigation_agent_path_point_material.is_valid()) {
		return _debug_navigation_agent_path_point_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	//material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	material->set_albedo(_debug_navigation_agent_path_color);
	material->set_flag(SpatialMaterial::FLAG_USE_POINT_SIZE, true);
	material->set_point_size(_debug_navigation_agent_path_point_size);
	if (_debug_navigation_enable_agent_paths_xray) {
		material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	}
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MAX - 2);

	_debug_navigation_agent_path_point_material = material;
	return _debug_navigation_agent_path_point_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_agents_radius_material() {
	if (_debug_navigation_avoidance_agents_radius_material.is_valid()) {
		return _debug_navigation_avoidance_agents_radius_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_agents_radius_color);
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);

	_debug_navigation_avoidance_agents_radius_material = material;
	return _debug_navigation_avoidance_agents_radius_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_obstacles_radius_material() {
	if (_debug_navigation_avoidance_obstacles_radius_material.is_valid()) {
		return _debug_navigation_avoidance_obstacles_radius_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_obstacles_radius_color);
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);

	_debug_navigation_avoidance_obstacles_radius_material = material;
	return _debug_navigation_avoidance_obstacles_radius_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushin_face_material() {
	if (_debug_navigation_avoidance_static_obstacle_pushin_face_material.is_valid()) {
		return _debug_navigation_avoidance_static_obstacle_pushin_face_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushin_face_color);
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);

	_debug_navigation_avoidance_static_obstacle_pushin_face_material = material;
	return _debug_navigation_avoidance_static_obstacle_pushin_face_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushout_face_material() {
	if (_debug_navigation_avoidance_static_obstacle_pushout_face_material.is_valid()) {
		return _debug_navigation_avoidance_static_obstacle_pushout_face_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushout_face_color);
	material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);

	_debug_navigation_avoidance_static_obstacle_pushout_face_material = material;
	return _debug_navigation_avoidance_static_obstacle_pushout_face_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushin_edge_material() {
	if (_debug_navigation_avoidance_static_obstacle_pushin_edge_material.is_valid()) {
		return _debug_navigation_avoidance_static_obstacle_pushin_edge_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	//material->set_transparency(SpatialMaterial::TRANSPARENCY_ALPHA);
	//material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushin_edge_color);
	//material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);
	material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);

	_debug_navigation_avoidance_static_obstacle_pushin_edge_material = material;
	return _debug_navigation_avoidance_static_obstacle_pushin_edge_material;
}

Ref<SpatialMaterial> NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushout_edge_material() {
	if (_debug_navigation_avoidance_static_obstacle_pushout_edge_material.is_valid()) {
		return _debug_navigation_avoidance_static_obstacle_pushout_edge_material;
	}

	Ref<SpatialMaterial> material = Ref<SpatialMaterial>(memnew(SpatialMaterial));
	material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	///material->set_transparency(SpatialMaterial::TRANSPARENCY_ALPHA);
	//material->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushout_edge_color);
	//material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN + 2);
	material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);

	_debug_navigation_avoidance_static_obstacle_pushout_edge_material = material;
	return _debug_navigation_avoidance_static_obstacle_pushout_edge_material;
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

void NavigationServer::set_debug_navigation_link_connection_color(const Color &p_color) {
	_debug_navigation_link_connection_color = p_color;
	if (_debug_navigation_link_connections_material.is_valid()) {
		_debug_navigation_link_connections_material->set_albedo(_debug_navigation_link_connection_color);
	}
}

Color NavigationServer::get_debug_navigation_link_connection_color() const {
	return _debug_navigation_link_connection_color;
}

void NavigationServer::set_debug_navigation_link_connection_disabled_color(const Color &p_color) {
	_debug_navigation_link_connection_disabled_color = p_color;
	if (_debug_navigation_link_connections_disabled_material.is_valid()) {
		_debug_navigation_link_connections_disabled_material->set_albedo(_debug_navigation_link_connection_disabled_color);
	}
}

Color NavigationServer::get_debug_navigation_link_connection_disabled_color() const {
	return _debug_navigation_link_connection_disabled_color;
}

void NavigationServer::set_debug_navigation_agent_path_point_size(float p_point_size) {
	_debug_navigation_agent_path_point_size = MAX(0.1, p_point_size);
	if (_debug_navigation_agent_path_point_material.is_valid()) {
		_debug_navigation_agent_path_point_material->set_point_size(_debug_navigation_agent_path_point_size);
	}
}

float NavigationServer::get_debug_navigation_agent_path_point_size() const {
	return _debug_navigation_agent_path_point_size;
}

void NavigationServer::set_debug_navigation_agent_path_color(const Color &p_color) {
	_debug_navigation_agent_path_color = p_color;
	if (_debug_navigation_agent_path_line_material.is_valid()) {
		_debug_navigation_agent_path_line_material->set_albedo(_debug_navigation_agent_path_color);
	}
	if (_debug_navigation_agent_path_point_material.is_valid()) {
		_debug_navigation_agent_path_point_material->set_albedo(_debug_navigation_agent_path_color);
	}
}

Color NavigationServer::get_debug_navigation_agent_path_color() const {
	return _debug_navigation_agent_path_color;
}

void NavigationServer::set_debug_navigation_enable_edge_connections(const bool p_value) {
	_debug_navigation_enable_edge_connections = p_value;
	_navigation_debug_dirty = true;
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
	_navigation_debug_dirty = true;
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
	_navigation_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enable_geometry_face_random_color() const {
	return _debug_navigation_enable_geometry_face_random_color;
}

void NavigationServer::set_debug_navigation_enable_link_connections(const bool p_value) {
	_debug_navigation_enable_link_connections = p_value;
	_navigation_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enable_link_connections() const {
	return _debug_navigation_enable_link_connections;
}

void NavigationServer::set_debug_navigation_enable_link_connections_xray(const bool p_value) {
	_debug_navigation_enable_link_connections_xray = p_value;
	if (_debug_navigation_link_connections_material.is_valid()) {
		_debug_navigation_link_connections_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, _debug_navigation_enable_link_connections_xray);
	}
}

bool NavigationServer::get_debug_navigation_enable_link_connections_xray() const {
	return _debug_navigation_enable_link_connections_xray;
}

void NavigationServer::set_debug_navigation_avoidance_enable_agents_radius(const bool p_value) {
	_debug_navigation_avoidance_enable_agents_radius = p_value;
	_avoidance_debug_dirty = true;
	call_deferred("_emit_avoidance_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_avoidance_enable_agents_radius() const {
	return _debug_navigation_avoidance_enable_agents_radius;
}

void NavigationServer::set_debug_navigation_avoidance_enable_obstacles_radius(const bool p_value) {
	_debug_navigation_avoidance_enable_obstacles_radius = p_value;
	_avoidance_debug_dirty = true;
	call_deferred("_emit_avoidance_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_avoidance_enable_obstacles_radius() const {
	return _debug_navigation_avoidance_enable_obstacles_radius;
}

void NavigationServer::set_debug_navigation_avoidance_enable_obstacles_static(const bool p_value) {
	_debug_navigation_avoidance_enable_obstacles_static = p_value;
	_avoidance_debug_dirty = true;
	call_deferred("_emit_avoidance_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_avoidance_enable_obstacles_static() const {
	return _debug_navigation_avoidance_enable_obstacles_static;
}

void NavigationServer::set_debug_navigation_avoidance_agents_radius_color(const Color &p_color) {
	_debug_navigation_avoidance_agents_radius_color = p_color;
	if (_debug_navigation_avoidance_agents_radius_material.is_valid()) {
		_debug_navigation_avoidance_agents_radius_material->set_albedo(_debug_navigation_avoidance_agents_radius_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_agents_radius_color() const {
	return _debug_navigation_avoidance_agents_radius_color;
}

void NavigationServer::set_debug_navigation_avoidance_obstacles_radius_color(const Color &p_color) {
	_debug_navigation_avoidance_obstacles_radius_color = p_color;
	if (_debug_navigation_avoidance_obstacles_radius_material.is_valid()) {
		_debug_navigation_avoidance_obstacles_radius_material->set_albedo(_debug_navigation_avoidance_obstacles_radius_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_obstacles_radius_color() const {
	return _debug_navigation_avoidance_obstacles_radius_color;
}

void NavigationServer::set_debug_navigation_avoidance_static_obstacle_pushin_face_color(const Color &p_color) {
	_debug_navigation_avoidance_static_obstacle_pushin_face_color = p_color;
	if (_debug_navigation_avoidance_static_obstacle_pushin_face_material.is_valid()) {
		_debug_navigation_avoidance_static_obstacle_pushin_face_material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushin_face_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushin_face_color() const {
	return _debug_navigation_avoidance_static_obstacle_pushin_face_color;
}

void NavigationServer::set_debug_navigation_avoidance_static_obstacle_pushout_face_color(const Color &p_color) {
	_debug_navigation_avoidance_static_obstacle_pushout_face_color = p_color;
	if (_debug_navigation_avoidance_static_obstacle_pushout_face_material.is_valid()) {
		_debug_navigation_avoidance_static_obstacle_pushout_face_material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushout_face_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushout_face_color() const {
	return _debug_navigation_avoidance_static_obstacle_pushout_face_color;
}

void NavigationServer::set_debug_navigation_avoidance_static_obstacle_pushin_edge_color(const Color &p_color) {
	_debug_navigation_avoidance_static_obstacle_pushin_edge_color = p_color;
	if (_debug_navigation_avoidance_static_obstacle_pushin_edge_material.is_valid()) {
		_debug_navigation_avoidance_static_obstacle_pushin_edge_material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushin_edge_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushin_edge_color() const {
	return _debug_navigation_avoidance_static_obstacle_pushin_edge_color;
}

void NavigationServer::set_debug_navigation_avoidance_static_obstacle_pushout_edge_color(const Color &p_color) {
	_debug_navigation_avoidance_static_obstacle_pushout_edge_color = p_color;
	if (_debug_navigation_avoidance_static_obstacle_pushout_edge_material.is_valid()) {
		_debug_navigation_avoidance_static_obstacle_pushout_edge_material->set_albedo(_debug_navigation_avoidance_static_obstacle_pushout_edge_color);
	}
}

Color NavigationServer::get_debug_navigation_avoidance_static_obstacle_pushout_edge_color() const {
	return _debug_navigation_avoidance_static_obstacle_pushout_edge_color;
}

void NavigationServer::set_debug_navigation_enable_agent_paths(const bool p_value) {
	if (_debug_navigation_enable_agent_paths != p_value) {
		_debug_dirty = true;
	}

	_debug_navigation_enable_agent_paths = p_value;

	if (_debug_dirty) {
		call_deferred("_emit_navigation_debug_changed_signal");
	}
}

bool NavigationServer::get_debug_navigation_enable_agent_paths() const {
	return _debug_navigation_enable_agent_paths;
}

void NavigationServer::set_debug_navigation_enable_agent_paths_xray(const bool p_value) {
	_debug_navigation_enable_agent_paths_xray = p_value;
	if (_debug_navigation_agent_path_line_material.is_valid()) {
		_debug_navigation_agent_path_line_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, _debug_navigation_enable_agent_paths_xray);
	}
	if (_debug_navigation_agent_path_point_material.is_valid()) {
		_debug_navigation_agent_path_point_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, _debug_navigation_enable_agent_paths_xray);
	}
}

bool NavigationServer::get_debug_navigation_enable_agent_paths_xray() const {
	return _debug_navigation_enable_agent_paths_xray;
}

void NavigationServer::set_debug_navigation_enabled(bool p_enabled) {
	_debug_navigation_enabled = p_enabled;
	_navigation_debug_dirty = true;
	call_deferred("_emit_navigation_debug_changed_signal");
}

bool NavigationServer::get_debug_navigation_enabled() const {
	return _debug_navigation_enabled;
}

void NavigationServer::set_debug_avoidance_enabled(bool p_enabled) {
	_debug_avoidance_enabled = p_enabled;
	_avoidance_debug_dirty = true;
	call_deferred("_emit_avoidance_debug_changed_signal");
}

bool NavigationServer::get_debug_avoidance_enabled() const {
	return _debug_avoidance_enabled;
}

#endif // DEBUG_ENABLED

void NavigationServer::query_path(const Ref<NavigationPathQueryParameters3D> &p_query_parameters, Ref<NavigationPathQueryResult3D> p_query_result) const {
	ERR_FAIL_COND(!p_query_parameters.is_valid());
	ERR_FAIL_COND(!p_query_result.is_valid());

	const NavigationUtilities::PathQueryResult _query_result = _query_path(p_query_parameters->get_parameters());

	p_query_result->set_path(_query_result.path);
	p_query_result->set_path_types(_query_result.path_types);
	p_query_result->set_path_rids(_query_result.path_rids);
	p_query_result->set_path_owner_ids(_query_result.path_owner_ids);
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
