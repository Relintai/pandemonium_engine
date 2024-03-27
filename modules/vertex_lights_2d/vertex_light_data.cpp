/*************************************************************************/
/*  vertex_light_data.cpp                                                */
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

//VertexLightQuadrant2D

void VertexLights2DServer::VertexLightQuadrant2D::get_lights(List<VertexLightData2D *> *p_lights) {
	for (uint32_t i = 0; i < lights.size(); ++i) {
		p_lights->push_back(lights[i]);
	}
}

Color VertexLights2DServer::VertexLightQuadrant2D::sample_light(const Color &p_current_color, const Vector2 &p_position, const int p_item_cull_mask, const int p_layer, const int p_z_index) {
	Color c = p_current_color;

	for (uint32_t i = 0; i < lights.size(); ++i) {
		VertexLightData2D *l = lights[i];
		
		if (!l->enabled) {
			continue;
		}

		if (l->range.x == 0 || l->range.y == 0) {
			continue;
		}

		if ((l->item_cull_mask & p_item_cull_mask) == 0) {
			continue;
		}

		if (p_layer < l->layer_range.x || p_layer > l->layer_range.y) {
			continue;
		}
		
		if (p_z_index < l->z_range.x || p_z_index > l->z_range.y) {
			continue;
		}
		
		Vector2 light_to_pos = p_position - l->position;
		
		Vector2 light_to_pos_normal_space = light_to_pos;
		light_to_pos_normal_space.x /= static_cast<real_t>(l->range.x);
		light_to_pos_normal_space.y /= static_cast<real_t>(l->range.y);

		real_t ltpnsl = light_to_pos_normal_space.length();

		// Position is outside the light's range.
		if (ltpnsl >= 1) {
			continue;
		}
		
		real_t attenuation = pow(1.0 - ltpnsl, l->attenuation);

		Color ac = l->color * attenuation;

		switch (l->mode) {
			case VertexLights2DServer::VERTEX_LIGHT_2D_MODE_ADD: {
				c += ac;
			} break;
			case VertexLights2DServer::VERTEX_LIGHT_2D_MODE_SUB: {
				c -= ac;
			} break;
			case VertexLights2DServer::VERTEX_LIGHT_2D_MODE_MIX: {
				c = c.blend(ac);
			} break;
		}
		
		c = c.clamp();
	}
	
	return c;
}

//VertexLightMap2D

void VertexLights2DServer::VertexLightMap2D::recreate_quadrants() {
	List<VertexLightData2D *> lights;
	get_lights(&lights);

	for (HashMap<Vector2i, VertexLightQuadrant2D *>::Element *E = quadrants.front(); E; E = E->next) {
		memdelete(E->value());
	}

	quadrants.clear();

	for (List<VertexLightData2D *>::Element *E = lights.front(); E; E = E->next()) {
		VertexLightData2D *l = E->get();
		add_light(l);
	}
}

void VertexLights2DServer::VertexLightMap2D::get_lights(List<VertexLightData2D *> *p_lights) {
	for (HashMap<Vector2i, VertexLightQuadrant2D *>::Element *E = quadrants.front(); E; E = E->next) {
		E->value()->get_lights(p_lights);
	}
}

void VertexLights2DServer::VertexLightMap2D::add_light(VertexLightData2D *p_light) {
	VertexLightQuadrant2D *quadrant = get_quadrant_for_position(p_light->position);

	p_light->map = this;
	p_light->quadrant = quadrant;

	quadrant->lights.push_back(p_light);
}
void VertexLights2DServer::VertexLightMap2D::remove_light(VertexLightData2D *p_light) {
	p_light->map = NULL;

	VertexLightQuadrant2D *quadrant = p_light->quadrant;

	// Quadrant wan not updated properly somewhere!
	ERR_FAIL_NULL(quadrant);

	quadrant->lights.erase(p_light);
	p_light->quadrant = NULL;

	if (quadrant->lights.size() == 0) {
		quadrants.erase(quadrant->position);

		memdelete(quadrant);
	}
}

VertexLights2DServer::VertexLightQuadrant2D *VertexLights2DServer::VertexLightMap2D::get_quadrant_for_position(const Vector2 &p_position) {
	Vector2i quadrant_position = to_quadrant_position(p_position);

	if (!quadrants.has(quadrant_position)) {
		VertexLightQuadrant2D *quadrant = memnew(VertexLightQuadrant2D);
		quadrant->position = quadrant_position;
		quadrants[quadrant_position] = quadrant;
		return quadrant;
	}

	return quadrants[quadrant_position];
}

void VertexLights2DServer::VertexLightMap2D::set_light_position(VertexLightData2D *p_light, const Vector2 &p_position) {
	remove_light(p_light);
	p_light->position = p_position;
	add_light(p_light);
}

void VertexLights2DServer::VertexLightMap2D::clear() {
	List<VertexLightData2D *> lights;
	get_lights(&lights);

	for (HashMap<Vector2i, VertexLightQuadrant2D *>::Element *E = quadrants.front(); E; E = E->next) {
		memdelete(E->value());
	}

	quadrants.clear();

	for (List<VertexLightData2D *>::Element *E = lights.front(); E; E = E->next()) {
		VertexLightData2D *l = E->get();

		l->map = NULL;
		l->quadrant = NULL;
	}
}

Color VertexLights2DServer::VertexLightMap2D::sample_light(const Vector2 &p_position, const int p_item_cull_mask, const int p_layer, const int p_z_index) {
	Color c = base_color;

	Vector2i quadrant_position = to_quadrant_position(p_position);
	
	for (int x = quadrant_position.x - 1; x <= quadrant_position.x + 1; ++x) {
		for (int y = quadrant_position.y - 1; y <= quadrant_position.y + 1; ++y) {
			Vector2i qp = Vector2i(x, y);

			if (quadrants.has(qp)) {
				VertexLightQuadrant2D *q = quadrants[qp];
				
				c = q->sample_light(c, p_position, p_item_cull_mask, p_layer, p_z_index);
			}
		}
	}

	return c;
}
