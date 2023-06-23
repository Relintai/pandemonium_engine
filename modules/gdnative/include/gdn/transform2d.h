/**************************************************************************/
/*  transform2d.h                                                         */
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

#ifndef GDNATIVE_TRANSFORM2D_H
#define GDNATIVE_TRANSFORM2D_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_TRANSFORM2D_SIZE 24

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_TRANSFORM2D_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_TRANSFORM2D_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_TRANSFORM2D_SIZE];
} pandemonium_transform2d;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/variant.h>
#include <gdn/vector2.h>
#include <gdn/vector2i.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_real GDAPI pandemonium_transform2d_tdotx(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);
pandemonium_real GDAPI pandemonium_transform2d_tdoty(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);

pandemonium_vector2 GDAPI pandemonium_transform2d_get_axis(const pandemonium_transform2d *p_self, pandemonium_int p_axis);
void GDAPI pandemonium_transform2d_set_axis(pandemonium_transform2d *p_self, pandemonium_int p_axis, const pandemonium_vector2 *p_vec);

pandemonium_vector2 GDAPI pandemonium_transform2d_get_column(const pandemonium_transform2d *p_self, pandemonium_int p_colum);
void GDAPI pandemonium_transform2d_set_column(pandemonium_transform2d *p_self, pandemonium_int p_colum, const pandemonium_vector2 *p_vec);

void GDAPI pandemonium_transform2d_invert(pandemonium_transform2d *p_self);
pandemonium_transform2d GDAPI pandemonium_transform2d_inverse(const pandemonium_transform2d *p_self);

void GDAPI pandemonium_transform2d_affine_invert(pandemonium_transform2d *p_self);
pandemonium_transform2d GDAPI pandemonium_transform2d_affine_inverse(const pandemonium_transform2d *p_self);

void GDAPI pandemonium_transform2d_set_rotation(pandemonium_transform2d *p_self, pandemonium_real p_rot);
pandemonium_real GDAPI pandemonium_transform2d_get_rotation(const pandemonium_transform2d *p_self);
pandemonium_real GDAPI pandemonium_transform2d_get_skew(const pandemonium_transform2d *p_self);
void GDAPI pandemonium_transform2d_set_skew(pandemonium_transform2d *p_self, const pandemonium_real p_angle);
void GDAPI pandemonium_transform2d_set_rotation_and_scale(pandemonium_transform2d *p_self, pandemonium_real p_rot, const pandemonium_vector2 *p_scale);
void GDAPI pandemonium_transform2d_set_rotation_scale_and_skew(pandemonium_transform2d *p_self, const pandemonium_real p_rot, const pandemonium_vector2 *p_scale, const pandemonium_real p_skew);
void GDAPI pandemonium_transform2d_rotate(pandemonium_transform2d *p_self, pandemonium_real p_phi);

void GDAPI pandemonium_transform2d_scale(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);
void GDAPI pandemonium_transform2d_scale_basis(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);

void GDAPI pandemonium_transform2d_translater(pandemonium_transform2d *p_self, pandemonium_real p_tx, pandemonium_real p_ty);
void GDAPI pandemonium_transform2d_translatev(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset);
void GDAPI pandemonium_transform2d_translate_localr(pandemonium_transform2d *p_self, pandemonium_real p_tx, pandemonium_real p_ty);
void GDAPI pandemonium_transform2d_translate_localv(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_translation);

pandemonium_real GDAPI pandemonium_transform2d_basis_determinant(const pandemonium_transform2d *p_self);

pandemonium_vector2 GDAPI pandemonium_transform2d_get_scale(const pandemonium_transform2d *p_self);
void GDAPI pandemonium_transform2d_set_scale(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);

pandemonium_vector2 GDAPI pandemonium_transform2d_get_origin(const pandemonium_transform2d *p_self);
void GDAPI pandemonium_transform2d_set_origin(pandemonium_transform2d *p_self, const pandemonium_vector2 *p_origin);

pandemonium_transform2d GDAPI pandemonium_transform2d_basis_scaled(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);
pandemonium_transform2d GDAPI pandemonium_transform2d_scaled(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);
pandemonium_transform2d GDAPI pandemonium_transform2d_scaled_local(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_scale);
pandemonium_transform2d GDAPI pandemonium_transform2d_translated(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset);
pandemonium_transform2d GDAPI pandemonium_transform2d_translated_local(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_offset);
pandemonium_transform2d GDAPI pandemonium_transform2d_rotated(const pandemonium_transform2d *p_self, const pandemonium_real p_phi);
pandemonium_transform2d GDAPI pandemonium_transform2d_rotated_local(const pandemonium_transform2d *p_self, const pandemonium_real p_angle);

pandemonium_transform2d GDAPI pandemonium_transform2d_untranslated(const pandemonium_transform2d *p_self);

void GDAPI pandemonium_transform2d_orthonormalize(pandemonium_transform2d *p_self);
pandemonium_transform2d GDAPI pandemonium_transform2d_orthonormalized(const pandemonium_transform2d *p_self);
pandemonium_bool GDAPI pandemonium_transform2d_is_equal_approx(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_transform);

pandemonium_transform2d GDAPI pandemonium_transform2d_looking_at(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_target);

pandemonium_bool GDAPI pandemonium_transform2d_operator_equal(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_b);
pandemonium_transform2d GDAPI pandemonium_transform2d_operator_multiply(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_b);

pandemonium_transform2d GDAPI pandemonium_transform2d_interpolate_with(const pandemonium_transform2d *p_self, const pandemonium_transform2d *p_m, const pandemonium_real p_c);

pandemonium_vector2 GDAPI pandemonium_transform2d_basis_xform_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);
pandemonium_vector2 GDAPI pandemonium_transform2d_basis_xform_inv_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);

pandemonium_vector2 GDAPI pandemonium_transform2d_xform_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);
pandemonium_vector2 GDAPI pandemonium_transform2d_xform_inv_vector2(const pandemonium_transform2d *p_self, const pandemonium_vector2 *p_v);

pandemonium_rect2 GDAPI pandemonium_transform2d_xform_rect2(const pandemonium_transform2d *p_self, const pandemonium_rect2 *p_v);
pandemonium_rect2 GDAPI pandemonium_transform2d_xform_inv_rect2(const pandemonium_transform2d *p_self, const pandemonium_rect2 *p_v);

pandemonium_vector2i GDAPI pandemonium_transform2d_basis_xform(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec);
pandemonium_vector2i GDAPI pandemonium_transform2d_basis_xform_inv(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec);

pandemonium_vector2i GDAPI pandemonium_transform2d_xform_vector2i(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec);
pandemonium_vector2i GDAPI pandemonium_transform2d_xform_inv_vector2i(const pandemonium_transform2d *p_self, const pandemonium_vector2i *p_vec);

pandemonium_pool_vector2_array GDAPI pandemonium_transform_xform_pool_vector2(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2_array *p_array);
pandemonium_pool_vector2_array GDAPI pandemonium_transform_xform_inv_pool_vector2(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2_array *p_array);
pandemonium_pool_vector2i_array GDAPI pandemonium_transform_xform_inv_pool_vector2i(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2i_array *p_array);
pandemonium_pool_vector2i_array GDAPI pandemonium_transform_xform_pool_vector2i(const pandemonium_transform2d *p_self, const pandemonium_pool_vector2i_array *p_array);

pandemonium_string GDAPI pandemonium_transform2d_as_string(const pandemonium_transform2d *p_self);

void GDAPI pandemonium_transform2d_new(pandemonium_transform2d *r_dest, const pandemonium_real p_rot, const pandemonium_vector2 *p_pos);
void GDAPI pandemonium_transform2d_new_axis_origin(pandemonium_transform2d *r_dest, const pandemonium_vector2 *p_x_axis, const pandemonium_vector2 *p_y_axis, const pandemonium_vector2 *p_origin);
void GDAPI pandemonium_transform2d_new_identity(pandemonium_transform2d *r_dest);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_TRANSFORM2D_H
