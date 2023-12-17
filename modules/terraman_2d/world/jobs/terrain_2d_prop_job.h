#ifndef TERRAIN_2D_PROP_JOB_H
#define TERRAIN_2D_PROP_JOB_H


#include "terrain_2d_job.h"

class Terrain2DMesher;

class Terrain2DProp2DJob : public Terrain2DJob {
	GDCLASS(Terrain2DProp2DJob, Terrain2DJob);

public:
	Ref<Terrain2DMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<Terrain2DMesher> &mesher);

	void phase_physics_process();
	void phase_prop();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup();

	void phase_steps();

	void step_type_normal();

	Terrain2DProp2DJob();
	~Terrain2DProp2DJob();

protected:
	static void _bind_methods();

	Ref<Terrain2DMesher> _prop_mesher;

	PoolVector<Vector2> temp_arr_collider;
	Array temp_mesh_arr;
};

#endif
