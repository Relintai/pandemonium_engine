#ifndef PROJECTION_H
#define PROJECTION_H

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

#include "core/containers/vector.h"
#include "core/math/math_defs.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"

class Array;
struct AABB;
struct Plane;
struct Rect2;
struct Transform;
struct Vector2;

struct _NO_DISCARD_CLASS_ Projection {
	enum Planes {
		PLANE_NEAR,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_TOP,
		PLANE_RIGHT,
		PLANE_BOTTOM
	};

	Vector4 matrix[4];

	_FORCE_INLINE_ const Vector4 &operator[](const int p_axis) const {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return matrix[p_axis];
	}

	_FORCE_INLINE_ Vector4 &operator[](const int p_axis) {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return matrix[p_axis];
	}

	float determinant() const;
	void set_identity();
	void set_zero();
	void set_light_bias();
	void set_depth_correction(bool p_flip_y = true);

	void set_light_atlas_rect(const Rect2 &p_rect);
	void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov = false);
	void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov, int p_eye, real_t p_intraocular_dist, real_t p_convergence_dist);
	void set_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist, real_t p_display_width, real_t p_display_to_lens, real_t p_oversample, real_t p_z_near, real_t p_z_far);
	void set_orthogonal(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_znear, real_t p_zfar);
	void set_orthogonal(real_t p_size, real_t p_aspect, real_t p_znear, real_t p_zfar, bool p_flip_fov = false);
	void set_frustum(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_near, real_t p_far);
	void set_frustum(real_t p_size, real_t p_aspect, Vector2 p_offset, real_t p_near, real_t p_far, bool p_flip_fov = false);
	void adjust_perspective_znear(real_t p_new_znear);

	static Projection create_depth_correction(bool p_flip_y);
	static Projection create_light_atlas_rect(const Rect2 &p_rect);
	static Projection create_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov = false);
	static Projection create_perspective_hmd(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov, int p_eye, real_t p_intraocular_dist, real_t p_convergence_dist);
	static Projection create_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist, real_t p_display_width, real_t p_display_to_lens, real_t p_oversample, real_t p_z_near, real_t p_z_far);
	static Projection create_orthogonal(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_znear, real_t p_zfar);
	static Projection create_orthogonal_aspect(real_t p_size, real_t p_aspect, real_t p_znear, real_t p_zfar, bool p_flip_fov = false);
	static Projection create_frustum(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_near, real_t p_far);
	static Projection create_frustum_aspect(real_t p_size, real_t p_aspect, Vector2 p_offset, real_t p_near, real_t p_far, bool p_flip_fov = false);
	static Projection create_fit_aabb(const AABB &p_aabb);

	Projection perspective_znear_adjusted(real_t p_new_znear) const;
	Plane get_projection_plane(Planes p_plane) const;
	Projection flipped_y() const;
	Projection jitter_offseted(const Vector2 &p_offset) const;

	static real_t get_fovy(real_t p_fovx, real_t p_aspect) {
		return Math::rad2deg(Math::atan(p_aspect * Math::tan(Math::deg2rad(p_fovx) * 0.5)) * 2.0);
	}

	real_t calculate_fovy(real_t p_fovx, real_t p_aspect) {
		return Math::rad2deg(Math::atan(p_aspect * Math::tan(Math::deg2rad(p_fovx) * 0.5)) * 2.0);
	}

	real_t get_z_far() const;
	real_t get_z_near() const;
	real_t get_aspect() const;
	real_t get_fov() const;
	bool is_orthogonal() const;

	Vector<Plane> get_projection_planes(const Transform &p_transform) const;
	Array get_projection_planes_array(const Transform &p_transform) const;

	// Returns efficiently all info needed for culling AABBs via AABB::intersects_convex_shape().
	bool get_projection_planes_and_endpoints(const Transform &p_transform, Plane *p_6planes, Vector3 *p_8points) const;

	bool get_endpoints(const Transform &p_transform, Vector3 *p_8points) const;
	Vector2 get_viewport_half_extents() const;
	Vector2 get_far_plane_half_extents() const;

	void invert();
	Projection inverse() const;

	Projection operator*(const Projection &p_matrix) const;

	Vector4 xform(const Vector4 &p_vec4) const;
	Vector4 xform_inv(const Vector4 &p_vec4) const;

	_FORCE_INLINE_ Vector3 xform(const Vector3 &p_vector) const;
	Plane xform(const Plane &p_plane) const;

	operator String() const;

	void scale_translate_to_fit(const AABB &p_aabb);
	void add_jitter_offset(const Vector2 &p_offset);
	void make_scale(const Vector3 &p_scale);
	int get_pixels_per_meter(int p_for_pixel_width) const;
	operator Transform() const;

	void flip_y();

	bool operator==(const Projection &p_cam) const {
		for (uint32_t i = 0; i < 4; i++) {
			for (uint32_t j = 0; j < 4; j++) {
				if (matrix[i][j] != p_cam.matrix[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator!=(const Projection &p_cam) const {
		return !(*this == p_cam);
	}

	float get_lod_multiplier() const;

	_FORCE_INLINE_ void set_perspective1(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov = false) {
		set_perspective(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov);
	}
	_FORCE_INLINE_ void set_perspective2(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov, int p_eye, real_t p_intraocular_dist, real_t p_convergence_dist) {
		set_perspective(p_fovy_degrees, p_aspect, p_z_near, p_z_far, p_flip_fov, p_eye, p_intraocular_dist, p_convergence_dist);
	}
	_FORCE_INLINE_ void set_orthogonal1(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_znear, real_t p_zfar) {
		set_orthogonal(p_left, p_right, p_bottom, p_top, p_znear, p_zfar);
	}
	_FORCE_INLINE_ void set_orthogonal2(real_t p_size, real_t p_aspect, real_t p_znear, real_t p_zfar, bool p_flip_fov = false) {
		set_orthogonal(p_size, p_aspect, p_znear, p_zfar, p_flip_fov);
	}
	_FORCE_INLINE_ void set_frustum1(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_near, real_t p_far) {
		set_frustum(p_left, p_right, p_bottom, p_top, p_near, p_far);
	}
	//Vector2 is incomplete here
	void set_frustum2(real_t p_size, real_t p_aspect, Vector2 p_offset, real_t p_near, real_t p_far, bool p_flip_fov = false);

	Projection();
	Projection(const Vector4 &p_x, const Vector4 &p_y, const Vector4 &p_z, const Vector4 &p_w);
	Projection(const Transform &p_transform);
	~Projection();

private:
	void _get_projection_planes(Plane *p_6planes) const;
	void _transform_projection_planes(const Transform &p_transform, Plane *p_6planes) const;
};

Vector3 Projection::xform(const Vector3 &p_vec3) const {
	Vector3 ret;
	ret.x = matrix[0][0] * p_vec3.x + matrix[1][0] * p_vec3.y + matrix[2][0] * p_vec3.z + matrix[3][0];
	ret.y = matrix[0][1] * p_vec3.x + matrix[1][1] * p_vec3.y + matrix[2][1] * p_vec3.z + matrix[3][1];
	ret.z = matrix[0][2] * p_vec3.x + matrix[1][2] * p_vec3.y + matrix[2][2] * p_vec3.z + matrix[3][2];
	real_t w = matrix[0][3] * p_vec3.x + matrix[1][3] * p_vec3.y + matrix[2][3] * p_vec3.z + matrix[3][3];
	return ret / w;
}

#endif // PROJECTION_H
