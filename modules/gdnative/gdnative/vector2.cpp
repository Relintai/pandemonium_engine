/**************************************************************************/
/*  vector2.cpp                                                           */
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

#include "gdn/vector2.h"

#include "core/math/vector2.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_vector2) == sizeof(Vector2), "Vector2 size mismatch");

void GDAPI pandemonium_vector2_set_x(pandemonium_vector2 *p_self, const pandemonium_real p_x) {
	Vector2 *self = (Vector2 *)p_self;
	self->x = p_x;
}
void GDAPI pandemonium_vector2_set_y(pandemonium_vector2 *p_self, const pandemonium_real p_y) {
	Vector2 *self = (Vector2 *)p_self;
	self->y = p_y;
}

pandemonium_real GDAPI pandemonium_vector2_get_x(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->x;
}
pandemonium_real GDAPI pandemonium_vector2_get_y(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->y;
}

pandemonium_real GDAPI *pandemonium_vector2_operator_index(pandemonium_vector2 *p_self, const pandemonium_int p_idx) {
	Vector2 *self = (Vector2 *)p_self;
	return (pandemonium_real *)&self->operator[](p_idx);
}
const pandemonium_real GDAPI *pandemonium_vector2_operator_index_const(const pandemonium_vector2 *p_self, const pandemonium_int p_idx) {
	const Vector2 *self = (const Vector2 *)p_self;
	return (const pandemonium_real *)&self->operator[](p_idx);
}

void GDAPI pandemonium_vector2_set_axis(pandemonium_vector2 *p_self, const pandemonium_vector2_axis p_axis, const pandemonium_real p_value) {
	Vector2 *self = (Vector2 *)p_self;
	self->operator[](p_axis) = p_value;
}
pandemonium_real GDAPI pandemonium_vector2_get_axis(const pandemonium_vector2 *p_self, const pandemonium_vector2_axis p_axis) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->operator[](p_axis);
}

void GDAPI pandemonium_vector2_set_all(pandemonium_vector2 *p_self, const pandemonium_real p_value) {
	Vector2 *self = (Vector2 *)p_self;
	self->set_all(p_value);
}

pandemonium_int GDAPI pandemonium_vector2_min_axis(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->min_axis();
}
pandemonium_int GDAPI pandemonium_vector2_max_axis(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->max_axis();
}

void GDAPI pandemonium_vector2_normalize(pandemonium_vector2 *p_self) {
	Vector2 *self = (Vector2 *)p_self;
	self->normalize();
}
pandemonium_vector2 GDAPI pandemonium_vector2_normalized(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->normalized();
	return dest;
}
pandemonium_bool GDAPI pandemonium_vector2_is_normalized(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->is_normalized();
}

pandemonium_real GDAPI pandemonium_vector2_length(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->length();
}
pandemonium_real GDAPI pandemonium_vector2_length_squared(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->length_squared();
}
pandemonium_vector2 GDAPI pandemonium_vector2_limit_length(const pandemonium_vector2 *p_self, const pandemonium_real p_length) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;

	*((Vector2 *)&dest) = self->limit_length(p_length);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_min(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*((Vector2 *)&dest) = self->min(*b);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_max(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*((Vector2 *)&dest) = self->max(*b);
	return dest;
}

pandemonium_real GDAPI pandemonium_vector2_distance_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	return self->distance_to(*to);
}
pandemonium_real GDAPI pandemonium_vector2_distance_squared_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	return self->distance_squared_to(*to);
}
pandemonium_real GDAPI pandemonium_vector2_angle_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	return self->angle_to(*to);
}
pandemonium_real GDAPI pandemonium_vector2_angle_to_point(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	return self->angle_to_point(*to);
}
pandemonium_vector2 GDAPI pandemonium_vector2_direction_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Vector2 *)&dest) = self->direction_to(*to);
	return dest;
}

pandemonium_real GDAPI pandemonium_vector2_dot(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_other) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *other = (const Vector2 *)p_other;
	return self->dot(*other);
}
pandemonium_real GDAPI pandemonium_vector2_cross(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_other) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *other = (const Vector2 *)p_other;
	return self->dot(*other);
}
pandemonium_vector2 GDAPI pandemonium_vector2_posmod(const pandemonium_vector2 *p_self, const pandemonium_real p_mod) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->posmod(p_mod);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_posmodv(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_modv) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *modv = (const Vector2 *)p_modv;
	*((Vector2 *)&dest) = self->posmodv(*modv);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_project(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Vector2 *)&dest) = self->project(*to);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_plane_project(const pandemonium_vector2 *p_self, const pandemonium_real p_length, const pandemonium_vector2 *p_vec) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *vec = (const Vector2 *)p_vec;
	*((Vector2 *)&dest) = self->plane_project(p_length, *vec);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_linear_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b, const pandemonium_real p_t) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*((Vector2 *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_slerp(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to, const pandemonium_real p_weight) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Vector2 *)&dest) = self->slerp(*to, p_weight);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_cubic_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b, const pandemonium_vector2 *p_pre_a, const pandemonium_vector2 *p_post_b, const pandemonium_real p_t) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	const Vector2 *pre_a = (const Vector2 *)p_pre_a;
	const Vector2 *post_b = (const Vector2 *)p_post_b;
	*((Vector2 *)&dest) = self->cubic_interpolate(*b, *pre_a, *post_b, p_t);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_bezier_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_control_1, const pandemonium_vector2 *p_control_2, const pandemonium_vector2 *p_end, const pandemonium_real p_t) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *control_1 = (const Vector2 *)p_control_1;
	const Vector2 *control_2 = (const Vector2 *)p_control_2;
	const Vector2 *end = (const Vector2 *)p_end;
	*((Vector2 *)&dest) = self->cubic_interpolate(*control_1, *control_2, *end, p_t);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_move_toward(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to, const pandemonium_real p_delta) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Vector2 *)&dest) = self->move_toward(*to, p_delta);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_slide(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *n = (const Vector2 *)p_n;
	*((Vector2 *)&dest) = self->slide(*n);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_bounce(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *n = (const Vector2 *)p_n;
	*((Vector2 *)&dest) = self->bounce(*n);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_reflect(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *n = (const Vector2 *)p_n;
	*((Vector2 *)&dest) = self->reflect(*n);
	return dest;
}

pandemonium_bool GDAPI pandemonium_vector2_is_equal_approx(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	return self->is_equal_approx(*b);
}

pandemonium_vector2 GDAPI pandemonium_vector2_operator_add(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*dest = *self + *b;
	return raw_dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_operator_subtract(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_operator_multiply_vector(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*dest = *self * *b;
	return raw_dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_operator_multiply_scalar(const pandemonium_vector2 *p_self, const pandemonium_real p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_operator_divide_vector(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	*dest = *self / *b;
	return raw_dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_operator_divide_scalar(const pandemonium_vector2 *p_self, const pandemonium_real p_b) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*dest = *self / p_b;
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_operator_neg(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*dest = -(*self);
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_vector2_operator_equal(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	return *self == *b;
}
pandemonium_bool GDAPI pandemonium_vector2_operator_less(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b) {
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *b = (const Vector2 *)p_b;
	return *self < *b;
}

pandemonium_real GDAPI pandemonium_vector2_angle(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->angle();
}

void GDAPI pandemonium_vector2_set_rotation(pandemonium_vector2 *p_self, const pandemonium_real p_phi) {
	Vector2 *self = (Vector2 *)p_self;
	self->set_rotation(p_phi);
}

pandemonium_vector2 GDAPI pandemonium_vector2_abs(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->abs();
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_rotated(const pandemonium_vector2 *p_self, const pandemonium_real p_phi) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;

	*((Vector2 *)&dest) = self->rotated(p_phi);
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_tangent(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->tangent();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_orthogonal(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;

	*((Vector2 *)&dest) = self->orthogonal();
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_vector2_sign(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->sign();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_floor(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->floor();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_ceil(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->ceil();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_round(const pandemonium_vector2 *p_self) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	*((Vector2 *)&dest) = self->round();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_vector2_snapped(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_by) {
	pandemonium_vector2 dest;
	const Vector2 *self = (const Vector2 *)p_self;
	const Vector2 *by = (const Vector2 *)p_by;
	*((Vector2 *)&dest) = self->snapped(*by);
	return dest;
}
pandemonium_real GDAPI pandemonium_vector2_aspect(const pandemonium_vector2 *p_self) {
	const Vector2 *self = (const Vector2 *)p_self;
	return self->aspect();
}

pandemonium_string GDAPI pandemonium_vector2_as_string(const pandemonium_vector2 *p_self) {
	pandemonium_string ret;
	const Vector2 *self = (const Vector2 *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_vector2_new(pandemonium_vector2 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y) {
	Vector2 *dest = (Vector2 *)r_dest;
	*dest = Vector2(p_x, p_y);
}

#ifdef __cplusplus
}
#endif
