
#include "polygon_base.h"

#include "core/math/geometry.h"

void PolygonBase::clear() {
	points.resize(0);
	_polygon_changed();
}

void PolygonBase::add_point(const float x, const float y, const bool closed) {
	Vector2 p = Vector2(x, y);
	int points_count = points.size();

	if (points_count < 3) {
		points.append(p);
		_polygon_changed();
		return;
	}

	Vector2 segs[2];
	segs[0] = points[0];
	segs[1] = points[points_count - 1];

	float min_length = (p - Geometry::get_closest_point_to_segment_2d(p, segs)).length();
	int insert_point = 0;

	for (int i = 0; i < points_count - 1; ++i) {
		segs[0] = points[i];
		segs[1] = points[i + 1];

		float length = (p - Geometry::get_closest_point_to_segment_2d(p, segs)).length();

		if (length < min_length) {
			min_length = length;
			insert_point = i + 1;
		}
	}

	if (!closed && insert_point == 0 && (points[0] - p).length() > (points[points_count - 1] - p).length()) {
		insert_point = points_count;
	}

	points.insert(insert_point, p);
	_polygon_changed();
}

bool PolygonBase::remove_point(const int index) {
	int s = points.size();

	if (s < 4 || index < 0 || index >= s) {
		return false;
	} else {
		points.remove(index);
		_polygon_changed();
	}

	return true;
}

int PolygonBase::get_point_count() {
	return points.size();
}

Vector2 PolygonBase::get_point(const int i) {
	return points[i];
}

void PolygonBase::set_point(const int i, const Vector2 &v) {
	points[i] = v;
	_polygon_changed();
}

void PolygonBase::set_points(const PoolVector2Array &v) {
	points = v;
	_polygon_changed();
}

void PolygonBase::polygon_changed() {
	_polygon_changed();
}

void PolygonBase::_polygon_changed() {
	emit_changed();
}

String PolygonBase::to_string() {
	String rv;

	PoolVector2Array ps = get_points();

	for (int i = 0; i < ps.size(); ++i) {
		Vector2 p = ps[i];

		rv += "(" + String::num(p.x) + "," + String::num(p.y) + ")" + ",";
	}

	return rv;
}

PolygonBase::PolygonBase() {
	points.push_back(Vector2(0.2, 0.2));
	points.push_back(Vector2(0.7, 0.4));
	points.push_back(Vector2(0.4, 0.7));
}

PolygonBase::~PolygonBase() {
}

void PolygonBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_points"), &PolygonBase::get_points);
	ClassDB::bind_method(D_METHOD("set_points", "value"), &PolygonBase::set_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "points"), "set_points", "get_points");

	ClassDB::bind_method(D_METHOD("clear"), &PolygonBase::clear);
	ClassDB::bind_method(D_METHOD("add_point", "x", "y", "closed"), &PolygonBase::add_point, true);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PolygonBase::remove_point);
	ClassDB::bind_method(D_METHOD("get_point_count"), &PolygonBase::get_point_count);

	ClassDB::bind_method(D_METHOD("get_point", "i"), &PolygonBase::get_point);
	ClassDB::bind_method(D_METHOD("set_point", "i", "v"), &PolygonBase::set_point);

	BIND_VMETHOD(MethodInfo("_polygon_changed"));
	ClassDB::bind_method(D_METHOD("polygon_changed"), &PolygonBase::polygon_changed);
	ClassDB::bind_method(D_METHOD("_polygon_changed"), &PolygonBase::_polygon_changed);

	ClassDB::bind_method(D_METHOD("to_string"), &PolygonBase::to_string);
}
