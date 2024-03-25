/*************************************************************************/
/*  vertex_lights_2d.cpp                                                 */
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

#include "vertex_lights_2d_server.h"

#include "core/config/project_settings.h"

// Defaults
Vector2i VertexLights2DServer::get_default_quadrant_size() const {
	return _default_quadrant_size;
}
void VertexLights2DServer::set_default_quadrant_size(const Vector2i &p_size) {
	_default_quadrant_size = p_size;
}

// Maps
RID VertexLights2DServer::map_create() {
	VertexLightMap2D *map = memnew(VertexLightMap2D);
	RID rid = map_owner.make_rid(map);
	map->self = rid;
	return rid;
}

Vector2i VertexLights2DServer::map_get_quadrant_size(RID p_map) const {
	const VertexLightMap2D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Vector2i());

	return map->quadrant_size;
}
void VertexLights2DServer::map_set_quadrant_size(RID p_map, const Vector2i &p_size) {
	VertexLightMap2D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND(map == NULL);

	map->quadrant_size = p_size;
	map->recreate_quadrants();
}

Array VertexLights2DServer::map_get_lights(RID p_map) const {
	VertexLightMap2D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND_V(map == NULL, Array());

	List<VertexLightData2D *> lights;
	map->get_lights(&lights);

	Array arr;

	arr.resize(lights.size());

	int i = 0;
	for (List<VertexLightData2D *>::Element *E = lights.front(); E; E = E->next()) {
		arr[i++] = E->get()->self;
	}

	return arr;
}

void VertexLights2DServer::map_clear(RID p_map) {
	VertexLightMap2D *map = map_owner.getornull(p_map);
	ERR_FAIL_COND(map == NULL);

	map->clear();
}

// Lights
RID VertexLights2DServer::light_create() {
	VertexLightData2D *light = memnew(VertexLightData2D);
	RID rid = light_owner.make_rid(light);
	light->self = rid;
	return rid;
}

RID VertexLights2DServer::light_get_map(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, RID());

	if (!light->map) {
		return RID();
	}

	return light->map->self;
}
void VertexLights2DServer::light_set_map(RID p_light, RID p_map) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	VertexLightMap2D *map = map_owner.getornull(p_map);

	if (light->map) {
		light->map->remove_light(light);
	}

	if (map) {
		map->add_light(light);
	}
}

bool VertexLights2DServer::light_get_is_enabled(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, false);

	return light->enabled;
}
void VertexLights2DServer::light_set_enabled(RID p_light, const bool p_enabled) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->enabled = p_enabled;
}

Vector2 VertexLights2DServer::light_get_position(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2());

	return light->position;
}
void VertexLights2DServer::light_set_position(RID p_light, const Vector2 &p_position) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	if (light->map) {
		// This ensure the light gets moved to the proper quadrant
		light->map->set_light_position(light, p_position);
		return;
	}

	light->position = p_position;
}

Color VertexLights2DServer::light_get_color(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Color());

	return light->color;
}
void VertexLights2DServer::light_set_color(RID p_light, const Color &p_color) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->color = p_color;
}

VertexLights2DServer::VertexLight2DMode VertexLights2DServer::light_get_mode(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, VertexLights2DServer::VERTEX_LIGHT_2D_MODE_ADD);

	return light->mode;
}
void VertexLights2DServer::light_set_mode(RID p_light, const VertexLights2DServer::VertexLight2DMode p_mode) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->mode = p_mode;
}

Vector2i VertexLights2DServer::light_get_z_range(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2i());

	return light->z_range;
}
void VertexLights2DServer::light_set_z_range(RID p_light, const Vector2i &p_z_range) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->z_range = p_z_range;
}

Vector2i VertexLights2DServer::light_get_layer_range(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, Vector2i());

	return light->layer_range;
}
void VertexLights2DServer::light_set_layer_range(RID p_light, const Vector2i &p_layer_range) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->layer_range = p_layer_range;
}

int VertexLights2DServer::light_get_item_cull_mask(RID p_light) {
	const VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND_V(light == NULL, 0);

	return light->item_cull_mask;
}
void VertexLights2DServer::light_set_item_cull_mask(RID p_light, const int p_item_cull_mask) {
	VertexLightData2D *light = light_owner.getornull(p_light);
	ERR_FAIL_COND(light == NULL);

	light->item_cull_mask = p_item_cull_mask;
}

// Rest

void VertexLights2DServer::free(RID p_rid) {
	if (!p_rid.is_valid()) {
		ERR_FAIL_MSG("Invalid RID.");
		return;
	}

	if (map_owner.owns(p_rid)) {
		VertexLightMap2D *map = map_owner.get(p_rid);

		map->clear();
		map->self = RID();

		map_owner.free(p_rid);
		memdelete(map);
	} else if (light_owner.owns(p_rid)) {
		VertexLightData2D *light = light_owner.get(p_rid);

		if (light->map) {
			light->map->remove_light(light);
		}

		light->self = RID();

		light_owner.free(p_rid);
		memdelete(light);

	} else {
		ERR_FAIL_MSG("Invalid RID.");
	}
}

VertexLights2DServer::VertexLights2DServer() {
	ERR_FAIL_COND(_self);

	_self = this;

	GLOBAL_DEF("vertex_lights_2d/default_quadrant_size", Vector2i(256, 256));
	_default_quadrant_size = GLOBAL_GET("vertex_lights_2d/default_quadrant_size");
}

VertexLights2DServer::~VertexLights2DServer() {
	_self = NULL;
}

void VertexLights2DServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_default_quadrant_size"), &VertexLights2DServer::get_default_quadrant_size);
	ClassDB::bind_method(D_METHOD("set_default_quadrant_size", "size"), &VertexLights2DServer::set_default_quadrant_size);

	ClassDB::bind_method(D_METHOD("map_create"), &VertexLights2DServer::map_create);

	ClassDB::bind_method(D_METHOD("map_get_quadrant_size", "map"), &VertexLights2DServer::map_get_quadrant_size);
	ClassDB::bind_method(D_METHOD("map_set_quadrant_size", "map", "size"), &VertexLights2DServer::map_set_quadrant_size);

	ClassDB::bind_method(D_METHOD("map_get_lights", "map"), &VertexLights2DServer::map_get_lights);

	ClassDB::bind_method(D_METHOD("map_clear", "map"), &VertexLights2DServer::map_clear);

	// Lights

	ClassDB::bind_method(D_METHOD("light_create"), &VertexLights2DServer::light_create);

	ClassDB::bind_method(D_METHOD("light_get_map", "light"), &VertexLights2DServer::light_get_map);
	ClassDB::bind_method(D_METHOD("light_set_map", "light", "map"), &VertexLights2DServer::light_set_map);

	ClassDB::bind_method(D_METHOD("light_get_is_enabled", "light"), &VertexLights2DServer::light_get_is_enabled);
	ClassDB::bind_method(D_METHOD("light_set_enabled", "light", "enabled"), &VertexLights2DServer::light_set_enabled);

	ClassDB::bind_method(D_METHOD("light_get_position", "light"), &VertexLights2DServer::light_get_position);
	ClassDB::bind_method(D_METHOD("light_set_position", "light", "position"), &VertexLights2DServer::light_set_position);

	ClassDB::bind_method(D_METHOD("light_get_color", "light"), &VertexLights2DServer::light_get_color);
	ClassDB::bind_method(D_METHOD("light_set_color", "light", "color"), &VertexLights2DServer::light_set_color);

	ClassDB::bind_method(D_METHOD("light_get_mode", "light"), &VertexLights2DServer::light_get_mode);
	ClassDB::bind_method(D_METHOD("light_set_mode", "light", "mode"), &VertexLights2DServer::light_set_mode);

	ClassDB::bind_method(D_METHOD("light_get_z_range", "light"), &VertexLights2DServer::light_get_z_range);
	ClassDB::bind_method(D_METHOD("light_set_z_range", "light", "z_range"), &VertexLights2DServer::light_set_z_range);

	ClassDB::bind_method(D_METHOD("light_get_layer_range", "light"), &VertexLights2DServer::light_get_layer_range);
	ClassDB::bind_method(D_METHOD("light_set_layer_range", "light", "layer_range"), &VertexLights2DServer::light_set_layer_range);

	ClassDB::bind_method(D_METHOD("light_get_item_cull_mask", "light"), &VertexLights2DServer::light_get_item_cull_mask);
	ClassDB::bind_method(D_METHOD("light_set_item_cull_mask", "light", "item_cull_mask"), &VertexLights2DServer::light_set_item_cull_mask);

	// Rest

	ClassDB::bind_method(D_METHOD("free", "rid"), &VertexLights2DServer::free);

	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_ADD);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_SUB);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_MIX);
}

VertexLights2DServer *VertexLights2DServer::_self = NULL;
