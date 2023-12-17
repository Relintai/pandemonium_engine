#ifndef GDNATIVE_VECTOR4_H
#define GDNATIVE_VECTOR4_H

/*************************************************************************/
/*  vector4.h                                                            */
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_VECTOR4_SIZE 16

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR4_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR4_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_VECTOR4_SIZE];
} pandemonium_vector4;
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
	PANDEMONIUM_VECTOR4_AXIS_X,
	PANDEMONIUM_VECTOR4_AXIS_Y,
	PANDEMONIUM_VECTOR4_AXIS_Z,
	PANDEMONIUM_VECTOR4_AXIS_W,
} pandemonium_vector4_axis;

pandemonium_real GDAPI pandemonium_vector4_get_x(const pandemonium_vector4 *p_self);
void GDAPI pandemonium_vector4_set_x(pandemonium_vector4 *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_vector4_get_y(const pandemonium_vector4 *p_self);
void GDAPI pandemonium_vector4_set_y(pandemonium_vector4 *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_vector4_get_z(const pandemonium_vector4 *p_self);
void GDAPI pandemonium_vector4_set_z(pandemonium_vector4 *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_vector4_get_w(const pandemonium_vector4 *p_self);
void GDAPI pandemonium_vector4_set_w(pandemonium_vector4 *p_self, const pandemonium_real val);

pandemonium_real GDAPI *pandemonium_vector4_operator_index(pandemonium_vector4 *p_self, const pandemonium_int p_idx);
const pandemonium_real GDAPI *pandemonium_vector4_operator_index_const(const pandemonium_vector4 *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_vector4_set_axis(pandemonium_vector4 *p_self, const pandemonium_vector4_axis p_axis, const pandemonium_real p_val);
pandemonium_real GDAPI pandemonium_vector4_get_axis(const pandemonium_vector4 *p_self, const pandemonium_vector4_axis p_axis);

void GDAPI pandemonium_vector4_set_all(pandemonium_vector4 *p_self, const pandemonium_real p_val);

pandemonium_int GDAPI pandemonium_vector4_min_axis(const pandemonium_vector4 *p_self);
pandemonium_int GDAPI pandemonium_vector4_max_axis(const pandemonium_vector4 *p_self);

pandemonium_real GDAPI pandemonium_vector4_length(const pandemonium_vector4 *p_self);
pandemonium_real GDAPI pandemonium_vector4_length_squared(const pandemonium_vector4 *p_self);

void GDAPI pandemonium_vector4_normalize(pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_normalized(const pandemonium_vector4 *p_self);
pandemonium_bool GDAPI pandemonium_vector4_is_normalized(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_inverse(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_limit_length(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_limit_length_len(const pandemonium_vector4 *p_self, const pandemonium_real p_val);

void GDAPI pandemonium_vector4_zero(pandemonium_vector4 *p_self);

void GDAPI pandemonium_vector4_snap(pandemonium_vector4 *p_self, const pandemonium_vector4 *p_val);
pandemonium_vector4 GDAPI pandemonium_vector4_snapped(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_by);

pandemonium_vector4 GDAPI pandemonium_vector4_linear_interpolate(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b, const pandemonium_real p_t);
pandemonium_vector4 GDAPI pandemonium_vector4_cubic_interpolate(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b, const pandemonium_vector4 *p_pre_a, const pandemonium_vector4 *p_post_b, const pandemonium_real p_t);

pandemonium_real GDAPI pandemonium_vector4_dot(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);

pandemonium_vector4 GDAPI pandemonium_vector4_abs(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_floor(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_sign(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_ceil(const pandemonium_vector4 *p_self);
pandemonium_vector4 GDAPI pandemonium_vector4_round(const pandemonium_vector4 *p_self);

pandemonium_vector4 GDAPI pandemonium_vector4_clamp(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_min, const pandemonium_vector4 *p_max);

pandemonium_real GDAPI pandemonium_vector4_distance_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);
pandemonium_real GDAPI pandemonium_vector4_distance_squared_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);

pandemonium_vector4 GDAPI pandemonium_vector4_posmod(const pandemonium_vector4 *p_self, const pandemonium_real p_mod);
pandemonium_vector4 GDAPI pandemonium_vector4_posmodv(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_modv);

pandemonium_vector4 GDAPI pandemonium_vector4_direction_to(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);

pandemonium_bool GDAPI pandemonium_vector4_is_equal_approx(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_v);

pandemonium_vector4 GDAPI pandemonium_vector4_operator_add(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);
pandemonium_vector4 GDAPI pandemonium_vector4_operator_subtract(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);

pandemonium_vector4 GDAPI pandemonium_vector4_operator_multiply_vector(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);
pandemonium_vector4 GDAPI pandemonium_vector4_operator_multiply_scalar(const pandemonium_vector4 *p_self, const pandemonium_real p_b);

pandemonium_vector4 GDAPI pandemonium_vector4_operator_divide_vector(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);
pandemonium_vector4 GDAPI pandemonium_vector4_operator_divide_scalar(const pandemonium_vector4 *p_self, const pandemonium_real p_b);

pandemonium_vector4 GDAPI pandemonium_vector4_operator_neg(const pandemonium_vector4 *p_self);

pandemonium_bool GDAPI pandemonium_vector4_operator_equal(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);
pandemonium_bool GDAPI pandemonium_vector4_operator_less(const pandemonium_vector4 *p_self, const pandemonium_vector4 *p_b);

pandemonium_string GDAPI pandemonium_vector4_as_string(const pandemonium_vector4 *p_self);

void GDAPI pandemonium_vector4_new(pandemonium_vector4 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_z, const pandemonium_real p_w);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR4_H
