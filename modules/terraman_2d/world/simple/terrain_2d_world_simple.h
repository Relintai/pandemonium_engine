#ifndef TERRAIN_2D_WORLD_BLOCKY_H
#define TERRAIN_2D_WORLD_BLOCKY_H


#include "../default/terrain_2d_world_default.h"

class Terrain2DWorldSimple : public Terrain2DWorldDefault {
	GDCLASS(Terrain2DWorldSimple, Terrain2DWorldDefault);

public:
	Terrain2DWorldSimple();
	~Terrain2DWorldSimple();

protected:
	Ref<Terrain2DChunk> _create_chunk(int x, int y, Ref<Terrain2DChunk> p_chunk);

	static void _bind_methods();
};

#endif
