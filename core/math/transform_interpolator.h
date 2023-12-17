#ifndef TRANSFORM_INTERPOLATOR_H
#define TRANSFORM_INTERPOLATOR_H

/*************************************************************************/
/*  transform_interpolator.h                                             */
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
#include "core/math/quaternion.h"
#include "core/math/transform.h"
#include "core/math/vector3.h"

// Keep all the functions for fixed timestep interpolation together.
// There are two stages involved:
// Finding a method, for determining the interpolation method between two
// keyframes (which are physics ticks).
// And applying that pre-determined method.

// Pre-determining the method makes sense because it is expensive and often
// several frames may occur between each physics tick, which will make it cheaper
// than performing every frame.

struct Transform;
struct Transform2D;

class TransformInterpolator {
public:
	enum Method {
		INTERP_LERP = 0,
		INTERP_SLERP = 1,
		INTERP_SCALED_SLERP = 2,
	};

private:
	static real_t _vec3_normalize(Vector3 &p_vec);
	static Vector3 _basis_orthonormalize(Basis &r_basis);
	static real_t vec3_sum(const Vector3 &p_pt) { return p_pt.x + p_pt.y + p_pt.z; }
	static Method _test_basis(Basis p_basis, bool r_needed_normalize, Quaternion &r_quat);
	static Basis _basis_slerp_unchecked(Basis p_from, Basis p_to, real_t p_fraction);
	static Quaternion _quat_slerp_unchecked(const Quaternion &p_from, const Quaternion &p_to, real_t p_fraction);
	static Quaternion _basis_to_quat_unchecked(const Basis &p_basis);
	static bool _basis_is_orthogonal(const Basis &p_basis, real_t p_epsilon = 0.01f);
	static bool _basis_is_orthogonal_any_scale(const Basis &p_basis);
	static bool _sign(real_t p_val) { return p_val >= 0; }

	static void interpolate_basis_linear(const Basis &p_prev, const Basis &p_curr, Basis &r_result, real_t p_fraction);
	static void interpolate_basis_scaled_slerp(Basis p_prev, Basis p_curr, Basis &r_result, real_t p_fraction);

public:
	// Generic functions, use when you don't know what method should be used, e.g. from gdscript.
	// These will be slower.
	static void interpolate_transform(const Transform &p_prev, const Transform &p_curr, Transform &r_result, real_t p_fraction);
	static void interpolate_basis(const Basis &p_prev, const Basis &p_curr, Basis &r_result, real_t p_fraction);
	static void interpolate_transform_2d(const Transform2D &p_prev, const Transform2D &p_curr, Transform2D &r_result, real_t p_fraction);

	// Optimized function when you know ahead of time the method
	static void interpolate_transform_via_method(const Transform &p_prev, const Transform &p_curr, Transform &r_result, real_t p_fraction, Method p_method);
	static void interpolate_basis_via_method(const Basis &p_prev, const Basis &p_curr, Basis &r_result, real_t p_fraction, Method p_method);

	static real_t checksum_transform(const Transform &p_transform);
	static Method find_method(const Basis &p_a, const Basis &p_b);
};

#endif // TRANSFORM_INTERPOLATOR_H
