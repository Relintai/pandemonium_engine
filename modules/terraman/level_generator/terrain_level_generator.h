#ifndef TERRAIN_LEVEL_GENERATOR_H
#define TERRAIN_LEVEL_GENERATOR_H


#include "core/object/resource.h"

class TerrainChunk;

class TerrainLevelGenerator : public Resource {
	GDCLASS(TerrainLevelGenerator, Resource);

public:
	void generate_chunk(Ref<TerrainChunk> chunk);

	TerrainLevelGenerator();
	~TerrainLevelGenerator();

protected:
	static void _bind_methods();
};

#endif
