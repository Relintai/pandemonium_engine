/*************************************************************************/
/*  color.cpp                                                            */
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

#include "gdn/color.h"

#include "core/math/color.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_color) == sizeof(Color), "Color size mismatch");

pandemonium_real pandemonium_color_get_r(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->r;
}

void pandemonium_color_set_r(pandemonium_color *p_self, const pandemonium_real val) {
	Color *self = (Color *)p_self;
	self->r = val;
}

pandemonium_real pandemonium_color_get_g(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->g;
}

void pandemonium_color_set_g(pandemonium_color *p_self, const pandemonium_real val) {
	Color *self = (Color *)p_self;
	self->g = val;
}

pandemonium_real pandemonium_color_get_b(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->b;
}

void pandemonium_color_set_b(pandemonium_color *p_self, const pandemonium_real val) {
	Color *self = (Color *)p_self;
	self->b = val;
}

pandemonium_real pandemonium_color_get_a(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->a;
}

void pandemonium_color_set_a(pandemonium_color *p_self, const pandemonium_real val) {
	Color *self = (Color *)p_self;
	self->a = val;
}

pandemonium_bool GDAPI pandemonium_color_operator_equal(const pandemonium_color *p_self, const pandemonium_color *p_b) {
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	return *self == *b;
}

pandemonium_int GDAPI pandemonium_color_to_rgba32(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_rgba32();
}

pandemonium_int GDAPI pandemonium_color_to_abgr32(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_abgr32();
}

pandemonium_int GDAPI pandemonium_color_to_abgr64(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_abgr64();
}

pandemonium_int GDAPI pandemonium_color_to_argb64(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_argb64();
}

pandemonium_int GDAPI pandemonium_color_to_rgba64(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_rgba64();
}

pandemonium_int GDAPI pandemonium_color_to_argb32(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_argb32();
}

pandemonium_real GDAPI pandemonium_color_gray(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->gray();
}

pandemonium_real pandemonium_color_get_h(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_h();
}

pandemonium_real pandemonium_color_get_s(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_s();
}

pandemonium_real pandemonium_color_get_v(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_v();
}

void GDAPI pandemonium_color_set_hsv(pandemonium_color *p_self, pandemonium_real p_h, pandemonium_real p_s, pandemonium_real p_v) {
	Color *self = (Color *)p_self;
	self->set_hsv(p_h, p_s, p_v);
}
void GDAPI pandemonium_color_set_hsv_alpha(pandemonium_color *p_self, pandemonium_real p_h, pandemonium_real p_s, pandemonium_real p_v, pandemonium_real p_alpha) {
	Color *self = (Color *)p_self;
	self->set_hsv(p_h, p_s, p_v, p_alpha);
}

pandemonium_real GDAPI *pandemonium_color_operator_index(pandemonium_color *p_self, pandemonium_int idx) {
	Color *self = (Color *)p_self;
	return (pandemonium_real *)&self->operator[](idx);
}
const pandemonium_real GDAPI *pandemonium_color_operator_index_const(const pandemonium_color *p_self, pandemonium_int idx) {
	const Color *self = (const Color *)p_self;
	return (const pandemonium_real *)&self->operator[](idx);
}

pandemonium_color GDAPI pandemonium_color_operator_add(const pandemonium_color *p_self, const pandemonium_color *p_color) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *color = (const Color *)p_color;
	*((Color *)&dest) = self->operator+(*color);
	return dest;
}
void GDAPI pandemonium_color_operator_add_eq(pandemonium_color *p_self, const pandemonium_color *p_color) {
	Color *self = (Color *)p_self;
	const Color *color = (const Color *)p_color;
	self->operator+=(*color);
}

pandemonium_color GDAPI pandemonium_color_operator_negate(const pandemonium_color *p_self) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->operator-();
	return dest;
}

pandemonium_color GDAPI pandemonium_color_operator_sub(const pandemonium_color *p_self, const pandemonium_color *p_color) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *color = (const Color *)p_color;
	*((Color *)&dest) = self->operator-(*color);
	return dest;
}
void GDAPI pandemonium_color_operator_sub_eq(pandemonium_color *p_self, const pandemonium_color *p_color) {
	Color *self = (Color *)p_self;
	const Color *color = (const Color *)p_color;
	self->operator-=(*color);
}

pandemonium_color GDAPI pandemonium_color_operator_mul_color(const pandemonium_color *p_self, const pandemonium_color *p_color) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *color = (const Color *)p_color;
	*((Color *)&dest) = self->operator*(*color);
	return dest;
}
pandemonium_color GDAPI pandemonium_color_operator_mul_real(const pandemonium_color *p_self, const pandemonium_real rvalue) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->operator*(rvalue);
	return dest;
}
void GDAPI pandemonium_color_operator_mul_color_eq(pandemonium_color *p_self, const pandemonium_color *p_color) {
	Color *self = (Color *)p_self;
	const Color *color = (const Color *)p_color;
	self->operator*=(*color);
}
void GDAPI pandemonium_color_operator_mul_real_eq(pandemonium_color *p_self, const pandemonium_real rvalue) {
	Color *self = (Color *)p_self;
	self->operator*=(rvalue);
}

pandemonium_color GDAPI pandemonium_color_operator_div_color(const pandemonium_color *p_self, const pandemonium_color *p_color) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *color = (const Color *)p_color;
	*((Color *)&dest) = self->operator/(*color);
	return dest;
}
pandemonium_color GDAPI pandemonium_color_operator_div_real(const pandemonium_color *p_self, const pandemonium_real rvalue) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->operator/(rvalue);
	return dest;
}
void GDAPI pandemonium_color_operator_div_color_eq(pandemonium_color *p_self, const pandemonium_color *p_color) {
	Color *self = (Color *)p_self;
	const Color *color = (const Color *)p_color;
	self->operator/=(*color);
}
void GDAPI pandemonium_color_operator_div_real_eq(pandemonium_color *p_self, const pandemonium_real rvalue) {
	Color *self = (Color *)p_self;
	self->operator/=(rvalue);
}

pandemonium_bool GDAPI pandemonium_color_is_equal_approx(const pandemonium_color *p_self, const pandemonium_color *p_color) {
	const Color *self = (const Color *)p_self;
	const Color *color = (const Color *)p_color;
	return self->is_equal_approx(*color);
}

pandemonium_color GDAPI pandemonium_color_clamp(const pandemonium_color *p_self, const pandemonium_color *p_min) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *min = (const Color *)p_min;
	*((Color *)&dest) = self->clamp(*min);
	return dest;
}

void GDAPI pandemonium_color_invert(pandemonium_color *p_self) {
	Color *self = (Color *)p_self;
	self->invert();
}
void GDAPI pandemonium_color_contrast(pandemonium_color *p_self) {
	Color *self = (Color *)p_self;
	self->contrast();
}

pandemonium_color GDAPI pandemonium_color_inverted(const pandemonium_color *p_self) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->inverted();
	return dest;
}

pandemonium_color GDAPI pandemonium_color_contrasted(const pandemonium_color *p_self) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->contrasted();
	return dest;
}

pandemonium_real GDAPI pandemonium_color_get_luminance(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_luminance();
}

pandemonium_color GDAPI pandemonium_color_linear_interpolate(const pandemonium_color *p_self, const pandemonium_color *p_b, const pandemonium_real p_t) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	*((Color *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}

pandemonium_color GDAPI pandemonium_color_darkened(const pandemonium_color *p_self, const pandemonium_real p_amount) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->darkened(p_amount);
	return dest;
}

pandemonium_color GDAPI pandemonium_color_lightened(const pandemonium_color *p_self, const pandemonium_real p_amount) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->lightened(p_amount);
	return dest;
}

pandemonium_int GDAPI pandemonium_color_to_rgbe9995(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_rgbe9995();
}

pandemonium_color GDAPI pandemonium_color_blend(const pandemonium_color *p_self, const pandemonium_color *p_over) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	const Color *over = (const Color *)p_over;
	*((Color *)&dest) = self->blend(*over);
	return dest;
}

pandemonium_color GDAPI pandemonium_color_from_hsv(const pandemonium_color *p_self, const pandemonium_real p_h, const pandemonium_real p_s, const pandemonium_real p_v, const pandemonium_real p_a) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->from_hsv(p_h, p_s, p_v, p_a);
	return dest;
}

pandemonium_color GDAPI pandemonium_color_from_rgbe9995(pandemonium_int p_rgbe) {
	pandemonium_color dest;
	*((Color *)&dest) = Color::from_rgbe9995(p_rgbe);
	return dest;
}

pandemonium_color GDAPI pandemonium_color_to_linear(const pandemonium_color *p_self) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->to_linear();
	return dest;
}
pandemonium_color GDAPI pandemonium_color_to_srgb(const pandemonium_color *p_self) {
	pandemonium_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->to_srgb();
	return dest;
}

pandemonium_color GDAPI pandemonium_color_hex(pandemonium_int p_hex) {
	pandemonium_color dest;
	*((Color *)&dest) = Color::hex(p_hex);
	return dest;
}
pandemonium_color GDAPI pandemonium_color_hex64(pandemonium_int p_hex) {
	pandemonium_color dest;
	*((Color *)&dest) = Color::hex64(p_hex);
	return dest;
}
pandemonium_color GDAPI pandemonium_color_html(const pandemonium_string *p_color) {
	pandemonium_color dest;
	const String *color = (const String *)p_color;
	*((Color *)&dest) = Color::html(*color);
	return dest;
}
pandemonium_bool GDAPI pandemonium_color_html_is_valid(const pandemonium_string *p_color) {
	const String *color = (const String *)p_color;
	return Color::html_is_valid(*color);
}
pandemonium_color GDAPI pandemonium_color_named(const pandemonium_string *p_name) {
	pandemonium_color dest;
	const String *name = (const String *)p_name;
	*((Color *)&dest) = Color::named(*name);
	return dest;
}

pandemonium_string GDAPI pandemonium_color_to_html(const pandemonium_color *p_self, const pandemonium_bool p_with_alpha) {
	pandemonium_string dest;
	const Color *self = (const Color *)p_self;

	memnew_placement(&dest, String(self->to_html(p_with_alpha)));
	return dest;
}

pandemonium_bool GDAPI pandemonium_color_operator_less(const pandemonium_color *p_self, const pandemonium_color *p_b) {
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	return *self < *b;
}

pandemonium_string GDAPI pandemonium_color_as_string(const pandemonium_color *p_self) {
	pandemonium_string ret;
	const Color *self = (const Color *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

pandemonium_color GDAPI pandemonium_color_color8(pandemonium_int r, pandemonium_int g, pandemonium_int b) {
	pandemonium_color dest;
	*((Color *)&dest) = Color::color8(r, g, b);
	return dest;
}
pandemonium_color GDAPI pandemonium_color_color8_alpha(pandemonium_int r, pandemonium_int g, pandemonium_int b, pandemonium_int a) {
	pandemonium_color dest;
	*((Color *)&dest) = Color::color8(r, g, b, a);
	return dest;
}

void GDAPI pandemonium_color_set_r8(pandemonium_color *p_self, pandemonium_int r8) {
	Color *self = (Color *)p_self;
	self->set_r8(r8);
}
pandemonium_int GDAPI pandemonium_color_get_r8(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_r8();
}
void GDAPI pandemonium_color_set_g8(pandemonium_color *p_self, pandemonium_int g8) {
	Color *self = (Color *)p_self;
	self->set_g8(g8);
}
pandemonium_int GDAPI pandemonium_color_get_g8(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_g8();
}
void GDAPI pandemonium_color_set_b8(pandemonium_color *p_self, pandemonium_int b8) {
	Color *self = (Color *)p_self;
	self->set_b8(b8);
}
pandemonium_int GDAPI pandemonium_color_get_b8(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_b8();
}
void GDAPI pandemonium_color_set_a8(pandemonium_color *p_self, pandemonium_int a8) {
	Color *self = (Color *)p_self;
	self->set_a8(a8);
}
pandemonium_int GDAPI pandemonium_color_get_a8(const pandemonium_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->get_a8();
}

void GDAPI pandemonium_color_set_h(pandemonium_color *p_self, pandemonium_real p_h) {
	Color *self = (Color *)p_self;
	self->set_h(p_h);
}
void GDAPI pandemonium_color_set_s(pandemonium_color *p_self, pandemonium_real p_s) {
	Color *self = (Color *)p_self;
	self->set_s(p_s);
}
void GDAPI pandemonium_color_set_v(pandemonium_color *p_self, pandemonium_real p_v) {
	Color *self = (Color *)p_self;
	self->set_v(p_v);
}

void GDAPI pandemonium_color_new_rgba(pandemonium_color *r_dest, const pandemonium_real p_r, const pandemonium_real p_g, const pandemonium_real p_b, const pandemonium_real p_a) {
	Color *dest = (Color *)r_dest;
	*dest = Color(p_r, p_g, p_b, p_a);
}

void GDAPI pandemonium_color_new_rgb(pandemonium_color *r_dest, const pandemonium_real p_r, const pandemonium_real p_g, const pandemonium_real p_b) {
	Color *dest = (Color *)r_dest;
	*dest = Color(p_r, p_g, p_b);
}

#ifdef __cplusplus
}
#endif
