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
	if (!curve.is_valid() || curve->get_point_count() == 0) {
		return Rect2(0, 0, 0, 0);
	}

	Rect2 aabb = Rect2(curve->get_point_position(0), Vector2(0, 0));

	for (int i = 0; i < curve->get_point_count(); i++) {
		for (int j = 0; j <= 8; j++) {
			real_t frac = j / 8.0;
			Vector2 p = curve->interpolate(i, frac);
			aabb.expand_to(p);
		}
	}

	return aabb;
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

		if (!Engine::get_singleton()->is_editor_hint() && !get_tree()->is_debugging_navigation_hint()) {
			return;
		}

		if (curve->get_point_count() < 2) {
			return;
		}

#ifdef TOOLS_ENABLED
		const float line_width = 2 * EDSCALE;
#else
		const float line_width = 2;
#endif
		const Color color = Color(1.0, 1.0, 1.0, 1.0);

		_cached_draw_pts.resize(curve->get_point_count() * 8);
		int count = 0;

		for (int i = 0; i < curve->get_point_count(); i++) {
			for (int j = 0; j < 8; j++) {
				real_t frac = j * (1.0 / 8.0);
				Vector2 p = curve->interpolate(i, frac);
				_cached_draw_pts.set(count++, p);
			}
		}

		draw_polyline(_cached_draw_pts, color, line_width, true);
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

void PaintCurve2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_curve", "curve"), &PaintCurve2D::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &PaintCurve2D::get_curve);
	ClassDB::bind_method(D_METHOD("_curve_changed"), &PaintCurve2D::_curve_changed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D"), "set_curve", "get_curve");
}

PaintCurve2D::PaintCurve2D() {
	set_curve(Ref<Curve2D>(memnew(Curve2D))); //create one by default
	set_self_modulate(Color(0.5, 0.6, 1.0, 0.7));
}
