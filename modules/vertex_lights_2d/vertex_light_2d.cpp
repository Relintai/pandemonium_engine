/*************************************************************************/
/*  vertex_light_2d.cpp                                                  */
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

#include "vertex_light_2d.h"

#include "core/config/engine.h"

#include "scene/resources/world_2d.h"

bool VertexLight2D::get_is_enabled() {
	return _enabled;
}
void VertexLight2D::set_enabled(const bool p_enabled) {
	_enabled = p_enabled;

	_update_light_visibility();
}

Vector2i VertexLight2D::get_range() {
	return _range;
}
void VertexLight2D::set_range(const Vector2i &p_range) {
	_range = p_range;

	VertexLights2DServer::get_singleton()->light_set_range(_vertex_light, _range);
}

real_t VertexLight2D::get_attenuation() {
	return _attenuation;
}
void VertexLight2D::set_attenuation(const real_t p_attenuation) {
	_attenuation = p_attenuation;

	VertexLights2DServer::get_singleton()->light_set_attenuation(_vertex_light, _attenuation);
}

Color VertexLight2D::get_color() {
	return _color;
}
void VertexLight2D::set_color(const Color &p_color) {
	_color = p_color;

	VertexLights2DServer::get_singleton()->light_set_color(_vertex_light, _color);
}

VertexLight2D::VertexLight2DMode VertexLight2D::get_mode() {
	return _mode;
}
void VertexLight2D::set_mode(const VertexLight2D::VertexLight2DMode p_mode) {
	_mode = p_mode;

	VertexLights2DServer::get_singleton()->light_set_mode(_vertex_light, (VertexLights2DServer::VertexLight2DMode)_mode);
}

Vector2i VertexLight2D::get_z_range() {
	return _z_range;
}
void VertexLight2D::set_z_range(const Vector2i &p_z_range) {
	_z_range = p_z_range;

	VertexLights2DServer::get_singleton()->light_set_z_range(_vertex_light, _z_range);
}

Vector2i VertexLight2D::get_layer_range() {
	return _layer_range;
}
void VertexLight2D::set_layer_range(const Vector2i &p_layer_range) {
	_layer_range = p_layer_range;

	VertexLights2DServer::get_singleton()->light_set_layer_range(_vertex_light, _layer_range);
}

int VertexLight2D::get_item_cull_mask() {
	return _item_cull_mask;
}
void VertexLight2D::set_item_cull_mask(const int p_item_cull_mask) {
	_item_cull_mask = p_item_cull_mask;

	VertexLights2DServer::get_singleton()->light_set_item_cull_mask(_vertex_light, _item_cull_mask);
}

VertexLight2D::VertexLight2D() {
	_vertex_light = RID_PRIME(VertexLights2DServer::get_singleton()->light_create());

	_enabled = true;
	_range = Vector2i(32, 32);
	_attenuation = 1;
	_color = Color(1, 1, 1, 1);
	_item_cull_mask = 1;
	_z_range = Vector2i(-1024, 1024);
	_layer_range = Vector2i(-512, 512);
	_mode = VERTEX_LIGHT_2D_MODE_ADD;

	set_notify_transform(true);
}

VertexLight2D::~VertexLight2D() {
	VertexLights2DServer::get_singleton()->free(_vertex_light);
}

void VertexLight2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			Ref<World2D> world = get_world_2d();
			ERR_FAIL_COND(!world.is_valid());
			RID map = world->get_vertex_lights_2d_map();
			VertexLights2DServer::get_singleton()->light_set_map(_vertex_light, map);
			
			VertexLights2DServer::get_singleton()->light_set_position(_vertex_light, get_global_transform().get_origin());

			_update_light_visibility();
		} break;
		case NOTIFICATION_EXIT_TREE: {
			VertexLights2DServer::get_singleton()->light_set_map(_vertex_light, RID());

			_update_light_visibility();
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			VertexLights2DServer::get_singleton()->light_set_position(_vertex_light, get_global_transform().get_origin());
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			_update_light_visibility();
		} break;
	}
}

void VertexLight2D::_update_light_visibility() {
	if (!is_inside_tree()) {
		return;
	}

	VertexLights2DServer::get_singleton()->light_set_enabled(_vertex_light, _enabled && is_visible_in_tree());
}

void VertexLight2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_enabled"), &VertexLight2D::get_is_enabled);
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &VertexLight2D::set_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_is_enabled");

	ClassDB::bind_method(D_METHOD("get_range"), &VertexLight2D::get_range);
	ClassDB::bind_method(D_METHOD("set_range", "range"), &VertexLight2D::set_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "range"), "set_range", "get_range");

	ClassDB::bind_method(D_METHOD("get_attenuation"), &VertexLight2D::get_attenuation);
	ClassDB::bind_method(D_METHOD("set_attenuation", "attenuation"), &VertexLight2D::set_attenuation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_attenuation", "get_attenuation");

	ClassDB::bind_method(D_METHOD("get_color"), &VertexLight2D::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &VertexLight2D::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_mode"), &VertexLight2D::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "mode"), &VertexLight2D::set_mode);
	//,Mask
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Add,Sub,Mix"), "set_mode", "get_mode");

	ClassDB::bind_method(D_METHOD("get_z_range"), &VertexLight2D::get_z_range);
	ClassDB::bind_method(D_METHOD("set_z_range", "z_range"), &VertexLight2D::set_z_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "z_range", PROPERTY_HINT_RANGE, itos(RS::CANVAS_ITEM_Z_MIN) + "," + itos(RS::CANVAS_ITEM_Z_MAX) + ",1"), "set_z_range", "get_z_range");

	ClassDB::bind_method(D_METHOD("get_layer_range"), &VertexLight2D::get_layer_range);
	ClassDB::bind_method(D_METHOD("set_layer_range", "layer_range"), &VertexLight2D::set_layer_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "layer_range", PROPERTY_HINT_RANGE, "-512,512,1"), "set_layer_range", "get_layer_range");

	ClassDB::bind_method(D_METHOD("get_item_cull_mask"), &VertexLight2D::get_item_cull_mask);
	ClassDB::bind_method(D_METHOD("set_item_cull_mask", "item_cull_mask"), &VertexLight2D::set_item_cull_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "item_cull_mask", PROPERTY_HINT_LAYERS_2D_RENDER), "set_item_cull_mask", "get_item_cull_mask");

	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_ADD);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_SUB);
	BIND_ENUM_CONSTANT(VERTEX_LIGHT_2D_MODE_MIX);
}
