#ifndef TRANSFORM_2D_H
#define TRANSFORM_2D_H

/*************************************************************************/
/*  transform_2d.h                                                       */
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

#include "core/containers/pool_vector.h"
#include "core/math/rect2.h" // also includes vector2, math_funcs, and ustring
#include "core/math/rect2i.h" // also includes vector2i, math_funcs, and ustring

struct _NO_DISCARD_CLASS_ Transform2D {
	// Warning #1: basis of Transform2D is stored differently from Basis. In terms of columns array, the basis matrix looks like "on paper":
	// M = (columns[0][0] columns[1][0])
	//     (columns[0][1] columns[1][1])
	// This is such that the columns, which can be interpreted as basis vectors of the coordinate system "painted" on the object, can be accessed as columns[i].
	// Note that this is the opposite of the indices in mathematical texts, meaning: $M_{12}$ in a math book corresponds to columns[1][0] here.
	// This requires additional care when working with explicit indices.
	// See https://en.wikipedia.org/wiki/Row-_and_column-major_order for further reading.

	// Warning #2: 2D be aware that unlike 3D code, 2D code uses a left-handed coordinate system: Y-axis points down,
	// and angle is measure from +X to +Y in a clockwise-fashion.

	Vector2 columns[3];

	_FORCE_INLINE_ real_t tdotx(const Vector2 &v) const { return columns[0][0] * v.x + columns[1][0] * v.y; }
	_FORCE_INLINE_ real_t tdoty(const Vector2 &v) const { return columns[0][1] * v.x + columns[1][1] * v.y; }

	const Vector2 &operator[](int p_idx) const { return columns[p_idx]; }
	Vector2 &operator[](int p_idx) { return columns[p_idx]; }

	_FORCE_INLINE_ Vector2 get_axis(int p_axis) const {
		ERR_FAIL_INDEX_V(p_axis, 3, Vector2());
		return columns[p_axis];
	}
	_FORCE_INLINE_ void set_axis(int p_axis, const Vector2 &p_vec) {
		ERR_FAIL_INDEX(p_axis, 3);
		columns[p_axis] = p_vec;
	}

	_FORCE_INLINE_ Vector2 get_column(int p_colum) const {
		ERR_FAIL_INDEX_V(p_colum, 3, Vector2());
		return columns[p_colum];
	}
	_FORCE_INLINE_ void set_column(int p_colum, const Vector2 &p_vec) {
		ERR_FAIL_INDEX(p_colum, 3);
		columns[p_colum] = p_vec;
	}

	void invert();
	Transform2D inverse() const;

	void affine_invert();
	Transform2D affine_inverse() const;

	void set_rotation(real_t p_rot);
	real_t get_rotation() const;
	real_t get_skew() const;
	void set_skew(const real_t p_angle);
	_FORCE_INLINE_ void set_rotation_and_scale(real_t p_rot, const Size2 &p_scale);
	_FORCE_INLINE_ void set_rotation_scale_and_skew(const real_t p_rot, const Size2 &p_scale, const real_t p_skew);
	void rotate(real_t p_phi);

	void scale(const Size2 &p_scale);
	void scale_basis(const Size2 &p_scale);
	void translate(real_t p_tx, real_t p_ty);
	void translate(const Vector2 &p_offset);
	void translate_local(real_t p_tx, real_t p_ty);
	void translate_local(const Vector2 &p_translation);

	void translater(real_t p_tx, real_t p_ty);
	void translatev(const Vector2 &p_offset);
	void translate_localr(real_t p_tx, real_t p_ty);
	void translate_localv(const Vector2 &p_translation);

	real_t basis_determinant() const;

	Size2 get_scale() const;
	void set_scale(const Size2 &p_scale);

	_FORCE_INLINE_ const Vector2 &get_origin() const { return columns[2]; }
	_FORCE_INLINE_ void set_origin(const Vector2 &p_origin) { columns[2] = p_origin; }

	Transform2D basis_scaled(const Size2 &p_scale) const;
	Transform2D scaled(const Size2 &p_scale) const;
	Transform2D scaled_local(const Size2 &p_scale) const;
	Transform2D translated(const Vector2 &p_offset) const;
	Transform2D translated_local(const Vector2 &p_offset) const;
	Transform2D rotated(const real_t p_angle) const;
	Transform2D rotated_local(const real_t p_angle) const;

	Transform2D untranslated() const;

	void orthonormalize();
	Transform2D orthonormalized() const;
	bool is_equal_approx(const Transform2D &p_transform) const;

	Transform2D looking_at(const Vector2 &p_target) const;

	bool operator==(const Transform2D &p_transform) const;
	bool operator!=(const Transform2D &p_transform) const;

	void operator*=(const Transform2D &p_transform);
	Transform2D operator*(const Transform2D &p_transform) const;
	void operator*=(const real_t p_val);
	Transform2D operator*(const real_t p_val) const;

	Transform2D interpolate_with(const Transform2D &p_transform, real_t p_c) const {
		return Transform2D(
				Math::lerp_angle(get_rotation(), p_transform.get_rotation(), p_c),
				get_scale().linear_interpolate(p_transform.get_scale(), p_c),
				Math::lerp_angle(get_skew(), p_transform.get_skew(), p_c),
				get_origin().linear_interpolate(p_transform.get_origin(), p_c));
	}

	_FORCE_INLINE_ Vector2 basis_xform(const Vector2 &p_vec) const;
	_FORCE_INLINE_ Vector2 basis_xform_inv(const Vector2 &p_vec) const;
	_FORCE_INLINE_ Vector2 xform(const Vector2 &p_vec) const;
	_FORCE_INLINE_ Vector2 xform_inv(const Vector2 &p_vec) const;

	_FORCE_INLINE_ Rect2 xform(const Rect2 &p_rect) const;
	_FORCE_INLINE_ Rect2 xform_inv(const Rect2 &p_rect) const;

	_FORCE_INLINE_ Vector2i basis_xform(const Vector2i &p_vec) const;
	_FORCE_INLINE_ Vector2i basis_xform_inv(const Vector2i &p_vec) const;
	_FORCE_INLINE_ Vector2i xform(const Vector2i &p_vec) const;
	_FORCE_INLINE_ Vector2i xform_inv(const Vector2i &p_vec) const;

	_FORCE_INLINE_ PoolVector<Vector2> xform(const PoolVector<Vector2> &p_array) const;
	_FORCE_INLINE_ PoolVector<Vector2> xform_inv(const PoolVector<Vector2> &p_array) const;
	_FORCE_INLINE_ PoolVector<Vector2i> xform(const PoolVector<Vector2i> &p_array) const;
	_FORCE_INLINE_ PoolVector<Vector2i> xform_inv(const PoolVector<Vector2i> &p_array) const;

	operator String() const;

	Transform2D(real_t xx, real_t xy, real_t yx, real_t yy, real_t ox, real_t oy) {
		columns[0][0] = xx;
		columns[0][1] = xy;
		columns[1][0] = yx;
		columns[1][1] = yy;
		columns[2][0] = ox;
		columns[2][1] = oy;
	}

	Transform2D(const Vector2 &p_x, const Vector2 &p_y, const Vector2 &p_origin) {
		columns[0] = p_x;
		columns[1] = p_y;
		columns[2] = p_origin;
	}

	Transform2D(real_t p_rot, const Vector2 &p_pos);

	Transform2D(const real_t p_rot, const Size2 &p_scale, const real_t p_skew, const Vector2 &p_pos);

	Transform2D() {
		columns[0][0] = 1.0;
		columns[1][1] = 1.0;
	}
};

Vector2 Transform2D::basis_xform(const Vector2 &p_vec) const {
	return Vector2(
			tdotx(p_vec),
			tdoty(p_vec));
}

Vector2 Transform2D::basis_xform_inv(const Vector2 &p_vec) const {
	return Vector2(
			columns[0].dot(p_vec),
			columns[1].dot(p_vec));
}

Vector2 Transform2D::xform(const Vector2 &p_vec) const {
	return Vector2(
				   tdotx(p_vec),
				   tdoty(p_vec)) +
			columns[2];
}
Vector2 Transform2D::xform_inv(const Vector2 &p_vec) const {
	Vector2 v = p_vec - columns[2];

	return Vector2(
			columns[0].dot(v),
			columns[1].dot(v));
}
Rect2 Transform2D::xform(const Rect2 &p_rect) const {
	Vector2 x = columns[0] * p_rect.size.x;
	Vector2 y = columns[1] * p_rect.size.y;
	Vector2 pos = xform(p_rect.position);

	Rect2 new_rect;
	new_rect.position = pos;
	new_rect.expand_to(pos + x);
	new_rect.expand_to(pos + y);
	new_rect.expand_to(pos + x + y);
	return new_rect;
}

void Transform2D::set_rotation_and_scale(real_t p_rot, const Size2 &p_scale) {
	columns[0][0] = Math::cos(p_rot) * p_scale.x;
	columns[1][1] = Math::cos(p_rot) * p_scale.y;
	columns[1][0] = -Math::sin(p_rot) * p_scale.y;
	columns[0][1] = Math::sin(p_rot) * p_scale.x;
}

void Transform2D::set_rotation_scale_and_skew(const real_t p_rot, const Size2 &p_scale, const real_t p_skew) {
	columns[0][0] = Math::cos(p_rot) * p_scale.x;
	columns[1][1] = Math::cos(p_rot + p_skew) * p_scale.y;
	columns[1][0] = -Math::sin(p_rot + p_skew) * p_scale.y;
	columns[0][1] = Math::sin(p_rot) * p_scale.x;
}

Rect2 Transform2D::xform_inv(const Rect2 &p_rect) const {
	Vector2 ends[4] = {
		xform_inv(p_rect.position),
		xform_inv(Vector2(p_rect.position.x, p_rect.position.y + p_rect.size.y)),
		xform_inv(Vector2(p_rect.position.x + p_rect.size.x, p_rect.position.y + p_rect.size.y)),
		xform_inv(Vector2(p_rect.position.x + p_rect.size.x, p_rect.position.y))
	};

	Rect2 new_rect;
	new_rect.position = ends[0];
	new_rect.expand_to(ends[1]);
	new_rect.expand_to(ends[2]);
	new_rect.expand_to(ends[3]);

	return new_rect;
}

Vector2i Transform2D::basis_xform(const Vector2i &p_vec) const {
	return Vector2i(
			tdotx(p_vec),
			tdoty(p_vec));
}

Vector2i Transform2D::basis_xform_inv(const Vector2i &p_vec) const {
	return Vector2i(
			columns[0].dot(p_vec),
			columns[1].dot(p_vec));
}

Vector2i Transform2D::xform(const Vector2i &p_vec) const {
	return Vector2i(
				   tdotx(p_vec),
				   tdoty(p_vec)) +
			columns[2];
}
Vector2i Transform2D::xform_inv(const Vector2i &p_vec) const {
	Vector2i v = p_vec - columns[2];

	return Vector2i(
			columns[0].dot(v),
			columns[1].dot(v));
}

PoolVector<Vector2> Transform2D::xform(const PoolVector<Vector2> &p_array) const {
	PoolVector<Vector2> array;
	array.resize(p_array.size());

	PoolVector<Vector2>::Read r = p_array.read();
	PoolVector<Vector2>::Write w = array.write();

	for (int i = 0; i < p_array.size(); ++i) {
		w[i] = xform(r[i]);
	}
	return array;
}

PoolVector<Vector2> Transform2D::xform_inv(const PoolVector<Vector2> &p_array) const {
	PoolVector<Vector2> array;
	array.resize(p_array.size());

	PoolVector<Vector2>::Read r = p_array.read();
	PoolVector<Vector2>::Write w = array.write();

	for (int i = 0; i < p_array.size(); ++i) {
		w[i] = xform_inv(r[i]);
	}
	return array;
}

PoolVector<Vector2i> Transform2D::xform(const PoolVector<Vector2i> &p_array) const {
	PoolVector<Vector2i> array;
	array.resize(p_array.size());

	PoolVector<Vector2i>::Read r = p_array.read();
	PoolVector<Vector2i>::Write w = array.write();

	for (int i = 0; i < p_array.size(); ++i) {
		w[i] = xform(r[i]);
	}
	return array;
}

PoolVector<Vector2i> Transform2D::xform_inv(const PoolVector<Vector2i> &p_array) const {
	PoolVector<Vector2i> array;
	array.resize(p_array.size());

	PoolVector<Vector2i>::Read r = p_array.read();
	PoolVector<Vector2i>::Write w = array.write();

	for (int i = 0; i < p_array.size(); ++i) {
		w[i] = xform_inv(r[i]);
	}
	return array;
}

#endif // TRANSFORM_2D_H
