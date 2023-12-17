#ifndef TERRAIN_2D_LEVEL_GENERATOR_FLAT_H
#define TERRAIN_2D_LEVEL_GENERATOR_FLAT_H


#include "terrain_2d_level_generator.h"

class Terrain2DChunk;

class Terrain2DLevelGeneratorFlat : public Terrain2DLevelGenerator {
	GDCLASS(Terrain2DLevelGeneratorFlat, Terrain2DLevelGenerator);

public:
	int get_floor_position() const;
	void set_floor_position(const int floor_height);

	Dictionary get_channel_map();
	void set_channel_map(const Dictionary &map);

	virtual void _generate_chunk(Ref<Terrain2DChunk> chunk);

	Terrain2DLevelGeneratorFlat();
	~Terrain2DLevelGeneratorFlat();

protected:
	static void _bind_methods();

private:
	int _floor_position;
	Dictionary _channel_map;
};

#endif
