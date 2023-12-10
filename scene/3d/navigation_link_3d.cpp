/**************************************************************************/
/*  navigation_link_3d.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "navigation_link_3d.h"

#include "core/config/engine.h"
#include "mesh_instance.h"
#include "scene/resources/mesh/mesh.h"
#include "scene/resources/world_3d.h"
#include "servers/navigation_server.h"
#include "scene/resources/material/spatial_material.h"

#ifdef DEBUG_ENABLED
void NavigationLink3D::_update_debug_mesh() {
	if (!is_inside_tree()) {
		return;
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		// don't update inside Editor as node 3d gizmo takes care of this
		// as collisions and selections for Editor Viewport need to be updated
		return;
	}

	if (!NavigationServer::get_singleton()->get_debug_navigation_enabled()) {
		if (debug_instance.is_valid()) {
			RS::get_singleton()->instance_set_visible(debug_instance, false);
		}
		return;
	}

	if (!debug_instance.is_valid()) {
		debug_instance = RenderingServer::get_singleton()->instance_create();
	}

	if (!debug_mesh.is_valid()) {
		debug_mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	}

	RID nav_map = get_world_3d()->get_navigation_map();
	real_t search_radius = NavigationServer::get_singleton()->map_get_link_connection_radius(nav_map);
	Vector3 up_vector = NavigationServer::get_singleton()->map_get_up(nav_map);
	Vector3::Axis up_axis = static_cast<Vector3::Axis>(up_vector.max_axis());

	debug_mesh->clear_surfaces();

	Vector<Vector3> lines;

	// Draw line between the points.
	lines.push_back(start_position);
	lines.push_back(end_position);

	// Draw start position search radius
	for (int i = 0; i < 30; i++) {
		// Create a circle
		const float ra = Math::deg2rad((float)(i * 12));
		const float rb = Math::deg2rad((float)((i + 1) * 12));
		const Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * search_radius;
		const Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * search_radius;

		// Draw axis-aligned circle
		switch (up_axis) {
			case Vector3::AXIS_X:
				lines.push_back(start_position + Vector3(0, a.x, a.y));
				lines.push_back(start_position + Vector3(0, b.x, b.y));
				break;
			case Vector3::AXIS_Y:
				lines.push_back(start_position + Vector3(a.x, 0, a.y));
				lines.push_back(start_position + Vector3(b.x, 0, b.y));
				break;
			case Vector3::AXIS_Z:
				lines.push_back(start_position + Vector3(a.x, a.y, 0));
				lines.push_back(start_position + Vector3(b.x, b.y, 0));
				break;
		}
	}

	// Draw end position search radius
	for (int i = 0; i < 30; i++) {
		// Create a circle
		const float ra = Math::deg2rad((float)(i * 12));
		const float rb = Math::deg2rad((float)((i + 1) * 12));
		const Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * search_radius;
		const Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * search_radius;

		// Draw axis-aligned circle
		switch (up_axis) {
			case Vector3::AXIS_X:
				lines.push_back(end_position + Vector3(0, a.x, a.y));
				lines.push_back(end_position + Vector3(0, b.x, b.y));
				break;
			case Vector3::AXIS_Y:
				lines.push_back(end_position + Vector3(a.x, 0, a.y));
				lines.push_back(end_position + Vector3(b.x, 0, b.y));
				break;
			case Vector3::AXIS_Z:
				lines.push_back(end_position + Vector3(a.x, a.y, 0));
				lines.push_back(end_position + Vector3(b.x, b.y, 0));
				break;
		}
	}

	Array mesh_array;
	mesh_array.resize(Mesh::ARRAY_MAX);
	mesh_array[Mesh::ARRAY_VERTEX] = lines;

	debug_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, mesh_array);

	RS::get_singleton()->instance_set_base(debug_instance, debug_mesh->get_rid());
	RS::get_singleton()->instance_set_scenario(debug_instance, get_world_3d()->get_scenario());
	RS::get_singleton()->instance_set_visible(debug_instance, is_visible_in_tree());

	Ref<SpatialMaterial> link_material = NavigationServer::get_singleton()->get_debug_navigation_link_connections_material();
	Ref<SpatialMaterial> disabled_link_material = NavigationServer::get_singleton()->get_debug_navigation_link_connections_disabled_material();

	if (enabled) {
		RS::get_singleton()->instance_set_surface_material(debug_instance, 0, link_material->get_rid());
	} else {
		RS::get_singleton()->instance_set_surface_material(debug_instance, 0, disabled_link_material->get_rid());
	}

	RS::get_singleton()->instance_set_transform(debug_instance, current_global_transform);
}
#endif // DEBUG_ENABLED

void NavigationLink3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &NavigationLink3D::set_enabled);
	ClassDB::bind_method(D_METHOD("is_enabled"), &NavigationLink3D::is_enabled);

	ClassDB::bind_method(D_METHOD("set_bidirectional", "bidirectional"), &NavigationLink3D::set_bidirectional);
	ClassDB::bind_method(D_METHOD("is_bidirectional"), &NavigationLink3D::is_bidirectional);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationLink3D::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationLink3D::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("set_navigation_layer_value", "layer_number", "value"), &NavigationLink3D::set_navigation_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_value", "layer_number"), &NavigationLink3D::get_navigation_layer_value);

	ClassDB::bind_method(D_METHOD("set_start_position", "position"), &NavigationLink3D::set_start_position);
	ClassDB::bind_method(D_METHOD("get_start_position"), &NavigationLink3D::get_start_position);

	ClassDB::bind_method(D_METHOD("set_end_position", "position"), &NavigationLink3D::set_end_position);
	ClassDB::bind_method(D_METHOD("get_end_position"), &NavigationLink3D::get_end_position);

	ClassDB::bind_method(D_METHOD("set_global_start_position", "position"), &NavigationLink3D::set_global_start_position);
	ClassDB::bind_method(D_METHOD("get_global_start_position"), &NavigationLink3D::get_global_start_position);

	ClassDB::bind_method(D_METHOD("set_global_end_position", "position"), &NavigationLink3D::set_global_end_position);
	ClassDB::bind_method(D_METHOD("get_global_end_position"), &NavigationLink3D::get_global_end_position);

	ClassDB::bind_method(D_METHOD("set_enter_cost", "enter_cost"), &NavigationLink3D::set_enter_cost);
	ClassDB::bind_method(D_METHOD("get_enter_cost"), &NavigationLink3D::get_enter_cost);

	ClassDB::bind_method(D_METHOD("set_travel_cost", "travel_cost"), &NavigationLink3D::set_travel_cost);
	ClassDB::bind_method(D_METHOD("get_travel_cost"), &NavigationLink3D::get_travel_cost);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "is_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bidirectional"), "set_bidirectional", "is_bidirectional");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_3D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "start_position"), "set_start_position", "get_start_position");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "end_position"), "set_end_position", "get_end_position");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "enter_cost"), "set_enter_cost", "get_enter_cost");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "travel_cost"), "set_travel_cost", "get_travel_cost");
}

void NavigationLink3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (enabled) {
				NavigationServer::get_singleton()->link_set_map(link, get_world_3d()->get_navigation_map());
			}
			current_global_transform = get_global_transform();
			NavigationServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));
			NavigationServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));

#ifdef DEBUG_ENABLED
			_update_debug_mesh();
#endif // DEBUG_ENABLED
		} break;

		case NOTIFICATION_TRANSFORM_CHANGED: {
			set_physics_process_internal(true);
		} break;

		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			set_physics_process_internal(false);
			if (is_inside_tree()) {
				Transform new_global_transform = get_global_transform();
				if (current_global_transform != new_global_transform) {
					current_global_transform = new_global_transform;
					NavigationServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));
					NavigationServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));
#ifdef DEBUG_ENABLED
					if (debug_instance.is_valid()) {
						RS::get_singleton()->instance_set_transform(debug_instance, current_global_transform);
					}
#endif // DEBUG_ENABLED
				}
			}
		} break;

		case NOTIFICATION_EXIT_TREE: {
			NavigationServer::get_singleton()->link_set_map(link, RID());

#ifdef DEBUG_ENABLED
			if (debug_instance.is_valid()) {
				RS::get_singleton()->instance_set_scenario(debug_instance, RID());
				RS::get_singleton()->instance_set_visible(debug_instance, false);
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

NavigationLink3D::NavigationLink3D() {
	link = NavigationServer::get_singleton()->link_create();
	NavigationServer::get_singleton()->link_set_owner_id(link, get_instance_id());

	set_notify_transform(true);
}

NavigationLink3D::~NavigationLink3D() {
	ERR_FAIL_NULL(NavigationServer::get_singleton());
	NavigationServer::get_singleton()->free(link);
	link = RID();

#ifdef DEBUG_ENABLED
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	if (debug_instance.is_valid()) {
		RenderingServer::get_singleton()->free(debug_instance);
	}
	if (debug_mesh.is_valid()) {
		RenderingServer::get_singleton()->free(debug_mesh->get_rid());
	}
#endif // DEBUG_ENABLED
}

void NavigationLink3D::set_enabled(bool p_enabled) {
	if (enabled == p_enabled) {
		return;
	}

	enabled = p_enabled;

	NavigationServer::get_singleton()->link_set_enabled(link, enabled);

#ifdef DEBUG_ENABLED
	if (debug_instance.is_valid() && debug_mesh.is_valid()) {
		if (enabled) {
			Ref<SpatialMaterial> link_material = NavigationServer::get_singleton()->get_debug_navigation_link_connections_material();
			RS::get_singleton()->instance_set_surface_material(debug_instance, 0, link_material->get_rid());
		} else {
			Ref<SpatialMaterial> disabled_link_material = NavigationServer::get_singleton()->get_debug_navigation_link_connections_disabled_material();
			RS::get_singleton()->instance_set_surface_material(debug_instance, 0, disabled_link_material->get_rid());
		}
	}
#endif // DEBUG_ENABLED

	update_gizmos();
}

void NavigationLink3D::set_bidirectional(bool p_bidirectional) {
	if (bidirectional == p_bidirectional) {
		return;
	}

	bidirectional = p_bidirectional;

	NavigationServer::get_singleton()->link_set_bidirectional(link, bidirectional);
}

void NavigationLink3D::set_navigation_layers(uint32_t p_navigation_layers) {
	if (navigation_layers == p_navigation_layers) {
		return;
	}

	navigation_layers = p_navigation_layers;

	NavigationServer::get_singleton()->link_set_navigation_layers(link, navigation_layers);
}

void NavigationLink3D::set_navigation_layer_value(int p_layer_number, bool p_value) {
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

bool NavigationLink3D::get_navigation_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");

	return get_navigation_layers() & (1 << (p_layer_number - 1));
}

void NavigationLink3D::set_start_position(Vector3 p_position) {
	if (start_position.is_equal_approx(p_position)) {
		return;
	}

	start_position = p_position;

	if (!is_inside_tree()) {
		return;
	}

	NavigationServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));

#ifdef DEBUG_ENABLED
	_update_debug_mesh();
#endif // DEBUG_ENABLED

	update_gizmos();
	update_configuration_warning();
}

void NavigationLink3D::set_end_position(Vector3 p_position) {
	if (end_position.is_equal_approx(p_position)) {
		return;
	}

	end_position = p_position;

	if (!is_inside_tree()) {
		return;
	}

	NavigationServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));

#ifdef DEBUG_ENABLED
	_update_debug_mesh();
#endif // DEBUG_ENABLED

	update_gizmos();
	update_configuration_warning();
}

void NavigationLink3D::set_global_start_position(Vector3 p_position) {
	if (is_inside_tree()) {
		set_start_position(to_local(p_position));
	} else {
		set_start_position(p_position);
	}
}

Vector3 NavigationLink3D::get_global_start_position() const {
	if (is_inside_tree()) {
		return to_global(start_position);
	} else {
		return start_position;
	}
}

void NavigationLink3D::set_global_end_position(Vector3 p_position) {
	if (is_inside_tree()) {
		set_end_position(to_local(p_position));
	} else {
		set_end_position(p_position);
	}
}

Vector3 NavigationLink3D::get_global_end_position() const {
	if (is_inside_tree()) {
		return to_global(end_position);
	} else {
		return end_position;
	}
}

void NavigationLink3D::set_enter_cost(real_t p_enter_cost) {
	ERR_FAIL_COND_MSG(p_enter_cost < 0.0, "The enter_cost must be positive.");
	if (Math::is_equal_approx(enter_cost, p_enter_cost)) {
		return;
	}

	enter_cost = p_enter_cost;

	NavigationServer::get_singleton()->link_set_enter_cost(link, enter_cost);
}

void NavigationLink3D::set_travel_cost(real_t p_travel_cost) {
	ERR_FAIL_COND_MSG(p_travel_cost < 0.0, "The travel_cost must be positive.");
	if (Math::is_equal_approx(travel_cost, p_travel_cost)) {
		return;
	}

	travel_cost = p_travel_cost;

	NavigationServer::get_singleton()->link_set_travel_cost(link, travel_cost);
}

String NavigationLink3D::get_configuration_warning() const {
	String warnings = Node::get_configuration_warning();

	if (start_position.is_equal_approx(end_position)) {
		warnings += RTR("NavigationLink3D start position should be different than the end position to be useful.");
	}

	return warnings;
}
