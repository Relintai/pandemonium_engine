/*************************************************************************/
/*  variant.cpp                                                          */
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

#include "gdn/variant.h"

#include "core/object/reference.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_variant) == sizeof(Variant), "Variant size mismatch");

// Workaround GCC ICE on armv7hl which was affected GCC 6.0 up to 8.0 (GH-16100).
// It was fixed upstream in 8.1, and a fix was backported to 7.4.
// This can be removed once no supported distro ships with versions older than 7.4.
#if defined(__arm__) && defined(__GNUC__) && !defined(__clang__) && \
		(__GNUC__ == 6 || (__GNUC__ == 7 && __GNUC_MINOR__ < 4) || (__GNUC__ == 8 && __GNUC_MINOR__ < 1))
#pragma GCC push_options
#pragma GCC optimize("-O0")
#endif

#define memnew_placement_custom(m_placement, m_class, m_constr) _post_initialize(new (m_placement, sizeof(m_class), "") m_constr)

#if defined(__arm__) && defined(__GNUC__) && !defined(__clang__) && \
		(__GNUC__ == 6 || (__GNUC__ == 7 && __GNUC_MINOR__ < 4) || (__GNUC__ == 8 && __GNUC_MINOR__ < 1))
#pragma GCC pop_options
#endif

// Constructors

pandemonium_variant_type GDAPI pandemonium_variant_get_type(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return (pandemonium_variant_type)self->get_type();
}

pandemonium_bool GDAPI pandemonium_variant_is_ref(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_ref();
}
pandemonium_bool GDAPI pandemonium_variant_is_num(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_num();
}
pandemonium_bool GDAPI pandemonium_variant_is_array(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_array();
}
pandemonium_bool GDAPI pandemonium_variant_is_null(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_null();
}
pandemonium_bool GDAPI pandemonium_variant_is_shared(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_shared();
}
pandemonium_bool GDAPI pandemonium_variant_is_zero(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_zero();
}
pandemonium_bool GDAPI pandemonium_variant_is_one(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_one();
}

pandemonium_int GDAPI pandemonium_variant_get_object_instance_id(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->get_object_instance_id();
}
pandemonium_bool GDAPI pandemonium_variant_is_invalid_object(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->is_invalid_object();
}

void GDAPI pandemonium_variant_new_copy(pandemonium_variant *p_dest, const pandemonium_variant *p_src) {
	Variant *dest = (Variant *)p_dest;
	Variant *src = (Variant *)p_src;
	memnew_placement(dest, Variant(*src));
}

void GDAPI pandemonium_variant_new_nil(pandemonium_variant *r_dest) {
	Variant *dest = (Variant *)r_dest;
	memnew_placement(dest, Variant);
}

void GDAPI pandemonium_variant_new_bool(pandemonium_variant *r_dest, const pandemonium_bool p_b) {
	Variant *dest = (Variant *)r_dest;
	memnew_placement_custom(dest, Variant, Variant(p_b));
}

void GDAPI pandemonium_variant_new_uint(pandemonium_variant *r_dest, const uint64_t p_i) {
	Variant *dest = (Variant *)r_dest;
	memnew_placement_custom(dest, Variant, Variant(p_i));
}

void GDAPI pandemonium_variant_new_int(pandemonium_variant *r_dest, const int64_t p_i) {
	Variant *dest = (Variant *)r_dest;
	memnew_placement_custom(dest, Variant, Variant(p_i));
}

void GDAPI pandemonium_variant_new_real(pandemonium_variant *r_dest, const double p_r) {
	Variant *dest = (Variant *)r_dest;
	memnew_placement_custom(dest, Variant, Variant(p_r));
}

void GDAPI pandemonium_variant_new_string(pandemonium_variant *r_dest, const pandemonium_string *p_s) {
	Variant *dest = (Variant *)r_dest;
	String *s = (String *)p_s;
	memnew_placement_custom(dest, Variant, Variant(*s));
}

void GDAPI pandemonium_variant_new_string_name(pandemonium_variant *r_dest, const pandemonium_string_name *p_s) {
	Variant *dest = (Variant *)r_dest;
	StringName *s = (StringName *)p_s;
	memnew_placement_custom(dest, Variant, Variant(*s));
}

void GDAPI pandemonium_variant_new_vector2(pandemonium_variant *r_dest, const pandemonium_vector2 *p_v2) {
	Variant *dest = (Variant *)r_dest;
	Vector2 *v2 = (Vector2 *)p_v2;
	memnew_placement_custom(dest, Variant, Variant(*v2));
}

void GDAPI pandemonium_variant_new_vector2i(pandemonium_variant *r_dest, const pandemonium_vector2i *p_v2i) {
	Variant *dest = (Variant *)r_dest;
	Vector2i *v2i = (Vector2i *)p_v2i;
	memnew_placement_custom(dest, Variant, Variant(*v2i));
}

void GDAPI pandemonium_variant_new_rect2(pandemonium_variant *r_dest, const pandemonium_rect2 *p_rect2) {
	Variant *dest = (Variant *)r_dest;
	Rect2 *rect2 = (Rect2 *)p_rect2;
	memnew_placement_custom(dest, Variant, Variant(*rect2));
}

void GDAPI pandemonium_variant_new_rect2i(pandemonium_variant *r_dest, const pandemonium_rect2i *p_rect2i) {
	Variant *dest = (Variant *)r_dest;
	Rect2i *rect2i = (Rect2i *)p_rect2i;
	memnew_placement_custom(dest, Variant, Variant(*rect2i));
}

void GDAPI pandemonium_variant_new_vector3(pandemonium_variant *r_dest, const pandemonium_vector3 *p_v3) {
	Variant *dest = (Variant *)r_dest;
	Vector3 *v3 = (Vector3 *)p_v3;
	memnew_placement_custom(dest, Variant, Variant(*v3));
}

void GDAPI pandemonium_variant_new_vector3i(pandemonium_variant *r_dest, const pandemonium_vector3i *p_v3i) {
	Variant *dest = (Variant *)r_dest;
	Vector3i *v3i = (Vector3i *)p_v3i;
	memnew_placement_custom(dest, Variant, Variant(*v3i));
}

void GDAPI pandemonium_variant_new_vector4(pandemonium_variant *r_dest, const pandemonium_vector4 *p_v4) {
	Variant *dest = (Variant *)r_dest;
	Vector4 *v4 = (Vector4 *)p_v4;
	memnew_placement_custom(dest, Variant, Variant(*v4));
}

void GDAPI pandemonium_variant_new_vector4i(pandemonium_variant *r_dest, const pandemonium_vector4i *p_v4i) {
	Variant *dest = (Variant *)r_dest;
	Vector4i *v4i = (Vector4i *)p_v4i;
	memnew_placement_custom(dest, Variant, Variant(*v4i));
}

void GDAPI pandemonium_variant_new_transform2d(pandemonium_variant *r_dest, const pandemonium_transform2d *p_t2d) {
	Variant *dest = (Variant *)r_dest;
	Transform2D *t2d = (Transform2D *)p_t2d;
	memnew_placement_custom(dest, Variant, Variant(*t2d));
}

void GDAPI pandemonium_variant_new_plane(pandemonium_variant *r_dest, const pandemonium_plane *p_plane) {
	Variant *dest = (Variant *)r_dest;
	Plane *plane = (Plane *)p_plane;
	memnew_placement_custom(dest, Variant, Variant(*plane));
}

void GDAPI pandemonium_variant_new_quaternion(pandemonium_variant *r_dest, const pandemonium_quaternion *p_quaternion) {
	Variant *dest = (Variant *)r_dest;
	Quaternion *quaternion = (Quaternion *)p_quaternion;
	memnew_placement_custom(dest, Variant, Variant(*quaternion));
}

void GDAPI pandemonium_variant_new_aabb(pandemonium_variant *r_dest, const pandemonium_aabb *p_aabb) {
	Variant *dest = (Variant *)r_dest;
	AABB *aabb = (AABB *)p_aabb;
	memnew_placement_custom(dest, Variant, Variant(*aabb));
}

void GDAPI pandemonium_variant_new_basis(pandemonium_variant *r_dest, const pandemonium_basis *p_basis) {
	Variant *dest = (Variant *)r_dest;
	Basis *basis = (Basis *)p_basis;
	memnew_placement_custom(dest, Variant, Variant(*basis));
}

void GDAPI pandemonium_variant_new_transform(pandemonium_variant *r_dest, const pandemonium_transform *p_trans) {
	Variant *dest = (Variant *)r_dest;
	Transform *trans = (Transform *)p_trans;
	memnew_placement_custom(dest, Variant, Variant(*trans));
}

void GDAPI pandemonium_variant_new_projection(pandemonium_variant *r_dest, const pandemonium_projection *p_proj) {
	Variant *dest = (Variant *)r_dest;
	Projection *proj = (Projection *)p_proj;
	memnew_placement_custom(dest, Variant, Variant(*proj));
}

void GDAPI pandemonium_variant_new_color(pandemonium_variant *r_dest, const pandemonium_color *p_color) {
	Variant *dest = (Variant *)r_dest;
	Color *color = (Color *)p_color;
	memnew_placement_custom(dest, Variant, Variant(*color));
}

void GDAPI pandemonium_variant_new_node_path(pandemonium_variant *r_dest, const pandemonium_node_path *p_np) {
	Variant *dest = (Variant *)r_dest;
	NodePath *np = (NodePath *)p_np;
	memnew_placement_custom(dest, Variant, Variant(*np));
}

void GDAPI pandemonium_variant_new_rid(pandemonium_variant *r_dest, const pandemonium_rid *p_rid) {
	Variant *dest = (Variant *)r_dest;
	RID *rid = (RID *)p_rid;
	memnew_placement_custom(dest, Variant, Variant(*rid));
}

void GDAPI pandemonium_variant_new_object(pandemonium_variant *r_dest, const pandemonium_object *p_obj) {
	Variant *dest = (Variant *)r_dest;
	Object *obj = (Object *)p_obj;
	Reference *reference = Object::cast_to<Reference>(obj);
	REF ref;
	if (reference) {
		ref = REF(reference);
	}
	if (!ref.is_null()) {
		memnew_placement_custom(dest, Variant, Variant(ref.get_ref_ptr()));
	} else {
#if defined(DEBUG_METHODS_ENABLED)
		if (reference) {
			ERR_PRINT("Reference object has 0 refcount in pandemonium_variant_new_object - you lost it somewhere.");
		}
#endif
		memnew_placement_custom(dest, Variant, Variant(obj));
	}
}

void GDAPI pandemonium_variant_new_dictionary(pandemonium_variant *r_dest, const pandemonium_dictionary *p_dict) {
	Variant *dest = (Variant *)r_dest;
	Dictionary *dict = (Dictionary *)p_dict;
	memnew_placement_custom(dest, Variant, Variant(*dict));
}

void GDAPI pandemonium_variant_new_array(pandemonium_variant *r_dest, const pandemonium_array *p_arr) {
	Variant *dest = (Variant *)r_dest;
	Array *arr = (Array *)p_arr;
	memnew_placement_custom(dest, Variant, Variant(*arr));
}

void GDAPI pandemonium_variant_new_pool_byte_array(pandemonium_variant *r_dest, const pandemonium_pool_byte_array *p_pba) {
	Variant *dest = (Variant *)r_dest;
	PoolByteArray *pba = (PoolByteArray *)p_pba;
	memnew_placement_custom(dest, Variant, Variant(*pba));
}

void GDAPI pandemonium_variant_new_pool_int_array(pandemonium_variant *r_dest, const pandemonium_pool_int_array *p_pia) {
	Variant *dest = (Variant *)r_dest;
	PoolIntArray *pia = (PoolIntArray *)p_pia;
	memnew_placement_custom(dest, Variant, Variant(*pia));
}

void GDAPI pandemonium_variant_new_pool_real_array(pandemonium_variant *r_dest, const pandemonium_pool_real_array *p_pra) {
	Variant *dest = (Variant *)r_dest;
	PoolRealArray *pra = (PoolRealArray *)p_pra;
	memnew_placement_custom(dest, Variant, Variant(*pra));
}

void GDAPI pandemonium_variant_new_pool_string_array(pandemonium_variant *r_dest, const pandemonium_pool_string_array *p_psa) {
	Variant *dest = (Variant *)r_dest;
	PoolStringArray *psa = (PoolStringArray *)p_psa;
	memnew_placement_custom(dest, Variant, Variant(*psa));
}

void GDAPI pandemonium_variant_new_pool_vector2_array(pandemonium_variant *r_dest, const pandemonium_pool_vector2_array *p_pv2a) {
	Variant *dest = (Variant *)r_dest;
	PoolVector2Array *pv2a = (PoolVector2Array *)p_pv2a;
	memnew_placement_custom(dest, Variant, Variant(*pv2a));
}

void GDAPI pandemonium_variant_new_pool_vector2i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector2i_array *p_pv2ia) {
	Variant *dest = (Variant *)r_dest;
	PoolVector2iArray *pv2ia = (PoolVector2iArray *)p_pv2ia;
	memnew_placement_custom(dest, Variant, Variant(*pv2ia));
}

void GDAPI pandemonium_variant_new_pool_vector3_array(pandemonium_variant *r_dest, const pandemonium_pool_vector3_array *p_pv3a) {
	Variant *dest = (Variant *)r_dest;
	PoolVector3Array *pv3a = (PoolVector3Array *)p_pv3a;
	memnew_placement_custom(dest, Variant, Variant(*pv3a));
}

void GDAPI pandemonium_variant_new_pool_vector3i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector3i_array *p_pv3ia) {
	Variant *dest = (Variant *)r_dest;
	PoolVector3iArray *pv3ia = (PoolVector3iArray *)p_pv3ia;
	memnew_placement_custom(dest, Variant, Variant(*pv3ia));
}

void GDAPI pandemonium_variant_new_pool_vector4_array(pandemonium_variant *r_dest, const pandemonium_pool_vector4_array *p_pv4a) {
	Variant *dest = (Variant *)r_dest;
	PoolVector4Array *pv4a = (PoolVector4Array *)p_pv4a;
	memnew_placement_custom(dest, Variant, Variant(*pv4a));
}

void GDAPI pandemonium_variant_new_pool_vector4i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector4i_array *p_pv4ia) {
	Variant *dest = (Variant *)r_dest;
	PoolVector4iArray *pv4ia = (PoolVector4iArray *)p_pv4ia;
	memnew_placement_custom(dest, Variant, Variant(*pv4ia));
}

void GDAPI pandemonium_variant_new_pool_color_array(pandemonium_variant *r_dest, const pandemonium_pool_color_array *p_pca) {
	Variant *dest = (Variant *)r_dest;
	PoolColorArray *pca = (PoolColorArray *)p_pca;
	memnew_placement_custom(dest, Variant, Variant(*pca));
}

pandemonium_bool GDAPI pandemonium_variant_as_bool(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->operator bool();
}

uint64_t GDAPI pandemonium_variant_as_uint(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->operator uint64_t();
}

int64_t GDAPI pandemonium_variant_as_int(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->operator int64_t();
}

double GDAPI pandemonium_variant_as_real(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->operator double();
}

pandemonium_string GDAPI pandemonium_variant_as_string(const pandemonium_variant *p_self) {
	pandemonium_string raw_dest;
	const Variant *self = (const Variant *)p_self;
	String *dest = (String *)&raw_dest;
	memnew_placement(dest, String(self->operator String())); // operator = is overloaded by String
	return raw_dest;
}

pandemonium_string_name GDAPI pandemonium_variant_as_string_name(const pandemonium_variant *p_self) {
	pandemonium_string_name raw_dest;
	const Variant *self = (const Variant *)p_self;
	StringName *dest = (StringName *)&raw_dest;
	memnew_placement(dest, StringName(self->operator StringName())); // operator = is overloaded by String
	return raw_dest;
}

pandemonium_vector2 GDAPI pandemonium_variant_as_vector2(const pandemonium_variant *p_self) {
	pandemonium_vector2 raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector2 *dest = (Vector2 *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_vector2i GDAPI pandemonium_variant_as_vector2i(const pandemonium_variant *p_self) {
	pandemonium_vector2i raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector2i *dest = (Vector2i *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_rect2 GDAPI pandemonium_variant_as_rect2(const pandemonium_variant *p_self) {
	pandemonium_rect2 raw_dest;
	const Variant *self = (const Variant *)p_self;
	Rect2 *dest = (Rect2 *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_rect2i GDAPI pandemonium_variant_as_rect2i(const pandemonium_variant *p_self) {
	pandemonium_rect2i raw_dest;
	const Variant *self = (const Variant *)p_self;
	Rect2i *dest = (Rect2i *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_vector3 GDAPI pandemonium_variant_as_vector3(const pandemonium_variant *p_self) {
	pandemonium_vector3 raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector3 *dest = (Vector3 *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_vector3i GDAPI pandemonium_variant_as_vector3i(const pandemonium_variant *p_self) {
	pandemonium_vector3i raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector3i *dest = (Vector3i *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_vector4 GDAPI pandemonium_variant_as_vector4(const pandemonium_variant *p_self) {
	pandemonium_vector4 raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector4 *dest = (Vector4 *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_vector4i GDAPI pandemonium_variant_as_vector4i(const pandemonium_variant *p_self) {
	pandemonium_vector4i raw_dest;
	const Variant *self = (const Variant *)p_self;
	Vector4i *dest = (Vector4i *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_transform2d GDAPI pandemonium_variant_as_transform2d(const pandemonium_variant *p_self) {
	pandemonium_transform2d raw_dest;
	const Variant *self = (const Variant *)p_self;
	Transform2D *dest = (Transform2D *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_plane GDAPI pandemonium_variant_as_plane(const pandemonium_variant *p_self) {
	pandemonium_plane raw_dest;
	const Variant *self = (const Variant *)p_self;
	Plane *dest = (Plane *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_quaternion GDAPI pandemonium_variant_as_quaternion(const pandemonium_variant *p_self) {
	pandemonium_quaternion raw_dest;
	const Variant *self = (const Variant *)p_self;
	Quaternion *dest = (Quaternion *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_aabb GDAPI pandemonium_variant_as_aabb(const pandemonium_variant *p_self) {
	pandemonium_aabb raw_dest;
	const Variant *self = (const Variant *)p_self;
	AABB *dest = (AABB *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_basis GDAPI pandemonium_variant_as_basis(const pandemonium_variant *p_self) {
	pandemonium_basis raw_dest;
	const Variant *self = (const Variant *)p_self;
	Basis *dest = (Basis *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_transform GDAPI pandemonium_variant_as_transform(const pandemonium_variant *p_self) {
	pandemonium_transform raw_dest;
	const Variant *self = (const Variant *)p_self;
	Transform *dest = (Transform *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_projection GDAPI pandemonium_variant_as_projection(const pandemonium_variant *p_self) {
	pandemonium_projection raw_dest;
	const Variant *self = (const Variant *)p_self;
	Projection *dest = (Projection *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_color GDAPI pandemonium_variant_as_color(const pandemonium_variant *p_self) {
	pandemonium_color raw_dest;
	const Variant *self = (const Variant *)p_self;
	Color *dest = (Color *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_node_path GDAPI pandemonium_variant_as_node_path(const pandemonium_variant *p_self) {
	pandemonium_node_path raw_dest;
	const Variant *self = (const Variant *)p_self;
	NodePath *dest = (NodePath *)&raw_dest;
	memnew_placement(dest, NodePath(self->operator NodePath())); // operator = is overloaded by NodePath
	return raw_dest;
}

pandemonium_rid GDAPI pandemonium_variant_as_rid(const pandemonium_variant *p_self) {
	pandemonium_rid raw_dest;
	const Variant *self = (const Variant *)p_self;
	RID *dest = (RID *)&raw_dest;
	*dest = *self;
	return raw_dest;
}

pandemonium_object GDAPI *pandemonium_variant_as_object(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	Object *dest;
	dest = *self;
	return (pandemonium_object *)dest;
}

pandemonium_dictionary GDAPI pandemonium_variant_as_dictionary(const pandemonium_variant *p_self) {
	pandemonium_dictionary raw_dest;
	const Variant *self = (const Variant *)p_self;
	Dictionary *dest = (Dictionary *)&raw_dest;
	memnew_placement(dest, Dictionary(self->operator Dictionary())); // operator = is overloaded by Dictionary
	return raw_dest;
}

pandemonium_array GDAPI pandemonium_variant_as_array(const pandemonium_variant *p_self) {
	pandemonium_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	Array *dest = (Array *)&raw_dest;
	memnew_placement(dest, Array(self->operator Array())); // operator = is overloaded by Array
	return raw_dest;
}

pandemonium_pool_byte_array GDAPI pandemonium_variant_as_pool_byte_array(const pandemonium_variant *p_self) {
	pandemonium_pool_byte_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolByteArray *dest = (PoolByteArray *)&raw_dest;
	memnew_placement(dest, PoolByteArray(self->operator PoolByteArray())); // operator = is overloaded by PoolByteArray
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_int_array GDAPI pandemonium_variant_as_pool_int_array(const pandemonium_variant *p_self) {
	pandemonium_pool_int_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolIntArray *dest = (PoolIntArray *)&raw_dest;
	memnew_placement(dest, PoolIntArray(self->operator PoolIntArray())); // operator = is overloaded by PoolIntArray
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_real_array GDAPI pandemonium_variant_as_pool_real_array(const pandemonium_variant *p_self) {
	pandemonium_pool_real_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolRealArray *dest = (PoolRealArray *)&raw_dest;
	memnew_placement(dest, PoolRealArray(self->operator PoolRealArray())); // operator = is overloaded by PoolRealArray
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_string_array GDAPI pandemonium_variant_as_pool_string_array(const pandemonium_variant *p_self) {
	pandemonium_pool_string_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolStringArray *dest = (PoolStringArray *)&raw_dest;
	memnew_placement(dest, PoolStringArray(self->operator PoolStringArray())); // operator = is overloaded by PoolStringArray
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector2_array GDAPI pandemonium_variant_as_pool_vector2_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector2_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector2Array *dest = (PoolVector2Array *)&raw_dest;
	memnew_placement(dest, PoolVector2Array(self->operator PoolVector2Array())); // operator = is overloaded by PoolVector2Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector2i_array GDAPI pandemonium_variant_as_pool_vector2i_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector2i_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector2iArray *dest = (PoolVector2iArray *)&raw_dest;
	memnew_placement(dest, PoolVector2iArray(self->operator PoolVector2iArray())); // operator = is overloaded by PoolVector2Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector3_array GDAPI pandemonium_variant_as_pool_vector3_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector3_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector3Array *dest = (PoolVector3Array *)&raw_dest;
	memnew_placement(dest, PoolVector3Array(self->operator PoolVector3Array())); // operator = is overloaded by PoolVector3Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector3i_array GDAPI pandemonium_variant_as_pool_vector3i_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector3i_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector3iArray *dest = (PoolVector3iArray *)&raw_dest;
	memnew_placement(dest, PoolVector3iArray(self->operator PoolVector3iArray())); // operator = is overloaded by PoolVector3Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector4_array GDAPI pandemonium_variant_as_pool_vector4_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector4_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector4Array *dest = (PoolVector4Array *)&raw_dest;
	memnew_placement(dest, PoolVector4Array(self->operator PoolVector4Array())); // operator = is overloaded by PoolVector3Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_vector4i_array GDAPI pandemonium_variant_as_pool_vector4i_array(const pandemonium_variant *p_self) {
	pandemonium_pool_vector4i_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolVector4iArray *dest = (PoolVector4iArray *)&raw_dest;
	memnew_placement(dest, PoolVector4iArray(self->operator PoolVector4iArray())); // operator = is overloaded by PoolVector3Array
	*dest = *self;
	return raw_dest;
}

pandemonium_pool_color_array GDAPI pandemonium_variant_as_pool_color_array(const pandemonium_variant *p_self) {
	pandemonium_pool_color_array raw_dest;
	const Variant *self = (const Variant *)p_self;
	PoolColorArray *dest = (PoolColorArray *)&raw_dest;
	memnew_placement(dest, PoolColorArray(self->operator PoolColorArray())); // operator = is overloaded by PoolColorArray
	*dest = *self;
	return raw_dest;
}

pandemonium_variant GDAPI pandemonium_variant_call(pandemonium_variant *p_self, const pandemonium_string *p_method, const pandemonium_variant **p_args, const pandemonium_int p_argcount, pandemonium_variant_call_error *r_error) {
	Variant *self = (Variant *)p_self;
	String *method = (String *)p_method;
	const Variant **args = (const Variant **)p_args;
	pandemonium_variant raw_dest;
	Variant *dest = (Variant *)&raw_dest;
	Variant::CallError error;
	memnew_placement_custom(dest, Variant, Variant(self->call(*method, args, p_argcount, error)));
	if (r_error) {
		r_error->error = (pandemonium_variant_call_error_error)error.error;
		r_error->argument = error.argument;
		r_error->expected = (pandemonium_variant_type)error.expected;
	}
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_variant_has_method(const pandemonium_variant *p_self, const pandemonium_string *p_method) {
	const Variant *self = (const Variant *)p_self;
	const String *method = (const String *)p_method;
	return self->has_method(*method);
}

pandemonium_bool GDAPI pandemonium_variant_deep_equal(const pandemonium_variant *p_self, const pandemonium_variant *p_variant) {
	const Variant *self = (const Variant *)p_self;
	const Variant *variant = (const Variant *)p_variant;
	return self->deep_equal(*variant);
}
pandemonium_bool GDAPI pandemonium_variant_deep_equal_recursion_count(const pandemonium_variant *p_self, const pandemonium_variant *p_variant, pandemonium_int p_recursion_count) {
	const Variant *self = (const Variant *)p_self;
	const Variant *variant = (const Variant *)p_variant;
	return self->deep_equal(*variant, p_recursion_count);
}

pandemonium_bool GDAPI pandemonium_variant_operator_equal(const pandemonium_variant *p_self, const pandemonium_variant *p_other) {
	const Variant *self = (const Variant *)p_self;
	const Variant *other = (const Variant *)p_other;
	return self->operator==(*other);
}

pandemonium_bool GDAPI pandemonium_variant_operator_less(const pandemonium_variant *p_self, const pandemonium_variant *p_other) {
	const Variant *self = (const Variant *)p_self;
	const Variant *other = (const Variant *)p_other;
	return self->operator<(*other);
}

pandemonium_int GDAPI pandemonium_variant_hash(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->hash();
}

pandemonium_bool GDAPI pandemonium_variant_hash_compare(const pandemonium_variant *p_self, const pandemonium_variant *p_other) {
	const Variant *self = (const Variant *)p_self;
	const Variant *other = (const Variant *)p_other;
	return self->hash_compare(*other);
}

pandemonium_bool GDAPI pandemonium_variant_booleanize(const pandemonium_variant *p_self) {
	const Variant *self = (const Variant *)p_self;
	return self->booleanize();
}

void GDAPI pandemonium_variant_destroy(pandemonium_variant *p_self) {
	Variant *self = (Variant *)p_self;
	self->~Variant();
}

// GDNative core 1.1

pandemonium_string GDAPI pandemonium_variant_get_operator_name(pandemonium_variant_operator p_op) {
	Variant::Operator op = (Variant::Operator)p_op;
	pandemonium_string raw_dest;
	String *dest = (String *)&raw_dest;
	memnew_placement(dest, String(Variant::get_operator_name(op))); // operator = is overloaded by String
	return raw_dest;
}

void GDAPI pandemonium_variant_evaluate(pandemonium_variant_operator p_op, const pandemonium_variant *p_a, const pandemonium_variant *p_b, pandemonium_variant *r_ret, pandemonium_bool *r_valid) {
	Variant::Operator op = (Variant::Operator)p_op;
	const Variant *a = (const Variant *)p_a;
	const Variant *b = (const Variant *)p_b;
	Variant *ret = (Variant *)r_ret;
	Variant::evaluate(op, *a, *b, *ret, *r_valid);
}

void GDAPI pandemonium_variant_zero(pandemonium_variant *p_self) {
	Variant *self = (Variant *)p_self;
	self->zero();
}
pandemonium_variant GDAPI pandemonium_variant_duplicate(const pandemonium_variant *p_self) {
	pandemonium_variant dest;
	const Variant *self = (const Variant *)p_self;
	*((Variant *)&dest) = self->duplicate();
	return dest;
}
pandemonium_variant GDAPI pandemonium_variant_duplicate_deep(const pandemonium_variant *p_self, pandemonium_bool deep) {
	pandemonium_variant dest;
	const Variant *self = (const Variant *)p_self;
	*((Variant *)&dest) = self->duplicate(deep);
	return dest;
}

#ifdef __cplusplus
}
#endif
