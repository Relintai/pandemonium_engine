#ifndef PANDEMONIUM_NAVIGATION_MESH_GENERATOR_H
#define PANDEMONIUM_NAVIGATION_MESH_GENERATOR_H

/*************************************************************************/
/*  pandemonium_navigation_mesh_generator.h                              */
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

#include "servers/navigation/navigation_mesh_generator.h"

#include "core/os/thread_pool.h"
#include "core/os/thread_pool_job.h"

#include "core/object/func_ref.h"

class NavigationPolygon;
class NavigationMeshSourceGeometryData2D;
class NavigationGeometryParser2D;
class NavigationMeshSourceGeometryData3D;
class NavigationGeometryParser3D;

class PandemoniumNavigationMeshGenerator : public NavigationMeshGenerator {
	GDCLASS(PandemoniumNavigationMeshGenerator, NavigationMeshGenerator);

public:
	// =======   TASKS   =======

	class NavigationGeneratorTask2D : public ThreadPoolJob {
		GDCLASS(NavigationGeneratorTask2D, ThreadPoolJob);

	public:
		enum TaskStatus {
			PARSING_REQUIRED,
			PARSING_STARTED,
			PARSING_FINISHED,
			PARSING_FAILED,
			BAKING_STARTED,
			BAKING_FINISHED,
			BAKING_FAILED,
		};

		Ref<NavigationPolygon> navigation_polygon;
		ObjectID parse_root_object_id;
		Ref<NavigationMeshSourceGeometryData2D> source_geometry_data;
		Ref<FuncRef> callback;
		NavigationGeneratorTask2D::TaskStatus status = NavigationGeneratorTask2D::TaskStatus::PARSING_REQUIRED;
		LocalVector<Ref<NavigationGeometryParser2D>> geometry_parsers;

		void call_callback();

		void _execute();

	protected:
		static void _bind_methods();
	};

#ifndef _3D_DISABLED
	class NavigationGeneratorTask3D : public ThreadPoolJob {
		GDCLASS(NavigationGeneratorTask3D, ThreadPoolJob);

	public:
		enum TaskStatus {
			PARSING_REQUIRED,
			PARSING_STARTED,
			PARSING_FINISHED,
			PARSING_FAILED,
			BAKING_STARTED,
			BAKING_FINISHED,
			BAKING_FAILED
		};

		Ref<NavigationMesh> navigation_mesh;
		ObjectID parse_root_object_id;
		Ref<NavigationMeshSourceGeometryData3D> source_geometry_data;
		Ref<FuncRef> callback;
		NavigationGeneratorTask3D::TaskStatus status = NavigationGeneratorTask3D::TaskStatus::PARSING_REQUIRED;
		LocalVector<Ref<NavigationGeometryParser3D>> geometry_parsers;

		void call_callback();

		void _execute();

	protected:
		static void _bind_methods();
	};
#endif // _3D_DISABLED

	// =======   TASKS END  =======

public:
	virtual void process();
	virtual void cleanup();

	// 2D ////////////////////////////////////
	virtual void register_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser);
	virtual void unregister_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser);

	virtual Ref<NavigationMeshSourceGeometryData2D> parse_2d_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback = Ref<FuncRef>());
	virtual void bake_2d_from_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, Ref<FuncRef> p_callback = Ref<FuncRef>());

	virtual void parse_and_bake_2d(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback = Ref<FuncRef>());

	static void _static_parse_2d_geometry_node(Ref<NavigationPolygon> p_navigation_polygon, Node *p_node, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, bool p_recurse_children, LocalVector<Ref<NavigationGeometryParser2D>> &p_geometry_2d_parsers);
	static void _static_parse_2d_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, LocalVector<Ref<NavigationGeometryParser2D>> &p_geometry_2d_parsers);
	static void _static_bake_2d_from_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data);

	virtual bool is_navigation_polygon_baking(Ref<NavigationPolygon> p_navigation_polygon) const;

#ifndef _3D_DISABLED
	virtual void register_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser);
	virtual void unregister_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser);

	virtual Ref<NavigationMeshSourceGeometryData3D> parse_3d_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback = Ref<FuncRef>());
	virtual void bake_3d_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Ref<FuncRef> p_callback = Ref<FuncRef>());

	virtual void parse_and_bake_3d(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback = Ref<FuncRef>());

	static void _static_parse_3d_geometry_node(Ref<NavigationMesh> p_navigation_mesh, Node *p_node, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, bool p_recurse_children, LocalVector<Ref<NavigationGeometryParser3D>> &p_geometry_3d_parsers);
	static void _static_parse_3d_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, LocalVector<Ref<NavigationGeometryParser3D>> &p_geometry_3d_parsers);
	static void _static_bake_3d_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data);

	virtual bool is_navigation_mesh_baking(Ref<NavigationMesh> p_navigation_mesh) const;
#endif // _3D_DISABLED

	PandemoniumNavigationMeshGenerator();
	~PandemoniumNavigationMeshGenerator();

private:
	void _process_2d_tasks();
	void _process_2d_parse_tasks();
	void _process_2d_bake_cleanup_tasks();

#ifndef _3D_DISABLED
	void _process_3d_tasks();
	void _process_3d_parse_tasks();
	void _process_3d_bake_cleanup_tasks();
#endif // _3D_DISABLED

private:
	Mutex _generator_mutex;

	bool _use_thread_pool = true;
	// TODO implement support into ThreadPool
	//bool _baking_use_high_priority_threads = true;

	LocalVector<Ref<NavigationGeometryParser2D>> _geometry_2d_parsers;

	LocalVector<Ref<NavigationPolygon>> _baking_navigation_polygons;
	LocalVector<Ref<NavigationGeneratorTask2D>> _2d_parse_jobs;
	LocalVector<Ref<NavigationGeneratorTask2D>> _2d_running_jobs;

#ifndef _3D_DISABLED
	LocalVector<Ref<NavigationGeometryParser3D>> _geometry_3d_parsers;

	LocalVector<Ref<NavigationMesh>> _baking_navigation_meshes;
	LocalVector<Ref<NavigationGeneratorTask3D>> _3d_parse_jobs;
	LocalVector<Ref<NavigationGeneratorTask3D>> _3d_running_jobs;
#endif // _3D_DISABLED
};

#endif // GODOT_NAVIGATION_MESH_GENERATOR_H
