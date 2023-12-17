#ifndef CURVE_BASE_H
#define CURVE_BASE_H

/*************************************************************************/
/*  curve_base.h                                                         */
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

#include "core/containers/vector.h"
#include "core/math/math_defs.h"
#include "core/math/math_funcs.h"
#include "core/math/vector2.h"
#include "core/variant/variant.h"

#include "../mm_node.h"

class CurveBase : public MMNode {
	GDCLASS(CurveBase, MMNode);

public:
	struct Point {
		Vector2 p;
		float ls;
		float rs;

		void set(const float x, const float y, const float nls, const float nrs) {
			p = Vector2(x, y);
			ls = nls;
			rs = nrs;
		}

		Point() {
			ls = 0;
			rs = 0;
		}

		Point(const float x, const float y, const float nls, const float nrs) {
			p = Vector2(x, y);
			ls = nls;
			rs = nrs;
		}
	};

public:
	PoolRealArray get_points_array();
	void set_points_array(const PoolRealArray &val);

	void init_points_01();
	void init_points_11();

	String _to_string();

	void clear();

	void add_point(const float x, const float y, float ls = Math_INF, float rs = Math_INF);
	bool remove_point(const int i);
	PoolRealArray get_point_arr(const int i);

	int get_point_count();

	void set_point(const int i, const Point &v);
	Point get_point(const int i);

	Vector<Point> get_points();
	void set_points(const Vector<Point> &arr, const bool notify = true);

	void curve_changed();
	virtual void _curve_changed();

	CurveBase();
	~CurveBase();

protected:
	static void _bind_methods();

	PoolRealArray points;
};

#endif
