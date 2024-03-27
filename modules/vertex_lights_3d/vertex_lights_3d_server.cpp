/*************************************************************************/
/*  vertex_lights_3d.cpp                                                 */
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

#include "vertex_lights_3d_server.h"

#include "core/config/project_settings.h"

#include "scene/main/scene_tree.h"

// Defaults
Vector2i VertexLights3DServer::get_default_quadrant_size() const {
	return _default_quadrant_size;
}
void VertexLights3DServer::set_default_quadrant_size(const Vector2i &p_size) {
	_default_quadrant_size = p_size;
}

// Maps
RID VertexLights3DServer::map_create() {
	VertexLightMap3D *map = memnew(VertexLightMap3D);
	RID rid = map_owner.make_rid(map);
	map->self = rid;
	map->quadrant_size = _default_quadrant_size;
	return rid;
}

Vector2i VertexLights3DServer::map_get_quadrant_size(RID p_map) const {
	const VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Vector2i());

	return map->quadrant_size;
}
void VertexLights3DServer::map_set_quadrant_size(RID p_map, const Vector2i &p_size) {
	VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND(map == NULL);

	map->quadrant_size = p_size;
	map->recreate_quadrants();

	_map_changed(map);
}

Color VertexLights3DServer::map_get_base_color(RID p_map) const {
	const VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Color());

	return map->base_color;
}
void VertexLights3DServer::map_set_base_color(RID p_map, const Color &p_base_color) {
	VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND(map == NULL);

	map->base_color = p_base_color;

	_map_changed(map);
}

Array VertexLights3DServer::map_get_lights(RID p_map) const {
	VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Array());

	List<VertexLightData3D *> lights;
	map->get_lights(&lights);

	Array arr;

	arr.resize(lights.size());

	int i = 0;
	for (List<VertexLightData3D *>::Element *E = lights.front(); E; E = E->next()) {
		arr[i++] = E->get()->self;
	}

	return arr;
}

void VertexLights3DServer::map_clear(RID p_map) {
	VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND(map == NULL);

	map->clear();

	_map_changed(map);
}

// Lights
RID VertexLights3DServer::light_create() {
	VertexLightData3D *light = memnew(VertexLightData3D);
	RID rid = light_owner.make_rid(light);
	light->self = rid;
	return rid;
}

RID VertexLights3DServer::light_get_map(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, RID());

	if (!light->map) {
		return RID();
	}

	return light->map->self;
}
void VertexLights3DServer::light_set_map(RID p_light, RID p_map) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	VertexLightMap3D *map = map_owner.getornull(p_map);

	if (light->map) {
		_light_changed(light);
		light->map->remove_light(light);
	}

	if (map) {
		map->add_light(light);
		_light_changed(light);
	}
}

bool VertexLights3DServer::light_get_is_enabled(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, false);

	return light->enabled;
}
void VertexLights3DServer::light_set_enabled(RID p_light, const bool p_enabled) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	if (light->enabled == p_enabled) {
		return;
	}

	light->enabled = p_enabled;

	_light_enabled_changed(light);
}

Vector2 VertexLights3DServer::light_get_position(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2());

	return light->position;
}
void VertexLights3DServer::light_set_position(RID p_light, const Vector2 &p_position) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	if (light->map) {
		// This ensure the light gets moved to the proper quadrant
		light->map->set_light_position(light, p_position);
		_light_changed(light);
		return;
	}

	light->position = p_position;

	_light_changed(light);
}

Vector2i VertexLights3DServer::light_get_range(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2i());

	return light->range;
}
void VertexLights3DServer::light_set_range(RID p_light, const Vector2i &p_range) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->range = p_range;

	_light_changed(light);
}

real_t VertexLights3DServer::light_get_attenuation(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, 0);

	return light->attenuation;
}
void VertexLights3DServer::light_set_attenuation(RID p_light, const real_t p_attenuation) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->attenuation = p_attenuation;

	_light_changed(light);
}

Color VertexLights3DServer::light_get_color(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Color());

	return light->color;
}
void VertexLights3DServer::light_set_color(RID p_light, const Color &p_color) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->color = p_color;

	_light_changed(light);
}

VertexLights3DServer::VertexLight3DMode VertexLights3DServer::light_get_mode(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, VertexLights3DServer::VERTEX_LIGHT_3D_MODE_ADD);

	return light->mode;
}
void VertexLights3DServer::light_set_mode(RID p_light, const VertexLights3DServer::VertexLight3DMode p_mode) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->mode = p_mode;

	_light_changed(light);
}

Vector2i VertexLights3DServer::light_get_z_range(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2i());

	return light->z_range;
}
void VertexLights3DServer::light_set_z_range(RID p_light, const Vector2i &p_z_range) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->z_range = p_z_range;

	_light_changed(light);
}

Vector2i VertexLights3DServer::light_get_layer_range(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2i());

	return light->layer_range;
}
void VertexLights3DServer::light_set_layer_range(RID p_light, const Vector2i &p_layer_range) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->layer_range = p_layer_range;

	_light_changed(light);
}

int VertexLights3DServer::light_get_item_cull_mask(RID p_light) {
	const VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, 0);

	return light->item_cull_mask;
}
void VertexLights3DServer::light_set_item_cull_mask(RID p_light, const int p_item_cull_mask) {
	VertexLightData3D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->item_cull_mask = p_item_cull_mask;

	_light_changed(light);
}

// Sampling

Color VertexLights3DServer::sample_light(RID p_map, const Vector2 &p_position, const int p_item_cull_mask, const int p_layer, const int p_z_index) {
	VertexLightMap3D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Color());

	return map->sample_light(p_position, p_item_cull_mask, p_layer, p_z_index);
}

// Rest

void VertexLights3DServer::free(RID p_rid) {
	if (!p_rid.is_valid()) {
		ERR_FAIL_MSG("Invalid RID.");
		return;
	}

	if (map_owner.owns(p_rid)) {
		VertexLightMap3D *map = map_owner.get(p_rid);

		map->clear();
		map->self = RID();

		map_owner.free(p_rid);
		memdelete(map);
	} else if (light_owner.owns(p_rid)) {
		VertexLightData3D *light = light_owner.get(p_rid);

		if (light->map) {
			_light_changed(light);
			light->map->remove_light(light);
		}

		light->self = RID();

		light_owner.free(p_rid);
		memdelete(light);

	} else {
		ERR_FAIL_MSG("Invalid RID.");
	}
}

void VertexLights3DServer::init() {
	call_deferred("_register_update");
}

void VertexLights3DServer::register_update() {
	if (SceneTree::get_singleton()) {
		if (!SceneTree::get_singleton()->is_connected("idle_frame", this, "flush_notifications")) {
			SceneTree::get_singleton()->connect("idle_frame", this, "flush_notifications");
		}
	}
}

void VertexLights3DServer::unregister_update() {
	if (SceneTree::get_singleton()) {
		if (SceneTree::get_singleton()->is_connected("idle_frame", this, "flush_notifications")) {
			SceneTree::get_singleton()->disconnect("idle_frame", this, "flush_notifications");
		}
	}
}

void VertexLights3DServer::flush_notifications() {
	for (HashSet<RID>::Iterator iter = _changed_maps.begin(); iter; ++iter) {
		emit_signal(_map_changed_name, iter.key());
	}

	_changed_maps.clear();
}

VertexLights3DServer::VertexLights3DServer() {
	ERR_FAIL_COND(_self);

	_self = this;

	GLOBAL_DEF("vertex_lights_3d/default_quadrant_size", Vector2i(256, 256));
	_default_quadrant_size = GLOBAL_GET("vertex_lights_3d/default_quadrant_size");

	_map_changed_name = "map_changed";
}

VertexLights3DServer::~VertexLights3DServer() {
	_self = NULL;
}

void VertexLights3DServer::_bind_methods() {
	ADD_SIGNAL(MethodInfo("map_changed", PropertyInfo(Variant::RID, "map")));

	ClassDB::bind_method(D_METHOD("get_default_quadrant_size"), &VertexLights3DServer::get_default_quadrant_size);
	ClassDB::bind_method(D_METHOD("set_default_quadrant_size", "size"), &VertexLights3DServer::set_default_quadrant_size);

	ClassDB::bind_method(D_METHOD("map_create"), &VertexLights3DServer::map_create);

	ClassDB::bind_method(D_METHOD("map_get_quadrant_size", "map"), &VertexLights3DServer::map_get_quadrant_size);
	ClassDB::bind_method(D_METHOD("map_set_quadrant_size", "map", "size"), &VertexLights3DServer::map_set_quadrant_size);

	ClassDB::bind_method(D_METHOD("map_get_base_color", "map"), &VertexLights3DServer::map_get_base_color);
	ClassDB::bind_method(D_METHOD("map_set_base_color", "map", "base_color"), &VertexLights3DServer::map_set_base_color);

	ClassDB::bind_method(D_METHOD("map_get_lights", "map"), &VertexLights3DServer::map_get_lights);

	ClassDB::bind_method(D_METHOD("map_clear", "map"), &VertexLights3DServer::map_clear);

	// Lights

	ClassDB::bind_method(D_METHOD("light_create"), &VertexLights3DServer::light_create);

	ClassDB::bind_method(D_METHOD("light_get_map", "light"), &VertexLights3DServer::light_get_map);
	ClassDB::bind_method(D_METHOD("light_set_map", "light", "map"), &VertexLights3DServer::light_set_map);

	ClassDB::bind_method(D_METHOD("light_get_is_enabled", "light"), &VertexLights3DServer::light_get_is_enabled);
	ClassDB::bind_method(D_METHOD("light_set_enabled", "light", "enabled"), &VertexLights3DServer::light_set_enabled);

	ClassDB::bind_method(D_METHOD("light_get_position", "light"), &VertexLights3DServer::light_get_position);
	ClassDB::bind_method(D_METHOD("light_set_position", "light", "position"), &VertexLights3DServer::light_set_position);

	ClassDB::bind_method(D_METHOD("light_get_range", "light"), &VertexLights3DServer::light_get_range);
	ClassDB::bind_method(D_METHOD("light_set_range", "light", "range"), &VertexLights3DServer::light_set_range);

	ClassDB::bind_method(D_METHOD("light_get_attenuation", "light"), &VertexLights3DServer::light_get_attenuation);
	ClassDB::bind_method(D_METHOD("light_set_attenuation", "light", "attenuation"), &VertexLights3DServer::light_set_attenuation);

	ClassDB::bind_method(D_METHOD("light_get_color", "light"), &VertexLights3DServer::light_get_color);
	ClassDB::bind_method(D_METHOD("light_set_color", "light", "color"), &VertexLights3DServer::light_set_color);

	ClassDB::bind_method(D_METHOD("light_get_mode", "light"), &VertexLights3DServer::light_get_mode);
	ClassDB::bind_method(D_METHOD("light_set_mode", "light", "mode"), &VertexLights3DServer::light_set_mode);

	ClassDB::bind_method(D_METHOD("light_get_z_range", "light"), &VertexLights3DServer::light_get_z_range);
	ClassDB::bind_method(D_METHOD("light_set_z_range", "light", "z_range"), &VertexLights3DServer::light_set_z_range);

	ClassDB::bind_method(D_METHOD("light_get_layer_range", "light"), &VertexLights3DServer::light_get_layer_range);
	ClassDB::bind_method(D_METHOD("light_set_layer_range", "light", "layer_range"), &VertexLights3DServer::light_set_layer_range);

	ClassDB::bind_method(D_METHOD("light_get_item_cull_mask", "light"), &VertexLights3DServer::light_get_item_cull_mask);
	ClassDB::bind_method(D_METHOD("light_set_item_cull_mask", "light", "item_cull_mask"), &VertexLights3DServer::light_set_item_cull_mask);

	// Sampling

	ClassDB::bind_method(D_METHOD("sample_light", "map", "position", "item_cull_mask", "layer", "z_index"), &VertexLights3DServer::sample_light, DEFVAL(1), DEFVAL(0), DEFVAL(0));

	// Rest, DEFVAL(0)

	ClassDB::bind_method(D_METHOD("free", "rid"), &VertexLights3DServer::free);

	ClassDB::bind_method(D_METHOD("_register_update"), &VertexLights3DServer::register_update);
	ClassDB::bind_method(D_METHOD("flush_notifications"), &VertexLights3DServer::flush_notifications);

	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_ADD);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_SUB);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_MIX);
}

VertexLights3DServer *VertexLights3DServer::_self = NULL;
