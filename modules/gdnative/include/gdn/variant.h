/**************************************************************************/
/*  variant.h                                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PANDEMONIUM ENGINE                               */
/*                        https://pandemoniumengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef GDNATIVE_VARIANT_H
#define GDNATIVE_VARIANT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_VARIANT_SIZE (16 + sizeof(int64_t))

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_VARIANT_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_VARIANT_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_VARIANT_SIZE];
} pandemonium_variant;
#endif

typedef enum pandemonium_variant_type {
	PANDEMONIUM_VARIANT_TYPE_NIL,

	// atomic types

	PANDEMONIUM_VARIANT_TYPE_BOOL,
	PANDEMONIUM_VARIANT_TYPE_INT,
	PANDEMONIUM_VARIANT_TYPE_REAL,
	PANDEMONIUM_VARIANT_TYPE_STRING,

	// math types

	PANDEMONIUM_VARIANT_TYPE_RECT2, // 5
	PANDEMONIUM_VARIANT_TYPE_RECT2I,
	PANDEMONIUM_VARIANT_TYPE_VECTOR2,
	PANDEMONIUM_VARIANT_TYPE_VECTOR2I,
	PANDEMONIUM_VARIANT_TYPE_VECTOR3,
	PANDEMONIUM_VARIANT_TYPE_VECTOR3I, // 10
	PANDEMONIUM_VARIANT_TYPE_VECTOR4,
	PANDEMONIUM_VARIANT_TYPE_VECTOR4I,

	PANDEMONIUM_VARIANT_TYPE_PLANE,
	PANDEMONIUM_VARIANT_TYPE_QUATERNION,
	PANDEMONIUM_VARIANT_TYPE_AABB,
	PANDEMONIUM_VARIANT_TYPE_BASIS,
	PANDEMONIUM_VARIANT_TYPE_TRANSFORM,
	PANDEMONIUM_VARIANT_TYPE_TRANSFORM2D,
	PANDEMONIUM_VARIANT_TYPE_PROJECTION,

	// misc types

	PANDEMONIUM_VARIANT_TYPE_COLOR, // 20
	PANDEMONIUM_VARIANT_TYPE_NODE_PATH,
	PANDEMONIUM_VARIANT_TYPE_RID,
	PANDEMONIUM_VARIANT_TYPE_OBJECT,
	PANDEMONIUM_VARIANT_TYPE_STRING_NAME,
	PANDEMONIUM_VARIANT_TYPE_DICTIONARY, // 25
	PANDEMONIUM_VARIANT_TYPE_ARRAY,

	// arrays

	PANDEMONIUM_VARIANT_TYPE_POOL_BYTE_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_INT_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_REAL_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_STRING_ARRAY, //30
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR2_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR2I_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR3_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR3I_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR4_ARRAY, //35
	PANDEMONIUM_VARIANT_TYPE_POOL_VECTOR4I_ARRAY,
	PANDEMONIUM_VARIANT_TYPE_POOL_COLOR_ARRAY,

	//VARIANT_MAX // 38
} pandemonium_variant_type;

typedef enum pandemonium_variant_call_error_error {
	PANDEMONIUM_CALL_ERROR_CALL_OK,
	PANDEMONIUM_CALL_ERROR_CALL_ERROR_INVALID_METHOD,
	PANDEMONIUM_CALL_ERROR_CALL_ERROR_INVALID_ARGUMENT,
	PANDEMONIUM_CALL_ERROR_CALL_ERROR_TOO_MANY_ARGUMENTS,
	PANDEMONIUM_CALL_ERROR_CALL_ERROR_TOO_FEW_ARGUMENTS,
	PANDEMONIUM_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL,
} pandemonium_variant_call_error_error;

typedef struct pandemonium_variant_call_error {
	pandemonium_variant_call_error_error error;
	int argument;
	pandemonium_variant_type expected;
} pandemonium_variant_call_error;

typedef enum pandemonium_variant_operator {
	// comparison
	PANDEMONIUM_VARIANT_OP_EQUAL,
	PANDEMONIUM_VARIANT_OP_NOT_EQUAL,
	PANDEMONIUM_VARIANT_OP_LESS,
	PANDEMONIUM_VARIANT_OP_LESS_EQUAL,
	PANDEMONIUM_VARIANT_OP_GREATER,
	PANDEMONIUM_VARIANT_OP_GREATER_EQUAL,

	// mathematic
	PANDEMONIUM_VARIANT_OP_ADD,
	PANDEMONIUM_VARIANT_OP_SUBTRACT,
	PANDEMONIUM_VARIANT_OP_MULTIPLY,
	PANDEMONIUM_VARIANT_OP_DIVIDE,
	PANDEMONIUM_VARIANT_OP_NEGATE,
	PANDEMONIUM_VARIANT_OP_POSITIVE,
	PANDEMONIUM_VARIANT_OP_MODULE,
	PANDEMONIUM_VARIANT_OP_STRING_CONCAT,

	// bitwise
	PANDEMONIUM_VARIANT_OP_SHIFT_LEFT,
	PANDEMONIUM_VARIANT_OP_SHIFT_RIGHT,
	PANDEMONIUM_VARIANT_OP_BIT_AND,
	PANDEMONIUM_VARIANT_OP_BIT_OR,
	PANDEMONIUM_VARIANT_OP_BIT_XOR,
	PANDEMONIUM_VARIANT_OP_BIT_NEGATE,

	// logic
	PANDEMONIUM_VARIANT_OP_AND,
	PANDEMONIUM_VARIANT_OP_OR,
	PANDEMONIUM_VARIANT_OP_XOR,
	PANDEMONIUM_VARIANT_OP_NOT,

	// containment
	PANDEMONIUM_VARIANT_OP_IN,

	PANDEMONIUM_VARIANT_OP_MAX,
} pandemonium_variant_operator;

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/aabb.h>
#include <gdn/array.h>
#include <gdn/basis.h>
#include <gdn/color.h>
#include <gdn/dictionary.h>
#include <gdn/node_path.h>
#include <gdn/plane.h>
#include <gdn/pool_arrays.h>
#include <gdn/projection.h>
#include <gdn/quaternion.h>
#include <gdn/rect2.h>
#include <gdn/rect2i.h>
#include <gdn/rid.h>
#include <gdn/string.h>
#include <gdn/string_name.h>
#include <gdn/transform.h>
#include <gdn/transform2d.h>
#include <gdn/variant.h>
#include <gdn/vector2.h>
#include <gdn/vector2i.h>
#include <gdn/vector3.h>
#include <gdn/vector3i.h>
#include <gdn/vector4.h>
#include <gdn/vector4i.h>

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_variant_type GDAPI pandemonium_variant_get_type(const pandemonium_variant *p_v);
//static String get_type_name(Variant::Type p_type);

//static bool can_convert(Type p_type_from, Type p_type_to);
//static bool can_convert_strict(Type p_type_from, Type p_type_to);

pandemonium_bool GDAPI pandemonium_variant_is_ref(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_num(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_array(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_null(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_shared(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_zero(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_one(const pandemonium_variant *p_self);

pandemonium_int GDAPI pandemonium_variant_get_object_instance_id(const pandemonium_variant *p_self);
pandemonium_bool GDAPI pandemonium_variant_is_invalid_object(const pandemonium_variant *p_self);

pandemonium_string GDAPI pandemonium_variant_get_operator_name(pandemonium_variant_operator p_op);
void GDAPI pandemonium_variant_evaluate(pandemonium_variant_operator p_op, const pandemonium_variant *p_a, const pandemonium_variant *p_b, pandemonium_variant *r_ret, pandemonium_bool *r_valid);
//static _FORCE_INLINE_ Variant evaluate(const Operator &p_op, const Variant &p_a, const Variant &p_b);

void GDAPI pandemonium_variant_zero(pandemonium_variant *p_self);
pandemonium_variant GDAPI pandemonium_variant_duplicate(const pandemonium_variant *p_self);
pandemonium_variant GDAPI pandemonium_variant_duplicate_deep(const pandemonium_variant *p_self, pandemonium_bool deep);

//static void blend(const Variant &a, const Variant &b, float c, Variant &r_dst);
//static void interpolate(const Variant &a, const Variant &b, float c, Variant &r_dst);
//static void sub(const Variant &a, const Variant &b, Variant &r_dst);

//void call_ptr(const StringName &p_method, const Variant **p_args, int p_argcount, Variant *r_ret, CallError &r_error);
pandemonium_variant GDAPI pandemonium_variant_call(pandemonium_variant *p_self, const pandemonium_string *p_method, const pandemonium_variant **p_args, const pandemonium_int p_argcount, pandemonium_variant_call_error *r_error);
//Variant call(const StringName &p_method, const Variant &p_arg1 = Variant(), const Variant &p_arg2 = Variant(), const Variant &p_arg3 = Variant(), const Variant &p_arg4 = Variant(), const Variant &p_arg5 = Variant(), const Variant &p_arg6 = Variant(), const Variant &p_arg7 = Variant(), const Variant &p_arg8 = Variant());

//static String get_call_error_text(Object *p_base, const StringName &p_method, const Variant **p_argptrs, int p_argcount, const Variant::CallError &ce);

//static Variant construct(const Variant::Type, const Variant **p_args, int p_argcount, CallError &r_error, bool p_strict = true);

//void get_method_list(List<MethodInfo> *p_list) const;
pandemonium_bool GDAPI pandemonium_variant_has_method(const pandemonium_variant *p_self, const pandemonium_string *p_method);
//static Vector<Variant::Type> get_method_argument_types(Variant::Type p_type, const StringName &p_method);
//static Vector<Variant> get_method_default_arguments(Variant::Type p_type, const StringName &p_method);
//static Variant::Type get_method_return_type(Variant::Type p_type, const StringName &p_method, bool *r_has_return = nullptr);
//static Vector<StringName> get_method_argument_names(Variant::Type p_type, const StringName &p_method);
//static bool is_method_const(Variant::Type p_type, const StringName &p_method);

//void set_named(const StringName &p_index, const Variant &p_value, bool *r_valid = nullptr);
//Variant get_named(const StringName &p_index, bool *r_valid = nullptr) const;

//void set(const Variant &p_index, const Variant &p_value, bool *r_valid = nullptr);
//Variant get(const Variant &p_index, bool *r_valid = nullptr) const;
//bool in(const Variant &p_index, bool *r_valid = nullptr) const;

//bool iter_init(Variant &r_iter, bool &r_valid) const;
//bool iter_next(Variant &r_iter, bool &r_valid) const;
//Variant iter_get(const Variant &r_iter, bool &r_valid) const;

//void get_property_list(List<PropertyInfo> *p_list) const;

pandemonium_bool GDAPI pandemonium_variant_deep_equal(const pandemonium_variant *p_self, const pandemonium_variant *p_variant);
pandemonium_bool GDAPI pandemonium_variant_deep_equal_recursion_count(const pandemonium_variant *p_self, const pandemonium_variant *p_variant, pandemonium_int p_recursion_count);
pandemonium_bool GDAPI pandemonium_variant_operator_equal(const pandemonium_variant *p_self, const pandemonium_variant *p_other);
pandemonium_bool GDAPI pandemonium_variant_operator_less(const pandemonium_variant *p_self, const pandemonium_variant *p_other);
pandemonium_int GDAPI pandemonium_variant_hash(const pandemonium_variant *p_self);

pandemonium_bool GDAPI pandemonium_variant_hash_compare(const pandemonium_variant *p_self, const pandemonium_variant *p_other);
pandemonium_bool GDAPI pandemonium_variant_booleanize(const pandemonium_variant *p_self);

//void static_assign(const Variant &p_variant);
//static bool has_constant(Variant::Type p_type, const StringName &p_value);
//static Variant get_constant_value(Variant::Type p_type, const StringName &p_value, bool *r_valid = nullptr);

//typedef String (*ObjectDeConstruct)(const Variant &p_object, void *ud);
//typedef void (*ObjectConstruct)(const String &p_text, void *ud, Variant &r_value);

//String get_construct_string() const;
//static void construct_from_string(const String &p_string, Variant &r_value, ObjectConstruct p_obj_construct = nullptr, void *p_construct_ud = nullptr);

pandemonium_bool GDAPI pandemonium_variant_as_bool(const pandemonium_variant *p_self);
uint64_t GDAPI pandemonium_variant_as_uint(const pandemonium_variant *p_self);
int64_t GDAPI pandemonium_variant_as_int(const pandemonium_variant *p_self);
double GDAPI pandemonium_variant_as_real(const pandemonium_variant *p_self);
pandemonium_string GDAPI pandemonium_variant_as_string(const pandemonium_variant *p_self);
pandemonium_string_name GDAPI pandemonium_variant_as_string_name(const pandemonium_variant *p_self);
pandemonium_vector2 GDAPI pandemonium_variant_as_vector2(const pandemonium_variant *p_self);
pandemonium_vector2i GDAPI pandemonium_variant_as_vector2i(const pandemonium_variant *p_self);
pandemonium_rect2 GDAPI pandemonium_variant_as_rect2(const pandemonium_variant *p_self);
pandemonium_rect2i GDAPI pandemonium_variant_as_rect2i(const pandemonium_variant *p_self);
pandemonium_vector3 GDAPI pandemonium_variant_as_vector3(const pandemonium_variant *p_self);
pandemonium_vector3i GDAPI pandemonium_variant_as_vector3i(const pandemonium_variant *p_self);
pandemonium_vector4 GDAPI pandemonium_variant_as_vector4(const pandemonium_variant *p_self);
pandemonium_vector4i GDAPI pandemonium_variant_as_vector4i(const pandemonium_variant *p_self);
pandemonium_transform2d GDAPI pandemonium_variant_as_transform2d(const pandemonium_variant *p_self);
pandemonium_plane GDAPI pandemonium_variant_as_plane(const pandemonium_variant *p_self);
pandemonium_quaternion GDAPI pandemonium_variant_as_quaternion(const pandemonium_variant *p_self);
pandemonium_aabb GDAPI pandemonium_variant_as_aabb(const pandemonium_variant *p_self);
pandemonium_basis GDAPI pandemonium_variant_as_basis(const pandemonium_variant *p_self);
pandemonium_transform GDAPI pandemonium_variant_as_transform(const pandemonium_variant *p_self);
pandemonium_projection GDAPI pandemonium_variant_as_projection(const pandemonium_variant *p_self);
pandemonium_color GDAPI pandemonium_variant_as_color(const pandemonium_variant *p_self);
pandemonium_node_path GDAPI pandemonium_variant_as_node_path(const pandemonium_variant *p_self);
pandemonium_rid GDAPI pandemonium_variant_as_rid(const pandemonium_variant *p_self);
pandemonium_object GDAPI *pandemonium_variant_as_object(const pandemonium_variant *p_self);
pandemonium_dictionary GDAPI pandemonium_variant_as_dictionary(const pandemonium_variant *p_self);
pandemonium_array GDAPI pandemonium_variant_as_array(const pandemonium_variant *p_self);
pandemonium_pool_byte_array GDAPI pandemonium_variant_as_pool_byte_array(const pandemonium_variant *p_self);
pandemonium_pool_int_array GDAPI pandemonium_variant_as_pool_int_array(const pandemonium_variant *p_self);
pandemonium_pool_real_array GDAPI pandemonium_variant_as_pool_real_array(const pandemonium_variant *p_self);
pandemonium_pool_string_array GDAPI pandemonium_variant_as_pool_string_array(const pandemonium_variant *p_self);
pandemonium_pool_vector2_array GDAPI pandemonium_variant_as_pool_vector2_array(const pandemonium_variant *p_self);
pandemonium_pool_vector2i_array GDAPI pandemonium_variant_as_pool_vector2i_array(const pandemonium_variant *p_self);
pandemonium_pool_vector3_array GDAPI pandemonium_variant_as_pool_vector3_array(const pandemonium_variant *p_self);
pandemonium_pool_vector3i_array GDAPI pandemonium_variant_as_pool_vector3i_array(const pandemonium_variant *p_self);
pandemonium_pool_vector4_array GDAPI pandemonium_variant_as_pool_vector4_array(const pandemonium_variant *p_self);
pandemonium_pool_vector4i_array GDAPI pandemonium_variant_as_pool_vector4i_array(const pandemonium_variant *p_self);
pandemonium_pool_color_array GDAPI pandemonium_variant_as_pool_color_array(const pandemonium_variant *p_self);

void GDAPI pandemonium_variant_new_copy(pandemonium_variant *r_dest, const pandemonium_variant *p_src);

void GDAPI pandemonium_variant_new_nil(pandemonium_variant *r_dest);
void GDAPI pandemonium_variant_new_bool(pandemonium_variant *r_dest, const pandemonium_bool p_b);
void GDAPI pandemonium_variant_new_uint(pandemonium_variant *r_dest, const uint64_t p_i);
void GDAPI pandemonium_variant_new_int(pandemonium_variant *r_dest, const int64_t p_i);
void GDAPI pandemonium_variant_new_real(pandemonium_variant *r_dest, const double p_r);
void GDAPI pandemonium_variant_new_string(pandemonium_variant *r_dest, const pandemonium_string *p_s);
void GDAPI pandemonium_variant_new_string_name(pandemonium_variant *r_dest, const pandemonium_string_name *p_s);
void GDAPI pandemonium_variant_new_vector2(pandemonium_variant *r_dest, const pandemonium_vector2 *p_v2);
void GDAPI pandemonium_variant_new_vector2i(pandemonium_variant *r_dest, const pandemonium_vector2i *p_v2i);
void GDAPI pandemonium_variant_new_rect2(pandemonium_variant *r_dest, const pandemonium_rect2 *p_rect2);
void GDAPI pandemonium_variant_new_rect2i(pandemonium_variant *r_dest, const pandemonium_rect2i *p_rect2i);
void GDAPI pandemonium_variant_new_vector3(pandemonium_variant *r_dest, const pandemonium_vector3 *p_v3);
void GDAPI pandemonium_variant_new_vector3i(pandemonium_variant *r_dest, const pandemonium_vector3i *p_v3i);
void GDAPI pandemonium_variant_new_vector4(pandemonium_variant *r_dest, const pandemonium_vector4 *p_v4);
void GDAPI pandemonium_variant_new_vector4i(pandemonium_variant *r_dest, const pandemonium_vector4i *p_v4i);
void GDAPI pandemonium_variant_new_transform2d(pandemonium_variant *r_dest, const pandemonium_transform2d *p_t2d);
void GDAPI pandemonium_variant_new_plane(pandemonium_variant *r_dest, const pandemonium_plane *p_plane);
void GDAPI pandemonium_variant_new_quaternion(pandemonium_variant *r_dest, const pandemonium_quaternion *p_quaternion);
void GDAPI pandemonium_variant_new_aabb(pandemonium_variant *r_dest, const pandemonium_aabb *p_aabb);
void GDAPI pandemonium_variant_new_basis(pandemonium_variant *r_dest, const pandemonium_basis *p_basis);
void GDAPI pandemonium_variant_new_transform(pandemonium_variant *r_dest, const pandemonium_transform *p_trans);
void GDAPI pandemonium_variant_new_projection(pandemonium_variant *r_dest, const pandemonium_projection *p_proj);
void GDAPI pandemonium_variant_new_color(pandemonium_variant *r_dest, const pandemonium_color *p_color);
void GDAPI pandemonium_variant_new_node_path(pandemonium_variant *r_dest, const pandemonium_node_path *p_np);
void GDAPI pandemonium_variant_new_rid(pandemonium_variant *r_dest, const pandemonium_rid *p_rid);
void GDAPI pandemonium_variant_new_object(pandemonium_variant *r_dest, const pandemonium_object *p_obj);
void GDAPI pandemonium_variant_new_dictionary(pandemonium_variant *r_dest, const pandemonium_dictionary *p_dict);
void GDAPI pandemonium_variant_new_array(pandemonium_variant *r_dest, const pandemonium_array *p_arr);
void GDAPI pandemonium_variant_new_pool_byte_array(pandemonium_variant *r_dest, const pandemonium_pool_byte_array *p_pba);
void GDAPI pandemonium_variant_new_pool_int_array(pandemonium_variant *r_dest, const pandemonium_pool_int_array *p_pia);
void GDAPI pandemonium_variant_new_pool_real_array(pandemonium_variant *r_dest, const pandemonium_pool_real_array *p_pra);
void GDAPI pandemonium_variant_new_pool_string_array(pandemonium_variant *r_dest, const pandemonium_pool_string_array *p_psa);
void GDAPI pandemonium_variant_new_pool_vector2_array(pandemonium_variant *r_dest, const pandemonium_pool_vector2_array *p_pv2a);
void GDAPI pandemonium_variant_new_pool_vector2i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector2i_array *p_pv2ia);
void GDAPI pandemonium_variant_new_pool_vector3_array(pandemonium_variant *r_dest, const pandemonium_pool_vector3_array *p_pv3a);
void GDAPI pandemonium_variant_new_pool_vector3i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector3i_array *p_pv3ia);
void GDAPI pandemonium_variant_new_pool_vector4_array(pandemonium_variant *r_dest, const pandemonium_pool_vector4_array *p_pv4a);
void GDAPI pandemonium_variant_new_pool_vector4i_array(pandemonium_variant *r_dest, const pandemonium_pool_vector4i_array *p_pv4ia);
void GDAPI pandemonium_variant_new_pool_color_array(pandemonium_variant *r_dest, const pandemonium_pool_color_array *p_pca);

void GDAPI pandemonium_variant_destroy(pandemonium_variant *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VARIANT_H
