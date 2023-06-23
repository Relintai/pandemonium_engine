/**************************************************************************/
/*  vector2.h                                                             */
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

#ifndef GDNATIVE_VECTOR2_H
#define GDNATIVE_VECTOR2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_VECTOR2_SIZE 8

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR2_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR2_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_VECTOR2_SIZE];
} pandemonium_vector2;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PANDEMONIUM_VECTOR2_AXIS_X,
	PANDEMONIUM_VECTOR2_AXIS_Y,
} pandemonium_vector2_axis;

void GDAPI pandemonium_vector2_set_x(pandemonium_vector2 *p_self, const pandemonium_real p_x);
void GDAPI pandemonium_vector2_set_y(pandemonium_vector2 *p_self, const pandemonium_real p_y);

pandemonium_real GDAPI pandemonium_vector2_get_x(const pandemonium_vector2 *p_self);
pandemonium_real GDAPI pandemonium_vector2_get_y(const pandemonium_vector2 *p_self);

pandemonium_real GDAPI *pandemonium_vector2_operator_index(pandemonium_vector2 *p_self, const pandemonium_int p_idx);
const pandemonium_real GDAPI *pandemonium_vector2_operator_index_const(const pandemonium_vector2 *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_vector2_set_axis(pandemonium_vector2 *p_self, const pandemonium_vector2_axis p_axis, const pandemonium_real p_value);
pandemonium_real GDAPI pandemonium_vector2_get_axis(const pandemonium_vector2 *p_self, const pandemonium_vector2_axis p_axis);

void GDAPI pandemonium_vector2_set_all(pandemonium_vector2 *p_self, const pandemonium_real p_value);

pandemonium_int GDAPI pandemonium_vector2_min_axis(const pandemonium_vector2 *p_self);
pandemonium_int GDAPI pandemonium_vector2_max_axis(const pandemonium_vector2 *p_self);

void GDAPI pandemonium_vector2_normalize(pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_normalized(const pandemonium_vector2 *p_self);
pandemonium_bool GDAPI pandemonium_vector2_is_normalized(const pandemonium_vector2 *p_self);

pandemonium_real GDAPI pandemonium_vector2_length(const pandemonium_vector2 *p_self);
pandemonium_real GDAPI pandemonium_vector2_length_squared(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_limit_length(const pandemonium_vector2 *p_self, const pandemonium_real p_length);

pandemonium_vector2 GDAPI pandemonium_vector2_min(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);
pandemonium_vector2 GDAPI pandemonium_vector2_max(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);

pandemonium_real GDAPI pandemonium_vector2_distance_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to);
pandemonium_real GDAPI pandemonium_vector2_distance_squared_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to);
pandemonium_real GDAPI pandemonium_vector2_angle_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to);
pandemonium_real GDAPI pandemonium_vector2_angle_to_point(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to);
pandemonium_vector2 GDAPI pandemonium_vector2_direction_to(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);

pandemonium_real GDAPI pandemonium_vector2_dot(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_other);
pandemonium_real GDAPI pandemonium_vector2_cross(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_other);
pandemonium_vector2 GDAPI pandemonium_vector2_posmod(const pandemonium_vector2 *p_self, const pandemonium_real p_mod);
pandemonium_vector2 GDAPI pandemonium_vector2_posmodv(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_modv);
pandemonium_vector2 GDAPI pandemonium_vector2_project(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to);

pandemonium_vector2 GDAPI pandemonium_vector2_plane_project(const pandemonium_vector2 *p_self, const pandemonium_real p_length, const pandemonium_vector2 *p_vec);

pandemonium_vector2 GDAPI pandemonium_vector2_linear_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b, const pandemonium_real p_t);
pandemonium_vector2 GDAPI pandemonium_vector2_slerp(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to, const pandemonium_real p_weight);
pandemonium_vector2 GDAPI pandemonium_vector2_cubic_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b, const pandemonium_vector2 *p_pre_a, const pandemonium_vector2 *p_post_b, const pandemonium_real p_t);
pandemonium_vector2 GDAPI pandemonium_vector2_bezier_interpolate(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_control_1, const pandemonium_vector2 *p_control_2, const pandemonium_vector2 *p_end, const pandemonium_real p_t);

pandemonium_vector2 GDAPI pandemonium_vector2_move_toward(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_to, const pandemonium_real p_delta);

pandemonium_vector2 GDAPI pandemonium_vector2_slide(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n);
pandemonium_vector2 GDAPI pandemonium_vector2_bounce(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n);
pandemonium_vector2 GDAPI pandemonium_vector2_reflect(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_n);

pandemonium_bool GDAPI pandemonium_vector2_is_equal_approx(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);

pandemonium_vector2 GDAPI pandemonium_vector2_operator_add(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);
pandemonium_vector2 GDAPI pandemonium_vector2_operator_subtract(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);

pandemonium_vector2 GDAPI pandemonium_vector2_operator_multiply_vector(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);
pandemonium_vector2 GDAPI pandemonium_vector2_operator_multiply_scalar(const pandemonium_vector2 *p_self, const pandemonium_real p_b);

pandemonium_vector2 GDAPI pandemonium_vector2_operator_divide_vector(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);
pandemonium_vector2 GDAPI pandemonium_vector2_operator_divide_scalar(const pandemonium_vector2 *p_self, const pandemonium_real p_b);

pandemonium_vector2 GDAPI pandemonium_vector2_operator_neg(const pandemonium_vector2 *p_self);

pandemonium_bool GDAPI pandemonium_vector2_operator_equal(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);
pandemonium_bool GDAPI pandemonium_vector2_operator_less(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_b);

pandemonium_real GDAPI pandemonium_vector2_angle(const pandemonium_vector2 *p_self);

void GDAPI pandemonium_vector2_set_rotation(pandemonium_vector2 *p_self, const pandemonium_real p_phi);

pandemonium_vector2 GDAPI pandemonium_vector2_abs(const pandemonium_vector2 *p_self);

pandemonium_vector2 GDAPI pandemonium_vector2_rotated(const pandemonium_vector2 *p_self, const pandemonium_real p_phi);
pandemonium_vector2 GDAPI pandemonium_vector2_tangent(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_orthogonal(const pandemonium_vector2 *p_self);

pandemonium_vector2 GDAPI pandemonium_vector2_sign(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_floor(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_ceil(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_round(const pandemonium_vector2 *p_self);
pandemonium_vector2 GDAPI pandemonium_vector2_snapped(const pandemonium_vector2 *p_self, const pandemonium_vector2 *p_by);
pandemonium_real GDAPI pandemonium_vector2_aspect(const pandemonium_vector2 *p_self);

pandemonium_string GDAPI pandemonium_vector2_as_string(const pandemonium_vector2 *p_self);

void GDAPI pandemonium_vector2_new(pandemonium_vector2 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR2_H
