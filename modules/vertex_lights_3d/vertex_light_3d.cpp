/*************************************************************************/
/*  vertex_light_3d.cpp                                                  */
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

#include "vertex_light_3d.h"

#include "core/config/engine.h"

#include "scene/resources/world_3d.h"

bool VertexLight3D::get_is_enabled() {
	return _enabled;
}
void VertexLight3D::set_enabled(const bool p_enabled) {
	_enabled = p_enabled;

	_update_light_visibility();
}

real_t VertexLight3D::get_range() {
	return _range;
}
void VertexLight3D::set_range(const real_t p_range) {
	_range = p_range;

	VertexLights3DServer::get_singleton()->light_set_range(_vertex_light, _range);
}

real_t VertexLight3D::get_attenuation() {
	return _attenuation;
}
void VertexLight3D::set_attenuation(const real_t p_attenuation) {
	_attenuation = p_attenuation;

	VertexLights3DServer::get_singleton()->light_set_attenuation(_vertex_light, _attenuation);
}

Color VertexLight3D::get_color() {
	return _color;
}
void VertexLight3D::set_color(const Color &p_color) {
	_color = p_color;

	VertexLights3DServer::get_singleton()->light_set_color(_vertex_light, _color);
}

VertexLight3D::VertexLight3DMode VertexLight3D::get_mode() {
	return _mode;
}
void VertexLight3D::set_mode(const VertexLight3D::VertexLight3DMode p_mode) {
	_mode = p_mode;

	VertexLights3DServer::get_singleton()->light_set_mode(_vertex_light, (VertexLights3DServer::VertexLight3DMode)_mode);
}

int VertexLight3D::get_item_cull_mask() {
	return _item_cull_mask;
}
void VertexLight3D::set_item_cull_mask(const int p_item_cull_mask) {
	_item_cull_mask = p_item_cull_mask;

	VertexLights3DServer::get_singleton()->light_set_item_cull_mask(_vertex_light, _item_cull_mask);
}

VertexLight3D::VertexLight3D() {
	_vertex_light = RID_PRIME(VertexLights3DServer::get_singleton()->light_create());

	_enabled = true;
	_range = 5;
	_attenuation = 1;
	_color = Color(1, 1, 1, 1);
	_item_cull_mask = 1;
	_mode = VERTEX_LIGHT_3D_MODE_ADD;

	set_notify_transform(true);
}

VertexLight3D::~VertexLight3D() {
	VertexLights3DServer::get_singleton()->free(_vertex_light);
}

void VertexLight3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Ref<World3D> world = get_world_3d();
			ERR_FAIL_COND(!world.is_valid());
			RID map = world->get_vertex_lights_3d_map();
			VertexLights3DServer::get_singleton()->light_set_map(_vertex_light, map);

			VertexLights3DServer::get_singleton()->light_set_position(_vertex_light, get_global_transform().get_origin());

			_update_light_visibility();
		} break;
		case NOTIFICATION_EXIT_TREE: {
			VertexLights3DServer::get_singleton()->light_set_map(_vertex_light, RID());

			_update_light_visibility();
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			VertexLights3DServer::get_singleton()->light_set_position(_vertex_light, get_global_transform().get_origin());
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			_update_light_visibility();
		} break;
	}
}

void VertexLight3D::_update_light_visibility() {
	if (!is_inside_tree()) {
		return;
	}

	VertexLights3DServer::get_singleton()->light_set_enabled(_vertex_light, _enabled && is_visible_in_tree());
}

void VertexLight3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_enabled"), &VertexLight3D::get_is_enabled);
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &VertexLight3D::set_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_is_enabled");

	ClassDB::bind_method(D_METHOD("get_range"), &VertexLight3D::get_range);
	ClassDB::bind_method(D_METHOD("set_range", "range"), &VertexLight3D::set_range);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "range"), "set_range", "get_range");

	ClassDB::bind_method(D_METHOD("get_attenuation"), &VertexLight3D::get_attenuation);
	ClassDB::bind_method(D_METHOD("set_attenuation", "attenuation"), &VertexLight3D::set_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_attenuation", "get_attenuation");

	ClassDB::bind_method(D_METHOD("get_color"), &VertexLight3D::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &VertexLight3D::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_mode"), &VertexLight3D::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "mode"), &VertexLight3D::set_mode);
	//,Mask
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Add,Sub,Mix"), "set_mode", "get_mode");

	ClassDB::bind_method(D_METHOD("get_item_cull_mask"), &VertexLight3D::get_item_cull_mask);
	ClassDB::bind_method(D_METHOD("set_item_cull_mask", "item_cull_mask"), &VertexLight3D::set_item_cull_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "item_cull_mask", PROPERTY_HINT_LAYERS_3D_RENDER), "set_item_cull_mask", "get_item_cull_mask");

	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_ADD);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_SUB);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_3D_MODE_MIX);
}
