/**************************************************************************/
/*  vector4i.h                                                             */
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

#ifndef GDNATIVE_VECTOR4I_H
#define GDNATIVE_VECTOR4I_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_VECTOR4I_SIZE 16

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR4I_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_VECTOR4I_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_VECTOR4I_SIZE];
} pandemonium_vector4i;
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
	PANDEMONIUM_VECTOR4I_AXIS_X,
	PANDEMONIUM_VECTOR4I_AXIS_Y,
	PANDEMONIUM_VECTOR4I_AXIS_Z,
	PANDEMONIUM_VECTOR4I_AXIS_W,
} pandemonium_vector4i_axis;

pandemonium_int GDAPI pandemonium_vector4i_get_x(const pandemonium_vector4i *p_self);
void GDAPI pandemonium_vector4i_set_x(pandemonium_vector4i *p_self, const pandemonium_int val);

pandemonium_int GDAPI pandemonium_vector4i_get_y(const pandemonium_vector4i *p_self);
void GDAPI pandemonium_vector4i_set_y(pandemonium_vector4i *p_self, const pandemonium_int val);

pandemonium_int GDAPI pandemonium_vector4i_get_z(const pandemonium_vector4i *p_self);
void GDAPI pandemonium_vector4i_set_z(pandemonium_vector4i *p_self, const pandemonium_int val);

pandemonium_int GDAPI pandemonium_vector4i_get_w(const pandemonium_vector4i *p_self);
void GDAPI pandemonium_vector4i_set_w(pandemonium_vector4i *p_self, const pandemonium_int val);

pandemonium_int GDAPI *pandemonium_vector4i_operator_index(pandemonium_vector4i *p_self, const pandemonium_int p_idx);
const pandemonium_int GDAPI *pandemonium_vector4i_operator_index_const(const pandemonium_vector4i *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_vector4i_set_axis(pandemonium_vector4i *p_self, const pandemonium_vector4i_axis p_axis, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_vector4i_get_axis(const pandemonium_vector4i *p_self, const pandemonium_vector4i_axis p_axis);

void GDAPI pandemonium_vector4i_set_all(pandemonium_vector4i *p_self, const pandemonium_int p_val);

pandemonium_int GDAPI pandemonium_vector4i_min_axis(const pandemonium_vector4i *p_self);
pandemonium_int GDAPI pandemonium_vector4i_max_axis(const pandemonium_vector4i *p_self);

pandemonium_real GDAPI pandemonium_vector4i_length(const pandemonium_vector4i *p_self);
pandemonium_int GDAPI pandemonium_vector4i_length_squared(const pandemonium_vector4i *p_self);

void GDAPI pandemonium_vector4i_zero(pandemonium_vector4i *p_self);

pandemonium_vector4i GDAPI pandemonium_vector4i_linear_interpolate(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b, const pandemonium_real p_t);

pandemonium_vector4i GDAPI pandemonium_vector4i_abs(const pandemonium_vector4i *p_self);
pandemonium_vector4i GDAPI pandemonium_vector4i_sign(const pandemonium_vector4i *p_self);

pandemonium_vector4i GDAPI pandemonium_vector4i_clamp(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_min, const pandemonium_vector4i *p_max);

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_add(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);
pandemonium_vector4i GDAPI pandemonium_vector4i_operator_subtract(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_multiply_vector(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);
pandemonium_vector4i GDAPI pandemonium_vector4i_operator_multiply_scalar(const pandemonium_vector4i *p_self, const pandemonium_int p_b);

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_divide_vector(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);
pandemonium_vector4i GDAPI pandemonium_vector4i_operator_divide_scalar(const pandemonium_vector4i *p_self, const pandemonium_int p_b);

pandemonium_vector4i GDAPI pandemonium_vector4i_operator_neg(const pandemonium_vector4i *p_self);

pandemonium_bool GDAPI pandemonium_vector4i_operator_equal(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);
pandemonium_bool GDAPI pandemonium_vector4i_operator_less(const pandemonium_vector4i *p_self, const pandemonium_vector4i *p_b);

pandemonium_vector4 GDAPI pandemonium_vector4i_to_vector4(const pandemonium_vector4i *p_self);

pandemonium_string GDAPI pandemonium_vector4i_as_string(const pandemonium_vector4i *p_self);

void GDAPI pandemonium_vector4i_new(pandemonium_vector4i *r_dest, const pandemonium_int p_x, const pandemonium_int p_y, const pandemonium_int p_z, const pandemonium_int p_w);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR4I_H
