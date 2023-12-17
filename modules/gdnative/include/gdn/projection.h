#ifndef GDNATIVE_PROJECTION_H
#define GDNATIVE_PROJECTION_H

/*************************************************************************/
/*  projection.h                                                         */
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

#define PANDEMONIUM_PROJECTION_SIZE 64

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_PROJECTION_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_PROJECTION_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_PROJECTION_SIZE];
} pandemonium_projection;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/plane.h>
#include <gdn/rect2.h>
#include <gdn/transform.h>
#include <gdn/vector2.h>
#include <gdn/vector3.h>
#include <gdn/vector4.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PANDEMONIUM_PROJECTION_PLANE_NEAR,
	PANDEMONIUM_PROJECTION_PLANE_FAR,
	PANDEMONIUM_PROJECTION_PLANE_LEFT,
	PANDEMONIUM_PROJECTION_PLANE_TOP,
	PANDEMONIUM_PROJECTION_PLANE_RIGHT,
	PANDEMONIUM_PROJECTION_PLANE_BOTTOM
} pandemonium_projection_planes;

pandemonium_vector4 GDAPI *pandemonium_projection_operator_index(pandemonium_projection *p_self, const pandemonium_int p_axis);
const pandemonium_vector4 GDAPI *pandemonium_projection_operator_index_const(const pandemonium_projection *p_self, const pandemonium_int p_axis);

pandemonium_real GDAPI pandemonium_projection_determinant(const pandemonium_projection *p_self);
void GDAPI pandemonium_projection_set_identity(pandemonium_projection *p_self);
void GDAPI pandemonium_projection_set_zero(pandemonium_projection *p_self);
void GDAPI pandemonium_projection_set_light_bias(pandemonium_projection *p_self);
void GDAPI pandemonium_projection_set_depth_correction(pandemonium_projection *p_self);
void GDAPI pandemonium_projection_set_depth_correction_flip(pandemonium_projection *p_self, pandemonium_bool p_flip_y);

void GDAPI pandemonium_projection_set_light_atlas_rect(pandemonium_projection *p_self, const pandemonium_rect2 *p_rect);
void GDAPI pandemonium_projection_set_perspective1(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far);
void GDAPI pandemonium_projection_set_perspective1_flip_fov(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov);
void GDAPI pandemonium_projection_set_perspective2(pandemonium_projection *p_self, pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov, pandemonium_int p_eye, pandemonium_real p_intraocular_dist, pandemonium_real p_convergence_dist);
void GDAPI pandemonium_projection_set_for_hmd(pandemonium_projection *p_self, pandemonium_int p_eye, pandemonium_real p_aspect, pandemonium_real p_intraocular_dist, pandemonium_real p_display_width, pandemonium_real p_display_to_lens, pandemonium_real p_oversample, pandemonium_real p_z_near, pandemonium_real p_z_far);
void GDAPI pandemonium_projection_set_orthogonal1(pandemonium_projection *p_self, pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_znear, pandemonium_real p_zfar);
void GDAPI pandemonium_projection_set_orthogonal2(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar);
void GDAPI pandemonium_projection_set_orthogonal2_flip_fov(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar, pandemonium_bool p_flip_fov);
void GDAPI pandemonium_projection_set_frustum1(pandemonium_projection *p_self, pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_near, pandemonium_real p_far);
void GDAPI pandemonium_projection_set_frustum2(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far);
void GDAPI pandemonium_projection_set_frustum2_flip_fov(pandemonium_projection *p_self, pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far, pandemonium_bool p_flip_fov);
void GDAPI pandemonium_projection_adjust_perspective_znear(pandemonium_projection *p_self, pandemonium_real p_new_znear);

pandemonium_projection GDAPI pandemonium_projection_create_depth_correction(pandemonium_bool p_flip_y);
pandemonium_projection GDAPI pandemonium_projection_create_light_atlas_rect(const pandemonium_rect2 *p_rect);
pandemonium_projection GDAPI pandemonium_projection_create_perspective(pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov);
pandemonium_projection GDAPI pandemonium_projection_create_perspective_hmd(pandemonium_real p_fovy_degrees, pandemonium_real p_aspect, pandemonium_real p_z_near, pandemonium_real p_z_far, pandemonium_bool p_flip_fov, pandemonium_int p_eye, pandemonium_real p_intraocular_dist, pandemonium_real p_convergence_dist);
pandemonium_projection GDAPI pandemonium_projection_create_for_hmd(pandemonium_int p_eye, pandemonium_real p_aspect, pandemonium_real p_intraocular_dist, pandemonium_real p_display_width, pandemonium_real p_display_to_lens, pandemonium_real p_oversample, pandemonium_real p_z_near, pandemonium_real p_z_far);
pandemonium_projection GDAPI pandemonium_projection_create_orthogonal(pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_znear, pandemonium_real p_zfar);
pandemonium_projection GDAPI pandemonium_projection_create_orthogonal_aspect(pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_real p_znear, pandemonium_real p_zfar, pandemonium_bool p_flip_fov);
pandemonium_projection GDAPI pandemonium_projection_create_frustum(pandemonium_real p_left, pandemonium_real p_right, pandemonium_real p_bottom, pandemonium_real p_top, pandemonium_real p_near, pandemonium_real p_far);
pandemonium_projection GDAPI pandemonium_projection_create_frustum_aspect(pandemonium_real p_size, pandemonium_real p_aspect, pandemonium_vector2 *p_offset, pandemonium_real p_near, pandemonium_real p_far, pandemonium_bool p_flip_fov);
pandemonium_projection GDAPI pandemonium_projection_create_fit_aabb(const pandemonium_aabb *p_aabb);

pandemonium_projection GDAPI pandemonium_projection_perspective_znear_adjusted(const pandemonium_projection *p_self, pandemonium_real p_new_znear);
pandemonium_plane GDAPI pandemonium_projection_get_projection_plane(const pandemonium_projection *p_self, pandemonium_projection_planes p_plane);
pandemonium_projection GDAPI pandemonium_projection_flipped_y(const pandemonium_projection *p_self);
pandemonium_projection GDAPI pandemonium_projection_jitter_offseted(const pandemonium_projection *p_self, const pandemonium_vector2 *p_offset);

pandemonium_real GDAPI pandemonium_projection_get_fovy(pandemonium_real p_fovx, pandemonium_real p_aspect);

pandemonium_real GDAPI pandemonium_projection_calculate_fovy(pandemonium_projection *p_self, pandemonium_real p_fovx, pandemonium_real p_aspect);

pandemonium_real GDAPI pandemonium_projection_get_z_far(const pandemonium_projection *p_self);
pandemonium_real GDAPI pandemonium_projection_get_z_near(const pandemonium_projection *p_self);
pandemonium_real GDAPI pandemonium_projection_get_aspect(const pandemonium_projection *p_self);
pandemonium_real GDAPI pandemonium_projection_get_fov(const pandemonium_projection *p_self);
pandemonium_bool GDAPI pandemonium_projection_is_orthogonal(const pandemonium_projection *p_self);

//Vector<Plane> get_projection_planes(const Projection &p_projection) const;
pandemonium_array GDAPI pandemonium_projection_get_projection_planes_array(const pandemonium_projection *p_self, const pandemonium_projection *p_projection);

pandemonium_bool GDAPI pandemonium_projection_get_endpoints(const pandemonium_projection *p_self, const pandemonium_projection *p_projection, pandemonium_vector3 *p_8points);
pandemonium_vector2 GDAPI pandemonium_projection_get_viewport_half_extents(const pandemonium_projection *p_self);
pandemonium_vector2 GDAPI pandemonium_projection_get_far_plane_half_extents(const pandemonium_projection *p_self);

void GDAPI pandemonium_projection_invert(pandemonium_projection *p_self);
pandemonium_projection GDAPI pandemonium_projection_inverse(const pandemonium_projection *p_self);

pandemonium_projection GDAPI pandemonium_projection_operator_mul(const pandemonium_projection *p_self, const pandemonium_projection *p_matrix);

pandemonium_vector4 GDAPI pandemonium_projection_xform_vector4(const pandemonium_projection *p_self, const pandemonium_vector4 *p_vec4);
pandemonium_vector4 GDAPI pandemonium_projection_xform_inv_vector4(const pandemonium_projection *p_self, const pandemonium_vector4 *p_vec4);

pandemonium_vector3 GDAPI pandemonium_projection_xform_vector3(const pandemonium_projection *p_self, const pandemonium_vector3 *p_vector);
pandemonium_plane GDAPI pandemonium_projection_xform_plane(const pandemonium_projection *p_self, const pandemonium_plane *p_plane);

pandemonium_string GDAPI pandemonium_projection_as_string(const pandemonium_projection *p_self);

void GDAPI pandemonium_projection_scale_translate_to_fit(pandemonium_projection *p_self, const pandemonium_aabb *p_aabb);
void GDAPI pandemonium_projection_add_jitter_offset(pandemonium_projection *p_self, const pandemonium_vector2 *p_offset);
void GDAPI pandemonium_projection_make_scale(pandemonium_projection *p_self, const pandemonium_vector3 *p_scale);
pandemonium_int GDAPI pandemonium_projection_get_pixels_per_meter(const pandemonium_projection *p_self, pandemonium_int p_for_pixel_width);

pandemonium_transform GDAPI pandemonium_projection_to_transform(const pandemonium_projection *p_self);

void GDAPI pandemonium_projection_flip_y(pandemonium_projection *p_self);

pandemonium_bool GDAPI pandemonium_projection_operator_equal(const pandemonium_projection *p_self, const pandemonium_projection *p_cam);

pandemonium_real GDAPI pandemonium_projection_get_lod_multiplier(const pandemonium_projection *p_self);

void GDAPI pandemonium_projection_new_identity(pandemonium_projection *r_dest);
void GDAPI pandemonium_projection_new_vector4s(pandemonium_projection *r_dest, const pandemonium_vector4 *p_x, const pandemonium_vector4 *p_y, const pandemonium_vector4 *p_z, const pandemonium_vector4 *p_w);
void GDAPI pandemonium_projection_new_from_projection(pandemonium_projection *r_dest, const pandemonium_projection *p_projection);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_PROJECTION_H
