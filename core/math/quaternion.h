#ifndef QUATERNION_H
#define QUATERNION_H

/*************************************************************************/
/*  quaternion.h                                                         */
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

#include "core/math/math_defs.h"
#include "core/math/math_funcs.h"
#include "core/math/vector3.h"
#include "core/string/ustring.h"

struct _NO_DISCARD_CLASS_ Quaternion {
	union {
		struct {
			real_t x;
			real_t y;
			real_t z;
			real_t w;
		};
		real_t components[4];
	};

	_FORCE_INLINE_ real_t &operator[](int idx) {
		return components[idx];
	}
	_FORCE_INLINE_ const real_t &operator[](int idx) const {
		return components[idx];
	}

	_FORCE_INLINE_ real_t length_squared() const;
	bool is_equal_approx(const Quaternion &p_quat) const;
	real_t length() const;
	void normalize();
	Quaternion normalized() const;
	bool is_normalized() const;
	Quaternion inverse() const;
	Quaternion log() const;
	Quaternion exp() const;
	_FORCE_INLINE_ real_t dot(const Quaternion &p_q) const;
	real_t angle_to(const Quaternion &p_to) const;

	void set_euler_xyz(const Vector3 &p_euler);
	Vector3 get_euler_xyz() const;
	void set_euler_yxz(const Vector3 &p_euler);
	Vector3 get_euler_yxz() const;

	void set_euler(const Vector3 &p_euler) { set_euler_yxz(p_euler); };
	Vector3 get_euler() const { return get_euler_yxz(); };

	Quaternion slerp(const Quaternion &p_to, const real_t p_weight) const;
	Quaternion slerpni(const Quaternion &p_to, const real_t p_weight) const;
	Quaternion cubic_slerp(const Quaternion &p_b, const Quaternion &p_pre_a, const Quaternion &p_post_b, const real_t p_weight) const;
	Quaternion spherical_cubic_interpolate(const Quaternion &p_b, const Quaternion &p_pre_a, const Quaternion &p_post_b, const real_t p_weight) const;

	Vector3 get_axis() const;
	float get_angle() const;

	void set_axis_angle(const Vector3 &axis, const real_t angle);
	_FORCE_INLINE_ void get_axis_angle(Vector3 &r_axis, real_t &r_angle) const {
		r_angle = 2 * Math::acos(w);
		real_t r = ((real_t)1) / Math::sqrt(1 - w * w);
		r_axis.x = x * r;
		r_axis.y = y * r;
		r_axis.z = z * r;
	}

	void operator*=(const Quaternion &p_q);
	Quaternion operator*(const Quaternion &p_q) const;

	Quaternion operator*(const Vector3 &v) const {
		return Quaternion(w * v.x + y * v.z - z * v.y,
				w * v.y + z * v.x - x * v.z,
				w * v.z + x * v.y - y * v.x,
				-x * v.x - y * v.y - z * v.z);
	}

	_FORCE_INLINE_ Vector3 xform(const Vector3 &v) const {
#ifdef MATH_CHECKS
		ERR_FAIL_COND_V_MSG(!is_normalized(), v, "The quaternion must be normalized.");
#endif
		Vector3 u(x, y, z);
		Vector3 uv = u.cross(v);
		return v + ((uv * w) + u.cross(uv)) * ((real_t)2);
	}

	_FORCE_INLINE_ void operator+=(const Quaternion &p_q);
	_FORCE_INLINE_ void operator-=(const Quaternion &p_q);
	_FORCE_INLINE_ void operator*=(const real_t s);
	_FORCE_INLINE_ void operator/=(const real_t s);
	_FORCE_INLINE_ Quaternion operator+(const Quaternion &q2) const;
	_FORCE_INLINE_ Quaternion operator-(const Quaternion &q2) const;
	_FORCE_INLINE_ Quaternion operator-() const;
	_FORCE_INLINE_ Quaternion operator*(const real_t s) const;
	_FORCE_INLINE_ Quaternion operator/(const real_t s) const;

	_FORCE_INLINE_ bool operator==(const Quaternion &p_quat) const;
	_FORCE_INLINE_ bool operator!=(const Quaternion &p_quat) const;

	operator String() const;

	// Note this is on in gdnative api, only the constructor that calls this.
	bool set_shortest_arc(const Vector3 &p_from, const Vector3 &p_to) {
#ifdef MATH_CHECKS
		ERR_FAIL_COND_V_MSG(p_from.is_zero_approx() || p_to.is_zero_approx(), false, "The vectors must not be zero.");
#endif
#ifdef REAL_T_IS_DOUBLE
		constexpr real_t ALMOST_ONE = 0.999999999999999;
#else
		constexpr real_t ALMOST_ONE = 0.99999975f;
#endif
		Vector3 n0 = p_from.normalized();
		Vector3 n1 = p_to.normalized();

		real_t d = n0.dot(n1);
		if (Math::abs(d) > ALMOST_ONE) {
			if (d >= 0) {
				// Vectors point to the same direction.
				x = 0;
				y = 0;
				z = 0;
				w = 1;
				return true;
			}

			// Get any perpendicular vector.
			Vector3 axis = n0.cross((Math::abs(n0.x) <= Math::abs(n0.y) && Math::abs(n0.x) <= Math::abs(n0.z)) ? Vector3(1, 0, 0) : Vector3(0, 1, 0)).normalized();
			x = axis.x;
			y = axis.y;
			z = axis.z;
			w = 0;
			return false;
		}

		Vector3 c = n0.cross(n1);
		real_t s = Math::sqrt((1 + d) * 2);
		real_t rs = 1 / s;

		x = c.x * rs;
		y = c.y * rs;
		z = c.z * rs;
		w = s * 0.5f;
		normalize();
		return true;
	}

	inline void set(real_t p_x, real_t p_y, real_t p_z, real_t p_w) {
		x = p_x;
		y = p_y;
		z = p_z;
		w = p_w;
	}
	inline Quaternion(real_t p_x, real_t p_y, real_t p_z, real_t p_w) :
			x(p_x),
			y(p_y),
			z(p_z),
			w(p_w) {
	}
	Quaternion(const Vector3 &axis, const real_t angle) {
		set_axis_angle(axis, angle);
	}

	Quaternion(const Vector3 &euler) {
		set_euler(euler);
	}
	Quaternion(const Quaternion &p_q) :
			x(p_q.x),
			y(p_q.y),
			z(p_q.z),
			w(p_q.w) {
	}

	Quaternion &operator=(const Quaternion &p_q) {
		x = p_q.x;
		y = p_q.y;
		z = p_q.z;
		w = p_q.w;
		return *this;
	}

	Quaternion(const Vector3 &p_v0, const Vector3 &p_v1) {
		set_shortest_arc(p_v0, p_v1);
	}

	inline Quaternion() :
			x(0),
			y(0),
			z(0),
			w(1) {
	}
};

real_t Quaternion::dot(const Quaternion &p_q) const {
	return x * p_q.x + y * p_q.y + z * p_q.z + w * p_q.w;
}

real_t Quaternion::length_squared() const {
	return dot(*this);
}

void Quaternion::operator+=(const Quaternion &p_q) {
	x += p_q.x;
	y += p_q.y;
	z += p_q.z;
	w += p_q.w;
}

void Quaternion::operator-=(const Quaternion &p_q) {
	x -= p_q.x;
	y -= p_q.y;
	z -= p_q.z;
	w -= p_q.w;
}

void Quaternion::operator*=(const real_t s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

void Quaternion::operator/=(const real_t s) {
	*this *= 1 / s;
}

Quaternion Quaternion::operator+(const Quaternion &q2) const {
	const Quaternion &q1 = *this;
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

Quaternion Quaternion::operator-(const Quaternion &q2) const {
	const Quaternion &q1 = *this;
	return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

Quaternion Quaternion::operator-() const {
	const Quaternion &q2 = *this;
	return Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
}

Quaternion Quaternion::operator*(const real_t s) const {
	return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator/(const real_t s) const {
	return *this * (1 / s);
}

bool Quaternion::operator==(const Quaternion &p_quat) const {
	return x == p_quat.x && y == p_quat.y && z == p_quat.z && w == p_quat.w;
}

bool Quaternion::operator!=(const Quaternion &p_quat) const {
	return x != p_quat.x || y != p_quat.y || z != p_quat.z || w != p_quat.w;
}

#endif
