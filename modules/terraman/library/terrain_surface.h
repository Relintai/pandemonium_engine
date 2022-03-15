/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TERRAIN_SURFACE_H
#define TERRAIN_SURFACE_H


#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/templates/vector.h"
#include "core/math/color.h"
#else
#include "core/resource.h"
#include "core/vector.h"
#include "core/color.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/material.h"

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
