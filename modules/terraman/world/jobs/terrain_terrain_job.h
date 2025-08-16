#ifndef TERRAIN_TERRAIN_JOB_H
#define TERRAIN_TERRAIN_JOB_H

/*************************************************************************/
/*  terrain_terrain_job.h                                                */
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

#include "terrain_job.h"

#include "terrain_mesher_job_step.h"

#include "../../defines.h"

#include "core/containers/pool_vector.h"

class TerrainMesher;

class TerrainTerrainJob : public TerrainJob {
	GDCLASS(TerrainTerrainJob, TerrainJob);

public:
	Ref<TerrainMesher> get_mesher() const;
	void set_mesher(const Ref<TerrainMesher> &mesher);

	Ref<TerrainMesher> get_liquid_mesher() const;
	void set_liquid_mesher(const Ref<TerrainMesher> &mesher);

	Ref<TerrainMesherJobStep> get_job_step(const int index) const;
	void set_job_step(const int index, const Ref<TerrainMesherJobStep> &step);
	void remove_job_step(const int index);
	void add_job_step(const Ref<TerrainMesherJobStep> &step);
	int get_job_step_count() const;

	void phase_setup();
	void phase_library_setup();
	void phase_terrain_mesh_setup();
	void phase_collider();
	void phase_physics_proces();
	void phase_terrain_mesh();
	void phase_finalize();
	void phase_physics_process();

	void _execute_phase();
	void _reset();
	void _physics_process(float delta);

	void step_type_normal();
	void step_type_normal_lod();
	void step_type_drop_uv2();
	void step_type_merge_verts();
	void step_type_bake_texture();
	void step_type_simplify_mesh();

	TerrainTerrainJob();
	~TerrainTerrainJob();

protected:
	static void _bind_methods();

	Ref<TerrainMesher> _mesher;
	Ref<TerrainMesher> _liquid_mesher;

	Vector<Ref<TerrainMesherJobStep>> _job_steps;
	int _current_job_step;
	int _current_mesh;

	PoolVector<Vector3> temp_arr_collider;
	PoolVector<Vector3> temp_arr_collider_liquid;
	Array temp_mesh_arr;
};

#endif
