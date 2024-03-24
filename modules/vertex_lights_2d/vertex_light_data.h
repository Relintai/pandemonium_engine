#ifndef VERTEX_LIGHT_DATA_H
#define VERTEX_LIGHT_DATA_H

/*************************************************************************/
/*  vertex_light_data.h                                                  */
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

#include "core/containers/hash_map.h"
#include "core/containers/rid.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2i.h"

#include "vertex_lights_2d.h"

class VertexLightMap2D;
class VertexLightQuadrant2D;
class VertexLightData2D;

class VertexLightData2D : public RID_Data {
public:
	Vector2 position;
	Color color;
	VertexLights2D::VertexLight2DMode mode;
	Vector2i z_range;
	Vector2i layer_range;
	int item_cull_mask;
	
	VertexLightMap2D *map;
	VertexLightQuadrant2D *quadrant;

	VertexLightData2D() {
		item_cull_mask = 1;
		map = NULL;
		quadrant = NULL;
	}
};

class VertexLightQuadrant2D : public RID_Data {
public:
	LocalVector<VertexLightData2D *> lights;
	
	VertexLightMap2D *map;
	
	VertexLightQuadrant2D() {
		map = NULL;
	}
};

class VertexLightMap2D : public RID_Data {
public:
	HashMap<Vector2i, VertexLightQuadrant2D *> quadrants;
	Vector2i quadrant_size;
};

#endif
