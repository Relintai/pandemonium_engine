#ifndef TERRAIN_CHUNK_BLOCKY_H
#define TERRAIN_CHUNK_BLOCKY_H


#include "../default/terrain_chunk_default.h"

class TerrainChunkBlocky : public TerrainChunkDefault {
	GDCLASS(TerrainChunkBlocky, TerrainChunkDefault);

public:
	TerrainChunkBlocky();
	~TerrainChunkBlocky();

protected:
	virtual void _setup_channels();

	static void _bind_methods();
};

#endif
