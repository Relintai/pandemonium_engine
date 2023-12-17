#ifndef BLOCK_TERRAIN_2D_STRUCTURE_H
#define BLOCK_TERRAIN_2D_STRUCTURE_H


#include "core/containers/vector.h"

#include "terrain_2d_structure.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"

#include "terrain_2d_chunk.h"

class BlockTerrain2DStructure : public Terrain2DStructure {
	GDCLASS(BlockTerrain2DStructure, Terrain2DStructure);

public:
	int get_channel_type() const;
	void set_channel_type(const int value);

	int get_voxel_type(int p_x, int p_y) const;

	void set_voxel(int p_x, int p_y, int p_type, int p_isolevel);

	void _write_to_chunk(Ref<Terrain2DChunk> chunk);

	void clear();

	BlockTerrain2DStructure();
	~BlockTerrain2DStructure();

protected:
	static void _bind_methods();

public:
	struct DataEntry {
		int x;
		int y;
		int data_type;
		int data_isolevel;
	};

private:
	int _channel_type;
	int _channel_isolevel;

	Vector<DataEntry> _data;
};

#endif
