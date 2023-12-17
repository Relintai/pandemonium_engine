#ifndef TERRAIN_LIGHT_H
#define TERRAIN_LIGHT_H


#include "core/math/color.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

class TerrainLight : public Reference {
	GDCLASS(TerrainLight, Reference);

public:
	int get_world_position_x() const;
	int get_world_position_y() const;
	int get_world_position_z() const;
	Vector3 get_world_position();
	void set_world_position(const int x, const int y, const int z);

	Color get_color() const;
	void set_color(const Color &color);

	float get_size() const;
	void set_size(const float strength);

	TerrainLight();
	~TerrainLight();

private:
	static void _bind_methods();

private:
	int _chunk_position_x;
	int _chunk_position_y;
	int _chunk_position_z;

	int _world_position_x;
	int _world_position_y;
	int _world_position_z;

	Color _color;
	int _size;
};

#endif
