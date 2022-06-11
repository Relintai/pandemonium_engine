#ifndef CURVE_BASE_H
#define CURVE_BASE_H

#include "core/variant.h"

#include "../mm_node.h"

class CurveBase : public MMNode {
	GDCLASS(CurveBase, MMNode);

public:
	class Point {
	public:
		Vector2 get_p();
		void set_p(const Vector2 &val);

		float get_ls() const;
		void set_ls(const float val);

		float get_rs() const;
		void set_rs(const float val);

		Point();
		Point(const float x, const float y, const float nls, const float nrs);
		~Point();

	protected:
		static void _bind_methods();

		Vector2 p;
		float ls;
		float rs;
	};

public:
	PoolRealArray get_points();
	void set_points(const PoolRealArray &val);

	void init_points_01();
	void init_points_11();

	String to_string();

	void clear();

	void add_point(const float x, const float y, const float ls = INF, const float rs = INF);
	bool remove_point(const int i);

	int get_point_count();

	void set_point(const int i, const Point &v);
	Point get_point(const int i);
	Array get_points();
	void set_points(const Array &arr, const bool notify = true);

	void curve_changed();
	void _curve_changed();

	CurveBase();
	~CurveBase();

protected:
	static void _bind_methods();

	PoolRealArray points;
};

#endif
