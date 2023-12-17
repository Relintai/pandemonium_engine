#ifndef TERRAIN_LEVEL_GENERATOR_FLAT_H
#define TERRAIN_LEVEL_GENERATOR_FLAT_H


#include "terrain_level_generator.h"

class TerrainChunk;

class TerrainLevelGeneratorFlat : public TerrainLevelGenerator {
	GDCLASS(TerrainLevelGeneratorFlat, TerrainLevelGenerator);

public:
	int get_floor_position() const;
	void set_floor_position(const int floor_height);

	Dictionary get_channel_map();
	void set_channel_map(const Dictionary &map);

	virtual void _generate_chunk(Ref<TerrainChunk> chunk);

	TerrainLevelGeneratorFlat();
	~TerrainLevelGeneratorFlat();

protected:
	static void _bind_methods();

private:
	int _floor_position;
	Dictionary _channel_map;
};

#endif
