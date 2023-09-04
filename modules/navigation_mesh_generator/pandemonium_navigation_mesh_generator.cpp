/**************************************************************************/
/*  godot_navigation_mesh_generator.cpp                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Pandemonium Engine contributors (see AUTHORS.md). */
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

#include "pandemonium_navigation_mesh_generator.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/core_string_names.h"

#include "scene/2d/navigation_geometry_parser_2d.h"
#include "scene/3d/navigation_geometry_parser_3d.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/navigation_mesh_source_geometry_data_3d.h"

#ifndef _3D_DISABLED
#include "scene/3d/mesh_instance.h"
#endif // _3D_DISABLED

#ifndef _3D_DISABLED
#include <Recast.h>
#endif // _3D_DISABLED

// todo port p_navigation_polygon->get_source_geometry_mode()

void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask2D::call_callback() {
	if (callback.is_valid() && callback->is_valid()) {
		Array arr;
		arr.push_back(navigation_polygon);
		callback->call_funcv(arr);
	}
}

void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask2D::_execute() {
	if (navigation_polygon.is_null() || source_geometry_data.is_null() || !source_geometry_data->has_data()) {
		set_complete(true);
		return;
	}

	_static_bake_2d_from_source_geometry_data(navigation_polygon, source_geometry_data);

	set_complete(true);
}

void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask2D::_bind_methods() {
}

#ifndef _3D_DISABLED
void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask3D::call_callback() {
	if (callback.is_valid() && callback->is_valid()) {
		Array arr;
		arr.push_back(navigation_mesh);
		callback->call_funcv(arr);
	}
}

void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask3D::_execute() {
	if (navigation_mesh.is_null() || source_geometry_data.is_null() || !source_geometry_data->has_data()) {
		set_complete(true);
		return;
	}

	_static_bake_3d_from_source_geometry_data(navigation_mesh, source_geometry_data);

	set_complete(true);
}

void PandemoniumNavigationMeshGenerator::NavigationGeneratorTask3D::_bind_methods() {
}
#endif // _3D_DISABLED

void PandemoniumNavigationMeshGenerator::process() {
	_generator_mutex.lock();

	_process_2d_tasks();
#ifndef _3D_DISABLED
	_process_3d_tasks();
#endif // _3D_DISABLED

	_generator_mutex.unlock();
}

void PandemoniumNavigationMeshGenerator::cleanup() {
	_baking_navigation_polygons.clear();
	_geometry_2d_parsers.clear();
#ifndef _3D_DISABLED
	_baking_navigation_meshes.clear();
	_geometry_3d_parsers.clear();
#endif // _3D_DISABLED
}

void PandemoniumNavigationMeshGenerator::_process_2d_tasks() {
	if (_2d_parse_jobs.size() == 0 && _2d_running_jobs.size() == 0) {
		return;
	}

	_process_2d_parse_tasks();
	_process_2d_bake_cleanup_tasks();
}

void PandemoniumNavigationMeshGenerator::_process_2d_parse_tasks() {
	// Note that this cannot be parallelized in a simple way, because we need mainloop to not delete nodes under it while processing.
	// If parallelization is implemented, it needs to wait for the tasks to complete. (Like in the original pr.)
	// Also the RenderingServer has locks that need the main thread to be active, so it can deadlock.

	// TODO implement ThreadPool like max processing per frame support

	if (_2d_parse_jobs.size() > 0) {
		while (_2d_parse_jobs.size() > 0) {
			Ref<NavigationGeneratorTask2D> navigation_generator_task = _2d_parse_jobs[0];

			_2d_parse_jobs.remove(0);

			if (navigation_generator_task.is_null()) {
				continue;
			}

			Ref<NavigationPolygon> navigation_polygon = navigation_generator_task->navigation_polygon;
			Ref<NavigationMeshSourceGeometryData2D> source_geometry_data = navigation_generator_task->source_geometry_data;
			ObjectID parse_root_object_id = navigation_generator_task->parse_root_object_id;

			if (navigation_polygon.is_null() || parse_root_object_id == ObjectID()) {
				navigation_generator_task->status = NavigationGeneratorTask2D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				continue;
			}
			Object *parse_root_obj = ObjectDB::get_instance(parse_root_object_id);
			if (parse_root_obj == nullptr) {
				navigation_generator_task->status = NavigationGeneratorTask2D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				return;
			}
			Node *parse_root_node = Object::cast_to<Node>(parse_root_obj);
			if (parse_root_node == nullptr) {
				navigation_generator_task->status = NavigationGeneratorTask2D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				return;
			}

			_static_parse_2d_source_geometry_data(navigation_polygon, parse_root_node, source_geometry_data, navigation_generator_task->geometry_parsers);

			navigation_generator_task->status = NavigationGeneratorTask2D::TaskStatus::PARSING_FINISHED;

			// Submit resulting class To threadpool
			navigation_generator_task->set_complete(false);
			if (_use_thread_pool) {
				ThreadPool::get_singleton()->add_job(navigation_generator_task);
			} else {
				navigation_generator_task->execute();
			}
			_2d_running_jobs.push_back(navigation_generator_task);
		}
	}
}

void PandemoniumNavigationMeshGenerator::_process_2d_bake_cleanup_tasks() {
	for (uint32_t i = 0; i < _2d_running_jobs.size(); ++i) {
		Ref<NavigationGeneratorTask2D> &e = _2d_running_jobs[i];

		if (e->get_complete()) {
			_2d_running_jobs.remove_unordered(i);
			--i;

			int indx = _baking_navigation_polygons.find(e->navigation_polygon);
			_baking_navigation_polygons.remove_unordered(indx);

			e->call_callback();
		}
	}
}

void PandemoniumNavigationMeshGenerator::register_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser) {
	_generator_mutex.lock();
	if (_geometry_2d_parsers.find(p_geometry_parser) < 0) {
		_geometry_2d_parsers.push_back(p_geometry_parser);
	}
	_generator_mutex.unlock();
}

void PandemoniumNavigationMeshGenerator::unregister_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser) {
	_generator_mutex.lock();
	_geometry_2d_parsers.erase(p_geometry_parser);
	_generator_mutex.unlock();
}

Ref<NavigationMeshSourceGeometryData2D> PandemoniumNavigationMeshGenerator::parse_2d_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_V_MSG(!p_navigation_polygon.is_valid(), Ref<NavigationMeshSourceGeometryData3D>(), "Invalid navigation mesh.");
	ERR_FAIL_COND_V_MSG(p_root_node == nullptr, Ref<NavigationMeshSourceGeometryData2D>(), "No parsing root node specified.");

	ObjectID root_node_object_id = p_root_node->get_instance_id();
	ERR_FAIL_COND_V_MSG(root_node_object_id == ObjectID(), Ref<NavigationMeshSourceGeometryData2D>(), "No root node object invalid.");

	Ref<NavigationMeshSourceGeometryData2D> source_geometry_data = Ref<NavigationMeshSourceGeometryData2D>(memnew(NavigationMeshSourceGeometryData2D));

	_static_parse_2d_source_geometry_data(p_navigation_polygon, p_root_node, source_geometry_data, _geometry_2d_parsers);

	return source_geometry_data;
};

void PandemoniumNavigationMeshGenerator::bake_2d_from_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_MSG(!p_navigation_polygon.is_valid(), "Invalid navigation mesh.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data.is_valid(), "Invalid NavigationMeshSourceGeometryData2D.");
	ERR_FAIL_COND_MSG(p_navigation_polygon->get_outline_count() == 0 && !p_source_geometry_data->has_data(), "NavigationMeshSourceGeometryData2D is empty. Parse source geometry first.");
	ERR_FAIL_COND_MSG(_baking_navigation_polygons.find(p_navigation_polygon) >= 0, "NavigationPolygon is already baking. Wait for current bake task to finish.");

	_generator_mutex.lock();
	_baking_navigation_polygons.push_back(p_navigation_polygon);
	_generator_mutex.unlock();

	_static_bake_2d_from_source_geometry_data(p_navigation_polygon, p_source_geometry_data);

	_generator_mutex.lock();
	int64_t navigation_polygon_index = _baking_navigation_polygons.find(p_navigation_polygon);
	if (navigation_polygon_index >= 0) {
		_baking_navigation_polygons.remove_unordered(navigation_polygon_index);
	}
	_generator_mutex.unlock();
}

void PandemoniumNavigationMeshGenerator::_static_parse_2d_geometry_node(Ref<NavigationPolygon> p_navigation_polygon, Node *p_node, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, bool p_recurse_children, LocalVector<Ref<NavigationGeometryParser2D>> &p_geometry_2d_parsers) {
	for (uint32_t i = 0; i < p_geometry_2d_parsers.size(); ++i) {
		Ref<NavigationGeometryParser2D> &geometry_2d_parser = p_geometry_2d_parsers[i];

		if (geometry_2d_parser->parses_node(p_node)) {
			geometry_2d_parser->parse_node_geometry(p_navigation_polygon, p_node, p_source_geometry_data);
		};
	};

	if (p_recurse_children) {
		for (int i = 0; i < p_node->get_child_count(); i++) {
			_static_parse_2d_geometry_node(p_navigation_polygon, p_node->get_child(i), p_source_geometry_data, p_recurse_children, p_geometry_2d_parsers);
		}
	}
}

void PandemoniumNavigationMeshGenerator::_static_parse_2d_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, LocalVector<Ref<NavigationGeometryParser2D>> &p_geometry_2d_parsers) {
	ERR_FAIL_COND_MSG(!p_navigation_polygon.is_valid(), "Invalid navigation polygon.");
	ERR_FAIL_COND_MSG(p_root_node == nullptr, "Invalid parse root node.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data.is_valid(), "Invalid source geometry data.");

	List<Node *> parse_nodes;

	if (p_navigation_polygon->get_source_geometry_mode() == NavigationPolygon::SOURCE_GEOMETRY_ROOT_NODE_CHILDREN) {
		parse_nodes.push_back(p_root_node);
	} else {
		p_root_node->get_tree()->get_nodes_in_group(p_navigation_polygon->get_source_group_name(), &parse_nodes);
	}

	Transform2D root_node_transform = Object::cast_to<Node2D>(p_root_node)->get_global_transform().affine_inverse();
	bool recurse_children = p_navigation_polygon->get_source_geometry_mode() != NavigationPolygon::SOURCE_GEOMETRY_GROUPS_EXPLICIT;

	p_source_geometry_data->clear();
	p_source_geometry_data->root_node_transform = root_node_transform;

	for (List<Node *>::Element *E = parse_nodes.front(); E; E = E->next()) {
		_static_parse_2d_geometry_node(p_navigation_polygon, E->get(), p_source_geometry_data, recurse_children, p_geometry_2d_parsers);
	}
}

void PandemoniumNavigationMeshGenerator::_static_bake_2d_from_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data) {
	ERR_FAIL_COND_MSG(!p_navigation_polygon.is_valid(), "Invalid navigation polygon.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data.is_valid(), "Invalid source geometry data.");
	ERR_FAIL_COND_MSG(p_navigation_polygon->get_outline_count() == 0 && !p_source_geometry_data->has_data(), "NavigationMeshSourceGeometryData2D is empty. Parse source geometry first.");

	const Vector<Vector<Vector2>> &traversable_outlines = p_source_geometry_data->_get_traversable_outlines();
	const Vector<Vector<Vector2>> &obstruction_outlines = p_source_geometry_data->_get_obstruction_outlines();

	int outline_count = p_navigation_polygon->get_outline_count();

	Vector<Vector<Point2>> s_traversable_polygon_paths;

	for (int i = 0; i < outline_count; i++) {
		PoolVector<Vector2> traversable_outline = p_navigation_polygon->get_outline(i);
		Vector<Point2> subject_path;
		for (int j = 0; j < traversable_outline.size(); ++j) {
			Vector2 traversable_point = traversable_outline[j];
			subject_path.push_back(traversable_point);
		}
		s_traversable_polygon_paths.push_back(subject_path);
	}

	Vector<Vector<Point2>> s_traversable_outlines;

	for (int i = 0; i < traversable_outlines.size(); i++) {
		Vector<Vector2> traversable_outline = traversable_outlines[i];

		Vector<Point2> subject_path;
		for (int j = 0; j < traversable_outline.size(); ++j) {
			Vector2 traversable_point = traversable_outline[j];
			subject_path.push_back(traversable_point);
		}
		s_traversable_outlines.push_back(subject_path);
	}

	Vector<Vector<Point2>> s_obstruction_polygon_paths;

	for (int i = 0; i < obstruction_outlines.size(); i++) {
		Vector<Vector2> obstruction_outline = obstruction_outlines[i];

		Vector<Point2> clip_path;

		for (int j = 0; j < obstruction_outline.size(); ++j) {
			Vector2 traversable_point = obstruction_outline[j];
			clip_path.push_back(traversable_point);
		}
		s_obstruction_polygon_paths.push_back(clip_path);
	}

	Geometry::PolygonFillType geom_fillrule = Geometry::POLYGON_FILL_TYPE_EVEN_ODD;

	switch (p_navigation_polygon->get_polygon_bake_fillrule()) {
		case NavigationPolygon::POLYGON_FILLRULE_EVENODD: {
			geom_fillrule = Geometry::POLYGON_FILL_TYPE_EVEN_ODD;
		} break;
		case NavigationPolygon::POLYGON_FILLRULE_NONZERO: {
			geom_fillrule = Geometry::POLYGON_FILL_TYPE_NON_ZERO;
		} break;
		case NavigationPolygon::POLYGON_FILLRULE_POSITIVE: {
			geom_fillrule = Geometry::POLYGON_FILL_TYPE_POSITIVE;
		} break;
		case NavigationPolygon::POLYGON_FILLRULE_NEGATIVE: {
			geom_fillrule = Geometry::POLYGON_FILL_TYPE_NEGATIVE;
		} break;
		default: {
			WARN_PRINT_ONCE("No match for used NavigationPolygon::POLYGON_FILLRULE - fallback to default");
			geom_fillrule = Geometry::POLYGON_FILL_TYPE_EVEN_ODD;
		} break;
	}

	// first merge all traversable polygons according to user specified fill rule
	s_traversable_polygon_paths = Geometry::merge_all_polygons_2d(s_traversable_polygon_paths, Vector<Point2>(), geom_fillrule);
	// merge all obstruction polygons, don't allow holes for what is considered "solid" 2D geometry
	s_obstruction_polygon_paths = Geometry::merge_all_polygons_2d(s_obstruction_polygon_paths, Vector<Point2>(), Geometry::POLYGON_FILL_TYPE_NON_ZERO);

	Vector<Vector<Point2>> s_path_solution;
	s_path_solution = Geometry::clip_all2_polygons_2d(s_traversable_polygon_paths, s_obstruction_polygon_paths, geom_fillrule);

	// Seems to be bugged when dealing with 2d
	/*
	JoinType clipper_jointype = JoinType::Square;

	switch (p_navigation_polygon->get_offsetting_jointype()) {
		case NavigationPolygon::OFFSETTING_JOINTYPE_SQUARE: {
			clipper_jointype = JoinType::Square;
		} break;
		case NavigationPolygon::OFFSETTING_JOINTYPE_ROUND: {
			clipper_jointype = JoinType::Round;
		} break;
		case NavigationPolygon::OFFSETTING_JOINTYPE_MITER: {
			clipper_jointype = JoinType::Miter;
		} break;
		default: {
			WARN_PRINT_ONCE("No match for used NavigationPolygon::OFFSETTING_JOINTYPE - fallback to default");
			clipper_jointype = JoinType::Square;
		} break;
	}

	real_t agent_radius_offset = p_navigation_polygon->get_agent_radius();
	if (agent_radius_offset > 0.0) {
		path_solution = InflatePaths(path_solution, -agent_radius_offset, clipper_jointype, EndType::Polygon);
	}
	*/
	//path_solution = RamerDouglasPeucker(path_solution, 0.025); //

	Vector<PoolVector<Vector2>> new_baked_outlines;

	for (int i = 0; i < s_path_solution.size(); i++) {
		const Vector<Point2> &scaled_path = s_path_solution[i];

		PoolVector<Vector2> polypath;
		for (int j = 0; j < scaled_path.size(); ++j) {
			const Vector2 &scaled_point = scaled_path[j];

			polypath.push_back(Vector2(static_cast<real_t>(scaled_point.x), static_cast<real_t>(scaled_point.y)));
		}

		new_baked_outlines.push_back(polypath);
	}

	p_navigation_polygon->set_baked_outlines(new_baked_outlines);

	if (new_baked_outlines.size() == 0) {
		p_navigation_polygon->set_vertices(PoolVector<Vector2>());
		p_navigation_polygon->set_polygons(Vector<Vector<int>>());
		p_navigation_polygon->commit_changes();
		return;
	}

	Vector<Vector<Point2>> s_polygon_paths;

	for (int i = 0; i < new_baked_outlines.size(); i++) {
		const PoolVector<Vector2> &baked_outline = new_baked_outlines[i];

		Vector<Point2> polygon_path;
		for (int j = 0; j < baked_outline.size(); ++j) {
			const Vector2 &baked_outline_point = baked_outline[j];
			polygon_path.push_back(baked_outline_point);
		}
		s_polygon_paths.push_back(polygon_path);
	}

	PoolVector<Vector2> s_new_vertices;
	Vector<Vector<int>> s_new_polygons;

	if (!Geometry::merge_convex_decompose_polygon_2d(s_polygon_paths, s_new_vertices, s_new_polygons)) {
		ERR_PRINT("NavigationPolygon Convex partition failed. Unable to create a valid NavigationMesh from defined polygon outline paths.");
		p_navigation_polygon->set_vertices(PoolVector<Vector2>());
		p_navigation_polygon->set_polygons(Vector<Vector<int>>());
		p_navigation_polygon->commit_changes();
		return;
	}

	p_navigation_polygon->set_vertices(s_new_vertices);
	p_navigation_polygon->set_polygons(s_new_polygons);
	p_navigation_polygon->commit_changes();
}

void PandemoniumNavigationMeshGenerator::parse_and_bake_2d(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_MSG(_baking_navigation_polygons.find(p_navigation_polygon) >= 0, "NavigationPolygon was already added to baking queue. Wait for current bake task to finish.");
	ERR_FAIL_COND_MSG(p_root_node == nullptr, "NavigationPolygon requires a valid root node.");

	_generator_mutex.lock();
	_baking_navigation_polygons.push_back(p_navigation_polygon);
	_generator_mutex.unlock();

	Ref<NavigationGeneratorTask2D> navigation_generator_task;
	navigation_generator_task.instance();
	navigation_generator_task->navigation_polygon = p_navigation_polygon;
	navigation_generator_task->parse_root_object_id = p_root_node->get_instance_id();
	navigation_generator_task->source_geometry_data = Ref<NavigationMeshSourceGeometryData2D>(memnew(NavigationMeshSourceGeometryData2D));
	navigation_generator_task->callback = p_callback;
	navigation_generator_task->status = NavigationGeneratorTask2D::TaskStatus::PARSING_REQUIRED;
	navigation_generator_task->geometry_parsers = _geometry_2d_parsers;

	_generator_mutex.lock();
	_2d_parse_jobs.push_back(navigation_generator_task);
	_generator_mutex.unlock();
}

bool PandemoniumNavigationMeshGenerator::is_navigation_polygon_baking(Ref<NavigationPolygon> p_navigation_polygon) const {
	ERR_FAIL_COND_V(!p_navigation_polygon.is_valid(), false);
	return _baking_navigation_polygons.find(p_navigation_polygon) >= 0;
}

#ifndef _3D_DISABLED
void PandemoniumNavigationMeshGenerator::_process_3d_tasks() {
	if (_3d_parse_jobs.size() == 0 && _3d_running_jobs.size() == 0) {
		return;
	}

	_process_3d_parse_tasks();
	_process_3d_bake_cleanup_tasks();
}

void PandemoniumNavigationMeshGenerator::_process_3d_parse_tasks() {
	// Note that this cannot be parallelized in a simple way, because we need mainloop to not delete nodes under it while processing.
	// If parallelization is implemented, it needs to wait for the tasks to complete. (Like in the original pr.)
	// Also the RenderingServer has locks that need the main thread to be active, so it can deadlock.

	// TODO implement ThreadPool like max processing per frame support

	if (_3d_parse_jobs.size() > 0) {
		while (_3d_parse_jobs.size() > 0) {
			Ref<NavigationGeneratorTask3D> navigation_generator_task = _3d_parse_jobs[0];

			_3d_parse_jobs.remove(0);

			if (navigation_generator_task.is_null()) {
				continue;
			}

			Ref<NavigationMesh> navigation_polygon = navigation_generator_task->navigation_mesh;
			Ref<NavigationMeshSourceGeometryData3D> source_geometry_data = navigation_generator_task->source_geometry_data;
			ObjectID parse_root_object_id = navigation_generator_task->parse_root_object_id;

			if (navigation_polygon.is_null() || parse_root_object_id == ObjectID()) {
				navigation_generator_task->status = NavigationGeneratorTask3D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				continue;
			}
			Object *parse_root_obj = ObjectDB::get_instance(parse_root_object_id);
			if (parse_root_obj == nullptr) {
				navigation_generator_task->status = NavigationGeneratorTask3D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				return;
			}
			Node *parse_root_node = Object::cast_to<Node>(parse_root_obj);
			if (parse_root_node == nullptr) {
				navigation_generator_task->status = NavigationGeneratorTask3D::TaskStatus::PARSING_FAILED;
				navigation_generator_task->call_callback();
				return;
			}

			_static_parse_3d_source_geometry_data(navigation_polygon, parse_root_node, source_geometry_data, navigation_generator_task->geometry_parsers);

			navigation_generator_task->status = NavigationGeneratorTask3D::TaskStatus::PARSING_FINISHED;

			// Submit resulting class To threadpool
			navigation_generator_task->set_complete(false);
			if (_use_thread_pool) {
				ThreadPool::get_singleton()->add_job(navigation_generator_task);
			} else {
				navigation_generator_task->execute();
			}
			_3d_running_jobs.push_back(navigation_generator_task);
		}
	}
}

void PandemoniumNavigationMeshGenerator::_process_3d_bake_cleanup_tasks() {
	for (uint32_t i = 0; i < _3d_running_jobs.size(); ++i) {
		Ref<NavigationGeneratorTask3D> &e = _3d_running_jobs[i];

		if (e->get_complete()) {
			_3d_running_jobs.remove_unordered(i);
			--i;

			int indx = _baking_navigation_polygons.find(e->navigation_mesh);
			_baking_navigation_polygons.remove_unordered(indx);

			e->call_callback();
		}
	}
}

void PandemoniumNavigationMeshGenerator::_static_bake_3d_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data) {
	if (p_navigation_mesh.is_null() || p_source_geometry_data.is_null()) {
		return;
	}

#ifndef _3D_DISABLED
	PoolRealArray vertices = p_source_geometry_data->get_vertices();
	PoolIntArray indices = p_source_geometry_data->get_indices();

	if (vertices.size() < 3 || indices.size() < 3) {
		return;
	}

	rcHeightfield *hf = nullptr;
	rcCompactHeightfield *chf = nullptr;
	rcContourSet *cset = nullptr;
	rcPolyMesh *poly_mesh = nullptr;
	rcPolyMeshDetail *detail_mesh = nullptr;
	rcContext ctx;

	// added to keep track of steps, no functionality rightnow
	String bake_state = "";

	bake_state = "Setting up Configuration..."; // step #1

	PoolRealArray::Read vertices_read = vertices.read();
	PoolIntArray::Read indices_read = indices.read();
	const float *verts = vertices_read.ptr();
	const int nverts = vertices.size() / 3;
	const int *tris = indices_read.ptr();
	const int ntris = indices.size() / 3;

	float bmin[3], bmax[3];
	rcCalcBounds(verts, nverts, bmin, bmax);

	rcConfig cfg;
	memset(&cfg, 0, sizeof(cfg));

	cfg.cs = p_navigation_mesh->get_cell_size();
	cfg.ch = p_navigation_mesh->get_cell_height();
	cfg.walkableSlopeAngle = p_navigation_mesh->get_agent_max_slope();
	cfg.walkableHeight = (int)Math::ceil(p_navigation_mesh->get_agent_height() / cfg.ch);
	cfg.walkableClimb = (int)Math::floor(p_navigation_mesh->get_agent_max_climb() / cfg.ch);
	cfg.walkableRadius = (int)Math::ceil(p_navigation_mesh->get_agent_radius() / cfg.cs);
	cfg.maxEdgeLen = (int)(p_navigation_mesh->get_edge_max_length() / p_navigation_mesh->get_cell_size());
	cfg.maxSimplificationError = p_navigation_mesh->get_edge_max_error();
	cfg.minRegionArea = (int)(p_navigation_mesh->get_region_min_size() * p_navigation_mesh->get_region_min_size());
	cfg.mergeRegionArea = (int)(p_navigation_mesh->get_region_merge_size() * p_navigation_mesh->get_region_merge_size());
	cfg.maxVertsPerPoly = (int)p_navigation_mesh->get_vertices_per_polyon();
	cfg.detailSampleDist = MAX(p_navigation_mesh->get_cell_size() * p_navigation_mesh->get_detail_sample_distance(), 0.1f);
	cfg.detailSampleMaxError = p_navigation_mesh->get_cell_height() * p_navigation_mesh->get_detail_sample_max_error();

	cfg.bmin[0] = bmin[0];
	cfg.bmin[1] = bmin[1];
	cfg.bmin[2] = bmin[2];
	cfg.bmax[0] = bmax[0];
	cfg.bmax[1] = bmax[1];
	cfg.bmax[2] = bmax[2];

	AABB baking_aabb = p_navigation_mesh->get_filter_baking_aabb();
	if (!baking_aabb.has_no_volume()) {
		Vector3 baking_aabb_offset = p_navigation_mesh->get_filter_baking_aabb_offset();
		cfg.bmin[0] = baking_aabb.position[0] + baking_aabb_offset.x;
		cfg.bmin[1] = baking_aabb.position[1] + baking_aabb_offset.y;
		cfg.bmin[2] = baking_aabb.position[2] + baking_aabb_offset.z;
		cfg.bmax[0] = cfg.bmin[0] + baking_aabb.size[0];
		cfg.bmax[1] = cfg.bmin[1] + baking_aabb.size[1];
		cfg.bmax[2] = cfg.bmin[2] + baking_aabb.size[2];
	}

	bake_state = "Calculating grid size..."; // step #2

	rcCalcGridSize(cfg.bmin, cfg.bmax, cfg.cs, &cfg.width, &cfg.height);

	bake_state = "Creating heightfield..."; // step #3

	hf = rcAllocHeightfield();

	ERR_FAIL_COND(!hf);
	ERR_FAIL_COND(!rcCreateHeightfield(&ctx, *hf, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch));

	bake_state = "Marking walkable triangles..."; // step #4

	{
		Vector<unsigned char> tri_areas;
		tri_areas.resize(ntris);

		ERR_FAIL_COND(tri_areas.size() == 0);

		memset(tri_areas.ptrw(), 0, ntris * sizeof(unsigned char));
		rcMarkWalkableTriangles(&ctx, cfg.walkableSlopeAngle, verts, nverts, tris, ntris, tri_areas.ptrw());

		ERR_FAIL_COND(!rcRasterizeTriangles(&ctx, verts, nverts, tris, tri_areas.ptr(), ntris, *hf, cfg.walkableClimb));
	}

	if (p_navigation_mesh->get_filter_low_hanging_obstacles()) {
		rcFilterLowHangingWalkableObstacles(&ctx, cfg.walkableClimb, *hf);
	}
	if (p_navigation_mesh->get_filter_ledge_spans()) {
		rcFilterLedgeSpans(&ctx, cfg.walkableHeight, cfg.walkableClimb, *hf);
	}
	if (p_navigation_mesh->get_filter_walkable_low_height_spans()) {
		rcFilterWalkableLowHeightSpans(&ctx, cfg.walkableHeight, *hf);
	}

	bake_state = "Constructing compact heightfield..."; // step #5

	chf = rcAllocCompactHeightfield();

	ERR_FAIL_COND(!chf);
	ERR_FAIL_COND(!rcBuildCompactHeightfield(&ctx, cfg.walkableHeight, cfg.walkableClimb, *hf, *chf));

	rcFreeHeightField(hf);
	hf = nullptr;

	bake_state = "Eroding walkable area..."; // step #6

	ERR_FAIL_COND(!rcErodeWalkableArea(&ctx, cfg.walkableRadius, *chf));

	bake_state = "Partitioning..."; // step #7

	if (p_navigation_mesh->get_sample_partition_type() == NavigationMesh::SAMPLE_PARTITION_WATERSHED) {
		ERR_FAIL_COND(!rcBuildDistanceField(&ctx, *chf));
		ERR_FAIL_COND(!rcBuildRegions(&ctx, *chf, 0, cfg.minRegionArea, cfg.mergeRegionArea));
	} else if (p_navigation_mesh->get_sample_partition_type() == NavigationMesh::SAMPLE_PARTITION_MONOTONE) {
		ERR_FAIL_COND(!rcBuildRegionsMonotone(&ctx, *chf, 0, cfg.minRegionArea, cfg.mergeRegionArea));
	} else {
		ERR_FAIL_COND(!rcBuildLayerRegions(&ctx, *chf, 0, cfg.minRegionArea));
	}

	bake_state = "Creating contours..."; // step #8

	cset = rcAllocContourSet();

	ERR_FAIL_COND(!cset);
	ERR_FAIL_COND(!rcBuildContours(&ctx, *chf, cfg.maxSimplificationError, cfg.maxEdgeLen, *cset));

	bake_state = "Creating polymesh..."; // step #9

	poly_mesh = rcAllocPolyMesh();
	ERR_FAIL_COND(!poly_mesh);
	ERR_FAIL_COND(!rcBuildPolyMesh(&ctx, *cset, cfg.maxVertsPerPoly, *poly_mesh));

	detail_mesh = rcAllocPolyMeshDetail();
	ERR_FAIL_COND(!detail_mesh);
	ERR_FAIL_COND(!rcBuildPolyMeshDetail(&ctx, *poly_mesh, *chf, cfg.detailSampleDist, cfg.detailSampleMaxError, *detail_mesh));

	rcFreeCompactHeightfield(chf);
	chf = nullptr;
	rcFreeContourSet(cset);
	cset = nullptr;

	bake_state = "Converting to native navigation mesh..."; // step #10

	PoolVector<Vector3> new_navigation_mesh_vertices;
	Vector<Vector<int>> new_navigation_mesh_polygons;

	for (int i = 0; i < detail_mesh->nverts; i++) {
		const float *v = &detail_mesh->verts[i * 3];
		new_navigation_mesh_vertices.push_back(Vector3(v[0], v[1], v[2]));
	}

	for (int i = 0; i < detail_mesh->nmeshes; i++) {
		const unsigned int *detail_mesh_m = &detail_mesh->meshes[i * 4];
		const unsigned int detail_mesh_bverts = detail_mesh_m[0];
		const unsigned int detail_mesh_m_btris = detail_mesh_m[2];
		const unsigned int detail_mesh_ntris = detail_mesh_m[3];
		const unsigned char *detail_mesh_tris = &detail_mesh->tris[detail_mesh_m_btris * 4];
		for (unsigned int j = 0; j < detail_mesh_ntris; j++) {
			Vector<int> new_navigation_mesh_polygon;
			new_navigation_mesh_polygon.resize(3);
			// Polygon order in recast is opposite than godot's
			new_navigation_mesh_polygon.write[0] = ((int)(detail_mesh_bverts + detail_mesh_tris[j * 4 + 0]));
			new_navigation_mesh_polygon.write[1] = ((int)(detail_mesh_bverts + detail_mesh_tris[j * 4 + 2]));
			new_navigation_mesh_polygon.write[2] = ((int)(detail_mesh_bverts + detail_mesh_tris[j * 4 + 1]));
			new_navigation_mesh_polygons.push_back(new_navigation_mesh_polygon);
		}
	}

	p_navigation_mesh->set_vertices(new_navigation_mesh_vertices);
	p_navigation_mesh->set_polygons(new_navigation_mesh_polygons);

	bake_state = "Cleanup..."; // step #11

	rcFreePolyMesh(poly_mesh);
	poly_mesh = nullptr;
	rcFreePolyMeshDetail(detail_mesh);
	detail_mesh = nullptr;

	rcFreeHeightField(hf);
	hf = nullptr;

	rcFreeCompactHeightfield(chf);
	chf = nullptr;

	rcFreeContourSet(cset);
	cset = nullptr;

	rcFreePolyMesh(poly_mesh);
	poly_mesh = nullptr;

	rcFreePolyMeshDetail(detail_mesh);
	detail_mesh = nullptr;

	bake_state = "Baking finished."; // step #12
#endif // _3D_DISABLED
	p_navigation_mesh->commit_changes();
}

void PandemoniumNavigationMeshGenerator::parse_and_bake_3d(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_MSG(_baking_navigation_meshes.find(p_navigation_mesh) >= 0, "NavigationMesh was already added to baking queue. Wait for current bake task to finish.");
	ERR_FAIL_COND_MSG(p_root_node == nullptr, "avigationMesh requires a valid root node.");

	_generator_mutex.lock();
	_baking_navigation_meshes.push_back(p_navigation_mesh);
	_generator_mutex.unlock();

	Ref<NavigationGeneratorTask3D> navigation_generator_task;
	navigation_generator_task.instance();
	navigation_generator_task->navigation_mesh = p_navigation_mesh;

	navigation_generator_task->navigation_mesh = p_navigation_mesh;
	navigation_generator_task->parse_root_object_id = p_root_node->get_instance_id();
	navigation_generator_task->source_geometry_data = Ref<NavigationMeshSourceGeometryData3D>(memnew(NavigationMeshSourceGeometryData3D));
	navigation_generator_task->callback = p_callback;
	navigation_generator_task->status = NavigationGeneratorTask3D::TaskStatus::PARSING_REQUIRED;
	navigation_generator_task->geometry_parsers = _geometry_3d_parsers;

	_generator_mutex.lock();
	_3d_parse_jobs.push_back(navigation_generator_task);
	_generator_mutex.unlock();
}

bool PandemoniumNavigationMeshGenerator::is_navigation_mesh_baking(Ref<NavigationMesh> p_navigation_mesh) const {
	ERR_FAIL_COND_V(!p_navigation_mesh.is_valid(), false);
	return _baking_navigation_meshes.find(p_navigation_mesh) >= 0;
}

void PandemoniumNavigationMeshGenerator::register_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser) {
	_generator_mutex.lock();
	if (_geometry_3d_parsers.find(p_geometry_parser) < 0) {
		_geometry_3d_parsers.push_back(p_geometry_parser);
	}
	_generator_mutex.unlock();
}

void PandemoniumNavigationMeshGenerator::unregister_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser) {
	_generator_mutex.lock();
	_geometry_3d_parsers.erase(p_geometry_parser);
	_generator_mutex.unlock();
}

Ref<NavigationMeshSourceGeometryData3D> PandemoniumNavigationMeshGenerator::parse_3d_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_V_MSG(!p_navigation_mesh.is_valid(), Ref<NavigationMeshSourceGeometryData3D>(), "Invalid navigation mesh.");
	ERR_FAIL_COND_V_MSG(p_root_node == nullptr, Ref<NavigationMeshSourceGeometryData3D>(), "No parsing root node specified.");

	ObjectID root_node_object_id = p_root_node->get_instance_id();
	ERR_FAIL_COND_V_MSG(root_node_object_id == ObjectID(), Ref<NavigationMeshSourceGeometryData3D>(), "No root node object invalid.");

	Ref<NavigationMeshSourceGeometryData3D> source_geometry_data = Ref<NavigationMeshSourceGeometryData3D>(memnew(NavigationMeshSourceGeometryData3D));

	_static_parse_3d_source_geometry_data(p_navigation_mesh, p_root_node, source_geometry_data, _geometry_3d_parsers);

	return source_geometry_data;
};

void PandemoniumNavigationMeshGenerator::bake_3d_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Ref<FuncRef> p_callback) {
	ERR_FAIL_COND_MSG(!p_navigation_mesh.is_valid(), "Invalid navigation mesh.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data.is_valid(), "Invalid NavigationMeshSourceGeometryData3D.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data->has_data(), "NavigationMeshSourceGeometryData3D is empty. Parse source geometry first.");
	ERR_FAIL_COND_MSG(_baking_navigation_meshes.find(p_navigation_mesh) >= 0, "NavigationMesh is already baking. Wait for current bake task to finish.");

	_generator_mutex.lock();
	_baking_navigation_meshes.push_back(p_navigation_mesh);
	_generator_mutex.unlock();

	_static_bake_3d_from_source_geometry_data(p_navigation_mesh, p_source_geometry_data);

	_generator_mutex.lock();
	int64_t navigation_mesh_index = _baking_navigation_meshes.find(p_navigation_mesh);
	if (navigation_mesh_index >= 0) {
		_baking_navigation_meshes.remove_unordered(navigation_mesh_index);
	}
	_generator_mutex.unlock();
}

void PandemoniumNavigationMeshGenerator::_static_parse_3d_geometry_node(Ref<NavigationMesh> p_navigation_mesh, Node *p_node, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, bool p_recurse_children, LocalVector<Ref<NavigationGeometryParser3D>> &p_geometry_3d_parsers) {
	for (uint32_t i = 0; i < p_geometry_3d_parsers.size(); ++i) {
		Ref<NavigationGeometryParser3D> &geometry_3d_parser = p_geometry_3d_parsers[i];

		if (geometry_3d_parser->parses_node(p_node)) {
			geometry_3d_parser->parse_node_geometry(p_navigation_mesh, p_node, p_source_geometry_data);
		};
	};

	if (p_recurse_children) {
		for (int i = 0; i < p_node->get_child_count(); i++) {
			_static_parse_3d_geometry_node(p_navigation_mesh, p_node->get_child(i), p_source_geometry_data, p_recurse_children, p_geometry_3d_parsers);
		}
	}
}

void PandemoniumNavigationMeshGenerator::_static_parse_3d_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, LocalVector<Ref<NavigationGeometryParser3D>> &p_geometry_3d_parsers) {
	ERR_FAIL_COND_MSG(!p_navigation_mesh.is_valid(), "Invalid navigation mesh.");
	ERR_FAIL_COND_MSG(p_root_node == nullptr, "Invalid parse root node.");
	ERR_FAIL_COND_MSG(!p_source_geometry_data.is_valid(), "Invalid source geometry data.");

	List<Node *> parse_nodes;

	if (p_navigation_mesh->get_source_geometry_mode() == NavigationMesh::SOURCE_GEOMETRY_ROOT_NODE_CHILDREN) {
		parse_nodes.push_back(p_root_node);
	} else {
		p_root_node->get_tree()->get_nodes_in_group(p_navigation_mesh->get_source_group_name(), &parse_nodes);
	}

	Transform root_node_transform = Object::cast_to<Spatial>(p_root_node)->get_global_transform().affine_inverse();
	bool recurse_children = p_navigation_mesh->get_source_geometry_mode() != NavigationMesh::SOURCE_GEOMETRY_GROUPS_EXPLICIT;

	p_source_geometry_data->clear();
	p_source_geometry_data->root_node_transform = root_node_transform;

	for (List<Node *>::Element *E = parse_nodes.front(); E; E = E->next()) {
		_static_parse_3d_geometry_node(p_navigation_mesh, E->get(), p_source_geometry_data, recurse_children, p_geometry_3d_parsers);
	}
}
#endif // _3D_DISABLED

PandemoniumNavigationMeshGenerator::PandemoniumNavigationMeshGenerator() {
	_use_thread_pool = GLOBAL_GET("navigation/baking/thread_model/use_thread_pool");
	// Can't use threads in Editor as parsing gets stuck on RenderingServer / PhysicsServer locks.
	// (TODO needs to be tested) The way I have it now, it should work.
	//_use_thread_pool = !Engine::get_singleton()->is_editor_hint();

	//_baking_use_high_priority_threads = GLOBAL_DEF("navigation/baking/thread_model/baking_use_high_priority_threads");
}

PandemoniumNavigationMeshGenerator::~PandemoniumNavigationMeshGenerator() {
	cleanup();
}