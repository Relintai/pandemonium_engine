/*************************************************************************/
/*  rect2i.cpp                                                           */
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

#include "gdn/rect2i.h"

#include "core/math/rect2.h"
#include "core/math/rect2i.h"
#include "core/math/transform_2d.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_rect2i) == sizeof(Rect2i), "Rect2i size mismatch");

pandemonium_vector2i GDAPI pandemonium_rect2i_get_position(const pandemonium_rect2i *p_self) {
	pandemonium_vector2i dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*d = self->get_position();
	return dest;
}
void GDAPI pandemonium_rect2i_set_position(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_pos) {
	Rect2i *self = (Rect2i *)p_self;
	const Vector2 *position = (const Vector2 *)p_pos;
	self->set_position(*position);
}

pandemonium_vector2i GDAPI pandemonium_rect2i_get_size(const pandemonium_rect2i *p_self) {
	pandemonium_vector2i dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*d = self->get_size();
	return dest;
}
void GDAPI pandemonium_rect2i_set_size(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_size) {
	Rect2i *self = (Rect2i *)p_self;
	const Vector2 *size = (const Vector2 *)p_size;
	self->set_size(*size);
}

pandemonium_real GDAPI pandemonium_rect2i_get_area(const pandemonium_rect2i *p_self) {
	const Rect2i *self = (const Rect2i *)p_self;
	return self->get_area();
}

pandemonium_vector2i GDAPI pandemonium_rect2i_get_center(const pandemonium_rect2i *p_self) {
	pandemonium_vector2i dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*d = self->get_center();
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2i_intersects(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b) {
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *b = (const Rect2i *)p_b;
	return self->intersects(*b);
}

pandemonium_bool GDAPI pandemonium_rect2i_encloses(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b) {
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *b = (const Rect2i *)p_b;
	return self->encloses(*b);
}

pandemonium_bool GDAPI pandemonium_rect2i_has_no_area(const pandemonium_rect2i *p_self) {
	const Rect2i *self = (const Rect2i *)p_self;
	return self->has_no_area();
}

pandemonium_rect2i GDAPI pandemonium_rect2i_clip(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *b = (const Rect2i *)p_b;
	*((Rect2i *)&dest) = self->clip(*b);
	return dest;
}

pandemonium_rect2i GDAPI pandemonium_rect2i_intersection(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_rect) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *rect = (const Rect2i *)p_rect;
	*((Rect2i *)&dest) = self->intersection(*rect);
	return dest;
}

pandemonium_rect2i GDAPI pandemonium_rect2i_merge(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *b = (const Rect2i *)p_b;
	*((Rect2i *)&dest) = self->merge(*b);
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2i_has_point(const pandemonium_rect2i *p_self, const pandemonium_vector2i *p_point) {
	const Rect2i *self = (const Rect2i *)p_self;
	const Vector2 *point = (const Vector2 *)p_point;
	return self->has_point(*point);
}

pandemonium_rect2i GDAPI pandemonium_rect2i_grow(const pandemonium_rect2i *p_self, const pandemonium_real p_by) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2i *)&dest) = self->grow(p_by);
	return dest;
}

void GDAPI pandemonium_rect2i_grow_by(pandemonium_rect2i *p_self, const pandemonium_real p_by) {
	Rect2i *self = (Rect2i *)p_self;
	self->grow_by(p_by);
}

pandemonium_rect2i GDAPI pandemonium_rect2i_grow_individual(const pandemonium_rect2i *p_self, const pandemonium_real p_left, const pandemonium_real p_top, const pandemonium_real p_right, const pandemonium_real p_bottom) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2i *)&dest) = self->grow_individual(p_left, p_top, p_right, p_bottom);
	return dest;
}

pandemonium_rect2i GDAPI pandemonium_rect2i_grow_side(const pandemonium_rect2i *p_self, const pandemonium_int p_side, const pandemonium_real p_amount) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2i *)&dest) = self->grow_side(static_cast<Side>(p_side), p_amount);
	return dest;
}

pandemonium_rect2i GDAPI pandemonium_rect2i_grow_margin(const pandemonium_rect2i *p_self, const pandemonium_int p_margin, const pandemonium_real p_by) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2i *)&dest) = self->grow_margin((Margin)p_margin, p_by);
	return dest;
}

pandemonium_rect2i GDAPI pandemonium_rect2i_expand(const pandemonium_rect2i *p_self, const pandemonium_vector2i *p_to) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Rect2i *)&dest) = self->expand(*to);
	return dest;
}

void GDAPI pandemonium_rect2i_expand_to(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_vector) {
	Rect2i *self = (Rect2i *)p_self;
	const Vector2 *vector = (const Vector2 *)p_vector;
	self->expand_to(*vector);
}

pandemonium_rect2i GDAPI pandemonium_rect2i_abs(const pandemonium_rect2i *p_self) {
	pandemonium_rect2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2i *)&dest) = self->abs();
	return dest;
}

void GDAPI pandemonium_rect2i_set_end(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_end) {
	Rect2i *self = (Rect2i *)p_self;
	const Vector2 *end = (const Vector2 *)p_end;
	self->set_end(*end);
}

pandemonium_vector2i GDAPI pandemonium_rect2i_get_end(const pandemonium_rect2i *p_self) {
	pandemonium_vector2i dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Vector2 *)&dest) = self->get_end();
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2i_operator_equal(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b) {
	const Rect2i *self = (const Rect2i *)p_self;
	const Rect2i *b = (const Rect2i *)p_b;
	return *self == *b;
}

pandemonium_rect2 GDAPI pandemonium_rect2i_to_rect2(const pandemonium_rect2i *p_self) {
	pandemonium_rect2 dest;
	const Rect2i *self = (const Rect2i *)p_self;
	*((Rect2 *)&dest) = self->to_rect2();
	return dest;
}

pandemonium_string GDAPI pandemonium_rect2i_as_string(const pandemonium_rect2i *p_self) {
	pandemonium_string ret;
	const Rect2i *self = (const Rect2i *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_rect2i_new_with_position_and_size(pandemonium_rect2i *r_dest, const pandemonium_vector2i *p_pos, const pandemonium_vector2i *p_size) {
	const Vector2 *position = (const Vector2 *)p_pos;
	const Vector2 *size = (const Vector2 *)p_size;
	Rect2i *dest = (Rect2i *)r_dest;
	*dest = Rect2i(*position, *size);
}

void GDAPI pandemonium_rect2i_new(pandemonium_rect2i *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_width, const pandemonium_real p_height) {
	Rect2i *dest = (Rect2i *)r_dest;
	*dest = Rect2i(p_x, p_y, p_width, p_height);
}

void GDAPI pandemonium_rect2i_new_from_rect2(pandemonium_rect2i *r_dest, const pandemonium_rect2 *p_r2) {
	Rect2i *dest = (Rect2i *)r_dest;
	const Rect2 *r2 = (const Rect2 *)p_r2;
	*dest = Rect2i(*r2);
}

#ifdef __cplusplus
}
#endif
