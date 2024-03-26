#ifndef VERTEX_LIGHT_2D_SERVER_H
#define VERTEX_LIGHT_2D_SERVER_H

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
#include "core/containers/hash_set.h"
#include "core/containers/list.h"
#include "core/containers/rid.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2i.h"

class VertexLights2DServer : public Object {
	GDCLASS(VertexLights2DServer, Object);

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

	bool light_get_is_enabled(RID p_light);
	void light_set_enabled(RID p_light, const bool p_enabled);

	Vector2 light_get_position(RID p_light);
	void light_set_position(RID p_light, const Vector2 &p_position);

	Vector2i light_get_range(RID p_light);
	void light_set_range(RID p_light, const Vector2i &p_range);

	real_t light_get_attenuation(RID p_light);
	void light_set_attenuation(RID p_light, const real_t p_attenuation);

	Color light_get_color(RID p_light);
	void light_set_color(RID p_light, const Color &p_color);

	VertexLights2DServer::VertexLight2DMode light_get_mode(RID p_light);
	void light_set_mode(RID p_light, const VertexLights2DServer::VertexLight2DMode p_mode);

	Vector2i light_get_z_range(RID p_light);
	void light_set_z_range(RID p_light, const Vector2i &p_z_range);

	Vector2i light_get_layer_range(RID p_light);
	void light_set_layer_range(RID p_light, const Vector2i &p_layer_range);

	int light_get_item_cull_mask(RID p_light);
	void light_set_item_cull_mask(RID p_light, const int p_item_cull_mask);

	// Sampling

	Color sample_light(RID p_map, const Vector2 &p_position, const int p_item_cull_mask = 1, const int p_layer = 0);

	// Rest

	void free(RID p_rid);

	void init();
	void finalize();
	void flush_notifications();

	_FORCE_INLINE_ static VertexLights2DServer *get_singleton() {
		return _self;
	}

	VertexLights2DServer();
	~VertexLights2DServer();

protected:
	static void _scene_tree_idle_callback() {
		VertexLights2DServer::get_singleton()->flush_notifications();
	}

	static void _bind_methods();

	class VertexLightMap2D;
	class VertexLightQuadrant2D;
	class VertexLightData2D;

	class VertexLightData2D : public RID_Data {
	public:
		bool enabled;
		Vector2 position;
		Vector2i range;
		real_t attenuation;
		Color color;
		VertexLights2DServer::VertexLight2DMode mode;
		Vector2i z_range;
		Vector2i layer_range;
		int item_cull_mask;

		VertexLightMap2D *map;
		VertexLightQuadrant2D *quadrant;

		RID self;

		VertexLightData2D() {
			map = NULL;
			quadrant = NULL;

			enabled = true;
			range = Vector2i(32, 32);
			attenuation = 1;
			color = Color(1, 1, 1, 1);
			item_cull_mask = 1;
			z_range = Vector2i(-1024, 1024);
			mode = VertexLights2DServer::VERTEX_LIGHT_2D_MODE_ADD;
		}
	};

	class VertexLightQuadrant2D {
	public:
		Vector2i position;
		LocalVector<VertexLightData2D *> lights;

		VertexLightMap2D *map;

		void get_lights(List<VertexLightData2D *> *p_lights);

		Color sample_light(const Color &p_current_color, const Vector2 &p_local_position, const int p_item_cull_mask, const int p_layer);

		VertexLightQuadrant2D() {
			map = NULL;
		}
	};

	class VertexLightMap2D : public RID_Data {
	public:
		HashMap<Vector2i, VertexLightQuadrant2D *> quadrants;
		Vector2i quadrant_size;

		RID self;

		void recreate_quadrants();

		void get_lights(List<VertexLightData2D *> *p_lights);

		void add_light(VertexLightData2D *p_light);
		void remove_light(VertexLightData2D *p_light);

		VertexLightQuadrant2D *get_quadrant_for_position(const Vector2 &p_position);

		void set_light_position(VertexLightData2D *p_light, const Vector2 &p_position);

		void clear();

		Color sample_light(const Vector2 &p_position, const int p_item_cull_mask = 1, const int p_layer = 0);

		_FORCE_INLINE_ Vector2i to_quadrant_position(const Vector2 &p_position) {
			return Vector2i(p_position.x / quadrant_size.x, p_position.y / quadrant_size.y);
		}

		_FORCE_INLINE_ Vector2 to_position(const Vector2i &p_quadrant_position) {
			return Vector2(p_quadrant_position.x * quadrant_size.x, p_quadrant_position.y * quadrant_size.y);
		}
	};

	_FORCE_INLINE_ void _light_changed(const VertexLightData2D *p_light) const {
		if (p_light && p_light->map) {
			_changed_maps.insert(p_light->map->self);
		}
	}

	_FORCE_INLINE_ void _map_changed(const VertexLightMap2D *p_map) const {
		if (p_map) {
			_changed_maps.insert(p_map->self);
		}
	}

	mutable RID_Owner<VertexLightMap2D> map_owner;
	mutable RID_Owner<VertexLightData2D> light_owner;

	Vector2i _default_quadrant_size;

	// Maybe an api could be adde that's per quadrant
	mutable HashSet<RID> _changed_maps;
	StringName _map_changed_name;

	static VertexLights2DServer *_self;
};

VARIANT_ENUM_CAST(VertexLights2DServer::VertexLight2DMode);

#endif
