#ifndef VERTEX_LIGHT_3D_SERVER_H
#define VERTEX_LIGHT_3D_SERVER_H

/*************************************************************************/
/*  vertex_lights_3d.h                                                   */
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
#include "core/math/vector3i.h"

class VertexLights3DServer : public Object {
	GDCLASS(VertexLights3DServer, Object);

public:
	enum VertexLight3DMode {
		VERTEX_LIGHT_3D_MODE_ADD = 0,
		VERTEX_LIGHT_3D_MODE_SUB,
		VERTEX_LIGHT_3D_MODE_MIX,
		//VERTEX_LIGHT_3D_MODE_MASK
	};

	// Defaults

	Vector3i get_default_quadrant_size() const;
	void set_default_quadrant_size(const Vector3i &p_size);

	// Maps

	RID map_create();

	Vector3i map_get_quadrant_size(RID p_map) const;
	void map_set_quadrant_size(RID p_map, const Vector3i &p_size);
	
	Color map_get_base_color(RID p_map) const;
	void map_set_base_color(RID p_map, const Color &p_base_color);

	Array map_get_lights(RID p_map) const;

	void map_clear(RID p_map);

	// Lights

	RID light_create();

	RID light_get_map(RID p_light);
	void light_set_map(RID p_light, RID p_map);

	bool light_get_is_enabled(RID p_light);
	void light_set_enabled(RID p_light, const bool p_enabled);

	Vector3 light_get_position(RID p_light);
	void light_set_position(RID p_light, const Vector3 &p_position);

	real_t light_get_range(RID p_light);
	void light_set_range(RID p_light, const real_t p_range);

	real_t light_get_attenuation(RID p_light);
	void light_set_attenuation(RID p_light, const real_t p_attenuation);

	Color light_get_color(RID p_light);
	void light_set_color(RID p_light, const Color &p_color);

	VertexLights3DServer::VertexLight3DMode light_get_mode(RID p_light);
	void light_set_mode(RID p_light, const VertexLights3DServer::VertexLight3DMode p_mode);

	int light_get_item_cull_mask(RID p_light);
	void light_set_item_cull_mask(RID p_light, const int p_item_cull_mask);

	// Sampling

	Color sample_light_value(RID p_map, const Vector3 &p_position, const int p_item_cull_mask = 1);
	Color sample_light(RID p_map, const Vector3 &p_position, const Vector3 &p_normal, const int p_item_cull_mask = 1);

	// Rest

	void free(RID p_rid);
	
	void init();
	void flush_notifications();

	_FORCE_INLINE_ static VertexLights3DServer *get_singleton() {
		return _self;
	}

	VertexLights3DServer();
	~VertexLights3DServer();

protected:
	void register_update();
	void unregister_update();

	static void _bind_methods();

	class VertexLightMap3D;
	class VertexLightQuadrant3D;
	class VertexLightData3D;

	class VertexLightData3D : public RID_Data {
	public:
		bool enabled;
		Vector3 position;
		real_t range;
		real_t attenuation;
		Color color;
		VertexLights3DServer::VertexLight3DMode mode;
		int item_cull_mask;

		VertexLightMap3D *map;
		VertexLightQuadrant3D *quadrant;

		RID self;

		VertexLightData3D() {
			map = NULL;
			quadrant = NULL;

			enabled = true;
			range = 5;
			attenuation = 1;
			color = Color(1, 1, 1, 1);
			item_cull_mask = 1;
			mode = VertexLights3DServer::VERTEX_LIGHT_3D_MODE_ADD;
		}
	};

	class VertexLightQuadrant3D {
	public:
		Vector3i position;
		LocalVector<VertexLightData3D *> lights;

		VertexLightMap3D *map;

		void get_lights(List<VertexLightData3D *> *p_lights);

		Color sample_light_value(const Color &p_current_color, const Vector3 &p_position, const int p_item_cull_mask);
		Color sample_light(const Color &p_current_color, const Vector3 &p_position, const Vector3 &p_normal, const int p_item_cull_mask);

		VertexLightQuadrant3D() {
			map = NULL;
		}
	};

	class VertexLightMap3D : public RID_Data {
	public:
		HashMap<Vector3i, VertexLightQuadrant3D *> quadrants;
		Vector3i quadrant_size;
		Color base_color;

		RID self;

		void recreate_quadrants();

		void get_lights(List<VertexLightData3D *> *p_lights);

		void add_light(VertexLightData3D *p_light);
		void remove_light(VertexLightData3D *p_light);

		VertexLightQuadrant3D *get_quadrant_for_position(const Vector3 &p_position);

		void set_light_position(VertexLightData3D *p_light, const Vector3 &p_position);

		void clear();

		Color sample_light_value(const Vector3 &p_position, const int p_item_cull_mask);
		Color sample_light(const Vector3 &p_position, const Vector3 &p_normal, const int p_item_cull_mask);

		_FORCE_INLINE_ Vector3i to_quadrant_position(const Vector3 &p_position) {
			return Vector3i(p_position.x / quadrant_size.x, p_position.y / quadrant_size.y, p_position.z / quadrant_size.z);
		}

		_FORCE_INLINE_ Vector3 to_position(const Vector3i &p_quadrant_position) {
			return Vector3(p_quadrant_position.x * quadrant_size.x, p_quadrant_position.y * quadrant_size.y, p_quadrant_position.z * quadrant_size.z);
		}
	};

	_FORCE_INLINE_ void _light_changed(const VertexLightData3D *p_light) const {
		if (p_light && p_light->enabled && p_light->map) {
			_changed_maps.insert(p_light->map->self);
		}
	}
	
	_FORCE_INLINE_ void _light_enabled_changed(const VertexLightData3D *p_light) const {
		if (p_light && p_light->map) {
			_changed_maps.insert(p_light->map->self);
		}
	}

	_FORCE_INLINE_ void _map_changed(const VertexLightMap3D *p_map) const {
		if (p_map) {
			_changed_maps.insert(p_map->self);
		}
	}

	mutable RID_Owner<VertexLightMap3D> map_owner;
	mutable RID_Owner<VertexLightData3D> light_owner;

	Vector3i _default_quadrant_size;

	// Maybe an api could be adde that's per quadrant
	mutable HashSet<RID> _changed_maps;
	StringName _map_changed_name;

	static VertexLights3DServer *_self;
};

VARIANT_ENUM_CAST(VertexLights3DServer::VertexLight3DMode);

#endif
