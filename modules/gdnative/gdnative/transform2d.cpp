/*************************************************************************/
/*  transform2d.cpp                                                      */
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

#include "gdn/transform2d.h"

#include "core/math/transform_2d.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_transform2d) == sizeof(Transform2D), "Transform2D size mismatch");

pandemonium_real GDAPI pandemonium_transform2d_tdotx(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	return self->tdotx(*v);
}
pandemonium_real GDAPI pandemonium_transform2d_tdoty(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	return self->tdoty(*v);
}

pandemonium_vector2 GDAPI pandemonium_transform2d_get_axis(const pandemonium_transform2d *p_self, pandemonium_int p_axis) {
	pandemonium_vector2 dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Vector2 *)&dest) = self->get_axis(p_axis);
	return dest;
}
void GDAPI pandemonium_transform2d_set_axis(pandemonium_transform2d *p_self, pandemonium_int p_axis, const pandemonium_vector2 *p_vec) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *vec = (const Vector2 *)p_vec;
	self->set_axis(p_axis, *vec);
}

pandemonium_vector2 GDAPI pandemonium_transform2d_get_column(const pandemonium_transform2d *p_self, pandemonium_int p_colum) {
	pandemonium_vector2 dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Vector2 *)&dest) = self->get_column(p_colum);
	return dest;
}
void GDAPI pandemonium_transform2d_set_column(pandemonium_transform2d *p_self, pandemonium_int p_colum, const pandemonium_vector2 *p_vec) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *vec = (const Vector2 *)p_vec;
	self->set_column(p_colum, *vec);
}

void GDAPI pandemonium_transform2d_invert(pandemonium_transform2d *p_self) {
	Transform2D *self = (Transform2D *)p_self;
	self->invert();
}

pandemonium_transform2d GDAPI pandemonium_transform2d_inverse(const pandemonium_transform2d *p_self) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Transform2D *)&dest) = self->inverse();
	return dest;
}

void GDAPI pandemonium_transform2d_affine_invert(pandemonium_transform2d *p_self) {
	Transform2D *self = (Transform2D *)p_self;
	self->affine_invert();
}

pandemonium_transform2d GDAPI pandemonium_transform2d_affine_inverse(const pandemonium_transform2d *p_self) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Transform2D *)&dest) = self->affine_inverse();
	return dest;
}

void GDAPI pandemonium_transform2d_set_rotation(pandemonium_transform2d *p_self, pandemonium_real p_rot) {
	Transform2D *self = (Transform2D *)p_self;
	self->set_rotation(p_rot);
}

pandemonium_real GDAPI pandemonium_transform2d_get_rotation(const pandemonium_transform2d *p_self) {
	const Transform2D *self = (const Transform2D *)p_self;
	return self->get_rotation();
}

pandemonium_real GDAPI pandemonium_transform2d_get_skew(const pandemonium_transform2d *p_self) {
	const Transform2D *self = (const Transform2D *)p_self;
	return self->get_skew();
}
void GDAPI pandemonium_transform2d_set_skew(pandemonium_transform2d *p_self, const pandemonium_real p_angle) {
	Transform2D *self = (Transform2D *)p_self;
	self->set_skew(p_angle);
}
void GDAPI pandemonium_transform2d_set_rotation_and_scale(pandemonium_transform2d *p_self, pandemonium_real p_rot, const pandemonium_vector2 *p_scale) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	self->set_rotation_and_scale(p_rot, *scale);
}
void GDAPI pandemonium_transform2d_set_rotation_scale_and_skew(pandemonium_transform2d *p_self, const pandemonium_real p_rot, const pandemonium_vector2 *p_scale, const pandemonium_real p_skew) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	self->set_rotation_scale_and_skew(p_rot, *scale, p_skew);
}
void GDAPI pandemonium_transform2d_rotate(pandemonium_transform2d *p_self, pandemonium_real p_phi) {
	Transform2D *self = (Transform2D *)p_self;
	self->rotate(p_phi);
}

void GDAPI pandemonium_transform2d_scale(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	self->scale(*scale);
}
void GDAPI pandemonium_transform2d_scale_basis(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	self->scale_basis(*scale);
}

void GDAPI pandemonium_transform2d_translater(pandemonium_transform2d *p_self, pandemonium_real p_tx, pandemonium_real p_ty) {
	Transform2D *self = (Transform2D *)p_self;
	self->translater(p_tx, p_ty);
}
void GDAPI pandemonium_transform2d_translatev(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	self->translatev(*offset);
}
void GDAPI pandemonium_transform2d_translate_localr(pandemonium_transform2d *p_self, pandemonium_real p_tx, pandemonium_real p_ty) {
	Transform2D *self = (Transform2D *)p_self;
	self->translate_localr(p_tx, p_ty);
}
void GDAPI pandemonium_transform2d_translate_localv(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_translation) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *translation = (const Vector2 *)p_translation;
	self->translate_localv(*translation);
}

pandemonium_real GDAPI pandemonium_transform2d_basis_determinant(const pandemonium_transform2d *p_self) {
	const Transform2D *self = (const Transform2D *)p_self;
	return self->basis_determinant();
}

pandemonium_vector2 GDAPI pandemonium_transform2d_get_scale(const pandemonium_transform2d *p_self) {
	pandemonium_vector2 dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Vector2 *)&dest) = self->get_scale();
	return dest;
}

void GDAPI pandemonium_transform2d_set_scale(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	self->set_scale(*scale);
}

pandemonium_vector2 GDAPI pandemonium_transform2d_get_origin(const pandemonium_transform2d *p_self) {
	pandemonium_vector2 dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Vector2 *)&dest) = self->get_origin();
	return dest;
}

void GDAPI pandemonium_transform2d_set_origin(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_origin) {
	Transform2D *self = (Transform2D *)p_self;
	const Vector2 *origin = (const Vector2 *)p_origin;
	self->set_origin(*origin);
}

pandemonium_transform2d GDAPI pandemonium_transform2d_basis_scaled(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	*((Transform2D *)&dest) = self->basis_scaled(*scale);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_scaled(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	*((Transform2D *)&dest) = self->scaled(*scale);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_scaled_local(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *scale = (const Vector2 *)p_scale;
	*((Transform2D *)&dest) = self->scaled_local(*scale);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_translated(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	*((Transform2D *)&dest) = self->translated(*offset);
	return dest;
}
pandemonium_transform2d GDAPI pandemonium_transform2d_translated_local(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	*((Transform2D *)&dest) = self->translated_local(*offset);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_rotated(const pandemonium_transform2d *p_self, const pandemonium_real p_phi) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;

	*((Transform2D *)&dest) = self->rotated(p_phi);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_rotated_local(const pandemonium_transform2d *p_self, const pandemonium_real p_angle) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Transform2D *)&dest) = self->rotated_local(p_angle);
	return dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_untranslated(const pandemonium_transform2d *p_self) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Transform2D *)&dest) = self->untranslated();
	return dest;
}

void GDAPI pandemonium_transform2d_orthonormalize(pandemonium_transform2d *p_self) {
	Transform2D *self = (Transform2D *)p_self;
	self->orthonormalize();
}

pandemonium_transform2d GDAPI pandemonium_transform2d_orthonormalized(const pandemonium_transform2d *p_self) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	*((Transform2D *)&dest) = self->orthonormalized();
	return dest;
}

pandemonium_bool GDAPI pandemonium_transform2d_is_equal_approx(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_transform) {
	const Transform2D *self = (const Transform2D *)p_self;
	const Transform2D *transform = (const Transform2D *)p_transform;
	return self->is_equal_approx(*transform);
}

pandemonium_transform2d GDAPI pandemonium_transform2d_looking_at(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_target) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *target = (const Vector2 *)p_target;
	*((Transform2D *)&dest) = self->looking_at(*target);
	return dest;
}

pandemonium_bool GDAPI pandemonium_transform2d_operator_equal(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_b) {
	const Transform2D *self = (const Transform2D *)p_self;
	const Transform2D *b = (const Transform2D *)p_b;
	return *self == *b;
}
pandemonium_transform2d GDAPI pandemonium_transform2d_operator_multiply(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_b) {
	pandemonium_transform2d raw_dest;
	Transform2D *dest = (Transform2D *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Transform2D *b = (const Transform2D *)p_b;
	*dest = *self * *b;
	return raw_dest;
}

pandemonium_transform2d GDAPI pandemonium_transform2d_interpolate_with(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_m, const pandemonium_real p_c) {
	pandemonium_transform2d dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Transform2D *m = (const Transform2D *)p_m;
	*((Transform2D *)&dest) = self->interpolate_with(*m, p_c);
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_transform2d_basis_xform_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	*dest = self->basis_xform(*v);
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_transform2d_basis_xform_inv_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	*dest = self->basis_xform_inv(*v);
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_transform2d_xform_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	*dest = self->xform(*v);
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_transform2d_xform_inv_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v) {
	pandemonium_vector2 raw_dest;
	Vector2 *dest = (Vector2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2 *v = (const Vector2 *)p_v;
	*dest = self->xform_inv(*v);
	return raw_dest;
}

pandemonium_rect2 GDAPI pandemonium_transform2d_xform_rect2(const pandemonium_transform2d *p_self, const pandemonium_rect2 *p_v) {
	pandemonium_rect2 raw_dest;
	Rect2 *dest = (Rect2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Rect2 *v = (const Rect2 *)p_v;
	*dest = self->xform(*v);
	return raw_dest;
}

pandemonium_rect2 GDAPI pandemonium_transform2d_xform_inv_rect2(const pandemonium_transform2d *p_self, const pandemonium_rect2 *p_v) {
	pandemonium_rect2 raw_dest;
	Rect2 *dest = (Rect2 *)&raw_dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Rect2 *v = (const Rect2 *)p_v;
	*dest = self->xform_inv(*v);
	return raw_dest;
}

pandemonium_vector2i GDAPI pandemonium_transform2d_basis_xform(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec) {
	pandemonium_vector2i dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2i *vec = (const Vector2i *)p_vec;
	*((Vector2i *)&dest) = self->basis_xform(*vec);
	return dest;
}
pandemonium_vector2i GDAPI pandemonium_transform2d_basis_xform_inv(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec) {
	pandemonium_vector2i dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2i *vec = (const Vector2i *)p_vec;
	*((Vector2i *)&dest) = self->basis_xform_inv(*vec);
	return dest;
}
pandemonium_vector2i GDAPI pandemonium_transform2d_xform_vector2i(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec) {
	pandemonium_vector2i dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2i *vec = (const Vector2i *)p_vec;
	*((Vector2i *)&dest) = self->xform(*vec);
	return dest;
}
pandemonium_vector2i GDAPI pandemonium_transform2d_xform_inv_vector2i(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec) {
	pandemonium_vector2i dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const Vector2i *vec = (const Vector2i *)p_vec;
	*((Vector2i *)&dest) = self->xform_inv(*vec);
	return dest;
}

pandemonium_pool_vector2_array GDAPI pandemonium_transform_xform_pool_vector2(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2_array *p_array) {
	pandemonium_pool_vector2_array dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const PoolVector2Array *array = (const PoolVector2Array *)p_array;
	memnew_placement(&dest, PoolVector<Vector2>(self->xform(*array)));
	return dest;
}
pandemonium_pool_vector2i_array GDAPI pandemonium_transform_xform_pool_vector2i(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2i_array *p_array) {
	pandemonium_pool_vector2i_array dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const PoolVector2iArray *array = (const PoolVector2iArray *)p_array;
	memnew_placement(&dest, PoolVector<Vector2i>(self->xform(*array)));
	return dest;
}
pandemonium_pool_vector2_array GDAPI pandemonium_transform_xform_inv_pool_vector2(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2_array *p_array) {
	pandemonium_pool_vector2_array dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const PoolVector2Array *array = (const PoolVector2Array *)p_array;
	memnew_placement(&dest, PoolVector<Vector2>(self->xform_inv(*array)));
	return dest;
}
pandemonium_pool_vector2i_array GDAPI pandemonium_transform_xform_inv_pool_vector2i(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2i_array *p_array) {
	pandemonium_pool_vector2i_array dest;
	const Transform2D *self = (const Transform2D *)p_self;
	const PoolVector2iArray *array = (const PoolVector2iArray *)p_array;
	memnew_placement(&dest, PoolVector<Vector2i>(self->xform_inv(*array)));
	return dest;
}

pandemonium_string GDAPI pandemonium_transform2d_as_string(const pandemonium_transform2d *p_self) {
	pandemonium_string ret;
	const Transform2D *self = (const Transform2D *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_transform2d_new(pandemonium_transform2d *r_dest, const pandemonium_real p_rot, const pandemonium_vector2 *p_pos) {
	const Vector2 *pos = (const Vector2 *)p_pos;
	Transform2D *dest = (Transform2D *)r_dest;
	*dest = Transform2D(p_rot, *pos);
}

void GDAPI pandemonium_transform2d_new_axis_origin(pandemonium_transform2d *r_dest, const pandemonium_vector2 *p_x_axis, const pandemonium_vector2 *p_y_axis, const pandemonium_vector2 *p_origin) {
	const Vector2 *x_axis = (const Vector2 *)p_x_axis;
	const Vector2 *y_axis = (const Vector2 *)p_y_axis;
	const Vector2 *origin = (const Vector2 *)p_origin;
	Transform2D *dest = (Transform2D *)r_dest;
	*dest = Transform2D(x_axis->x, x_axis->y, y_axis->x, y_axis->y, origin->x, origin->y);
}

void GDAPI pandemonium_transform2d_new_identity(pandemonium_transform2d *r_dest) {
	Transform2D *dest = (Transform2D *)r_dest;
	*dest = Transform2D();
}

#ifdef __cplusplus
}
#endif
