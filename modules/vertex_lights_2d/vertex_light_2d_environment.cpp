/*************************************************************************/
/*  vertex_light_2d_environment.cpp                                      */
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

#include "vertex_light_2d_environment.h"

#include "core/config/engine.h"

#include "scene/resources/world_2d.h"

#include "vertex_lights_2d_server.h"

bool VertexLight2DEnvironment::get_is_enabled() {
	return _enabled;
}
void VertexLight2DEnvironment::set_enabled(const bool p_enabled) {
	_enabled = p_enabled;

	_update_light_visibility();
}

Color VertexLight2DEnvironment::get_color() {
	return _color;
}
void VertexLight2DEnvironment::set_color(const Color &p_color) {
	_color = p_color;

	_update_light_visibility();
}

VertexLight2DEnvironment::VertexLight2DEnvironment() {
	_enabled = true;
}

VertexLight2DEnvironment::~VertexLight2DEnvironment() {
}

void VertexLight2DEnvironment::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			_update_light_visibility();
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			_update_light_visibility();
		} break;
	}
}

void VertexLight2DEnvironment::_update_light_visibility() {
	if (!is_inside_tree() || !_enabled || !is_visible_in_tree()) {
		return;
	}

	Ref<World2D> world = get_world_2d();
	ERR_FAIL_COND(!world.is_valid());
	RID map = world->get_vertex_lights_2d_map();

	VertexLights2DServer::get_singleton()->map_set_base_color(map, _color);
}

void VertexLight2DEnvironment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_enabled"), &VertexLight2DEnvironment::get_is_enabled);
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &VertexLight2DEnvironment::set_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_is_enabled");

	ClassDB::bind_method(D_METHOD("get_color"), &VertexLight2DEnvironment::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &VertexLight2DEnvironment::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}
