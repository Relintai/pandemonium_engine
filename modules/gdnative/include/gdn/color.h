/**************************************************************************/
/*  color.h                                                               */
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

#ifndef GDNATIVE_COLOR_H
#define GDNATIVE_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_COLOR_SIZE 16

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_COLOR_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_COLOR_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_COLOR_SIZE];
} pandemonium_color;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/string.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_real pandemonium_color_get_r(const pandemonium_color *p_self);
void pandemonium_color_set_r(pandemonium_color *p_self, const pandemonium_real r);

pandemonium_real pandemonium_color_get_g(const pandemonium_color *p_self);
void pandemonium_color_set_g(pandemonium_color *p_self, const pandemonium_real g);

pandemonium_real pandemonium_color_get_b(const pandemonium_color *p_self);
void pandemonium_color_set_b(pandemonium_color *p_self, const pandemonium_real b);

pandemonium_real pandemonium_color_get_a(const pandemonium_color *p_self);
void pandemonium_color_set_a(pandemonium_color *p_self, const pandemonium_real a);

pandemonium_bool GDAPI pandemonium_color_operator_equal(const pandemonium_color *p_self, const pandemonium_color *p_b);

pandemonium_int GDAPI pandemonium_color_to_rgba32(const pandemonium_color *p_self);
pandemonium_int GDAPI pandemonium_color_to_argb32(const pandemonium_color *p_self);
pandemonium_int GDAPI pandemonium_color_to_abgr32(const pandemonium_color *p_self);
pandemonium_int GDAPI pandemonium_color_to_rgba64(const pandemonium_color *p_self);
pandemonium_int GDAPI pandemonium_color_to_argb64(const pandemonium_color *p_self);
pandemonium_int GDAPI pandemonium_color_to_abgr64(const pandemonium_color *p_self);

pandemonium_real GDAPI pandemonium_color_gray(const pandemonium_color *p_self);
pandemonium_real pandemonium_color_get_h(const pandemonium_color *p_self);
pandemonium_real pandemonium_color_get_s(const pandemonium_color *p_self);
pandemonium_real pandemonium_color_get_v(const pandemonium_color *p_self);
void GDAPI pandemonium_color_set_hsv(pandemonium_color *p_self, pandemonium_real p_h, pandemonium_real p_s, pandemonium_real p_v);
void GDAPI pandemonium_color_set_hsv_alpha(pandemonium_color *p_self, pandemonium_real p_h, pandemonium_real p_s, pandemonium_real p_v, pandemonium_real p_alpha);

pandemonium_real GDAPI *pandemonium_color_operator_index(pandemonium_color *p_self, pandemonium_int idx);
const pandemonium_real GDAPI *pandemonium_color_operator_index_const(const pandemonium_color *p_self, pandemonium_int idx);

pandemonium_color GDAPI pandemonium_color_operator_add(const pandemonium_color *p_self, const pandemonium_color *p_color);
void GDAPI pandemonium_color_operator_add_eq(pandemonium_color *p_self, const pandemonium_color *p_color);

pandemonium_color GDAPI pandemonium_color_operator_negate(const pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_operator_sub(const pandemonium_color *p_self, const pandemonium_color *p_color);
void GDAPI pandemonium_color_operator_sub_eq(pandemonium_color *p_self, const pandemonium_color *p_color);

pandemonium_color GDAPI pandemonium_color_operator_mul_color(const pandemonium_color *p_self, const pandemonium_color *p_color);
pandemonium_color GDAPI pandemonium_color_operator_mul_real(const pandemonium_color *p_self, const pandemonium_real rvalue);
void GDAPI pandemonium_color_operator_mul_color_eq(pandemonium_color *p_self, const pandemonium_color *p_color);
void GDAPI pandemonium_color_operator_mul_real_eq(pandemonium_color *p_self, const pandemonium_real rvalue);

pandemonium_color GDAPI pandemonium_color_operator_div_color(const pandemonium_color *p_self, const pandemonium_color *p_color);
pandemonium_color GDAPI pandemonium_color_operator_div_real(const pandemonium_color *p_self, const pandemonium_real rvalue);
void GDAPI pandemonium_color_operator_div_color_eq(pandemonium_color *p_self, const pandemonium_color *p_color);
void GDAPI pandemonium_color_operator_div_real_eq(pandemonium_color *p_self, const pandemonium_real rvalue);

pandemonium_bool GDAPI pandemonium_color_is_equal_approx(const pandemonium_color *p_self, const pandemonium_color *p_color);

pandemonium_color GDAPI pandemonium_color_clamp(const pandemonium_color *p_self, const pandemonium_color *p_min);

void GDAPI pandemonium_color_invert(pandemonium_color *p_self);
void GDAPI pandemonium_color_contrast(pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_inverted(const pandemonium_color *p_self);
pandemonium_color GDAPI pandemonium_color_contrasted(const pandemonium_color *p_self);

pandemonium_real GDAPI pandemonium_color_get_luminance(const pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_linear_interpolate(const pandemonium_color *p_self, const pandemonium_color *p_b, const pandemonium_real p_t);

pandemonium_color GDAPI pandemonium_color_darkened(const pandemonium_color *p_self, const pandemonium_real p_amount);
pandemonium_color GDAPI pandemonium_color_lightened(const pandemonium_color *p_self, const pandemonium_real p_amount);

pandemonium_int GDAPI pandemonium_color_to_rgbe9995(const pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_blend(const pandemonium_color *p_self, const pandemonium_color *p_over);

pandemonium_color GDAPI pandemonium_color_to_linear(const pandemonium_color *p_self);
pandemonium_color GDAPI pandemonium_color_to_srgb(const pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_hex(pandemonium_int p_hex);
pandemonium_color GDAPI pandemonium_color_hex64(pandemonium_int p_hex);
pandemonium_color GDAPI pandemonium_color_html(const pandemonium_string *p_color);
pandemonium_bool GDAPI pandemonium_color_html_is_valid(const pandemonium_string *p_color);
pandemonium_color GDAPI pandemonium_color_named(const pandemonium_string *p_name);

pandemonium_string GDAPI pandemonium_color_to_html(const pandemonium_color *p_self, const pandemonium_bool p_with_alpha);
pandemonium_color GDAPI pandemonium_color_from_hsv(const pandemonium_color *p_self, const pandemonium_real p_h, const pandemonium_real p_s, const pandemonium_real p_v, const pandemonium_real p_a);
pandemonium_color GDAPI pandemonium_color_from_rgbe9995(pandemonium_int p_rgbe);

pandemonium_bool GDAPI pandemonium_color_operator_less(const pandemonium_color *p_self, const pandemonium_color *p_b);
pandemonium_string GDAPI pandemonium_color_as_string(const pandemonium_color *p_self);

pandemonium_color GDAPI pandemonium_color_color8(pandemonium_int r, pandemonium_int g, pandemonium_int b);
pandemonium_color GDAPI pandemonium_color_color8_alpha(pandemonium_int r, pandemonium_int g, pandemonium_int b, pandemonium_int a);

void GDAPI pandemonium_color_set_r8(pandemonium_color *p_self, pandemonium_int r8);
pandemonium_int GDAPI pandemonium_color_get_r8(const pandemonium_color *p_self);
void GDAPI pandemonium_color_set_g8(pandemonium_color *p_self, pandemonium_int g8);
pandemonium_int GDAPI pandemonium_color_get_g8(const pandemonium_color *p_self);
void GDAPI pandemonium_color_set_b8(pandemonium_color *p_self, pandemonium_int b8);
pandemonium_int GDAPI pandemonium_color_get_b8(const pandemonium_color *p_self);
void GDAPI pandemonium_color_set_a8(pandemonium_color *p_self, pandemonium_int a8);
pandemonium_int GDAPI pandemonium_color_get_a8(const pandemonium_color *p_self);

void GDAPI pandemonium_color_set_h(pandemonium_color *p_self, pandemonium_real p_h);
void GDAPI pandemonium_color_set_s(pandemonium_color *p_self, pandemonium_real p_s);
void GDAPI pandemonium_color_set_v(pandemonium_color *p_self, pandemonium_real p_v);

void GDAPI pandemonium_color_new_rgba(pandemonium_color *r_dest, const pandemonium_real p_r, const pandemonium_real p_g, const pandemonium_real p_b, const pandemonium_real p_a);
void GDAPI pandemonium_color_new_rgb(pandemonium_color *r_dest, const pandemonium_real p_r, const pandemonium_real p_g, const pandemonium_real p_b);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_COLOR_H
