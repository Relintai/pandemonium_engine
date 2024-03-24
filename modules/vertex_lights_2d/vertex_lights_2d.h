#ifndef VERTEX_LIGHTS_2D_H
#define VERTEX_LIGHTS_2D_H

/*************************************************************************/
/*  vertex_lights_2d.h                                                   */
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

#include "core/object/object.h"

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2i.h"

class VertexLightMap2D;
class VertexLightQuadrant2D;
class VertexLightData2D;

class VertexLights2D : public Object {
	GDCLASS(VertexLights2D, Object);

public:
	enum VertexLight2DMode {
		VERTEX_LIGHT_2D_MODE_ADD = 0,
		VERTEX_LIGHT_2D_MODE_SUB,
		VERTEX_LIGHT_2D_MODE_MIX,
		//VERTEX_LIGHT_2D_MODE_MASK
	};

	// Defaults
	
	Vector2i get_default_quadrant_size() const;
	void set_default_quadrant_size(const Vector2i &p_size);

	// Maps
	
	RID map_create();

	Vector2i map_get_quadrant_size(RID p_map) const;
	void map_set_quadrant_size(RID p_map, const Vector2i &p_size);

	Array map_get_lights(RID p_map) const;
	
	void map_clear(RID p_map);

	// Lights
	
	RID light_create();

	RID light_get_map(RID p_light);
	void light_set_map(RID p_light, RID p_map);

	Vector2 light_get_position(RID p_light);
	void light_set_position(RID p_light, const Vector2 &p_position);

	Color light_get_color(RID p_light);
	void light_set_color(RID p_light, const Color &p_color);

	VertexLights2D::VertexLight2DMode light_get_mode(RID p_light);
	void light_set_mode(RID p_light, const VertexLights2D::VertexLight2DMode p_mode);

	Vector2i light_get_z_range(RID p_light);
	void light_set_z_range(RID p_light, const Vector2i &p_z_range);

	Vector2i light_get_layer_range(RID p_light);
	void light_set_layer_range(RID p_light, const Vector2i &p_layer_range);

	int light_get_item_cull_mask(RID p_light);
	void light_set_item_cull_mask(RID p_light, const int p_item_cull_mask);

	// Rest

	void free(RID p_rid);

	_FORCE_INLINE_ static VertexLights2D *get_singleton() {
		return _self;
	}

	VertexLights2D();
	~VertexLights2D();

protected:
	static void _bind_methods();

	mutable RID_Owner<VertexLightMap2D> map_owner;
	mutable RID_Owner<VertexLightData2D> light_owner;

	Vector2i _default_quadrant_size;

	static VertexLights2D *_self;
};

VARIANT_ENUM_CAST(VertexLights2D::VertexLight2DMode);

#endif
