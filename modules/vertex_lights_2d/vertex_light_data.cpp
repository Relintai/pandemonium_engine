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

#include "vertex_light_data.h"

//VertexLightQuadrant2D

void VertexLightQuadrant2D::get_lights(List<VertexLightData2D *> *p_lights) {
	for (uint32_t i = 0; i < lights.size(); ++i) {
		p_lights->push_back(lights[i]);
	}
}

//VertexLightMap2D

void VertexLightMap2D::recreate_quadrants() {
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

void VertexLightMap2D::get_lights(List<VertexLightData2D *> *p_lights) {
	for (HashMap<Vector2i, VertexLightQuadrant2D *>::Element *E = quadrants.front(); E; E = E->next) {
		E->value()->get_lights(p_lights);
	}
}

void VertexLightMap2D::add_light(VertexLightData2D *p_light) {
	VertexLightQuadrant2D *quadrant = get_quadrant_for_position(p_light->position);

	p_light->map = this;
	p_light->quadrant = quadrant;

	quadrant->lights.push_back(p_light);
}
void VertexLightMap2D::remove_light(VertexLightData2D *p_light) {
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

VertexLightQuadrant2D *VertexLightMap2D::get_quadrant_for_position(const Vector2 &p_position) {
	Vector2i quadrant_position = to_quadrant_position(p_position);

	if (!quadrants.has(quadrant_position)) {
		VertexLightQuadrant2D *quadrant = memnew(VertexLightQuadrant2D);
		quadrant->position = quadrant_position;
		quadrants[quadrant_position] = quadrant;
		return quadrant;
	}

	return quadrants[quadrant_position];
}

void VertexLightMap2D::set_light_position(VertexLightData2D *p_light, const Vector2 &p_position) {
	remove_light(p_light);
	add_light(p_light);
}

void VertexLightMap2D::clear() {
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
