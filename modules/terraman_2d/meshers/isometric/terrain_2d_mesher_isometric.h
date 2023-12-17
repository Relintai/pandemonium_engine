#ifndef TERRAIN_2D_MESHER_ISOMETRIC_H
#define TERRAIN_2D_MESHER_ISOMETRIC_H


#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../../world/default/terrain_2d_chunk_default.h"
#include "../default/terrain_2d_mesher_default.h"

class Terrain2DMesherIsometric : public Terrain2DMesherDefault {
	GDCLASS(Terrain2DMesherIsometric, Terrain2DMesherDefault);

public:
	void _add_chunk(Ref<Terrain2DChunk> p_chunk);

	void mesh_base(Ref<Terrain2DChunkDefault> chunk);
	void mesh_walls(Ref<Terrain2DChunkDefault> chunk);

	Ref<Shape2D> create_terrain_tile_collider_shape(Ref<Terrain2DChunk> chunk);

	Terrain2DMesherIsometric();
	~Terrain2DMesherIsometric();

protected:
	static void _bind_methods();
};

#endif
