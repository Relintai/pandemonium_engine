/**************************************************************************/
/*  vector3i.cpp                                                           */
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

#include "gdn/vector3i.h"

#include "core/math/vector3i.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_vector3i) == sizeof(Vector3i), "Vector3i size mismatch");

pandemonium_int GDAPI pandemonium_vector3i_get_x(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->x;
}

void GDAPI pandemonium_vector3i_set_x(pandemonium_vector3i *p_self, const pandemonium_int val) {
	Vector3i *self = (Vector3i *)p_self;
	self->x = val;
}

pandemonium_int GDAPI pandemonium_vector3i_get_y(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->y;
}

void GDAPI pandemonium_vector3i_set_y(pandemonium_vector3i *p_self, const pandemonium_int val) {
	Vector3i *self = (Vector3i *)p_self;
	self->y = val;
}

pandemonium_int GDAPI pandemonium_vector3i_get_z(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->z;
}

void GDAPI pandemonium_vector3i_set_z(pandemonium_vector3i *p_self, const pandemonium_int val) {
	Vector3i *self = (Vector3i *)p_self;
	self->z = val;
}

pandemonium_int GDAPI *pandemonium_vector3i_operator_index(pandemonium_vector3i *p_self, const pandemonium_int p_idx) {
	Vector3i *self = (Vector3i *)p_self;
	return (pandemonium_int *)&self->operator[](p_idx);
}
const pandemonium_int GDAPI *pandemonium_vector3i_operator_index_const(const pandemonium_vector3i *p_self, const pandemonium_int p_idx) {
	const Vector3i *self = (const Vector3i *)p_self;
	return (const pandemonium_int *)&self->operator[](p_idx);
}

void GDAPI pandemonium_vector3i_set_axis(pandemonium_vector3i *p_self, const pandemonium_vector3i_axis p_axis, const pandemonium_int p_val) {
	Vector3i *self = (Vector3i *)p_self;
	self->set_axis(p_axis, p_val);
}
pandemonium_int GDAPI pandemonium_vector3i_get_axis(const pandemonium_vector3i *p_self, const pandemonium_vector3i_axis p_axis) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->get_axis(p_axis);
}

void GDAPI pandemonium_vector3i_set_all(pandemonium_vector3i *p_self, const pandemonium_int p_value) {
	Vector3i *self = (Vector3i *)p_self;
	self->set_all(p_value);
}

pandemonium_int GDAPI pandemonium_vector3i_min_axis(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->min_axis();
}
pandemonium_int GDAPI pandemonium_vector3i_max_axis(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->max_axis();
}

pandemonium_real GDAPI pandemonium_vector3i_length(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->length();
}
pandemonium_int GDAPI pandemonium_vector3i_length_squared(const pandemonium_vector3i *p_self) {
	const Vector3i *self = (const Vector3i *)p_self;
	return self->length_squared();
}

void GDAPI pandemonium_vector3i_zero(pandemonium_vector3i *p_self) {
	Vector3i *self = (Vector3i *)p_self;
	self->zero();
}

pandemonium_vector3i GDAPI pandemonium_vector3i_linear_interpolate(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b, const pandemonium_real p_t) {
	pandemonium_vector3i dest;
	const Vector3i *self = (const Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	*((Vector3i *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_abs(const pandemonium_vector3i *p_self) {
	pandemonium_vector3i dest;
	const Vector3i *self = (const Vector3i *)p_self;
	*((Vector3i *)&dest) = self->abs();
	return dest;
}
pandemonium_vector3i GDAPI pandemonium_vector3i_sign(const pandemonium_vector3i *p_self) {
	pandemonium_vector3i dest;
	const Vector3i *self = (const Vector3i *)p_self;
	*((Vector3i *)&dest) = self->sign();
	return dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_clamp(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_min, const pandemonium_vector3i *p_max) {
	pandemonium_vector3i dest;
	const Vector3i *self = (const Vector3i *)p_self;
	const Vector3i *min = (const Vector3i *)p_min;
	const Vector3i *max = (const Vector3i *)p_max;
	*((Vector3i *)&dest) = self->clamp(*min, *max);
	return dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_add(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	*dest = *self + *b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_subtract(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_multiply_vector(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_multiply_scalar(const pandemonium_vector3i *p_self, const pandemonium_int p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_divide_vector(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	*dest = *self / *b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_divide_scalar(const pandemonium_vector3i *p_self, const pandemonium_int p_b) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	Vector3i *self = (Vector3i *)p_self;
	*dest = *self / p_b;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_vector3i_operator_neg(const pandemonium_vector3i *p_self) {
	pandemonium_vector3i raw_dest;
	Vector3i *dest = (Vector3i *)&raw_dest;
	const Vector3i *self = (const Vector3i *)p_self;
	*dest = -(*self);
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_vector3i_operator_equal(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_vector3i_operator_less(const pandemonium_vector3i *p_self, const pandemonium_vector3i *p_b) {
	Vector3i *self = (Vector3i *)p_self;
	const Vector3i *b = (const Vector3i *)p_b;
	return *self < *b;
}

pandemonium_vector3 GDAPI pandemonium_vector3i_to_vector3(const pandemonium_vector3i *p_self) {
	pandemonium_vector3 dest;
	const Vector3i *self = (const Vector3i *)p_self;
	*((Vector3 *)&dest) = self->to_vector3();
	return dest;
}

pandemonium_string GDAPI pandemonium_vector3i_as_string(const pandemonium_vector3i *p_self) {
	pandemonium_string ret;
	const Vector3i *self = (const Vector3i *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_vector3i_new(pandemonium_vector3i *r_dest, const pandemonium_int p_x, const pandemonium_int p_y, const pandemonium_int p_z) {
	Vector3i *dest = (Vector3i *)r_dest;
	*dest = Vector3i(p_x, p_y, p_z);
}

#ifdef __cplusplus
}
#endif
