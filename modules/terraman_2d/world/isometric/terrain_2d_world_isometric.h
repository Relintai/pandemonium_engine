#ifndef TERRAIN_2D_WORLD_ISOMETRIC_H
#define TERRAIN_2D_WORLD_ISOMETRIC_H


#include "../default/terrain_2d_world_default.h"

class Terrain2DWorldIsometric : public Terrain2DWorldDefault {
	GDCLASS(Terrain2DWorldIsometric, Terrain2DWorldDefault);

public:
	Terrain2DWorldIsometric();
	~Terrain2DWorldIsometric();

protected:
	Ref<Terrain2DChunk> _create_chunk(int x, int y, Ref<Terrain2DChunk> p_chunk);

	static void _bind_methods();
};

#endif
