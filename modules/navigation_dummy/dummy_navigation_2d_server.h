#ifndef DUMMY_NAVIGATION_2D_SERVER_H
#define DUMMY_NAVIGATION_2D_SERVER_H

#include "core/object/reference.h"

#include "servers/navigation_2d_server.h"

class NavigationPolygon;

class DummyNavigation2DServer : public Navigation2DServer {
public:
	virtual Array get_maps() const { return Array(); }

	virtual RID map_create() { return RID(); }
	virtual void map_set_active(RID p_map, bool p_active) {}
	virtual bool map_is_active(RID p_map) const { return false; }
	virtual void map_set_cell_size(RID p_map, real_t p_cell_size) {}
	virtual real_t map_get_cell_size(RID p_map) const { return 0; }
	virtual void map_set_cell_height(RID p_map, real_t p_cell_height) {}
	virtual real_t map_get_cell_height(RID p_map) const { return 0; }
	void map_set_use_edge_connections(RID p_map, bool p_enabled) {}
	bool map_get_use_edge_connections(RID p_map) const { return false; }
	virtual void map_set_edge_connection_margin(RID p_map, real_t p_connection_margin) {}
	virtual real_t map_get_edge_connection_margin(RID p_map) const { return 0; }

	virtual void map_set_link_connection_radius(RID p_map, real_t p_connection_radius) {}
	virtual real_t map_get_link_connection_radius(RID p_map) const { return 0; }

	virtual Vector<Vector2> map_get_path(RID p_map, Vector2 p_origin, Vector2 p_destination, bool p_optimize, uint32_t p_navigation_layers = 1) const { return Vector<Vector2>(); }
	virtual Vector2 map_get_closest_point(RID p_map, const Vector2 &p_point) const { return Vector2(); }
	virtual RID map_get_closest_point_owner(RID p_map, const Vector2 &p_point) const { return RID(); }

	virtual Array map_get_links(RID p_map) const { return Array(); }
	virtual Array map_get_regions(RID p_map) const { return Array(); }
	virtual Array map_get_agents(RID p_map) const { return Array(); }
	virtual Array map_get_obstacles(RID p_map) const { return Array(); }
	virtual void map_force_update(RID p_map) {}

	virtual RID region_create() { return RID(); }
	void region_set_use_edge_connections(RID p_region, bool p_enabled) {}
	bool region_get_use_edge_connections(RID p_region) const { return false; }
	virtual void region_set_enter_cost(RID p_region, real_t p_enter_cost) {}
	virtual real_t region_get_enter_cost(RID p_region) const { return 0; }
	virtual void region_set_travel_cost(RID p_region, real_t p_travel_cost) {}
	virtual real_t region_get_travel_cost(RID p_region) const { return 0; }
	virtual void region_set_owner_id(RID p_region, ObjectID p_owner_id) {}
	virtual ObjectID region_get_owner_id(RID p_region) const { return 0; }
	virtual bool region_owns_point(RID p_region, const Vector2 &p_point) const { return false; }
	virtual void region_set_map(RID p_region, RID p_map) {}
	virtual RID region_get_map(RID p_region) const { return RID(); }
	virtual void region_set_navigation_layers(RID p_region, uint32_t p_navigation_layers) {}
	virtual uint32_t region_get_navigation_layers(RID p_region) const { return 0; }
	virtual void region_set_transform(RID p_region, Transform2D p_transform) {}
	virtual void region_set_navigation_polygon(RID p_region, Ref<NavigationPolygon> p_navigation_mesh);
	virtual int region_get_connections_count(RID p_region) const { return 0; }
	virtual Vector2 region_get_connection_pathway_start(RID p_region, int p_connection_id) const { return Vector2(); }
	virtual Vector2 region_get_connection_pathway_end(RID p_region, int p_connection_id) const { return Vector2(); }

	virtual RID link_create() { return RID(); }
	virtual void link_set_map(RID p_link, RID p_map) {}
	virtual RID link_get_map(RID p_link) const { return RID(); }
	virtual void link_set_bidirectional(RID p_link, bool p_bidirectional) {}
	virtual bool link_is_bidirectional(RID p_link) const { return false; }
	virtual void link_set_navigation_layers(RID p_link, uint32_t p_navigation_layers) {}
	virtual uint32_t link_get_navigation_layers(RID p_link) const { return 0; }
	virtual void link_set_start_position(RID p_link, Vector2 p_position) {}
	virtual Vector2 link_get_start_position(RID p_link) const { return Vector2(); }
	virtual void link_set_end_position(RID p_link, Vector2 p_position) {}
	virtual Vector2 link_get_end_position(RID p_link) const { return Vector2(); }
	virtual void link_set_enter_cost(RID p_link, real_t p_enter_cost) {}
	virtual real_t link_get_enter_cost(RID p_link) const { return 0; }
	virtual void link_set_travel_cost(RID p_link, real_t p_travel_cost) {}
	virtual real_t link_get_travel_cost(RID p_link) const { return 0; }
	virtual void link_set_owner_id(RID p_link, ObjectID p_owner_id) {}
	virtual ObjectID link_get_owner_id(RID p_link) const { return 0; }

	virtual RID agent_create() { return RID(); }
	virtual void agent_set_map(RID p_agent, RID p_map) {}
	virtual RID agent_get_map(RID p_agent) const { return RID(); }
	virtual void agent_set_paused(RID p_agent, bool p_paused) {}
	virtual bool agent_get_paused(RID p_agent) const { return false; }
	virtual void agent_set_avoidance_enabled(RID p_agent, bool p_enabled) {}
	virtual bool agent_get_avoidance_enabled(RID p_agent) const { return false; }
	virtual void agent_set_neighbor_distance(RID p_agent, real_t p_dist) {}
	virtual void agent_set_max_neighbors(RID p_agent, int p_count) {}
	virtual void agent_set_time_horizon_agents(RID p_agent, real_t p_time_horizon) {}
	virtual void agent_set_time_horizon_obstacles(RID p_agent, real_t p_time_horizon) {}
	virtual void agent_set_radius(RID p_agent, real_t p_radius) {}
	virtual void agent_set_max_speed(RID p_agent, real_t p_max_speed) {}
	virtual void agent_set_velocity_forced(RID p_agent, Vector2 p_velocity) {}
	virtual void agent_set_velocity(RID p_agent, Vector2 p_velocity) {}
	virtual void agent_set_position(RID p_agent, Vector2 p_position) {}
	virtual bool agent_is_map_changed(RID p_agent) const { return false; }
	virtual void agent_set_avoidance_callback(RID p_agent, ObjectID p_object_id, StringName p_method, Variant p_udata = Variant()) {}

	virtual void agent_set_avoidance_layers(RID p_agent, uint32_t p_layers) {}
	virtual void agent_set_avoidance_mask(RID p_agent, uint32_t p_mask) {}
	virtual void agent_set_avoidance_priority(RID p_agent, real_t p_priority) {}

	/// Creates the obstacle.
	virtual RID obstacle_create() { return RID(); }
	virtual void obstacle_set_map(RID p_obstacle, RID p_map) {}
	virtual RID obstacle_get_map(RID p_obstacle) const { return RID(); }
	virtual void obstacle_set_avoidance_enabled(RID p_obstacle, bool p_enabled) {}
	virtual bool obstacle_get_avoidance_enabled(RID p_obstacle) const { return false; }
	virtual void obstacle_set_radius(RID p_obstacle, real_t p_radius) {}
	virtual void obstacle_set_paused(RID p_obstacle, bool p_paused) {}
	virtual bool obstacle_get_paused(RID p_obstacle) const { return false; }
	virtual void obstacle_set_velocity(RID p_obstacle, Vector2 p_velocity) {}
	virtual void obstacle_set_position(RID p_obstacle, Vector2 p_position) {}
	virtual void obstacle_set_vertices(RID p_obstacle, const Vector<Vector2> &p_vertices) {}
	virtual void obstacle_set_avoidance_layers(RID p_obstacle, uint32_t p_layers) {}

	virtual void free(RID p_object) {}

	virtual NavigationUtilities::PathQueryResult2D _query_path(const NavigationUtilities::PathQueryParameters2D &p_parameters) const;

	DummyNavigation2DServer();
	virtual ~DummyNavigation2DServer();
};

#endif
