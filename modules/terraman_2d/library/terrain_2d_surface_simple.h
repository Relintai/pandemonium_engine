#ifndef TERRAIN_2D_SURFACE_SIMPLE_H
#define TERRAIN_2D_SURFACE_SIMPLE_H


#include "terrain_2d_surface.h"

class Terrain2DSurfaceSimple : public Terrain2DSurface {
	GDCLASS(Terrain2DSurfaceSimple, Terrain2DSurface)

public:
	int get_atlas_x() const;
	void set_atlas_x(int value);

	int get_atlas_y() const;
	void set_atlas_y(int value);

	void refresh_rects();

	Terrain2DSurfaceSimple();
	~Terrain2DSurfaceSimple();

protected:
	static void _bind_methods();

private:
	int _atlas_position_x;
	int _atlas_position_y;
};

#endif
