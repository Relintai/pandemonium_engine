#ifndef GDNATIVE_QUATERNION_H
#define GDNATIVE_QUATERNION_H

/*************************************************************************/
/*  quaternion.h                                                         */
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

#define PANDEMONIUM_QUATERNION_SIZE 16

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_QUATERNION_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_QUATERNION_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_QUATERNION_SIZE];
} pandemonium_quaternion;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_real GDAPI pandemonium_quaternion_get_x(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_set_x(pandemonium_quaternion *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_quaternion_get_y(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_set_y(pandemonium_quaternion *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_quaternion_get_z(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_set_z(pandemonium_quaternion *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_quaternion_get_w(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_set_w(pandemonium_quaternion *p_self, const pandemonium_real val);

pandemonium_real GDAPI pandemonium_quaternion_length_squared(const pandemonium_quaternion *p_self);
pandemonium_bool GDAPI pandemonium_quaternion_is_equal_approx(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_quat);
pandemonium_real GDAPI pandemonium_quaternion_length(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_normalize(pandemonium_quaternion *p_self);
pandemonium_quaternion GDAPI pandemonium_quaternion_normalized(const pandemonium_quaternion *p_self);
pandemonium_bool GDAPI pandemonium_quaternion_is_normalized(const pandemonium_quaternion *p_self);

pandemonium_quaternion GDAPI pandemonium_quaternion_inverse(const pandemonium_quaternion *p_self);
pandemonium_quaternion GDAPI pandemonium_quaternion_log(const pandemonium_quaternion *p_self);
pandemonium_quaternion GDAPI pandemonium_quaternion_exp(const pandemonium_quaternion *p_self);
pandemonium_real GDAPI pandemonium_quaternion_dot(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b);
pandemonium_real GDAPI pandemonium_quaternion_angle_to(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_to);

void GDAPI pandemonium_quaternion_set_euler_xyz(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler);
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler_xyz(const pandemonium_quaternion *p_self);
void GDAPI pandemonium_quaternion_set_euler_yxz(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler);
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler_yxz(const pandemonium_quaternion *p_self);

void GDAPI pandemonium_quaternion_set_euler(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_euler);
pandemonium_vector3 GDAPI pandemonium_quaternion_get_euler(const pandemonium_quaternion *p_self);

pandemonium_quaternion GDAPI pandemonium_quaternion_slerp(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_real p_t);
pandemonium_quaternion GDAPI pandemonium_quaternion_slerpni(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_real p_t);
pandemonium_quaternion GDAPI pandemonium_quaternion_cubic_slerp(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_quaternion *p_pre_a, const pandemonium_quaternion *p_post_b, const pandemonium_real p_t);
pandemonium_quaternion GDAPI pandemonium_quaternion_spherical_cubic_interpolate(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b, const pandemonium_quaternion *p_pre_a, const pandemonium_quaternion *p_post_b, const pandemonium_real p_weight);

pandemonium_vector3 GDAPI pandemonium_quaternion_get_axis(const pandemonium_quaternion *p_self);
pandemonium_real GDAPI pandemonium_quaternion_get_angle(const pandemonium_quaternion *p_self);

void GDAPI pandemonium_quaternion_set_axis_angle(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_axis, const pandemonium_real p_angle);
void GDAPI pandemonium_quaternion_get_axis_angle(const pandemonium_quaternion *p_self, pandemonium_vector3 *r_axis, pandemonium_real r_angle);

void GDAPI pandemonium_quaternion_operator_mul_eq(pandemonium_quaternion *p_self, const pandemonium_quaternion *p_q);

pandemonium_vector3 GDAPI pandemonium_quaternion_xform(const pandemonium_quaternion *p_self, const pandemonium_vector3 *p_v);

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_add(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b);
pandemonium_quaternion GDAPI pandemonium_quaternion_operator_subtract(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b);

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_multiply(const pandemonium_quaternion *p_self, const pandemonium_real p_b);
pandemonium_quaternion GDAPI pandemonium_quaternion_operator_divide(const pandemonium_quaternion *p_self, const pandemonium_real p_b);

pandemonium_quaternion GDAPI pandemonium_quaternion_operator_neg(const pandemonium_quaternion *p_self);

pandemonium_bool GDAPI pandemonium_quaternion_operator_equal(const pandemonium_quaternion *p_self, const pandemonium_quaternion *p_b);

pandemonium_string GDAPI pandemonium_quaternion_as_string(const pandemonium_quaternion *p_self);

pandemonium_bool GDAPI pandemonium_quaternion_set_shortest_arc(pandemonium_quaternion *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to);

void GDAPI pandemonium_quaternion_set(pandemonium_quaternion *p_self, pandemonium_real p_x, pandemonium_real p_y, pandemonium_real p_z, pandemonium_real p_w);

void GDAPI pandemonium_quaternion_new(pandemonium_quaternion *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_z, const pandemonium_real p_w);
void GDAPI pandemonium_quaternion_new_with_axis_angle(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_axis, const pandemonium_real p_angle);
void GDAPI pandemonium_quaternion_new_with_basis(pandemonium_quaternion *r_dest, const pandemonium_basis *p_basis);
void GDAPI pandemonium_quaternion_new_with_euler(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_euler);
void GDAPI pandemonium_quaternion_new_shortest_arc(pandemonium_quaternion *r_dest, const pandemonium_vector3 *p_v0, const pandemonium_vector3 *p_v1);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_QUATERNION_H
