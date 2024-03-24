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

#include "core/math/transform.h"

class VertexLights2D : public Object {
	GDCLASS(VertexLights2D, Object);

public:
	enum VertexLight2DMode {
		VERTEX_LIGHT_2D_MODE_ADD = 0,
		VERTEX_LIGHT_2D_MODE_SUB,
		VERTEX_LIGHT_2D_MODE_MIX,
		//VERTEX_LIGHT_2D_MODE_MASK
	};

	/*
	Transform get_transform() const;
	void set_transform(const Transform &p_transform);

	real_t get_range() const;
	void set_range(const real_t value);

	real_t get_attenuation() const;
	void set_attenuation(const real_t value);

	Color get_color() const;
	void set_color(const Color value);

	real_t get_energy() const;
	void set_energy(const real_t value);

	real_t get_indirect_energy() const;
	void set_indirect_energy(const real_t value);

	bool get_negative() const;
	void set_negative(const bool value);

	real_t get_specular() const;
	void set_specular(const real_t value);
	*/

	_FORCE_INLINE_ static VertexLights2D *get_singleton() {
		return _self;
	}

	VertexLights2D();
	~VertexLights2D();

protected:
	static void _bind_methods();

	struct VertexLightData2D {
		Vector2 position;
		Color color;
		VertexLight2DMode mode;
		Vector2i z_range;
		Vector2i layer_range;
		int item_cull_mask;

		VertexLightData2D() {
			item_cull_mask = 1;
		}
	};

	struct VertexLightQuadrant2D {
		HashMap<RID, VertexLightData2D*> lights;
		Vector2i size;
	};

	struct VertexLightMap2D {
		HashMap<Vector2i, VertexLightQuadrant2D*> quadrants;
	};

	HashMap<RID, VertexLightMap2D*> _maps;
	Vector2i _default_quadrant_size;

	static VertexLights2D *_self;
};

VARIANT_ENUM_CAST(VertexLights2D::VertexLight2DMode);

#endif
