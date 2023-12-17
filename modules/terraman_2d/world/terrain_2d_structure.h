#ifndef TERRAIN_2D_STRUCTURE_H
#define TERRAIN_2D_STRUCTURE_H


#include "core/containers/hash_map.h"
#include "core/object/resource.h"

#include "../defines.h"

#include "core/math/rect2.h"
#include "core/containers/pool_vector.h"
#include "terrain_2d_chunk.h"

class Terrain2DStructure : public Resource {
	GDCLASS(Terrain2DStructure, Resource);

public:
	bool get_use_rect() const;
	void set_use_rect(const bool value);

	Rect2 get_chunk_rect() const;
	void set_chunk_rect(const Rect2 &value);

	int get_position_x() const;
	void set_position_x(const int value);

	int get_position_y() const;
	void set_position_y(const int value);

	void set_position(const int x, const int y);

	void write_to_chunk(Ref<Terrain2DChunk> chunk);

	Terrain2DStructure();
	~Terrain2DStructure();

protected:
	static void _bind_methods();

private:
	bool _use_rect;
	Rect2 _chunk_rect;

	int _position_x;
	int _position_y;
};

#endif
