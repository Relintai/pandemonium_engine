
#include "curve_base.h"

PoolRealArray CurveBase::get_points_array() {
	return points;
}

void CurveBase::set_points_array(const PoolRealArray &val) {
	points = val;
}

void CurveBase::init_points_01() {
	if (points.size() == 0) {
		points.push_back(0.0);
		points.push_back(0.0);
		points.push_back(0.0);
		points.push_back(1.0);
		points.push_back(1.0);
		points.push_back(1.0);
		points.push_back(1.0);
		points.push_back(0.0);
	}
}

void CurveBase::init_points_11() {
	if (points.size() == 0) {
		points.push_back(0.0);
		points.push_back(1.0);
		points.push_back(0.0);
		points.push_back(0.0);
		points.push_back(1.0);
		points.push_back(1.0);
		points.push_back(0.0);
		points.push_back(0.0);
	}
}

String CurveBase::_to_string() {
	PoolStringArray rv;

	Vector<CurveBase::Point> ps = get_points();

	for (int i = 0; i < ps.size(); ++i) {
		Point p = ps[i];
		rv.append("(" + String::num(p.p.x) + "," + String::num(p.p.y) + "," + String::num(p.ls) + "," + String::num(p.rs) + ")");
	}

	return rv.join(",");
}

void CurveBase::clear() {
	points.resize(0);
	curve_changed();
}

void CurveBase::add_point(const float x, const float y, float ls, float rs) {
	int indx = points.size() / 4;

	for (int i = 0; i < indx; ++i) {
		int ii = i * 4;

		if (x < points[ii]) {
			if (ls == Math_INF) {
				ls = 0;
			}

			if (rs == Math_INF) {
				rs = 0;
			}

			points.insert(ii, x);
			points.insert(ii + 1, y);
			points.insert(ii + 2, ls);
			points.insert(ii + 3, rs);
			curve_changed();
			return;
		}
	}

	points.append(x);
	points.append(y);
	points.append(ls);
	points.append(rs);
	curve_changed();
}

bool CurveBase::remove_point(const int i) {
	int index = i * 4;

	if (index <= 0 || index >= points.size() - 1) {
		return false;
	} else {
		points.remove(index);
		points.remove(index);
		points.remove(index);
		points.remove(index);
		curve_changed();
	}

	return true;
}

PoolRealArray CurveBase::get_point_arr(const int i) {
	int indx = i * 4;

	PoolRealArray arr;

	arr.push_back(points[indx + 0]);
	arr.push_back(points[indx + 1]);
	arr.push_back(points[indx + 2]);
	arr.push_back(points[indx + 3]);

	return arr;
}

int CurveBase::get_point_count() {
	return points.size() / 4;
}

void CurveBase::set_point(const int i, const Point &v) {
	int indx = i * 4;

	points.set(indx + 0, v.p.x);
	points.set(indx + 1, v.p.y);
	points.set(indx + 2, v.ls);
	points.set(indx + 3, v.rs);

	curve_changed();
}

CurveBase::Point CurveBase::get_point(const int i) {
	int indx = i * 4;

	return Point(points[indx + 0], points[indx + 1], points[indx + 2], points[indx + 3]);
}

Vector<CurveBase::Point> CurveBase::get_points() {
	Vector<CurveBase::Point> arr;
	int c = get_point_count();

	for (int i = 0; i < c; ++i) { //i in range(c)
		arr.push_back(get_point(i));
	}

	return arr;
}

void CurveBase::set_points(const Vector<CurveBase::Point> &arr, const bool notify) {
	points.resize(0);

	for (int i = 0; i < arr.size(); ++i) {
		Point p = arr[i];
		points.append(p.p.x);
		points.append(p.p.y);
		points.append(p.ls);
		points.append(p.rs);
	}

	if (notify) {
		curve_changed();
	}
}

void CurveBase::curve_changed() {
	call("_curve_changed");
}

void CurveBase::_curve_changed() {
	emit_changed();
}

CurveBase::CurveBase() {
}

CurveBase::~CurveBase() {
}

void CurveBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_points_array"), &CurveBase::get_points_array);
	ClassDB::bind_method(D_METHOD("set_points_array", "value"), &CurveBase::set_points_array);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_REAL_ARRAY, "points_array"), "set_points_array", "get_points_array");

	ClassDB::bind_method(D_METHOD("init_points_01"), &CurveBase::init_points_01);
	ClassDB::bind_method(D_METHOD("init_points_11"), &CurveBase::init_points_11);

	ClassDB::bind_method(D_METHOD("_to_string"), &CurveBase::_to_string);

	ClassDB::bind_method(D_METHOD("clear"), &CurveBase::clear);

	ClassDB::bind_method(D_METHOD("add_point", "x", "y", "ls", "rs"), &CurveBase::add_point, Math_INF, Math_INF);
	ClassDB::bind_method(D_METHOD("remove_point", "i"), &CurveBase::remove_point);
	ClassDB::bind_method(D_METHOD("get_point_arr", "i"), &CurveBase::get_point_arr);

	ClassDB::bind_method(D_METHOD("get_point_count"), &CurveBase::get_point_count);

	//ClassDB::bind_method(D_METHOD("set_point", "i", "v"), &CurveBase::set_point);
	//ClassDB::bind_method(D_METHOD("get_point", "i"), &CurveBase::get_point);

	//ClassDB::bind_method(D_METHOD("get_points"), &CurveBase::get_points);
	//ClassDB::bind_method(D_METHOD("set_points", "arr", "notify"), &CurveBase::set_points, true);

	BIND_VMETHOD(MethodInfo("_curve_changed"));
	ClassDB::bind_method(D_METHOD("curve_changed"), &CurveBase::curve_changed);
	ClassDB::bind_method(D_METHOD("_curve_changed"), &CurveBase::_curve_changed);
}
