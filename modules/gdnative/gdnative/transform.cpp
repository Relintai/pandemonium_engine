/*************************************************************************/
/*  transform.cpp                                                        */
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

#include "gdn/transform.h"

#include "core/math/transform.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_transform) == sizeof(Transform), "Transform size mismatch");

void GDAPI pandemonium_transform_invert(pandemonium_transform *p_self) {
	Transform *self = (Transform *)p_self;
	self->invert();
}

pandemonium_transform GDAPI pandemonium_transform_inverse(const pandemonium_transform *p_self) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	*((Transform *)&dest) = self->inverse();
	return dest;
}

void GDAPI pandemonium_transform_affine_invert(pandemonium_transform *p_self) {
	Transform *self = (Transform *)p_self;
	self->affine_invert();
}
pandemonium_transform GDAPI pandemonium_transform_affine_inverse(const pandemonium_transform *p_self) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	*((Transform *)&dest) = self->affine_inverse();
	return dest;
}

pandemonium_transform GDAPI pandemonium_transform_rotated(const pandemonium_transform *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	*((Transform *)&dest) = self->rotated(*axis, p_phi);
	return dest;
}
pandemonium_transform GDAPI pandemonium_transform_rotated_local(const pandemonium_transform *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	*((Transform *)&dest) = self->rotated_local(*axis, p_phi);
	return dest;
}

void GDAPI pandemonium_transform_rotate(pandemonium_transform *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Transform *self = (Transform *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->rotate(*axis, p_phi);
}
void GDAPI pandemonium_transform_rotate_local(pandemonium_transform *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Transform *self = (Transform *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->rotate_local(*axis, p_phi);
}
void GDAPI pandemonium_transform_rotate_basis(pandemonium_transform *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi) {
	Transform *self = (Transform *)p_self;
	const Vector3 *axis = (const Vector3 *)p_axis;
	self->rotate_basis(*axis, p_phi);
}

void GDAPI pandemonium_transform_set_look_at(pandemonium_transform *p_self, const pandemonium_vector3 *p_eye, const pandemonium_vector3 *p_target, const pandemonium_vector3 *p_up, const pandemonium_bool p_use_model_front) {
	Transform *self = (Transform *)p_self;
	const Vector3 *eye = (const Vector3 *)p_eye;
	const Vector3 *target = (const Vector3 *)p_target;
	const Vector3 *up = (const Vector3 *)p_up;
	self->set_look_at(*eye, *target, *up, p_use_model_front);
}

pandemonium_transform GDAPI pandemonium_transform_looking_at(const pandemonium_transform *p_self, const pandemonium_vector3 *p_target, const pandemonium_vector3 *p_up, const pandemonium_bool p_use_model_front) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *target = (const Vector3 *)p_target;
	const Vector3 *up = (const Vector3 *)p_up;
	*((Transform *)&dest) = self->looking_at(*target, *up, p_use_model_front);
	return dest;
}

void GDAPI pandemonium_transform_scale(pandemonium_transform *p_self, const pandemonium_vector3 *p_scale) {
	Transform *self = (Transform *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->scale(*scale);
}
pandemonium_transform GDAPI pandemonium_transform_scaled(const pandemonium_transform *p_self, const pandemonium_vector3 *p_scale) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Transform *)&dest) = self->scaled(*scale);
	return dest;
}

pandemonium_transform GDAPI pandemonium_transform_scaled_local(const pandemonium_transform *p_self, const pandemonium_vector3 *p_scale) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	*((Transform *)&dest) = self->scaled_local(*scale);
	return dest;
}
void GDAPI pandemonium_transform_scale_basis(pandemonium_transform *p_self, const pandemonium_vector3 *p_scale) {
	Transform *self = (Transform *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->scale_basis(*scale);
}

void GDAPI pandemonium_transform_translate_localr(pandemonium_transform *p_self, pandemonium_real p_tx, pandemonium_real p_ty, pandemonium_real p_tz) {
	Transform *self = (Transform *)p_self;
	self->translate_localr(p_tx, p_ty, p_tz);
}
void GDAPI pandemonium_transform_translate_localv(pandemonium_transform *p_self, const pandemonium_vector3 *p_translation) {
	Transform *self = (Transform *)p_self;
	const Vector3 *translation = (const Vector3 *)p_translation;
	self->translate_localv(*translation);
}

pandemonium_transform GDAPI pandemonium_transform_translated(const pandemonium_transform *p_self, const pandemonium_vector3 *p_ofs) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *ofs = (const Vector3 *)p_ofs;
	*((Transform *)&dest) = self->translated(*ofs);
	return dest;
}
pandemonium_transform GDAPI pandemonium_transform_translated_local(const pandemonium_transform *p_self, const pandemonium_vector3 *p_translation) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *translation = (const Vector3 *)p_translation;
	*((Transform *)&dest) = self->translated_local(*translation);
	return dest;
}

pandemonium_basis GDAPI pandemonium_transform_get_basis(const pandemonium_transform *p_self) {
	pandemonium_basis dest;
	const Transform *self = (const Transform *)p_self;
	*((Basis *)&dest) = self->basis;
	return dest;
}

void GDAPI pandemonium_transform_set_basis(pandemonium_transform *p_self, const pandemonium_basis *p_v) {
	Transform *self = (Transform *)p_self;
	const Basis *v = (const Basis *)p_v;
	self->basis = *v;
}

pandemonium_vector3 GDAPI pandemonium_transform_get_origin(const pandemonium_transform *p_self) {
	pandemonium_vector3 dest;
	const Transform *self = (const Transform *)p_self;
	*((Vector3 *)&dest) = self->origin;
	return dest;
}

void GDAPI pandemonium_transform_set_origin(pandemonium_transform *p_self, const pandemonium_vector3 *p_v) {
	Transform *self = (Transform *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	self->origin = *v;
}

void GDAPI pandemonium_transform_orthonormalize(pandemonium_transform *p_self) {
	Transform *self = (Transform *)p_self;
	self->orthonormalize();
}
pandemonium_transform GDAPI pandemonium_transform_orthonormalized(const pandemonium_transform *p_self) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	*((Transform *)&dest) = self->orthonormalized();
	return dest;
}
void GDAPI pandemonium_transform_orthogonalize(pandemonium_transform *p_self) {
	Transform *self = (Transform *)p_self;
	self->orthogonalize();
}
pandemonium_transform GDAPI pandemonium_transform_orthogonalized(const pandemonium_transform *p_self) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	*((Transform *)&dest) = self->orthogonalized();
	return dest;
}
pandemonium_bool GDAPI pandemonium_transform_is_equal_approx(const pandemonium_transform *p_self, const pandemonium_transform *p_transform) {
	const Transform *self = (const Transform *)p_self;
	const Transform *transform = (const Transform *)p_transform;
	return self->is_equal_approx(*transform);
}

pandemonium_bool GDAPI pandemonium_transform_operator_equal(const pandemonium_transform *p_self, const pandemonium_transform *p_b) {
	const Transform *self = (const Transform *)p_self;
	const Transform *b = (const Transform *)p_b;
	return *self == *b;
}

pandemonium_vector3 GDAPI pandemonium_transform_xform_vector3(const pandemonium_transform *p_self, const pandemonium_vector3 *p_v) {
	pandemonium_vector3 raw_dest;
	Vector3 *dest = (Vector3 *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	*dest = self->xform(*v);
	return raw_dest;
}
pandemonium_vector3i GDAPI pandemonium_transform_xform_vector3i(const pandemonium_transform *p_self, const pandemonium_vector3i *p_vector) {
	pandemonium_vector3i dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3i *vector = (const Vector3i *)p_vector;
	*((Vector3i *)&dest) = self->xform(*vector);
	return dest;
}

pandemonium_aabb GDAPI pandemonium_transform_xform_aabb(const pandemonium_transform *p_self, const pandemonium_aabb *p_v) {
	pandemonium_aabb raw_dest;
	AABB *dest = (AABB *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const AABB *v = (const AABB *)p_v;
	*dest = self->xform(*v);
	return raw_dest;
}

pandemonium_pool_vector3_array GDAPI pandemonium_transform_xform_pool_vector3(const pandemonium_transform *p_self, const pandemonium_pool_vector3_array *p_array) {
	pandemonium_pool_vector3_array dest;
	const Transform *self = (const Transform *)p_self;
	const PoolVector3Array *array = (const PoolVector3Array *)p_array;
	memnew_placement(&dest, PoolVector<Vector3>(self->xform(*array)));
	return dest;
}
pandemonium_pool_vector3i_array GDAPI pandemonium_transform_xform_pool_vector3i(const pandemonium_transform *p_self, const pandemonium_pool_vector3i_array *p_array) {
	pandemonium_pool_vector3i_array dest;
	const Transform *self = (const Transform *)p_self;
	const PoolVector3iArray *array = (const PoolVector3iArray *)p_array;
	memnew_placement(&dest, PoolVector<Vector3i>(self->xform(*array)));
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_transform_xform_inv_vector3(const pandemonium_transform *p_self, const pandemonium_vector3 *p_v) {
	pandemonium_vector3 raw_dest;
	Vector3 *dest = (Vector3 *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	*dest = self->xform_inv(*v);
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_transform_xform_inv_vector3i(const pandemonium_transform *p_self, const pandemonium_vector3i *p_vector) {
	pandemonium_vector3i dest;
	const Transform *self = (const Transform *)p_self;
	const Vector3i *vector = (const Vector3i *)p_vector;
	*((Vector3i *)&dest) = self->xform_inv(*vector);
	return dest;
}

pandemonium_aabb GDAPI pandemonium_transform_xform_inv_aabb(const pandemonium_transform *p_self, const pandemonium_aabb *p_v) {
	pandemonium_aabb raw_dest;
	AABB *dest = (AABB *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const AABB *v = (const AABB *)p_v;
	*dest = self->xform_inv(*v);
	return raw_dest;
}

pandemonium_pool_vector3_array GDAPI pandemonium_transform_xform_inv_pool_vector3(const pandemonium_transform *p_self, const pandemonium_pool_vector3_array *p_array) {
	pandemonium_pool_vector3_array dest;
	const Transform *self = (const Transform *)p_self;
	const PoolVector3Array *array = (const PoolVector3Array *)p_array;
	memnew_placement(&dest, PoolVector<Vector3>(self->xform_inv(*array)));
	return dest;
}
pandemonium_pool_vector3i_array GDAPI pandemonium_transform_xform_inv_pool_vector3i(const pandemonium_transform *p_self, const pandemonium_pool_vector3i_array *p_array) {
	pandemonium_pool_vector3i_array dest;
	const Transform *self = (const Transform *)p_self;
	const PoolVector3iArray *array = (const PoolVector3iArray *)p_array;
	memnew_placement(&dest, PoolVector<Vector3i>(self->xform_inv(*array)));
	return dest;
}

pandemonium_plane GDAPI pandemonium_transform_xform_plane(const pandemonium_transform *p_self, const pandemonium_plane *p_v) {
	pandemonium_plane raw_dest;
	Plane *dest = (Plane *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const Plane *v = (const Plane *)p_v;
	*dest = self->xform(*v);
	return raw_dest;
}

pandemonium_plane GDAPI pandemonium_transform_xform_inv_plane(const pandemonium_transform *p_self, const pandemonium_plane *p_v) {
	pandemonium_plane raw_dest;
	Plane *dest = (Plane *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const Plane *v = (const Plane *)p_v;
	*dest = self->xform_inv(*v);
	return raw_dest;
}

pandemonium_plane GDAPI pandemonium_transform_xform_fast(const pandemonium_transform *p_self, const pandemonium_plane *p_plane, const pandemonium_basis *p_basis_inverse_transpose) {
	pandemonium_plane dest;
	const Transform *self = (const Transform *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	const Basis *basis_inverse_transpose = (const Basis *)p_basis_inverse_transpose;
	*((Plane *)&dest) = self->xform_fast(*plane, *basis_inverse_transpose);
	return dest;
}
pandemonium_plane GDAPI pandemonium_transform_xform_inv_fast(const pandemonium_plane *p_plane, const pandemonium_transform *p_inverse, const pandemonium_basis *p_basis_transpose) {
	pandemonium_plane dest;
	const Plane *plane = (const Plane *)p_plane;
	const Transform *inverse = (const Transform *)p_inverse;
	const Basis *basis_transpose = (const Basis *)p_basis_transpose;
	*((Plane *)&dest) = Transform::xform_inv_fast(*plane, *inverse, *basis_transpose);
	return dest;
}

pandemonium_transform GDAPI pandemonium_transform_operator_multiply(const pandemonium_transform *p_self, const pandemonium_transform *p_b) {
	pandemonium_transform raw_dest;
	Transform *dest = (Transform *)&raw_dest;
	const Transform *self = (const Transform *)p_self;
	const Transform *b = (const Transform *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_transform GDAPI pandemonium_transform_spherical_interpolate_with(const pandemonium_transform *p_self, const pandemonium_transform *p_transform, pandemonium_real p_c) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Transform *transform = (const Transform *)p_transform;
	*((Transform *)&dest) = self->spherical_interpolate_with(*transform, p_c);
	return dest;
}
pandemonium_transform GDAPI pandemonium_transform_interpolate_with(const pandemonium_transform *p_self, const pandemonium_transform *p_transform, pandemonium_real p_c) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Transform *transform = (const Transform *)p_transform;
	*((Transform *)&dest) = self->interpolate_with(*transform, p_c);
	return dest;
}

pandemonium_transform GDAPI pandemonium_transform_inverse_xform(const pandemonium_transform *p_self, const pandemonium_transform *p_t) {
	pandemonium_transform dest;
	const Transform *self = (const Transform *)p_self;
	const Transform *t = (const Transform *)p_t;
	*((Transform *)&dest) = self->inverse_xform(*t);
	return dest;
}
void GDAPI pandemonium_transform_set(pandemonium_transform *p_self, pandemonium_real xx, pandemonium_real xy, pandemonium_real xz, pandemonium_real yx, pandemonium_real yy, pandemonium_real yz, pandemonium_real zx, pandemonium_real zy, pandemonium_real zz, pandemonium_real tx, pandemonium_real ty, pandemonium_real tz) {
	Transform *self = (Transform *)p_self;
	self->set(xx, xy, xz, yx, yy, yz, zx, zy, zz, tx, ty, tz);
}

pandemonium_string GDAPI pandemonium_transform_as_string(const pandemonium_transform *p_self) {
	pandemonium_string ret;
	const Transform *self = (const Transform *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_transform_new_with_axis_origin(pandemonium_transform *r_dest, const pandemonium_vector3 *p_x_axis, const pandemonium_vector3 *p_y_axis, const pandemonium_vector3 *p_z_axis, const pandemonium_vector3 *p_origin) {
	const Vector3 *x_axis = (const Vector3 *)p_x_axis;
	const Vector3 *y_axis = (const Vector3 *)p_y_axis;
	const Vector3 *z_axis = (const Vector3 *)p_z_axis;
	const Vector3 *origin = (const Vector3 *)p_origin;
	Transform *dest = (Transform *)r_dest;
	dest->basis.set_axis(0, *x_axis);
	dest->basis.set_axis(1, *y_axis);
	dest->basis.set_axis(2, *z_axis);
	dest->origin = *origin;
}

void GDAPI pandemonium_transform_new(pandemonium_transform *r_dest, const pandemonium_basis *p_basis, const pandemonium_vector3 *p_origin) {
	const Basis *basis = (const Basis *)p_basis;
	const Vector3 *origin = (const Vector3 *)p_origin;
	Transform *dest = (Transform *)r_dest;
	*dest = Transform(*basis, *origin);
}

void GDAPI pandemonium_transform_new_with_quaternion(pandemonium_transform *r_dest, const pandemonium_quaternion *p_quaternion) {
	const Quaternion *quaternion = (const Quaternion *)p_quaternion;
	Transform *dest = (Transform *)r_dest;
	*dest = Transform(*quaternion);
}

void GDAPI pandemonium_transform_new_identity(pandemonium_transform *r_dest) {
	Transform *dest = (Transform *)r_dest;
	*dest = Transform();
}

#ifdef __cplusplus
}
#endif
