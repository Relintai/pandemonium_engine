/**************************************************************************/
/*  basis.cpp                                                             */
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

#include "gdn/basis.h"

#include "core/math/basis.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_basis) == sizeof(Basis), "Basis size mismatch");

void GDAPI pandemonium_basis_invert(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->invert();
}
void GDAPI pandemonium_basis_transpose(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->transpose();
}

pandemonium_basis GDAPI pandemonium_basis_inverse(const pandemonium_basis *p_self) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	*((Basis *)&dest) = self->inverse();
	return dest;
}

pandemonium_basis GDAPI pandemonium_basis_transposed(const pandemonium_basis *p_self) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	*((Basis *)&dest) = self->transposed();
	return dest;
}

pandemonium_real GDAPI pandemonium_basis_determinant(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->determinant();
}

void GDAPI pandemonium_basis_from_z(pandemonium_basis *p_self, const pandemonium_vector3 *p_z) {
	Basis *self = (Basis *)p_self;
	const Vector3 *z = (const Vector3 *)p_z;
	self->from_z(*z);
}

void GDAPI pandemonium_basis_rotate(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Basis *self = (Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->rotate(*axis, p_phi);
}

pandemonium_basis GDAPI pandemonium_basis_rotated(const pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	*((Basis *)&dest) = self->rotated(*axis, p_phi);
	return dest;
}

void GDAPI pandemonium_basis_rotate_local(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Basis *self = (Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->rotate_local(*axis, p_phi);
}
pandemonium_basis GDAPI pandemonium_basis_rotated_local(const pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	*((Basis *)&dest) = self->rotated_local(*axis, p_phi);
	return dest;
}

void GDAPI pandemonium_basis_rotate_euler(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->rotate(*euler);
}
pandemonium_basis GDAPI pandemonium_basis_rotated_euler(const pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	*((Basis *)&dest) = self->rotated(*euler);
	return dest;
}

void GDAPI pandemonium_basis_rotate_quat(pandemonium_basis *p_self, const pandemonium_quaternion *p_quat) {
	Basis *self = (Basis *)p_self;
	const Quaternion *quat = (const Quaternion *)p_quat;
	self->rotate(*quat);
}
pandemonium_basis GDAPI pandemonium_basis_rotated_quat(const pandemonium_basis *p_self, const pandemonium_quaternion *p_quat) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Quaternion *quat = (const Quaternion *)p_quat;
	*((Basis *)&dest) = self->rotated(*quat);
	return dest;
}

void GDAPI pandemonium_basis_rotatev(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->rotatev(*euler);
}
pandemonium_basis GDAPI pandemonium_basis_rotatedv(const pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	*((Basis *)&dest) = self->rotatedv(*euler);
	return dest;
}
void GDAPI pandemonium_basis_rotateq(pandemonium_basis *p_self, const pandemonium_quaternion *p_quat) {
	Basis *self = (Basis *)p_self;
	const Quaternion *quat = (const Quaternion *)p_quat;
	self->rotateq(*quat);
}
pandemonium_basis GDAPI pandemonium_basis_rotatedq(const pandemonium_basis *p_self, const pandemonium_quaternion *p_quat) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Quaternion *quat = (const Quaternion *)p_quat;
	*((Basis *)&dest) = self->rotatedq(*quat);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_basis_get_rotation_euler(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_rotation_euler();
	return dest;
}
void GDAPI pandemonium_basis_get_rotation_axis_angle(const pandemonium_basis *p_self, pandemonium_vector3 *p_axis, pandemonium_real p_angle) {
	const Basis *self = (const Basis *)p_self;
	Vector3 *axis = (Vector3 *)p_axis;
	self->get_rotation_axis_angle(*axis, p_angle);
}
void GDAPI pandemonium_basis_get_rotation_axis_angle_local(const pandemonium_basis *p_self, pandemonium_vector3 *p_axis, pandemonium_real p_angle) {
	const Basis *self = (const Basis *)p_self;
	Vector3 *axis = (Vector3 *)p_axis;
	self->get_rotation_axis_angle_local(*axis, p_angle);
}
pandemonium_quaternion GDAPI pandemonium_basis_get_rotation_quaternion(const pandemonium_basis *p_self) {
	pandemonium_quaternion dest;
	const Basis *self = (const Basis *)p_self;
	*((Quaternion *)&dest) = self->get_rotation_quaternion();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_basis_get_rotation(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_rotation();
	return dest;
}

void GDAPI pandemonium_basis_rotate_to_align(pandemonium_basis *p_self, const pandemonium_vector3 *p_start_direction, const pandemonium_vector3 *p_end_direction) {
	Basis *self = (Basis *)p_self;
	const Vector3 *start_direction = (const Vector3 *)p_start_direction;
	const Vector3 *end_direction = (const Vector3 *)p_end_direction;
	self->rotate_to_align(*start_direction, *end_direction);
}

pandemonium_vector3 GDAPI pandemonium_basis_rotref_posscale_decomposition(const pandemonium_basis *p_self, pandemonium_basis *rotref) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	Basis *tref = (Basis *)rotref;
	*((Vector3 *)&dest) = self->rotref_posscale_decomposition(*tref);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_xyz(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_xyz();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_xyz(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_xyz(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_xzy(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_xzy();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_xzy(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_xzy(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_yzx(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_yzx();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_yzx(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_yzx(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_yxz(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_yxz();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_yxz(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_yxz(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_zxy(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_zxy();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_zxy(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_zxy(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_zyx(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler_zyx();
	return dest;
}
void GDAPI pandemonium_basis_set_euler_zyx(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler_zyx(*euler);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_euler(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_euler();
	return dest;
}
void GDAPI pandemonium_basis_set_euler(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	self->set_euler(*euler);
}

pandemonium_quaternion GDAPI pandemonium_basis_get_quaternion(const pandemonium_basis *p_self) {
	pandemonium_quaternion dest;
	const Basis *self = (const Basis *)p_self;
	*((Quaternion *)&dest) = self->get_quaternion();
	return dest;
}

void GDAPI pandemonium_basis_set_quaternion(pandemonium_basis *p_self, const pandemonium_quaternion *p_quaternion) {
	Basis *self = (Basis *)p_self;
	const Quaternion *quaternion = (const Quaternion *)p_quaternion;
	self->set_quaternion(*quaternion);
}

void GDAPI pandemonium_basis_get_axis_angle(const pandemonium_basis *p_self, pandemonium_vector3 *r_axis, pandemonium_real *r_angle) {
	const Basis *self = (const Basis *)p_self;
	Vector3 *axis = (Vector3 *)r_axis;
	self->get_axis_angle(*axis, *r_angle);
}
void GDAPI pandemonium_basis_set_axis_angle(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Basis *self = (Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->set_axis_angle(*axis, p_phi);
}

void GDAPI pandemonium_basis_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->scale(*scale);
}

pandemonium_basis GDAPI pandemonium_basis_scaled(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Basis *)&dest) = self->scaled(*scale);
	return dest;
}

void GDAPI pandemonium_basis_scale_local(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->scale_local(*scale);
}
pandemonium_basis GDAPI pandemonium_basis_scaled_local(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Basis *)&dest) = self->scaled_local(*scale);
	return dest;
}

void GDAPI pandemonium_basis_scale_orthogonal(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->scale_orthogonal(*scale);
}
pandemonium_basis GDAPI pandemonium_basis_scaled_orthogonal(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Basis *)&dest) = self->scaled_orthogonal(*scale);
	return dest;
}

void GDAPI pandemonium_basis_make_scale_uniform(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->make_scale_uniform();
}
pandemonium_real GDAPI pandemonium_basis_get_uniform_scale(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->get_uniform_scale();
}

pandemonium_vector3 GDAPI pandemonium_basis_get_scale(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_scale();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_basis_get_scale_abs(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_scale_abs();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_basis_get_scale_local(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_scale_local();
	return dest;
}

void GDAPI pandemonium_basis_set_axis_angle_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->set_axis_angle_scale(*axis, p_phi, *scale);
}

void GDAPI pandemonium_basis_set_euler_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Vector3 *euler = (const Vector3 *)p_euler;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->set_euler_scale(*euler, *scale);
}

void GDAPI pandemonium_basis_set_quaternion_scale(pandemonium_basis *p_self, const pandemonium_quaternion *p_quaternion, const pandemonium_vector3 *p_scale) {
	Basis *self = (Basis *)p_self;
	const Quaternion *quaternion = (const Quaternion *)p_quaternion;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->set_quaternion_scale(*quaternion, *scale);
}

pandemonium_real GDAPI pandemonium_basis_tdotx(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with) {
	const Basis *self = (const Basis *)p_self;
	const Vector3 *with = (const Vector3 *)p_with;
	return self->tdotx(*with);
}

pandemonium_real GDAPI pandemonium_basis_tdoty(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with) {
	const Basis *self = (const Basis *)p_self;
	const Vector3 *with = (const Vector3 *)p_with;
	return self->tdoty(*with);
}

pandemonium_real GDAPI pandemonium_basis_tdotz(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with) {
	const Basis *self = (const Basis *)p_self;
	const Vector3 *with = (const Vector3 *)p_with;
	return self->tdotz(*with);
}

pandemonium_bool GDAPI pandemonium_basis_is_equal_approx(const pandemonium_basis *p_self, const pandemonium_basis *p_basis) {
	const Basis *self = (const Basis *)p_self;
	const Basis *basis = (const Basis *)p_basis;
	return self->is_equal_approx(*basis);
}
pandemonium_bool GDAPI pandemonium_basis_is_equal_approx_ratio(const pandemonium_basis *p_self, const pandemonium_basis *p_a, const pandemonium_basis *p_b) {
	const Basis *self = (const Basis *)p_self;
	const Basis *a = (const Basis *)p_a;
	const Basis *b = (const Basis *)p_b;
	return self->is_equal_approx_ratio(*a, *b);
}
pandemonium_bool GDAPI pandemonium_basis_is_equal_approx_ratio_eps(const pandemonium_basis *p_self, const pandemonium_basis *p_a, const pandemonium_basis *p_b, pandemonium_real p_epsilon) {
	const Basis *self = (const Basis *)p_self;
	const Basis *a = (const Basis *)p_a;
	const Basis *b = (const Basis *)p_b;
	return self->is_equal_approx_ratio(*a, *b, p_epsilon);
}

pandemonium_bool GDAPI pandemonium_basis_operator_equal(const pandemonium_basis *p_self, const pandemonium_basis *p_b) {
	const Basis *self = (const Basis *)p_self;
	const Basis *b = (const Basis *)p_b;
	return *self == *b;
}

pandemonium_vector3 GDAPI pandemonium_basis_xform(const pandemonium_basis *p_self, const pandemonium_vector3 *p_v) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	*((Vector3 *)&dest) = self->xform(*v);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_basis_xform_inv(const pandemonium_basis *p_self, const pandemonium_vector3 *p_v) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	*((Vector3 *)&dest) = self->xform_inv(*v);
	return dest;
}

pandemonium_vector3i GDAPI pandemonium_basis_xform_v3i(const pandemonium_basis *p_self, const pandemonium_vector3i *p_vector) {
	pandemonium_vector3i dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3i *vector = (const Vector3i *)p_vector;
	*((Vector3i *)&dest) = self->xform(*vector);
	return dest;
}
pandemonium_vector3i GDAPI pandemonium_basis_xform_inv_v3i(const pandemonium_basis *p_self, const pandemonium_vector3i *p_vector) {
	pandemonium_vector3i dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3i *vector = (const Vector3i *)p_vector;
	*((Vector3i *)&dest) = self->xform_inv(*vector);
	return dest;
}

pandemonium_basis GDAPI pandemonium_basis_operator_multiply(const pandemonium_basis *p_self, const pandemonium_basis *p_b) {
	pandemonium_basis raw_dest;
	Basis *dest = (Basis *)&raw_dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *b = (const Basis *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_basis GDAPI pandemonium_basis_operator_add(const pandemonium_basis *p_self, const pandemonium_basis *p_b) {
	pandemonium_basis raw_dest;
	Basis *dest = (Basis *)&raw_dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *b = (const Basis *)p_b;
	*dest = *self + *b;
	return raw_dest;
}

pandemonium_basis GDAPI pandemonium_basis_operator_subtract(const pandemonium_basis *p_self, const pandemonium_basis *p_b) {
	pandemonium_basis raw_dest;
	Basis *dest = (Basis *)&raw_dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *b = (const Basis *)p_b;
	*dest = *self - *b;
	return raw_dest;
}

pandemonium_basis GDAPI pandemonium_basis_operator_multiply_scalar(const pandemonium_basis *p_self, const pandemonium_real p_b) {
	pandemonium_basis raw_dest;
	Basis *dest = (Basis *)&raw_dest;
	const Basis *self = (const Basis *)p_self;
	*dest = *self * p_b;
	return raw_dest;
}

pandemonium_int GDAPI pandemonium_basis_get_orthogonal_index(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->get_orthogonal_index();
}

void GDAPI pandemonium_basis_set_orthogonal_index(pandemonium_basis *p_self, pandemonium_int p_index) {
	Basis *self = (Basis *)p_self;
	self->set_orthogonal_index(p_index);
}

void GDAPI pandemonium_basis_set_diagonal(pandemonium_basis *p_self, const pandemonium_vector3 *p_diag) {
	Basis *self = (Basis *)p_self;
	const Vector3 *diag = (const Vector3 *)p_diag;
	self->set_diagonal(*diag);
}

pandemonium_bool GDAPI pandemonium_basis_is_orthogonal(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->is_orthogonal();
}
pandemonium_bool GDAPI pandemonium_basis_is_diagonal(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->is_diagonal();
}
pandemonium_bool GDAPI pandemonium_basis_is_rotation(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->is_rotation();
}

pandemonium_basis GDAPI pandemonium_basis_slerp(const pandemonium_basis *p_self, const pandemonium_basis *p_b, const pandemonium_real p_t) {
	pandemonium_basis raw_dest;
	Basis *dest = (Basis *)&raw_dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *b = (const Basis *)p_b;
	*dest = self->slerp(*b, p_t);
	return raw_dest;
}
pandemonium_basis GDAPI pandemonium_basis_lerp(const pandemonium_basis *p_self, const pandemonium_basis *p_to, const pandemonium_real p_weight) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *to = (const Basis *)p_to;
	*((Basis *)&dest) = self->lerp(*to, p_weight);
	return dest;
}
void GDAPI pandemonium_basis_rotate_sh(pandemonium_basis *p_self, pandemonium_real *p_values) {
	Basis *self = (Basis *)p_self;
	self->rotate_sh(p_values);
}

pandemonium_string GDAPI pandemonium_basis_as_string(const pandemonium_basis *p_self) {
	pandemonium_string ret;
	const Basis *self = (const Basis *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_basis_set_real(pandemonium_basis *p_self, pandemonium_real xx, pandemonium_real xy, pandemonium_real xz, pandemonium_real yx, pandemonium_real yy, pandemonium_real yz, pandemonium_real zx, pandemonium_real zy, pandemonium_real zz) {
	Basis *self = (Basis *)p_self;
	self->set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}
void GDAPI pandemonium_basis_set_vector3(pandemonium_basis *p_self, const pandemonium_vector3 *p_x, const pandemonium_vector3 *p_y, const pandemonium_vector3 *p_z) {
	Basis *self = (Basis *)p_self;
	const Vector3 *x = (const Vector3 *)p_x;
	const Vector3 *y = (const Vector3 *)p_y;
	const Vector3 *z = (const Vector3 *)p_z;
	self->set(*x, *y, *z);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_column(const pandemonium_basis *p_self, pandemonium_int i) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_column(i);
	return dest;
}
void GDAPI pandemonium_basis_set_column(pandemonium_basis *p_self, pandemonium_int p_index, const pandemonium_vector3 *p_value) {
	Basis *self = (Basis *)p_self;
	const Vector3 *value = (const Vector3 *)p_value;
	self->set_column(p_index, *value);
}
void GDAPI pandemonium_basis_set_columns(pandemonium_basis *p_self, const pandemonium_vector3 *p_x, const pandemonium_vector3 *p_y, const pandemonium_vector3 *p_z) {
	Basis *self = (Basis *)p_self;
	const Vector3 *x = (const Vector3 *)p_x;
	const Vector3 *y = (const Vector3 *)p_y;
	const Vector3 *z = (const Vector3 *)p_z;
	self->set_columns(*x, *y, *z);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_row(const pandemonium_basis *p_self, const pandemonium_int p_row) {
	pandemonium_vector3 dest;
	Vector3 *d = (Vector3 *)&dest;
	const Basis *self = (const Basis *)p_self;
	*d = self->get_row(p_row);
	return dest;
}

void GDAPI pandemonium_basis_set_row(pandemonium_basis *p_self, const pandemonium_int p_row, const pandemonium_vector3 *p_value) {
	Basis *self = (Basis *)p_self;
	const Vector3 *value = (const Vector3 *)p_value;
	self->set_row(p_row, *value);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_axis(const pandemonium_basis *p_self, const pandemonium_int p_axis) {
	pandemonium_vector3 dest;
	Vector3 *d = (Vector3 *)&dest;
	const Basis *self = (const Basis *)p_self;
	*d = self->get_axis(p_axis);
	return dest;
}

void GDAPI pandemonium_basis_set_axis(pandemonium_basis *p_self, const pandemonium_int p_axis, const pandemonium_vector3 *p_value) {
	Basis *self = (Basis *)p_self;
	const Vector3 *value = (const Vector3 *)p_value;
	self->set_axis(p_axis, *value);
}

pandemonium_vector3 GDAPI pandemonium_basis_get_main_diagonal(const pandemonium_basis *p_self) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	*((Vector3 *)&dest) = self->get_main_diagonal();
	return dest;
}

void GDAPI pandemonium_basis_set_zero(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->set_zero();
}

pandemonium_basis GDAPI pandemonium_basis_transpose_xform(const pandemonium_basis *p_self, const pandemonium_basis *p_m) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	const Basis *m = (const Basis *)p_m;
	*((Basis *)&dest) = self->transpose_xform(*m);
	return dest;
}

void GDAPI pandemonium_basis_orthonormalize(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->orthonormalize();
}

pandemonium_basis GDAPI pandemonium_basis_orthonormalized(const pandemonium_basis *p_self) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	*((Basis *)&dest) = self->orthonormalized();
	return dest;
}

void GDAPI pandemonium_basis_orthogonalize(pandemonium_basis *p_self) {
	Basis *self = (Basis *)p_self;
	self->orthogonalize();
}
pandemonium_basis GDAPI pandemonium_basis_orthogonalized(const pandemonium_basis *p_self) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	*((Basis *)&dest) = self->orthogonalized();
	return dest;
}

pandemonium_bool GDAPI pandemonium_basis_is_symmetric(const pandemonium_basis *p_self) {
	const Basis *self = (const Basis *)p_self;
	return self->is_symmetric();
}
pandemonium_basis GDAPI pandemonium_basis_diagonalize(pandemonium_basis *p_self) {
	pandemonium_basis dest;
	Basis *self = (Basis *)p_self;
	*((Basis *)&dest) = self->diagonalize();
	return dest;
}

pandemonium_basis GDAPI pandemonium_basis_get_normal_xform_basis(const pandemonium_basis *p_self) {
	pandemonium_basis dest;
	const Basis *self = (const Basis *)p_self;
	*((Basis *)&dest) = self->get_normal_xform_basis();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_basis_xform_normal_fast(const pandemonium_basis *p_self, const pandemonium_vector3 *p_vector) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *vector = (const Vector3 *)p_vector;
	*((Vector3 *)&dest) = self->xform_normal_fast(*vector);
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_basis_xform_normal(const pandemonium_basis *p_self, const pandemonium_vector3 *p_vector) {
	pandemonium_vector3 dest;
	const Basis *self = (const Basis *)p_self;
	const Vector3 *vector = (const Vector3 *)p_vector;
	*((Vector3 *)&dest) = self->xform_normal(*vector);
	return dest;
}

pandemonium_basis GDAPI pandemonium_basis_looking_at(const pandemonium_vector3 *p_target) {
	pandemonium_basis dest;
	const Vector3 *target = (const Vector3 *)p_target;
	*((Basis *)&dest) = Basis::looking_at(*target);
	return dest;
}
pandemonium_basis GDAPI pandemonium_basis_looking_at_up(const pandemonium_vector3 *p_target, const pandemonium_vector3 *p_up) {
	pandemonium_basis dest;
	const Vector3 *target = (const Vector3 *)p_target;
	const Vector3 *up = (const Vector3 *)p_up;
	*((Basis *)&dest) = Basis::looking_at(*target, *up);
	return dest;
}
pandemonium_basis GDAPI pandemonium_basis_from_scale(const pandemonium_vector3 *p_scale) {
	pandemonium_basis dest;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Basis *)&dest) = Basis::from_scale(*scale);
	return dest;
}

// p_elements is a pointer to an array of 3 (!!) vector3
void GDAPI pandemonium_basis_get_elements(const pandemonium_basis *p_self, pandemonium_vector3 *p_elements) {
	const Basis *self = (const Basis *)p_self;
	Vector3 *elements = (Vector3 *)p_elements;
	elements[0] = self->rows[0];
	elements[1] = self->rows[1];
	elements[2] = self->rows[2];
}

void GDAPI pandemonium_basis_new(pandemonium_basis *r_dest) {
	Basis *dest = (Basis *)r_dest;
	*dest = Basis();
}

void GDAPI pandemonium_basis_new_with_euler_quaternion(pandemonium_basis *r_dest, const pandemonium_quaternion *p_euler) {
	Basis *dest = (Basis *)r_dest;
	const Quaternion *euler = (const Quaternion *)p_euler;
	*dest = Basis(*euler);
}

void GDAPI pandemonium_basis_new_with_rows(pandemonium_basis *r_dest, const pandemonium_vector3 *p_x_axis, const pandemonium_vector3 *p_y_axis, const pandemonium_vector3 *p_z_axis) {
	const Vector3 *x_axis = (const Vector3 *)p_x_axis;
	const Vector3 *y_axis = (const Vector3 *)p_y_axis;
	const Vector3 *z_axis = (const Vector3 *)p_z_axis;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*x_axis, *y_axis, *z_axis);
}

void GDAPI pandemonium_basis_new_with_axis_and_angle(pandemonium_basis *r_dest, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi) {
	const Vector3 *axis = (const Vector3 *)p_axis;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*axis, p_phi);
}

void GDAPI pandemonium_basis_new_with_euler(pandemonium_basis *r_dest, const pandemonium_vector3 *p_euler) {
	const Vector3 *euler = (const Vector3 *)p_euler;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*euler);
}

void GDAPI pandemonium_basis_new_quaternion_scale(pandemonium_basis *r_dest, const pandemonium_quaternion *p_quat, const pandemonium_vector3 *p_scale) {
	const Quaternion *quat = (const Quaternion *)p_quat;
	const Vector3 *scale = (const Vector3 *)p_scale;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*quat, *scale);
}
void GDAPI pandemonium_basis_new_euler_scale(pandemonium_basis *r_dest, const pandemonium_vector3 *p_euler, const pandemonium_vector3 *p_scale) {
	const Vector3 *euler = (const Vector3 *)p_euler;
	const Vector3 *scale = (const Vector3 *)p_scale;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*euler, *scale);
}
void GDAPI pandemonium_basis_new_axis_phi_scale(pandemonium_basis *r_dest, const pandemonium_vector3 *p_axis, pandemonium_real p_phi, const pandemonium_vector3 *p_scale) {
	const Vector3 *axis = (const Vector3 *)p_axis;
	const Vector3 *scale = (const Vector3 *)p_scale;
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(*axis, p_phi, *scale);
}
void GDAPI pandemonium_basis_new_floats(pandemonium_basis *r_dest, pandemonium_real xx, pandemonium_real xy, pandemonium_real xz, pandemonium_real yx, pandemonium_real yy, pandemonium_real yz, pandemonium_real zx, pandemonium_real zy, pandemonium_real zz) {
	Basis *dest = (Basis *)r_dest;
	*dest = Basis(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}

#ifdef __cplusplus
}
#endif
