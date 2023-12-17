#ifndef GDNATIVE_DICTIONARY_H
#define GDNATIVE_DICTIONARY_H

/*************************************************************************/
/*  dictionary.h                                                         */
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

#define PANDEMONIUM_DICTIONARY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_DICTIONARY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_DICTIONARY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_DICTIONARY_SIZE];
} pandemonium_dictionary;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/array.h>
#include <gdn/gdnative.h>
#include <gdn/variant.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_variant GDAPI *pandemonium_dictionary_operator_index(pandemonium_dictionary *p_self, const pandemonium_variant *p_key);
const pandemonium_variant GDAPI *pandemonium_dictionary_operator_index_const(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key);

pandemonium_variant GDAPI pandemonium_dictionary_get(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key);
void GDAPI pandemonium_dictionary_set(pandemonium_dictionary *p_self, const pandemonium_variant *p_key, const pandemonium_variant *p_value);
pandemonium_variant GDAPI pandemonium_dictionary_get_with_default(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key, const pandemonium_variant *p_default);

pandemonium_int GDAPI pandemonium_dictionary_size(const pandemonium_dictionary *p_self);
pandemonium_bool GDAPI pandemonium_dictionary_empty(const pandemonium_dictionary *p_self);
void GDAPI pandemonium_dictionary_clear(pandemonium_dictionary *p_self);
void GDAPI pandemonium_dictionary_merge(pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary);
void GDAPI pandemonium_dictionary_merge_overwrite(pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary, const pandemonium_bool p_overwrite);

pandemonium_bool GDAPI pandemonium_dictionary_has(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key);
pandemonium_bool GDAPI pandemonium_dictionary_has_all(const pandemonium_dictionary *p_self, const pandemonium_array *p_keys);

void GDAPI pandemonium_dictionary_erase(pandemonium_dictionary *p_self, const pandemonium_variant *p_key);
pandemonium_bool GDAPI pandemonium_dictionary_erase_with_return(pandemonium_dictionary *p_self, const pandemonium_variant *p_key);

pandemonium_bool GDAPI pandemonium_dictionary_deep_equal(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary);
pandemonium_bool GDAPI pandemonium_dictionary_deep_equal_recursion_count(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary, pandemonium_int p_recursion_count);
pandemonium_bool GDAPI pandemonium_dictionary_operator_equal(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_b);

pandemonium_int GDAPI pandemonium_dictionary_hash(const pandemonium_dictionary *p_self);

pandemonium_variant GDAPI *pandemonium_dictionary_next(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key);

pandemonium_array GDAPI pandemonium_dictionary_keys(const pandemonium_dictionary *p_self);
pandemonium_array GDAPI pandemonium_dictionary_values(const pandemonium_dictionary *p_self);

pandemonium_dictionary GDAPI pandemonium_dictionary_duplicate(const pandemonium_dictionary *p_self, const pandemonium_bool p_deep);

pandemonium_string GDAPI pandemonium_dictionary_to_json(const pandemonium_dictionary *p_self);

void GDAPI pandemonium_dictionary_new(pandemonium_dictionary *r_dest);
void GDAPI pandemonium_dictionary_new_copy(pandemonium_dictionary *r_dest, const pandemonium_dictionary *p_src);
void GDAPI pandemonium_dictionary_destroy(pandemonium_dictionary *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_DICTIONARY_H
