/**************************************************************************/
/*  vector3.h                                                             */
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

#ifndef GDNATIVE_VECTOR3_H
#define GDNATIVE_VECTOR3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_VECTOR3_SIZE 12

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR3_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR3_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_VECTOR3_SIZE];
} pandemonium_vector3;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/basis.h>
#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PANDEMONIUM_VECTOR3_AXIS_X,
	PANDEMONIUM_VECTOR3_AXIS_Y,
	PANDEMONIUM_VECTOR3_AXIS_Z,
} pandemonium_vector3_axis;

pandemonium_real GDAPI pandemonium_vector3_get_x(const pandemonium_vector3 *p_self);
void GDAPI pandemonium_vector3_set_x(pandemonium_vector3 *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_vector3_get_y(const pandemonium_vector3 *p_self);
void GDAPI pandemonium_vector3_set_y(pandemonium_vector3 *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_vector3_get_z(const pandemonium_vector3 *p_self);
void GDAPI pandemonium_vector3_set_z(pandemonium_vector3 *p_self, const pandemonium_real val);

pandemonium_real GDAPI *pandemonium_vector3_operator_index(pandemonium_vector3 *p_self, const pandemonium_int p_idx);
const pandemonium_real GDAPI *pandemonium_vector3_operator_index_const(const pandemonium_vector3 *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_vector3_set_axis(pandemonium_vector3 *p_self, const pandemonium_vector3_axis p_axis, const pandemonium_real p_val);
pandemonium_real GDAPI pandemonium_vector3_get_axis(const pandemonium_vector3 *p_self, const pandemonium_vector3_axis p_axis);

void GDAPI pandemonium_vector3_set_all(pandemonium_vector3 *p_self, const pandemonium_real p_val);

pandemonium_int GDAPI pandemonium_vector3_min_axis(const pandemonium_vector3 *p_self);
pandemonium_int GDAPI pandemonium_vector3_max_axis(const pandemonium_vector3 *p_self);

pandemonium_real GDAPI pandemonium_vector3_length(const pandemonium_vector3 *p_self);
pandemonium_real GDAPI pandemonium_vector3_length_squared(const pandemonium_vector3 *p_self);

void GDAPI pandemonium_vector3_normalize(pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_normalized(const pandemonium_vector3 *p_self);
pandemonium_bool GDAPI pandemonium_vector3_is_normalized(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_inverse(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_limit_length(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_limit_length_len(const pandemonium_vector3 *p_self, const pandemonium_real p_val);

void GDAPI pandemonium_vector3_zero(pandemonium_vector3 *p_self);

void GDAPI pandemonium_vector3_snap(pandemonium_vector3 *p_self, const pandemonium_vector3 *p_val);
pandemonium_vector3 GDAPI pandemonium_vector3_snapped(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_by);

void GDAPI pandemonium_vector3_rotate(pandemonium_vector3 *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi);
pandemonium_vector3 GDAPI pandemonium_vector3_rotated(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_phi);

pandemonium_vector3 GDAPI pandemonium_vector3_linear_interpolate(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b, const pandemonium_real p_t);
pandemonium_vector3 GDAPI pandemonium_vector3_slerp(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_to, const pandemonium_real p_weight);
pandemonium_vector3 GDAPI pandemonium_vector3_cubic_interpolate(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b, const pandemonium_vector3 *p_pre_a, const pandemonium_vector3 *p_post_b, const pandemonium_real p_t);
pandemonium_vector3 GDAPI pandemonium_vector3_bezier_interpolate(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_control_1, const pandemonium_vector3 *p_control_2, const pandemonium_vector3 *p_end, const pandemonium_real p_t);

pandemonium_vector3 GDAPI pandemonium_vector3_move_toward(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_to, const pandemonium_real p_delta);

pandemonium_vector3 GDAPI pandemonium_vector3_cross(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_real GDAPI pandemonium_vector3_dot(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_basis GDAPI pandemonium_vector3_outer(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_basis GDAPI pandemonium_vector3_to_diagonal_matrix(const pandemonium_vector3 *p_self);

pandemonium_vector3 GDAPI pandemonium_vector3_abs(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_floor(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_sign(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_ceil(const pandemonium_vector3 *p_self);
pandemonium_vector3 GDAPI pandemonium_vector3_round(const pandemonium_vector3 *p_self);

pandemonium_vector3 GDAPI pandemonium_vector3_clamp(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_min, const pandemonium_vector3 *p_max);

pandemonium_real GDAPI pandemonium_vector3_distance_to(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_real GDAPI pandemonium_vector3_distance_squared_to(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);

pandemonium_vector3 GDAPI pandemonium_vector3_posmod(const pandemonium_vector3 *p_self, const pandemonium_real p_mod);
pandemonium_vector3 GDAPI pandemonium_vector3_posmodv(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_modv);
pandemonium_vector3 GDAPI pandemonium_vector3_project(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_to);

pandemonium_real GDAPI pandemonium_vector3_angle_to(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_to);
pandemonium_real GDAPI pandemonium_vector3_signed_angle_to(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_to, const pandemonium_vector3 *p_axis);
pandemonium_vector3 GDAPI pandemonium_vector3_direction_to(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);

pandemonium_vector3 GDAPI pandemonium_vector3_slide(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_n);
pandemonium_vector3 GDAPI pandemonium_vector3_bounce(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_n);
pandemonium_vector3 GDAPI pandemonium_vector3_reflect(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_n);

pandemonium_bool GDAPI pandemonium_vector3_is_equal_approx(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_v);
pandemonium_bool GDAPI pandemonium_vector3_is_equal_approx_tolerance(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_v, const pandemonium_real p_tolerance);
pandemonium_bool GDAPI pandemonium_vector3_is_equal_approxt(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_v, const pandemonium_real p_tolerance);

pandemonium_vector3 GDAPI pandemonium_vector3_operator_add(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_vector3 GDAPI pandemonium_vector3_operator_subtract(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);

pandemonium_vector3 GDAPI pandemonium_vector3_operator_multiply_vector(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_vector3 GDAPI pandemonium_vector3_operator_multiply_scalar(const pandemonium_vector3 *p_self, const pandemonium_real p_b);

pandemonium_vector3 GDAPI pandemonium_vector3_operator_divide_vector(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_vector3 GDAPI pandemonium_vector3_operator_divide_scalar(const pandemonium_vector3 *p_self, const pandemonium_real p_b);

pandemonium_vector3 GDAPI pandemonium_vector3_operator_neg(const pandemonium_vector3 *p_self);

pandemonium_bool GDAPI pandemonium_vector3_operator_equal(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);
pandemonium_bool GDAPI pandemonium_vector3_operator_less(const pandemonium_vector3 *p_self, const pandemonium_vector3 *p_b);

pandemonium_string GDAPI pandemonium_vector3_as_string(const pandemonium_vector3 *p_self);

void GDAPI pandemonium_vector3_new(pandemonium_vector3 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_z);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR3_H
