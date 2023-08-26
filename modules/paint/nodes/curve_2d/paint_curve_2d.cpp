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
	//if (rect_cache_dirty) {
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
	//	rect_cache_dirty = false;
	//}

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

		if (curve->get_point_count() < 2) {
			return;
		}

		if (_outline_enabled) {
			_cached_draw_pts.resize(curve->get_point_count() * 8);
			int count = 0;

			for (int i = 0; i < curve->get_point_count(); i++) {
				for (int j = 0; j < 8; j++) {
					real_t frac = j * (1.0 / 8.0);
					Vector2 p = curve->interpolate(i, frac);
					_cached_draw_pts.set(count++, p);
				}
			}

			draw_polyline(_cached_draw_pts, _outline_color, _outline_width, _outline_antialiased);
		}
	}
}

void PaintCurve2D::_curve_changed() {
	if (!is_inside_tree()) {
		return;
	}

	if (!Engine::get_singleton()->is_editor_hint() && !get_tree()->is_debugging_navigation_hint()) {
		return;
	}

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

void PaintCurve2D::set_offset(const Vector2 &p_offset) {
	offset = p_offset;
	rect_cache_dirty = true;
	update();
	_change_notify("offset");
}

Vector2 PaintCurve2D::get_offset() const {
	return offset;
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

void PaintCurve2D::fill_set_antialiased(bool p_antialiased) {
	_fill_antialiased = p_antialiased;
	update();
}
bool PaintCurve2D::fill_get_antialiased() const {
	return _fill_antialiased;
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

void PaintCurve2D::outline_set_antialiased(bool p_antialiased) {
	_outline_antialiased = p_antialiased;
	update();
}
bool PaintCurve2D::outline_get_antialiased() const {
	return _outline_antialiased;
}

void PaintCurve2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_curve", "curve"), &PaintCurve2D::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &PaintCurve2D::get_curve);

	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &PaintCurve2D::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &PaintCurve2D::get_offset);

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

	ClassDB::bind_method(D_METHOD("fill_set_antialiased", "antialiased"), &PaintCurve2D::fill_set_antialiased);
	ClassDB::bind_method(D_METHOD("fill_get_antialiased"), &PaintCurve2D::fill_get_antialiased);

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

	ClassDB::bind_method(D_METHOD("outline_set_antialiased", "antialiased"), &PaintCurve2D::outline_set_antialiased);
	ClassDB::bind_method(D_METHOD("outline_get_antialiased"), &PaintCurve2D::outline_get_antialiased);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D"), "set_curve", "get_curve");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");

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
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fill_antialiased"), "fill_set_antialiased", "fill_get_antialiased");

	ADD_GROUP("Outline", "outline_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outline_enabled"), "outline_set_enabled", "outline_get_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outline_color"), "outline_set_color", "outline_get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "outline_width"), "outline_set_width", "outline_get_width");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "outline_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "outline_set_texture", "outline_get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "outline_texture_offset"), "outline_set_texture_offset", "outline_get_texture_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "outline_texture_scale", PROPERTY_HINT_LINK), "outline_set_texture_scale", "outline_get_texture_scale");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "outline_texture_rotation_degrees", PROPERTY_HINT_RANGE, "-360,360,0.1,or_lesser,or_greater"), "outline_set_texture_rotation_degrees", "outline_get_texture_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "outline_texture_rotation", PROPERTY_HINT_NONE, "", 0), "outline_set_texture_rotation", "outline_get_texture_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outline_antialiased"), "outline_set_antialiased", "outline_get_antialiased");

	ClassDB::bind_method(D_METHOD("_curve_changed"), &PaintCurve2D::_curve_changed);
}

PaintCurve2D::PaintCurve2D() {
	set_curve(Ref<Curve2D>(memnew(Curve2D))); //create one by default
	//set_self_modulate(Color(0.5, 0.6, 1.0, 0.7));

	_fill_enabled = true;
	_fill_color = Color(1, 1, 1);
	_fill_tex_tile = false;
	_fill_tex_rot = 0;
	_fill_invert = false;
	_fill_invert_border = 0;
	_fill_antialiased = false;

	_outline_enabled = true;
	//_outline_color = Color(0.5, 0.6, 1.0, 0.7);
	_outline_width = 2;
	_outline_tex_tile = false;
	_outline_tex_rot = 0;
	_outline_antialiased = true;

	rect_cache_dirty = true;
}
