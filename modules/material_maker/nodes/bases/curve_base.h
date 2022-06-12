#ifndef CURVE_BASE_H
#define CURVE_BASE_H

#include "core/math/math_defs.h"
#include "core/math/vector2.h"
#include "core/variant.h"
#include "core/vector.h"

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

	String to_string();

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
	void _curve_changed();

	CurveBase();
	~CurveBase();

protected:
	static void _bind_methods();

	PoolRealArray points;
};

#endif
