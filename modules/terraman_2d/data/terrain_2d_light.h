#ifndef TERRAIN_2D_LIGHT_H
#define TERRAIN_2D_LIGHT_H


#include "core/math/color.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

class Terrain2DLight : public Reference {
	GDCLASS(Terrain2DLight, Reference);

public:
	int get_world_position_x() const;
	int get_world_position_y() const;
	Vector2 get_world_position();
	void set_world_position(const int x, const int y);

	Color get_color() const;
	void set_color(const Color &color);

	float get_size() const;
	void set_size(const float strength);

	Terrain2DLight();
	~Terrain2DLight();

private:
	static void _bind_methods();

private:
	int _world_position_x;
	int _world_position_y;

	Color _color;
	int _size;
};

#endif
