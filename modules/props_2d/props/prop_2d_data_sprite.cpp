/*************************************************************************/
/*  prop_2d_data_sprite.cpp                                              */
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

#include "prop_2d_data_sprite.h"

#include "prop_2d_data.h"

#include "scene/2d/sprite.h"

Ref<Texture> Prop2DDataSprite::get_texture() const {
	return _texture;
}
void Prop2DDataSprite::set_texture(const Ref<Texture> &value) {
	_texture = value;
}

Ref<Texture> Prop2DDataSprite::get_normal_map() const {
	return _normal_map;
}
void Prop2DDataSprite::set_normal_map(const Ref<Texture> &value) {
	_normal_map = value;
}

bool Prop2DDataSprite::get_centered() const {
	return _centered;
}
void Prop2DDataSprite::set_centered(const bool value) {
	_centered = value;
}

Vector2 Prop2DDataSprite::get_offset() const {
	return _offset;
}
void Prop2DDataSprite::set_offset(const Vector2 &value) {
	_offset = value;
}

bool Prop2DDataSprite::get_flip_h() const {
	return _flip_h;
}
void Prop2DDataSprite::set_flip_h(const bool value) {
	_flip_h = value;
}

bool Prop2DDataSprite::get_flip_v() const {
	return _flip_v;
}
void Prop2DDataSprite::set_flip_v(const bool value) {
	_flip_v = value;
}

int Prop2DDataSprite::get_animation_h_frames() const {
	return _animation_h_frames;
}
void Prop2DDataSprite::set_animation_h_frames(const int value) {
	_animation_h_frames = value;
}

int Prop2DDataSprite::get_animation_v_frames() const {
	return _animation_v_frames;
}
void Prop2DDataSprite::set_animation_v_frames(const int value) {
	_animation_v_frames = value;
}

int Prop2DDataSprite::get_animation_frame() const {
	return _animation_frame;
}
void Prop2DDataSprite::set_animation_frame(const int value) {
	_animation_frame = value;
}

Vector2 Prop2DDataSprite::get_animation_frame_coords() const {
	return _animation_frame_coords;
}
void Prop2DDataSprite::set_animation_frame_coords(const Vector2 &value) {
	_animation_frame_coords = value;
}

bool Prop2DDataSprite::get_region_enabled() const {
	return _region_enabled;
}
void Prop2DDataSprite::set_region_enabled(const bool value) {
	_region_enabled = value;
}

Rect2 Prop2DDataSprite::get_region() const {
	return _region;
}
void Prop2DDataSprite::set_region(const Rect2 &value) {
	_region = value;
}

bool Prop2DDataSprite::get_filter_clip() const {
	return _filter_clip;
}
void Prop2DDataSprite::set_filter_clip(const bool value) {
	_filter_clip = value;
}

bool Prop2DDataSprite::_processor_handles(Node *node) {
	Sprite *s = Object::cast_to<Sprite>(node);

	return s;
}

void Prop2DDataSprite::_processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry) {
	Sprite *s = Object::cast_to<Sprite>(node);

	ERR_FAIL_COND(!s);

	Ref<Prop2DDataSprite> l;

	if (entry.is_valid()) {
		l = entry;
	} else {
		l.instance();
	}

	l->set_texture(s->get_texture());
	l->set_normal_map(s->get_normal_map());

	l->set_centered(s->is_centered());
	l->set_offset(s->get_offset());

	l->set_flip_h(s->is_flipped_h());
	l->set_flip_v(s->is_flipped_v());

	l->set_animation_h_frames(s->get_hframes());
	l->set_animation_v_frames(s->get_vframes());
	l->set_animation_frame(s->get_frame());
	l->set_animation_frame_coords(s->get_frame_coords());

	l->set_region_enabled(s->is_region());
	l->set_region(s->get_region_rect());
	l->set_filter_clip(s->is_region_filter_clip_enabled());

	Prop2DDataEntry::_processor_process(prop_data, node, transform, l);
}

Node *Prop2DDataSprite::_processor_get_node_for(const Transform2D &transform, Node *node) {
	Sprite *n;

	if (node) {
		n = Object::cast_to<Sprite>(node);
	} else {
		n = memnew(Sprite());
	}

	n->set_texture(get_texture());
	n->set_normal_map(get_normal_map());
	n->set_centered(get_centered());
	n->set_offset(get_offset());

	n->set_flip_h(get_flip_h());
	n->set_flip_v(get_flip_v());

	n->set_hframes(get_animation_h_frames());
	n->set_vframes(get_animation_v_frames());
	n->set_frame(get_animation_frame());
	n->set_frame_coords(get_animation_frame_coords());

	n->set_region(get_region_enabled());
	n->set_region_rect(get_region());
	n->set_region_filter_clip(get_filter_clip());

	return Prop2DDataEntry::_processor_get_node_for(transform, n);
}

Prop2DDataSprite::Prop2DDataSprite() {
	_centered = false;
	_flip_h = false;
	_flip_v = false;

	_animation_h_frames = 0;
	_animation_v_frames = 0;
	_animation_frame = 0;

	_region_enabled = false;

	_filter_clip = false;
}
Prop2DDataSprite::~Prop2DDataSprite() {
}

#ifndef DISABLE_DEPRECATED
// TODO REMOVE AFTER NEXT RELEASE
bool Prop2DDataSprite::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "get_flip_v") {
		set_flip_v(p_value);

		return true;
	} else if (p_name == "get_animation_h_frames") {
		set_animation_h_frames(p_value);

		return true;
	} else if (p_name == "set_animation_v_frames") {
		set_animation_v_frames(p_value);

		return true;
	} else if (p_name == "get_animation_frame") {
		set_animation_frame(p_value);

		return true;
	} else if (p_name == "get_animation_frame_coords") {
		set_animation_frame_coords(p_value);

		return true;
	} else if (p_name == "set_region_enabled") {
		set_region_enabled(p_value);

		return true;
	} else if (p_name == "get_region") {
		set_region(p_value);

		return true;
	} else if (p_name == "get_filter_clip") {
		set_filter_clip(p_value);

		return true;
	}

	return false;
}
#endif

void Prop2DDataSprite::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture"), &Prop2DDataSprite::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &Prop2DDataSprite::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_normal_map"), &Prop2DDataSprite::get_normal_map);
	ClassDB::bind_method(D_METHOD("set_normal_map", "value"), &Prop2DDataSprite::set_normal_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal_map"), "set_normal_map", "get_normal_map");

	ClassDB::bind_method(D_METHOD("get_centered"), &Prop2DDataSprite::get_centered);
	ClassDB::bind_method(D_METHOD("set_centered", "value"), &Prop2DDataSprite::set_centered);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "centered"), "set_centered", "get_centered");

	ClassDB::bind_method(D_METHOD("get_offset"), &Prop2DDataSprite::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "value"), &Prop2DDataSprite::set_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");

	ClassDB::bind_method(D_METHOD("get_flip_h"), &Prop2DDataSprite::get_flip_h);
	ClassDB::bind_method(D_METHOD("set_flip_h", "value"), &Prop2DDataSprite::set_flip_h);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");

	ClassDB::bind_method(D_METHOD("get_flip_v"), &Prop2DDataSprite::get_flip_v);
	ClassDB::bind_method(D_METHOD("set_flip_v", "value"), &Prop2DDataSprite::set_flip_v);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");

	ClassDB::bind_method(D_METHOD("get_animation_h_frames"), &Prop2DDataSprite::get_animation_h_frames);
	ClassDB::bind_method(D_METHOD("set_animation_h_frames", "value"), &Prop2DDataSprite::set_animation_h_frames);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "animation_h_frames"), "set_animation_h_frames", "get_animation_h_frames");

	ClassDB::bind_method(D_METHOD("get_animation_v_frames"), &Prop2DDataSprite::get_animation_v_frames);
	ClassDB::bind_method(D_METHOD("set_animation_v_frames", "value"), &Prop2DDataSprite::set_animation_v_frames);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "animation_v_frames"), "set_animation_v_frames", "get_animation_v_frames");

	ClassDB::bind_method(D_METHOD("get_animation_frame"), &Prop2DDataSprite::get_animation_frame);
	ClassDB::bind_method(D_METHOD("set_animation_frame", "value"), &Prop2DDataSprite::set_animation_frame);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "animation_frame"), "set_animation_frame", "get_animation_frame");

	ClassDB::bind_method(D_METHOD("get_animation_frame_coords"), &Prop2DDataSprite::get_animation_frame_coords);
	ClassDB::bind_method(D_METHOD("set_animation_frame_coords", "value"), &Prop2DDataSprite::set_animation_frame_coords);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "animation_frame_coords"), "set_animation_frame_coords", "get_animation_frame_coords");

	ClassDB::bind_method(D_METHOD("get_region_enabled"), &Prop2DDataSprite::get_region_enabled);
	ClassDB::bind_method(D_METHOD("set_region_enabled", "value"), &Prop2DDataSprite::set_region_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "region_enabled"), "set_region_enabled", "get_region_enabled");

	ClassDB::bind_method(D_METHOD("get_region"), &Prop2DDataSprite::get_region);
	ClassDB::bind_method(D_METHOD("set_region", "value"), &Prop2DDataSprite::set_region);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "region"), "set_region", "get_region");

	ClassDB::bind_method(D_METHOD("get_filter_clip"), &Prop2DDataSprite::get_filter_clip);
	ClassDB::bind_method(D_METHOD("set_filter_clip", "value"), &Prop2DDataSprite::set_filter_clip);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "filter_clip"), "set_filter_clip", "get_filter_clip");
}
