#ifndef GDNATIVE_RECT2I_H
#define GDNATIVE_RECT2I_H

/*************************************************************************/
/*  rect2i.h                                                             */
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

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_RECT2I_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_RECT2I_TYPE_DEFINED
typedef struct pandemonium_rect2i {
	uint8_t _dont_touch_that[16];
} pandemonium_rect2i;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/transform2d.h>
#include <gdn/vector2i.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_vector2i GDAPI pandemonium_rect2i_get_position(const pandemonium_rect2i *p_self);
void GDAPI pandemonium_rect2i_set_position(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_pos);

pandemonium_vector2i GDAPI pandemonium_rect2i_get_size(const pandemonium_rect2i *p_self);
void GDAPI pandemonium_rect2i_set_size(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_size);

pandemonium_real GDAPI pandemonium_rect2i_get_area(const pandemonium_rect2i *p_self);

pandemonium_vector2i GDAPI pandemonium_rect2i_get_center(const pandemonium_rect2i *p_self);

pandemonium_bool GDAPI pandemonium_rect2i_intersects(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b);

pandemonium_bool GDAPI pandemonium_rect2i_encloses(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b);

pandemonium_bool GDAPI pandemonium_rect2i_has_no_area(const pandemonium_rect2i *p_self);
pandemonium_rect2i GDAPI pandemonium_rect2i_clip(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b);

pandemonium_rect2i GDAPI pandemonium_rect2i_intersection(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_rect);

pandemonium_rect2i GDAPI pandemonium_rect2i_merge(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b);
pandemonium_bool GDAPI pandemonium_rect2i_has_point(const pandemonium_rect2i *p_self, const pandemonium_vector2i *p_point);

pandemonium_bool GDAPI pandemonium_rect2i_operator_equal(const pandemonium_rect2i *p_self, const pandemonium_rect2i *p_b);

pandemonium_rect2i GDAPI pandemonium_rect2i_grow(const pandemonium_rect2i *p_self, const pandemonium_real p_by);
void GDAPI pandemonium_rect2i_grow_by(pandemonium_rect2i *p_self, const pandemonium_real p_by);
pandemonium_rect2i GDAPI pandemonium_rect2i_grow_margin(const pandemonium_rect2i *p_self, const pandemonium_int p_margin, const pandemonium_real p_by);
pandemonium_rect2i GDAPI pandemonium_rect2i_grow_side(const pandemonium_rect2i *p_self, const pandemonium_int p_side, const pandemonium_real p_amount);
pandemonium_rect2i GDAPI pandemonium_rect2i_grow_individual(const pandemonium_rect2i *p_self, const pandemonium_real p_left, const pandemonium_real p_top, const pandemonium_real p_right, const pandemonium_real p_bottom);

pandemonium_rect2i GDAPI pandemonium_rect2i_expand(const pandemonium_rect2i *p_self, const pandemonium_vector2i *p_to);
void GDAPI pandemonium_rect2i_expand_to(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_vector);

pandemonium_rect2i GDAPI pandemonium_rect2i_abs(const pandemonium_rect2i *p_self);

void GDAPI pandemonium_rect2i_set_end(pandemonium_rect2i *p_self, const pandemonium_vector2i *p_end);
pandemonium_vector2i GDAPI pandemonium_rect2i_get_end(const pandemonium_rect2i *p_self);

pandemonium_rect2 GDAPI pandemonium_rect2i_to_rect2(const pandemonium_rect2i *p_self);

pandemonium_string GDAPI pandemonium_rect2i_as_string(const pandemonium_rect2i *p_self);

void GDAPI pandemonium_rect2i_new_with_position_and_size(pandemonium_rect2i *r_dest, const pandemonium_vector2i *p_pos, const pandemonium_vector2i *p_size);
void GDAPI pandemonium_rect2i_new(pandemonium_rect2i *r_dest, const pandemonium_real p_x, const pandemonium_real p_y, const pandemonium_real p_width, const pandemonium_real p_height);
void GDAPI pandemonium_rect2i_new_from_rect2(pandemonium_rect2i *r_dest, const pandemonium_rect2 *p_r2);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_RECT2I_H
