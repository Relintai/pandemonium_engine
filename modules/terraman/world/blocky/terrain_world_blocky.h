#ifndef TERRAIN_WORLD_BLOCKY_H
#define TERRAIN_WORLD_BLOCKY_H


#include "../default/terrain_world_default.h"

class TerrainWorldBlocky : public TerrainWorldDefault {
	GDCLASS(TerrainWorldBlocky, TerrainWorldDefault);

public:
	TerrainWorldBlocky();
	~TerrainWorldBlocky();

protected:
	Ref<TerrainChunk> _create_chunk(int x, int z, Ref<TerrainChunk> p_chunk);

	static void _bind_methods();
};

#endif
