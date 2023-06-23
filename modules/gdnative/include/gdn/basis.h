/**************************************************************************/
/*  basis.h                                                               */
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

#ifndef GDNATIVE_BASIS_H
#define GDNATIVE_BASIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_BASIS_SIZE 36

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_BASIS_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_BASIS_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_BASIS_SIZE];
} pandemonium_basis;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/quaternion.h>
#include <gdn/vector3.h>
#include <gdn/vector3i.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI pandemonium_basis_invert(pandemonium_basis *p_self);
void GDAPI pandemonium_basis_transpose(pandemonium_basis *p_self);

pandemonium_basis GDAPI pandemonium_basis_inverse(const pandemonium_basis *p_self);
pandemonium_basis GDAPI pandemonium_basis_transposed(const pandemonium_basis *p_self);

pandemonium_real GDAPI pandemonium_basis_determinant(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_from_z(pandemonium_basis *p_self, const pandemonium_vector3 *p_z);

void GDAPI pandemonium_basis_rotate(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi);
pandemonium_basis GDAPI pandemonium_basis_rotated(const pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi);

void GDAPI pandemonium_basis_rotate_local(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi);
pandemonium_basis GDAPI pandemonium_basis_rotated_local(const pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi);

void GDAPI pandemonium_basis_rotate_euler(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);
pandemonium_basis GDAPI pandemonium_basis_rotated_euler(const pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

void GDAPI pandemonium_basis_rotate_quat(pandemonium_basis *p_self, const pandemonium_quaternion *p_quat);
pandemonium_basis GDAPI pandemonium_basis_rotated_quat(const pandemonium_basis *p_self, const pandemonium_quaternion *p_quat);

void GDAPI pandemonium_basis_rotatev(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);
pandemonium_basis GDAPI pandemonium_basis_rotatedv(const pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);
void GDAPI pandemonium_basis_rotateq(pandemonium_basis *p_self, const pandemonium_quaternion *p_quat);
pandemonium_basis GDAPI pandemonium_basis_rotatedq(const pandemonium_basis *p_self, const pandemonium_quaternion *p_quat);

pandemonium_vector3 GDAPI pandemonium_basis_get_rotation_euler(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_get_rotation_axis_angle(const pandemonium_basis *p_self, pandemonium_vector3 *p_axis, pandemonium_real p_angle);
void GDAPI pandemonium_basis_get_rotation_axis_angle_local(const pandemonium_basis *p_self, pandemonium_vector3 *p_axis, pandemonium_real p_angle);
pandemonium_quaternion GDAPI pandemonium_basis_get_rotation_quaternion(const pandemonium_basis *p_self);
pandemonium_vector3 GDAPI pandemonium_basis_get_rotation(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_rotate_to_align(pandemonium_basis *p_self, const pandemonium_vector3 *p_start_direction, const pandemonium_vector3 *p_end_direction);

pandemonium_vector3 GDAPI pandemonium_basis_rotref_posscale_decomposition(const pandemonium_basis *p_self, pandemonium_basis *rotref);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_xyz(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_xyz(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_xzy(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_xzy(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_yzx(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_yzx(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_yxz(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_yxz(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_zxy(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_zxy(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler_zyx(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler_zyx(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_vector3 GDAPI pandemonium_basis_get_euler(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_euler(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler);

pandemonium_quaternion GDAPI pandemonium_basis_get_quaternion(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_quaternion(pandemonium_basis *p_self, const pandemonium_quaternion *p_quaternion);

void GDAPI pandemonium_basis_get_axis_angle(const pandemonium_basis *p_self, pandemonium_vector3 *r_axis, pandemonium_real *r_angle);
void GDAPI pandemonium_basis_set_axis_angle(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi);

void GDAPI pandemonium_basis_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);
pandemonium_basis GDAPI pandemonium_basis_scaled(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);

void GDAPI pandemonium_basis_scale_local(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);
pandemonium_basis GDAPI pandemonium_basis_scaled_local(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);

void GDAPI pandemonium_basis_scale_orthogonal(pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);
pandemonium_basis GDAPI pandemonium_basis_scaled_orthogonal(const pandemonium_basis *p_self, const pandemonium_vector3 *p_scale);

void GDAPI pandemonium_basis_make_scale_uniform(pandemonium_basis *p_self);
pandemonium_real GDAPI pandemonium_basis_get_uniform_scale(const pandemonium_basis *p_self);

pandemonium_vector3 GDAPI pandemonium_basis_get_scale(const pandemonium_basis *p_self);
pandemonium_vector3 GDAPI pandemonium_basis_get_scale_abs(const pandemonium_basis *p_self);
pandemonium_vector3 GDAPI pandemonium_basis_get_scale_local(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_set_axis_angle_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_axis, pandemonium_real p_phi, const pandemonium_vector3 *p_scale);
void GDAPI pandemonium_basis_set_euler_scale(pandemonium_basis *p_self, const pandemonium_vector3 *p_euler, const pandemonium_vector3 *p_scale);
void GDAPI pandemonium_basis_set_quaternion_scale(pandemonium_basis *p_self, const pandemonium_quaternion *p_quaternion, const pandemonium_vector3 *p_scale);

pandemonium_real GDAPI pandemonium_basis_tdotx(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with);
pandemonium_real GDAPI pandemonium_basis_tdoty(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with);
pandemonium_real GDAPI pandemonium_basis_tdotz(const pandemonium_basis *p_self, const pandemonium_vector3 *p_with);

pandemonium_bool GDAPI pandemonium_basis_is_equal_approx(const pandemonium_basis *p_self, const pandemonium_basis *p_basis);
pandemonium_bool GDAPI pandemonium_basis_is_equal_approx_ratio(const pandemonium_basis *p_self, const pandemonium_basis *p_a, const pandemonium_basis *p_b);
pandemonium_bool GDAPI pandemonium_basis_is_equal_approx_ratio_eps(const pandemonium_basis *p_self, const pandemonium_basis *p_a, const pandemonium_basis *p_b, pandemonium_real p_epsilon);

pandemonium_bool GDAPI pandemonium_basis_operator_equal(const pandemonium_basis *p_self, const pandemonium_basis *p_b);

pandemonium_vector3 GDAPI pandemonium_basis_xform(const pandemonium_basis *p_self, const pandemonium_vector3 *p_v);
pandemonium_vector3 GDAPI pandemonium_basis_xform_inv(const pandemonium_basis *p_self, const pandemonium_vector3 *p_v);

pandemonium_vector3i GDAPI pandemonium_basis_xform_v3i(const pandemonium_basis *p_self, const pandemonium_vector3i *p_vector);
pandemonium_vector3i GDAPI pandemonium_basis_xform_inv_v3i(const pandemonium_basis *p_self, const pandemonium_vector3i *p_vector);

pandemonium_basis GDAPI pandemonium_basis_operator_multiply(const pandemonium_basis *p_self, const pandemonium_basis *p_b);
pandemonium_basis GDAPI pandemonium_basis_operator_add(const pandemonium_basis *p_self, const pandemonium_basis *p_b);
pandemonium_basis GDAPI pandemonium_basis_operator_subtract(const pandemonium_basis *p_self, const pandemonium_basis *p_b);
pandemonium_basis GDAPI pandemonium_basis_operator_multiply_scalar(const pandemonium_basis *p_self, const pandemonium_real p_b);

pandemonium_int GDAPI pandemonium_basis_get_orthogonal_index(const pandemonium_basis *p_self);
void GDAPI pandemonium_basis_set_orthogonal_index(pandemonium_basis *p_self, pandemonium_int p_index);

void GDAPI pandemonium_basis_set_diagonal(pandemonium_basis *p_self, const pandemonium_vector3 *p_diag);

pandemonium_bool GDAPI pandemonium_basis_is_orthogonal(const pandemonium_basis *p_self);
pandemonium_bool GDAPI pandemonium_basis_is_diagonal(const pandemonium_basis *p_self);
pandemonium_bool GDAPI pandemonium_basis_is_rotation(const pandemonium_basis *p_self);

pandemonium_basis GDAPI pandemonium_basis_slerp(const pandemonium_basis *p_self, const pandemonium_basis *p_b, const pandemonium_real p_t);
pandemonium_basis GDAPI pandemonium_basis_lerp(const pandemonium_basis *p_self, const pandemonium_basis *p_to, const pandemonium_real p_weight);
void GDAPI pandemonium_basis_rotate_sh(pandemonium_basis *p_self, pandemonium_real *p_values);

pandemonium_string GDAPI pandemonium_basis_as_string(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_set_real(pandemonium_basis *p_self, pandemonium_real xx, pandemonium_real xy, pandemonium_real xz, pandemonium_real yx, pandemonium_real yy, pandemonium_real yz, pandemonium_real zx, pandemonium_real zy, pandemonium_real zz);
void GDAPI pandemonium_basis_set_vector3(pandemonium_basis *p_self, const pandemonium_vector3 *p_x, const pandemonium_vector3 *p_y, const pandemonium_vector3 *p_z);

pandemonium_vector3 GDAPI pandemonium_basis_get_column(const pandemonium_basis *p_self, pandemonium_int i);
void GDAPI pandemonium_basis_set_column(pandemonium_basis *p_self, pandemonium_int p_index, const pandemonium_vector3 *p_value);
void GDAPI pandemonium_basis_set_columns(pandemonium_basis *p_self, const pandemonium_vector3 *p_x, const pandemonium_vector3 *p_y, const pandemonium_vector3 *p_z);

pandemonium_vector3 GDAPI pandemonium_basis_get_row(const pandemonium_basis *p_self, const pandemonium_int p_row);
void GDAPI pandemonium_basis_set_row(pandemonium_basis *p_self, const pandemonium_int p_row, const pandemonium_vector3 *p_value);

pandemonium_vector3 GDAPI pandemonium_basis_get_axis(const pandemonium_basis *p_self, const pandemonium_int p_axis);
void GDAPI pandemonium_basis_set_axis(pandemonium_basis *p_self, const pandemonium_int p_axis, const pandemonium_vector3 *p_value);

pandemonium_vector3 GDAPI pandemonium_basis_get_main_diagonal(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_set_zero(pandemonium_basis *p_self);

pandemonium_basis GDAPI pandemonium_basis_transpose_xform(const pandemonium_basis *p_self, const pandemonium_basis *m);

void GDAPI pandemonium_basis_orthonormalize(pandemonium_basis *p_self);
pandemonium_basis GDAPI pandemonium_basis_orthonormalized(const pandemonium_basis *p_self);

void GDAPI pandemonium_basis_orthogonalize(pandemonium_basis *p_self);
pandemonium_basis GDAPI pandemonium_basis_orthogonalized(const pandemonium_basis *p_self);

pandemonium_bool GDAPI pandemonium_basis_is_symmetric(const pandemonium_basis *p_self);
pandemonium_basis GDAPI pandemonium_basis_diagonalize(pandemonium_basis *p_self);

pandemonium_basis GDAPI pandemonium_basis_get_normal_xform_basis(const pandemonium_basis *p_self);
pandemonium_vector3 GDAPI pandemonium_basis_xform_normal_fast(const pandemonium_basis *p_self, const pandemonium_vector3 *p_vector);
pandemonium_vector3 GDAPI pandemonium_basis_xform_normal(const pandemonium_basis *p_self, const pandemonium_vector3 *p_vector);

pandemonium_basis GDAPI pandemonium_basis_looking_at(const pandemonium_vector3 *p_target);
pandemonium_basis GDAPI pandemonium_basis_looking_at_up(const pandemonium_vector3 *p_target, const pandemonium_vector3 *p_up);
pandemonium_basis GDAPI pandemonium_basis_from_scale(const pandemonium_vector3 *p_scale);

// p_elements is a pointer to an array of 3 (!!) vector3
void GDAPI pandemonium_basis_get_elements(const pandemonium_basis *p_self, pandemonium_vector3 *p_elements);

void GDAPI pandemonium_basis_new(pandemonium_basis *r_dest);
void GDAPI pandemonium_basis_new_with_rows(pandemonium_basis *r_dest, const pandemonium_vector3 *p_x_axis, const pandemonium_vector3 *p_y_axis, const pandemonium_vector3 *p_z_axis);
void GDAPI pandemonium_basis_new_with_axis_and_angle(pandemonium_basis *r_dest, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi);
void GDAPI pandemonium_basis_new_with_euler(pandemonium_basis *r_dest, const pandemonium_vector3 *p_euler);
void GDAPI pandemonium_basis_new_with_euler_quaternion(pandemonium_basis *r_dest, const pandemonium_quaternion *p_euler);
void GDAPI pandemonium_basis_new_quaternion_scale(pandemonium_basis *r_dest, const pandemonium_quaternion *p_quat, const pandemonium_vector3 *p_scale);
void GDAPI pandemonium_basis_new_euler_scale(pandemonium_basis *r_dest, const pandemonium_vector3 *p_euler, const pandemonium_vector3 *p_scale);
void GDAPI pandemonium_basis_new_axis_phi_scale(pandemonium_basis *r_dest, const pandemonium_vector3 *p_axis, pandemonium_real p_phi, const pandemonium_vector3 *p_scale);
void GDAPI pandemonium_basis_new_floats(pandemonium_basis *r_dest, pandemonium_real xx, pandemonium_real xy, pandemonium_real xz, pandemonium_real yx, pandemonium_real yy, pandemonium_real yz, pandemonium_real zx, pandemonium_real zy, pandemonium_real zz);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_BASIS_H
