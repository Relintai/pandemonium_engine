/**************************************************************************/
/*  plane.cpp                                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PANDEMONIUM ENGINE                               */
/*                        https://pandemoniumengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "gdn/plane.h"

#include "core/math/plane.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_plane) == sizeof(Plane), "Plane size mismatch");

void GDAPI pandemonium_plane_new_with_reals(pandemonium_plane *r_dest, const pandemonium_real p_a, const pandemonium_real p_b, const pandemonium_real p_c, const pandemonium_real p_d) {
	Plane *dest = (Plane *)r_dest;
	*dest = Plane(p_a, p_b, p_c, p_d);
}

void GDAPI pandemonium_plane_new_with_vectors(pandemonium_plane *r_dest, const pandemonium_vector3 *p_v1, const pandemonium_vector3 *p_v2, const pandemonium_vector3 *p_v3) {
	const Vector3 *v1 = (const Vector3 *)p_v1;
	const Vector3 *v2 = (const Vector3 *)p_v2;
	const Vector3 *v3 = (const Vector3 *)p_v3;
	Plane *dest = (Plane *)r_dest;
	*dest = Plane(*v1, *v2, *v3);
}

void GDAPI pandemonium_plane_new_with_normal(pandemonium_plane *r_dest, const pandemonium_vector3 *p_normal, const pandemonium_real p_d) {
	const Vector3 *normal = (const Vector3 *)p_normal;
	Plane *dest = (Plane *)r_dest;
	*dest = Plane(*normal, p_d);
}

pandemonium_string GDAPI pandemonium_plane_as_string(const pandemonium_plane *p_self) {
	pandemonium_string ret;
	const Plane *self = (const Plane *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_plane_normalize(pandemonium_plane *p_self) {
	Plane *self = (Plane *)p_self;
	self->normalize();
}
pandemonium_plane GDAPI pandemonium_plane_normalized(const pandemonium_plane *p_self) {
	pandemonium_plane dest;
	const Plane *self = (const Plane *)p_self;
	*((Plane *)&dest) = self->normalized();
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_plane_center(const pandemonium_plane *p_self) {
	pandemonium_vector3 dest;
	const Plane *self = (const Plane *)p_self;
	*((Vector3 *)&dest) = self->center();
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_plane_get_any_point(const pandemonium_plane *p_self) {
	pandemonium_vector3 dest;
	const Plane *self = (const Plane *)p_self;
	*((Vector3 *)&dest) = self->get_any_point();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_plane_get_any_perpendicular_normal(const pandemonium_plane *p_self) {
	pandemonium_vector3 dest;
	const Plane *self = (const Plane *)p_self;
	*((Vector3 *)&dest) = self->get_any_perpendicular_normal();
	return dest;
}

pandemonium_bool GDAPI pandemonium_plane_is_point_over(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 *point = (const Vector3 *)p_point;
	return self->is_point_over(*point);
}

pandemonium_real GDAPI pandemonium_plane_distance_to(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 *point = (const Vector3 *)p_point;
	return self->distance_to(*point);
}

pandemonium_bool GDAPI pandemonium_plane_has_point(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point, const pandemonium_real p_epsilon) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 *point = (const Vector3 *)p_point;
	return self->has_point(*point, p_epsilon);
}

pandemonium_vector3 GDAPI pandemonium_plane_project(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point) {
	pandemonium_vector3 dest;
	const Plane *self = (const Plane *)p_self;
	const Vector3 *point = (const Vector3 *)p_point;
	*((Vector3 *)&dest) = self->project(*point);
	return dest;
}

pandemonium_bool GDAPI pandemonium_plane_intersect_3(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_plane *p_b, const pandemonium_plane *p_c) {
	const Plane *self = (const Plane *)p_self;
	const Plane *b = (const Plane *)p_b;
	const Plane *c = (const Plane *)p_c;
	Vector3 *dest = (Vector3 *)r_dest;
	return self->intersect_3(*b, *c, dest);
}

pandemonium_bool GDAPI pandemonium_plane_intersects_ray(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_dir) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	Vector3 *dest = (Vector3 *)r_dest;
	return self->intersects_ray(*from, *dir, dest);
}

pandemonium_bool GDAPI pandemonium_plane_intersects_segment(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_vector3 *p_begin, const pandemonium_vector3 *p_end) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 *begin = (const Vector3 *)p_begin;
	const Vector3 *end = (const Vector3 *)p_end;
	Vector3 *dest = (Vector3 *)r_dest;
	return self->intersects_segment(*begin, *end, dest);
}

pandemonium_plane GDAPI pandemonium_plane_operator_neg(const pandemonium_plane *p_self) {
	pandemonium_plane raw_dest;
	Plane *dest = (Plane *)&raw_dest;
	const Plane *self = (const Plane *)p_self;
	*dest = -(*self);
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_plane_is_equal_approx(const pandemonium_plane *p_self, const pandemonium_plane *p_plane) {
	const Plane *self = (const Plane *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	return self->is_equal_approx(*plane);
}
pandemonium_bool GDAPI pandemonium_plane_is_equal_approx_any_side(const pandemonium_plane *p_self, const pandemonium_plane *p_plane) {
	const Plane *self = (const Plane *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	return self->is_equal_approx_any_side(*plane);
}

pandemonium_bool GDAPI pandemonium_plane_operator_equal(const pandemonium_plane *p_self, const pandemonium_plane *p_b) {
	const Plane *self = (const Plane *)p_self;
	const Plane *b = (const Plane *)p_b;
	return *self == *b;
}

void GDAPI pandemonium_plane_set_normal(pandemonium_plane *p_self, const pandemonium_vector3 *p_normal) {
	Plane *self = (Plane *)p_self;
	const Vector3 *normal = (const Vector3 *)p_normal;
	self->set_normal(*normal);
}

pandemonium_vector3 GDAPI pandemonium_plane_get_normal(const pandemonium_plane *p_self) {
	const Plane *self = (const Plane *)p_self;
	const Vector3 normal = self->get_normal();
	pandemonium_vector3 *v3 = (pandemonium_vector3 *)&normal;
	return *v3;
}

pandemonium_real GDAPI pandemonium_plane_get_d(const pandemonium_plane *p_self) {
	const Plane *self = (const Plane *)p_self;
	return self->d;
}

void GDAPI pandemonium_plane_set_d(pandemonium_plane *p_self, const pandemonium_real p_d) {
	Plane *self = (Plane *)p_self;
	self->d = p_d;
}

#ifdef __cplusplus
}
#endif
