#ifndef TERRAIN_2D_SURFACE_H
#define TERRAIN_2D_SURFACE_H

/*************************************************************************/
/*  terrain_2d_surface.h                                                 */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/object/resource.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

#include "terrain_2d_library.h"

class Terrain2DLibrary;

class Terrain2DSurface : public Resource {
	GDCLASS(Terrain2DSurface, Resource)

public:
	int get_id() const;
	void set_id(const int value);

	Rect2 get_rect() const;
	void set_rect(const Rect2 &rect);

	Ref<Terrain2DLibrary> get_library() const;
	void set_library(Ref<Terrain2DLibrary> library);

	Vector2 transform_uv(const Vector2 &p_uv) const;
	Vector2 transform_uv_scaled(const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const;

	virtual void refresh_rects();

	Terrain2DSurface();
	~Terrain2DSurface();

protected:
	static void _bind_methods();

	Terrain2DLibrary *_library;

	int _id;
	int _mesher_index;
	bool _transparent;
	bool _liquid;
	Rect2 _rect;
};

#endif
