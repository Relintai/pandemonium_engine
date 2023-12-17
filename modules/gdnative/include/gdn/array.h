#ifndef GDNATIVE_ARRAY_H
#define GDNATIVE_ARRAY_H

/*************************************************************************/
/*  array.h                                                              */
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

#define PANDEMONIUM_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_ARRAY_SIZE];
} pandemonium_array;
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

pandemonium_variant GDAPI *pandemonium_array_operator_index(pandemonium_array *p_self, const pandemonium_int p_idx);
const pandemonium_variant GDAPI *pandemonium_array_operator_index_const(const pandemonium_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_array_set(pandemonium_array *p_self, const pandemonium_int p_idx, const pandemonium_variant *p_value);
pandemonium_variant GDAPI pandemonium_array_get(const pandemonium_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_array_size(const pandemonium_array *p_self);
pandemonium_bool GDAPI pandemonium_array_empty(const pandemonium_array *p_self);
void GDAPI pandemonium_array_clear(pandemonium_array *p_self);

pandemonium_bool GDAPI pandemonium_array_deep_equal(const pandemonium_array *p_self, const pandemonium_array *p_array);
pandemonium_bool GDAPI pandemonium_array_deep_equal_recursion_count(const pandemonium_array *p_self, const pandemonium_array *p_array, pandemonium_int p_recursion_count);
pandemonium_bool GDAPI pandemonium_array_operator_equals(pandemonium_array *p_self, const pandemonium_array *p_array);

pandemonium_int GDAPI pandemonium_array_hash(const pandemonium_array *p_self);
//bool operator=(const Array &p_array) const;

void GDAPI pandemonium_array_push_back(pandemonium_array *p_self, const pandemonium_variant *p_value);
void GDAPI pandemonium_array_append(pandemonium_array *p_self, const pandemonium_variant *p_value);
void GDAPI pandemonium_array_append_array(pandemonium_array *p_self, const pandemonium_array *p_array);
void GDAPI pandemonium_array_resize(pandemonium_array *p_self, const pandemonium_int p_size);

void GDAPI pandemonium_array_insert(pandemonium_array *p_self, const pandemonium_int p_pos, const pandemonium_variant *p_value);
void GDAPI pandemonium_array_remove(pandemonium_array *p_self, const pandemonium_int p_idx);
void GDAPI pandemonium_array_fill(pandemonium_array *p_self, const pandemonium_variant *p_value);

pandemonium_variant GDAPI pandemonium_array_front(const pandemonium_array *p_self);
pandemonium_variant GDAPI pandemonium_array_back(const pandemonium_array *p_self);

void GDAPI pandemonium_array_sort(pandemonium_array *p_self);
void GDAPI pandemonium_array_sort_custom(pandemonium_array *p_self, pandemonium_object *p_obj, const pandemonium_string *p_func);
void GDAPI pandemonium_array_shuffle(pandemonium_array *p_self);
//before variant
pandemonium_int GDAPI pandemonium_array_bsearch(pandemonium_array *p_self, const pandemonium_variant *p_value, const pandemonium_bool p_before);
pandemonium_int GDAPI pandemonium_array_bsearch_custom(pandemonium_array *p_self, const pandemonium_variant *p_value, pandemonium_object *p_obj, const pandemonium_string *p_func, const pandemonium_bool p_before);

void GDAPI pandemonium_array_invert(pandemonium_array *p_self);

pandemonium_int GDAPI pandemonium_array_find(const pandemonium_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_array_find_from(const pandemonium_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_array_rfind(const pandemonium_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_array_rfind_from(const pandemonium_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_array_find_last(const pandemonium_array *p_self, const pandemonium_variant *p_what);
pandemonium_int GDAPI pandemonium_array_count(const pandemonium_array *p_self, const pandemonium_variant *p_value);
pandemonium_bool GDAPI pandemonium_array_has(const pandemonium_array *p_self, const pandemonium_variant *p_value);

void GDAPI pandemonium_array_erase(pandemonium_array *p_self, const pandemonium_variant *p_value);

void GDAPI pandemonium_array_push_front(pandemonium_array *p_self, const pandemonium_variant *p_value);
pandemonium_variant GDAPI pandemonium_array_pop_back(pandemonium_array *p_self);
pandemonium_variant GDAPI pandemonium_array_pop_front(pandemonium_array *p_self);
pandemonium_variant GDAPI pandemonium_array_pop_at(pandemonium_array *p_self, pandemonium_int p_pos);

pandemonium_array GDAPI pandemonium_array_duplicate(const pandemonium_array *p_self);
pandemonium_array GDAPI pandemonium_array_duplicate_deep(const pandemonium_array *p_self, const pandemonium_bool p_deep);

pandemonium_array GDAPI pandemonium_array_slice(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end);
pandemonium_array GDAPI pandemonium_array_slice_step(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step);
pandemonium_array GDAPI pandemonium_array_slice_step_deep(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step, const pandemonium_bool p_deep);

pandemonium_variant GDAPI pandemonium_array_min(const pandemonium_array *p_self);
pandemonium_variant GDAPI pandemonium_array_max(const pandemonium_array *p_self);

void GDAPI pandemonium_array_new(pandemonium_array *r_dest);
void GDAPI pandemonium_array_new_copy(pandemonium_array *r_dest, const pandemonium_array *p_src);
void GDAPI pandemonium_array_new_pool_byte_array(pandemonium_array *r_dest, const pandemonium_pool_byte_array *p_pba);
void GDAPI pandemonium_array_new_pool_int_array(pandemonium_array *r_dest, const pandemonium_pool_int_array *p_pia);
void GDAPI pandemonium_array_new_pool_real_array(pandemonium_array *r_dest, const pandemonium_pool_real_array *p_pra);
void GDAPI pandemonium_array_new_pool_string_array(pandemonium_array *r_dest, const pandemonium_pool_string_array *p_psa);
void GDAPI pandemonium_array_new_pool_vector2_array(pandemonium_array *r_dest, const pandemonium_pool_vector2_array *p_pv2a);
void GDAPI pandemonium_array_new_pool_vector2i_array(pandemonium_array *r_dest, const pandemonium_pool_vector2i_array *p_pv2a);
void GDAPI pandemonium_array_new_pool_vector3_array(pandemonium_array *r_dest, const pandemonium_pool_vector3_array *p_pv3a);
void GDAPI pandemonium_array_new_pool_vector3i_array(pandemonium_array *r_dest, const pandemonium_pool_vector3i_array *p_pv3a);
void GDAPI pandemonium_array_new_pool_vector4_array(pandemonium_array *r_dest, const pandemonium_pool_vector4_array *p_pv3a);
void GDAPI pandemonium_array_new_pool_vector4i_array(pandemonium_array *r_dest, const pandemonium_pool_vector4i_array *p_pv3a);
void GDAPI pandemonium_array_new_pool_color_array(pandemonium_array *r_dest, const pandemonium_pool_color_array *p_pca);
void GDAPI pandemonium_array_destroy(pandemonium_array *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_ARRAY_H
