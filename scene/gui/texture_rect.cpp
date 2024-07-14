/*************************************************************************/
/*  texture_rect.cpp                                                     */
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

#include "texture_rect.h"
#include "core/core_string_names.h"
#include "servers/rendering_server.h"

void TextureRect::_notification(int p_what) {
	if (p_what == NOTIFICATION_DRAW) {
		if (texture.is_null()) {
			return;
		}

		Size2 size;
		Point2 offset;
		Rect2 region;
		bool tile = false;

		switch (stretch_mode) {
			case STRETCH_SCALE_ON_EXPAND: {
				size = expand ? get_size() : texture->get_size();
			} break;
			case STRETCH_SCALE: {
				size = get_size();
			} break;
			case STRETCH_TILE: {
				size = get_size();
				tile = true;
			} break;
			case STRETCH_KEEP: {
				size = texture->get_size();
			} break;
			case STRETCH_KEEP_CENTERED: {
				offset = (get_size() - texture->get_size()) / 2;
				size = texture->get_size();
			} break;
			case STRETCH_KEEP_ASPECT_CENTERED:
			case STRETCH_KEEP_ASPECT: {
				size = get_size();
				int tex_width = texture->get_width() * size.height / texture->get_height();
				int tex_height = size.height;

				if (tex_width > size.width) {
					tex_width = size.width;
					tex_height = texture->get_height() * tex_width / texture->get_width();
				}

				if (stretch_mode == STRETCH_KEEP_ASPECT_CENTERED) {
					offset.x += (size.width - tex_width) / 2;
					offset.y += (size.height - tex_height) / 2;
				}

				size.width = tex_width;
				size.height = tex_height;
			} break;
			case STRETCH_KEEP_ASPECT_COVERED: {
				size = get_size();

				Size2 tex_size = texture->get_size();
				Size2 scale_size(size.width / tex_size.width, size.height / tex_size.height);
				float scale = scale_size.width > scale_size.height ? scale_size.width : scale_size.height;
				Size2 scaled_tex_size = tex_size * scale;

				region.position = ((scaled_tex_size - size) / scale).abs() / 2.0f;
				region.size = size / scale;
			} break;
		}

		size.width *= hflip ? -1.0f : 1.0f;
		size.height *= vflip ? -1.0f : 1.0f;

		if (region.has_no_area()) {
			draw_texture_rect(texture, Rect2(offset, size), tile);
		} else {
			draw_texture_rect_region(texture, Rect2(offset, size), region);
		}
	}
}

Size2 TextureRect::get_minimum_size() const {
	if (!expand && !texture.is_null()) {
		return texture->get_size();
	} else {
		return Size2();
	}
}

void TextureRect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &TextureRect::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &TextureRect::get_texture);
	ClassDB::bind_method(D_METHOD("set_expand", "enable"), &TextureRect::set_expand);
	ClassDB::bind_method(D_METHOD("has_expand"), &TextureRect::has_expand);
	ClassDB::bind_method(D_METHOD("set_flip_h", "enable"), &TextureRect::set_flip_h);
	ClassDB::bind_method(D_METHOD("is_flipped_h"), &TextureRect::is_flipped_h);
	ClassDB::bind_method(D_METHOD("set_flip_v", "enable"), &TextureRect::set_flip_v);
	ClassDB::bind_method(D_METHOD("is_flipped_v"), &TextureRect::is_flipped_v);
	ClassDB::bind_method(D_METHOD("set_stretch_mode", "stretch_mode"), &TextureRect::set_stretch_mode);
	ClassDB::bind_method(D_METHOD("get_stretch_mode"), &TextureRect::get_stretch_mode);
	ClassDB::bind_method(D_METHOD("_texture_changed"), &TextureRect::_texture_changed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "expand"), "set_expand", "has_expand");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stretch_mode", PROPERTY_HINT_ENUM, "Scale On Expand (Compat),Scale,Tile,Keep,Keep Centered,Keep Aspect,Keep Aspect Centered,Keep Aspect Covered"), "set_stretch_mode", "get_stretch_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "is_flipped_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "is_flipped_v");

	BIND_ENUM_CONSTANT(STRETCH_SCALE_ON_EXPAND);
	BIND_ENUM_CONSTANT(STRETCH_SCALE);
	BIND_ENUM_CONSTANT(STRETCH_TILE);
	BIND_ENUM_CONSTANT(STRETCH_KEEP);
	BIND_ENUM_CONSTANT(STRETCH_KEEP_CENTERED);
	BIND_ENUM_CONSTANT(STRETCH_KEEP_ASPECT);
	BIND_ENUM_CONSTANT(STRETCH_KEEP_ASPECT_CENTERED);
	BIND_ENUM_CONSTANT(STRETCH_KEEP_ASPECT_COVERED);
}

void TextureRect::_texture_changed() {
	if (texture.is_valid()) {
		update();
		minimum_size_changed();
	}
}

void TextureRect::set_texture(const Ref<Texture> &p_tex) {
	if (p_tex == texture) {
		return;
	}

	if (texture.is_valid()) {
		texture->disconnect(CoreStringNames::get_singleton()->changed, this, "_texture_changed");
	}

	texture = p_tex;

	if (texture.is_valid()) {
		texture->connect(CoreStringNames::get_singleton()->changed, this, "_texture_changed");
	}

	update();
	minimum_size_changed();
}

Ref<Texture> TextureRect::get_texture() const {
	return texture;
}

void TextureRect::set_expand(bool p_expand) {
	expand = p_expand;
	update();
	minimum_size_changed();
}

bool TextureRect::has_expand() const {
	return expand;
}

void TextureRect::set_stretch_mode(StretchMode p_mode) {
	stretch_mode = p_mode;
	update();
}

TextureRect::StretchMode TextureRect::get_stretch_mode() const {
	return stretch_mode;
}

void TextureRect::set_flip_h(bool p_flip) {
	hflip = p_flip;
	update();
}

bool TextureRect::is_flipped_h() const {
	return hflip;
}

void TextureRect::set_flip_v(bool p_flip) {
	vflip = p_flip;
	update();
}

bool TextureRect::is_flipped_v() const {
	return vflip;
}

TextureRect::TextureRect() {
	expand = false;
	hflip = false;
	vflip = false;
	set_mouse_filter(MOUSE_FILTER_PASS);
	stretch_mode = STRETCH_SCALE_ON_EXPAND;
}

TextureRect::~TextureRect() {
}
