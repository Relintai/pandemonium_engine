/*************************************************************************/
/*  navigation_link_2d.cpp                                               */
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

#include "navigation_link_2d.h"

#include "core/config/engine.h"
#include "core/math/geometry.h"
#include "scene/resources/world_2d.h"
#include "servers/navigation_2d_server.h"
#include "servers/navigation_server.h"

void NavigationLink2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &NavigationLink2D::set_enabled);
	ClassDB::bind_method(D_METHOD("is_enabled"), &NavigationLink2D::is_enabled);

	ClassDB::bind_method(D_METHOD("set_bidirectional", "bidirectional"), &NavigationLink2D::set_bidirectional);
	ClassDB::bind_method(D_METHOD("is_bidirectional"), &NavigationLink2D::is_bidirectional);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationLink2D::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationLink2D::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("set_navigation_layer_value", "layer_number", "value"), &NavigationLink2D::set_navigation_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_value", "layer_number"), &NavigationLink2D::get_navigation_layer_value);

	ClassDB::bind_method(D_METHOD("set_start_position", "position"), &NavigationLink2D::set_start_position);
	ClassDB::bind_method(D_METHOD("get_start_position"), &NavigationLink2D::get_start_position);

	ClassDB::bind_method(D_METHOD("set_end_position", "position"), &NavigationLink2D::set_end_position);
	ClassDB::bind_method(D_METHOD("get_end_position"), &NavigationLink2D::get_end_position);

	ClassDB::bind_method(D_METHOD("set_global_start_position", "position"), &NavigationLink2D::set_global_start_position);
	ClassDB::bind_method(D_METHOD("get_global_start_position"), &NavigationLink2D::get_global_start_position);

	ClassDB::bind_method(D_METHOD("set_global_end_position", "position"), &NavigationLink2D::set_global_end_position);
	ClassDB::bind_method(D_METHOD("get_global_end_position"), &NavigationLink2D::get_global_end_position);

	ClassDB::bind_method(D_METHOD("set_enter_cost", "enter_cost"), &NavigationLink2D::set_enter_cost);
	ClassDB::bind_method(D_METHOD("get_enter_cost"), &NavigationLink2D::get_enter_cost);

	ClassDB::bind_method(D_METHOD("set_travel_cost", "travel_cost"), &NavigationLink2D::set_travel_cost);
	ClassDB::bind_method(D_METHOD("get_travel_cost"), &NavigationLink2D::get_travel_cost);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "is_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bidirectional"), "set_bidirectional", "is_bidirectional");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_2D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "start_position"), "set_start_position", "get_start_position");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "end_position"), "set_end_position", "get_end_position");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "enter_cost"), "set_enter_cost", "get_enter_cost");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "travel_cost"), "set_travel_cost", "get_travel_cost");
}

void NavigationLink2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (enabled) {
				Navigation2DServer::get_singleton()->link_set_map(link, get_world_2d()->get_navigation_map());
			}
			current_global_transform = get_global_transform();
			Navigation2DServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));
			Navigation2DServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));
		} break;

		case NOTIFICATION_TRANSFORM_CHANGED: {
			set_physics_process_internal(true);
		} break;

		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			set_physics_process_internal(false);
			if (is_inside_tree()) {
				Transform2D new_global_transform = get_global_transform();
				if (current_global_transform != new_global_transform) {
					current_global_transform = new_global_transform;
					Navigation2DServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));
					Navigation2DServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));
					update();
				}
			}
		} break;

		case NOTIFICATION_EXIT_TREE: {
			Navigation2DServer::get_singleton()->link_set_map(link, RID());
		} break;
		case NOTIFICATION_DRAW: {
#ifdef DEBUG_ENABLED
			if (is_inside_tree() && (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled())) {
				Color color;
				if (enabled) {
					color = Navigation2DServer::get_singleton()->get_debug_navigation_link_connection_color();
				} else {
					color = Navigation2DServer::get_singleton()->get_debug_navigation_link_connection_disabled_color();
				}

				real_t radius = Navigation2DServer::get_singleton()->map_get_link_connection_radius(get_world_2d()->get_navigation_map());

				draw_line(get_start_position(), get_end_position(), color);
				draw_arc(get_start_position(), radius, 0, Math_TAU, 10, color);
				draw_arc(get_end_position(), radius, 0, Math_TAU, 10, color);
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

#ifdef TOOLS_ENABLED
Rect2 NavigationLink2D::_edit_get_rect() const {
	if (!is_inside_tree()) {
		return Rect2();
	}

	real_t radius = Navigation2DServer::get_singleton()->map_get_link_connection_radius(get_world_2d()->get_navigation_map());

	Rect2 rect(get_start_position(), Size2());
	rect.expand_to(get_end_position());
	rect.grow_by(radius);
	return rect;
}

bool NavigationLink2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	Point2 segment[2] = { get_start_position(), get_end_position() };

	Vector2 closest_point = Geometry::get_closest_point_to_segment_2d(p_point, segment);
	return p_point.distance_to(closest_point) < p_tolerance;
}
#endif // TOOLS_ENABLED

void NavigationLink2D::set_enabled(bool p_enabled) {
	if (enabled == p_enabled) {
		return;
	}

	enabled = p_enabled;

	Navigation2DServer::get_singleton()->link_set_enabled(link, enabled);

#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled()) {
		update();
	}
#endif // DEBUG_ENABLED
}

void NavigationLink2D::set_bidirectional(bool p_bidirectional) {
	if (bidirectional == p_bidirectional) {
		return;
	}

	bidirectional = p_bidirectional;

	Navigation2DServer::get_singleton()->link_set_bidirectional(link, bidirectional);
}

void NavigationLink2D::set_navigation_layers(uint32_t p_navigation_layers) {
	if (navigation_layers == p_navigation_layers) {
		return;
	}

	navigation_layers = p_navigation_layers;

	Navigation2DServer::get_singleton()->link_set_navigation_layers(link, navigation_layers);
}

void NavigationLink2D::set_navigation_layer_value(int p_layer_number, bool p_value) {
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

bool NavigationLink2D::get_navigation_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");

	return get_navigation_layers() & (1 << (p_layer_number - 1));
}

void NavigationLink2D::set_start_position(Vector2 p_position) {
	if (start_position.is_equal_approx(p_position)) {
		return;
	}

	start_position = p_position;

	if (!is_inside_tree()) {
		return;
	}

	Navigation2DServer::get_singleton()->link_set_start_position(link, current_global_transform.xform(start_position));

	update_configuration_warning();

#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled()) {
		update();
	}
#endif // DEBUG_ENABLED
}

void NavigationLink2D::set_end_position(Vector2 p_position) {
	if (end_position.is_equal_approx(p_position)) {
		return;
	}

	end_position = p_position;

	if (!is_inside_tree()) {
		return;
	}

	Navigation2DServer::get_singleton()->link_set_end_position(link, current_global_transform.xform(end_position));

	update_configuration_warning();

#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled()) {
		update();
	}
#endif // DEBUG_ENABLED
}

void NavigationLink2D::set_global_start_position(Vector2 p_position) {
	if (is_inside_tree()) {
		set_start_position(to_local(p_position));
	} else {
		set_start_position(p_position);
	}
}

Vector2 NavigationLink2D::get_global_start_position() const {
	if (is_inside_tree()) {
		return to_global(start_position);
	} else {
		return start_position;
	}
}

void NavigationLink2D::set_global_end_position(Vector2 p_position) {
	if (is_inside_tree()) {
		set_end_position(to_local(p_position));
	} else {
		set_end_position(p_position);
	}
}

Vector2 NavigationLink2D::get_global_end_position() const {
	if (is_inside_tree()) {
		return to_global(end_position);
	} else {
		return end_position;
	}
}

void NavigationLink2D::set_enter_cost(real_t p_enter_cost) {
	ERR_FAIL_COND_MSG(p_enter_cost < 0.0, "The enter_cost must be positive.");
	if (Math::is_equal_approx(enter_cost, p_enter_cost)) {
		return;
	}

	enter_cost = p_enter_cost;

	Navigation2DServer::get_singleton()->link_set_enter_cost(link, enter_cost);
}

void NavigationLink2D::set_travel_cost(real_t p_travel_cost) {
	ERR_FAIL_COND_MSG(p_travel_cost < 0.0, "The travel_cost must be positive.");
	if (Math::is_equal_approx(travel_cost, p_travel_cost)) {
		return;
	}

	travel_cost = p_travel_cost;

	Navigation2DServer::get_singleton()->link_set_travel_cost(link, travel_cost);
}

String NavigationLink2D::get_configuration_warning() const {
	String warnings = Node::get_configuration_warning();

	if (start_position.is_equal_approx(end_position)) {
		warnings += RTR("NavigationLink2D start position should be different than the end position to be useful.");
	}

	return warnings;
}

NavigationLink2D::NavigationLink2D() {
	enabled = true;
	bidirectional = true;
	navigation_layers = 1;
	enter_cost = 0.0;
	travel_cost = 1.0;

	link = Navigation2DServer::get_singleton()->link_create();
	Navigation2DServer::get_singleton()->link_set_owner_id(link, get_instance_id());

	set_notify_transform(true);
}

NavigationLink2D::~NavigationLink2D() {
	ERR_FAIL_NULL(Navigation2DServer::get_singleton());
	Navigation2DServer::get_singleton()->free(link);
	link = RID();
}
