/*************************************************************************/
/*  navigation_obstacle.cpp                                              */
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

#include "navigation_obstacle.h"

#include "core/config/engine.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/navigation.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/spatial.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/shapes/shape.h"
#include "scene/resources/world_3d.h"
#include "servers/navigation_server.h"
#include "servers/rendering_server.h"

void NavigationObstacle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_rid"), &NavigationObstacle::get_rid);

	ClassDB::bind_method(D_METHOD("set_avoidance_enabled", "enabled"), &NavigationObstacle::set_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("get_avoidance_enabled"), &NavigationObstacle::get_avoidance_enabled);

	ClassDB::bind_method(D_METHOD("set_navigation_map", "navigation_map"), &NavigationObstacle::set_navigation_map);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &NavigationObstacle::get_navigation_map);

	ClassDB::bind_method(D_METHOD("set_navigation", "navigation"), &NavigationObstacle::set_navigation_node);
	ClassDB::bind_method(D_METHOD("get_navigation"), &NavigationObstacle::get_navigation_node);

	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &NavigationObstacle::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &NavigationObstacle::get_radius);

	ClassDB::bind_method(D_METHOD("set_height", "height"), &NavigationObstacle::set_height);
	ClassDB::bind_method(D_METHOD("get_height"), &NavigationObstacle::get_height);

	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &NavigationObstacle::set_velocity);
	ClassDB::bind_method(D_METHOD("get_velocity"), &NavigationObstacle::get_velocity);

	ClassDB::bind_method(D_METHOD("set_vertices", "vertices"), &NavigationObstacle::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertices"), &NavigationObstacle::get_vertices);

	ClassDB::bind_method(D_METHOD("set_avoidance_layers", "layers"), &NavigationObstacle::set_avoidance_layers);
	ClassDB::bind_method(D_METHOD("get_avoidance_layers"), &NavigationObstacle::get_avoidance_layers);

	ClassDB::bind_method(D_METHOD("set_avoidance_layer_value", "layer_number", "value"), &NavigationObstacle::set_avoidance_layer_value);
	ClassDB::bind_method(D_METHOD("get_avoidance_layer_value", "layer_number"), &NavigationObstacle::get_avoidance_layer_value);

	ClassDB::bind_method(D_METHOD("set_use_3d_avoidance", "enabled"), &NavigationObstacle::set_use_3d_avoidance);
	ClassDB::bind_method(D_METHOD("get_use_3d_avoidance"), &NavigationObstacle::get_use_3d_avoidance);

#ifdef DEBUG_ENABLED
	ClassDB::bind_method(D_METHOD("_update_fake_agent_radius_debug"), &NavigationObstacle::_update_fake_agent_radius_debug);
	ClassDB::bind_method(D_METHOD("_update_static_obstacle_debug"), &NavigationObstacle::_update_static_obstacle_debug);
#endif // DEBUG_ENABLED

	ADD_GROUP("Avoidance", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "avoidance_enabled"), "set_avoidance_enabled", "get_avoidance_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_velocity", "get_velocity");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius", PROPERTY_HINT_RANGE, "0.0,100,0.01,suffix:m"), "set_radius", "get_radius");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "height", PROPERTY_HINT_RANGE, "0.0,100,0.01,suffix:m"), "set_height", "get_height");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR3_ARRAY, "vertices"), "set_vertices", "get_vertices");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "avoidance_layers", PROPERTY_HINT_LAYERS_AVOIDANCE), "set_avoidance_layers", "get_avoidance_layers");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_3d_avoidance"), "set_use_3d_avoidance", "get_use_3d_avoidance");
}

void NavigationObstacle::_notification(int p_what) {
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

			_update_map(get_navigation_map());

			previous_transform = get_global_transform();
			// need to trigger map controlled agent assignment somehow for the fake_agent since obstacles use no callback like regular agents
			NavigationServer::get_singleton()->obstacle_set_avoidance_enabled(obstacle, avoidance_enabled);
			_update_position(get_global_transform().origin);

			set_physics_process_internal(true);

#ifdef DEBUG_ENABLED
			if ((NavigationServer::get_singleton()->get_debug_avoidance_enabled()) &&
					(NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_radius())) {
				_update_fake_agent_radius_debug();
				_update_static_obstacle_debug();
			}
#endif // DEBUG_ENABLED
		} break;
		case NOTIFICATION_PAUSED: {
			if (!can_process()) {
				map_before_pause = map_current;
				_update_map(RID());
			} else if (can_process() && !(map_before_pause == RID())) {
				_update_map(map_before_pause);
				map_before_pause = RID();
			}
			NavigationServer::get_singleton()->obstacle_set_paused(obstacle, !can_process());
		} break;
		case NOTIFICATION_UNPAUSED: {
			if (!can_process()) {
				map_before_pause = map_current;
				_update_map(RID());
			} else if (can_process() && !(map_before_pause == RID())) {
				_update_map(map_before_pause);
				map_before_pause = RID();
			}
			NavigationServer::get_singleton()->obstacle_set_paused(obstacle, !can_process());
		} break;
		case NOTIFICATION_EXIT_TREE: {
			set_navigation(nullptr);
			set_physics_process_internal(false);
			request_ready(); // required to solve an issue with losing the navigation

			_update_map(RID());
#ifdef DEBUG_ENABLED
			if (fake_agent_radius_debug_instance.is_valid()) {
				RS::get_singleton()->instance_set_visible(fake_agent_radius_debug_instance, false);
			}
			if (static_obstacle_debug_instance.is_valid()) {
				RS::get_singleton()->instance_set_visible(static_obstacle_debug_instance, false);
			}
#endif // DEBUG_ENABLED
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			if (is_inside_tree()) {
				_update_position(get_global_transform().origin);

				if (velocity_submitted) {
					velocity_submitted = false;
					// only update if there is a noticeable change, else the rvo agent preferred velocity stays the same
					if (!previous_velocity.is_equal_approx(velocity)) {
						NavigationServer::get_singleton()->obstacle_set_velocity(obstacle, velocity);
					}
					previous_velocity = velocity;
				}
#ifdef DEBUG_ENABLED
				if (fake_agent_radius_debug_instance.is_valid() && radius > 0.0) {
					RS::get_singleton()->instance_set_transform(fake_agent_radius_debug_instance, get_global_transform());
				}
				if (static_obstacle_debug_instance.is_valid() && get_vertices().size() > 0) {
					RS::get_singleton()->instance_set_transform(static_obstacle_debug_instance, get_global_transform());
				}
#endif // DEBUG_ENABLED
			}

		} break;
	}
}

NavigationObstacle::NavigationObstacle() {
	navigation = nullptr;

	height = 1.0;
	radius = 0.0;

	avoidance_layers = 1;

	use_3d_avoidance = false;

	velocity_submitted = false;
	avoidance_enabled = true;

	obstacle = NavigationServer::get_singleton()->obstacle_create();

	set_radius(radius);
	set_height(height);
	set_vertices(vertices);
	set_avoidance_layers(avoidance_layers);
	set_avoidance_enabled(avoidance_enabled);
	set_use_3d_avoidance(use_3d_avoidance);

#ifdef DEBUG_ENABLED
	NavigationServer::get_singleton()->connect("avoidance_debug_changed", this, "_update_fake_agent_radius_debug");
	NavigationServer::get_singleton()->connect("avoidance_debug_changed", this, "_update_static_obstacle_debug");
	_update_fake_agent_radius_debug();
	_update_static_obstacle_debug();
#endif // DEBUG_ENABLED
}

NavigationObstacle::~NavigationObstacle() {
	ERR_FAIL_NULL(NavigationServer::get_singleton());

	NavigationServer::get_singleton()->free(obstacle);
	obstacle = RID();

#ifdef DEBUG_ENABLED
	NavigationServer::get_singleton()->disconnect("avoidance_debug_changed", this, "_update_fake_agent_radius_debug");
	NavigationServer::get_singleton()->disconnect("avoidance_debug_changed", this, "_update_static_obstacle_debug");

	if (fake_agent_radius_debug_instance.is_valid()) {
		RenderingServer::get_singleton()->free(fake_agent_radius_debug_instance);
	}
	if (fake_agent_radius_debug_mesh.is_valid()) {
		RenderingServer::get_singleton()->free(fake_agent_radius_debug_mesh->get_rid());
	}

	if (static_obstacle_debug_instance.is_valid()) {
		RenderingServer::get_singleton()->free(static_obstacle_debug_instance);
	}
	if (static_obstacle_debug_mesh.is_valid()) {
		RenderingServer::get_singleton()->free(static_obstacle_debug_mesh->get_rid());
	}
#endif // DEBUG_ENABLED
}

void NavigationObstacle::set_navigation(Navigation *p_nav) {
	if (navigation == p_nav && navigation != nullptr) {
		return; // Pointless
	}

	navigation = p_nav;

	NavigationServer::get_singleton()->obstacle_set_map(obstacle, get_navigation_map());
}

void NavigationObstacle::set_navigation_node(Node *p_nav) {
	Navigation *nav = Object::cast_to<Navigation>(p_nav);
	ERR_FAIL_NULL(nav);
	set_navigation(nav);
}

Node *NavigationObstacle::get_navigation_node() const {
	return Object::cast_to<Node>(navigation);
}

void NavigationObstacle::set_navigation_map(RID p_navigation_map) {
	if (map_override == p_navigation_map) {
		return;
	}
	map_override = p_navigation_map;
	_update_map(get_navigation_map());
}

RID NavigationObstacle::get_navigation_map() const {
	if (navigation) {
		navigation->get_rid();
	} else if (map_override.is_valid()) {
		return map_override;
	} else if (is_inside_tree()) {
		return get_world_3d()->get_navigation_map();
	}
	return RID();
}

void NavigationObstacle::set_vertices(const Vector<Vector3> &p_vertices) {
	vertices = p_vertices;
	NavigationServer::get_singleton()->obstacle_set_vertices(obstacle, vertices);
#ifdef DEBUG_ENABLED
	_update_static_obstacle_debug();
#endif // DEBUG_ENABLED
}

void NavigationObstacle::set_radius(real_t p_radius) {
	ERR_FAIL_COND_MSG(p_radius < 0.0, "Radius must be positive.");
	if (Math::is_equal_approx(radius, p_radius)) {
		return;
	}

	radius = p_radius;
	NavigationServer::get_singleton()->obstacle_set_radius(obstacle, radius);

#ifdef DEBUG_ENABLED
	_update_fake_agent_radius_debug();
#endif // DEBUG_ENABLED
}

void NavigationObstacle::set_height(real_t p_height) {
	ERR_FAIL_COND_MSG(p_height < 0.0, "Height must be positive.");
	if (Math::is_equal_approx(height, p_height)) {
		return;
	}

	height = p_height;
	NavigationServer::get_singleton()->obstacle_set_height(obstacle, height);

#ifdef DEBUG_ENABLED
	_update_static_obstacle_debug();
#endif // DEBUG_ENABLED
}

void NavigationObstacle::set_avoidance_layers(uint32_t p_layers) {
	avoidance_layers = p_layers;
	NavigationServer::get_singleton()->obstacle_set_avoidance_layers(obstacle, avoidance_layers);
}

uint32_t NavigationObstacle::get_avoidance_layers() const {
	return avoidance_layers;
}

void NavigationObstacle::set_avoidance_layer_value(int p_layer_number, bool p_value) {
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

bool NavigationObstacle::get_avoidance_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	return get_avoidance_layers() & (1 << (p_layer_number - 1));
}

void NavigationObstacle::set_avoidance_enabled(bool p_enabled) {
	if (avoidance_enabled == p_enabled) {
		return;
	}

	avoidance_enabled = p_enabled;
	NavigationServer::get_singleton()->obstacle_set_avoidance_enabled(obstacle, avoidance_enabled);
}

bool NavigationObstacle::get_avoidance_enabled() const {
	return avoidance_enabled;
}

void NavigationObstacle::set_use_3d_avoidance(bool p_use_3d_avoidance) {
	use_3d_avoidance = p_use_3d_avoidance;
	_update_use_3d_avoidance(use_3d_avoidance);
	property_list_changed_notify();
}

void NavigationObstacle::set_velocity(const Vector3 p_velocity) {
	velocity = p_velocity;
	velocity_submitted = true;
}

void NavigationObstacle::_update_map(RID p_map) {
	NavigationServer::get_singleton()->obstacle_set_map(obstacle, p_map);
	map_current = p_map;
}

void NavigationObstacle::_update_position(const Vector3 p_position) {
	NavigationServer::get_singleton()->obstacle_set_position(obstacle, p_position);
}

void NavigationObstacle::_update_use_3d_avoidance(bool p_use_3d_avoidance) {
	NavigationServer::get_singleton()->obstacle_set_use_3d_avoidance(obstacle, use_3d_avoidance);
	_update_map(map_current);
}

#ifdef DEBUG_ENABLED
void NavigationObstacle::_update_fake_agent_radius_debug() {
	bool is_debug_enabled = false;
	if (Engine::get_singleton()->is_editor_hint()) {
		is_debug_enabled = true;
	} else if (NavigationServer::get_singleton()->get_debug_enabled() &&
			NavigationServer::get_singleton()->get_debug_avoidance_enabled() &&
			NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_radius()) {
		is_debug_enabled = true;
	}

	if (is_debug_enabled == false) {
		if (fake_agent_radius_debug_instance.is_valid()) {
			RS::get_singleton()->instance_set_visible(fake_agent_radius_debug_instance, false);
		}
		return;
	}

	if (!fake_agent_radius_debug_instance.is_valid()) {
		fake_agent_radius_debug_instance = RenderingServer::get_singleton()->instance_create();
	}
	if (!fake_agent_radius_debug_mesh.is_valid()) {
		fake_agent_radius_debug_mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	}
	fake_agent_radius_debug_mesh->clear_surfaces();

	Vector<Vector3> face_vertex_array;
	Vector<int> face_indices_array;

	int i, j, prevrow, thisrow, point;
	float x, y, z;

	int rings = 16;
	int radial_segments = 32;

	point = 0;

	thisrow = 0;
	prevrow = 0;
	for (j = 0; j <= (rings + 1); j++) {
		float v = j;
		float w;

		v /= (rings + 1);
		w = sin(Math_PI * v);
		y = (radius)*cos(Math_PI * v);

		for (i = 0; i <= radial_segments; i++) {
			float u = i;
			u /= radial_segments;

			x = sin(u * Math_TAU);
			z = cos(u * Math_TAU);

			Vector3 p = Vector3(x * radius * w, y, z * radius * w);
			face_vertex_array.push_back(p);

			point++;

			if (i > 0 && j > 0) {
				face_indices_array.push_back(prevrow + i - 1);
				face_indices_array.push_back(prevrow + i);
				face_indices_array.push_back(thisrow + i - 1);

				face_indices_array.push_back(prevrow + i);
				face_indices_array.push_back(thisrow + i);
				face_indices_array.push_back(thisrow + i - 1);
			};
		};

		prevrow = thisrow;
		thisrow = point;
	};

	Array face_mesh_array;
	face_mesh_array.resize(Mesh::ARRAY_MAX);
	face_mesh_array[Mesh::ARRAY_VERTEX] = face_vertex_array;
	face_mesh_array[Mesh::ARRAY_INDEX] = face_indices_array;

	fake_agent_radius_debug_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, face_mesh_array);
	Ref<SpatialMaterial> face_material = NavigationServer::get_singleton()->get_debug_navigation_avoidance_obstacles_radius_material();
	fake_agent_radius_debug_mesh->surface_set_material(0, face_material);

	RS::get_singleton()->instance_set_base(fake_agent_radius_debug_instance, fake_agent_radius_debug_mesh->get_rid());
	if (is_inside_tree()) {
		RS::get_singleton()->instance_set_scenario(fake_agent_radius_debug_instance, get_world_3d()->get_scenario());
		RS::get_singleton()->instance_set_visible(fake_agent_radius_debug_instance, is_visible_in_tree());
	}
}
#endif // DEBUG_ENABLED

#ifdef DEBUG_ENABLED
void NavigationObstacle::_update_static_obstacle_debug() {
	bool is_debug_enabled = false;
	if (Engine::get_singleton()->is_editor_hint()) {
		is_debug_enabled = true;
	} else if (NavigationServer::get_singleton()->get_debug_enabled() &&
			NavigationServer::get_singleton()->get_debug_avoidance_enabled() &&
			NavigationServer::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_static()) {
		is_debug_enabled = true;
	}

	if (is_debug_enabled == false) {
		if (static_obstacle_debug_instance.is_valid()) {
			RS::get_singleton()->instance_set_visible(static_obstacle_debug_instance, false);
		}
		return;
	}

	if (vertices.size() < 3) {
		if (static_obstacle_debug_instance.is_valid()) {
			RS::get_singleton()->instance_set_visible(static_obstacle_debug_instance, false);
		}
		return;
	}

	if (!static_obstacle_debug_instance.is_valid()) {
		static_obstacle_debug_instance = RenderingServer::get_singleton()->instance_create();
	}
	if (!static_obstacle_debug_mesh.is_valid()) {
		static_obstacle_debug_mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	}
	static_obstacle_debug_mesh->clear_surfaces();

	Vector<Vector2> polygon_2d_vertices;
	polygon_2d_vertices.resize(vertices.size());
	Vector2 *polygon_2d_vertices_ptr = polygon_2d_vertices.ptrw();

	for (int i = 0; i < vertices.size(); ++i) {
		Vector3 obstacle_vertex = vertices[i];
		Vector2 obstacle_vertex_2d = Vector2(obstacle_vertex.x, obstacle_vertex.z);
		polygon_2d_vertices_ptr[i] = obstacle_vertex_2d;
	}

	Vector<int> triangulated_polygon_2d_indices = Geometry::triangulate_polygon(polygon_2d_vertices);

	if (triangulated_polygon_2d_indices.empty()) {
		// failed triangulation
		return;
	}

	bool obstacle_pushes_inward = Geometry::is_polygon_clockwise(polygon_2d_vertices);

	Vector<Vector3> face_vertex_array;
	Vector<int> face_indices_array;

	face_vertex_array.resize(polygon_2d_vertices.size());
	face_indices_array.resize(triangulated_polygon_2d_indices.size());

	Vector3 *face_vertex_array_ptr = face_vertex_array.ptrw();
	int *face_indices_array_ptr = face_indices_array.ptrw();

	for (int i = 0; i < triangulated_polygon_2d_indices.size(); ++i) {
		int vertex_index = triangulated_polygon_2d_indices[i];
		const Vector2 &vertex_2d = polygon_2d_vertices[vertex_index];
		Vector3 vertex_3d = Vector3(vertex_2d.x, 0.0, vertex_2d.y);
		face_vertex_array_ptr[vertex_index] = vertex_3d;
		face_indices_array_ptr[i] = vertex_index;
	}

	Array face_mesh_array;
	face_mesh_array.resize(Mesh::ARRAY_MAX);
	face_mesh_array[Mesh::ARRAY_VERTEX] = face_vertex_array;
	face_mesh_array[Mesh::ARRAY_INDEX] = face_indices_array;

	static_obstacle_debug_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, face_mesh_array);

	Vector<Vector3> edge_vertex_array;

	for (int i = 0; i < polygon_2d_vertices.size(); ++i) {
		int from_index = i - 1;
		int to_index = i;

		if (i == 0) {
			from_index = polygon_2d_vertices.size() - 1;
		}

		const Vector2 &vertex_2d_from = polygon_2d_vertices[from_index];
		const Vector2 &vertex_2d_to = polygon_2d_vertices[to_index];

		Vector3 vertex_3d_ground_from = Vector3(vertex_2d_from.x, 0.0, vertex_2d_from.y);
		Vector3 vertex_3d_ground_to = Vector3(vertex_2d_to.x, 0.0, vertex_2d_to.y);

		edge_vertex_array.push_back(vertex_3d_ground_from);
		edge_vertex_array.push_back(vertex_3d_ground_to);

		Vector3 vertex_3d_height_from = Vector3(vertex_2d_from.x, height, vertex_2d_from.y);
		Vector3 vertex_3d_height_to = Vector3(vertex_2d_to.x, height, vertex_2d_to.y);

		edge_vertex_array.push_back(vertex_3d_height_from);
		edge_vertex_array.push_back(vertex_3d_height_to);

		edge_vertex_array.push_back(vertex_3d_ground_from);
		edge_vertex_array.push_back(vertex_3d_height_from);
	}

	Array edge_mesh_array;
	edge_mesh_array.resize(Mesh::ARRAY_MAX);
	edge_mesh_array[Mesh::ARRAY_VERTEX] = edge_vertex_array;

	static_obstacle_debug_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, edge_mesh_array);

	Ref<SpatialMaterial> face_material;
	Ref<SpatialMaterial> edge_material;

	if (obstacle_pushes_inward) {
		face_material = NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_face_material();
		edge_material = NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_edge_material();
	} else {
		face_material = NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_material();
		edge_material = NavigationServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_edge_material();
	}

	static_obstacle_debug_mesh->surface_set_material(0, face_material);
	static_obstacle_debug_mesh->surface_set_material(1, edge_material);

	RS::get_singleton()->instance_set_base(static_obstacle_debug_instance, static_obstacle_debug_mesh->get_rid());
	if (is_inside_tree()) {
		RS::get_singleton()->instance_set_scenario(static_obstacle_debug_instance, get_world_3d()->get_scenario());
		RS::get_singleton()->instance_set_visible(static_obstacle_debug_instance, is_visible_in_tree());
	}
}
#endif // DEBUG_ENABLED
