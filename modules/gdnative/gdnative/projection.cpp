/*************************************************************************/
/*  projection.cpp                                                       */
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

#include "gdn/projection.h"

#include "core/math/plane.h"
#include "core/math/projection.h"
#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/string/ustring.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_projection) == sizeof(Projection), "Projection size mismatch");

pandemonium_vector4 GDAPI *pandemonium_projection_operator_index(pandemonium_projection *p_self, const pandemonium_int p_axis) {
	Projection *self = (Projection *)p_self;
	return (pandemonium_vector4 *)&self->operator[](p_axis);
}
const pandemonium_vector4 GDAPI *pandemonium_projection_operator_index_const(const pandemonium_projection *p_self, const pandemonium_int p_axis) {
	const Projection *self = (const Projection *)p_self;
	return (const pandemonium_vector4 *)&self->operator[](p_axis);
}

pandemonium_real GDAPI pandemonium_projection_determinant(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->determinant();
}
void GDAPI pandemonium_projection_set_identity(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->set_identity();
}
void GDAPI pandemonium_projection_set_zero(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->set_zero();
}
void GDAPI pandemonium_projection_set_light_bias(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->set_light_bias();
}
void GDAPI pandemonium_projection_set_depth_correction(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->set_depth_correction();
}
void GDAPI pandemonium_projection_set_depth_correction_flip(pandemonium_projection *p_self, pandemonium_bool p_flip_y) {
	Projection *self = (Projection *)p_self;
	self->set_depth_correction(p_flip_y);
}

void GDAPI pandemonium_projection_set_light_atlas_rect(pandemonium_projection *p_self, const pandemonium_rect2 *p_rect) {
	Projection *self = (Projection *)p_self;
	const Rect2 *rect = (const Rect2 *)p_rect;
	self->set_light_atlas_rect(*rect);
}
void GDAPI pandemonium_projection_set_perspective1(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far) {
	Projection *self = (Projection *)p_self;
	self->set_perspective1(p_fovy_degrees, p_aspect, p_z_near, p_z_far);
}
void GDAPI pandemonium_projection_set_perspective1_flip_fov(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov) {
	Projection *self = (Projection *)p_self;
	self->set_perspective1(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov);
}
void GDAPI pandemonium_projection_set_perspective2(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov, pandemonium_int p_eye, pandemonium_real p_intraocular_dist, pandemonium_real p_convergence_dist) {
	Projection *self = (Projection *)p_self;
	self->set_perspective2(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov, p_eye, p_intraocular_dist, p_convergence_dist);
}
void GDAPI pandemonium_projection_set_for_hmd(pandemonium_projection *p_self, pandemonium_int p_eye, pandemonium_real p_aspect, pandemonium_real p_intraocular_dist, pandemonium_real p_display_width, pandemonium_real p_display_to_lens, pandemonium_real p_oversample, pandemonium_real p_z_near, pandemonium_real p_z_far) {
	Projection *self = (Projection *)p_self;
	self->set_for_hmd(p_eye, p_aspect, p_intraocular_dist, p_display_width, p_display_to_lens, p_oversample, p_z_near, p_z_far);
}
void GDAPI pandemonium_projection_set_orthogonal1(pandemonium_projection *p_self, pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_znear, pandemonium_real p_zfar) {
	Projection *self = (Projection *)p_self;
	self->set_orthogonal1(p_left, p_right, p_bottom, p_top, p_znear, p_zfar);
}
void GDAPI pandemonium_projection_set_orthogonal2(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar) {
	Projection *self = (Projection *)p_self;
	self->set_orthogonal2(p_size, p_aspect, p_znear, p_zfar);
}
void GDAPI pandemonium_projection_set_orthogonal2_flip_fov(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar, pandemonium_bool p_flip_fov) {
	Projection *self = (Projection *)p_self;
	self->set_orthogonal2(p_size, p_aspect, p_znear, p_zfar, p_flip_fov);
}
void GDAPI pandemonium_projection_set_frustum1(pandemonium_projection *p_self, pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_near, pandemonium_real p_far) {
	Projection *self = (Projection *)p_self;
	self->set_frustum1(p_left, p_right, p_bottom, p_top, p_near, p_far);
}
void GDAPI pandemonium_projection_set_frustum2(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far) {
	Projection *self = (Projection *)p_self;
	Vector2 *offset = (Vector2 *)p_offset;
	self->set_frustum2(p_size, p_aspect, *offset, p_near, p_far);
}
void GDAPI pandemonium_projection_set_frustum2_flip_fov(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far, pandemonium_bool p_flip_fov) {
	Projection *self = (Projection *)p_self;
	Vector2 *offset = (Vector2 *)p_offset;
	self->set_frustum2(p_size, p_aspect, *offset, p_near, p_far, p_flip_fov);
}
void GDAPI pandemonium_projection_adjust_perspective_znear(pandemonium_projection *p_self, pandemonium_real p_new_znear) {
	Projection *self = (Projection *)p_self;
	self->adjust_perspective_znear(p_new_znear);
}

pandemonium_projection GDAPI pandemonium_projection_create_depth_correction(pandemonium_bool p_flip_y) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_depth_correction(p_flip_y);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_light_atlas_rect(const pandemonium_rect2 *p_rect) {
	pandemonium_projection dest;
	const Rect2 *rect = (const Rect2 *)p_rect;
	*((Projection *)&dest) = Projection::create_light_atlas_rect(*rect);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_perspective(pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_perspective(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_perspective_hmd(pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov, pandemonium_int p_eye, pandemonium_real p_intraocular_dist, pandemonium_real p_convergence_dist) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_perspective_hmd(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov, p_eye, p_intraocular_dist, p_convergence_dist);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_for_hmd(pandemonium_int p_eye, pandemonium_real p_aspect, pandemonium_real p_intraocular_dist, pandemonium_real p_display_width, pandemonium_real p_display_to_lens, pandemonium_real p_oversample, pandemonium_real p_z_near, pandemonium_real p_z_far) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_for_hmd(p_eye, p_aspect, p_intraocular_dist, p_display_width, p_display_to_lens, p_oversample, p_z_near, p_z_far);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_orthogonal(pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_znear, pandemonium_real p_zfar) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_orthogonal(p_left, p_right, p_bottom, p_top, p_znear, p_zfar);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_orthogonal_aspect(pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar, pandemonium_bool p_flip_fov) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_orthogonal_aspect(p_size, p_aspect, p_znear, p_zfar, p_flip_fov);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_frustum(pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_near, pandemonium_real p_far) {
	pandemonium_projection dest;
	*((Projection *)&dest) = Projection::create_frustum(p_left, p_right, p_bottom, p_top, p_near, p_far);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_frustum_aspect(pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far, pandemonium_bool p_flip_fov) {
	pandemonium_projection dest;
	Vector2 *offset = (Vector2 *)p_offset;
	*((Projection *)&dest) = Projection::create_frustum_aspect(p_size, p_aspect, *offset, p_near, p_far, p_flip_fov);
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_create_fit_aabb(const pandemonium_aabb *p_aabb) {
	pandemonium_projection dest;
	const AABB *aabb = (const AABB *)p_aabb;
	*((Projection *)&dest) = Projection::create_fit_aabb(*aabb);
	return dest;
}

pandemonium_projection GDAPI pandemonium_projection_perspective_znear_adjusted(const pandemonium_projection *p_self, pandemonium_real p_new_znear) {
	pandemonium_projection dest;
	const Projection *self = (const Projection *)p_self;
	*((Projection *)&dest) = self->perspective_znear_adjusted(p_new_znear);
	return dest;
}
pandemonium_plane GDAPI pandemonium_projection_get_projection_plane(const pandemonium_projection *p_self, pandemonium_projection_planes p_plane) {
	pandemonium_plane dest;
	const Projection *self = (const Projection *)p_self;
	*((Plane *)&dest) = self->get_projection_plane(static_cast<Projection::Planes>(p_plane));
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_flipped_y(const pandemonium_projection *p_self) {
	pandemonium_projection dest;
	const Projection *self = (const Projection *)p_self;
	*((Projection *)&dest) = self->flipped_y();
	return dest;
}
pandemonium_projection GDAPI pandemonium_projection_jitter_offseted(const pandemonium_projection *p_self, const pandemonium_vector2 *p_offset) {
	pandemonium_projection dest;
	const Projection *self = (const Projection *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	*((Projection *)&dest) = self->jitter_offseted(*offset);
	return dest;
}

pandemonium_real GDAPI pandemonium_projection_get_fovy(pandemonium_real p_fovx, pandemonium_real p_aspect) {
	return Projection::get_fovy(p_fovx, p_aspect);
}

pandemonium_real GDAPI pandemonium_projection_calculate_fovy(pandemonium_projection *p_self, pandemonium_real p_fovx, pandemonium_real p_aspect) {
	Projection *self = (Projection *)p_self;
	return self->calculate_fovy(p_fovx, p_aspect);
}

pandemonium_real GDAPI pandemonium_projection_get_z_far(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->get_z_far();
}
pandemonium_real GDAPI pandemonium_projection_get_z_near(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->get_z_near();
}
pandemonium_real GDAPI pandemonium_projection_get_aspect(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->get_aspect();
}
pandemonium_real GDAPI pandemonium_projection_get_fov(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->get_fov();
}
pandemonium_bool GDAPI pandemonium_projection_is_orthogonal(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->is_orthogonal();
}

pandemonium_array GDAPI pandemonium_projection_get_projection_planes_array(const pandemonium_projection *p_self, const pandemonium_projection *p_projection) {
	pandemonium_array dest;
	const Projection *self = (const Projection *)p_self;
	const Projection *projection = (const Projection *)p_projection;
	*((Array *)&dest) = self->get_projection_planes_array(*projection);
	return dest;
}

pandemonium_bool GDAPI pandemonium_projection_get_endpoints(const pandemonium_projection *p_self, const pandemonium_projection *p_projection, pandemonium_vector3 *p_8points) {
	const Projection *self = (const Projection *)p_self;
	const Projection *projection = (const Projection *)p_projection;
	Vector3 *points = (Vector3 *)p_8points;
	return self->get_endpoints(*projection, points);
}
pandemonium_vector2 GDAPI pandemonium_projection_get_viewport_half_extents(const pandemonium_projection *p_self) {
	pandemonium_vector2 dest;
	const Projection *self = (const Projection *)p_self;
	*((Vector2 *)&dest) = self->get_viewport_half_extents();
	return dest;
}
pandemonium_vector2 GDAPI pandemonium_projection_get_far_plane_half_extents(const pandemonium_projection *p_self) {
	pandemonium_vector2 dest;
	const Projection *self = (const Projection *)p_self;
	*((Vector2 *)&dest) = self->get_far_plane_half_extents();
	return dest;
}

void GDAPI pandemonium_projection_invert(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->invert();
}
pandemonium_projection GDAPI pandemonium_projection_inverse(const pandemonium_projection *p_self) {
	pandemonium_projection dest;
	const Projection *self = (const Projection *)p_self;
	*((Projection *)&dest) = self->inverse();
	return dest;
}

pandemonium_projection GDAPI pandemonium_projection_operator_mul(const pandemonium_projection *p_self, const pandemonium_projection *p_matrix) {
	pandemonium_projection dest;
	const Projection *self = (const Projection *)p_self;
	const Projection *matrix = (const Projection *)p_matrix;
	*((Projection *)&dest) = self->operator*(*matrix);
	return dest;
}

pandemonium_vector4 GDAPI pandemonium_projection_xform_vector4(const pandemonium_projection *p_self, const pandemonium_vector4 *p_vec4) {
	pandemonium_vector4 dest;
	const Projection *self = (const Projection *)p_self;
	const Vector4 *vec4 = (const Vector4 *)p_vec4;
	*((Vector4 *)&dest) = self->xform(*vec4);
	return dest;
}
pandemonium_vector4 GDAPI pandemonium_projection_xform_inv_vector4(const pandemonium_projection *p_self, const pandemonium_vector4 *p_vec4) {
	pandemonium_vector4 dest;
	const Projection *self = (const Projection *)p_self;
	const Vector4 *vec4 = (const Vector4 *)p_vec4;
	*((Vector4 *)&dest) = self->xform_inv(*vec4);
	return dest;
}

pandemonium_vector3 GDAPI pandemonium_projection_xform_vector3(const pandemonium_projection *p_self, const pandemonium_vector3 *p_vector) {
	pandemonium_vector3 dest;
	const Projection *self = (const Projection *)p_self;
	const Vector3 *vector = (const Vector3 *)p_vector;
	*((Vector3 *)&dest) = self->xform(*vector);
	return dest;
}
pandemonium_plane GDAPI pandemonium_projection_xform_plane(const pandemonium_projection *p_self, const pandemonium_plane *p_plane) {
	pandemonium_plane dest;
	const Projection *self = (const Projection *)p_self;
	const Plane *plane = (const Plane *)p_plane;
	*((Plane *)&dest) = self->xform(*plane);
	return dest;
}

pandemonium_string GDAPI pandemonium_projection_as_string(const pandemonium_projection *p_self) {
	pandemonium_string ret;
	const Projection *self = (const Projection *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

void GDAPI pandemonium_projection_scale_translate_to_fit(pandemonium_projection *p_self, const pandemonium_aabb *p_aabb) {
	Projection *self = (Projection *)p_self;
	const AABB *aabb = (const AABB *)p_aabb;
	self->scale_translate_to_fit(*aabb);
}
void GDAPI pandemonium_projection_add_jitter_offset(pandemonium_projection *p_self, const pandemonium_vector2 *p_offset) {
	Projection *self = (Projection *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	self->add_jitter_offset(*offset);
}
void GDAPI pandemonium_projection_make_scale(pandemonium_projection *p_self, const pandemonium_vector3 *p_scale) {
	Projection *self = (Projection *)p_self;
	const Vector3 *scale = (const Vector3 *)p_scale;
	self->make_scale(*scale);
}
pandemonium_int GDAPI pandemonium_projection_get_pixels_per_meter(const pandemonium_projection *p_self, pandemonium_int p_for_pixel_width) {
	const Projection *self = (const Projection *)p_self;
	return self->get_pixels_per_meter(p_for_pixel_width);
}

pandemonium_transform GDAPI pandemonium_projection_to_transform(const pandemonium_projection *p_self) {
	pandemonium_transform dest;
	const Projection *self = (const Projection *)p_self;
	*((Transform *)&dest) = self->operator Transform();
	return dest;
}

void GDAPI pandemonium_projection_flip_y(pandemonium_projection *p_self) {
	Projection *self = (Projection *)p_self;
	self->flip_y();
}

pandemonium_bool GDAPI pandemonium_projection_operator_equal(const pandemonium_projection *p_self, const pandemonium_projection *p_cam) {
	const Projection *self = (const Projection *)p_self;
	const Projection *cam = (const Projection *)p_cam;
	return self->operator==(*cam);
}

pandemonium_real GDAPI pandemonium_projection_get_lod_multiplier(const pandemonium_projection *p_self) {
	const Projection *self = (const Projection *)p_self;
	return self->get_lod_multiplier();
}

void GDAPI pandemonium_projection_new_identity(pandemonium_projection *r_dest) {
	Projection *dest = (Projection *)r_dest;
	*dest = Projection();
}

void GDAPI pandemonium_projection_new_vector4s(pandemonium_projection *r_dest, const pandemonium_vector4 *p_x, const pandemonium_vector4 *p_y, const pandemonium_vector4 *p_z, const pandemonium_vector4 *p_w) {
	const Vector4 *x = (const Vector4 *)p_x;
	const Vector4 *y = (const Vector4 *)p_y;
	const Vector4 *z = (const Vector4 *)p_z;
	const Vector4 *w = (const Vector4 *)p_w;
	Projection *dest = (Projection *)r_dest;
	*dest = Projection(*x, *y, *z, *w);
}
void GDAPI pandemonium_projection_new_from_projection(pandemonium_projection *r_dest, const pandemonium_projection *p_projection) {
	const Projection *projection = (const Projection *)p_projection;
	Projection *dest = (Projection *)r_dest;
	*dest = Projection(*projection);
}

#ifdef __cplusplus
}
#endif
