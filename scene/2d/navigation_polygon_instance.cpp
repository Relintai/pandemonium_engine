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
#include "core/math/geometry.h"
#include "core/object/func_ref.h"
#include "core/os/mutex.h"
#include "core/os/os.h"
#include "navigation_2d.h"
#include "scene/2d/navigation_obstacle_2d.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/navigation_polygon.h"
#include "scene/resources/world_2d.h"
#include "servers/navigation/navigation_mesh_generator.h"
#include "servers/navigation_2d_server.h"
#include "servers/navigation_server.h"

#include "thirdparty/misc/triangulator.h"

void NavigationPolygonInstance::set_enabled(bool p_enabled) {
	if (enabled == p_enabled) {
		return;
	}
	enabled = p_enabled;

	Navigation2DServer::get_singleton()->region_set_enabled(region, enabled);

#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled()) {
		update();
	}
#endif // DEBUG_ENABLED
}

bool NavigationPolygonInstance::is_enabled() const {
	return enabled;
}

void NavigationPolygonInstance::set_use_edge_connections(bool p_enabled) {
	if (use_edge_connections == p_enabled) {
		return;
	}

	use_edge_connections = p_enabled;

	Navigation2DServer::get_singleton()->region_set_use_edge_connections(region, use_edge_connections);
}

bool NavigationPolygonInstance::get_use_edge_connections() const {
	return use_edge_connections;
}

void NavigationPolygonInstance::set_navigation_layers(uint32_t p_navigation_layers) {
	navigation_layers = p_navigation_layers;
	Navigation2DServer::get_singleton()->region_set_navigation_layers(region, navigation_layers);
}

uint32_t NavigationPolygonInstance::get_navigation_layers() const {
	return navigation_layers;
}

void NavigationPolygonInstance::set_navigation_layer_value(int p_layer_number, bool p_value) {
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

bool NavigationPolygonInstance::get_navigation_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");
	return get_navigation_layers() & (1 << (p_layer_number - 1));
}

void NavigationPolygonInstance::set_enter_cost(real_t p_enter_cost) {
	ERR_FAIL_COND_MSG(p_enter_cost < 0.0, "The enter_cost must be positive.");
	if (Math::is_equal_approx(enter_cost, p_enter_cost)) {
		return;
	}

	enter_cost = p_enter_cost;

	Navigation2DServer::get_singleton()->region_set_enter_cost(region, p_enter_cost);
}

real_t NavigationPolygonInstance::get_enter_cost() const {
	return enter_cost;
}

void NavigationPolygonInstance::set_travel_cost(real_t p_travel_cost) {
	ERR_FAIL_COND_MSG(p_travel_cost < 0.0, "The travel_cost must be positive.");
	if (Math::is_equal_approx(travel_cost, p_travel_cost)) {
		return;
	}

	travel_cost = p_travel_cost;

	Navigation2DServer::get_singleton()->region_set_travel_cost(region, travel_cost);
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
	return navigation_polygon.is_valid() ? navigation_polygon->_edit_get_rect() : Rect2();
}

bool NavigationPolygonInstance::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	return navigation_polygon.is_valid() ? navigation_polygon->_edit_is_selected_on_click(p_point, p_tolerance) : false;
}
#endif

void NavigationPolygonInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Node2D *c = this;
			while (c) {
				navigation = Object::cast_to<Navigation2D>(c);

				if (navigation) {
					break;
				}

				c = Object::cast_to<Node2D>(c->get_parent());
			}

			_region_enter_navigation_map();
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			set_physics_process_internal(true);
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			set_physics_process_internal(false);
			
			_region_update_transform();
		} break;
		case NOTIFICATION_EXIT_TREE: {
			_region_exit_navigation_map();

			navigation = nullptr;
		} break;
		case NOTIFICATION_DRAW: {
#ifdef DEBUG_ENABLED
			if (is_inside_tree() && (Engine::get_singleton()->is_editor_hint() || Navigation2DServer::get_singleton()->get_debug_navigation_enabled()) && navigation_polygon.is_valid()) {
				_update_debug_mesh();
				_update_debug_edge_connections_mesh();
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

void NavigationPolygonInstance::set_navigation_polygon(const Ref<NavigationPolygon> &p_navigation_polygon) {
	if (p_navigation_polygon == navigation_polygon) {
		return;
	}

	if (navigation_polygon.is_valid()) {
		navigation_polygon->disconnect(CoreStringNames::get_singleton()->changed, this, "_navigation_polygon_changed");
	}

	navigation_polygon = p_navigation_polygon;
	Navigation2DServer::get_singleton()->region_set_navigation_polygon(region, p_navigation_polygon);

	if (navigation_polygon.is_valid()) {
		navigation_polygon->connect(CoreStringNames::get_singleton()->changed, this, "_navigation_polygon_changed");
	}

	_navigation_polygon_changed();
}

Ref<NavigationPolygon> NavigationPolygonInstance::get_navigation_polygon() const {
	return navigation_polygon;
}

void NavigationPolygonInstance::set_navigation_map(RID p_navigation_map) {
	if (map_override == p_navigation_map) {
		return;
	}

	map_override = p_navigation_map;

	Navigation2DServer::get_singleton()->region_set_map(region, map_override);
}

RID NavigationPolygonInstance::get_navigation_map() const {
	if (map_override.is_valid()) {
		return map_override;
	} else if (navigation) {
		return navigation->get_rid();
	} else if (is_inside_tree()) {
		return get_world_2d()->get_navigation_map();
	}
	return RID();
}

void NavigationPolygonInstance::bake_navigation_polygon(bool p_on_thread) {
	ERR_FAIL_COND_MSG(!get_navigation_polygon().is_valid(), "Can't bake the navigation polygon if the `NavigationPolygon` resource doesn't exist");
	if (baking_started) {
		WARN_PRINT("NavigationPolygon baking already started. Wait for it to finish.");
		return;
	}
	baking_started = true;

	navigation_polygon->clear();
	if (p_on_thread && OS::get_singleton()->can_use_threads()) {
		Ref<FuncRef> f;
		f.instance();
		f->set_instance(this);
		f->set_function("_bake_finished");

		NavigationMeshGenerator::get_singleton()->parse_and_bake_2d(navigation_polygon, this, f);
	} else {
		Ref<NavigationMeshSourceGeometryData2D> source_geometry_data = NavigationMeshGenerator::get_singleton()->parse_2d_source_geometry_data(navigation_polygon, this);
		NavigationMeshGenerator::get_singleton()->bake_2d_from_source_geometry_data(navigation_polygon, source_geometry_data);
		_bake_finished();
	}
}

void NavigationPolygonInstance::_bake_finished() {
	baking_started = false;
	set_navigation_polygon(navigation_polygon);
	emit_signal("bake_finished");
}

void NavigationPolygonInstance::_navigation_polygon_changed() {
	if (is_inside_tree() && (Engine::get_singleton()->is_editor_hint() || get_tree()->is_debugging_navigation_hint())) {
		update();
	}
	if (navigation_polygon.is_valid()) {
		Navigation2DServer::get_singleton()->region_set_navigation_polygon(region, navigation_polygon);
	}

	_update_avoidance_constrain();

	emit_signal("navigation_polygon_changed");
	_change_notify("navigation_polygon");

	update_configuration_warning();
}

#ifdef DEBUG_ENABLED
void NavigationPolygonInstance::_navigation_map_changed(RID p_map) {
	if (navigation != nullptr && enabled && (navigation->get_rid() == p_map)) {
		update();
	} else if (is_inside_tree() && enabled && (get_world_2d()->get_navigation_map() == p_map)) {
		update();
	}
}
void NavigationPolygonInstance::_navigation_debug_changed() {
	if (navigation != nullptr && enabled) {
		update();
	} else if (is_inside_tree() && enabled) {
		update();
	}
}
#endif // DEBUG_ENABLED

String NavigationPolygonInstance::get_configuration_warning() const {
	if (!is_visible_in_tree() || !is_inside_tree()) {
		return String();
	}

	String warning = Node2D::get_configuration_warning();
	if (!navigation_polygon.is_valid()) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("A NavigationPolygon resource must be set or created for this node to work. Please set a property or draw a polygon.");
	}

	return warning;
}

void NavigationPolygonInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &NavigationPolygonInstance::set_enabled);
	ClassDB::bind_method(D_METHOD("is_enabled"), &NavigationPolygonInstance::is_enabled);

	ClassDB::bind_method(D_METHOD("set_use_edge_connections", "enabled"), &NavigationPolygonInstance::set_use_edge_connections);
	ClassDB::bind_method(D_METHOD("get_use_edge_connections"), &NavigationPolygonInstance::get_use_edge_connections);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationPolygonInstance::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationPolygonInstance::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("set_navigation_layer_value", "layer_number", "value"), &NavigationPolygonInstance::set_navigation_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_value", "layer_number"), &NavigationPolygonInstance::get_navigation_layer_value);

	ClassDB::bind_method(D_METHOD("set_constrain_avoidance", "enabled"), &NavigationPolygonInstance::set_constrain_avoidance);
	ClassDB::bind_method(D_METHOD("get_constrain_avoidance"), &NavigationPolygonInstance::get_constrain_avoidance);
	ClassDB::bind_method(D_METHOD("set_avoidance_layers", "layers"), &NavigationPolygonInstance::set_avoidance_layers);
	ClassDB::bind_method(D_METHOD("get_avoidance_layers"), &NavigationPolygonInstance::get_avoidance_layers);
	ClassDB::bind_method(D_METHOD("set_avoidance_layer_value", "layer_number", "value"), &NavigationPolygonInstance::set_avoidance_layer_value);
	ClassDB::bind_method(D_METHOD("get_avoidance_layer_value", "layer_number"), &NavigationPolygonInstance::get_avoidance_layer_value);

	ClassDB::bind_method(D_METHOD("set_enter_cost", "enter_cost"), &NavigationPolygonInstance::set_enter_cost);
	ClassDB::bind_method(D_METHOD("get_enter_cost"), &NavigationPolygonInstance::get_enter_cost);

	ClassDB::bind_method(D_METHOD("set_travel_cost", "travel_cost"), &NavigationPolygonInstance::set_travel_cost);
	ClassDB::bind_method(D_METHOD("get_travel_cost"), &NavigationPolygonInstance::get_travel_cost);

	ClassDB::bind_method(D_METHOD("set_navigation_polygon", "navigation_polygon"), &NavigationPolygonInstance::set_navigation_polygon);
	ClassDB::bind_method(D_METHOD("get_navigation_polygon"), &NavigationPolygonInstance::get_navigation_polygon);

	ClassDB::bind_method(D_METHOD("set_navigation_map", "navigation_map"), &NavigationPolygonInstance::set_navigation_map);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &NavigationPolygonInstance::get_navigation_map);

	ClassDB::bind_method(D_METHOD("bake_navigation_polygon", "on_thread"), &NavigationPolygonInstance::bake_navigation_polygon);
	ClassDB::bind_method(D_METHOD("_bake_finished"), &NavigationPolygonInstance::_bake_finished);

	ClassDB::bind_method(D_METHOD("get_region_rid"), &NavigationPolygonInstance::get_region_rid);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "navigation_polygon", PROPERTY_HINT_RESOURCE_TYPE, "NavigationPolygon"), "set_navigation_polygon", "get_navigation_polygon");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "is_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_edge_connections"), "set_use_edge_connections", "get_use_edge_connections");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_2D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "enter_cost"), "set_enter_cost", "get_enter_cost");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "travel_cost"), "set_travel_cost", "get_travel_cost");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "constrain_avoidance"), "set_constrain_avoidance", "get_constrain_avoidance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "avoidance_layers", PROPERTY_HINT_LAYERS_AVOIDANCE), "set_avoidance_layers", "get_avoidance_layers");

#ifdef DEBUG_ENABLED
	ClassDB::bind_method(D_METHOD("_navigation_debug_changed"), &NavigationPolygonInstance::_navigation_debug_changed);
	ClassDB::bind_method(D_METHOD("_navigation_map_changed"), &NavigationPolygonInstance::_navigation_map_changed);
#endif
	ClassDB::bind_method(D_METHOD("_navigation_polygon_changed"), &NavigationPolygonInstance::_navigation_polygon_changed);

	ADD_SIGNAL(MethodInfo("navigation_polygon_changed"));
	ADD_SIGNAL(MethodInfo("bake_finished"));
}

NavigationPolygonInstance::NavigationPolygonInstance() {
	enabled = true;
	use_edge_connections = true;
	set_notify_transform(true);
	region = Navigation2DServer::get_singleton()->region_create();

	navigation = nullptr;

	enter_cost = 0.0;
	travel_cost = 1.0;

	navigation_layers = 1;

	constrain_avoidance = false;
	avoidance_layers = 1;

	baking_started = false;
	Navigation2DServer::get_singleton()->region_set_owner_id(region, get_instance_id());
	Navigation2DServer::get_singleton()->region_set_enter_cost(region, get_enter_cost());
	Navigation2DServer::get_singleton()->region_set_travel_cost(region, get_travel_cost());

#ifdef DEBUG_ENABLED
	Navigation2DServer::get_singleton()->connect("map_changed", this, "_navigation_map_changed");
	NavigationServer::get_singleton()->connect("navigation_debug_changed", this, "_navigation_debug_changed");
#endif // DEBUG_ENABLED
}

NavigationPolygonInstance::~NavigationPolygonInstance() {
	ERR_FAIL_NULL(Navigation2DServer::get_singleton());

	Navigation2DServer::get_singleton()->free(region);

	for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
		if (constrain_avoidance_obstacles[i].is_valid()) {
			Navigation2DServer::get_singleton()->free(constrain_avoidance_obstacles[i]);
		}
	}
	constrain_avoidance_obstacles.clear();

#ifdef DEBUG_ENABLED
	Navigation2DServer::get_singleton()->disconnect("map_changed", this, "_navigation_map_changed");
	NavigationServer::get_singleton()->disconnect("navigation_debug_changed", this, "_navigation_debug_changed");
#endif // DEBUG_ENABLED
}

void NavigationPolygonInstance::_update_avoidance_constrain() {
	for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
		if (constrain_avoidance_obstacles[i].is_valid()) {
			Navigation2DServer::get_singleton()->free(constrain_avoidance_obstacles[i]);
			constrain_avoidance_obstacles[i] = RID();
		}
	}
	constrain_avoidance_obstacles.clear();

	if (!constrain_avoidance) {
		return;
	}

	if (get_navigation_polygon() == nullptr) {
		return;
	}

	Ref<NavigationPolygon> _navigation_polygon = get_navigation_polygon();
	int _outline_count = _navigation_polygon->get_outline_count();
	if (_outline_count == 0) {
		return;
	}

	for (int outline_index(0); outline_index < _outline_count; outline_index++) {
		//TODO navigation_polygon should probably use normal vectors internally
		const PoolVector<Vector2> &_outline = _navigation_polygon->get_outline(outline_index);

		Vector<Vector2> outline;
		outline.resize(_outline.size());
		for (int i = 0; i < _outline.size(); ++i) {
			outline.write[i] = outline[i];
		}

		const int outline_size = _outline.size();
		if (outline_size < 3) {
			ERR_FAIL_COND_MSG(_outline.size() < 3, "NavigationPolygon outline needs to have at least 3 vertex to create avoidance obstacles to constrain avoidance agent's");
			continue;
		}

		RID obstacle_rid = Navigation2DServer::get_singleton()->obstacle_create();
		constrain_avoidance_obstacles.push_back(obstacle_rid);

		Vector<Vector2> new_obstacle_outline;

		if (outline_index == 0) {
			for (int i(0); i < outline_size; i++) {
				new_obstacle_outline.push_back(_outline[outline_size - i - 1]);
			}
			ERR_FAIL_COND_MSG(Geometry::is_polygon_clockwise(outline), "Outermost outline needs to be clockwise to push avoidance agent inside");
		} else {
			for (int i(0); i < outline_size; i++) {
				new_obstacle_outline.push_back(_outline[i]);
			}
		}
		new_obstacle_outline.resize(outline_size);

		Navigation2DServer::get_singleton()->obstacle_set_vertices(obstacle_rid, new_obstacle_outline);
		Navigation2DServer::get_singleton()->obstacle_set_avoidance_layers(obstacle_rid, avoidance_layers);
		if (is_inside_tree()) {
			Navigation2DServer::get_singleton()->obstacle_set_map(obstacle_rid, get_world_2d()->get_navigation_map());
			Navigation2DServer::get_singleton()->obstacle_set_position(obstacle_rid, get_global_position());
		}
	}
	constrain_avoidance_obstacles.resize(_outline_count);
}

void NavigationPolygonInstance::set_constrain_avoidance(bool p_enabled) {
	constrain_avoidance = p_enabled;
	_update_avoidance_constrain();
	property_list_changed_notify();
}

bool NavigationPolygonInstance::get_constrain_avoidance() const {
	return constrain_avoidance;
}

void NavigationPolygonInstance::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "avoidance_layers") {
		if (!constrain_avoidance) {
			p_property.usage = PROPERTY_USAGE_NOEDITOR;
		}
	}
}

void NavigationPolygonInstance::set_avoidance_layers(uint32_t p_layers) {
	avoidance_layers = p_layers;
	if (constrain_avoidance_obstacles.size() > 0) {
		for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
			Navigation2DServer::get_singleton()->obstacle_set_avoidance_layers(constrain_avoidance_obstacles[i], avoidance_layers);
		}
	}
}

uint32_t NavigationPolygonInstance::get_avoidance_layers() const {
	return avoidance_layers;
}

void NavigationPolygonInstance::set_avoidance_layer_value(int p_layer_number, bool p_value) {
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

bool NavigationPolygonInstance::get_avoidance_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Avoidance layer number must be between 1 and 32 inclusive.");
	return get_avoidance_layers() & (1 << (p_layer_number - 1));
}

void NavigationPolygonInstance::_region_enter_navigation_map() {
	if (!is_inside_tree()) {
		return;
	}

	if (enabled) {
		RID map = get_navigation_map();

		Navigation2DServer::get_singleton()->region_set_map(region, map);
		for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
			if (constrain_avoidance_obstacles[i].is_valid()) {
				Navigation2DServer::get_singleton()->obstacle_set_map(constrain_avoidance_obstacles[i], map);
			}
		}
	}

	current_global_transform = get_global_transform();
	Navigation2DServer::get_singleton()->region_set_transform(region, current_global_transform);
	for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
		if (constrain_avoidance_obstacles[i].is_valid()) {
			Navigation2DServer::get_singleton()->obstacle_set_position(constrain_avoidance_obstacles[i], get_global_position());
		}
	}

	update();
}

void NavigationPolygonInstance::_region_exit_navigation_map() {
	Navigation2DServer::get_singleton()->region_set_map(region, RID());
	for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
		if (constrain_avoidance_obstacles[i].is_valid()) {
			Navigation2DServer::get_singleton()->obstacle_set_map(constrain_avoidance_obstacles[i], RID());
		}
	}
}

void NavigationPolygonInstance::_region_update_transform() {
	if (!is_inside_tree()) {
		return;
	}

	Transform2D new_global_transform = get_global_transform();
	if (current_global_transform != new_global_transform) {
		current_global_transform = new_global_transform;
		Navigation2DServer::get_singleton()->region_set_transform(region, current_global_transform);
		for (uint32_t i = 0; i < constrain_avoidance_obstacles.size(); i++) {
			if (constrain_avoidance_obstacles[i].is_valid()) {
				Navigation2DServer::get_singleton()->obstacle_set_position(constrain_avoidance_obstacles[i], get_global_position());
			}
		}
	}

	update();
}

#ifdef DEBUG_ENABLED
void NavigationPolygonInstance::_update_debug_mesh() {
	PoolVector<Vector2> navigation_polygon_vertices = navigation_polygon->get_vertices();
	if (navigation_polygon_vertices.size() < 3) {
		return;
	}

	const Navigation2DServer *ns2d = Navigation2DServer::get_singleton();

	bool enabled_geometry_face_random_color = ns2d->get_debug_navigation_enable_geometry_face_random_color();
	bool enabled_edge_lines = ns2d->get_debug_navigation_enable_edge_lines();
	//bool enable_edge_connections = use_edge_connections && ns2d->get_debug_navigation_enable_edge_connections() && ns2d->map_get_use_edge_connections(get_world_2d()->get_navigation_map());

	Color debug_face_color = ns2d->get_debug_navigation_geometry_face_color();
	Color debug_edge_color = ns2d->get_debug_navigation_geometry_edge_color();
	//Color debug_edge_connection_color = ns2d->get_debug_navigation_edge_connection_color();

	if (!enabled) {
		debug_face_color = ns2d->get_debug_navigation_geometry_face_disabled_color();
		debug_edge_color = ns2d->get_debug_navigation_geometry_edge_disabled_color();
	}

	RandomPCG rand;

	for (int i = 0; i < navigation_polygon->get_polygon_count(); i++) {
		// An array of vertices for this polygon.
		Vector<int> polygon = navigation_polygon->get_polygon(i);

		Vector<Vector2> debug_polygon_vertices;
		debug_polygon_vertices.resize(polygon.size());
		for (int j = 0; j < polygon.size(); j++) {
			ERR_FAIL_INDEX(polygon[j], navigation_polygon_vertices.size());
			debug_polygon_vertices.write[j] = navigation_polygon_vertices[polygon[j]];
		}
		// Generate the polygon color, slightly randomly modified from the settings one.
		Color random_variation_color = debug_face_color;
		if (enabled_geometry_face_random_color) {
			random_variation_color.set_hsv(debug_face_color.get_h() + rand.random(-1.0, 1.0) * 0.1, debug_face_color.get_s(), debug_face_color.get_v() + rand.random(-1.0, 1.0) * 0.2);
		}
		random_variation_color.a = debug_face_color.a;

		Vector<Color> debug_face_colors;
		debug_face_colors.push_back(random_variation_color);
		RS::get_singleton()->canvas_item_add_polygon(get_canvas_item(), debug_polygon_vertices, debug_face_colors);

		if (enabled_edge_lines) {
			Vector<Color> debug_edge_colors;
			debug_edge_colors.push_back(debug_edge_color);
			debug_polygon_vertices.push_back(debug_polygon_vertices[0]); // Add first again for closing polyline.
			RS::get_singleton()->canvas_item_add_polyline(get_canvas_item(), debug_polygon_vertices, debug_edge_colors);
		}
	}
}
#endif // DEBUG_ENABLED

#ifdef DEBUG_ENABLED
void NavigationPolygonInstance::_update_debug_edge_connections_mesh() {
	const Navigation2DServer *ns2d = Navigation2DServer::get_singleton();
	bool enable_edge_connections = ns2d->get_debug_navigation_enable_edge_connections();

	if (enable_edge_connections) {
		Color debug_edge_connection_color = ns2d->get_debug_navigation_edge_connection_color();

		// Draw the region edge connections.
		Transform2D xform = get_global_transform();

		RID map = get_navigation_map();

		real_t radius = 1.0;
		radius = ns2d->map_get_edge_connection_margin(map) / 2.0;

		for (int i = 0; i < ns2d->region_get_connections_count(region); i++) {
			// Two main points
			Vector2 a = ns2d->region_get_connection_pathway_start(region, i);
			a = xform.affine_inverse().xform(a);
			Vector2 b = ns2d->region_get_connection_pathway_end(region, i);
			b = xform.affine_inverse().xform(b);
			draw_line(a, b, debug_edge_connection_color);

			// Draw a circle to illustrate the margins.
			real_t angle = a.angle_to_point(b);
			draw_arc(a, radius, angle + Math_PI / 2.0, angle - Math_PI / 2.0 + Math_TAU, 10, debug_edge_connection_color);
			draw_arc(b, radius, angle - Math_PI / 2.0, angle + Math_PI / 2.0, 10, debug_edge_connection_color);
		}
	}
}
#endif // DEBUG_ENABLED
