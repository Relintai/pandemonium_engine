#ifndef TERRAIN_2D_LEVEL_GENERATOR_H
#define TERRAIN_2D_LEVEL_GENERATOR_H


#include "core/object/resource.h"

class Terrain2DChunk;

class Terrain2DLevelGenerator : public Resource {
	GDCLASS(Terrain2DLevelGenerator, Resource);

public:
	void generate_chunk(Ref<Terrain2DChunk> chunk);

	Terrain2DLevelGenerator();
	~Terrain2DLevelGenerator();

protected:
	static void _bind_methods();
};

#endif
