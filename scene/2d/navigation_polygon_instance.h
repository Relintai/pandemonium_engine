#ifndef NAVIGATION_REGION_2D_H
#define NAVIGATION_REGION_2D_H

/*************************************************************************/
/*  navigation_polygon_instance.h                                        */
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

#include "core/object/resource.h"
#include "scene/main/node_2d.h"

class NavigationMesh;
class Navigation2D;
class NavigationPolygon;

class NavigationPolygonInstance : public Node2D {
	GDCLASS(NavigationPolygonInstance, Node2D);

	bool enabled;
	bool use_edge_connections;

	RID region;
	RID map_override;
	Navigation2D *navigation;
	Ref<NavigationPolygon> navigation_polygon;
	bool baking_started;

	real_t enter_cost;
	real_t travel_cost;

	uint32_t navigation_layers;

	Transform2D current_global_transform;

	bool constrain_avoidance;
	LocalVector<RID> constrain_avoidance_obstacles;
	uint32_t avoidance_layers;

	void _navigation_polygon_changed();

#ifdef DEBUG_ENABLED
	void _update_debug_mesh();
	void _update_debug_edge_connections_mesh();
	void _navigation_map_changed(RID p_map);
	void _navigation_debug_changed();
#endif // DEBUG_ENABLED

protected:
	void _notification(int p_what);
	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
#endif // DISABLE_DEPRECATED

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const;
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;
#endif

	void set_enabled(bool p_enabled);
	bool is_enabled() const;

	void set_use_edge_connections(bool p_enabled);
	bool get_use_edge_connections() const;

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const;

	void set_navigation_layer_value(int p_layer_number, bool p_value);
	bool get_navigation_layer_value(int p_layer_number) const;

	RID get_region_rid() const;

	void set_enter_cost(real_t p_enter_cost);
	real_t get_enter_cost() const;

	void set_travel_cost(real_t p_travel_cost);
	real_t get_travel_cost() const;

	void set_navigation_polygon(const Ref<NavigationPolygon> &p_navigation_polygon);
	Ref<NavigationPolygon> get_navigation_polygon() const;

	void set_constrain_avoidance(bool p_enabled);
	bool get_constrain_avoidance() const;

	void set_avoidance_layers(uint32_t p_layers);
	uint32_t get_avoidance_layers() const;

	void set_avoidance_layer_value(int p_layer_number, bool p_value);
	bool get_avoidance_layer_value(int p_layer_number) const;

	void set_navigation_map(RID p_navigation_map);
	RID get_navigation_map() const;

	void bake_navigation_polygon(bool p_on_thread);
	void _bake_finished();

	String get_configuration_warning() const;

	NavigationPolygonInstance();
	~NavigationPolygonInstance();

private:
	void _update_avoidance_constrain();
	void _region_enter_navigation_map();
	void _region_exit_navigation_map();
	void _region_update_transform();
};

#endif // NAVIGATIONPOLYGON_H
