#ifndef GDNATIVE_TYPED_ARRAY_H
#define GDNATIVE_TYPED_ARRAY_H

/*************************************************************************/
/*  typed_typed_array.h                                                        */
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

#define PANDEMONIUM_TYPED_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_TYPED_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_TYPED_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_TYPED_ARRAY_SIZE];
} pandemonium_typed_array;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/pool_arrays.h>
#include <gdn/variant.h>

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

const pandemonium_variant GDAPI *pandemonium_typed_array_operator_index_const(const pandemonium_typed_array *p_self, const pandemonium_int p_idx);

pandemonium_bool GDAPI pandemonium_typed_array_set(pandemonium_typed_array *p_self, const pandemonium_int p_idx, const pandemonium_variant *p_value);
pandemonium_variant GDAPI pandemonium_typed_array_get(const pandemonium_typed_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_typed_array_size(const pandemonium_typed_array *p_self);
pandemonium_bool GDAPI pandemonium_typed_array_empty(const pandemonium_typed_array *p_self);
void GDAPI pandemonium_typed_array_clear(pandemonium_typed_array *p_self);

pandemonium_bool GDAPI pandemonium_typed_array_deep_equal(const pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array);
pandemonium_bool GDAPI pandemonium_typed_array_deep_equal_recursion_count(const pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array, pandemonium_int p_recursion_count);
pandemonium_bool GDAPI pandemonium_typed_array_operator_equals(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array);

pandemonium_int GDAPI pandemonium_typed_array_hash(const pandemonium_typed_array *p_self);
//bool operator=(const Array &p_typed_array) const;

pandemonium_bool GDAPI pandemonium_typed_array_push_back(pandemonium_typed_array *p_self, const pandemonium_variant *p_value);
pandemonium_bool GDAPI pandemonium_typed_array_append(pandemonium_typed_array *p_self, const pandemonium_variant *p_value);
pandemonium_bool GDAPI pandemonium_typed_array_append_array(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array);
pandemonium_bool GDAPI pandemonium_typed_array_append_from(pandemonium_typed_array *p_self, const pandemonium_variant *p_array);
void GDAPI pandemonium_typed_array_resize(pandemonium_typed_array *p_self, const pandemonium_int p_size);

pandemonium_bool GDAPI pandemonium_typed_array_insert(pandemonium_typed_array *p_self, const pandemonium_int p_pos, const pandemonium_variant *p_value);
void GDAPI pandemonium_typed_array_remove(pandemonium_typed_array *p_self, const pandemonium_int p_idx);
pandemonium_bool GDAPI pandemonium_typed_array_fill(pandemonium_typed_array *p_self, const pandemonium_variant *p_value);

pandemonium_variant GDAPI pandemonium_typed_array_front(const pandemonium_typed_array *p_self);
pandemonium_variant GDAPI pandemonium_typed_array_back(const pandemonium_typed_array *p_self);

void GDAPI pandemonium_typed_array_sort(pandemonium_typed_array *p_self);
void GDAPI pandemonium_typed_array_sort_custom(pandemonium_typed_array *p_self, pandemonium_object *p_obj, const pandemonium_string *p_func);
void GDAPI pandemonium_typed_array_shuffle(pandemonium_typed_array *p_self);
//before variant
pandemonium_int GDAPI pandemonium_typed_array_bsearch(pandemonium_typed_array *p_self, const pandemonium_variant *p_value, const pandemonium_bool p_before);
pandemonium_int GDAPI pandemonium_typed_array_bsearch_custom(pandemonium_typed_array *p_self, const pandemonium_variant *p_value, pandemonium_object *p_obj, const pandemonium_string *p_func, const pandemonium_bool p_before);

void GDAPI pandemonium_typed_array_invert(pandemonium_typed_array *p_self);

pandemonium_int GDAPI pandemonium_typed_array_find(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_typed_array_find_from(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_typed_array_rfind(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_typed_array_rfind_from(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_typed_array_find_last(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_typed_array_count(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value);
pandemonium_bool GDAPI pandemonium_typed_array_has(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value);

void GDAPI pandemonium_typed_array_erase(pandemonium_typed_array *p_self, const pandemonium_variant *p_value);

pandemonium_bool GDAPI pandemonium_typed_array_push_front(pandemonium_typed_array *p_self, const pandemonium_variant *p_value);
pandemonium_variant GDAPI pandemonium_typed_array_pop_back(pandemonium_typed_array *p_self);
pandemonium_variant GDAPI pandemonium_typed_array_pop_front(pandemonium_typed_array *p_self);
pandemonium_variant GDAPI pandemonium_typed_array_pop_at(pandemonium_typed_array *p_self, pandemonium_int p_pos);

pandemonium_typed_array GDAPI pandemonium_typed_array_duplicate(const pandemonium_typed_array *p_self);
pandemonium_typed_array GDAPI pandemonium_typed_array_duplicate_deep(const pandemonium_typed_array *p_self, const pandemonium_bool p_deep);

pandemonium_typed_array GDAPI pandemonium_typed_array_slice(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end);
pandemonium_typed_array GDAPI pandemonium_typed_array_slice_step(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step);
pandemonium_typed_array GDAPI pandemonium_typed_array_slice_step_deep(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step, const pandemonium_bool p_deep);

pandemonium_variant GDAPI pandemonium_typed_array_min(const pandemonium_typed_array *p_self);
pandemonium_variant GDAPI pandemonium_typed_array_max(const pandemonium_typed_array *p_self);

pandemonium_string GDAPI pandemonium_typed_array_get_typename_string(const pandemonium_typed_array *p_self);

pandemonium_int GDAPI pandemonium_typed_array_get_variant_type(const pandemonium_typed_array *p_self);
void GDAPI pandemonium_typed_array_set_variant_type(pandemonium_typed_array *p_self, const pandemonium_int p_variant_type);

pandemonium_string_name GDAPI pandemonium_typed_array_get_object_class_name(const pandemonium_typed_array *p_self);
void GDAPI pandemonium_typed_array_set_object_class_name(pandemonium_typed_array *p_self, const pandemonium_string_name *p_object_type_name);

void GDAPI pandemonium_typed_array_set_type_from_name(pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name);
void GDAPI pandemonium_typed_array_set_type_from_variant(pandemonium_typed_array *p_self, const pandemonium_variant *p_variant);
void GDAPI pandemonium_typed_array_set_type_from_array_element(pandemonium_typed_array *p_self, const pandemonium_variant *p_array);
void GDAPI pandemonium_typed_array_set_type_from(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_array);

pandemonium_bool GDAPI pandemonium_typed_array_validate_type_name(const pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name);
pandemonium_bool GDAPI pandemonium_typed_array_validate_object_type_name(const pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name);
pandemonium_bool GDAPI pandemonium_typed_array_can_take_variant(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value);

void GDAPI pandemonium_typed_array_new(pandemonium_typed_array *r_dest);
void GDAPI pandemonium_typed_array_new_copy(pandemonium_typed_array *r_dest, const pandemonium_typed_array *p_src);
void GDAPI pandemonium_typed_array_new_pool_byte_array(pandemonium_typed_array *r_dest, const pandemonium_pool_byte_array *p_pba);
void GDAPI pandemonium_typed_array_new_pool_int_array(pandemonium_typed_array *r_dest, const pandemonium_pool_int_array *p_pia);
void GDAPI pandemonium_typed_array_new_pool_real_array(pandemonium_typed_array *r_dest, const pandemonium_pool_real_array *p_pra);
void GDAPI pandemonium_typed_array_new_pool_string_array(pandemonium_typed_array *r_dest, const pandemonium_pool_string_array *p_psa);
void GDAPI pandemonium_typed_array_new_pool_vector2_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector2_array *p_pv2a);
void GDAPI pandemonium_typed_array_new_pool_vector2i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector2i_array *p_pv2a);
void GDAPI pandemonium_typed_array_new_pool_vector3_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector3_array *p_pv3a);
void GDAPI pandemonium_typed_array_new_pool_vector3i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector3i_array *p_pv3a);
void GDAPI pandemonium_typed_array_new_pool_vector4_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector4_array *p_pv3a);
void GDAPI pandemonium_typed_array_new_pool_vector4i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector4i_array *p_pv3a);
void GDAPI pandemonium_typed_array_new_pool_color_array(pandemonium_typed_array *r_dest, const pandemonium_pool_color_array *p_pca);
void GDAPI pandemonium_typed_array_destroy(pandemonium_typed_array *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_TYPED_ARRAY_H
