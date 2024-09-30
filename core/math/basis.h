#ifndef BASIS_H
#define BASIS_H

/*************************************************************************/
/*  basis.h                                                              */
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

#include "core/math/quaternion.h"
#include "core/math/vector3.h"
#include "core/math/vector3i.h"

struct _NO_DISCARD_CLASS_ Basis {
	Vector3 rows[3] = {
		Vector3(1, 0, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};

	_FORCE_INLINE_ const Vector3 &operator[](int p_row) const {
		return rows[p_row];
	}
	_FORCE_INLINE_ Vector3 &operator[](int p_row) {
		return rows[p_row];
	}

	void invert();
	void transpose();

	Basis inverse() const;
	Basis transposed() const;

	_FORCE_INLINE_ real_t determinant() const;

	void from_z(const Vector3 &p_z);

	void rotate(const Vector3 &p_axis, real_t p_phi);
	Basis rotated(const Vector3 &p_axis, real_t p_phi) const;

	void rotate_local(const Vector3 &p_axis, real_t p_phi);
	Basis rotated_local(const Vector3 &p_axis, real_t p_phi) const;

	void rotate(const Vector3 &p_euler);
	Basis rotated(const Vector3 &p_euler) const;

	void rotate(const Quaternion &p_quat);
	Basis rotated(const Quaternion &p_quat) const;

	_FORCE_INLINE_ void rotatev(const Vector3 &p_euler) { rotate(p_euler); }
	_FORCE_INLINE_ Basis rotatedv(const Vector3 &p_euler) const { return rotated(p_euler); }
	_FORCE_INLINE_ void rotateq(const Quaternion &p_quat) { rotate(p_quat); }
	_FORCE_INLINE_ Basis rotatedq(const Quaternion &p_quat) const { return rotated(p_quat); }

	Vector3 get_rotation_euler() const;
	void get_rotation_axis_angle(Vector3 &p_axis, real_t &p_angle) const;
	void get_rotation_axis_angle_local(Vector3 &p_axis, real_t &p_angle) const;
	Quaternion get_rotation_quaternion() const;
	Vector3 get_rotation() const { return get_rotation_euler(); };

	void rotate_to_align(const Vector3 &p_start_direction, const Vector3 &p_end_direction);

	Vector3 rotref_posscale_decomposition(Basis &rotref) const;

	Vector3 get_euler_xyz() const;
	void set_euler_xyz(const Vector3 &p_euler);

	Vector3 get_euler_xzy() const;
	void set_euler_xzy(const Vector3 &p_euler);

	Vector3 get_euler_yzx() const;
	void set_euler_yzx(const Vector3 &p_euler);

	Vector3 get_euler_yxz() const;
	void set_euler_yxz(const Vector3 &p_euler);

	Vector3 get_euler_zxy() const;
	void set_euler_zxy(const Vector3 &p_euler);

	Vector3 get_euler_zyx() const;
	void set_euler_zyx(const Vector3 &p_euler);

	Vector3 get_euler() const { return get_euler_yxz(); }
	void set_euler(const Vector3 &p_euler) { set_euler_yxz(p_euler); }

	Quaternion get_quaternion() const;
	void set_quaternion(const Quaternion &p_quat);

	void get_axis_angle(Vector3 &r_axis, real_t &r_angle) const;
	void set_axis_angle(const Vector3 &p_axis, real_t p_phi);

	void scale(const Vector3 &p_scale);
	Basis scaled(const Vector3 &p_scale) const;

	void scale_local(const Vector3 &p_scale);
	Basis scaled_local(const Vector3 &p_scale) const;

	void scale_orthogonal(const Vector3 &p_scale);
	Basis scaled_orthogonal(const Vector3 &p_scale) const;

	void make_scale_uniform();
	real_t get_uniform_scale() const;

	Vector3 get_scale() const;
	Vector3 get_scale_abs() const;
	Vector3 get_scale_local() const;

	void set_axis_angle_scale(const Vector3 &p_axis, real_t p_phi, const Vector3 &p_scale);
	void set_euler_scale(const Vector3 &p_euler, const Vector3 &p_scale);
	void set_quaternion_scale(const Quaternion &p_quat, const Vector3 &p_scale);

	// transposed dot products
	_FORCE_INLINE_ real_t tdotx(const Vector3 &v) const {
		return rows[0][0] * v[0] + rows[1][0] * v[1] + rows[2][0] * v[2];
	}
	_FORCE_INLINE_ real_t tdoty(const Vector3 &v) const {
		return rows[0][1] * v[0] + rows[1][1] * v[1] + rows[2][1] * v[2];
	}
	_FORCE_INLINE_ real_t tdotz(const Vector3 &v) const {
		return rows[0][2] * v[0] + rows[1][2] * v[1] + rows[2][2] * v[2];
	}

	bool is_equal_approx(const Basis &p_basis) const;
	bool is_equal_approx_ratio(const Basis &a, const Basis &b, real_t p_epsilon = UNIT_EPSILON) const;

	bool operator==(const Basis &p_matrix) const;
	bool operator!=(const Basis &p_matrix) const;

	_FORCE_INLINE_ Vector3 xform(const Vector3 &p_vector) const;
	_FORCE_INLINE_ Vector3 xform_inv(const Vector3 &p_vector) const;

	_FORCE_INLINE_ Vector3i xform(const Vector3i &p_vector) const;
	_FORCE_INLINE_ Vector3i xform_inv(const Vector3i &p_vector) const;

	_FORCE_INLINE_ void operator*=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator*(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator+=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator+(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator-=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator-(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator*=(real_t p_val);
	_FORCE_INLINE_ Basis operator*(real_t p_val) const;

	int get_orthogonal_index() const;
	void set_orthogonal_index(int p_index);

	void set_diagonal(const Vector3 &p_diag);

	bool is_orthogonal() const;
	bool is_diagonal() const;
	bool is_rotation() const;

	Basis slerp(const Basis &p_to, const real_t p_weight) const;
	_FORCE_INLINE_ Basis lerp(const Basis &p_to, const real_t p_weight) const;
	void rotate_sh(real_t *p_values);

	operator String() const;

	/* create / set */

	_FORCE_INLINE_ void set(real_t xx, real_t xy, real_t xz, real_t yx, real_t yy, real_t yz, real_t zx, real_t zy, real_t zz) {
		rows[0][0] = xx;
		rows[0][1] = xy;
		rows[0][2] = xz;
		rows[1][0] = yx;
		rows[1][1] = yy;
		rows[1][2] = yz;
		rows[2][0] = zx;
		rows[2][1] = zy;
		rows[2][2] = zz;
	}
	_FORCE_INLINE_ void set(const Vector3 &p_x, const Vector3 &p_y, const Vector3 &p_z) {
		set_column(0, p_x);
		set_column(1, p_y);
		set_column(2, p_z);
	}

	_FORCE_INLINE_ Vector3 get_column(int i) const {
		return Vector3(rows[0][i], rows[1][i], rows[2][i]);
	}

	_FORCE_INLINE_ void set_column(int p_index, const Vector3 &p_value) {
		// Set actual basis axis column (we store transposed as rows for performance).
		rows[0][p_index] = p_value.x;
		rows[1][p_index] = p_value.y;
		rows[2][p_index] = p_value.z;
	}

	_FORCE_INLINE_ void set_columns(const Vector3 &p_x, const Vector3 &p_y, const Vector3 &p_z) {
		set_column(0, p_x);
		set_column(1, p_y);
		set_column(2, p_z);
	}

	_FORCE_INLINE_ Vector3 get_row(int i) const {
		return Vector3(rows[i][0], rows[i][1], rows[i][2]);
	}
	_FORCE_INLINE_ void set_row(int i, const Vector3 &p_row) {
		rows[i][0] = p_row.x;
		rows[i][1] = p_row.y;
		rows[i][2] = p_row.z;
	}

	_FORCE_INLINE_ Vector3 get_axis(int i) const {
		return Vector3(rows[0][i], rows[1][i], rows[2][i]);
	}

	_FORCE_INLINE_ void set_axis(int p_index, const Vector3 &p_value) {
		// Set actual basis axis column (we store transposed as rows for performance).
		rows[0][p_index] = p_value.x;
		rows[1][p_index] = p_value.y;
		rows[2][p_index] = p_value.z;
	}

	_FORCE_INLINE_ Vector3 get_main_diagonal() const {
		return Vector3(rows[0][0], rows[1][1], rows[2][2]);
	}

	_FORCE_INLINE_ void set_zero() {
		rows[0].zero();
		rows[1].zero();
		rows[2].zero();
	}

	_FORCE_INLINE_ Basis transpose_xform(const Basis &m) const {
		return Basis(
				rows[0].x * m[0].x + rows[1].x * m[1].x + rows[2].x * m[2].x,
				rows[0].x * m[0].y + rows[1].x * m[1].y + rows[2].x * m[2].y,
				rows[0].x * m[0].z + rows[1].x * m[1].z + rows[2].x * m[2].z,
				rows[0].y * m[0].x + rows[1].y * m[1].x + rows[2].y * m[2].x,
				rows[0].y * m[0].y + rows[1].y * m[1].y + rows[2].y * m[2].y,
				rows[0].y * m[0].z + rows[1].y * m[1].z + rows[2].y * m[2].z,
				rows[0].z * m[0].x + rows[1].z * m[1].x + rows[2].z * m[2].x,
				rows[0].z * m[0].y + rows[1].z * m[1].y + rows[2].z * m[2].y,
				rows[0].z * m[0].z + rows[1].z * m[1].z + rows[2].z * m[2].z);
	}
	Basis(real_t xx, real_t xy, real_t xz, real_t yx, real_t yy, real_t yz, real_t zx, real_t zy, real_t zz) {
		set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	}

	void orthonormalize();
	Basis orthonormalized() const;

	void orthogonalize();
	Basis orthogonalized() const;

	bool is_symmetric() const;
	Basis diagonalize();

	// The following normal xform functions are correct for non-uniform scales.
	// Use these two functions in combination to xform a series of normals.
	// First use get_normal_xform_basis() to precalculate the inverse transpose.
	// Then apply xform_normal_fast() multiple times using the inverse transpose basis.
	Basis get_normal_xform_basis() const { return inverse().transposed(); }

	// N.B. This only does a normal transform if the basis used is the inverse transpose!
	// Otherwise use xform_normal().
	Vector3 xform_normal_fast(const Vector3 &p_vector) const { return xform(p_vector).normalized(); }

	// This function does the above but for a single normal vector. It is considerably slower, so should usually
	// only be used in cases of single normals, or when the basis changes each time.
	Vector3 xform_normal(const Vector3 &p_vector) const { return get_normal_xform_basis().xform_normal_fast(p_vector); }

	static Basis create_looking_at(const Vector3 &p_target, const Vector3 &p_up = Vector3(0, 1, 0), bool p_use_model_front = false);
	Basis looking_at(const Vector3 &p_target, const Vector3 &p_up = Vector3(0, 1, 0), bool p_use_model_front = false);
	void set_look_at(const Vector3 &p_target, const Vector3 &p_up = Vector3(0, 1, 0), bool p_use_model_front = false);

	static Basis create_from_scale(const Vector3 &p_scale);
	Basis from_scale(const Vector3 &p_scale);

	operator Quaternion() const { return get_quaternion(); }

	Basis(const Quaternion &p_quat) { set_quaternion(p_quat); }
	Basis(const Quaternion &p_quat, const Vector3 &p_scale) { set_quaternion_scale(p_quat, p_scale); }

	Basis(const Vector3 &p_euler) { set_euler(p_euler); }
	Basis(const Vector3 &p_euler, const Vector3 &p_scale) { set_euler_scale(p_euler, p_scale); }

	Basis(const Vector3 &p_axis, real_t p_phi) { set_axis_angle(p_axis, p_phi); }
	Basis(const Vector3 &p_axis, real_t p_phi, const Vector3 &p_scale) { set_axis_angle_scale(p_axis, p_phi, p_scale); }

	_FORCE_INLINE_ Basis(const Vector3 &row0, const Vector3 &row1, const Vector3 &row2) {
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
	}

	_FORCE_INLINE_ Basis() {}
};

_FORCE_INLINE_ void Basis::operator*=(const Basis &p_matrix) {
	set(
			p_matrix.tdotx(rows[0]), p_matrix.tdoty(rows[0]), p_matrix.tdotz(rows[0]),
			p_matrix.tdotx(rows[1]), p_matrix.tdoty(rows[1]), p_matrix.tdotz(rows[1]),
			p_matrix.tdotx(rows[2]), p_matrix.tdoty(rows[2]), p_matrix.tdotz(rows[2]));
}

_FORCE_INLINE_ Basis Basis::operator*(const Basis &p_matrix) const {
	return Basis(
			p_matrix.tdotx(rows[0]), p_matrix.tdoty(rows[0]), p_matrix.tdotz(rows[0]),
			p_matrix.tdotx(rows[1]), p_matrix.tdoty(rows[1]), p_matrix.tdotz(rows[1]),
			p_matrix.tdotx(rows[2]), p_matrix.tdoty(rows[2]), p_matrix.tdotz(rows[2]));
}

_FORCE_INLINE_ void Basis::operator+=(const Basis &p_matrix) {
	rows[0] += p_matrix.rows[0];
	rows[1] += p_matrix.rows[1];
	rows[2] += p_matrix.rows[2];
}

_FORCE_INLINE_ Basis Basis::operator+(const Basis &p_matrix) const {
	Basis ret(*this);
	ret += p_matrix;
	return ret;
}

_FORCE_INLINE_ void Basis::operator-=(const Basis &p_matrix) {
	rows[0] -= p_matrix.rows[0];
	rows[1] -= p_matrix.rows[1];
	rows[2] -= p_matrix.rows[2];
}

_FORCE_INLINE_ Basis Basis::operator-(const Basis &p_matrix) const {
	Basis ret(*this);
	ret -= p_matrix;
	return ret;
}

_FORCE_INLINE_ void Basis::operator*=(real_t p_val) {
	rows[0] *= p_val;
	rows[1] *= p_val;
	rows[2] *= p_val;
}

_FORCE_INLINE_ Basis Basis::operator*(real_t p_val) const {
	Basis ret(*this);
	ret *= p_val;
	return ret;
}

Vector3 Basis::xform(const Vector3 &p_vector) const {
	return Vector3(
			rows[0].dot(p_vector),
			rows[1].dot(p_vector),
			rows[2].dot(p_vector));
}

Vector3i Basis::xform_inv(const Vector3i &p_vector) const {
	return Vector3i(
			(rows[0][0] * p_vector.x) + (rows[1][0] * p_vector.y) + (rows[2][0] * p_vector.z),
			(rows[0][1] * p_vector.x) + (rows[1][1] * p_vector.y) + (rows[2][1] * p_vector.z),
			(rows[0][2] * p_vector.x) + (rows[1][2] * p_vector.y) + (rows[2][2] * p_vector.z));
}

Vector3i Basis::xform(const Vector3i &p_vector) const {
	return Vector3i(
			rows[0].dot(p_vector),
			rows[1].dot(p_vector),
			rows[2].dot(p_vector));
}

Vector3 Basis::xform_inv(const Vector3 &p_vector) const {
	return Vector3(
			(rows[0][0] * p_vector.x) + (rows[1][0] * p_vector.y) + (rows[2][0] * p_vector.z),
			(rows[0][1] * p_vector.x) + (rows[1][1] * p_vector.y) + (rows[2][1] * p_vector.z),
			(rows[0][2] * p_vector.x) + (rows[1][2] * p_vector.y) + (rows[2][2] * p_vector.z));
}

real_t Basis::determinant() const {
	return rows[0][0] * (rows[1][1] * rows[2][2] - rows[2][1] * rows[1][2]) -
			rows[1][0] * (rows[0][1] * rows[2][2] - rows[2][1] * rows[0][2]) +
			rows[2][0] * (rows[0][1] * rows[1][2] - rows[1][1] * rows[0][2]);
}

Basis Basis::lerp(const Basis &p_to, const real_t p_weight) const {
	Basis b;
	b.rows[0] = rows[0].linear_interpolate(p_to.rows[0], p_weight);
	b.rows[1] = rows[1].linear_interpolate(p_to.rows[1], p_weight);
	b.rows[2] = rows[2].linear_interpolate(p_to.rows[2], p_weight);

	return b;
}
#endif // BASIS_H
