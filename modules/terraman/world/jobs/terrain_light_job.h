#ifndef TERRAIN_LIGHT_JOB_H
#define TERRAIN_LIGHT_JOB_H


#include "terrain_job.h"

class TerrainMesher;

class TerrainLightJob : public TerrainJob {
	GDCLASS(TerrainLightJob, TerrainJob);

public:
	void phase_light();

	void _execute_phase();

	TerrainLightJob();
	~TerrainLightJob();

protected:
	static void _bind_methods();
};

#endif
