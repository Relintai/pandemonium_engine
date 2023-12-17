#ifndef TERRAIN_2D_MESHER_SIMPLE_H
#define TERRAIN_2D_MESHER_SIMPLE_H


#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../../world/default/terrain_2d_chunk_default.h"
#include "../default/terrain_2d_mesher_default.h"

class Terrain2DMesherSimple : public Terrain2DMesherDefault {
	GDCLASS(Terrain2DMesherSimple, Terrain2DMesherDefault);

public:
	void _add_chunk(Ref<Terrain2DChunk> p_chunk);

	void add_chunk_normal(Ref<Terrain2DChunkDefault> chunk);

	Terrain2DMesherSimple();
	~Terrain2DMesherSimple();

protected:
	static void _bind_methods();
};

#endif
