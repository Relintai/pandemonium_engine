/*************************************************************************/
/*  quaternion.cpp                                                       */
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

#include "gdn/quaternion.h"

#include "core/math/quaternion.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_quaternion) == sizeof(Quaternion), "Quaternion size mismatch");

pandemonium_real GDAPI pandemonium_quaternion_get_x(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->x;
}

void GDAPI pandemonium_quaternion_set_x(pandemonium_quaternion *p_self, const pandemonium_real val) {
	Quaternion *self = (Quaternion *)p_self;
	self->x = val;
}

pandemonium_real GDAPI pandemonium_quaternion_get_y(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->y;
}

void GDAPI pandemonium_quaternion_set_y(pandemonium_quaternion *p_self, const pandemonium_real val) {
	Quaternion *self = (Quaternion *)p_self;
	self->y = val;
}

pandemonium_real GDAPI pandemonium_quaternion_get_z(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->z;
}

void GDAPI pandemonium_quaternion_set_z(pandemonium_quaternion *p_self, const pandemonium_real val) {
	Quaternion *self = (Quaternion *)p_self;
	self->z = val;
}

pandemonium_real GDAPI pandemonium_quaternion_get_w(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->w;
}

void GDAPI pandemonium_quaternion_set_w(pandemonium_quaternion *p_self, const pandemonium_real val) {
	Quaternion *self = (Quaternion *)p_self;
	self->w = val;
}

pandemonium_real GDAPI pandemonium_quaternion_length(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->length();
}

void GDAPI pandemonium_quaternion_normalize(pandemonium_quaternion *p_self) {
	Quaternion *self = (Quaternion *)p_self;
	self->normalize();
}

pandemonium_real GDAPI pandemonium_quaternion_length_squared(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->length_squared();
}

pandemonium_bool GDAPI pandemonium_quaternion_is_equal_approx(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_quat) {
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *quat = (const Quaternion *)p_quat;
	return self->is_equal_approx(*quat);
}

pandemonium_quaternion GDAPI pandemonium_quaternion_normalized(const pandemonium_quaternion *p_self) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Quaternion *)&dest) = self->normalized();
	return dest;
}

pandemonium_bool GDAPI pandemonium_quaternion_is_normalized(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->is_normalized();
}

pandemonium_quaternion GDAPI pandemonium_quaternion_inverse(const pandemonium_quaternion *p_self) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Quaternion *)&dest) = self->inverse();
	return dest;
}
pandemonium_quaternion GDAPI pandemonium_quaternion_log(const pandemonium_quaternion *p_self) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Quaternion *)&dest) = self->log();
	return dest;
}
pandemonium_quaternion GDAPI pandemonium_quaternion_exp(const pandemonium_quaternion *p_self) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Quaternion *)&dest) = self->exp();
	return dest;
}

pandemonium_real GDAPI pandemonium_quaternion_dot(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b) {
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	return self->dot(*b);
}
pandemonium_real GDAPI pandemonium_quaternion_angle_to(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_to) {
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *to = (const Quaternion *)p_to;
	return self->angle_to(*to);
}

void GDAPI pandemonium_quaternion_set_euler_xyz(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler) {
	Quaternion *self = (Quaternion *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_xyz(*euler);
}
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler_xyz(const pandemonium_quaternion *p_self) {
	pandemonium_vector3 dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Vector3 *)&dest) = self->get_euler_xyz();
	return dest;
}
void GDAPI pandemonium_quaternion_set_euler_yxz(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler) {
	Quaternion *self = (Quaternion *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_yxz(*euler);
}
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler_yxz(const pandemonium_quaternion *p_self) {
	pandemonium_vector3 dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Vector3 *)&dest) = self->get_euler_yxz();
	return dest;
}

void GDAPI pandemonium_quaternion_set_euler(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler) {
	Quaternion *self = (Quaternion *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler(*euler);
}
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler(const pandemonium_quaternion *p_self) {
	pandemonium_vector3 dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Vector3 *)&dest) = self->get_euler();
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_quaternion_xform(const pandemonium_quaternion *p_self, const pandemonium_vector3 *p_v) {
	pandemonium_vector3 dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	*((Vector3 *)&dest) = self->xform(*v);
	return dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_slerp(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_real p_t) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	*((Quaternion *)&dest) = self->slerp(*b, p_t);
	return dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_slerpni(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_real p_t) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	*((Quaternion *)&dest) = self->slerpni(*b, p_t);
	return dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_cubic_slerp(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_quaternion *p_pre_a, const pandemonium_quaternion *p_post_b, const pandemonium_real p_t) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	const Quaternion *pre_a = (const Quaternion *)p_pre_a;
	const Quaternion *post_b = (const Quaternion *)p_post_b;
	*((Quaternion *)&dest) = self->cubic_slerp(*b, *pre_a, *post_b, p_t);
	return dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_spherical_cubic_interpolate(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_quaternion *p_pre_a, const pandemonium_quaternion *p_post_b, const pandemonium_real p_weight) {
	pandemonium_quaternion dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	const Quaternion *pre_a = (const Quaternion *)p_pre_a;
	const Quaternion *post_b = (const Quaternion *)p_post_b;
	*((Quaternion *)&dest) = self->spherical_cubic_interpolate(*b, *pre_a, *post_b, p_weight);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_quaternion_get_axis(const pandemonium_quaternion *p_self) {
	pandemonium_vector3 dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*((Vector3 *)&dest) = self->get_axis();
	return dest;
}
pandemonium_real GDAPI pandemonium_quaternion_get_angle(const pandemonium_quaternion *p_self) {
	const Quaternion *self = (const Quaternion *)p_self;
	return self->get_angle();
}

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_multiply(const pandemonium_quaternion *p_self, const pandemonium_real p_b) {
	pandemonium_quaternion raw_dest;
	Quaternion *dest = (Quaternion *)&raw_dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_add(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b) {
	pandemonium_quaternion raw_dest;
	Quaternion *dest = (Quaternion *)&raw_dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	*dest = *self + *b;
	return raw_dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_subtract(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b) {
	pandemonium_quaternion raw_dest;
	Quaternion *dest = (Quaternion *)&raw_dest;
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_divide(const pandemonium_quaternion *p_self, const pandemonium_real p_b) {
	pandemonium_quaternion raw_dest;
	Quaternion *dest = (Quaternion *)&raw_dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*dest = *self / p_b;
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_quaternion_operator_equal(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b) {
	const Quaternion *self = (const Quaternion *)p_self;
	const Quaternion *b = (const Quaternion *)p_b;
	return *self == *b;
}

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_neg(const pandemonium_quaternion *p_self) {
	pandemonium_quaternion raw_dest;
	Quaternion *dest = (Quaternion *)&raw_dest;
	const Quaternion *self = (const Quaternion *)p_self;
	*dest = -(*self);
	return raw_dest;
}

void GDAPI pandemonium_quaternion_set_axis_angle(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_angle) {
	Quaternion *self = (Quaternion *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->set_axis_angle(*axis, p_angle);
}
void GDAPI pandemonium_quaternion_get_axis_angle(const pandemonium_quaternion *p_self, pandemonium_vector3 *r_axis, pandemonium_real r_angle) {
	const Quaternion *self = (const Quaternion *)p_self;
	Vector3 *axis = (Vector3 *)r_axis;
	self->get_axis_angle(*axis, r_angle);
}

void GDAPI pandemonium_quaternion_operator_mul_eq(pandemonium_quaternion *p_self, const pandemonium_quaternion *p_q) {
	Quaternion *self = (Quaternion *)p_self;
	const Quaternion *q = (const Quaternion *)p_q;
	self->operator*=(*q);
}

pandemonium_string GDAPI pandemonium_quaternion_as_string(const pandemonium_quaternion *p_self) {
	pandemonium_string ret;
	const Quaternion *self = (const Quaternion *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

pandemonium_bool GDAPI pandemonium_quaternion_set_shortest_arc(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to) {
	Quaternion *self = (Quaternion *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *to = (const Vector3 *)p_to;
	return self->set_shortest_arc(*from, *to);
}

void GDAPI pandemonium_quaternion_set(pandemonium_quaternion *p_self, pandemonium_real p_x, pandemonium_real p_y, pandemonium_real p_z, pandemonium_real p_w) {
	Quaternion *self = (Quaternion *)p_self;
	self->set(p_x, p_y, p_z, p_w);
}

void GDAPI pandemonium_quaternion_new(pandemonium_quaternion *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_z, const pandemonium_real p_w) {
	Quaternion *dest = (Quaternion *)r_dest;
	*dest = Quaternion(p_x, p_y, p_z, p_w);
}

void GDAPI pandemonium_quaternion_new_with_axis_angle(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_axis, const pandemonium_real p_angle) {
	const Vector3 *axis = (const Vector3 *)p_axis;
	Quaternion *dest = (Quaternion *)r_dest;
	*dest = Quaternion(*axis, p_angle);
}

void GDAPI pandemonium_quaternion_new_with_basis(pandemonium_quaternion *r_dest, const pandemonium_basis *p_basis) {
	const Basis *basis = (const Basis *)p_basis;
	Quaternion *dest = (Quaternion *)r_dest;
	*dest = Quaternion(*basis);
}

void GDAPI pandemonium_quaternion_new_with_euler(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_euler) {
	const Vector3 *euler = (const Vector3 *)p_euler;
	Quaternion *dest = (Quaternion *)r_dest;
	*dest = Quaternion(*euler);
}

void GDAPI pandemonium_quaternion_new_shortest_arc(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_v0, const pandemonium_vector3 *p_v1) {
	const Vector3 *v0 = (const Vector3 *)p_v0;
	const Vector3 *v1 = (const Vector3 *)p_v1;
	Quaternion *dest = (Quaternion *)r_dest;
	*dest = Quaternion(*v0, *v1);
}

#ifdef __cplusplus
}
#endif
