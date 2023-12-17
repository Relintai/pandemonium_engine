/*************************************************************************/
/*  aabb.cpp                                                             */
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

#include "gdn/aabb.h"

#include "core/math/aabb.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_aabb) == sizeof(AABB), "AABB size mismatch");

pandemonium_real GDAPI pandemonium_aabb_get_volume(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->get_volume();
}

pandemonium_bool GDAPI pandemonium_aabb_has_no_volume(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->has_no_volume();
}

pandemonium_bool GDAPI pandemonium_aabb_has_no_surface(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->has_no_surface();
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_position(const pandemonium_aabb *p_self) {
	pandemonium_vector3 raw_ret;
	const AABB *self = (const AABB *)p_self;
	Vector3 *ret = (Vector3 *)&raw_ret;
	*ret = self->position;
	return raw_ret;
}

void GDAPI pandemonium_aabb_set_position(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_v) {
	AABB *self = (AABB *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	self->position = *v;
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_size(const pandemonium_aabb *p_self) {
	pandemonium_vector3 raw_ret;
	const AABB *self = (const AABB *)p_self;
	Vector3 *ret = (Vector3 *)&raw_ret;
	*ret = self->size;
	return raw_ret;
}

void GDAPI pandemonium_aabb_set_size(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_v) {
	AABB *self = (AABB *)p_self;
	const Vector3 *v = (const Vector3 *)p_v;
	self->size = *v;
}

pandemonium_bool GDAPI pandemonium_aabb_operator_equal(const pandemonium_aabb *p_self, const pandemonium_aabb *p_b) {
	const AABB *self = (const AABB *)p_self;
	const AABB *b = (const AABB *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_aabb_is_equal_approx(const pandemonium_aabb *p_self, const pandemonium_aabb *p_aabb) {
	const AABB *self = (const AABB *)p_self;
	const AABB *aabb = (const AABB *)p_aabb;
	return self->is_equal_approx(*aabb);
}

pandemonium_bool GDAPI pandemonium_aabb_intersects(const pandemonium_aabb *p_self, const pandemonium_aabb *p_with) {
	const AABB *self = (const AABB *)p_self;
	const AABB *with = (const AABB *)p_with;
	return self->intersects(*with);
}

pandemonium_bool GDAPI pandemonium_aabb_intersects_inclusive(const pandemonium_aabb *p_self, const pandemonium_aabb *p_aabb) {
	const AABB *self = (const AABB *)p_self;
	const AABB *aabb = (const AABB *)p_aabb;
	return self->intersects_inclusive(*aabb);
}

pandemonium_bool GDAPI pandemonium_aabb_encloses(const pandemonium_aabb *p_self, const pandemonium_aabb *p_with) {
	const AABB *self = (const AABB *)p_self;
	const AABB *with = (const AABB *)p_with;
	return self->encloses(*with);
}

pandemonium_aabb GDAPI pandemonium_aabb_merge(const pandemonium_aabb *p_self, const pandemonium_aabb *p_with) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;
	const AABB *with = (const AABB *)p_with;
	*((AABB *)&dest) = self->merge(*with);
	return dest;
}

void GDAPI pandemonium_aabb_merge_with(pandemonium_aabb *p_self, const pandemonium_aabb *p_with) {
	AABB *self = (AABB *)p_self;
	const AABB *with = (const AABB *)p_with;
	self->merge_with(*with);
}

pandemonium_aabb GDAPI pandemonium_aabb_intersection(const pandemonium_aabb *p_self, const pandemonium_aabb *p_with) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;
	const AABB *with = (const AABB *)p_with;
	*((AABB *)&dest) = self->intersection(*with);
	return dest;
}

pandemonium_bool GDAPI pandemonium_aabb_intersects_segment(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *to = (const Vector3 *)p_to;
	return self->intersects_segment(*from, *to);
}
pandemonium_bool GDAPI pandemonium_aabb_intersects_segment_clip(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to, const pandemonium_vector3 *r_clip) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *to = (const Vector3 *)p_to;
	Vector3 *clip = (Vector3 *)r_clip;
	return self->intersects_segment(*from, *to, clip);
}
pandemonium_bool GDAPI pandemonium_aabb_intersects_segment_clip_normal(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to, const pandemonium_vector3 *r_clip, const pandemonium_vector3 *r_normal) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *to = (const Vector3 *)p_to;
	Vector3 *clip = (Vector3 *)r_clip;
	Vector3 *normal = (Vector3 *)r_normal;
	return self->intersects_segment(*from, *to, clip, normal);
}

pandemonium_bool GDAPI pandemonium_aabb_intersects_ray(const pandemonium_aabb *p_self, const pandemonium_aabb *p_from, const pandemonium_aabb *p_dir) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	return self->intersects_ray(*from, *dir);
}
pandemonium_bool GDAPI pandemonium_aabb_intersects_ray_clip(const pandemonium_aabb *p_self, const pandemonium_aabb *p_from, const pandemonium_aabb *p_dir, pandemonium_aabb *r_clip) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	Vector3 *clip = (Vector3 *)r_clip;
	return self->intersects_ray(*from, *dir, clip);
}
pandemonium_bool GDAPI pandemonium_aabb_intersects_ray_clip_normal(const pandemonium_aabb *p_self, const pandemonium_aabb *p_from, const pandemonium_aabb *p_dir, pandemonium_aabb *r_clip, pandemonium_aabb *r_normal) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	Vector3 *clip = (Vector3 *)r_clip;
	Vector3 *normal = (Vector3 *)r_normal;
	return self->intersects_ray(*from, *dir, clip, normal);
}
pandemonium_bool GDAPI pandemonium_aabb_intersects_smits_intersect_ray(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_dir, const pandemonium_real t0, const pandemonium_real t1) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	return self->smits_intersect_ray(*from, *dir, t0, t1);
}

pandemonium_bool GDAPI pandemonium_aabb_intersects_plane(const pandemonium_aabb *p_self, const pandemonium_plane *p_plane) {
	const AABB *self = (const AABB *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	return self->intersects_plane(*plane);
}

pandemonium_bool GDAPI pandemonium_aabb_has_point(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_point) {
	const AABB *self = (const AABB *)p_self;
	const Vector3 *point = (const Vector3 *)p_point;
	return self->has_point(*point);
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_support(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_dir) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;
	const Vector3 *dir = (const Vector3 *)p_dir;
	*((Vector3 *)&dest) = self->get_support(*dir);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_longest_axis(const pandemonium_aabb *p_self) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;
	*((Vector3 *)&dest) = self->get_longest_axis();
	return dest;
}

pandemonium_int GDAPI pandemonium_aabb_get_longest_axis_index(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->get_longest_axis_index();
}

pandemonium_real GDAPI pandemonium_aabb_get_longest_axis_size(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->get_longest_axis_size();
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_shortest_axis(const pandemonium_aabb *p_self) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;
	*((Vector3 *)&dest) = self->get_shortest_axis();
	return dest;
}

pandemonium_int GDAPI pandemonium_aabb_get_shortest_axis_index(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->get_shortest_axis_index();
}

pandemonium_real GDAPI pandemonium_aabb_get_shortest_axis_size(const pandemonium_aabb *p_self) {
	const AABB *self = (const AABB *)p_self;
	return self->get_shortest_axis_size();
}

pandemonium_aabb GDAPI pandemonium_aabb_grow(const pandemonium_aabb *p_self, const pandemonium_real p_by) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;

	*((AABB *)&dest) = self->grow(p_by);
	return dest;
}

void GDAPI pandemonium_aabb_grow_by(pandemonium_aabb *p_self, const pandemonium_real p_amount) {
	AABB *self = (AABB *)p_self;

	self->grow_by(p_amount);
}

void GDAPI pandemonium_aabb_get_edge(const pandemonium_aabb *p_self, const pandemonium_int p_edge, pandemonium_vector3 *r_from, pandemonium_vector3 *r_to) {
	const AABB *self = (const AABB *)p_self;
	Vector3 *from = (Vector3 *)r_from;
	Vector3 *to = (Vector3 *)r_to;
	self->get_edge(p_edge, *from, *to);
}

pandemonium_vector3 GDAPI pandemonium_aabb_get_endpoint(const pandemonium_aabb *p_self, const pandemonium_int p_idx) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;

	*((Vector3 *)&dest) = self->get_endpoint(p_idx);
	return dest;
}

pandemonium_aabb GDAPI pandemonium_aabb_expand(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_to_point) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;
	const Vector3 *to_point = (const Vector3 *)p_to_point;
	*((AABB *)&dest) = self->expand(*to_point);
	return dest;
}

void GDAPI pandemonium_aabb_project_range_in_plane(const pandemonium_aabb *p_self, const pandemonium_plane *p_plane, pandemonium_real *r_min, pandemonium_real *r_max) {
	const AABB *self = (const AABB *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	self->project_range_in_plane(*plane, *r_min, *r_max);
}

void GDAPI pandemonium_aabb_expand_to(pandemonium_aabb *p_self, const pandemonium_vector3 *p_vector) {
	AABB *self = (AABB *)p_self;
	const Vector3 *vector = (const Vector3 *)p_vector;
	self->expand_to(*vector);
}

pandemonium_aabb GDAPI pandemonium_aabb_abs(const pandemonium_aabb *p_self) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;
	*((AABB *)&dest) = self->abs();
	return dest;
}

pandemonium_variant GDAPI pandemonium_aabb_intersects_segmentv(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_to) {
	pandemonium_variant dest;
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *to = (const Vector3 *)p_to;
	*((Variant *)&dest) = self->intersects_segmentv(*from, *to);
	return dest;
}
pandemonium_variant GDAPI pandemonium_aabb_intersects_rayv(const pandemonium_aabb *p_self, const pandemonium_vector3 *p_from, const pandemonium_vector3 *p_dir) {
	pandemonium_variant dest;
	const AABB *self = (const AABB *)p_self;
	const Vector3 *from = (const Vector3 *)p_from;
	const Vector3 *dir = (const Vector3 *)p_dir;
	*((Variant *)&dest) = self->intersects_rayv(*from, *dir);
	return dest;
}

void GDAPI pandemonium_aabb_quantize(pandemonium_aabb *p_self, const pandemonium_real p_unit) {
	AABB *self = (AABB *)p_self;
	self->quantize(p_unit);
}

pandemonium_aabb GDAPI pandemonium_aabb_quantized(const pandemonium_aabb *p_self, const pandemonium_real p_unit) {
	pandemonium_aabb dest;
	const AABB *self = (const AABB *)p_self;
	*((AABB *)&dest) = self->quantized(p_unit);
	return dest;
}

void GDAPI pandemonium_aabb_set_end(pandemonium_aabb *p_self, const pandemonium_vector3 *p_end) {
	AABB *self = (AABB *)p_self;
	const Vector3 *end = (const Vector3 *)p_end;
	self->set_end(*end);
}
pandemonium_vector3 GDAPI pandemonium_aabb_get_end(const pandemonium_aabb *p_self) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;
	*((Vector3 *)&dest) = self->get_end();
	return dest;
}
pandemonium_vector3 GDAPI pandemonium_aabb_get_center(const pandemonium_aabb *p_self) {
	pandemonium_vector3 dest;
	const AABB *self = (const AABB *)p_self;
	*((Vector3 *)&dest) = self->get_center();
	return dest;
}

pandemonium_string GDAPI pandemonium_aabb_as_string(const pandemonium_aabb *p_self) {
	pandemonium_string ret;
	const AABB *self = (const AABB *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_aabb_new(pandemonium_aabb *r_dest, const pandemonium_vector3 *p_pos, const pandemonium_vector3 *p_size) {
	const Vector3 *pos = (const Vector3 *)p_pos;
	const Vector3 *size = (const Vector3 *)p_size;
	AABB *dest = (AABB *)r_dest;
	*dest = AABB(*pos, *size);
}

#ifdef __cplusplus
}
#endif
