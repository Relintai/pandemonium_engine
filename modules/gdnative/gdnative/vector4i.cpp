/**************************************************************************/
/*  vector4i.cpp                                                           */
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

#include "gdn/vector4i.h"

#include "core/math/vector4i.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_vector4i) == sizeof(Vector4i), "Vector4i size mismatch");

pandemonium_int GDAPI pandemonium_vector4i_get_x(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->x;
}

void GDAPI pandemonium_vector4i_set_x(pandemonium_vector4i *p_self, const pandemonium_int val) {
	Vector4i *self = (Vector4i *)p_self;
	self->x = val;
}

pandemonium_int GDAPI pandemonium_vector4i_get_y(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->y;
}

void GDAPI pandemonium_vector4i_set_y(pandemonium_vector4i *p_self, const pandemonium_int val) {
	Vector4i *self = (Vector4i *)p_self;
	self->y = val;
}

pandemonium_int GDAPI pandemonium_vector4i_get_z(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->z;
}

void GDAPI pandemonium_vector4i_set_z(pandemonium_vector4i *p_self, const pandemonium_int val) {
	Vector4i *self = (Vector4i *)p_self;
	self->z = val;
}

pandemonium_int GDAPI pandemonium_vector4i_get_w(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->w;
}

void GDAPI pandemonium_vector4i_set_w(pandemonium_vector4i *p_self, const pandemonium_int val) {
	Vector4i *self = (Vector4i *)p_self;
	self->w = val;
}

pandemonium_int GDAPI *pandemonium_vector4i_operator_index(pandemonium_vector4i *p_self, const pandemonium_int p_idx) {
	Vector4i *self = (Vector4i *)p_self;
	return (pandemonium_int *)&self->operator[](p_idx);
}
const pandemonium_int GDAPI *pandemonium_vector4i_operator_index_const(const pandemonium_vector4i *p_self, const pandemonium_int p_idx) {
	const Vector4i *self = (const Vector4i *)p_self;
	return (const pandemonium_int *)&self->operator[](p_idx);
}

void GDAPI pandemonium_vector4i_set_axis(pandemonium_vector4i *p_self, const pandemonium_vector4i_axis p_axis, const pandemonium_int p_val) {
	Vector4i *self = (Vector4i *)p_self;
	self->set_axis(p_axis, p_val);
}
pandemonium_int GDAPI pandemonium_vector4i_get_axis(const pandemonium_vector4i *p_self, const pandemonium_vector4i_axis p_axis) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->get_axis(p_axis);
}

void GDAPI pandemonium_vector4i_set_all(pandemonium_vector4i *p_self, const pandemonium_int p_value) {
	Vector4i *self = (Vector4i *)p_self;
	self->set_all(p_value);
}

pandemonium_int GDAPI pandemonium_vector4i_min_axis(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->min_axis();
}
pandemonium_int GDAPI pandemonium_vector4i_max_axis(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->max_axis();
}

pandemonium_real GDAPI pandemonium_vector4i_length(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->length();
}
pandemonium_int GDAPI pandemonium_vector4i_length_squared(const pandemonium_vector4i *p_self) {
	const Vector4i *self = (const Vector4i *)p_self;
	return self->length_squared();
}

void GDAPI pandemonium_vector4i_zero(pandemonium_vector4i *p_self) {
	Vector4i *self = (Vector4i *)p_self;
	self->zero();
}

pandemonium_vector4i GDAPI pandemonium_vector4i_linear_interpolate(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b, const pandemonium_real p_t) {
	pandemonium_vector4i dest;
	const Vector4i *self = (const Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	*((Vector4i *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_abs(const pandemonium_vector4i *p_self) {
	pandemonium_vector4i dest;
	const Vector4i *self = (const Vector4i *)p_self;
	*((Vector4i *)&dest) = self->abs();
	return dest;
}
pandemonium_vector4i GDAPI pandemonium_vector4i_sign(const pandemonium_vector4i *p_self) {
	pandemonium_vector4i dest;
	const Vector4i *self = (const Vector4i *)p_self;
	*((Vector4i *)&dest) = self->sign();
	return dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_clamp(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_min, const pandemonium_vector4i *p_max) {
	pandemonium_vector4i dest;
	const Vector4i *self = (const Vector4i *)p_self;
	const Vector4i *min = (const Vector4i *)p_min;
	const Vector4i *max = (const Vector4i *)p_max;
	*((Vector4i *)&dest) = self->clamp(*min, *max);
	return dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_add(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	*dest = *self + *b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_subtract(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_multiply_vector(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_multiply_scalar(const pandemonium_vector4i *p_self, const pandemonium_int p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_divide_vector(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	*dest = *self / *b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_divide_scalar(const pandemonium_vector4i *p_self, const pandemonium_int p_b) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	Vector4i *self = (Vector4i *)p_self;
	*dest = *self / p_b;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_neg(const pandemonium_vector4i *p_self) {
	pandemonium_vector4i raw_dest;
	Vector4i *dest = (Vector4i *)&raw_dest;
	const Vector4i *self = (const Vector4i *)p_self;
	*dest = -(*self);
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_vector4i_operator_equal(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_vector4i_operator_less(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b) {
	Vector4i *self = (Vector4i *)p_self;
	const Vector4i *b = (const Vector4i *)p_b;
	return *self < *b;
}

pandemonium_vector4 GDAPI pandemonium_vector4i_to_vector4(const pandemonium_vector4i *p_self) {
	pandemonium_vector4 dest;
	const Vector4i *self = (const Vector4i *)p_self;
	*((Vector4 *)&dest) = self->to_vector4();
	return dest;
}

pandemonium_string GDAPI pandemonium_vector4i_as_string(const pandemonium_vector4i *p_self) {
	pandemonium_string ret;
	const Vector4i *self = (const Vector4i *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_vector4i_new(pandemonium_vector4i *r_dest, const pandemonium_int p_x, const pandemonium_int p_y, const pandemonium_int p_z, const pandemonium_int p_w) {
	Vector4i *dest = (Vector4i *)r_dest;
	*dest = Vector4i(p_x, p_y, p_z, p_w);
}

#ifdef __cplusplus
}
#endif
