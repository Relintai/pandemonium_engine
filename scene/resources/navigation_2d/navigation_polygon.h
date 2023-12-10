#ifndef NAVIGATION_POLYGON_H
#define NAVIGATION_POLYGON_H
/*************************************************************************/
/*  navigation_polygon.h                                                 */
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

#include "core/object/resource.h"
#include "scene/2d/node_2d.h"

class NavigationMesh;

class NavigationPolygon : public Resource {
	GDCLASS(NavigationPolygon, Resource);

public:
	enum ParsedGeometryType {
		PARSED_GEOMETRY_MESH_INSTANCES = 0,
		PARSED_GEOMETRY_STATIC_COLLIDERS,
		PARSED_GEOMETRY_BOTH,
		PARSED_GEOMETRY_MAX
	};

	enum SourceGeometryMode {
		SOURCE_GEOMETRY_ROOT_NODE_CHILDREN = 0,
		SOURCE_GEOMETRY_GROUPS_WITH_CHILDREN,
		SOURCE_GEOMETRY_GROUPS_EXPLICIT,
		SOURCE_GEOMETRY_MAX
	};

	enum PolygonFillRule {
		POLYGON_FILLRULE_EVENODD = 0,
		POLYGON_FILLRULE_NONZERO,
		POLYGON_FILLRULE_POSITIVE,
		POLYGON_FILLRULE_NEGATIVE,
		POLYGON_FILLRULE_MAX
	};

	enum OffsettingJoinType {
		OFFSETTING_JOINTYPE_SQUARE = 0,
		OFFSETTING_JOINTYPE_ROUND,
		OFFSETTING_JOINTYPE_MITER,
		OFFSETTING_JOINTYPE_MAX
	};

public:
#ifdef TOOLS_ENABLED
	Rect2 _edit_get_rect() const;
	bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;
#endif

	void set_parsed_geometry_type(ParsedGeometryType p_value);
	ParsedGeometryType get_parsed_geometry_type() const;

	void set_collision_mask(uint32_t p_mask);
	uint32_t get_collision_mask() const;

	void set_collision_mask_value(int p_layer_number, bool p_value);
	bool get_collision_mask_value(int p_layer_number) const;

	void set_source_geometry_mode(SourceGeometryMode p_geometry_mode);
	SourceGeometryMode get_source_geometry_mode() const;

	void set_source_group_name(StringName p_group_name);
	StringName get_source_group_name() const;

	void set_polygon_bake_fillrule(PolygonFillRule p_polygon_fillrule);
	PolygonFillRule get_polygon_bake_fillrule() const;

	void set_offsetting_jointype(OffsettingJoinType p_offsetting_jointype);
	OffsettingJoinType get_offsetting_jointype() const;

	void set_agent_radius(real_t p_value);
	real_t get_agent_radius() const;

	void set_cell_size(real_t p_cell_size);
	real_t get_cell_size() const;

	void set_vertices(const PoolVector<Vector2> &p_vertices);
	PoolVector<Vector2> get_vertices() const;

	void add_polygon(const Vector<int> &p_polygon);
	Vector<int> get_polygon(int p_idx);
	int get_polygon_count() const;

	void clear_polygons();
	void clear_outlines();
	void clear_baked_outlines();
	void clear();
	void commit_changes();

	void add_outline(const PoolVector<Vector2> &p_outline);
	void add_outline_at_index(const PoolVector<Vector2> &p_outline, int p_index);
	void set_outline(int p_idx, const PoolVector<Vector2> &p_outline);
	PoolVector<Vector2> get_outline(int p_idx) const;
	void remove_outline(int p_idx);
	int get_outline_count() const;

	void make_polygons_from_outlines();

	RID get_rid() const;

	Ref<NavigationMesh> get_navigation_mesh();

	void set_polygons(const Vector<Vector<int>> &p_array);
	Vector<Vector<int>> get_polygons() const;

	void set_outlines(const Vector<PoolVector<Vector2>> &p_array);
	Vector<PoolVector<Vector2>> get_outlines() const;

	void set_baked_outlines(const Vector<PoolVector<Vector2>> &p_array);
	Vector<PoolVector<Vector2>> get_baked_outlines() const;

	void _set_polygons(const Array &p_array);
	Array _get_polygons() const;

	void _set_outlines(const Array &p_array);
	Array _get_outlines() const;

	void _set_baked_outlines(const Array &p_array);
	Array _get_baked_outlines() const;

	NavigationPolygon();
	~NavigationPolygon();

protected:
	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

private:
	RID navigation_mesh_rid;

	real_t agent_radius;

	ParsedGeometryType parsed_geometry_type;
	uint32_t collision_mask;

	SourceGeometryMode source_geometry_mode;
	StringName source_group_name;

	PolygonFillRule polygon_bake_fillrule;
	OffsettingJoinType offsetting_jointype;

	bool navigation_polygon_dirty;

	real_t cell_size;
	PoolVector<Vector2> vertices;
	Vector<Vector<int>> polygons;
	Vector<PoolVector<Vector2>> outlines;
	Vector<PoolVector<Vector2>> baked_outlines;

	mutable Rect2 item_rect;
	mutable bool rect_cache_dirty;

	Ref<NavigationMesh> navigation_mesh;
};

VARIANT_ENUM_CAST(NavigationPolygon::ParsedGeometryType);
VARIANT_ENUM_CAST(NavigationPolygon::SourceGeometryMode);
VARIANT_ENUM_CAST(NavigationPolygon::PolygonFillRule);
VARIANT_ENUM_CAST(NavigationPolygon::OffsettingJoinType);

#endif // NAVIGATIONPOLYGON_H
