/*************************************************************************/
/*  navigation_agent.cpp                                                 */
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

#include "navigation_agent.h"

#include "core/config/engine.h"
#include "core/math/geometry.h"
#include "scene/3d/navigation.h"
#include "scene/3d/navigation_link_3d.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/world_3d.h"
#include "servers/navigation/navigation_path_query_result_3d.h"
#include "servers/navigation_server.h"

void NavigationAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_rid"), &NavigationAgent::get_rid);

	ClassDB::bind_method(D_METHOD("set_avoidance_enabled", "enabled"), &NavigationAgent::set_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("get_avoidance_enabled"), &NavigationAgent::get_avoidance_enabled);

	ClassDB::bind_method(D_METHOD("set_path_desired_distance", "desired_distance"), &NavigationAgent::set_path_desired_distance);
	ClassDB::bind_method(D_METHOD("get_path_desired_distance"), &NavigationAgent::get_path_desired_distance);

	ClassDB::bind_method(D_METHOD("set_target_desired_distance", "desired_distance"), &NavigationAgent::set_target_desired_distance);
	ClassDB::bind_method(D_METHOD("get_target_desired_distance"), &NavigationAgent::get_target_desired_distance);

	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &NavigationAgent::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &NavigationAgent::get_radius);

	ClassDB::bind_method(D_METHOD("set_height", "height"), &NavigationAgent::set_height);
	ClassDB::bind_method(D_METHOD("get_height"), &NavigationAgent::get_height);

	ClassDB::bind_method(D_METHOD("set_path_height_offset", "path_height_offset"), &NavigationAgent::set_path_height_offset);
	ClassDB::bind_method(D_METHOD("get_path_height_offset"), &NavigationAgent::get_path_height_offset);

	ClassDB::bind_method(D_METHOD("set_use_3d_avoidance", "enabled"), &NavigationAgent::set_use_3d_avoidance);
	ClassDB::bind_method(D_METHOD("get_use_3d_avoidance"), &NavigationAgent::get_use_3d_avoidance);

	ClassDB::bind_method(D_METHOD("set_navigation", "navigation"), &NavigationAgent::set_navigation_node);
	ClassDB::bind_method(D_METHOD("get_navigation"), &NavigationAgent::get_navigation_node);

	ClassDB::bind_method(D_METHOD("set_navigation_map", "navigation_map"), &NavigationAgent::set_navigation_map);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &NavigationAgent::get_navigation_map);

	ClassDB::bind_method(D_METHOD("set_neighbor_distance", "neighbor_distance"), &NavigationAgent::set_neighbor_distance);
	ClassDB::bind_method(D_METHOD("get_neighbor_distance"), &NavigationAgent::get_neighbor_distance);

	ClassDB::bind_method(D_METHOD("set_max_neighbors", "max_neighbors"), &NavigationAgent::set_max_neighbors);
	ClassDB::bind_method(D_METHOD("get_max_neighbors"), &NavigationAgent::get_max_neighbors);

	ClassDB::bind_method(D_METHOD("set_time_horizon_agents", "time_horizon"), &NavigationAgent::set_time_horizon_agents);
	ClassDB::bind_method(D_METHOD("get_time_horizon_agents"), &NavigationAgent::get_time_horizon_agents);

	ClassDB::bind_method(D_METHOD("set_time_horizon_obstacles", "time_horizon"), &NavigationAgent::set_time_horizon_obstacles);
	ClassDB::bind_method(D_METHOD("get_time_horizon_obstacles"), &NavigationAgent::get_time_horizon_obstacles);

	ClassDB::bind_method(D_METHOD("set_max_speed", "max_speed"), &NavigationAgent::set_max_speed);
	ClassDB::bind_method(D_METHOD("get_max_speed"), &NavigationAgent::get_max_speed);

	ClassDB::bind_method(D_METHOD("set_path_max_distance", "max_speed"), &NavigationAgent::set_path_max_distance);
	ClassDB::bind_method(D_METHOD("get_path_max_distance"), &NavigationAgent::get_path_max_distance);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationAgent::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationAgent::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("set_navigation_layer_value", "layer_number", "value"), &NavigationAgent::set_navigation_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_value", "layer_number"), &NavigationAgent::get_navigation_layer_value);

	ClassDB::bind_method(D_METHOD("set_pathfinding_algorithm", "pathfinding_algorithm"), &NavigationAgent::set_pathfinding_algorithm);
	ClassDB::bind_method(D_METHOD("get_pathfinding_algorithm"), &NavigationAgent::get_pathfinding_algorithm);

	ClassDB::bind_method(D_METHOD("set_path_postprocessing", "path_postprocessing"), &NavigationAgent::set_path_postprocessing);
	ClassDB::bind_method(D_METHOD("get_path_postprocessing"), &NavigationAgent::get_path_postprocessing);

	ClassDB::bind_method(D_METHOD("set_path_metadata_flags", "flags"), &NavigationAgent::set_path_metadata_flags);
	ClassDB::bind_method(D_METHOD("get_path_metadata_flags"), &NavigationAgent::get_path_metadata_flags);

	ClassDB::bind_method(D_METHOD("set_target_position", "position"), &NavigationAgent::set_target_position);
	ClassDB::bind_method(D_METHOD("get_target_position"), &NavigationAgent::get_target_position);

	ClassDB::bind_method(D_METHOD("get_next_position"), &NavigationAgent::get_next_position);

	ClassDB::bind_method(D_METHOD("set_velocity_forced", "velocity"), &NavigationAgent::set_velocity_forced);

	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &NavigationAgent::set_velocity);
	ClassDB::bind_method(D_METHOD("get_velocity"), &NavigationAgent::get_velocity);

	ClassDB::bind_method(D_METHOD("distance_to_target"), &NavigationAgent::distance_to_target);

	ClassDB::bind_method(D_METHOD("get_current_navigation_result"), &NavigationAgent::get_current_navigation_result);
	ClassDB::bind_method(D_METHOD("get_current_navigation_path"), &NavigationAgent::get_current_navigation_path);
	ClassDB::bind_method(D_METHOD("get_current_navigation_path_index"), &NavigationAgent::get_current_navigation_path_index);
	ClassDB::bind_method(D_METHOD("is_target_reached"), &NavigationAgent::is_target_reached);
	ClassDB::bind_method(D_METHOD("is_target_reachable"), &NavigationAgent::is_target_reachable);
	ClassDB::bind_method(D_METHOD("is_navigation_finished"), &NavigationAgent::is_navigation_finished);
	ClassDB::bind_method(D_METHOD("get_final_position"), &NavigationAgent::get_final_position);

	ClassDB::bind_method(D_METHOD("set_avoidance_layers", "layers"), &NavigationAgent::set_avoidance_layers);
	ClassDB::bind_method(D_METHOD("get_avoidance_layers"), &NavigationAgent::get_avoidance_layers);
	ClassDB::bind_method(D_METHOD("set_avoidance_mask", "mask"), &NavigationAgent::set_avoidance_mask);
	ClassDB::bind_method(D_METHOD("get_avoidance_mask"), &NavigationAgent::get_avoidance_mask);
	ClassDB::bind_method(D_METHOD("set_avoidance_layer_value", "layer_number", "value"), &NavigationAgent::set_avoidance_layer_value);
	ClassDB::bind_method(D_METHOD("get_avoidance_layer_value", "layer_number"), &NavigationAgent::get_avoidance_layer_value);
	ClassDB::bind_method(D_METHOD("set_avoidance_mask_value", "mask_number", "value"), &NavigationAgent::set_avoidance_mask_value);
	ClassDB::bind_method(D_METHOD("get_avoidance_mask_value", "mask_number"), &NavigationAgent::get_avoidance_mask_value);
	ClassDB::bind_method(D_METHOD("set_avoidance_priority", "priority"), &NavigationAgent::set_avoidance_priority);
	ClassDB::bind_method(D_METHOD("get_avoidance_priority"), &NavigationAgent::get_avoidance_priority);

	ClassDB::bind_method(D_METHOD("_avoidance_done", "new_velocity"), &NavigationAgent::_avoidance_done);

	ADD_GROUP("Pathfinding", "");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "target_position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_target_position", "get_target_position");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "path_desired_distance", PROPERTY_HINT_RANGE, "0.1,100,0.01,or_greater,suffix:m"), "set_path_desired_distance", "get_path_desired_distance");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "target_desired_distance", PROPERTY_HINT_RANGE, "0.1,100,0.01,or_greater,suffix:m"), "set_target_desired_distance", "get_target_desired_distance");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "path_height_offset", PROPERTY_HINT_RANGE, "-100.0,100,0.01,or_greater,suffix:m"), "set_path_height_offset", "get_path_height_offset");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "path_max_distance", PROPERTY_HINT_RANGE, "0.01,100,0.1,or_greater,suffix:m"), "set_path_max_distance", "get_path_max_distance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_3D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "pathfinding_algorithm", PROPERTY_HINT_ENUM, "AStar"), "set_pathfinding_algorithm", "get_pathfinding_algorithm");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "path_postprocessing", PROPERTY_HINT_ENUM, "Corridorfunnel,Edgecentered"), "set_path_postprocessing", "get_path_postprocessing");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "path_metadata_flags", PROPERTY_HINT_FLAGS, "Include Types,Include RIDs,Include Owners"), "set_path_metadata_flags", "get_path_metadata_flags");

	ADD_GROUP("Avoidance", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "avoidance_enabled"), "set_avoidance_enabled", "get_avoidance_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_velocity", "get_velocity");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "height", PROPERTY_HINT_RANGE, "0.01,100,0.01,or_greater,suffix:m"), "set_height", "get_height");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius", PROPERTY_HINT_RANGE, "0.1,100,0.01,or_greater,suffix:m"), "set_radius", "get_radius");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "neighbor_distance", PROPERTY_HINT_RANGE, "0.1,10000,0.01,or_greater,suffix:m"), "set_neighbor_distance", "get_neighbor_distance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_neighbors", PROPERTY_HINT_RANGE, "1,10000,1,or_greater"), "set_max_neighbors", "get_max_neighbors");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_horizon_agents", PROPERTY_HINT_RANGE, "0.0,10,0.01,or_greater,suffix:s"), "set_time_horizon_agents", "get_time_horizon_agents");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_horizon_obstacles", PROPERTY_HINT_RANGE, "0.0,10,0.01,or_greater,suffix:s"), "set_time_horizon_obstacles", "get_time_horizon_obstacles");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_speed", PROPERTY_HINT_RANGE, "0.1,1000,0.01,or_greater,suffix:m/s"), "set_max_speed", "get_max_speed");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_3d_avoidance"), "set_use_3d_avoidance", "get_use_3d_avoidance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "avoidance_layers", PROPERTY_HINT_LAYERS_AVOIDANCE), "set_avoidance_layers", "get_avoidance_layers");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "avoidance_mask", PROPERTY_HINT_LAYERS_AVOIDANCE), "set_avoidance_mask", "get_avoidance_mask");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "avoidance_priority", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_avoidance_priority", "get_avoidance_priority");

#ifdef DEBUG_ENABLED
	ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &NavigationAgent::set_debug_enabled);
	ClassDB::bind_method(D_METHOD("get_debug_enabled"), &NavigationAgent::get_debug_enabled);
	ClassDB::bind_method(D_METHOD("set_debug_use_custom", "enabled"), &NavigationAgent::set_debug_use_custom);
	ClassDB::bind_method(D_METHOD("get_debug_use_custom"), &NavigationAgent::get_debug_use_custom);
	ClassDB::bind_method(D_METHOD("set_debug_path_custom_color", "color"), &NavigationAgent::set_debug_path_custom_color);
	ClassDB::bind_method(D_METHOD("get_debug_path_custom_color"), &NavigationAgent::get_debug_path_custom_color);
	ClassDB::bind_method(D_METHOD("set_debug_path_custom_point_size", "point_size"), &NavigationAgent::set_debug_path_custom_point_size);
	ClassDB::bind_method(D_METHOD("get_debug_path_custom_point_size"), &NavigationAgent::get_debug_path_custom_point_size);
	ClassDB::bind_method(D_METHOD("_navigation_debug_changed"), &NavigationAgent::_navigation_debug_changed);

	ADD_GROUP("Debug", "debug_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_enabled"), "set_debug_enabled", "get_debug_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_use_custom"), "set_debug_use_custom", "get_debug_use_custom");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "debug_path_custom_color"), "set_debug_path_custom_color", "get_debug_path_custom_color");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "debug_path_custom_point_size", PROPERTY_HINT_RANGE, "0,50,0.01,or_greater,suffix:px"), "set_debug_path_custom_point_size", "get_debug_path_custom_point_size");
#endif // DEBUG_ENABLED

	ADD_SIGNAL(MethodInfo("path_changed"));
	ADD_SIGNAL(MethodInfo("target_reached"));
	ADD_SIGNAL(MethodInfo("waypoint_reached", PropertyInfo(Variant::DICTIONARY, "details")));
	ADD_SIGNAL(MethodInfo("link_reached", PropertyInfo(Variant::DICTIONARY, "details")));
	ADD_SIGNAL(MethodInfo("navigation_finished"));
	ADD_SIGNAL(MethodInfo("velocity_computed", PropertyInfo(Variant::VECTOR3, "safe_velocity")));
}

void NavigationAgent::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POST_ENTER_TREE: {
			// Search the navigation node and set it
			{
				Navigation *nav = nullptr;
				Node *p = get_parent();
				while (p != nullptr) {
					nav = Object::cast_to<Navigation>(p);
					if (nav != nullptr) {
						p = nullptr;
					} else {
						p = p->get_parent();
					}
				}

				set_navigation(nav);
			}

			// need to use POST_ENTER_TREE cause with normal ENTER_TREE not all required Nodes are ready.
			// cannot use READY as ready does not get called if Node is readded to SceneTree
			set_agent_parent(get_parent());
			set_physics_process_internal(true);

			if (agent_parent && avoidance_enabled) {
				NavigationServer::get_singleton()->agent_set_position(agent, agent_parent->get_global_transform().origin);
			}

#ifdef DEBUG_ENABLED
			if (NavigationServer::get_singleton()->get_debug_enabled()) {
				debug_path_dirty = true;
			}
#endif // DEBUG_ENABLED
		} break;
		case NOTIFICATION_PARENTED: {
			if (is_inside_tree() && (get_parent() != agent_parent)) {
				// only react to PARENTED notifications when already inside_tree and parent changed, e.g. users switch nodes around
				// PARENTED notification fires also when Node is added in scripts to a parent
				// this would spam transforms fails and world fails while Node is outside SceneTree
				// when node gets reparented when joining the tree POST_ENTER_TREE takes care of this
				set_agent_parent(get_parent());
				set_physics_process_internal(true);
			}
		} break;
		case NOTIFICATION_UNPARENTED: {
			// if agent has no parent no point in processing it until reparented
			set_agent_parent(nullptr);
			set_physics_process_internal(false);
		} break;
		case NOTIFICATION_EXIT_TREE: {
			set_agent_parent(nullptr);
			set_navigation(nullptr);
			set_physics_process_internal(false);

#ifdef DEBUG_ENABLED
			if (debug_path_instance.is_valid()) {
				RS::get_singleton()->instance_set_visible(debug_path_instance, false);
			}
#endif // DEBUG_ENABLED
		} break;
		case NOTIFICATION_PAUSED: {
			if (agent_parent) {
				NavigationServer::get_singleton()->agent_set_paused(get_rid(), !agent_parent->can_process());
			}
		} break;
		case NOTIFICATION_UNPAUSED: {
			if (agent_parent) {
				NavigationServer::get_singleton()->agent_set_paused(get_rid(), !agent_parent->can_process());
			}
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			if (agent_parent && avoidance_enabled) {
				NavigationServer::get_singleton()->agent_set_position(agent, agent_parent->get_global_transform().origin);
			}

			if (agent_parent && target_position_submitted) {
				if (velocity_submitted) {
					velocity_submitted = false;
					if (avoidance_enabled) {
						if (!use_3d_avoidance) {
							stored_y_velocity = velocity.y;
							velocity.y = 0.0;
						}
						NavigationServer::get_singleton()->agent_set_velocity(agent, velocity);
					}
				}
				if (velocity_forced_submitted) {
					velocity_forced_submitted = false;
					if (avoidance_enabled) {
						NavigationServer::get_singleton()->agent_set_velocity_forced(agent, velocity_forced);
					}
				}
				_check_distance_to_target();
			}

#ifdef DEBUG_ENABLED
			if (debug_path_dirty) {
				_update_debug_path();
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

NavigationAgent::NavigationAgent() {
	agent_parent = nullptr;
	navigation = nullptr;

	avoidance_enabled = false;
	use_3d_avoidance = false;
	avoidance_layers = 1;
	avoidance_mask = 1;
	avoidance_priority = 1.0;
	navigation_layers = 1;
	pathfinding_algorithm = NavigationPathQueryParameters3D::PathfindingAlgorithm::PATHFINDING_ALGORITHM_ASTAR;
	path_postprocessing = NavigationPathQueryParameters3D::PathPostProcessing::PATH_POSTPROCESSING_CORRIDORFUNNEL;
	path_metadata_flags = NavigationPathQueryParameters3D::PATH_METADATA_INCLUDE_ALL;

	path_desired_distance = 1.0;
	target_desired_distance = 1.0;
	height = 1.0;
	radius = 0.5;
	path_height_offset = 0.0;
	neighbor_distance = 50.0;
	max_neighbors = 10;
	time_horizon_agents = 1.0;
	time_horizon_obstacles = 0.0;
	max_speed = 10.0;
	path_max_distance = 5.0;

	target_position_submitted = false;
	nav_path_index = 0;

	velocity_submitted = false;
	velocity_forced_submitted = false;

	stored_y_velocity = 0.0;
	target_reached = false;
	navigation_finished = true;
	update_frame_id = 0;

	agent = NavigationServer::get_singleton()->agent_create();

	NavigationServer::get_singleton()->agent_set_neighbor_distance(agent, neighbor_distance);
	NavigationServer::get_singleton()->agent_set_max_neighbors(agent, max_neighbors);
	NavigationServer::get_singleton()->agent_set_time_horizon_agents(agent, time_horizon_agents);
	NavigationServer::get_singleton()->agent_set_time_horizon_obstacles(agent, time_horizon_obstacles);
	NavigationServer::get_singleton()->agent_set_radius(agent, radius);
	NavigationServer::get_singleton()->agent_set_height(agent, height);
	NavigationServer::get_singleton()->agent_set_max_speed(agent, max_speed);

	navigation_query.instance();
	navigation_result.instance();

	set_avoidance_layers(avoidance_layers);
	set_avoidance_mask(avoidance_mask);
	set_avoidance_priority(avoidance_priority);
	set_use_3d_avoidance(use_3d_avoidance);
	set_avoidance_enabled(avoidance_enabled);

#ifdef DEBUG_ENABLED
	debug_enabled = false;
	debug_path_dirty = true;
	debug_path_custom_point_size = 4.0;
	debug_use_custom = false;
	debug_path_custom_color = Color(1.0, 1.0, 1.0, 1.0);

	NavigationServer::get_singleton()->connect("navigation_debug_changed", this, "_navigation_debug_changed");
#endif // DEBUG_ENABLED
}

NavigationAgent::~NavigationAgent() {
	ERR_FAIL_NULL(NavigationServer::get_singleton());
	NavigationServer::get_singleton()->free(agent);
	agent = RID(); // Pointless

#ifdef DEBUG_ENABLED
	NavigationServer::get_singleton()->disconnect("navigation_debug_changed", this, "_navigation_debug_changed");

	ERR_FAIL_NULL(RenderingServer::get_singleton());
	if (debug_path_instance.is_valid()) {
		RenderingServer::get_singleton()->free(debug_path_instance);
	}
	if (debug_path_mesh.is_valid()) {
		RenderingServer::get_singleton()->free(debug_path_mesh->get_rid());
	}
#endif // DEBUG_ENABLED
}

void NavigationAgent::set_avoidance_enabled(bool p_enabled) {
	avoidance_enabled = p_enabled;
	if (avoidance_enabled) {
		NavigationServer::get_singleton()->agent_set_avoidance_enabled(agent, true);
		NavigationServer::get_singleton()->agent_set_avoidance_callback(agent, get_instance_id(), "_avoidance_done");
	} else {
		NavigationServer::get_singleton()->agent_set_avoidance_enabled(agent, false);
		NavigationServer::get_singleton()->agent_set_avoidance_callback(agent, ObjectID(), "_avoidance_done");
	}
}

bool NavigationAgent::get_avoidance_enabled() const {
	return avoidance_enabled;
}

void NavigationAgent::set_navigation(Navigation *p_nav) {
	if (navigation == p_nav) {
		return; // Pointless
	}

	navigation = p_nav;
	NavigationServer::get_singleton()->agent_set_map(agent, navigation == nullptr ? RID() : navigation->get_rid());
}

void NavigationAgent::set_navigation_node(Node *p_nav) {
	Navigation *nav = Object::cast_to<Navigation>(p_nav);
	ERR_FAIL_NULL(nav);
	set_navigation(nav);
}

Node *NavigationAgent::get_navigation_node() const {
	return Object::cast_to<Node>(navigation);
}

void NavigationAgent::set_agent_parent(Node *p_agent_parent) {
	// remove agent from any avoidance map before changing parent or there will be leftovers on the RVO map
	NavigationServer::get_singleton()->agent_set_avoidance_callback(agent, ObjectID(), "_avoidance_done");

	if (Object::cast_to<Spatial>(p_agent_parent) != nullptr) {
		// place agent on navigation map first or else the RVO agent callback creation fails silently later
		agent_parent = Object::cast_to<Spatial>(p_agent_parent);
		NavigationServer::get_singleton()->agent_set_map(get_rid(), get_navigation_map());
		// create new avoidance callback if enabled
		set_avoidance_enabled(avoidance_enabled);
	} else {
		agent_parent = nullptr;
		NavigationServer::get_singleton()->agent_set_map(get_rid(), get_navigation_map());
	}
}

void NavigationAgent::set_navigation_layers(uint32_t p_layers) {
	bool _navigation_layers_changed = navigation_layers != p_layers;
	navigation_layers = p_layers;
	if (_navigation_layers_changed) {
		_request_repath();
	}
}

uint32_t NavigationAgent::get_navigation_layers() const {
	return navigation_layers;
}

void NavigationAgent::set_navigation_layer_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Navigation layer number must be between 1 and 32 inclusive.");
	uint32_t _navigation_layers = get_navigation_layers();
	if (p_value) {
		_navigation_layers |= 1 << (p_layer_number - 1);
	} else {
		_navigation_layers &= ~(1 << (p_layer_number - 1));
	}
	set_navigation_layers(_navigation_layers);
}

bool NavigationAgent::get_navigation_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");
	return get_navigation_layers() & (1 << (p_layer_number - 1));
}

void NavigationAgent::set_pathfinding_algorithm(const NavigationPathQueryParameters3D::PathfindingAlgorithm p_pathfinding_algorithm) {
	if (pathfinding_algorithm == p_pathfinding_algorithm) {
		return;
	}

	pathfinding_algorithm = p_pathfinding_algorithm;

	navigation_query->set_pathfinding_algorithm(pathfinding_algorithm);
}

void NavigationAgent::set_path_postprocessing(const NavigationPathQueryParameters3D::PathPostProcessing p_path_postprocessing) {
	if (path_postprocessing == p_path_postprocessing) {
		return;
	}

	path_postprocessing = p_path_postprocessing;

	navigation_query->set_path_postprocessing(path_postprocessing);
}

void NavigationAgent::set_path_metadata_flags(const int p_flags) {
	path_metadata_flags = p_flags;
}
int NavigationAgent::get_path_metadata_flags() const {
	return path_metadata_flags;
}

void NavigationAgent::set_navigation_map(RID p_navigation_map) {
	map_override = p_navigation_map;
	NavigationServer::get_singleton()->agent_set_map(agent, map_override);
	_request_repath();
}

RID NavigationAgent::get_navigation_map() const {
	if (navigation) {
		return navigation->get_rid();
	} else if (map_override.is_valid()) {
		return map_override;
	} else if (agent_parent != nullptr) {
		return agent_parent->get_world_3d()->get_navigation_map();
	}
	return RID();
}

void NavigationAgent::set_path_desired_distance(real_t p_dd) {
	path_desired_distance = p_dd;
}

void NavigationAgent::set_target_desired_distance(real_t p_dd) {
	target_desired_distance = p_dd;
}

void NavigationAgent::set_radius(real_t p_radius) {
	ERR_FAIL_COND_MSG(p_radius < 0.0, "Radius must be positive.");
	radius = p_radius;
	NavigationServer::get_singleton()->agent_set_radius(agent, radius);
}

void NavigationAgent::set_height(real_t p_height) {
	ERR_FAIL_COND_MSG(p_height < 0.0, "Height must be positive.");
	if (Math::is_equal_approx(height, p_height)) {
		return;
	}

	height = p_height;
	NavigationServer::get_singleton()->agent_set_height(agent, height);
}

void NavigationAgent::set_path_height_offset(real_t p_path_height_offset) {
	path_height_offset = p_path_height_offset;
}

void NavigationAgent::set_use_3d_avoidance(bool p_use_3d_avoidance) {
	use_3d_avoidance = p_use_3d_avoidance;
	NavigationServer::get_singleton()->agent_set_use_3d_avoidance(agent, use_3d_avoidance);
	property_list_changed_notify();
}

void NavigationAgent::set_neighbor_distance(real_t p_dist) {
	neighbor_distance = p_dist;
	NavigationServer::get_singleton()->agent_set_neighbor_distance(agent, neighbor_distance);
}

void NavigationAgent::set_max_neighbors(int p_count) {
	max_neighbors = p_count;
	NavigationServer::get_singleton()->agent_set_max_neighbors(agent, max_neighbors);
}

void NavigationAgent::set_time_horizon_agents(real_t p_time_horizon) {
	ERR_FAIL_COND_MSG(p_time_horizon < 0.0, "Time horizion must be positive.");
	if (Math::is_equal_approx(time_horizon_agents, p_time_horizon)) {
		return;
	}
	time_horizon_agents = p_time_horizon;
	NavigationServer::get_singleton()->agent_set_time_horizon_agents(agent, time_horizon_agents);
}

void NavigationAgent::set_time_horizon_obstacles(real_t p_time_horizon) {
	ERR_FAIL_COND_MSG(p_time_horizon < 0.0, "Time horizion must be positive.");
	if (Math::is_equal_approx(time_horizon_obstacles, p_time_horizon)) {
		return;
	}
	time_horizon_obstacles = p_time_horizon;
	NavigationServer::get_singleton()->agent_set_time_horizon_obstacles(agent, time_horizon_obstacles);
}

void NavigationAgent::set_max_speed(real_t p_max_speed) {
	ERR_FAIL_COND_MSG(p_max_speed < 0.0, "Max speed must be positive.");
	max_speed = p_max_speed;
	NavigationServer::get_singleton()->agent_set_max_speed(agent, max_speed);
}

void NavigationAgent::set_path_max_distance(real_t p_pmd) {
	path_max_distance = p_pmd;
}

real_t NavigationAgent::get_path_max_distance() {
	return path_max_distance;
}

void NavigationAgent::set_target_position(Vector3 p_position) {
	target_position = p_position;
	target_position_submitted = true;
	_request_repath();
}

Vector3 NavigationAgent::get_target_position() const {
	return target_position;
}

Vector3 NavigationAgent::get_next_position() {
	update_navigation();

	const Vector<Vector3> &navigation_path = navigation_result->get_path();
	if (navigation_path.size() == 0) {
		ERR_FAIL_COND_V_MSG(agent_parent == nullptr, Vector3(), "The agent has no parent.");
		return agent_parent->get_global_transform().origin;
	} else {
		return navigation_path[nav_path_index] - Vector3(0, path_height_offset, 0);
	}
}

Ref<NavigationPathQueryResult3D> NavigationAgent::get_current_navigation_result() const {
	return navigation_result;
}

Vector<Vector3> NavigationAgent::get_current_navigation_path() const {
	return navigation_result->get_path();
}

real_t NavigationAgent::distance_to_target() const {
	ERR_FAIL_COND_V_MSG(agent_parent == nullptr, 0.0, "The agent has no parent.");
	return agent_parent->get_global_transform().origin.distance_to(target_position);
}

bool NavigationAgent::is_target_reached() const {
	return target_reached;
}

bool NavigationAgent::is_target_reachable() {
	return target_desired_distance >= get_final_position().distance_to(target_position);
}

bool NavigationAgent::is_navigation_finished() {
	update_navigation();
	return navigation_finished;
}

Vector3 NavigationAgent::get_final_position() {
	update_navigation();

	const Vector<Vector3> &navigation_path = navigation_result->get_path();

	if (navigation_path.size() == 0) {
		return Vector3();
	}

	return navigation_path[navigation_path.size() - 1];
}

void NavigationAgent::set_velocity_forced(Vector3 p_velocity) {
	// Intentionally not checking for equality of the parameter.
	// We need to always submit the velocity to the navigation server, even when it is the same, in order to run avoidance every frame.
	// Revisit later when the navigation server can update avoidance without users resubmitting the velocity.

	velocity_forced = p_velocity;
	velocity_forced_submitted = true;
}

void NavigationAgent::set_velocity(const Vector3 p_velocity) {
	velocity = p_velocity;
	velocity_submitted = true;
}

void NavigationAgent::_avoidance_done(Vector3 p_new_velocity) {
	safe_velocity = p_new_velocity;

	if (!use_3d_avoidance) {
		safe_velocity.y = stored_y_velocity;
	}

	emit_signal("velocity_computed", safe_velocity);
}

String NavigationAgent::get_configuration_warning() const {
	if (!Object::cast_to<Spatial>(get_parent())) {
		return TTR("The NavigationAgent can be used only under a Spatial inheriting parent node.");
	}

	return String();
}

void NavigationAgent::update_navigation() {
	if (agent_parent == nullptr) {
		return;
	}

	if (!agent_parent->is_inside_tree()) {
		return;
	}

	if (!target_position_submitted) {
		return;
	}

	if (update_frame_id == Engine::get_singleton()->get_physics_frames()) {
		return;
	}

	update_frame_id = Engine::get_singleton()->get_physics_frames();

	Vector3 origin = agent_parent->get_global_transform().origin;

	bool reload_path = false;

	if (NavigationServer::get_singleton()->agent_is_map_changed(agent)) {
		reload_path = true;
	} else if (navigation_result->get_path().size() == 0) {
		reload_path = true;
	} else {
		// Check if too far from the navigation path
		if (nav_path_index > 0) {
			const Vector<Vector3> &navigation_path = navigation_result->get_path();

			Vector3 segment[2];
			segment[0] = navigation_path[nav_path_index - 1];
			segment[1] = navigation_path[nav_path_index];
			segment[0].y -= path_height_offset;
			segment[1].y -= path_height_offset;
			Vector3 p = Geometry::get_closest_point_to_segment(origin, segment);

			if (origin.distance_to(p) >= path_max_distance) {
				// To faraway, reload path
				reload_path = true;
			}
		}
	}

	if (reload_path) {
		navigation_query->set_start_position(origin);
		navigation_query->set_target_position(target_position);
		navigation_query->set_navigation_layers(navigation_layers);
		navigation_query->set_metadata_flags(path_metadata_flags);
		navigation_query->set_map(get_navigation_map());

		NavigationServer::get_singleton()->query_path(navigation_query, navigation_result);

#ifdef DEBUG_ENABLED
		debug_path_dirty = true;
#endif // DEBUG_ENABLED

		navigation_finished = false;
		nav_path_index = 0;
		emit_signal("path_changed");
	}

	if (navigation_result->get_path().size() == 0) {
		return;
	}

	// Check if we can advance the navigation path
	if (navigation_finished == false) {
		// Advances to the next far away position.
		const Vector<Vector3> &navigation_path = navigation_result->get_path();
		const Vector<int32_t> &navigation_path_types = navigation_result->get_path_types();
		const Array &navigation_path_rids = navigation_result->get_path_rids();
		const Vector<uint64_t> &navigation_path_owners = navigation_result->get_path_owner_ids();

		while (origin.distance_to(navigation_path[nav_path_index] - Vector3(0, path_height_offset, 0)) < path_desired_distance) {
			Dictionary details;

			const Vector3 waypoint = navigation_path[nav_path_index];
			details["location"] = waypoint;

			int waypoint_type = -1;
			if ((path_metadata_flags & NavigationPathQueryParameters3D::PathMetadataFlags::PATH_METADATA_INCLUDE_TYPES) != 0) {
				const NavigationPathQueryResult3D::PathSegmentType type = NavigationPathQueryResult3D::PathSegmentType(navigation_path_types[nav_path_index]);

				details["type"] = type;
				waypoint_type = type;
			}

			if ((path_metadata_flags & NavigationPathQueryParameters3D::PathMetadataFlags::PATH_METADATA_INCLUDE_RIDS) != 0) {
				details["rid"] = navigation_path_rids[nav_path_index];
			}

			if ((path_metadata_flags & NavigationPathQueryParameters3D::PathMetadataFlags::PATH_METADATA_INCLUDE_OWNERS) != 0) {
				const ObjectID waypoint_owner_id = ObjectID(navigation_path_owners[nav_path_index]);

				// Get a reference to the owning object.
				Object *owner = nullptr;
				if (waypoint_owner_id != 0) {
					owner = ObjectDB::get_instance(waypoint_owner_id);
				}

				details["owner"] = owner;

				if (waypoint_type == NavigationPathQueryResult3D::PATH_SEGMENT_TYPE_LINK) {
					const NavigationLink3D *navlink = Object::cast_to<NavigationLink3D>(owner);
					if (navlink) {
						Vector3 link_global_start_position = navlink->get_global_start_position();
						Vector3 link_global_end_position = navlink->get_global_end_position();
						if (waypoint.distance_to(link_global_start_position) < waypoint.distance_to(link_global_end_position)) {
							details["link_entry_position"] = link_global_start_position;
							details["link_exit_position"] = link_global_end_position;
						} else {
							details["link_entry_position"] = link_global_end_position;
							details["link_exit_position"] = link_global_start_position;
						}
					}
				}
			}

			// Emit a signal for the waypoint
			emit_signal("waypoint_reached", details);

			// Emit a signal if we've reached a navigation link
			if (waypoint_type == NavigationPathQueryResult3D::PATH_SEGMENT_TYPE_LINK) {
				emit_signal("link_reached", details);
			}

			// Move to the next waypoint on the list

			nav_path_index += 1;

			// Check to see if we've finished our route
			if (nav_path_index == navigation_path.size()) {
				_check_distance_to_target();
				nav_path_index -= 1;
				navigation_finished = true;
				target_position_submitted = false;
				emit_signal("navigation_finished");
				break;
			}
		}
	}
}

void NavigationAgent::set_avoidance_layers(uint32_t p_layers) {
	avoidance_layers = p_layers;
	NavigationServer::get_singleton()->agent_set_avoidance_layers(get_rid(), avoidance_layers);
}

uint32_t NavigationAgent::get_avoidance_layers() const {
	return avoidance_layers;
}

void NavigationAgent::set_avoidance_mask(uint32_t p_mask) {
	avoidance_mask = p_mask;
	NavigationServer::get_singleton()->agent_set_avoidance_mask(get_rid(), avoidance_mask);
}

uint32_t NavigationAgent::get_avoidance_mask() const {
	return avoidance_mask;
}

void NavigationAgent::set_avoidance_layer_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Avoidance layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Avoidance layer number must be between 1 and 32 inclusive.");
	uint32_t avoidance_layers_new = get_avoidance_layers();
	if (p_value) {
		avoidance_layers_new |= 1 << (p_layer_number - 1);
	} else {
		avoidance_layers_new &= ~(1 << (p_layer_number - 1));
	}
	set_avoidance_layers(avoidance_layers_new);
}

bool NavigationAgent::get_avoidance_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	return get_avoidance_layers() & (1 << (p_layer_number - 1));
}

void NavigationAgent::set_avoidance_mask_value(int p_mask_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_mask_number < 1, "Avoidance mask number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_mask_number > 32, "Avoidance mask number must be between 1 and 32 inclusive.");
	uint32_t mask = get_avoidance_mask();
	if (p_value) {
		mask |= 1 << (p_mask_number - 1);
	} else {
		mask &= ~(1 << (p_mask_number - 1));
	}
	set_avoidance_mask(mask);
}

bool NavigationAgent::get_avoidance_mask_value(int p_mask_number) const {
	ERR_FAIL_COND_V_MSG(p_mask_number < 1, false, "Avoidance mask number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_mask_number > 32, false, "Avoidance mask number must be between 1 and 32 inclusive.");
	return get_avoidance_mask() & (1 << (p_mask_number - 1));
}

void NavigationAgent::set_avoidance_priority(real_t p_priority) {
	ERR_FAIL_COND_MSG(p_priority < 0.0, "Avoidance priority must be between 0.0 and 1.0 inclusive.");
	ERR_FAIL_COND_MSG(p_priority > 1.0, "Avoidance priority must be between 0.0 and 1.0 inclusive.");
	avoidance_priority = p_priority;
	NavigationServer::get_singleton()->agent_set_avoidance_priority(get_rid(), p_priority);
}

real_t NavigationAgent::get_avoidance_priority() const {
	return avoidance_priority;
}

void NavigationAgent::_request_repath() {
	navigation_result->reset();
	target_reached = false;
	navigation_finished = false;
	update_frame_id = 0;
}

void NavigationAgent::_check_distance_to_target() {
	if (!target_reached) {
		if (distance_to_target() < target_desired_distance) {
			target_reached = true;
			emit_signal("target_reached");
		}
	}
}

////////DEBUG////////////////////////////////////////////////////////////

#ifdef DEBUG_ENABLED
void NavigationAgent::set_debug_enabled(bool p_enabled) {
	debug_enabled = p_enabled;
	debug_path_dirty = true;
}

bool NavigationAgent::get_debug_enabled() const {
	return debug_enabled;
}

void NavigationAgent::set_debug_use_custom(bool p_enabled) {
	debug_use_custom = p_enabled;
	debug_path_dirty = true;
}

bool NavigationAgent::get_debug_use_custom() const {
	return debug_use_custom;
}

void NavigationAgent::set_debug_path_custom_color(Color p_color) {
	debug_path_custom_color = p_color;
	debug_path_dirty = true;
}

Color NavigationAgent::get_debug_path_custom_color() const {
	return debug_path_custom_color;
}

void NavigationAgent::set_debug_path_custom_point_size(float p_point_size) {
	debug_path_custom_point_size = MAX(0.0, p_point_size);
	;
	debug_path_dirty = true;
}

float NavigationAgent::get_debug_path_custom_point_size() const {
	return debug_path_custom_point_size;
}

void NavigationAgent::_navigation_debug_changed() {
	debug_path_dirty = true;
}

void NavigationAgent::_update_debug_path() {
	if (!debug_path_dirty) {
		return;
	}
	debug_path_dirty = false;

	if (!debug_path_instance.is_valid()) {
		debug_path_instance = RenderingServer::get_singleton()->instance_create();
	}

	if (!debug_path_mesh.is_valid()) {
		debug_path_mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	}

	debug_path_mesh->clear_surfaces();

	if (!(debug_enabled && NavigationServer::get_singleton()->get_debug_navigation_enable_agent_paths())) {
		return;
	}

	if (!(agent_parent && agent_parent->is_inside_tree())) {
		return;
	}

	const Vector<Vector3> &navigation_path = navigation_result->get_path();

	if (navigation_path.size() <= 1) {
		return;
	}

	Vector<Vector3> debug_path_lines_vertex_array;

	for (int i = 0; i < navigation_path.size() - 1; i++) {
		debug_path_lines_vertex_array.push_back(navigation_path[i]);
		debug_path_lines_vertex_array.push_back(navigation_path[i + 1]);
	}

	Array debug_path_lines_mesh_array;
	debug_path_lines_mesh_array.resize(Mesh::ARRAY_MAX);
	debug_path_lines_mesh_array[Mesh::ARRAY_VERTEX] = debug_path_lines_vertex_array;

	debug_path_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, debug_path_lines_mesh_array);

	Ref<SpatialMaterial> debug_agent_path_line_material = NavigationServer::get_singleton()->get_debug_navigation_agent_path_line_material();
	if (debug_use_custom) {
		if (!debug_agent_path_line_custom_material.is_valid()) {
			debug_agent_path_line_custom_material = debug_agent_path_line_material->duplicate();
		}
		debug_agent_path_line_custom_material->set_albedo(debug_path_custom_color);
		debug_path_mesh->surface_set_material(0, debug_agent_path_line_custom_material);
	} else {
		debug_path_mesh->surface_set_material(0, debug_agent_path_line_material);
	}

	if (debug_path_custom_point_size > 0.0) {
		Vector<Vector3> debug_path_points_vertex_array;

		for (int i = 0; i < navigation_path.size(); i++) {
			debug_path_points_vertex_array.push_back(navigation_path[i]);
		}

		Array debug_path_points_mesh_array;
		debug_path_points_mesh_array.resize(Mesh::ARRAY_MAX);
		debug_path_points_mesh_array[Mesh::ARRAY_VERTEX] = debug_path_points_vertex_array;

		debug_path_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_POINTS, debug_path_points_mesh_array);

		Ref<SpatialMaterial> debug_agent_path_point_material = NavigationServer::get_singleton()->get_debug_navigation_agent_path_point_material();
		if (debug_use_custom) {
			if (!debug_agent_path_point_custom_material.is_valid()) {
				debug_agent_path_point_custom_material = debug_agent_path_point_material->duplicate();
			}
			debug_agent_path_point_custom_material->set_albedo(debug_path_custom_color);
			debug_agent_path_point_custom_material->set_point_size(debug_path_custom_point_size);
			debug_path_mesh->surface_set_material(1, debug_agent_path_point_custom_material);
		} else {
			debug_path_mesh->surface_set_material(1, debug_agent_path_point_material);
		}
	}

	RS::get_singleton()->instance_set_base(debug_path_instance, debug_path_mesh->get_rid());
	RS::get_singleton()->instance_set_scenario(debug_path_instance, agent_parent->get_world_3d()->get_scenario());
	RS::get_singleton()->instance_set_visible(debug_path_instance, agent_parent->is_visible_in_tree());
}
#endif // DEBUG_ENABLED