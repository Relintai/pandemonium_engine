#ifndef TERRAIN_STRUCTURE_H
#define TERRAIN_STRUCTURE_H


#include "core/containers/hash_map.h"
#include "core/object/resource.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"

#include "core/math/aabb.h"
#include "terrain_chunk.h"

class TerrainStructure : public Resource {
	GDCLASS(TerrainStructure, Resource);

public:
	bool get_use_aabb() const;
	void set_use_aabb(const bool value);

	AABB get_chunk_aabb() const;
	void set_chunk_aabb(const AABB &value);

	int get_position_x() const;
	void set_position_x(const int value);

	int get_position_y() const;
	void set_position_y(const int value);

	int get_position_z() const;
	void set_position_z(const int value);

	void set_position(const int x, const int y, const int z);

	void write_to_chunk(Ref<TerrainChunk> chunk);

	TerrainStructure();
	~TerrainStructure();

protected:
	static void _bind_methods();

private:
	bool _use_aabb;
	AABB _chunk_aabb;

	int _position_x;
	int _position_y;
	int _position_z;
};

#endif
