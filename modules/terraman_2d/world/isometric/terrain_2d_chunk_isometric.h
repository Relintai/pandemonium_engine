#ifndef TERRAIN_2D_CHUNK_ISOMETRIC_H
#define TERRAIN_2D_CHUNK_ISOMETRIC_H


#include "../default/terrain_2d_chunk_default.h"

class Terrain2DChunkIsometric : public Terrain2DChunkDefault {
	GDCLASS(Terrain2DChunkIsometric, Terrain2DChunkDefault);

public:
	Terrain2DChunkIsometric();
	~Terrain2DChunkIsometric();

protected:
	virtual void _setup_channels();

	static void _bind_methods();
};

#endif
