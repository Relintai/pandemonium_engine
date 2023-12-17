#ifndef PROP_JOB_H
#define PROP_JOB_H

/*************************************************************************/
/*  prop_instance_prop_job.h                                             */
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

#include "prop_instance_job.h"

#include "modules/modules_enabled.gen.h"

class PropMesher;
class PropInstance;
class PropInstanceMerger;
class PropMesherJobStep;
class PropMaterialCache;
class Shape;
class PropLight;
class PropDataTiledWall;

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
class PropDataMeshData;
#endif

class PropInstancePropJob : public PropInstanceJob {
	GDCLASS(PropInstancePropJob, PropInstanceJob);

public:
	Ref<PropMaterialCache> get_material_cache();
	void set_material_cache(const Ref<PropMaterialCache> &cache);

	Ref<PropMesherJobStep> get_jobs_step(const int index) const;
	void set_jobs_step(const int index, const Ref<PropMesherJobStep> &step);
	void remove_jobs_step(const int index);
	void add_jobs_step(const Ref<PropMesherJobStep> &step);
	int get_jobs_step_count() const;

	void add_collision_shape(const Ref<Shape> &shape, const Transform &transform, const bool owns_shape = false);
	void clear_collision_shapes();

	PropInstanceMerger *get_prop_instace();
	void set_prop_instace(PropInstanceMerger *prop);
	void set_prop_instace_bind(Node *prop);

	Ref<PropMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<PropMesher> &mesher);

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	void add_mesh(const Ref<PropDataMeshData> &mesh_data, const Transform &base_transform);
	void clear_meshes();
#endif

	void add_tiled_wall(const Ref<PropDataTiledWall> &data, const Transform &base_transform);
	void clear_tiled_walls();

	void add_light(const Ref<PropLight> &light);
	void clear_lights();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup_cache();
	void phase_physics_process();
	void phase_prop();
	void phase_steps();

	void step_type_normal();
	void step_type_normal_lod();
	void step_type_drop_uv2();
	void step_type_merge_verts();
	void step_type_bake_texture();
	void step_type_simplify_mesh();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	void reset_meshes();

	PropInstancePropJob();
	~PropInstancePropJob();

protected:
	static void _bind_methods();

protected:
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	struct PMDREntry {
		Ref<PropDataMeshData> mesh_data;
		Transform base_transform;
	};
#endif

	struct PTWEntry {
		Ref<PropDataTiledWall> data;
		Transform base_transform;
	};

	struct CollisionShapeEntry {
		Ref<Shape> shape;
		Transform transform;
		bool owns_shape;

		CollisionShapeEntry() {
			owns_shape = false;
		}
	};

	Ref<PropMaterialCache> _material_cache;

	Vector<Ref<PropMesherJobStep>> _job_steps;
	int _current_job_step;
	int _current_mesh;

	Ref<PropMesher> _prop_mesher;
	PropInstanceMerger *_prop_instace;

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	Vector<PMDREntry> _prop_mesh_datas;
#endif

	Vector<CollisionShapeEntry> _collision_shapes;

	Vector<PTWEntry> _prop_tiled_wall_datas;

	Array temp_mesh_arr;
};

#endif
