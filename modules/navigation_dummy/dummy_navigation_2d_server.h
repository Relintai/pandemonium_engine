#ifndef DUMMY_NAVIGATION_2D_SERVER_H
#define DUMMY_NAVIGATION_2D_SERVER_H

#include "core/object/reference.h"

#include "servers/navigation_2d_server.h"

class NavigationPolygon;

class DummyNavigation2DServer : public Navigation2DServer {
public:
	virtual Array get_maps() const { return Array(); }

	virtual RID map_create() const { return RID(); }
	virtual void map_set_active(RID p_map, bool p_active) const {}
	virtual bool map_is_active(RID p_map) const { return false; }
	virtual void map_set_cell_size(RID p_map, real_t p_cell_size) const {}
	virtual real_t map_get_cell_size(RID p_map) const { return 0; }
	virtual void map_set_cell_height(RID p_map, real_t p_cell_height) const {}
	virtual real_t map_get_cell_height(RID p_map) const { return 0; }
	virtual void map_set_edge_connection_margin(RID p_map, real_t p_connection_margin) const {}
	virtual real_t map_get_edge_connection_margin(RID p_map) const { return 0; }
	virtual Vector<Vector2> map_get_path(RID p_map, Vector2 p_origin, Vector2 p_destination, bool p_optimize, uint32_t p_navigation_layers = 1) const { return Vector<Vector2>(); }
	virtual Vector2 map_get_closest_point(RID p_map, const Vector2 &p_point) const { return Vector2(); }
	virtual RID map_get_closest_point_owner(RID p_map, const Vector2 &p_point) const { return RID(); }
	virtual Array map_get_regions(RID p_map) const { return Array(); }
	virtual Array map_get_agents(RID p_map) const { return Array(); }
	virtual void map_force_update(RID p_map) {}

	virtual RID region_create() const { return RID(); }
	virtual void region_set_enter_cost(RID p_region, real_t p_enter_cost) const {}
	virtual real_t region_get_enter_cost(RID p_region) const { return 0; }
	virtual void region_set_travel_cost(RID p_region, real_t p_travel_cost) const {}
	virtual real_t region_get_travel_cost(RID p_region) const { return 0; }
	virtual bool region_owns_point(RID p_region, const Vector2 &p_point) const { return false; }
	virtual void region_set_map(RID p_region, RID p_map) const {}
	virtual RID region_get_map(RID p_region) const { return RID(); }
	virtual void region_set_navigation_layers(RID p_region, uint32_t p_navigation_layers) const {}
	virtual uint32_t region_get_navigation_layers(RID p_region) const { return 0; }
	virtual void region_set_transform(RID p_region, Transform2D p_transform) const {}
	virtual void region_set_navpoly(RID p_region, Ref<NavigationPolygon> p_nav_mesh) const;
	virtual int region_get_connections_count(RID p_region) const { return 0; }
	virtual Vector2 region_get_connection_pathway_start(RID p_region, int p_connection_id) const { return Vector2(); }
	virtual Vector2 region_get_connection_pathway_end(RID p_region, int p_connection_id) const { return Vector2(); }

	virtual RID agent_create() const { return RID(); }
	virtual void agent_set_map(RID p_agent, RID p_map) const {}
	virtual RID agent_get_map(RID p_agent) const { return RID(); }
	virtual void agent_set_neighbor_dist(RID p_agent, real_t p_dist) const {}
	virtual void agent_set_max_neighbors(RID p_agent, int p_count) const {}
	virtual void agent_set_time_horizon(RID p_agent, real_t p_time) const {}
	virtual void agent_set_radius(RID p_agent, real_t p_radius) const {}
	virtual void agent_set_max_speed(RID p_agent, real_t p_max_speed) const {}
	virtual void agent_set_velocity(RID p_agent, Vector2 p_velocity) const {}
	virtual void agent_set_target_velocity(RID p_agent, Vector2 p_velocity) const {}
	virtual void agent_set_position(RID p_agent, Vector2 p_position) const {}
	virtual void agent_set_ignore_y(RID p_agent, bool p_ignore) const {}
	virtual bool agent_is_map_changed(RID p_agent) const { return false; }
	virtual void agent_set_callback(RID p_agent, ObjectID p_object_id, StringName p_method, Variant p_udata = Variant()) const {}

	virtual void free(RID p_object) const {}

	virtual NavigationUtilities::PathQueryResult2D _query_path(const NavigationUtilities::PathQueryParameters2D &p_parameters) const;

	DummyNavigation2DServer();
	virtual ~DummyNavigation2DServer();
};

#endif
