/*************************************************************************/
/*  polygon_2d.cpp                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "paint_polygon_2d.h"

#include "core/math/geometry.h"

#ifdef TOOLS_ENABLED
Dictionary PaintPolygon2D::_edit_get_state() const {
	Dictionary state = Node2D::_edit_get_state();
	state["offset"] = offset;
	return state;
}

void PaintPolygon2D::_edit_set_state(const Dictionary &p_state) {
	Node2D::_edit_set_state(p_state);
	set_offset(p_state["offset"]);
}

void PaintPolygon2D::_edit_set_pivot(const Point2 &p_pivot) {
	set_position(get_transform().xform(p_pivot));
	set_offset(get_offset() - p_pivot);
}

Point2 PaintPolygon2D::_edit_get_pivot() const {
	return Vector2();
}

bool PaintPolygon2D::_edit_use_pivot() const {
	return true;
}

Rect2 PaintPolygon2D::_edit_get_rect() const {
	if (rect_cache_dirty) {
		int l = polygon.size();
		PoolVector<Vector2>::Read r = polygon.read();
		item_rect = Rect2();
		for (int i = 0; i < l; i++) {
			Vector2 pos = r[i] + offset;
			if (i == 0) {
				item_rect.position = pos;
			} else {
				item_rect.expand_to(pos);
			}
		}
		rect_cache_dirty = false;
	}

	return item_rect;
}

bool PaintPolygon2D::_edit_use_rect() const {
	return polygon.size() > 0;
}

bool PaintPolygon2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	Vector<Vector2> polygon2d = Variant(polygon);
	if (internal_vertices > 0) {
		polygon2d.resize(polygon2d.size() - internal_vertices);
	}
	return Geometry::is_point_in_polygon(p_point - get_offset(), polygon2d);
}
#endif

void PaintPolygon2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			if (polygon.size() < 3) {
				return;
			}

			Vector<Vector2> points;
			Vector<Vector2> uvs;
			Vector<int> bones;
			Vector<float> weights;

			int len = polygon.size();
			if ((invert || polygons.size() == 0) && internal_vertices > 0) {
				//if no polygons are around, internal vertices must not be drawn, else let them be
				len -= internal_vertices;
			}

			if (len <= 0) {
				return;
			}
			points.resize(len);

			{
				PoolVector<Vector2>::Read polyr = polygon.read();
				for (int i = 0; i < len; i++) {
					points.write[i] = polyr[i] + offset;
				}
			}

			if (invert) {
				Rect2 bounds;
				int highest_idx = -1;
				float highest_y = -1e20;
				float sum = 0;

				for (int i = 0; i < len; i++) {
					if (i == 0) {
						bounds.position = points[i];
					} else {
						bounds.expand_to(points[i]);
					}
					if (points[i].y > highest_y) {
						highest_idx = i;
						highest_y = points[i].y;
					}
					int ni = (i + 1) % len;
					sum += (points[ni].x - points[i].x) * (points[ni].y + points[i].y);
				}

				bounds = bounds.grow(invert_border);

				Vector2 ep[7] = {
					Vector2(points[highest_idx].x, points[highest_idx].y + invert_border),
					Vector2(bounds.position + bounds.size),
					Vector2(bounds.position + Vector2(bounds.size.x, 0)),
					Vector2(bounds.position),
					Vector2(bounds.position + Vector2(0, bounds.size.y)),
					Vector2(points[highest_idx].x - CMP_EPSILON, points[highest_idx].y + invert_border),
					Vector2(points[highest_idx].x - CMP_EPSILON, points[highest_idx].y),
				};

				if (sum > 0) {
					SWAP(ep[1], ep[4]);
					SWAP(ep[2], ep[3]);
					SWAP(ep[5], ep[0]);
					SWAP(ep[6], points.write[highest_idx]);
				}

				points.resize(points.size() + 7);
				for (int i = points.size() - 1; i >= highest_idx + 7; i--) {
					points.write[i] = points[i - 7];
				}

				for (int i = 0; i < 7; i++) {
					points.write[highest_idx + i + 1] = ep[i];
				}

				len = points.size();
			}

			if (texture.is_valid()) {
				Transform2D texmat(tex_rot, tex_ofs);
				texmat.scale(tex_scale);
				Size2 tex_size = texture->get_size();

				uvs.resize(len);

				if (points.size() == uv.size()) {
					PoolVector<Vector2>::Read uvr = uv.read();

					for (int i = 0; i < len; i++) {
						uvs.write[i] = texmat.xform(uvr[i]) / tex_size;
					}

				} else {
					for (int i = 0; i < len; i++) {
						uvs.write[i] = texmat.xform(points[i]) / tex_size;
					}
				}
			}

			Vector<Color> colors;
			if (vertex_colors.size() == points.size()) {
				colors.resize(len);
				PoolVector<Color>::Read color_r = vertex_colors.read();
				for (int i = 0; i < len; i++) {
					colors.write[i] = color_r[i];
				}
			} else {
				colors.push_back(color);
			}

			//			Vector<int> indices = Geometry::triangulate_polygon(points);
			//			RS::get_singleton()->canvas_item_add_triangle_array(get_canvas_item(), indices, points, colors, uvs, texture.is_valid() ? texture->get_rid() : RID());

			if (invert || polygons.size() == 0) {
				Vector<int> indices = Geometry::triangulate_polygon(points);
				if (indices.size()) {
					RS::get_singleton()->canvas_item_add_triangle_array(get_canvas_item(), indices, points, colors, uvs, bones, weights, texture.is_valid() ? texture->get_rid() : RID(), -1, RID(), antialiased);
				}
			} else {
				//draw individual polygons
				Vector<int> total_indices;
				for (int i = 0; i < polygons.size(); i++) {
					PoolVector<int> src_indices = polygons[i];
					int ic = src_indices.size();
					if (ic < 3) {
						continue;
					}
					PoolVector<int>::Read r = src_indices.read();

					Vector<Vector2> tmp_points;
					tmp_points.resize(ic);

					for (int j = 0; j < ic; j++) {
						int idx = r[j];
						ERR_CONTINUE(idx < 0 || idx >= points.size());
						tmp_points.write[j] = points[r[j]];
					}
					Vector<int> indices = Geometry::triangulate_polygon(tmp_points);
					int ic2 = indices.size();
					const int *r2 = indices.ptr();

					int bic = total_indices.size();
					total_indices.resize(bic + ic2);
					int *w2 = total_indices.ptrw();

					for (int j = 0; j < ic2; j++) {
						w2[j + bic] = r[r2[j]];
					}
				}

				if (total_indices.size()) {
					RS::get_singleton()->canvas_item_add_triangle_array(get_canvas_item(), total_indices, points, colors, uvs, bones, weights, texture.is_valid() ? texture->get_rid() : RID(), -1, RID(), antialiased);
				}
			}

		} break;
	}
}

void PaintPolygon2D::set_polygon(const PoolVector<Vector2> &p_polygon) {
	polygon = p_polygon;
	rect_cache_dirty = true;
	update();
}

PoolVector<Vector2> PaintPolygon2D::get_polygon() const {
	return polygon;
}

void PaintPolygon2D::set_internal_vertex_count(int p_count) {
	internal_vertices = p_count;
}

int PaintPolygon2D::get_internal_vertex_count() const {
	return internal_vertices;
}

void PaintPolygon2D::set_uv(const PoolVector<Vector2> &p_uv) {
	uv = p_uv;
	update();
}

PoolVector<Vector2> PaintPolygon2D::get_uv() const {
	return uv;
}

void PaintPolygon2D::set_polygons(const Array &p_polygons) {
	polygons = p_polygons;
	update();
}

Array PaintPolygon2D::get_polygons() const {
	return polygons;
}

void PaintPolygon2D::set_color(const Color &p_color) {
	color = p_color;
	update();
}
Color PaintPolygon2D::get_color() const {
	return color;
}

void PaintPolygon2D::set_vertex_colors(const PoolVector<Color> &p_colors) {
	vertex_colors = p_colors;
	update();
}
PoolVector<Color> PaintPolygon2D::get_vertex_colors() const {
	return vertex_colors;
}

void PaintPolygon2D::set_texture(const Ref<Texture> &p_texture) {
	texture = p_texture;

	/*if (texture.is_valid()) {
		uint32_t flags=texture->get_flags();
		flags&=~Texture::FLAG_REPEAT;
		if (tex_tile)
			flags|=Texture::FLAG_REPEAT;

		texture->set_flags(flags);
	}*/
	update();
}
Ref<Texture> PaintPolygon2D::get_texture() const {
	return texture;
}

void PaintPolygon2D::set_texture_offset(const Vector2 &p_offset) {
	tex_ofs = p_offset;
	update();
}
Vector2 PaintPolygon2D::get_texture_offset() const {
	return tex_ofs;
}

void PaintPolygon2D::set_texture_rotation(float p_rot) {
	tex_rot = p_rot;
	update();
}
float PaintPolygon2D::get_texture_rotation() const {
	return tex_rot;
}

void PaintPolygon2D::set_texture_rotation_degrees(float p_rot) {
	set_texture_rotation(Math::deg2rad(p_rot));
}
float PaintPolygon2D::get_texture_rotation_degrees() const {
	return Math::rad2deg(get_texture_rotation());
}

void PaintPolygon2D::set_texture_scale(const Size2 &p_scale) {
	tex_scale = p_scale;
	update();
}
Size2 PaintPolygon2D::get_texture_scale() const {
	return tex_scale;
}

void PaintPolygon2D::set_invert(bool p_invert) {
	invert = p_invert;
	update();
}
bool PaintPolygon2D::get_invert() const {
	return invert;
}

void PaintPolygon2D::set_antialiased(bool p_antialiased) {
	antialiased = p_antialiased;
	update();
}
bool PaintPolygon2D::get_antialiased() const {
	return antialiased;
}

void PaintPolygon2D::set_invert_border(float p_invert_border) {
	invert_border = p_invert_border;
	update();
}
float PaintPolygon2D::get_invert_border() const {
	return invert_border;
}

void PaintPolygon2D::set_offset(const Vector2 &p_offset) {
	offset = p_offset;
	rect_cache_dirty = true;
	update();
	_change_notify("offset");
}

Vector2 PaintPolygon2D::get_offset() const {
	return offset;
}

void PaintPolygon2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_polygon", "polygon"), &PaintPolygon2D::set_polygon);
	ClassDB::bind_method(D_METHOD("get_polygon"), &PaintPolygon2D::get_polygon);

	ClassDB::bind_method(D_METHOD("set_uv", "uv"), &PaintPolygon2D::set_uv);
	ClassDB::bind_method(D_METHOD("get_uv"), &PaintPolygon2D::get_uv);

	ClassDB::bind_method(D_METHOD("set_color", "color"), &PaintPolygon2D::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &PaintPolygon2D::get_color);

	ClassDB::bind_method(D_METHOD("set_polygons", "polygons"), &PaintPolygon2D::set_polygons);
	ClassDB::bind_method(D_METHOD("get_polygons"), &PaintPolygon2D::get_polygons);

	ClassDB::bind_method(D_METHOD("set_vertex_colors", "vertex_colors"), &PaintPolygon2D::set_vertex_colors);
	ClassDB::bind_method(D_METHOD("get_vertex_colors"), &PaintPolygon2D::get_vertex_colors);

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &PaintPolygon2D::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &PaintPolygon2D::get_texture);

	ClassDB::bind_method(D_METHOD("set_texture_offset", "texture_offset"), &PaintPolygon2D::set_texture_offset);
	ClassDB::bind_method(D_METHOD("get_texture_offset"), &PaintPolygon2D::get_texture_offset);

	ClassDB::bind_method(D_METHOD("set_texture_rotation", "texture_rotation"), &PaintPolygon2D::set_texture_rotation);
	ClassDB::bind_method(D_METHOD("get_texture_rotation"), &PaintPolygon2D::get_texture_rotation);

	ClassDB::bind_method(D_METHOD("set_texture_rotation_degrees", "texture_rotation"), &PaintPolygon2D::set_texture_rotation_degrees);
	ClassDB::bind_method(D_METHOD("get_texture_rotation_degrees"), &PaintPolygon2D::get_texture_rotation_degrees);

	ClassDB::bind_method(D_METHOD("set_texture_scale", "texture_scale"), &PaintPolygon2D::set_texture_scale);
	ClassDB::bind_method(D_METHOD("get_texture_scale"), &PaintPolygon2D::get_texture_scale);

	ClassDB::bind_method(D_METHOD("set_invert", "invert"), &PaintPolygon2D::set_invert);
	ClassDB::bind_method(D_METHOD("get_invert"), &PaintPolygon2D::get_invert);

	ClassDB::bind_method(D_METHOD("set_antialiased", "antialiased"), &PaintPolygon2D::set_antialiased);
	ClassDB::bind_method(D_METHOD("get_antialiased"), &PaintPolygon2D::get_antialiased);

	ClassDB::bind_method(D_METHOD("set_invert_border", "invert_border"), &PaintPolygon2D::set_invert_border);
	ClassDB::bind_method(D_METHOD("get_invert_border"), &PaintPolygon2D::get_invert_border);

	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &PaintPolygon2D::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &PaintPolygon2D::get_offset);

	ClassDB::bind_method(D_METHOD("set_internal_vertex_count", "internal_vertex_count"), &PaintPolygon2D::set_internal_vertex_count);
	ClassDB::bind_method(D_METHOD("get_internal_vertex_count"), &PaintPolygon2D::get_internal_vertex_count);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "antialiased"), "set_antialiased", "get_antialiased");
	ADD_GROUP("Texture", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_GROUP("Texture", "texture_");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "texture_offset"), "set_texture_offset", "get_texture_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "texture_scale", PROPERTY_HINT_LINK), "set_texture_scale", "get_texture_scale");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "texture_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater"), "set_texture_rotation_degrees", "get_texture_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "texture_rotation", PROPERTY_HINT_NONE, "", 0), "set_texture_rotation", "get_texture_rotation");

	ADD_GROUP("Invert", "invert_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "invert_enable"), "set_invert", "get_invert");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "invert_border", PROPERTY_HINT_RANGE, "0.1,16384,0.1"), "set_invert_border", "get_invert_border");

	ADD_GROUP("Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "polygon"), "set_polygon", "get_polygon");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "uv"), "set_uv", "get_uv");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_COLOR_ARRAY, "vertex_colors"), "set_vertex_colors", "get_vertex_colors");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "polygons"), "set_polygons", "get_polygons");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_vertex_count", PROPERTY_HINT_RANGE, "0,1000"), "set_internal_vertex_count", "get_internal_vertex_count");
}

PaintPolygon2D::PaintPolygon2D() {
	invert = false;
	invert_border = 100;
	antialiased = false;
	tex_rot = 0;
	tex_tile = true;
	tex_scale = Vector2(1, 1);
	color = Color(1, 1, 1);
	rect_cache_dirty = true;
	internal_vertices = 0;
}

PaintPolygon2D::~PaintPolygon2D() {
}