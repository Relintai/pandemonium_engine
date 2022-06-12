
#include "gradient_base.h"

int GradientBase::get_interpolation_type() const {
	return interpolation_type;
}

void GradientBase::set_interpolation_type(const int val) {
	interpolation_type = val;
	set_dirty(true);
}

PoolRealArray GradientBase::get_points() {
	return points;
}

void GradientBase::set_points(const PoolRealArray &val) {
	points = val;
	set_dirty(true);
}

Color GradientBase::get_gradient_color(const float x) {
	//	if interpolation_type == 0:;
	//		return Gradients.gradient_type_1(x, points);
	//	elif interpolation_type == 1:;
	//		return Gradients.gradient_type_2(x, points);
	//	elif interpolation_type == 2:;
	//		return Gradients.gradient_type_3(x, points);
	//	elif interpolation_type == 3:;
	//		return Gradients.gradient_type_4(x, points);
	return Color(1, 1, 1, 1);
}

float GradientBase::get_point_value(const int index) {
	return points[index * 5];
}

Color GradientBase::get_point_color(const int index) {
	int indx = index * 5;
	return Color(points[indx + 1], points[indx + 2], points[indx + 3], points[indx + 4]);
}

void GradientBase::add_point(const float val, const Color &color) {
	int s = points.size();

	points.resize(s + 5);
	points.set(s, val);
	points.set(s + 1, color.r);
	points.set(s + 2, color.g);
	points.set(s + 3, color.b);
	points.set(s + 4, color.a);

	set_dirty(true);
}

int GradientBase::get_point_count() {
	return points.size() / 5;
}

void GradientBase::clear() {
	points.resize(0);
	set_dirty(true);
}

GradientBase::GradientBase() {
	interpolation_type = 1;
}

GradientBase::~GradientBase() {
}

void GradientBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_interpolation_type"), &GradientBase::get_interpolation_type);
	ClassDB::bind_method(D_METHOD("set_interpolation_type", "value"), &GradientBase::set_interpolation_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation_type"), "set_interpolation_type", "get_interpolation_type");

	ClassDB::bind_method(D_METHOD("get_points"), &GradientBase::get_points);
	ClassDB::bind_method(D_METHOD("set_points", "value"), &GradientBase::set_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_REAL_ARRAY, "points"), "set_points", "get_points");

	ClassDB::bind_method(D_METHOD("get_gradient_color", "x"), &GradientBase::get_gradient_color);

	ClassDB::bind_method(D_METHOD("get_point_value", "index"), &GradientBase::get_point_value);
	ClassDB::bind_method(D_METHOD("get_point_color", "index"), &GradientBase::get_point_color);

	ClassDB::bind_method(D_METHOD("add_point", "val", "color"), &GradientBase::add_point);

	ClassDB::bind_method(D_METHOD("get_point_count"), &GradientBase::get_point_count);

	ClassDB::bind_method(D_METHOD("clear"), &GradientBase::clear);
}
