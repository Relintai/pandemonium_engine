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

#include "navigation_polygon.h"

#include "core/config/engine.h"
#include "core/core_string_names.h"
#include "core/os/mutex.h"
#include "scene/2d/navigation_2d.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/world_2d.h"
#include "servers/navigation_2d_server.h"

#include "thirdparty/misc/triangulator.h"

#ifdef TOOLS_ENABLED
Rect2 NavigationPolygon::_edit_get_rect() const {
	if (rect_cache_dirty) {
		item_rect = Rect2();
		bool first = true;

		for (int i = 0; i < outlines.size(); i++) {
			const PoolVector<Vector2> &outline = outlines[i];
			const int outline_size = outline.size();
			if (outline_size < 3) {
				continue;
			}
			PoolVector<Vector2>::Read p = outline.read();
			for (int j = 0; j < outline_size; j++) {
				if (first) {
					item_rect = Rect2(p[j], Vector2(0, 0));
					first = false;
				} else {
					item_rect.expand_to(p[j]);
				}
			}
		}

		rect_cache_dirty = false;
	}
	return item_rect;
}

bool NavigationPolygon::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	for (int i = 0; i < outlines.size(); i++) {
		const PoolVector<Vector2> &outline = outlines[i];
		const int outline_size = outline.size();
		if (outline_size < 3) {
			continue;
		}
		if (Geometry::is_point_in_polygon(p_point, Variant(outline))) {
			return true;
		}
	}
	return false;
}
#endif

void NavigationPolygon::set_parsed_geometry_type(ParsedGeometryType p_value) {
	ERR_FAIL_INDEX(p_value, PARSED_GEOMETRY_MAX);
	parsed_geometry_type = p_value;
	property_list_changed_notify();
}

NavigationPolygon::ParsedGeometryType NavigationPolygon::get_parsed_geometry_type() const {
	return parsed_geometry_type;
}

void NavigationPolygon::set_collision_mask(uint32_t p_mask) {
	collision_mask = p_mask;
}

uint32_t NavigationPolygon::get_collision_mask() const {
	return collision_mask;
}

void NavigationPolygon::set_collision_mask_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Collision layer number must be between 1 and 32 inclusive.");
	uint32_t mask = get_collision_mask();
	if (p_value) {
		mask |= 1 << (p_layer_number - 1);
	} else {
		mask &= ~(1 << (p_layer_number - 1));
	}
	set_collision_mask(mask);
}

bool NavigationPolygon::get_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_collision_mask() & (1 << (p_layer_number - 1));
}

void NavigationPolygon::set_source_geometry_mode(SourceGeometryMode p_geometry_mode) {
	ERR_FAIL_INDEX(p_geometry_mode, SOURCE_GEOMETRY_MAX);
	source_geometry_mode = p_geometry_mode;
	property_list_changed_notify();
}

NavigationPolygon::SourceGeometryMode NavigationPolygon::get_source_geometry_mode() const {
	return source_geometry_mode;
}

void NavigationPolygon::set_polygon_bake_fillrule(PolygonFillRule p_polygon_fillrule) {
	ERR_FAIL_INDEX(p_polygon_fillrule, POLYGON_FILLRULE_MAX);
	polygon_bake_fillrule = p_polygon_fillrule;
	property_list_changed_notify();
}

NavigationPolygon::PolygonFillRule NavigationPolygon::get_polygon_bake_fillrule() const {
	return polygon_bake_fillrule;
}

void NavigationPolygon::set_offsetting_jointype(OffsettingJoinType p_offsetting_jointype) {
	ERR_FAIL_INDEX(p_offsetting_jointype, OFFSETTING_JOINTYPE_MAX);
	offsetting_jointype = p_offsetting_jointype;
	property_list_changed_notify();
}

NavigationPolygon::OffsettingJoinType NavigationPolygon::get_offsetting_jointype() const {
	return offsetting_jointype;
}

void NavigationPolygon::set_source_group_name(StringName p_group_name) {
	source_group_name = p_group_name;
}

StringName NavigationPolygon::get_source_group_name() const {
	return source_group_name;
}

void NavigationPolygon::set_agent_radius(real_t p_value) {
	ERR_FAIL_COND(p_value < 0);
	agent_radius = p_value;
}

real_t NavigationPolygon::get_agent_radius() const {
	return agent_radius;
}

void NavigationPolygon::set_vertices(const PoolVector<Vector2> &p_vertices) {
	vertices = p_vertices;
	rect_cache_dirty = true;
	navigation_polygon_dirty = true;
}

PoolVector<Vector2> NavigationPolygon::get_vertices() const {
	return vertices;
}

void NavigationPolygon::add_polygon(const Vector<int> &p_polygon) {
	polygons.push_back(p_polygon);
	navigation_mesh.unref();
}

Vector<int> NavigationPolygon::get_polygon(int p_idx) {
	ERR_FAIL_INDEX_V(p_idx, polygons.size(), Vector<int>());
	return polygons[p_idx];
}

int NavigationPolygon::get_polygon_count() const {
	return polygons.size();
}

void NavigationPolygon::clear_polygons() {
	polygons.clear();
	navigation_mesh.unref();
}
void NavigationPolygon::clear_outlines() {
	outlines.clear();
	rect_cache_dirty = true;
}
void NavigationPolygon::clear_baked_outlines() {
	baked_outlines.clear();
	rect_cache_dirty = true;
}

void NavigationPolygon::clear() {
	clear_outlines();
	clear_polygons();
	clear_baked_outlines();
	vertices.clear();
	navigation_polygon_dirty = true;
}
void NavigationPolygon::commit_changes() {
	if (navigation_polygon_dirty) {
		navigation_polygon_dirty = false;

		PoolVector<Vector3> new_navigation_mesh_vertices;
		Vector<Vector<int>> new_navigation_mesh_polygons;

		new_navigation_mesh_vertices.resize(vertices.size());

		PoolVector3Array::Write new_navigation_mesh_vertices_write = new_navigation_mesh_vertices.write();
		Vector3 *new_navigation_mesh_vertices_ptrw = new_navigation_mesh_vertices_write.ptr();

		PoolVector2Array::Read vertices_read = vertices.read();
		const Vector2 *vertices_ptr = vertices_read.ptr();

		for (int i = 0; i < vertices.size(); i++) {
			new_navigation_mesh_vertices_ptrw[i] = Vector3(vertices_ptr[i].x, 0.0, vertices_ptr[i].y);
		}

		for (int i = 0; i < get_polygon_count(); i++) {
			Vector<int> new_navigation_mesh_polygon = get_polygon(i);
			new_navigation_mesh_polygons.push_back(new_navigation_mesh_polygon);
			//OLD, remove:
			navigation_mesh->add_polygon(get_polygon(i));
		}

		navigation_mesh->set_vertices(new_navigation_mesh_vertices);
		// New, enable:
		//navigation_mesh->set_polygons(new_navigation_mesh_polygons);
		//navigation_mesh->commit_changes();

		emit_changed();
	}
}

Ref<NavigationMesh> NavigationPolygon::get_navigation_mesh() {
	if (navigation_polygon_dirty) {
		commit_changes();
	}
	return navigation_mesh;
}

void NavigationPolygon::add_outline(const PoolVector<Vector2> &p_outline) {
	outlines.push_back(p_outline);
	rect_cache_dirty = true;
}

void NavigationPolygon::add_outline_at_index(const PoolVector<Vector2> &p_outline, int p_index) {
	outlines.insert(p_index, p_outline);
	rect_cache_dirty = true;
}

int NavigationPolygon::get_outline_count() const {
	return outlines.size();
}

void NavigationPolygon::set_outline(int p_idx, const PoolVector<Vector2> &p_outline) {
	ERR_FAIL_INDEX(p_idx, outlines.size());
	outlines.write[p_idx] = p_outline;
	rect_cache_dirty = true;
}

void NavigationPolygon::remove_outline(int p_idx) {
	ERR_FAIL_INDEX(p_idx, outlines.size());
	outlines.remove(p_idx);
	rect_cache_dirty = true;
}

PoolVector<Vector2> NavigationPolygon::get_outline(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, outlines.size(), PoolVector<Vector2>());
	return outlines[p_idx];
}

void NavigationPolygon::make_polygons_from_outlines() {
	navigation_mesh.unref();

	List<TriangulatorPoly> in_poly, out_poly;

	Vector2 outside_point(-1e10, -1e10);

	for (int i = 0; i < outlines.size(); i++) {
		PoolVector<Vector2> ol = outlines[i];
		int olsize = ol.size();
		if (olsize < 3) {
			continue;
		}
		PoolVector<Vector2>::Read r = ol.read();
		for (int j = 0; j < olsize; j++) {
			outside_point.x = MAX(r[j].x, outside_point.x);
			outside_point.y = MAX(r[j].y, outside_point.y);
		}
	}

	outside_point += Vector2(0.7239784, 0.819238); //avoid precision issues

	for (int i = 0; i < outlines.size(); i++) {
		PoolVector<Vector2> ol = outlines[i];
		int olsize = ol.size();
		if (olsize < 3) {
			continue;
		}
		PoolVector<Vector2>::Read r = ol.read();

		int interscount = 0;
		//test if this is an outer outline
		for (int k = 0; k < outlines.size(); k++) {
			if (i == k) {
				continue; //no self intersect
			}

			PoolVector<Vector2> ol2 = outlines[k];
			int olsize2 = ol2.size();
			if (olsize2 < 3) {
				continue;
			}
			PoolVector<Vector2>::Read r2 = ol2.read();

			for (int l = 0; l < olsize2; l++) {
				if (Geometry::segment_intersects_segment_2d(r[0], outside_point, r2[l], r2[(l + 1) % olsize2], nullptr)) {
					interscount++;
				}
			}
		}

		bool outer = (interscount % 2) == 0;

		TriangulatorPoly tp;
		tp.Init(olsize);
		for (int j = 0; j < olsize; j++) {
			tp[j] = r[j];
		}

		if (outer) {
			tp.SetOrientation(TRIANGULATOR_CCW);
		} else {
			tp.SetOrientation(TRIANGULATOR_CW);
			tp.SetHole(true);
		}

		in_poly.push_back(tp);
	}

	TriangulatorPartition tpart;
	if (tpart.ConvexPartition_HM(&in_poly, &out_poly) == 0) { //failed!
		ERR_PRINT("NavigationPolygon: Convex partition failed! Failed to convert outlines to a valid NavigationMesh."
				  "\nNavigationPolygon outlines can not overlap vertices or edges inside same outline or with other outlines or have any intersections."
				  "\nAdd the outmost and largest outline first. To add holes inside this outline add the smaller outlines with opposite winding order.");
		return;
	}

	polygons.clear();
	vertices.resize(0);

	RBMap<Vector2, int> points;
	for (List<TriangulatorPoly>::Element *I = out_poly.front(); I; I = I->next()) {
		TriangulatorPoly &tp = I->get();

		Vector<int> p;

		for (int64_t i = 0; i < tp.GetNumPoints(); i++) {
			RBMap<Vector2, int>::Element *E = points.find(tp[i]);
			if (!E) {
				E = points.insert(tp[i], vertices.size());
				vertices.push_back(tp[i]);
			}
			p.push_back(E->get());
		}

		polygons.push_back(p);
	}

	emit_signal(CoreStringNames::get_singleton()->changed);
}

RID NavigationPolygon::get_rid() const {
	if (navigation_mesh.is_valid()) {
		navigation_mesh->get_rid();
	}
	return RID();
}

NavigationPolygon::NavigationPolygon() {
	agent_radius = 10.0f;

	parsed_geometry_type = PARSED_GEOMETRY_MESH_INSTANCES;
	collision_mask = 0xFFFFFFFF;

	source_geometry_mode = SOURCE_GEOMETRY_ROOT_NODE_CHILDREN;
	source_group_name = "navigation_polygon_source_group";

	polygon_bake_fillrule = POLYGON_FILLRULE_EVENODD;
	offsetting_jointype = OFFSETTING_JOINTYPE_MITER;

	navigation_polygon_dirty = true;

	rect_cache_dirty = true;
}

NavigationPolygon::~NavigationPolygon() {
}

void NavigationPolygon::_set_polygons(const Array &p_array) {
	navigation_mesh.unref();

	polygons.resize(p_array.size());
	for (int i = 0; i < p_array.size(); i++) {
		polygons.write[i] = p_array[i];
	}
}

Array NavigationPolygon::_get_polygons() const {
	Array ret;
	ret.resize(polygons.size());
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = polygons[i];
	}

	return ret;
}

void NavigationPolygon::_set_outlines(const Array &p_array) {
	outlines.resize(p_array.size());
	for (int i = 0; i < p_array.size(); i++) {
		outlines.write[i] = p_array[i];
	}
	rect_cache_dirty = true;
}

Array NavigationPolygon::_get_outlines() const {
	Array ret;
	ret.resize(outlines.size());
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = outlines[i];
	}

	return ret;
}

void NavigationPolygon::_set_baked_outlines(const Array &p_array) {
	baked_outlines.resize(p_array.size());
	for (int i = 0; i < p_array.size(); i++) {
		baked_outlines.write[i] = p_array[i];
	}
	rect_cache_dirty = true;
}

Array NavigationPolygon::_get_baked_outlines() const {
	Array ret;
	ret.resize(baked_outlines.size());
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = baked_outlines[i];
	}

	return ret;
}

void NavigationPolygon::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "geometry_collision_mask") {
		if (parsed_geometry_type == PARSED_GEOMETRY_MESH_INSTANCES) {
			p_property.usage = 0;
			return;
		}
	}

	if (p_property.name == "geometry_source_group_name") {
		if (source_geometry_mode == SOURCE_GEOMETRY_ROOT_NODE_CHILDREN) {
			p_property.usage = 0;
			return;
		}
	}
}

void NavigationPolygon::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_parsed_geometry_type", "geometry_type"), &NavigationPolygon::set_parsed_geometry_type);
	ClassDB::bind_method(D_METHOD("get_parsed_geometry_type"), &NavigationPolygon::get_parsed_geometry_type);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &NavigationPolygon::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &NavigationPolygon::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_collision_mask_value", "layer_number", "value"), &NavigationPolygon::set_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_collision_mask_value", "layer_number"), &NavigationPolygon::get_collision_mask_value);

	ClassDB::bind_method(D_METHOD("set_source_geometry_mode", "mask"), &NavigationPolygon::set_source_geometry_mode);
	ClassDB::bind_method(D_METHOD("get_source_geometry_mode"), &NavigationPolygon::get_source_geometry_mode);

	ClassDB::bind_method(D_METHOD("set_source_group_name", "mask"), &NavigationPolygon::set_source_group_name);
	ClassDB::bind_method(D_METHOD("get_source_group_name"), &NavigationPolygon::get_source_group_name);

	ClassDB::bind_method(D_METHOD("set_polygon_bake_fillrule", "polygon_fillrule"), &NavigationPolygon::set_polygon_bake_fillrule);
	ClassDB::bind_method(D_METHOD("get_polygon_bake_fillrule"), &NavigationPolygon::get_polygon_bake_fillrule);

	ClassDB::bind_method(D_METHOD("set_offsetting_jointype", "offsetting_jointype"), &NavigationPolygon::set_offsetting_jointype);
	ClassDB::bind_method(D_METHOD("get_offsetting_jointype"), &NavigationPolygon::get_offsetting_jointype);

	ClassDB::bind_method(D_METHOD("set_agent_radius", "agent_radius"), &NavigationPolygon::set_agent_radius);
	ClassDB::bind_method(D_METHOD("get_agent_radius"), &NavigationPolygon::get_agent_radius);

	ClassDB::bind_method(D_METHOD("set_vertices", "vertices"), &NavigationPolygon::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertices"), &NavigationPolygon::get_vertices);

	ClassDB::bind_method(D_METHOD("add_polygon", "polygon"), &NavigationPolygon::add_polygon);
	ClassDB::bind_method(D_METHOD("get_polygon_count"), &NavigationPolygon::get_polygon_count);
	ClassDB::bind_method(D_METHOD("get_polygon", "idx"), &NavigationPolygon::get_polygon);
	ClassDB::bind_method(D_METHOD("clear_polygons"), &NavigationPolygon::clear_polygons);

	ClassDB::bind_method(D_METHOD("add_outline", "outline"), &NavigationPolygon::add_outline);
	ClassDB::bind_method(D_METHOD("add_outline_at_index", "outline", "index"), &NavigationPolygon::add_outline_at_index);
	ClassDB::bind_method(D_METHOD("get_outline_count"), &NavigationPolygon::get_outline_count);
	ClassDB::bind_method(D_METHOD("set_outline", "idx", "outline"), &NavigationPolygon::set_outline);
	ClassDB::bind_method(D_METHOD("get_outline", "idx"), &NavigationPolygon::get_outline);
	ClassDB::bind_method(D_METHOD("remove_outline", "idx"), &NavigationPolygon::remove_outline);
	ClassDB::bind_method(D_METHOD("clear_outlines"), &NavigationPolygon::clear_outlines);
	ClassDB::bind_method(D_METHOD("make_polygons_from_outlines"), &NavigationPolygon::make_polygons_from_outlines);

	ClassDB::bind_method(D_METHOD("commit_changes"), &NavigationPolygon::commit_changes);

	ClassDB::bind_method(D_METHOD("get_navigation_mesh"), &NavigationPolygon::get_navigation_mesh);

	ClassDB::bind_method(D_METHOD("set_polygons", "polygons"), &NavigationPolygon::_set_polygons);
	ClassDB::bind_method(D_METHOD("get_polygons"), &NavigationPolygon::_get_polygons);

	ClassDB::bind_method(D_METHOD("set_outlines", "outlines"), &NavigationPolygon::_set_outlines);
	ClassDB::bind_method(D_METHOD("get_outlines"), &NavigationPolygon::_get_outlines);

	ClassDB::bind_method(D_METHOD("set_baked_outlines", "outlines"), &NavigationPolygon::_set_baked_outlines);
	ClassDB::bind_method(D_METHOD("get_baked_outlines"), &NavigationPolygon::_get_baked_outlines);

	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "vertices", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "set_vertices", "get_vertices");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "polygons", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "set_polygons", "get_polygons");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "outlines", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "set_outlines", "get_outlines");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "baked_outlines", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "set_baked_outlines", "get_baked_outlines");

	ADD_GROUP("Polygons", "polygon_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "polygon_bake_fillrule", PROPERTY_HINT_ENUM, "EvenOdd,NonZero,Positive,Negative"), "set_polygon_bake_fillrule", "get_polygon_bake_fillrule");
	ADD_GROUP("Geometry", "geometry_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "geometry_parsed_geometry_type", PROPERTY_HINT_ENUM, "Mesh Instances,Static Colliders,Both"), "set_parsed_geometry_type", "get_parsed_geometry_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "geometry_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY_DEFAULT("geometry_collision_mask", 0xFFFFFFFF);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "geometry_source_geometry_mode", PROPERTY_HINT_ENUM, "Root Node Children,Group With Children,Group Explicit"), "set_source_geometry_mode", "get_source_geometry_mode");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "geometry_source_group_name"), "set_source_group_name", "get_source_group_name");
	ADD_PROPERTY_DEFAULT("geometry_source_group_name", StringName("navigation_polygon_source_group"));
	ADD_GROUP("Agents", "agent_");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "agent_radius", PROPERTY_HINT_RANGE, "0.0,500.0,0.01,or_greater,suffix:px"), "set_agent_radius", "get_agent_radius");
	ADD_GROUP("Offsetting", "offsetting_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "offsetting_jointype", PROPERTY_HINT_ENUM, "Square,Round,Miter"), "set_offsetting_jointype", "get_offsetting_jointype");

	BIND_ENUM_CONSTANT(PARSED_GEOMETRY_MESH_INSTANCES);
	BIND_ENUM_CONSTANT(PARSED_GEOMETRY_STATIC_COLLIDERS);
	BIND_ENUM_CONSTANT(PARSED_GEOMETRY_BOTH);
	BIND_ENUM_CONSTANT(PARSED_GEOMETRY_MAX);

	BIND_ENUM_CONSTANT(SOURCE_GEOMETRY_ROOT_NODE_CHILDREN);
	BIND_ENUM_CONSTANT(SOURCE_GEOMETRY_GROUPS_WITH_CHILDREN);
	BIND_ENUM_CONSTANT(SOURCE_GEOMETRY_GROUPS_EXPLICIT);
	BIND_ENUM_CONSTANT(SOURCE_GEOMETRY_MAX);

	BIND_ENUM_CONSTANT(POLYGON_FILLRULE_EVENODD);
	BIND_ENUM_CONSTANT(POLYGON_FILLRULE_NONZERO);
	BIND_ENUM_CONSTANT(POLYGON_FILLRULE_POSITIVE);
	BIND_ENUM_CONSTANT(POLYGON_FILLRULE_NEGATIVE);
	BIND_ENUM_CONSTANT(POLYGON_FILLRULE_MAX);

	BIND_ENUM_CONSTANT(OFFSETTING_JOINTYPE_SQUARE);
	BIND_ENUM_CONSTANT(OFFSETTING_JOINTYPE_ROUND);
	BIND_ENUM_CONSTANT(OFFSETTING_JOINTYPE_MITER);
	BIND_ENUM_CONSTANT(OFFSETTING_JOINTYPE_MAX);
}
