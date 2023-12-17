#ifndef TERRAIN_2D_CHUNK_SIMPLE_H
#define TERRAIN_2D_CHUNK_SIMPLE_H


#include "../default/terrain_2d_chunk_default.h"

class Terrain2DChunkSimple : public Terrain2DChunkDefault {
	GDCLASS(Terrain2DChunkSimple, Terrain2DChunkDefault);

public:
	Terrain2DChunkSimple();
	~Terrain2DChunkSimple();

protected:
	virtual void _setup_channels();

	static void _bind_methods();
};

#endif
