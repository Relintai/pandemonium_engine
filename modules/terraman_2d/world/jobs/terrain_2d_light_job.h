#ifndef TERRAIN_2D_LIGHT_JOB_H
#define TERRAIN_2D_LIGHT_JOB_H


#include "terrain_2d_job.h"

class Terrain2DMesher;

class Terrain2DLightJob : public Terrain2DJob {
	GDCLASS(Terrain2DLightJob, Terrain2DJob);

public:
	int get_rao_octaves();
	void set_rao_octaves(const int val);

	int get_rao_period();
	void set_rao_period(const int val);

	float get_rao_persistence();
	void set_rao_persistence(const float val);

	float get_rao_scale_factor();
	void set_rao_scale_factor(const float val);

	void phase_light();

	void _execute_phase();

	Terrain2DLightJob();
	~Terrain2DLightJob();

protected:
	static void _bind_methods();

	int _rao_octaves;
	int _rao_period;
	float _rao_persistence;
	float _rao_scale_factor;
};

#endif
