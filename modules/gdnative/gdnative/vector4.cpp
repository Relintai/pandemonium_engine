/*************************************************************************/
/*  vector4.cpp                                                          */
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

#include "gdn/vector4.h"

#include "core/math/vector4.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_vector4) == sizeof(Vector4), "Vector4 size mismatch");

pandemonium_real GDAPI pandemonium_vector4_get_x(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->x;
}

void GDAPI pandemonium_vector4_set_x(pandemonium_vector4 *p_self, const pandemonium_real val) {
	Vector4 *self = (Vector4 *)p_self;
	self->x = val;
}

pandemonium_real GDAPI pandemonium_vector4_get_y(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->y;
}

void GDAPI pandemonium_vector4_set_y(pandemonium_vector4 *p_self, const pandemonium_real val) {
	Vector4 *self = (Vector4 *)p_self;
	self->y = val;
}

pandemonium_real GDAPI pandemonium_vector4_get_z(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->z;
}

void GDAPI pandemonium_vector4_set_z(pandemonium_vector4 *p_self, const pandemonium_real val) {
	Vector4 *self = (Vector4 *)p_self;
	self->z = val;
}

pandemonium_real GDAPI pandemonium_vector4_get_w(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->w;
}

void GDAPI pandemonium_vector4_set_w(pandemonium_vector4 *p_self, const pandemonium_real val) {
	Vector4 *self = (Vector4 *)p_self;
	self->w = val;
}

pandemonium_real GDAPI *pandemonium_vector4_operator_index(pandemonium_vector4 *p_self, const pandemonium_int p_idx) {
	Vector4 *self = (Vector4 *)p_self;
	return (pandemonium_real *)&self->operator[](p_idx);
}
const pandemonium_real GDAPI *pandemonium_vector4_operator_index_const(const pandemonium_vector4 *p_self, const pandemonium_int p_idx) {
	const Vector4 *self = (const Vector4 *)p_self;
	return (const pandemonium_real *)&self->operator[](p_idx);
}

void GDAPI pandemonium_vector4_set_axis(pandemonium_vector4 *p_self, const pandemonium_vector4_axis p_axis, const pandemonium_real p_val) {
	Vector4 *self = (Vector4 *)p_self;
	self->set_axis(p_axis, p_val);
}
pandemonium_real GDAPI pandemonium_vector4_get_axis(const pandemonium_vector4 *p_self, const pandemonium_vector4_axis p_axis) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->get_axis(p_axis);
}

void GDAPI pandemonium_vector4_set_all(pandemonium_vector4 *p_self, const pandemonium_real p_value) {
	Vector4 *self = (Vector4 *)p_self;
	self->set_all(p_value);
}

pandemonium_int GDAPI pandemonium_vector4_min_axis(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->min_axis();
}
pandemonium_int GDAPI pandemonium_vector4_max_axis(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->max_axis();
}

pandemonium_real GDAPI pandemonium_vector4_length(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->length();
}
pandemonium_real GDAPI pandemonium_vector4_length_squared(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->length_squared();
}

void GDAPI pandemonium_vector4_normalize(pandemonium_vector4 *p_self) {
	Vector4 *self = (Vector4 *)p_self;
	self->normalize();
}
pandemonium_vector4 GDAPI pandemonium_vector4_normalized(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->normalized();
	return dest;
}
pandemonium_bool GDAPI pandemonium_vector4_is_normalized(const pandemonium_vector4 *p_self) {
	const Vector4 *self = (const Vector4 *)p_self;
	return self->is_normalized();
}
pandemonium_vector4 GDAPI pandemonium_vector4_inverse(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->inverse();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_limit_length(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->limit_length();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_limit_length_len(const pandemonium_vector4 *p_self, const pandemonium_real p_val) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->limit_length(p_val);
	return dest;
}

void GDAPI pandemonium_vector4_zero(pandemonium_vector4 *p_self) {
	Vector4 *self = (Vector4 *)p_self;
	self->zero();
}

void GDAPI pandemonium_vector4_snap(pandemonium_vector4 *p_self, const pandemonium_vector4 *p_val) {
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *val = (const Vector4 *)p_val;
	self->snap(*val);
}
pandemonium_vector4 GDAPI pandemonium_vector4_snapped(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_by) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *snap_axis = (const Vector4 *)p_by;

	*((Vector4 *)&dest) = self->snapped(*snap_axis);
	return dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_linear_interpolate(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b, const pandemonium_real p_t) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	*((Vector4 *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_cubic_interpolate(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b, const pandemonium_vector4 *p_pre_a, const pandemonium_vector4 *p_post_b, const pandemonium_real p_t) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	const Vector4 *pre_a = (const Vector4 *)p_pre_a;
	const Vector4 *post_b = (const Vector4 *)p_post_b;
	*((Vector4 *)&dest) = self->cubic_interpolate(*b, *pre_a, *post_b, p_t);
	return dest;
}

pandemonium_real GDAPI pandemonium_vector4_dot(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	return self->dot(*b);
}

pandemonium_vector4 GDAPI pandemonium_vector4_abs(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->abs();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_floor(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->floor();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_sign(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->sign();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_ceil(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->ceil();
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_round(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->round();
	return dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_posmod(const pandemonium_vector4 *p_self, const pandemonium_real p_mod) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*((Vector4 *)&dest) = self->posmod(p_mod);
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_vector4_posmodv(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_modv) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *modv = (const Vector4 *)p_modv;
	*((Vector4 *)&dest) = self->posmodv(*modv);
	return dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_direction_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_to) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *to = (const Vector4 *)p_to;
	*((Vector4 *)&dest) = self->direction_to(*to);
	return dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_clamp(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_min, const pandemonium_vector4 *p_max) {
	pandemonium_vector4 dest;
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *min = (const Vector4 *)p_min;
	const Vector4 *max = (const Vector4 *)p_max;
	*((Vector4 *)&dest) = self->clamp(*min, *max);
	return dest;
}

pandemonium_real GDAPI pandemonium_vector4_distance_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	return self->distance_to(*b);
}

pandemonium_real GDAPI pandemonium_vector4_distance_squared_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	return self->distance_squared_to(*b);
}

pandemonium_bool GDAPI pandemonium_vector4_is_equal_approx(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_v) {
	const Vector4 *self = (const Vector4 *)p_self;
	const Vector4 *v = (const Vector4 *)p_v;
	return self->is_equal_approx(*v);
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_add(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	*dest = *self + *b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_subtract(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_multiply_vector(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_multiply_scalar(const pandemonium_vector4 *p_self, const pandemonium_real p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_divide_vector(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	*dest = *self / *b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_divide_scalar(const pandemonium_vector4 *p_self, const pandemonium_real p_b) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	Vector4 *self = (Vector4 *)p_self;
	*dest = *self / p_b;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_vector4_operator_neg(const pandemonium_vector4 *p_self) {
	pandemonium_vector4 raw_dest;
	Vector4 *dest = (Vector4 *)&raw_dest;
	const Vector4 *self = (const Vector4 *)p_self;
	*dest = -(*self);
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_vector4_operator_equal(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_vector4_operator_less(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b) {
	Vector4 *self = (Vector4 *)p_self;
	const Vector4 *b = (const Vector4 *)p_b;
	return *self < *b;
}

pandemonium_string GDAPI pandemonium_vector4_as_string(const pandemonium_vector4 *p_self) {
	pandemonium_string ret;
	const Vector4 *self = (const Vector4 *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_vector4_new(pandemonium_vector4 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_z, const pandemonium_real p_w) {
	Vector4 *dest = (Vector4 *)r_dest;
	*dest = Vector4(p_x, p_y, p_z, p_w);
}

#ifdef __cplusplus
}
#endif
