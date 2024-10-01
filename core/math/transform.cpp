/*************************************************************************/
/*  transform.cpp                                                        */
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

#include "transform.h"

#include "core/math/math_funcs.h"
#include "core/string/print_string.h"

void Transform::invert() {
	basis.transpose();
	origin = basis.xform(-origin);
}

Transform Transform::inverse() const {
	// FIXME: this function assumes the basis is a rotation matrix, with no scaling.
	// Transform::affine_inverse can handle matrices with scaling, so GDScript should eventually use that.
	Transform ret = *this;
	ret.invert();
	return ret;
}

void Transform::affine_invert() {
	basis.invert();
	origin = basis.xform(-origin);
}

Transform Transform::affine_inverse() const {
	Transform ret = *this;
	ret.affine_invert();
	return ret;
}

Transform Transform::rotated(const Vector3 &p_axis, real_t p_angle) const {
	// Equivalent to left multiplication
	Basis p_basis(p_axis, p_angle);
	return Transform(p_basis * basis, p_basis.xform(origin));
}

Transform Transform::rotated_local(const Vector3 &p_axis, real_t p_angle) const {
	// Equivalent to right multiplication
	Basis p_basis(p_axis, p_angle);
	return Transform(basis * p_basis, origin);
}

void Transform::rotate(const Vector3 &p_axis, real_t p_phi) {
	*this = rotated(p_axis, p_phi);
}

void Transform::rotate_local(const Vector3 &p_axis, real_t p_phi) {
	*this = rotated_local(p_axis, p_phi);
}

void Transform::rotate_basis(const Vector3 &p_axis, real_t p_phi) {
	basis.rotate(p_axis, p_phi);
}

void Transform::set_look_at(const Vector3 &p_eye, const Vector3 &p_target, const Vector3 &p_up, bool p_use_model_front) {
	basis = Basis::create_looking_at(p_target - p_eye, p_up, p_use_model_front);
	origin = p_eye;
}

Transform Transform::looking_at(const Vector3 &p_target, const Vector3 &p_up, bool p_use_model_front) const {
	Transform t = *this;
	t.set_look_at(origin, p_target, p_up,p_use_model_front);
	return t;
}

void Transform::scale(const Vector3 &p_scale) {
	basis.scale(p_scale);
	origin *= p_scale;
}

Transform Transform::scaled(const Vector3 &p_scale) const {
	// Equivalent to left multiplication
	return Transform(basis.scaled(p_scale), origin * p_scale);
}

Transform Transform::scaled_local(const Vector3 &p_scale) const {
	// Equivalent to right multiplication
	return Transform(basis.scaled_local(p_scale), origin);
}

void Transform::scale_basis(const Vector3 &p_scale) {
	basis.scale(p_scale);
}

void Transform::translate_local(real_t p_tx, real_t p_ty, real_t p_tz) {
	translate_local(Vector3(p_tx, p_ty, p_tz));
}
void Transform::translate_local(const Vector3 &p_translation) {
	for (int i = 0; i < 3; i++) {
		origin[i] += basis[i].dot(p_translation);
	}
}

void Transform::translate_localr(real_t p_tx, real_t p_ty, real_t p_tz) {
	translate_local(Vector3(p_tx, p_ty, p_tz));
}
void Transform::translate_localv(const Vector3 &p_translation) {
	for (int i = 0; i < 3; i++) {
		origin[i] += basis[i].dot(p_translation);
	}
}

Transform Transform::translated(const Vector3 &p_translation) const {
	// Equivalent to left multiplication
	return Transform(basis, origin + p_translation);
}

Transform Transform::translated_local(const Vector3 &p_translation) const {
	// Equivalent to right multiplication
	return Transform(basis, origin + basis.xform(p_translation));
}

void Transform::orthonormalize() {
	basis.orthonormalize();
}

Transform Transform::orthonormalized() const {
	Transform _copy = *this;
	_copy.orthonormalize();
	return _copy;
}

void Transform::orthogonalize() {
	basis.orthogonalize();
}

Transform Transform::orthogonalized() const {
	Transform _copy = *this;
	_copy.orthogonalize();
	return _copy;
}

bool Transform::is_equal_approx(const Transform &p_transform) const {
	return basis.is_equal_approx(p_transform.basis) && origin.is_equal_approx(p_transform.origin);
}

bool Transform::operator==(const Transform &p_transform) const {
	return (basis == p_transform.basis && origin == p_transform.origin);
}
bool Transform::operator!=(const Transform &p_transform) const {
	return (basis != p_transform.basis || origin != p_transform.origin);
}

void Transform::operator*=(const Transform &p_transform) {
	origin = xform(p_transform.origin);
	basis *= p_transform.basis;
}

Transform Transform::operator*(const Transform &p_transform) const {
	Transform t = *this;
	t *= p_transform;
	return t;
}

void Transform::operator*=(const real_t p_val) {
	origin *= p_val;
	basis *= p_val;
}

Transform Transform::operator*(const real_t p_val) const {
	Transform ret(*this);
	ret *= p_val;
	return ret;
}

Transform Transform::spherical_interpolate_with(const Transform &p_transform, real_t p_c) const {
	/* not sure if very "efficient" but good enough? */

	Transform interp;

	Vector3 src_scale = basis.get_scale();
	Quaternion src_rot = basis.get_rotation_quaternion();
	Vector3 src_loc = origin;

	Vector3 dst_scale = p_transform.basis.get_scale();
	Quaternion dst_rot = p_transform.basis.get_rotation_quaternion();
	Vector3 dst_loc = p_transform.origin;

	interp.basis.set_quaternion_scale(src_rot.slerp(dst_rot, p_c).normalized(), src_scale.linear_interpolate(dst_scale, p_c));
	interp.origin = src_loc.linear_interpolate(dst_loc, p_c);

	return interp;
}

Transform Transform::interpolate_with(const Transform &p_transform, real_t p_c) const {
	/* not sure if very "efficient" but good enough? */

	Vector3 src_scale = basis.get_scale();
	Quaternion src_rot = basis.get_rotation_quaternion();
	Vector3 src_loc = origin;

	Vector3 dst_scale = p_transform.basis.get_scale();
	Quaternion dst_rot = p_transform.basis.get_rotation_quaternion();
	Vector3 dst_loc = p_transform.origin;

	Transform interp;
	interp.basis.set_quaternion_scale(src_rot.slerp(dst_rot, p_c).normalized(), src_scale.linear_interpolate(dst_scale, p_c));
	interp.origin = src_loc.linear_interpolate(dst_loc, p_c);

	return interp;
}

Transform::operator String() const {
	return "[X: " + basis.get_axis(0).operator String() +
			", Y: " + basis.get_axis(1).operator String() +
			", Z: " + basis.get_axis(2).operator String() +
			", O: " + origin.operator String() + "]";
}

Transform::Transform(const Basis &p_basis, const Vector3 &p_origin) :
		basis(p_basis),
		origin(p_origin) {
}

Transform::Transform(real_t xx, real_t xy, real_t xz, real_t yx, real_t yy, real_t yz, real_t zx, real_t zy, real_t zz, real_t ox, real_t oy, real_t oz) {
	basis = Basis(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	origin = Vector3(ox, oy, oz);
}

Transform::Transform(const Vector3 &p_x, const Vector3 &p_y, const Vector3 &p_z, const Vector3 &p_origin) :
		origin(p_origin) {
	basis.set_column(0, p_x);
	basis.set_column(1, p_y);
	basis.set_column(2, p_z);
}
