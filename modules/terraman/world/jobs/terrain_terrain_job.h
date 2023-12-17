#ifndef TERRAIN_TERRAIN_JOB_H
#define TERRAIN_TERRAIN_JOB_H


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

	Ref<TerrainMesherJobStep> get_jobs_step(const int index) const;
	void set_jobs_step(const int index, const Ref<TerrainMesherJobStep> &step);
	void remove_jobs_step(const int index);
	void add_jobs_step(const Ref<TerrainMesherJobStep> &step);
	int get_jobs_step_count() const;

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
