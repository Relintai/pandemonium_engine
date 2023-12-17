#ifndef TERRAIN_SURFACE_H
#define TERRAIN_SURFACE_H


#include "core/math/color.h"
#include "core/object/resource.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

#include "terrain_library.h"

class TerrainLibrary;

class TerrainSurface : public Resource {
	GDCLASS(TerrainSurface, Resource)

public:
	/*
	   _____
	 /_____/I
	I     I I  <- BACK
	I  F  I I
	I_____I/


		I y
		I        I
		O----- x I B
	  /  I F  I /
	/ z+ I ---I/
	*/

	enum TerrainSurfaceSides {
		TERRAIN_SIDE_TOP = 0,
		TERRAIN_SIDE_BOTTOM = 1,
		TERRAIN_SIDE_SIDE = 2,
	};

	enum {
		TERRAIN_SIDES_COUNT = 3,
		TERRAIN_SIDES_ARRAY_SIZE = TERRAIN_SIDES_COUNT * 2,
	};

	int get_id() const;
	void set_id(const int value);

	Rect2 get_rect(const TerrainSurfaceSides side) const;
	void set_rect(const TerrainSurfaceSides side, const Rect2 &rect);

	Ref<TerrainLibrary> get_library() const;
	void set_library(Ref<TerrainLibrary> library);

	Vector2 transform_uv(const TerrainSurfaceSides p_side, const Vector2 &p_uv) const;
	Vector2 transform_uv_scaled(const TerrainSurfaceSides p_side, const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const;

	virtual void refresh_rects();

	TerrainSurface();
	~TerrainSurface();

protected:
	static void _bind_methods();

	TerrainLibrary *_library;

	int _id;
	int _mesher_index;
	bool _transparent;
	bool _liquid;
	Rect2 _rects[TERRAIN_SIDES_COUNT];
};

VARIANT_ENUM_CAST(TerrainSurface::TerrainSurfaceSides);

#endif
