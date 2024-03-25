#ifndef VERTEX_LIGHT_2D_H
#define VERTEX_LIGHT_2D_H

/*************************************************************************/
/*  vertex_2d.h                                                    */
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

#include "scene/main/node_2d.h"

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2i.h"

#include "vertex_lights_2d_server.h"

class VertexLight2D : public Node2D {
	GDCLASS(VertexLight2D, Node2D);

public:
	enum VertexLight2DMode {
		VERTEX_LIGHT_2D_MODE_ADD = VertexLights2DServer::VERTEX_LIGHT_2D_MODE_ADD,
		VERTEX_LIGHT_2D_MODE_SUB = VertexLights2DServer::VERTEX_LIGHT_2D_MODE_SUB,
		VERTEX_LIGHT_2D_MODE_MIX = VertexLights2DServer::VERTEX_LIGHT_2D_MODE_MIX,
		//VERTEX_LIGHT_2D_MODE_MASK = VertexLights2DServer::VERTEX_LIGHT_2D_MODE_MASK
	};

	bool get_is_enabled();
	void set_enabled(const bool p_enabled);

	Vector2i get_range();
	void set_range(const Vector2i &p_range);

	real_t get_attenuation();
	void set_attenuation(const real_t p_attenuation);

	Color get_color();
	void set_color(const Color &p_color);

	VertexLight2D::VertexLight2DMode get_mode();
	void set_mode(const VertexLight2D::VertexLight2DMode p_mode);

	Vector2i get_z_range();
	void set_z_range(const Vector2i &p_z_range);

	Vector2i get_layer_range();
	void set_layer_range(const Vector2i &p_layer_range);

	int get_item_cull_mask();
	void set_item_cull_mask(const int p_item_cull_mask);

	VertexLight2D();
	~VertexLight2D();

protected:
	void _notification(int p_what);

	void _update_light_visibility();

	static void _bind_methods();

	RID _vertex_light;

	bool _enabled;
	Vector2i _range;
	real_t _attenuation;
	Color _color;
	VertexLight2DMode _mode;
	Vector2i _z_range;
	Vector2i _layer_range;
	int _item_cull_mask;
};

VARIANT_ENUM_CAST(VertexLight2D::VertexLight2DMode);

#endif
