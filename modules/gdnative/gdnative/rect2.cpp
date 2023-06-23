/**************************************************************************/
/*  rect2.cpp                                                             */
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

#include "gdn/rect2.h"

#include "core/math/rect2.h"
#include "core/math/transform_2d.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_rect2) == sizeof(Rect2), "Rect2 size mismatch");

pandemonium_vector2 GDAPI pandemonium_rect2_get_position(const pandemonium_rect2 *p_self) {
	pandemonium_vector2 dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*d = self->get_position();
	return dest;
}
void GDAPI pandemonium_rect2_set_position(pandemonium_rect2 *p_self, const pandemonium_vector2 *p_pos) {
	Rect2 *self = (Rect2 *)p_self;
	const Vector2 *position = (const Vector2 *)p_pos;
	self->set_position(*position);
}

pandemonium_vector2 GDAPI pandemonium_rect2_get_size(const pandemonium_rect2 *p_self) {
	pandemonium_vector2 dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*d = self->get_size();
	return dest;
}
void GDAPI pandemonium_rect2_set_size(pandemonium_rect2 *p_self, const pandemonium_vector2 *p_size) {
	Rect2 *self = (Rect2 *)p_self;
	const Vector2 *size = (const Vector2 *)p_size;
	self->set_size(*size);
}

pandemonium_real GDAPI pandemonium_rect2_get_area(const pandemonium_rect2 *p_self) {
	const Rect2 *self = (const Rect2 *)p_self;
	return self->get_area();
}

pandemonium_vector2 GDAPI pandemonium_rect2_get_center(const pandemonium_rect2 *p_self) {
	pandemonium_vector2 dest;
	Vector2 *d = (Vector2 *)&dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*d = self->get_center();
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2_intersects(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_b) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *b = (const Rect2 *)p_b;
	return self->intersects(*b);
}

pandemonium_real GDAPI pandemonium_rect2_distance_to(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_point) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *point = (const Vector2 *)p_point;
	return self->distance_to(*point);
}

pandemonium_bool GDAPI pandemonium_rect2_intersects_transformed(const pandemonium_rect2 *p_self, const pandemonium_transform2d *p_xform, const pandemonium_rect2 *p_rect) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Transform2D *xform = (const Transform2D *)p_xform;
	const Rect2 *rect = (const Rect2 *)p_rect;
	return self->intersects_transformed(*xform, *rect);
}

pandemonium_bool GDAPI pandemonium_rect2_intersects_segment(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_from, const pandemonium_vector2 *p_to) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *from = (const Vector2 *)p_from;
	const Vector2 *to = (const Vector2 *)p_to;
	return self->intersects_segment(*from, *to);
}
pandemonium_bool GDAPI pandemonium_rect2_intersects_segment_pos(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_from, const pandemonium_vector2 *p_to, pandemonium_vector2 *r_pos) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *from = (const Vector2 *)p_from;
	const Vector2 *to = (const Vector2 *)p_to;
	Vector2 *pos = (Vector2 *)r_pos;
	return self->intersects_segment(*from, *to, pos);
}
pandemonium_bool GDAPI pandemonium_rect2_intersects_segment_pos_normal(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_from, const pandemonium_vector2 *p_to, pandemonium_vector2 *r_pos, pandemonium_vector2 *r_normal) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *from = (const Vector2 *)p_from;
	const Vector2 *to = (const Vector2 *)p_to;
	Vector2 *pos = (Vector2 *)r_pos;
	Vector2 *normal = (Vector2 *)r_normal;
	return self->intersects_segment(*from, *to, pos, normal);
}

pandemonium_bool GDAPI pandemonium_rect2_encloses(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_b) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *b = (const Rect2 *)p_b;
	return self->encloses(*b);
}

pandemonium_bool GDAPI pandemonium_rect2_has_no_area(const pandemonium_rect2 *p_self) {
	const Rect2 *self = (const Rect2 *)p_self;
	return self->has_no_area();
}

pandemonium_rect2 GDAPI pandemonium_rect2_clip(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_b) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *b = (const Rect2 *)p_b;
	*((Rect2 *)&dest) = self->clip(*b);
	return dest;
}

pandemonium_rect2 GDAPI pandemonium_rect2_intersection(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_rect) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *rect = (const Rect2 *)p_rect;
	*((Rect2 *)&dest) = self->intersection(*rect);
	return dest;
}

pandemonium_rect2 GDAPI pandemonium_rect2_merge(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_b) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *b = (const Rect2 *)p_b;
	*((Rect2 *)&dest) = self->merge(*b);
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2_has_point(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_point) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *point = (const Vector2 *)p_point;
	return self->has_point(*point);
}

pandemonium_bool GDAPI pandemonium_rect2_is_equal_approx(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_rect) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *rect = (const Rect2 *)p_rect;
	return self->is_equal_approx(*rect);
}

pandemonium_rect2 GDAPI pandemonium_rect2_grow(const pandemonium_rect2 *p_self, const pandemonium_real p_by) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Rect2 *)&dest) = self->grow(p_by);
	return dest;
}

void GDAPI pandemonium_rect2_grow_by(pandemonium_rect2 *p_self, const pandemonium_real p_by) {
	Rect2 *self = (Rect2 *)p_self;
	self->grow_by(p_by);
}

pandemonium_rect2 GDAPI pandemonium_rect2_grow_individual(const pandemonium_rect2 *p_self, const pandemonium_real p_left, const pandemonium_real p_top, const pandemonium_real p_right, const pandemonium_real p_bottom) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Rect2 *)&dest) = self->grow_individual(p_left, p_top, p_right, p_bottom);
	return dest;
}

pandemonium_rect2 GDAPI pandemonium_rect2_grow_side(const pandemonium_rect2 *p_self, const pandemonium_int p_side, const pandemonium_real p_amount) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Rect2 *)&dest) = self->grow_side(static_cast<Side>(p_side), p_amount);
	return dest;
}

pandemonium_rect2 GDAPI pandemonium_rect2_grow_margin(const pandemonium_rect2 *p_self, const pandemonium_int p_margin, const pandemonium_real p_by) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Rect2 *)&dest) = self->grow_margin((Margin)p_margin, p_by);
	return dest;
}

pandemonium_rect2 GDAPI pandemonium_rect2_expand(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_to) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *to = (const Vector2 *)p_to;
	*((Rect2 *)&dest) = self->expand(*to);
	return dest;
}

void GDAPI pandemonium_rect2_expand_to(pandemonium_rect2 *p_self, const pandemonium_vector2 *p_vector) {
	Rect2 *self = (Rect2 *)p_self;
	const Vector2 *vector = (const Vector2 *)p_vector;
	self->expand_to(*vector);
}

pandemonium_rect2 GDAPI pandemonium_rect2_abs(const pandemonium_rect2 *p_self) {
	pandemonium_rect2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Rect2 *)&dest) = self->abs();
	return dest;
}

pandemonium_vector2 GDAPI pandemonium_rect2_get_support(const pandemonium_rect2 *p_self, const pandemonium_vector2 *p_normal) {
	pandemonium_vector2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	const Vector2 *normal = (const Vector2 *)p_normal;
	*((Vector2 *)&dest) = self->get_support(*normal);
	return dest;
}

void GDAPI pandemonium_rect2_set_end(pandemonium_rect2 *p_self, const pandemonium_vector2 *p_end) {
	Rect2 *self = (Rect2 *)p_self;
	const Vector2 *end = (const Vector2 *)p_end;
	self->set_end(*end);
}

pandemonium_vector2 GDAPI pandemonium_rect2_get_end(const pandemonium_rect2 *p_self) {
	pandemonium_vector2 dest;
	const Rect2 *self = (const Rect2 *)p_self;
	*((Vector2 *)&dest) = self->get_end();
	return dest;
}

pandemonium_bool GDAPI pandemonium_rect2_operator_equal(const pandemonium_rect2 *p_self, const pandemonium_rect2 *p_b) {
	const Rect2 *self = (const Rect2 *)p_self;
	const Rect2 *b = (const Rect2 *)p_b;
	return *self == *b;
}

pandemonium_string GDAPI pandemonium_rect2_as_string(const pandemonium_rect2 *p_self) {
	pandemonium_string ret;
	const Rect2 *self = (const Rect2 *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_rect2_new_with_position_and_size(pandemonium_rect2 *r_dest, const pandemonium_vector2 *p_pos, const pandemonium_vector2 *p_size) {
	const Vector2 *position = (const Vector2 *)p_pos;
	const Vector2 *size = (const Vector2 *)p_size;
	Rect2 *dest = (Rect2 *)r_dest;
	*dest = Rect2(*position, *size);
}

void GDAPI pandemonium_rect2_new(pandemonium_rect2 *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_width, const pandemonium_real p_height) {
	Rect2 *dest = (Rect2 *)r_dest;
	*dest = Rect2(p_x, p_y, p_width, p_height);
}

#ifdef __cplusplus
}
#endif
