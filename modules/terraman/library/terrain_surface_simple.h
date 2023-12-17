#ifndef TERRAIN_SURFACE_SIMPLE_H
#define TERRAIN_SURFACE_SIMPLE_H


#include "terrain_surface.h"

class TerrainSurfaceSimple : public TerrainSurface {
	GDCLASS(TerrainSurfaceSimple, TerrainSurface)

public:
	int get_atlas_x(const TerrainSurfaceSides side) const;
	void set_atlas_x(const TerrainSurfaceSides side, int value);

	int get_atlas_y(const TerrainSurfaceSides side) const;
	void set_atlas_y(const TerrainSurfaceSides side, int value);

	void refresh_rects();

	TerrainSurfaceSimple();
	~TerrainSurfaceSimple();

protected:
	static void _bind_methods();

private:
	int _atlas_positions[TERRAIN_SIDES_ARRAY_SIZE];
};

#endif
