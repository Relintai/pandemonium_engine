#ifndef TERRAIN_PROP_JOB_H
#define TERRAIN_PROP_JOB_H


#include "terrain_job.h"

#include "terrain_mesher_job_step.h"

class TerrainMesher;

class TerrainPropJob : public TerrainJob {
	GDCLASS(TerrainPropJob, TerrainJob);

public:
	Ref<TerrainMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<TerrainMesher> &mesher);

	Ref<TerrainMesherJobStep> get_jobs_step(const int index) const;
	void set_jobs_step(const int index, const Ref<TerrainMesherJobStep> &step);
	void remove_jobs_step(const int index);
	void add_jobs_step(const Ref<TerrainMesherJobStep> &step);
	int get_jobs_step_count() const;

	void phase_physics_process();
	void phase_prop();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup();

	void phase_steps();

	void step_type_normal();
	void step_type_normal_lod();
	void step_type_drop_uv2();
	void step_type_merge_verts();
	void step_type_bake_texture();
	void step_type_simplify_mesh();

	TerrainPropJob();
	~TerrainPropJob();

protected:
	static void _bind_methods();

	Ref<TerrainMesher> _prop_mesher;

	Vector<Ref<TerrainMesherJobStep>> _job_steps;
	int _current_job_step;
	int _current_mesh;

	PoolVector<Vector3> temp_arr_collider;
	Array temp_mesh_arr;
};

#endif
