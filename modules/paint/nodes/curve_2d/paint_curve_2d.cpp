/*************************************************************************/
/*  path_2d.cpp                                                          */
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

#include "paint_curve_2d.h"

#include "core/config/engine.h"
#include "scene/resources/curve.h"
#include "scene/scene_string_names.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_scale.h"
#endif

#ifdef TOOLS_ENABLED
Rect2 PaintCurve2D::_edit_get_rect() const {
	if (rect_cache_dirty) {
		if (!curve.is_valid() || curve->get_point_count() == 0) {
			return Rect2(0, 0, 0, 0);
		}

		item_rect = Rect2(curve->get_point_position(0), Vector2(0, 0));

		for (int i = 0; i < curve->get_point_count(); i++) {
			for (int j = 0; j <= 8; j++) {
				real_t frac = j / 8.0;
				Vector2 p = curve->interpolate(i, frac);
				item_rect.expand_to(p);
			}
		}

		rect_cache_dirty = false;
	}

	return item_rect;
}

bool PaintCurve2D::_edit_use_rect() const {
	return curve.is_valid() && curve->get_point_count() != 0;
}

bool PaintCurve2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	if (curve.is_null()) {
		return false;
	}

	for (int i = 0; i < curve->get_point_count(); i++) {
		Vector2 s[2];
		s[0] = curve->get_point_position(i);

		for (int j = 1; j <= 8; j++) {
			real_t frac = j / 8.0;
			s[1] = curve->interpolate(i, frac);

			Vector2 p = Geometry::get_closest_point_to_segment_2d(p_point, s);
			if (p.distance_to(p_point) <= p_tolerance) {
				return true;
			}

			s[0] = s[1];
		}
	}

	return false;
}
#endif

void PaintCurve2D::_notification(int p_what) {
	if (p_what == NOTIFICATION_DRAW && curve.is_valid()) {
		//draw the curve!!

		_rendered_image.unref();

		if (curve->get_point_count() < 2) {
			return;
		}

		if (_fill_enabled) {
			Vector<Vector2> points;
			Vector<Vector2> uvs;
			Vector<Color> colors;
			Vector<int> indices;

			_prepare_render_data_fill(points, uvs, colors, indices);

			if (indices.size()) {
				RS::get_singleton()->canvas_item_add_triangle_array(get_canvas_item(), indices, points, colors, uvs, Vector<int>(), Vector<float>(), _fill_texture.is_valid() ? _fill_texture->get_rid() : RID(), -1, RID(), true);
			}
		}

		if (_outline_enabled) {
			_cached_draw_pts = Variant(curve->tessellate());

			if (_outline_texture.is_valid()) {
				Vector<Vector2> points;
				Vector<Vector2> uvs;
				Vector<Color> colors;
				Vector<int> indices;

				_prepare_render_data_outline(points, uvs, colors, indices);

				if (indices.size()) {
					RS::get_singleton()->canvas_item_add_triangle_array(get_canvas_item(), indices, points, colors, uvs, Vector<int>(), Vector<float>(), _outline_texture.is_valid() ? _outline_texture->get_rid() : RID(), -1, RID(), true);
				}
			} else {
				draw_polyline(_cached_draw_pts, _outline_color, _outline_width, true);
			}
		}
	}
}

void PaintCurve2D::_curve_changed() {
	if (!is_inside_tree()) {
		return;
	}

	rect_cache_dirty = true;

	update();
}

void PaintCurve2D::set_curve(const Ref<Curve2D> &p_curve) {
	if (curve.is_valid()) {
		curve->disconnect("changed", this, "_curve_changed");
	}

	curve = p_curve;

	if (curve.is_valid()) {
		curve->connect("changed", this, "_curve_changed");
	}

	_curve_changed();
}

Ref<Curve2D> PaintCurve2D::get_curve() const {
	return curve;
}

void PaintCurve2D::fill_set_enabled(bool p_enabled) {
	_fill_enabled = p_enabled;
	update();
}
bool PaintCurve2D::fill_get_enabled() const {
	return _fill_enabled;
}

void PaintCurve2D::fill_set_color(const Color &p_color) {
	_fill_color = p_color;
	update();
}
Color PaintCurve2D::fill_get_color() const {
	return _fill_color;
}

void PaintCurve2D::fill_set_texture(const Ref<Texture> &p_texture) {
	_fill_texture = p_texture;

	update();
}
Ref<Texture> PaintCurve2D::fill_get_texture() const {
	return _fill_texture;
}

void PaintCurve2D::fill_set_texture_offset(const Vector2 &p_offset) {
	_fill_tex_ofs = p_offset;
	update();
}
Vector2 PaintCurve2D::fill_get_texture_offset() const {
	return _fill_tex_ofs;
}

void PaintCurve2D::fill_set_texture_rotation(float p_rot) {
	_fill_tex_rot = p_rot;
	update();
}
float PaintCurve2D::fill_get_texture_rotation() const {
	return _fill_tex_rot;
}

void PaintCurve2D::fill_set_texture_rotation_degrees(float p_rot) {
	fill_set_texture_rotation(Math::deg2rad(p_rot));
}
float PaintCurve2D::fill_get_texture_rotation_degrees() const {
	return Math::rad2deg(fill_get_texture_rotation());
}

void PaintCurve2D::fill_set_texture_scale(const Size2 &p_scale) {
	_fill_tex_scale = p_scale;
	update();
}
Size2 PaintCurve2D::fill_get_texture_scale() const {
	return _fill_tex_scale;
}

void PaintCurve2D::fill_set_invert(bool p_invert) {
	_fill_invert = p_invert;
	update();
}
bool PaintCurve2D::fill_get_invert() const {
	return _fill_invert;
}

void PaintCurve2D::fill_set_invert_border(float p_invert_border) {
	_fill_invert_border = p_invert_border;
	update();
}
float PaintCurve2D::fill_get_invert_border() const {
	return _fill_invert_border;
}

void PaintCurve2D::outline_set_enabled(bool p_enabled) {
	_outline_enabled = p_enabled;
	update();
}
bool PaintCurve2D::outline_get_enabled() const {
	return _outline_enabled;
}

void PaintCurve2D::outline_set_width(int p_width) {
	_outline_width = p_width;
	update();
}
int PaintCurve2D::outline_get_width() const {
	return _outline_width;
}

void PaintCurve2D::outline_set_color(const Color &p_color) {
	_outline_color = p_color;
	update();
}
Color PaintCurve2D::outline_get_color() const {
	return _outline_color;
}

void PaintCurve2D::outline_set_texture(const Ref<Texture> &p_texture) {
	_outline_texture = p_texture;

	update();
}
Ref<Texture> PaintCurve2D::outline_get_texture() const {
	return _outline_texture;
}

void PaintCurve2D::outline_set_texture_offset(const Vector2 &p_offset) {
	_outline_tex_ofs = p_offset;
	update();
}
Vector2 PaintCurve2D::outline_get_texture_offset() const {
	return _outline_tex_ofs;
}

void PaintCurve2D::outline_set_texture_rotation(float p_rot) {
	_outline_tex_rot = p_rot;
	update();
}
float PaintCurve2D::outline_get_texture_rotation() const {
	return _outline_tex_rot;
}

void PaintCurve2D::outline_set_texture_rotation_degrees(float p_rot) {
	outline_set_texture_rotation(Math::deg2rad(p_rot));
}
float PaintCurve2D::outline_get_texture_rotation_degrees() const {
	return Math::rad2deg(outline_get_texture_rotation());
}

void PaintCurve2D::outline_set_texture_scale(const Size2 &p_scale) {
	_outline_tex_scale = p_scale;
	update();
}
Size2 PaintCurve2D::outline_get_texture_scale() const {
	return _outline_tex_scale;
}

Ref<Image> PaintCurve2D::_get_rendered_image() {
	if (_rendered_image.is_valid()) {
		return _rendered_image;
	}

	if (_size.x == 0 || _size.y == 0) {
		return Ref<Image>();
	}

	if (!curve.is_valid()) {
		return Ref<Image>();
	}

	if (curve->get_point_count() < 2) {
		return Ref<Image>();
	}

	_rendered_image.instance();
	_rendered_image->create(_size.x, _size.y, false, Image::FORMAT_RGBA8);

	Vector<Vector2> points;
	Vector<Vector2> uvs;
	Vector<Color> colors;
	Vector<int> indices;

	if (_outline_enabled) {
		_prepare_render_data_outline(points, uvs, colors, indices);

		_image_render_triangles(points, uvs, colors, indices, _outline_texture);

		points.clear();
		uvs.clear();
		colors.clear();
		indices.clear();
	}

	if (_fill_enabled) {
		_prepare_render_data_fill(points, uvs, colors, indices);

		_image_render_triangles(points, uvs, colors, indices, _fill_texture);
	}

	return _rendered_image;
}

PoolVector2Array PaintCurve2D::generate_uvs(const Vector<Vector2> &p_points) {
	PoolVector2Array uvs;

	int len = p_points.size();

	if (len == 0) {
		return uvs;
	}

	const Vector2 *pr = p_points.ptr();

	Rect2 bounds = Rect2(pr[0], Vector2());

	for (int i = 1; i < len; i++) {
		Vector2 e = pr[i];

		bounds.expand_to(e);
	}

	uvs.resize(len);

	PoolVector2Array::Write uvr = uvs.write();
	Vector2 *uvp = uvr.ptr();

	for (int i = 0; i < len; i++) {
		Vector2 e = pr[i];

		e -= bounds.position;
		e /= bounds.size;

		uvp[i] = e;
	}

	return uvs;
}

PoolVector2Array PaintCurve2D::generate_uvs(const Vector<Vector2> &p_points, const Rect2 &p_uv_rect) {
	PoolVector2Array uvs;

	int len = p_points.size();

	if (len == 0) {
		return uvs;
	}

	const Vector2 *pr = p_points.ptr();

	Rect2 bounds = Rect2(pr[0], Vector2());

	for (int i = 1; i < len; i++) {
		Vector2 e = pr[i];

		bounds.expand_to(e);
	}

	uvs.resize(len);

	PoolVector2Array::Write uvr = uvs.write();
	Vector2 *uvp = uvr.ptr();

	for (int i = 0; i < len; i++) {
		Vector2 e = pr[i];

		e -= bounds.position;
		e /= bounds.size;

		e *= p_uv_rect.size;
		e += p_uv_rect.position;

		uvp[i] = e;
	}

	return uvs;
}

void PaintCurve2D::generate_polyline_mesh(const Vector<Point2> &p_points, float p_width, Vector<Vector2> &r_triangles, Vector<int> &r_indices) {
	Vector2 prev_t;

	if (p_points.size() < 2) {
		return;
	}

	r_triangles.resize(p_points.size() * 2);
	r_indices.resize(p_points.size() * 6);

	for (int i = 0; i < p_points.size(); i++) {
		Vector2 t;
		if (i == p_points.size() - 1) {
			t = prev_t;
		} else {
			t = (p_points[i + 1] - p_points[i]).normalized().tangent();
			if (i == 0) {
				prev_t = t;
			}
		}

		Vector2 tangent = ((t + prev_t).normalized()) * p_width * 0.5;

		r_triangles.write[i * 2 + 0] = p_points[i] + tangent;
		r_triangles.write[i * 2 + 1] = p_points[i] - tangent;

		if (i != 0) {
			int indx = (i - 1) * 2;
			int iindx = (i - 1) * 6;

			r_indices.write[iindx] = indx;
			r_indices.write[iindx + 1] = indx + 1;
			r_indices.write[iindx + 2] = indx + 3;

			r_indices.write[iindx + 3] = indx;
			r_indices.write[iindx + 4] = indx + 3;
			r_indices.write[iindx + 5] = indx + 2;
		}

		prev_t = t;
	}

	int indx = (p_points.size() - 1) * 2;
	int iindx = (p_points.size() - 1) * 6;

	r_indices.write[iindx] = indx;
	r_indices.write[iindx + 1] = indx + 1;
	r_indices.write[iindx + 2] = 1;

	r_indices.write[iindx + 3] = indx;
	r_indices.write[iindx + 4] = 1;
	r_indices.write[iindx + 5] = 0;
}

void PaintCurve2D::_prepare_render_data_fill(Vector<Vector2> &r_points, Vector<Vector2> &r_uvs, Vector<Color> &r_colors, Vector<int> &r_indices) {
	if (!curve.is_valid()) {
		return;
	}

	int curve_len = curve->get_point_count();

	if (curve_len < 2) {
		return;
	}

	PoolVector2Array polygon = curve->tessellate();
	int len = polygon.size();

	r_points.resize(len);

	{
		PoolVector<Vector2>::Read polyr = polygon.read();
		for (int i = 0; i < len; i++) {
			r_points.write[i] = polyr[i];
		}
	}

	if (_fill_invert) {
		Rect2 bounds;
		int highest_idx = -1;
		float highest_y = -1e20;
		float sum = 0;

		for (int i = 0; i < len; i++) {
			if (i == 0) {
				bounds.position = r_points[i];
			} else {
				bounds.expand_to(r_points[i]);
			}
			if (r_points[i].y > highest_y) {
				highest_idx = i;
				highest_y = r_points[i].y;
			}
			int ni = (i + 1) % len;
			sum += (r_points[ni].x - r_points[i].x) * (r_points[ni].y + r_points[i].y);
		}

		bounds = bounds.grow(_fill_invert_border);

		Vector2 ep[7] = {
			Vector2(r_points[highest_idx].x, r_points[highest_idx].y + _fill_invert_border),
			Vector2(bounds.position + bounds.size),
			Vector2(bounds.position + Vector2(bounds.size.x, 0)),
			Vector2(bounds.position),
			Vector2(bounds.position + Vector2(0, bounds.size.y)),
			Vector2(r_points[highest_idx].x - CMP_EPSILON, r_points[highest_idx].y + _fill_invert_border),
			Vector2(r_points[highest_idx].x - CMP_EPSILON, r_points[highest_idx].y),
		};

		if (sum > 0) {
			SWAP(ep[1], ep[4]);
			SWAP(ep[2], ep[3]);
			SWAP(ep[5], ep[0]);
			SWAP(ep[6], r_points.write[highest_idx]);
		}

		r_points.resize(r_points.size() + 7);
		for (int i = r_points.size() - 1; i >= highest_idx + 7; i--) {
			r_points.write[i] = r_points[i - 7];
		}

		for (int i = 0; i < 7; i++) {
			r_points.write[highest_idx + i + 1] = ep[i];
		}

		len = r_points.size();
	}

	if (_fill_texture.is_valid()) {
		Transform2D texmat(_fill_tex_rot, _fill_tex_ofs);
		texmat.scale(_fill_tex_scale);
		//Size2 tex_size = _fill_texture->get_size();

		PoolVector2Array uv = generate_uvs(r_points);

		r_uvs.resize(len);

		PoolVector<Vector2>::Read uvr = uv.read();

		for (int i = 0; i < len; i++) {
			r_uvs.write[i] = texmat.xform(uvr[i]);
		}
	}

	r_colors.push_back(_fill_color);

	r_indices = Geometry::triangulate_polygon(r_points);
}

void PaintCurve2D::_prepare_render_data_outline(Vector<Vector2> &r_points, Vector<Vector2> &r_uvs, Vector<Color> &r_colors, Vector<int> &r_indices) {
	if (!curve.is_valid()) {
		return;
	}

	if (_outline_width <= 0) {
		return;
	}

	int curve_len = curve->get_point_count();

	if (curve_len < 2) {
		return;
	}

	PoolVector2Array polygon = curve->tessellate();
	int len = polygon.size();

	Vector<Vector2> curve_points;
	curve_points.resize(len);

	{
		PoolVector<Vector2>::Read polyr = polygon.read();
		for (int i = 0; i < len; i++) {
			curve_points.write[i] = polyr[i];
		}
	}

	generate_polyline_mesh(curve_points, _outline_width, r_points, r_indices);

	len = r_points.size();

	if (_outline_texture.is_valid()) {
		Transform2D texmat(_outline_tex_rot, _outline_tex_ofs);
		texmat.scale(_outline_tex_scale);
		//Size2 tex_size = _outline_texture->get_size();

		PoolVector2Array uv = generate_uvs(r_points);

		r_uvs.resize(len);

		PoolVector<Vector2>::Read uvr = uv.read();

		for (int i = 0; i < len; i++) {
			r_uvs.write[i] = texmat.xform(uvr[i]);
		}
	}

	r_colors.push_back(_outline_color);
}

void PaintCurve2D::_image_render_triangles(const Vector<Vector2> &p_points, const Vector<Vector2> &p_uvs, const Vector<Color> &p_colors, const Vector<int> &p_indices, const Ref<Texture> &p_texture) {
	if (p_indices.size() == 0) {
		return;
	}

	Ref<Image> texture_image;
	Vector2 texture_image_size;

	if (p_uvs.size() > 0 && p_texture.is_valid()) {
		texture_image = p_texture->get_data();
	}

	bool use_uvs = texture_image.is_valid();

	if (use_uvs) {
		texture_image_size = texture_image->get_size();
	}

	_rendered_image->lock();

	if (use_uvs) {
		texture_image->lock();
	}

	Vector2i cpoints[3];
	Vector2 cuvs[3];
	Color ccolors[3];

	if (p_colors.size() == 1) {
		for (int j = 0; j < 3; ++j) {
			ccolors[j] = p_colors[0];
		}
	}

	//Note: Don't worry about the Node's Transform here, that will get applied automatically in the caller

	for (int index = 0; index < p_indices.size(); index += 3) {
		// Rasterize triangle
		// Based on https://www.youtube.com/watch?v=PahbNFypubE

		for (int i = 0; i < 3; ++i) {
			int cind = p_indices[index + i];

			ERR_FAIL_INDEX(cind, p_points.size());

			cpoints[i] = p_points[cind].round();

			if (p_colors.size() > 1) {
				ccolors[i] = p_colors[cind];
			}

			if (use_uvs) {
				cuvs[i] = p_uvs[cind];
			}
		}

		//Sort them

		if (cpoints[1].y < cpoints[0].y || (cpoints[1].y == cpoints[0].y && cpoints[1].x < cpoints[0].x)) {
			SWAP(cpoints[0], cpoints[1]);
			SWAP(ccolors[0], ccolors[1]);
			SWAP(cuvs[0], cuvs[1]);
		}

		if (cpoints[2].y < cpoints[0].y || (cpoints[2].y == cpoints[0].y && cpoints[2].x < cpoints[0].x)) {
			SWAP(cpoints[0], cpoints[2]);
			SWAP(ccolors[0], ccolors[2]);
			SWAP(cuvs[0], cuvs[2]);
		}

		if (cpoints[2].y < cpoints[1].y || (cpoints[2].y == cpoints[1].y && cpoints[2].x < cpoints[1].x)) {
			SWAP(cpoints[1], cpoints[2]);
			SWAP(ccolors[1], ccolors[2]);
			SWAP(cuvs[1], cuvs[2]);
		}

		if (cpoints[0].y == cpoints[2].y) {
			continue;
		}

		bool shortside = (cpoints[1].y - cpoints[0].y) * (cpoints[2].x - cpoints[0].x) < (cpoints[1].x - cpoints[0].x) * (cpoints[2].y - cpoints[0].y);

		Slope sides[2];

		sides[!shortside].setup_position(cpoints[0], cpoints[2], cpoints[2].y - cpoints[0].y);
		sides[!shortside].setup_color(ccolors[0], ccolors[2], cpoints[2].y - cpoints[0].y);

		if (use_uvs) {
			sides[!shortside].setup_uv(cuvs[0], cuvs[2], cpoints[2].y - cpoints[0].y);
		}

		if (cpoints[0].y < cpoints[1].y) {
			sides[shortside].setup_position(cpoints[0], cpoints[1], cpoints[1].y - cpoints[0].y);
			sides[shortside].setup_color(ccolors[0], ccolors[1], cpoints[1].y - cpoints[0].y);

			if (use_uvs) {
				sides[shortside].setup_uv(cuvs[0], cuvs[1], cpoints[1].y - cpoints[0].y);
			}

			int starty = MAX(0, cpoints[0].y);
			int endy = MIN(cpoints[1].y, _size.y);

			for (int y = starty; y < endy; ++y) {
				Slope s;
				s.setup_color(sides[0].color_current, sides[1].color_current, sides[1].position_current.x - sides[0].position_current.x);
				if (use_uvs) {
					s.setup_uv(sides[0].uv_current, sides[1].uv_current, sides[1].position_current.x - sides[0].position_current.x);
				}

				int startx = MAX(0, sides[0].position_current.x);
				int endx = MIN(sides[1].position_current.x, _size.x);

				for (int x = startx; x < endx; ++x) {
					Color color = s.color_current;

					if (use_uvs) {
						Vector2 uv = s.uv_current;

						uv.x = CLAMP(uv.x, 0, 1);
						uv.y = CLAMP(uv.y, 0, 1);

						Vector2 imgcoord = uv * texture_image_size;

						imgcoord.x = CLAMP(imgcoord.x, 0, texture_image_size.x - 1);
						imgcoord.y = CLAMP(imgcoord.y, 0, texture_image_size.y - 1);

						Color img_color = texture_image->get_pixelv(imgcoord);

						color *= img_color;
					}

					_rendered_image->set_pixelv(Vector2(x, y), color);

					s.advance_color();

					if (use_uvs) {
						s.advance_uv();
					}
				}

				sides[0].advance();
				sides[1].advance();

				if (use_uvs) {
					sides[0].advance_uv();
					sides[1].advance_uv();
				}
			}
		}

		if (cpoints[1].y < cpoints[2].y) {
			sides[shortside].setup_position(cpoints[1], cpoints[2], cpoints[2].y - cpoints[1].y);
			sides[shortside].setup_color(ccolors[1], ccolors[2], cpoints[2].y - cpoints[1].y);

			if (use_uvs) {
				sides[shortside].setup_uv(cuvs[1], cuvs[2], cpoints[2].y - cpoints[1].y);
			}

			int starty = MAX(0, cpoints[1].y);
			int endy = MIN(cpoints[2].y, _size.y);

			for (int y = starty; y < endy; ++y) {
				Slope s;
				s.setup_color(sides[0].color_current, sides[1].color_current, sides[1].position_current.x - sides[0].position_current.x);
				if (use_uvs) {
					s.setup_uv(sides[0].uv_current, sides[1].uv_current, sides[1].position_current.x - sides[0].position_current.x);
				}

				int startx = MAX(0, sides[0].position_current.x);
				int endx = MIN(sides[1].position_current.x, _size.x);

				for (int x = startx; x < endx; ++x) {
					Color color = s.color_current;

					if (use_uvs) {
						Vector2 uv = s.uv_current;

						uv.x = CLAMP(uv.x, 0, 1);
						uv.y = CLAMP(uv.y, 0, 1);

						Vector2 imgcoord = uv * texture_image_size;

						imgcoord.x = CLAMP(imgcoord.x, 0, texture_image_size.x - 1);
						imgcoord.y = CLAMP(imgcoord.y, 0, texture_image_size.y - 1);

						Color img_color = texture_image->get_pixelv(imgcoord);

						color *= img_color;
					}

					_rendered_image->set_pixelv(Vector2(x, y), color);

					s.advance_color();

					if (use_uvs) {
						s.advance_uv();
					}
				}

				sides[0].advance();
				sides[1].advance();

				if (use_uvs) {
					sides[0].advance_uv();
					sides[1].advance_uv();
				}
			}
		}
	}

	if (use_uvs) {
		texture_image->unlock();
	}

	_rendered_image->unlock();
}

void PaintCurve2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_curve", "curve"), &PaintCurve2D::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &PaintCurve2D::get_curve);

	ClassDB::bind_method(D_METHOD("fill_set_enabled", "enabled"), &PaintCurve2D::fill_set_enabled);
	ClassDB::bind_method(D_METHOD("fill_get_enabled"), &PaintCurve2D::fill_get_enabled);

	ClassDB::bind_method(D_METHOD("fill_set_color", "color"), &PaintCurve2D::fill_set_color);
	ClassDB::bind_method(D_METHOD("fill_get_color"), &PaintCurve2D::fill_get_color);

	ClassDB::bind_method(D_METHOD("fill_set_texture", "texture"), &PaintCurve2D::fill_set_texture);
	ClassDB::bind_method(D_METHOD("fill_get_texture"), &PaintCurve2D::fill_get_texture);

	ClassDB::bind_method(D_METHOD("fill_set_texture_offset", "texture_offset"), &PaintCurve2D::fill_set_texture_offset);
	ClassDB::bind_method(D_METHOD("fill_get_texture_offset"), &PaintCurve2D::fill_get_texture_offset);

	ClassDB::bind_method(D_METHOD("fill_set_texture_rotation", "texture_rotation"), &PaintCurve2D::fill_set_texture_rotation);
	ClassDB::bind_method(D_METHOD("fill_get_texture_rotation"), &PaintCurve2D::fill_get_texture_rotation);

	ClassDB::bind_method(D_METHOD("fill_set_texture_rotation_degrees", "texture_rotation"), &PaintCurve2D::fill_set_texture_rotation_degrees);
	ClassDB::bind_method(D_METHOD("fill_get_texture_rotation_degrees"), &PaintCurve2D::fill_get_texture_rotation_degrees);

	ClassDB::bind_method(D_METHOD("fill_set_texture_scale", "texture_scale"), &PaintCurve2D::fill_set_texture_scale);
	ClassDB::bind_method(D_METHOD("fill_get_texture_scale"), &PaintCurve2D::fill_get_texture_scale);

	ClassDB::bind_method(D_METHOD("fill_set_invert", "invert"), &PaintCurve2D::fill_set_invert);
	ClassDB::bind_method(D_METHOD("fill_get_invert"), &PaintCurve2D::fill_get_invert);

	ClassDB::bind_method(D_METHOD("fill_set_invert_border", "invert_border"), &PaintCurve2D::fill_set_invert_border);
	ClassDB::bind_method(D_METHOD("fill_get_invert_border"), &PaintCurve2D::fill_get_invert_border);

	ClassDB::bind_method(D_METHOD("outline_set_enabled", "enabled"), &PaintCurve2D::outline_set_enabled);
	ClassDB::bind_method(D_METHOD("outline_get_enabled"), &PaintCurve2D::outline_get_enabled);

	ClassDB::bind_method(D_METHOD("outline_set_width", "width"), &PaintCurve2D::outline_set_width);
	ClassDB::bind_method(D_METHOD("outline_get_width"), &PaintCurve2D::outline_get_width);

	ClassDB::bind_method(D_METHOD("outline_set_color", "color"), &PaintCurve2D::outline_set_color);
	ClassDB::bind_method(D_METHOD("outline_get_color"), &PaintCurve2D::outline_get_color);

	ClassDB::bind_method(D_METHOD("outline_set_texture", "texture"), &PaintCurve2D::outline_set_texture);
	ClassDB::bind_method(D_METHOD("outline_get_texture"), &PaintCurve2D::outline_get_texture);

	ClassDB::bind_method(D_METHOD("outline_set_texture_offset", "texture_offset"), &PaintCurve2D::outline_set_texture_offset);
	ClassDB::bind_method(D_METHOD("outline_get_texture_offset"), &PaintCurve2D::outline_get_texture_offset);

	ClassDB::bind_method(D_METHOD("outline_set_texture_rotation", "texture_rotation"), &PaintCurve2D::outline_set_texture_rotation);
	ClassDB::bind_method(D_METHOD("outline_get_texture_rotation"), &PaintCurve2D::outline_get_texture_rotation);

	ClassDB::bind_method(D_METHOD("outline_set_texture_rotation_degrees", "texture_rotation"), &PaintCurve2D::outline_set_texture_rotation_degrees);
	ClassDB::bind_method(D_METHOD("outline_get_texture_rotation_degrees"), &PaintCurve2D::outline_get_texture_rotation_degrees);

	ClassDB::bind_method(D_METHOD("outline_set_texture_scale", "texture_scale"), &PaintCurve2D::outline_set_texture_scale);
	ClassDB::bind_method(D_METHOD("outline_get_texture_scale"), &PaintCurve2D::outline_get_texture_scale);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D"), "set_curve", "get_curve");

	ADD_GROUP("Fill", "fill_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fill_enabled"), "fill_set_enabled", "fill_get_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "fill_color"), "fill_set_color", "fill_get_color");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fill_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "fill_set_texture", "fill_get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "fill_texture_offset"), "fill_set_texture_offset", "fill_get_texture_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "fill_texture_scale", PROPERTY_HINT_LINK), "fill_set_texture_scale", "fill_get_texture_scale");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fill_texture_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater"), "fill_set_texture_rotation_degrees", "fill_get_texture_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fill_texture_rotation", PROPERTY_HINT_NONE, "", 0), "fill_set_texture_rotation", "fill_get_texture_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fill_invert_enable"), "fill_set_invert", "fill_get_invert");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fill_invert_border", PROPERTY_HINT_RANGE, "0.1,16384,0.1"), "fill_set_invert_border", "fill_get_invert_border");

	ADD_GROUP("Outline", "outline_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outline_enabled"), "outline_set_enabled", "outline_get_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outline_color"), "outline_set_color", "outline_get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "outline_width"), "outline_set_width", "outline_get_width");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "outline_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "outline_set_texture", "outline_get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "outline_texture_offset"), "outline_set_texture_offset", "outline_get_texture_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "outline_texture_scale", PROPERTY_HINT_LINK), "outline_set_texture_scale", "outline_get_texture_scale");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "outline_texture_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater"), "outline_set_texture_rotation_degrees", "outline_get_texture_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "outline_texture_rotation", PROPERTY_HINT_NONE, "", 0), "outline_set_texture_rotation", "outline_get_texture_rotation");

	ClassDB::bind_method(D_METHOD("_curve_changed"), &PaintCurve2D::_curve_changed);
}

PaintCurve2D::PaintCurve2D() {
	set_curve(Ref<Curve2D>(memnew(Curve2D))); //create one by default
	//set_self_modulate(Color(0.5, 0.6, 1.0, 0.7));

	_fill_enabled = true;
	_fill_color = Color(1, 1, 1);
	_fill_tex_scale = Vector2(1, 1);
	_fill_tex_tile = false;
	_fill_tex_rot = 0;
	_fill_invert = false;
	_fill_invert_border = 100;

	_outline_enabled = false;
	//_outline_color = Color(0.5, 0.6, 1.0, 0.7);
	_outline_color = Color(1, 1, 1);
	_outline_width = 2;
	_outline_tex_scale = Vector2(1, 1);
	_outline_tex_tile = false;
	_outline_tex_rot = 0;

	rect_cache_dirty = true;
}
