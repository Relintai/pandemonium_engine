#ifndef GDNATIVE_PLANE_H
#define GDNATIVE_PLANE_H

/*************************************************************************/
/*  plane.h                                                              */
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

#define PANDEMONIUM_PLANE_SIZE 16

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_PLANE_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_PLANE_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_PLANE_SIZE];
} pandemonium_plane;
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

pandemonium_real GDAPI pandemonium_plane_get_d(const pandemonium_plane *p_self);
void GDAPI pandemonium_plane_set_d(pandemonium_plane *p_self, const pandemonium_real p_d);

void GDAPI pandemonium_plane_set_normal(pandemonium_plane *p_self, const pandemonium_vector3 *p_normal);
pandemonium_vector3 GDAPI pandemonium_plane_get_normal(const pandemonium_plane *p_self);

void GDAPI pandemonium_plane_normalize(pandemonium_plane *p_self);
pandemonium_plane GDAPI pandemonium_plane_normalized(const pandemonium_plane *p_self);

pandemonium_vector3 GDAPI pandemonium_plane_center(const pandemonium_plane *p_self);
pandemonium_vector3 GDAPI pandemonium_plane_get_any_point(const pandemonium_plane *p_self);
pandemonium_vector3 GDAPI pandemonium_plane_get_any_perpendicular_normal(const pandemonium_plane *p_self);

pandemonium_bool GDAPI pandemonium_plane_is_point_over(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point);
pandemonium_real GDAPI pandemonium_plane_distance_to(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point);
pandemonium_bool GDAPI pandemonium_plane_has_point(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point, const pandemonium_real p_epsilon);

pandemonium_bool GDAPI pandemonium_plane_intersect_3(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_plane *p_b, const pandemonium_plane *p_c);
pandemonium_bool GDAPI pandemonium_plane_intersects_ray(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_dir);
pandemonium_bool GDAPI pandemonium_plane_intersects_segment(const pandemonium_plane *p_self, pandemonium_vector3 *r_dest, const pandemonium_vector3 *p_begin, const pandemonium_vector3 *p_end);

pandemonium_vector3 GDAPI pandemonium_plane_project(const pandemonium_plane *p_self, const pandemonium_vector3 *p_point);

pandemonium_plane GDAPI pandemonium_plane_operator_neg(const pandemonium_plane *p_self);
pandemonium_bool GDAPI pandemonium_plane_is_equal_approx(const pandemonium_plane *p_self, const pandemonium_plane *p_plane);
pandemonium_bool GDAPI pandemonium_plane_is_equal_approx_any_side(const pandemonium_plane *p_self, const pandemonium_plane *p_plane);

pandemonium_bool GDAPI pandemonium_plane_operator_equal(const pandemonium_plane *p_self, const pandemonium_plane *p_b);

pandemonium_string GDAPI pandemonium_plane_as_string(const pandemonium_plane *p_self);

void GDAPI pandemonium_plane_new_with_reals(pandemonium_plane *r_dest, const pandemonium_real p_a, const pandemonium_real p_b, const pandemonium_real p_c, const pandemonium_real p_d);
void GDAPI pandemonium_plane_new_with_vectors(pandemonium_plane *r_dest, const pandemonium_vector3 *p_v1, const pandemonium_vector3 *p_v2, const pandemonium_vector3 *p_v3);
void GDAPI pandemonium_plane_new_with_normal(pandemonium_plane *r_dest, const pandemonium_vector3 *p_normal, const pandemonium_real p_d);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_PLANE_H
