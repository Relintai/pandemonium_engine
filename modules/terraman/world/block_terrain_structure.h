#ifndef BLOCK_TERRAIN_STRUCTURE_H
#define BLOCK_TERRAIN_STRUCTURE_H


#include "core/containers/vector.h"

#include "terrain_structure.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"

#include "terrain_chunk.h"

class BlockTerrainStructure : public TerrainStructure {
	GDCLASS(BlockTerrainStructure, TerrainStructure);

public:
	int get_channel_type() const;
	void set_channel_type(const int value);

	int get_channel_isolevel() const;
	void set_channel_isolevel(const int value);

	int get_voxel_type(int p_x, int p_y, int p_z) const;
	int get_voxel_isolevel(int p_x, int p_y, int p_z) const;

	void set_voxel(int p_x, int p_y, int p_z, int p_type, int p_isolevel);

	void _write_to_chunk(Ref<TerrainChunk> chunk);

	void clear();

	BlockTerrainStructure();
	~BlockTerrainStructure();

protected:
	static void _bind_methods();

public:
	struct DataEntry {
		int x;
		int y;
		int z;
		int data_type;
		int data_isolevel;
	};

private:
	int _channel_type;
	int _channel_isolevel;

	Vector<DataEntry> _data;
};

#endif
