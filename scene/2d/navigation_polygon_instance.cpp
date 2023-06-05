/*************************************************************************/
/*  navigation_polygon.cpp                                               */
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

#include "navigation_polygon_instance.h"

#include "core/config/engine.h"
#include "core/core_string_names.h"
#include "core/os/mutex.h"
#include "navigation_2d.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/navigation_polygon.h"
#include "scene/resources/world_2d.h"
#include "servers/navigation_2d_server.h"
#include "servers/navigation_server.h"

#include "thirdparty/misc/triangulator.h"

void NavigationPolygonInstance::set_enabled(bool p_enabled) {
	if (enabled == p_enabled) {
		return;
	}
	enabled = p_enabled;

	if (!is_inside_tree()) {
		return;
	}

	if (!enabled) {
		Navigation2DServer::get_singleton()->region_set_map(region, RID());
		Navigation2DServer::get_singleton_mut()->disconnect("map_changed", this, "_map_changed");
	} else {
		if (navigation != nullptr) {
			Navigation2DServer::get_singleton()->region_set_map(region, navigation->get_rid());
		} else {
			Navigation2DServer::get_singleton()->region_set_map(region, get_world_2d()->get_navigation_map());
		}
		Navigation2DServer::get_singleton_mut()->connect("map_changed", this, "_map_changed");
	}

#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint() || NavigationServer::get_singleton()->get_debug_enabled()) {
		update();
	}
#endif // DEBUG_ENABLED
}

bool NavigationPolygonInstance::is_enabled() const {
	return enabled;
}

void NavigationPolygonInstance::set_navigation_layers(uint32_t p_navigation_layers) {
	navigation_layers = p_navigation_layers;
	Navigation2DServer::get_singleton()->region_set_navigation_layers(region, navigation_layers);
}

uint32_t NavigationPolygonInstance::get_navigation_layers() const {
	return navigation_layers;
}

void NavigationPolygonInstance::set_enter_cost(real_t p_enter_cost) {
	ERR_FAIL_COND_MSG(p_enter_cost < 0.0, "The enter_cost must be positive.");
	enter_cost = MAX(p_enter_cost, 0.0);
	Navigation2DServer::get_singleton()->region_set_enter_cost(region, p_enter_cost);
}

real_t NavigationPolygonInstance::get_enter_cost() const {
	return enter_cost;
}

void NavigationPolygonInstance::set_travel_cost(real_t p_travel_cost) {
	ERR_FAIL_COND_MSG(p_travel_cost < 0.0, "The travel_cost must be positive.");
	travel_cost = MAX(p_travel_cost, 0.0);
	Navigation2DServer::get_singleton()->region_set_enter_cost(region, travel_cost);
}

real_t NavigationPolygonInstance::get_travel_cost() const {
	return travel_cost;
}

RID NavigationPolygonInstance::get_region_rid() const {
	return region;
}

/////////////////////////////
#ifdef TOOLS_ENABLED
Rect2 NavigationPolygonInstance::_edit_get_rect() const {
	return navpoly.is_valid() ? navpoly->_edit_get_rect() : Rect2();
}

bool NavigationPolygonInstance::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	return navpoly.is_valid() ? navpoly->_edit_is_selected_on_click(p_point, p_tolerance) : false;
}
#endif

void NavigationPolygonInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Node2D *c = this;
			while (c) {
				navigation = Object::cast_to<Navigation2D>(c);

				if (navigation) {
					if (enabled) {
						Navigation2DServer::get_singleton()->region_set_map(region, navigation->get_rid());
					}
					break;
				}

				c = Object::cast_to<Node2D>(c->get_parent());
			}

			if (enabled && navigation == nullptr) {
				// did not find a valid navigation node parent, fallback to default navigation map on world resource
				Navigation2DServer::get_singleton()->region_set_map(region, get_world_2d()->get_navigation_map());
			}
			if (enabled) {
				Navigation2DServer::get_singleton_mut()->connect("map_changed", this, "_map_changed");
			}
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			Navigation2DServer::get_singleton()->region_set_transform(region, get_global_transform());
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (navigation) {
				Navigation2DServer::get_singleton()->region_set_map(region, RID());
			}
			navigation = nullptr;

			if (enabled) {
				Navigation2DServer::get_singleton_mut()->disconnect("map_changed", this, "_map_changed");
			}
		} break;
		case NOTIFICATION_DRAW: {
#ifdef DEBUG_ENABLED
			if (is_inside_tree() && (Engine::get_singleton()->is_editor_hint() || NavigationServer::get_singleton()->get_debug_enabled()) && navpoly.is_valid()) {
				PoolVector<Vector2> verts = navpoly->get_vertices();
				if (verts.size() < 3) {
					return;
				}

				Color color;
				if (enabled) {
					color = NavigationServer::get_singleton()->get_debug_navigation_geometry_face_color();
				} else {
					color = NavigationServer::get_singleton()->get_debug_navigation_geometry_face_disabled_color();
				}

				Color doors_color = NavigationServer::get_singleton()->get_debug_navigation_edge_connection_color();

				RandomPCG rand;

				for (int i = 0; i < navpoly->get_polygon_count(); i++) {
					// An array of vertices for this polygon.
					Vector<int> polygon = navpoly->get_polygon(i);

					Vector<Vector2> vertices;
					vertices.resize(polygon.size());
					for (int j = 0; j < polygon.size(); j++) {
						ERR_FAIL_INDEX(polygon[j], verts.size());
						vertices.write[j] = verts[polygon[j]];
					}
					// Generate the polygon color, slightly randomly modified from the settings one.
					Color random_variation_color;
					random_variation_color.set_hsv(color.get_h() + rand.random(-1.0, 1.0) * 0.1, color.get_s(), color.get_v() + rand.random(-1.0, 1.0) * 0.2);
					random_variation_color.a = color.a;
					Vector<Color> colors;
					colors.push_back(random_variation_color);

					RS::get_singleton()->canvas_item_add_polygon(get_canvas_item(), vertices, colors);
				}

				// Draw the region
				Transform2D xform = get_global_transform();
				const Navigation2DServer *ns = Navigation2DServer::get_singleton();
				real_t radius = 1.0;
				if (navigation != nullptr) {
					radius = Navigation2DServer::get_singleton()->map_get_edge_connection_margin(navigation->get_rid());
				} else {
					radius = Navigation2DServer::get_singleton()->map_get_edge_connection_margin(get_world_2d()->get_navigation_map());
				}
				radius = radius * 0.5;
				for (int i = 0; i < ns->region_get_connections_count(region); i++) {
					// Two main points
					Vector2 a = ns->region_get_connection_pathway_start(region, i);
					a = xform.affine_inverse().xform(a);
					Vector2 b = ns->region_get_connection_pathway_end(region, i);
					b = xform.affine_inverse().xform(b);
					draw_line(a, b, doors_color);

					// Draw a circle to illustrate the margins.
					real_t angle = a.angle_to_point(b);
					draw_arc(a, radius, angle + Math_PI / 2.0, angle - Math_PI / 2.0 + Math_TAU, 10, doors_color);
					draw_arc(b, radius, angle - Math_PI / 2.0, angle + Math_PI / 2.0, 10, doors_color);
				}
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

void NavigationPolygonInstance::set_navigation_polygon(const Ref<NavigationPolygon> &p_navpoly) {
	if (p_navpoly == navpoly) {
		return;
	}

	if (navpoly.is_valid()) {
		navpoly->disconnect(CoreStringNames::get_singleton()->changed, this, "_navpoly_changed");
	}

	navpoly = p_navpoly;
	Navigation2DServer::get_singleton()->region_set_navpoly(region, p_navpoly);

	if (navpoly.is_valid()) {
		navpoly->connect(CoreStringNames::get_singleton()->changed, this, "_navpoly_changed");
	}
	_navpoly_changed();

	_change_notify("navpoly");
	update_configuration_warning();
}

Ref<NavigationPolygon> NavigationPolygonInstance::get_navigation_polygon() const {
	return navpoly;
}

void NavigationPolygonInstance::_navpoly_changed() {
	if (is_inside_tree() && (Engine::get_singleton()->is_editor_hint() || get_tree()->is_debugging_navigation_hint())) {
		update();
	}
	if (navpoly.is_valid()) {
		Navigation2DServer::get_singleton()->region_set_navpoly(region, navpoly);
	}
}

void NavigationPolygonInstance::_map_changed(RID p_map) {
#ifdef DEBUG_ENABLED
	if (navigation != nullptr && enabled && (navigation->get_rid() == p_map)) {
		update();
	} else if (is_inside_tree() && enabled && (get_world_2d()->get_navigation_map() == p_map)) {
		update();
	}
#endif // DEBUG_ENABLED
}

String NavigationPolygonInstance::get_configuration_warning() const {
	if (!is_visible_in_tree() || !is_inside_tree()) {
		return String();
	}

	String warning = Node2D::get_configuration_warning();
	if (!navpoly.is_valid()) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("A NavigationPolygon resource must be set or created for this node to work. Please set a property or draw a polygon.");
	}

	return warning;
}

void NavigationPolygonInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_navigation_polygon", "navpoly"), &NavigationPolygonInstance::set_navigation_polygon);
	ClassDB::bind_method(D_METHOD("get_navigation_polygon"), &NavigationPolygonInstance::get_navigation_polygon);

	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &NavigationPolygonInstance::set_enabled);
	ClassDB::bind_method(D_METHOD("is_enabled"), &NavigationPolygonInstance::is_enabled);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationPolygonInstance::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationPolygonInstance::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("get_region_rid"), &NavigationPolygonInstance::get_region_rid);

	ClassDB::bind_method(D_METHOD("set_enter_cost", "enter_cost"), &NavigationPolygonInstance::set_enter_cost);
	ClassDB::bind_method(D_METHOD("get_enter_cost"), &NavigationPolygonInstance::get_enter_cost);

	ClassDB::bind_method(D_METHOD("set_travel_cost", "travel_cost"), &NavigationPolygonInstance::set_travel_cost);
	ClassDB::bind_method(D_METHOD("get_travel_cost"), &NavigationPolygonInstance::get_travel_cost);

	ClassDB::bind_method(D_METHOD("_navpoly_changed"), &NavigationPolygonInstance::_navpoly_changed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "navpoly", PROPERTY_HINT_RESOURCE_TYPE, "NavigationPolygon"), "set_navigation_polygon", "get_navigation_polygon");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "is_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_2D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "enter_cost"), "set_enter_cost", "get_enter_cost");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "travel_cost"), "set_travel_cost", "get_travel_cost");

	ClassDB::bind_method(D_METHOD("_map_changed"), &NavigationPolygonInstance::_map_changed);
}

NavigationPolygonInstance::NavigationPolygonInstance() {
	enabled = true;
	set_notify_transform(true);
	region = Navigation2DServer::get_singleton()->region_create();

	navigation = nullptr;

	enter_cost = 0.0;
	travel_cost = 1.0;

	navigation_layers = 1;

	Navigation2DServer::get_singleton()->region_set_enter_cost(region, get_enter_cost());
	Navigation2DServer::get_singleton()->region_set_travel_cost(region, get_travel_cost());

#ifdef DEBUG_ENABLED
	Navigation2DServer::get_singleton_mut()->connect("map_changed", this, "_map_changed");
	NavigationServer::get_singleton_mut()->connect("navigation_debug_changed", this, "_map_changed");
#endif // DEBUG_ENABLED
}

NavigationPolygonInstance::~NavigationPolygonInstance() {
	Navigation2DServer::get_singleton()->free(region);

#ifdef DEBUG_ENABLED
	Navigation2DServer::get_singleton_mut()->disconnect("map_changed", this, "_map_changed");
	NavigationServer::get_singleton_mut()->disconnect("navigation_debug_changed", this, "_map_changed");
#endif // DEBUG_ENABLED
}
