#ifndef TERRAIN_2D_TERRAIN_2D_JOB_H
#define TERRAIN_2D_TERRAIN_2D_JOB_H


#include "terrain_2d_job.h"

#include "../../defines.h"

#include "core/containers/pool_vector.h"

class Terrain2DMesher;

class Terrain2DTerrain2DJob : public Terrain2DJob {
	GDCLASS(Terrain2DTerrain2DJob, Terrain2DJob);

public:
	Ref<Terrain2DMesher> get_mesher() const;
	void set_mesher(const Ref<Terrain2DMesher> &mesher);

	Ref<Terrain2DMesher> get_liquid_mesher() const;
	void set_liquid_mesher(const Ref<Terrain2DMesher> &mesher);

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

	Terrain2DTerrain2DJob();
	~Terrain2DTerrain2DJob();

protected:
	static void _bind_methods();

	Ref<Terrain2DMesher> _mesher;
	Ref<Terrain2DMesher> _liquid_mesher;

	PoolVector<Vector2> temp_arr_collider;
	PoolVector<Vector2> temp_arr_collider_liquid;
	Array temp_mesh_arr;
};

#endif
