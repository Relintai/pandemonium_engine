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

#include "vertex_lights_3d_server.h"

//VertexLightQuadrant3D

void VertexLights3DServer::VertexLightQuadrant3D::get_lights(List<VertexLightData3D *> *p_lights) {
	for (uint32_t i = 0; i < lights.size(); ++i) {
		p_lights->push_back(lights[i]);
	}
}

Color VertexLights3DServer::VertexLightQuadrant3D::sample_light_value(const Color &p_current_color, const Vector3 &p_position, const int p_item_cull_mask) {
	Color c = p_current_color;

	for (uint32_t i = 0; i < lights.size(); ++i) {
		VertexLightData3D *l = lights[i];

		if (!l->enabled) {
			continue;
		}

		if (l->range == 0) {
			continue;
		}

		if ((l->item_cull_mask & p_item_cull_mask) == 0) {
			continue;
		}


		Vector3 light_to_pos = p_position - l->position;

		Vector3 light_to_pos_normal_space = light_to_pos;
		light_to_pos_normal_space.x /= l->range;
		light_to_pos_normal_space.y /= l->range;
		light_to_pos_normal_space.z /= l->range;

		real_t ltpnsl = light_to_pos_normal_space.length();

		// Position is outside the light's range.
		if (ltpnsl >= 1) {
			continue;
		}

		real_t attenuation = pow(1.0 - ltpnsl, l->attenuation);

		Color ac = l->color * attenuation;

		switch (l->mode) {
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_ADD: {
				c += ac;
			} break;
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_SUB: {
				c -= ac;
			} break;
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_MIX: {
				c = c.blend(ac);
			} break;
		}

		c = c.clamp();
	}

	return c;
}

Color VertexLights3DServer::VertexLightQuadrant3D::sample_light(const Color &p_current_color, const Vector3 &p_position, const Vector3 &p_normal, const int p_item_cull_mask) {
	Color c = p_current_color;

	for (uint32_t i = 0; i < lights.size(); ++i) {
		VertexLightData3D *l = lights[i];

		if (!l->enabled) {
			continue;
		}

		if (l->range == 0) {
			continue;
		}

		if ((l->item_cull_mask & p_item_cull_mask) == 0) {
			continue;
		}

		Vector3 light_to_pos = p_position - l->position;

		Vector3 light_to_pos_normal_space = light_to_pos;
		light_to_pos_normal_space.x /= l->range;
		light_to_pos_normal_space.y /= l->range;
		light_to_pos_normal_space.z /= l->range;

		real_t ltpnsl = light_to_pos_normal_space.length();

		// Position is outside the light's range.
		if (ltpnsl >= 1) {
			continue;
		}

		real_t attenuation = pow(1.0 - ltpnsl, l->attenuation);

		Color ac = l->color * attenuation;

		switch (l->mode) {
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_ADD: {
				c += ac;
			} break;
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_SUB: {
				c -= ac;
			} break;
			case VertexLights3DServer::VERTEX_LIGHT_3D_MODE_MIX: {
				c = c.blend(ac);
			} break;
		}

		c = c.clamp();
	}

	return c;
}

//VertexLightMap3D

void VertexLights3DServer::VertexLightMap3D::recreate_octants() {
	List<VertexLightData3D *> lights;
	get_lights(&lights);

	for (HashMap<Vector3i, VertexLightQuadrant3D *>::Element *E = octants.front(); E; E = E->next) {
		memdelete(E->value());
	}

	octants.clear();

	for (List<VertexLightData3D *>::Element *E = lights.front(); E; E = E->next()) {
		VertexLightData3D *l = E->get();
		add_light(l);
	}
}

void VertexLights3DServer::VertexLightMap3D::get_lights(List<VertexLightData3D *> *p_lights) {
	for (HashMap<Vector3i, VertexLightQuadrant3D *>::Element *E = octants.front(); E; E = E->next) {
		E->value()->get_lights(p_lights);
	}
}

void VertexLights3DServer::VertexLightMap3D::add_light(VertexLightData3D *p_light) {
	VertexLightQuadrant3D *octant = get_octant_for_position(p_light->position);

	p_light->map = this;
	p_light->octant = octant;

	octant->lights.push_back(p_light);
}
void VertexLights3DServer::VertexLightMap3D::remove_light(VertexLightData3D *p_light) {
	p_light->map = NULL;

	VertexLightQuadrant3D *octant = p_light->octant;

	// Quadrant wan not updated properly somewhere!
	ERR_FAIL_NULL(octant);

	octant->lights.erase(p_light);
	p_light->octant = NULL;

	if (octant->lights.size() == 0) {
		octants.erase(octant->position);

		memdelete(octant);
	}
}

VertexLights3DServer::VertexLightQuadrant3D *VertexLights3DServer::VertexLightMap3D::get_octant_for_position(const Vector3 &p_position) {
	Vector3i octant_position = to_octant_position(p_position);

	if (!octants.has(octant_position)) {
		VertexLightQuadrant3D *octant = memnew(VertexLightQuadrant3D);
		octant->position = octant_position;
		octants[octant_position] = octant;
		return octant;
	}

	return octants[octant_position];
}

void VertexLights3DServer::VertexLightMap3D::set_light_position(VertexLightData3D *p_light, const Vector3 &p_position) {
	remove_light(p_light);
	p_light->position = p_position;
	add_light(p_light);
}

void VertexLights3DServer::VertexLightMap3D::clear() {
	List<VertexLightData3D *> lights;
	get_lights(&lights);

	for (HashMap<Vector3i, VertexLightQuadrant3D *>::Element *E = octants.front(); E; E = E->next) {
		memdelete(E->value());
	}

	octants.clear();

	for (List<VertexLightData3D *>::Element *E = lights.front(); E; E = E->next()) {
		VertexLightData3D *l = E->get();

		l->map = NULL;
		l->octant = NULL;
	}
}

Color VertexLights3DServer::VertexLightMap3D::sample_light_value(const Vector3 &p_position, const int p_item_cull_mask) {
	Color c = base_color;

	Vector3i octant_position = to_octant_position(p_position);

	for (int x = octant_position.x - 1; x <= octant_position.x + 1; ++x) {
		for (int y = octant_position.y - 1; y <= octant_position.y + 1; ++y) {
			for (int z = octant_position.z - 1; z <= octant_position.z + 1; ++z) {
				Vector3i qp = Vector3i(x, y, z);

				if (octants.has(qp)) {
					VertexLightQuadrant3D *q = octants[qp];

					c = q->sample_light_value(c, p_position, p_item_cull_mask);
				}
			}
		}
	}

	return c;
}

Color VertexLights3DServer::VertexLightMap3D::sample_light(const Vector3 &p_position, const Vector3 &p_normal, const int p_item_cull_mask) {
	Color c = base_color;

	Vector3i octant_position = to_octant_position(p_position);

	for (int x = octant_position.x - 1; x <= octant_position.x + 1; ++x) {
		for (int y = octant_position.y - 1; y <= octant_position.y + 1; ++y) {
			for (int z = octant_position.z - 1; z <= octant_position.z + 1; ++z) {
				Vector3i qp = Vector3i(x, y, z);

				if (octants.has(qp)) {
					VertexLightQuadrant3D *q = octants[qp];

					c = q->sample_light(c, p_position, p_normal, p_item_cull_mask);
				}
			}
		}
	}

	return c;
}
