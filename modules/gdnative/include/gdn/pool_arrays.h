#ifndef GDNATIVE_POOL_ARRAYS_H
#define GDNATIVE_POOL_ARRAYS_H

/*************************************************************************/
/*  pool_arrays.h                                                        */
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

/////// Read Access

#define PANDEMONIUM_POOL_ARRAY_READ_ACCESS_SIZE 1

typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_ARRAY_READ_ACCESS_SIZE];
} pandemonium_pool_array_read_access;

typedef pandemonium_pool_array_read_access pandemonium_pool_byte_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_int_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_real_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_string_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector2_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector2i_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector3_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector3i_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector4_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_vector4i_array_read_access;
typedef pandemonium_pool_array_read_access pandemonium_pool_color_array_read_access;

/////// Write Access

#define PANDEMONIUM_POOL_ARRAY_WRITE_ACCESS_SIZE 1

typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_ARRAY_WRITE_ACCESS_SIZE];
} pandemonium_pool_array_write_access;

typedef pandemonium_pool_array_write_access pandemonium_pool_byte_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_int_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_real_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_string_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector2_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector2i_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector3_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector3i_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector4_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_vector4i_array_write_access;
typedef pandemonium_pool_array_write_access pandemonium_pool_color_array_write_access;

/////// PoolByteArray

#define PANDEMONIUM_POOL_BYTE_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_BYTE_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_BYTE_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_BYTE_ARRAY_SIZE];
} pandemonium_pool_byte_array;
#endif

/////// PoolIntArray

#define PANDEMONIUM_POOL_INT_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_INT_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_INT_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_INT_ARRAY_SIZE];
} pandemonium_pool_int_array;
#endif

/////// PoolRealArray

#define PANDEMONIUM_POOL_REAL_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_REAL_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_REAL_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_REAL_ARRAY_SIZE];
} pandemonium_pool_real_array;
#endif

/////// PoolStringArray

#define PANDEMONIUM_POOL_STRING_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_STRING_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_STRING_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_STRING_ARRAY_SIZE];
} pandemonium_pool_string_array;
#endif

/////// PoolVector2Array

#define PANDEMONIUM_POOL_VECTOR2_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR2_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR2_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR2_ARRAY_SIZE];
} pandemonium_pool_vector2_array;
#endif

/////// PoolVector2iArray

#define PANDEMONIUM_POOL_VECTOR2I_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR2I_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR2I_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR2I_ARRAY_SIZE];
} pandemonium_pool_vector2i_array;
#endif

/////// PoolVector3Array

#define PANDEMONIUM_POOL_VECTOR3_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR3_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR3_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR3_ARRAY_SIZE];
} pandemonium_pool_vector3_array;
#endif

/////// PoolVector3iArray

#define PANDEMONIUM_POOL_VECTOR3I_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR3I_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR3I_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR3I_ARRAY_SIZE];
} pandemonium_pool_vector3i_array;
#endif

/////// PoolVector4Array

#define PANDEMONIUM_POOL_VECTOR4_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR4_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR4_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR4_ARRAY_SIZE];
} pandemonium_pool_vector4_array;
#endif

/////// PoolVector4iArray

#define PANDEMONIUM_POOL_VECTOR4I_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR4I_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_VECTOR4I_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_VECTOR4I_ARRAY_SIZE];
} pandemonium_pool_vector4i_array;
#endif

/////// PoolColorArray

#define PANDEMONIUM_POOL_COLOR_ARRAY_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_COLOR_ARRAY_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_POOL_COLOR_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_POOL_COLOR_ARRAY_SIZE];
} pandemonium_pool_color_array;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/array.h>
#include <gdn/color.h>
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

// byte

pandemonium_pool_byte_array_read_access GDAPI *pandemonium_pool_byte_array_read(const pandemonium_pool_byte_array *p_self);
pandemonium_pool_byte_array_write_access GDAPI *pandemonium_pool_byte_array_write(pandemonium_pool_byte_array *p_self);

void GDAPI pandemonium_pool_byte_array_remove(pandemonium_pool_byte_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_byte_array_size(const pandemonium_pool_byte_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_byte_array_empty(const pandemonium_pool_byte_array *p_self);

void GDAPI pandemonium_pool_byte_array_set(pandemonium_pool_byte_array *p_self, const pandemonium_int p_idx, const uint8_t p_data);
uint8_t GDAPI pandemonium_pool_byte_array_get(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_byte_array_fill(pandemonium_pool_byte_array *p_self, const pandemonium_int p_val);
void GDAPI pandemonium_pool_byte_array_push_back(pandemonium_pool_byte_array *p_self, const uint8_t p_data);
void GDAPI pandemonium_pool_byte_array_append(pandemonium_pool_byte_array *p_self, const uint8_t p_data);
void GDAPI pandemonium_pool_byte_array_append_array(pandemonium_pool_byte_array *p_self, const pandemonium_pool_byte_array *p_array);

pandemonium_pool_byte_array GDAPI pandemonium_pool_byte_array_subarray(const pandemonium_pool_byte_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_byte_array_insert(pandemonium_pool_byte_array *p_self, const pandemonium_int p_idx, const uint8_t p_data);

pandemonium_bool GDAPI pandemonium_pool_byte_array_contains(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_byte_array_find(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_byte_array_find_from(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_byte_array_rfind(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_byte_array_rfind_from(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_byte_array_count(const pandemonium_pool_byte_array *p_self, const pandemonium_int p_val);
pandemonium_bool GDAPI pandemonium_pool_byte_array_has(const pandemonium_pool_byte_array *p_self, const uint8_t p_data);

pandemonium_bool GDAPI pandemonium_pool_byte_array_is_locked(const pandemonium_pool_byte_array *p_self);

void GDAPI pandemonium_pool_byte_array_resize(pandemonium_pool_byte_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_byte_array_clear(pandemonium_pool_byte_array *p_self);

void GDAPI pandemonium_pool_byte_array_invert(pandemonium_pool_byte_array *p_self);
void GDAPI pandemonium_pool_byte_array_sort(pandemonium_pool_byte_array *p_self);

void GDAPI pandemonium_pool_byte_array_new(pandemonium_pool_byte_array *r_dest);
void GDAPI pandemonium_pool_byte_array_new_copy(pandemonium_pool_byte_array *r_dest, const pandemonium_pool_byte_array *p_src);
void GDAPI pandemonium_pool_byte_array_new_with_array(pandemonium_pool_byte_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_byte_array_destroy(pandemonium_pool_byte_array *p_self);

// int

pandemonium_pool_int_array_read_access GDAPI *pandemonium_pool_int_array_read(const pandemonium_pool_int_array *p_self);
pandemonium_pool_int_array_write_access GDAPI *pandemonium_pool_int_array_write(pandemonium_pool_int_array *p_self);

void GDAPI pandemonium_pool_int_array_remove(pandemonium_pool_int_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_int_array_size(const pandemonium_pool_int_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_int_array_empty(const pandemonium_pool_int_array *p_self);

void GDAPI pandemonium_pool_int_array_set(pandemonium_pool_int_array *p_self, const pandemonium_int p_idx, const pandemonium_int p_data);
pandemonium_int GDAPI pandemonium_pool_int_array_get(const pandemonium_pool_int_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_int_array_fill(pandemonium_pool_int_array *p_self, const pandemonium_int p_val);
void GDAPI pandemonium_pool_int_array_push_back(pandemonium_pool_int_array *p_self, const pandemonium_int p_data);
void GDAPI pandemonium_pool_int_array_append(pandemonium_pool_int_array *p_self, const pandemonium_int p_data);
void GDAPI pandemonium_pool_int_array_append_array(pandemonium_pool_int_array *p_self, const pandemonium_pool_int_array *p_array);

pandemonium_pool_int_array GDAPI pandemonium_pool_int_array_subarray(const pandemonium_pool_int_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_int_array_insert(pandemonium_pool_int_array *p_self, const pandemonium_int p_idx, const pandemonium_int p_data);

pandemonium_string GDAPI pandemonium_pool_string_array_join(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_delimiter);

pandemonium_bool GDAPI pandemonium_pool_int_array_contains(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_int_array_find(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_int_array_find_from(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_int_array_rfind(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val);
pandemonium_int GDAPI pandemonium_pool_int_array_rfind_from(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_int_array_count(const pandemonium_pool_int_array *p_self, const pandemonium_int p_val);
pandemonium_bool GDAPI pandemonium_pool_int_array_has(const pandemonium_pool_int_array *p_self, const pandemonium_int p_data);

pandemonium_bool GDAPI pandemonium_pool_int_array_is_locked(const pandemonium_pool_int_array *p_self);

void GDAPI pandemonium_pool_int_array_resize(pandemonium_pool_int_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_int_array_clear(pandemonium_pool_int_array *p_self);

void GDAPI pandemonium_pool_int_array_invert(pandemonium_pool_int_array *p_self);
void GDAPI pandemonium_pool_int_array_sort(pandemonium_pool_int_array *p_self);

void GDAPI pandemonium_pool_int_array_new(pandemonium_pool_int_array *r_dest);
void GDAPI pandemonium_pool_int_array_new_copy(pandemonium_pool_int_array *r_dest, const pandemonium_pool_int_array *p_src);
void GDAPI pandemonium_pool_int_array_new_with_array(pandemonium_pool_int_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_int_array_destroy(pandemonium_pool_int_array *p_self);

// real

pandemonium_pool_real_array_read_access GDAPI *pandemonium_pool_real_array_read(const pandemonium_pool_real_array *p_self);
pandemonium_pool_real_array_write_access GDAPI *pandemonium_pool_real_array_write(pandemonium_pool_real_array *p_self);

void GDAPI pandemonium_pool_real_array_remove(pandemonium_pool_real_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_real_array_size(const pandemonium_pool_real_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_real_array_empty(const pandemonium_pool_real_array *p_self);

void GDAPI pandemonium_pool_real_array_set(pandemonium_pool_real_array *p_self, const pandemonium_int p_idx, const pandemonium_real p_data);
pandemonium_real GDAPI pandemonium_pool_real_array_get(const pandemonium_pool_real_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_real_array_fill(pandemonium_pool_real_array *p_self, const pandemonium_real p_val);
void GDAPI pandemonium_pool_real_array_push_back(pandemonium_pool_real_array *p_self, const pandemonium_real p_data);
void GDAPI pandemonium_pool_real_array_append(pandemonium_pool_real_array *p_self, const pandemonium_real p_data);
void GDAPI pandemonium_pool_real_array_append_array(pandemonium_pool_real_array *p_self, const pandemonium_pool_real_array *p_array);

pandemonium_pool_real_array GDAPI pandemonium_pool_real_array_subarray(const pandemonium_pool_real_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_real_array_insert(pandemonium_pool_real_array *p_self, const pandemonium_int p_idx, const pandemonium_real p_data);

pandemonium_bool GDAPI pandemonium_pool_real_array_contains(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val);
pandemonium_int GDAPI pandemonium_pool_real_array_find(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val);
pandemonium_int GDAPI pandemonium_pool_real_array_find_from(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_real_array_rfind(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val);
pandemonium_int GDAPI pandemonium_pool_real_array_rfind_from(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_real_array_count(const pandemonium_pool_real_array *p_self, const pandemonium_real p_val);
pandemonium_bool GDAPI pandemonium_pool_real_array_has(const pandemonium_pool_real_array *p_self, const pandemonium_real p_data);

pandemonium_bool GDAPI pandemonium_pool_real_array_is_locked(const pandemonium_pool_real_array *p_self);

void GDAPI pandemonium_pool_real_array_resize(pandemonium_pool_real_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_real_array_clear(pandemonium_pool_real_array *p_self);

void GDAPI pandemonium_pool_real_array_invert(pandemonium_pool_real_array *p_self);
void GDAPI pandemonium_pool_real_array_sort(pandemonium_pool_real_array *p_self);

void GDAPI pandemonium_pool_real_array_new(pandemonium_pool_real_array *r_dest);
void GDAPI pandemonium_pool_real_array_new_copy(pandemonium_pool_real_array *r_dest, const pandemonium_pool_real_array *p_src);
void GDAPI pandemonium_pool_real_array_new_with_array(pandemonium_pool_real_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_real_array_destroy(pandemonium_pool_real_array *p_self);

// String

pandemonium_pool_string_array_read_access GDAPI *pandemonium_pool_string_array_read(const pandemonium_pool_string_array *p_self);
pandemonium_pool_string_array_write_access GDAPI *pandemonium_pool_string_array_write(pandemonium_pool_string_array *p_self);

void GDAPI pandemonium_pool_string_array_remove(pandemonium_pool_string_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_string_array_size(const pandemonium_pool_string_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_string_array_empty(const pandemonium_pool_string_array *p_self);

void GDAPI pandemonium_pool_string_array_set(pandemonium_pool_string_array *p_self, const pandemonium_int p_idx, const pandemonium_string *p_data);
pandemonium_string GDAPI pandemonium_pool_string_array_get(const pandemonium_pool_string_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_string_array_fill(pandemonium_pool_string_array *p_self, const pandemonium_string *p_val);
void GDAPI pandemonium_pool_string_array_push_back(pandemonium_pool_string_array *p_self, const pandemonium_string *p_data);
void GDAPI pandemonium_pool_string_array_append(pandemonium_pool_string_array *p_self, const pandemonium_string *p_data);
void GDAPI pandemonium_pool_string_array_append_array(pandemonium_pool_string_array *p_self, const pandemonium_pool_string_array *p_array);

pandemonium_pool_string_array GDAPI pandemonium_pool_string_array_subarray(const pandemonium_pool_string_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_string_array_insert(pandemonium_pool_string_array *p_self, const pandemonium_int p_idx, const pandemonium_string *p_data);

pandemonium_bool GDAPI pandemonium_pool_string_array_contains(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val);
pandemonium_int GDAPI pandemonium_pool_string_array_find(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val);
pandemonium_int GDAPI pandemonium_pool_string_array_find_from(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_string_array_rfind(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val);
pandemonium_int GDAPI pandemonium_pool_string_array_rfind_from(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_string_array_count(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_val);
pandemonium_bool GDAPI pandemonium_pool_string_array_has(const pandemonium_pool_string_array *p_self, const pandemonium_string *p_data);

pandemonium_bool GDAPI pandemonium_pool_string_array_is_locked(const pandemonium_pool_string_array *p_self);

void GDAPI pandemonium_pool_string_array_resize(pandemonium_pool_string_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_string_array_clear(pandemonium_pool_string_array *p_self);

void GDAPI pandemonium_pool_string_array_invert(pandemonium_pool_string_array *p_self);
void GDAPI pandemonium_pool_string_array_sort(pandemonium_pool_string_array *p_self);

void GDAPI pandemonium_pool_string_array_new(pandemonium_pool_string_array *r_dest);
void GDAPI pandemonium_pool_string_array_new_copy(pandemonium_pool_string_array *r_dest, const pandemonium_pool_string_array *p_src);
void GDAPI pandemonium_pool_string_array_new_with_array(pandemonium_pool_string_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_string_array_destroy(pandemonium_pool_string_array *p_self);

// vector2

pandemonium_pool_vector2_array_read_access GDAPI *pandemonium_pool_vector2_array_read(const pandemonium_pool_vector2_array *p_self);
pandemonium_pool_vector2_array_write_access GDAPI *pandemonium_pool_vector2_array_write(pandemonium_pool_vector2_array *p_self);

void GDAPI pandemonium_pool_vector2_array_remove(pandemonium_pool_vector2_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector2_array_size(const pandemonium_pool_vector2_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector2_array_empty(const pandemonium_pool_vector2_array *p_self);

void GDAPI pandemonium_pool_vector2_array_set(pandemonium_pool_vector2_array *p_self, const pandemonium_int p_idx, const pandemonium_vector2 *p_data);
pandemonium_vector2 GDAPI pandemonium_pool_vector2_array_get(const pandemonium_pool_vector2_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector2_array_fill(pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val);
void GDAPI pandemonium_pool_vector2_array_push_back(pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_data);
void GDAPI pandemonium_pool_vector2_array_append(pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_data);
void GDAPI pandemonium_pool_vector2_array_append_array(pandemonium_pool_vector2_array *p_self, const pandemonium_pool_vector2_array *p_array);

pandemonium_pool_vector2_array GDAPI pandemonium_pool_vector2_array_subarray(const pandemonium_pool_vector2_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector2_array_insert(pandemonium_pool_vector2_array *p_self, const pandemonium_int p_idx, const pandemonium_vector2 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector2_array_contains(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2_array_find(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2_array_find_from(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector2_array_rfind(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2_array_rfind_from(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector2_array_count(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector2_array_has(const pandemonium_pool_vector2_array *p_self, const pandemonium_vector2 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector2_array_is_locked(const pandemonium_pool_vector2_array *p_self);

void GDAPI pandemonium_pool_vector2_array_resize(pandemonium_pool_vector2_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector2_array_clear(pandemonium_pool_vector2_array *p_self);

void GDAPI pandemonium_pool_vector2_array_invert(pandemonium_pool_vector2_array *p_self);
void GDAPI pandemonium_pool_vector2_array_sort(pandemonium_pool_vector2_array *p_self);

void GDAPI pandemonium_pool_vector2_array_new(pandemonium_pool_vector2_array *r_dest);
void GDAPI pandemonium_pool_vector2_array_new_copy(pandemonium_pool_vector2_array *r_dest, const pandemonium_pool_vector2_array *p_src);
void GDAPI pandemonium_pool_vector2_array_new_with_array(pandemonium_pool_vector2_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector2_array_destroy(pandemonium_pool_vector2_array *p_self);

// vector2i

pandemonium_pool_vector2i_array_read_access GDAPI *pandemonium_pool_vector2i_array_read(const pandemonium_pool_vector2i_array *p_self);
pandemonium_pool_vector2i_array_write_access GDAPI *pandemonium_pool_vector2i_array_write(pandemonium_pool_vector2i_array *p_self);

void GDAPI pandemonium_pool_vector2i_array_remove(pandemonium_pool_vector2i_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector2i_array_size(const pandemonium_pool_vector2i_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector2i_array_empty(const pandemonium_pool_vector2i_array *p_self);

void GDAPI pandemonium_pool_vector2i_array_set(pandemonium_pool_vector2i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector2i *p_data);
pandemonium_vector2i GDAPI pandemonium_pool_vector2i_array_get(const pandemonium_pool_vector2i_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector2i_array_fill(pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val);
void GDAPI pandemonium_pool_vector2i_array_push_back(pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_data);
void GDAPI pandemonium_pool_vector2i_array_append(pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_data);
void GDAPI pandemonium_pool_vector2i_array_append_array(pandemonium_pool_vector2i_array *p_self, const pandemonium_pool_vector2i_array *p_array);

pandemonium_pool_vector2i_array GDAPI pandemonium_pool_vector2i_array_subarray(const pandemonium_pool_vector2i_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector2i_array_insert(pandemonium_pool_vector2i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector2i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector2i_array_contains(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_find(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_find_from(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_rfind(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_rfind_from(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_count(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector2i_array_has(const pandemonium_pool_vector2i_array *p_self, const pandemonium_vector2i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector2i_array_is_locked(const pandemonium_pool_vector2i_array *p_self);

void GDAPI pandemonium_pool_vector2i_array_resize(pandemonium_pool_vector2i_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector2i_array_clear(pandemonium_pool_vector2i_array *p_self);

void GDAPI pandemonium_pool_vector2i_array_invert(pandemonium_pool_vector2i_array *p_self);
void GDAPI pandemonium_pool_vector2i_array_sort(pandemonium_pool_vector2i_array *p_self);

void GDAPI pandemonium_pool_vector2i_array_new(pandemonium_pool_vector2i_array *r_dest);
void GDAPI pandemonium_pool_vector2i_array_new_copy(pandemonium_pool_vector2i_array *r_dest, const pandemonium_pool_vector2i_array *p_src);
void GDAPI pandemonium_pool_vector2i_array_new_with_array(pandemonium_pool_vector2i_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector2i_array_destroy(pandemonium_pool_vector2i_array *p_self);

// vector3

pandemonium_pool_vector3_array_read_access GDAPI *pandemonium_pool_vector3_array_read(const pandemonium_pool_vector3_array *p_self);
pandemonium_pool_vector3_array_write_access GDAPI *pandemonium_pool_vector3_array_write(pandemonium_pool_vector3_array *p_self);

void GDAPI pandemonium_pool_vector3_array_remove(pandemonium_pool_vector3_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector3_array_size(const pandemonium_pool_vector3_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector3_array_empty(const pandemonium_pool_vector3_array *p_self);

void GDAPI pandemonium_pool_vector3_array_set(pandemonium_pool_vector3_array *p_self, const pandemonium_int p_idx, const pandemonium_vector3 *p_data);
pandemonium_vector3 GDAPI pandemonium_pool_vector3_array_get(const pandemonium_pool_vector3_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector3_array_fill(pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val);
void GDAPI pandemonium_pool_vector3_array_push_back(pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_data);
void GDAPI pandemonium_pool_vector3_array_append(pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_data);
void GDAPI pandemonium_pool_vector3_array_append_array(pandemonium_pool_vector3_array *p_self, const pandemonium_pool_vector3_array *p_array);

pandemonium_pool_vector3_array GDAPI pandemonium_pool_vector3_array_subarray(const pandemonium_pool_vector3_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector3_array_insert(pandemonium_pool_vector3_array *p_self, const pandemonium_int p_idx, const pandemonium_vector3 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector3_array_contains(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3_array_find(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3_array_find_from(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector3_array_rfind(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3_array_rfind_from(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector3_array_count(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector3_array_has(const pandemonium_pool_vector3_array *p_self, const pandemonium_vector3 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector3_array_is_locked(const pandemonium_pool_vector3_array *p_self);

void GDAPI pandemonium_pool_vector3_array_resize(pandemonium_pool_vector3_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector3_array_clear(pandemonium_pool_vector3_array *p_self);

void GDAPI pandemonium_pool_vector3_array_invert(pandemonium_pool_vector3_array *p_self);
void GDAPI pandemonium_pool_vector3_array_sort(pandemonium_pool_vector3_array *p_self);

void GDAPI pandemonium_pool_vector3_array_new(pandemonium_pool_vector3_array *r_dest);
void GDAPI pandemonium_pool_vector3_array_new_copy(pandemonium_pool_vector3_array *r_dest, const pandemonium_pool_vector3_array *p_src);
void GDAPI pandemonium_pool_vector3_array_new_with_array(pandemonium_pool_vector3_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector3_array_destroy(pandemonium_pool_vector3_array *p_self);

// vector3i

pandemonium_pool_vector3i_array_read_access GDAPI *pandemonium_pool_vector3i_array_read(const pandemonium_pool_vector3i_array *p_self);
pandemonium_pool_vector3i_array_write_access GDAPI *pandemonium_pool_vector3i_array_write(pandemonium_pool_vector3i_array *p_self);

void GDAPI pandemonium_pool_vector3i_array_remove(pandemonium_pool_vector3i_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector3i_array_size(const pandemonium_pool_vector3i_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector3i_array_empty(const pandemonium_pool_vector3i_array *p_self);

void GDAPI pandemonium_pool_vector3i_array_set(pandemonium_pool_vector3i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector3i *p_data);
pandemonium_vector3i GDAPI pandemonium_pool_vector3i_array_get(const pandemonium_pool_vector3i_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector3i_array_fill(pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val);
void GDAPI pandemonium_pool_vector3i_array_push_back(pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_data);
void GDAPI pandemonium_pool_vector3i_array_append(pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_data);
void GDAPI pandemonium_pool_vector3i_array_append_array(pandemonium_pool_vector3i_array *p_self, const pandemonium_pool_vector3i_array *p_array);

pandemonium_pool_vector3i_array GDAPI pandemonium_pool_vector3i_array_subarray(const pandemonium_pool_vector3i_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector3i_array_insert(pandemonium_pool_vector3i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector3i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector3i_array_contains(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_find(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_find_from(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_rfind(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_rfind_from(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_count(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector3i_array_has(const pandemonium_pool_vector3i_array *p_self, const pandemonium_vector3i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector3i_array_is_locked(const pandemonium_pool_vector3i_array *p_self);

void GDAPI pandemonium_pool_vector3i_array_resize(pandemonium_pool_vector3i_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector3i_array_clear(pandemonium_pool_vector3i_array *p_self);

void GDAPI pandemonium_pool_vector3i_array_invert(pandemonium_pool_vector3i_array *p_self);
void GDAPI pandemonium_pool_vector3i_array_sort(pandemonium_pool_vector3i_array *p_self);

void GDAPI pandemonium_pool_vector3i_array_new(pandemonium_pool_vector3i_array *r_dest);
void GDAPI pandemonium_pool_vector3i_array_new_copy(pandemonium_pool_vector3i_array *r_dest, const pandemonium_pool_vector3i_array *p_src);
void GDAPI pandemonium_pool_vector3i_array_new_with_array(pandemonium_pool_vector3i_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector3i_array_destroy(pandemonium_pool_vector3i_array *p_self);

// vector4

pandemonium_pool_vector4_array_read_access GDAPI *pandemonium_pool_vector4_array_read(const pandemonium_pool_vector4_array *p_self);
pandemonium_pool_vector4_array_write_access GDAPI *pandemonium_pool_vector4_array_write(pandemonium_pool_vector4_array *p_self);

void GDAPI pandemonium_pool_vector4_array_remove(pandemonium_pool_vector4_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector4_array_size(const pandemonium_pool_vector4_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector4_array_empty(const pandemonium_pool_vector4_array *p_self);

void GDAPI pandemonium_pool_vector4_array_set(pandemonium_pool_vector4_array *p_self, const pandemonium_int p_idx, const pandemonium_vector4 *p_data);
pandemonium_vector4 GDAPI pandemonium_pool_vector4_array_get(const pandemonium_pool_vector4_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector4_array_fill(pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val);
void GDAPI pandemonium_pool_vector4_array_push_back(pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_data);
void GDAPI pandemonium_pool_vector4_array_append(pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_data);
void GDAPI pandemonium_pool_vector4_array_append_array(pandemonium_pool_vector4_array *p_self, const pandemonium_pool_vector4_array *p_array);

pandemonium_pool_vector4_array GDAPI pandemonium_pool_vector4_array_subarray(const pandemonium_pool_vector4_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector4_array_insert(pandemonium_pool_vector4_array *p_self, const pandemonium_int p_idx, const pandemonium_vector4 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector4_array_contains(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4_array_find(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4_array_find_from(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector4_array_rfind(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4_array_rfind_from(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector4_array_count(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector4_array_has(const pandemonium_pool_vector4_array *p_self, const pandemonium_vector4 *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector4_array_is_locked(const pandemonium_pool_vector4_array *p_self);

void GDAPI pandemonium_pool_vector4_array_resize(pandemonium_pool_vector4_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector4_array_clear(pandemonium_pool_vector4_array *p_self);

void GDAPI pandemonium_pool_vector4_array_invert(pandemonium_pool_vector4_array *p_self);
void GDAPI pandemonium_pool_vector4_array_sort(pandemonium_pool_vector4_array *p_self);

void GDAPI pandemonium_pool_vector4_array_new(pandemonium_pool_vector4_array *r_dest);
void GDAPI pandemonium_pool_vector4_array_new_copy(pandemonium_pool_vector4_array *r_dest, const pandemonium_pool_vector4_array *p_src);
void GDAPI pandemonium_pool_vector4_array_new_with_array(pandemonium_pool_vector4_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector4_array_destroy(pandemonium_pool_vector4_array *p_self);

// vector4i

pandemonium_pool_vector4i_array_read_access GDAPI *pandemonium_pool_vector4i_array_read(const pandemonium_pool_vector4i_array *p_self);
pandemonium_pool_vector4i_array_write_access GDAPI *pandemonium_pool_vector4i_array_write(pandemonium_pool_vector4i_array *p_self);

void GDAPI pandemonium_pool_vector4i_array_remove(pandemonium_pool_vector4i_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_vector4i_array_size(const pandemonium_pool_vector4i_array *p_self);
pandemonium_bool GDAPI pandemonium_pool_vector4i_array_empty(const pandemonium_pool_vector4i_array *p_self);

void GDAPI pandemonium_pool_vector4i_array_set(pandemonium_pool_vector4i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector4i *p_data);
pandemonium_vector4i GDAPI pandemonium_pool_vector4i_array_get(const pandemonium_pool_vector4i_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_vector4i_array_fill(pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val);
void GDAPI pandemonium_pool_vector4i_array_push_back(pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_data);
void GDAPI pandemonium_pool_vector4i_array_append(pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_data);
void GDAPI pandemonium_pool_vector4i_array_append_array(pandemonium_pool_vector4i_array *p_self, const pandemonium_pool_vector4i_array *p_array);

pandemonium_pool_vector4i_array GDAPI pandemonium_pool_vector4i_array_subarray(const pandemonium_pool_vector4i_array *p_self, pandemonium_int p_from, pandemonium_int p_to);

pandemonium_error GDAPI pandemonium_pool_vector4i_array_insert(pandemonium_pool_vector4i_array *p_self, const pandemonium_int p_idx, const pandemonium_vector4i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector4i_array_contains(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_find(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_find_from(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_rfind(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_rfind_from(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val, pandemonium_int p_from);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_count(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_val);
pandemonium_bool GDAPI pandemonium_pool_vector4i_array_has(const pandemonium_pool_vector4i_array *p_self, const pandemonium_vector4i *p_data);

pandemonium_bool GDAPI pandemonium_pool_vector4i_array_is_locked(const pandemonium_pool_vector4i_array *p_self);

void GDAPI pandemonium_pool_vector4i_array_resize(pandemonium_pool_vector4i_array *p_self, const pandemonium_int p_size);
pandemonium_int GDAPI pandemonium_pool_vector4i_array_clear(pandemonium_pool_vector4i_array *p_self);

void GDAPI pandemonium_pool_vector4i_array_invert(pandemonium_pool_vector4i_array *p_self);
void GDAPI pandemonium_pool_vector4i_array_sort(pandemonium_pool_vector4i_array *p_self);

void GDAPI pandemonium_pool_vector4i_array_new(pandemonium_pool_vector4i_array *r_dest);
void GDAPI pandemonium_pool_vector4i_array_new_copy(pandemonium_pool_vector4i_array *r_dest, const pandemonium_pool_vector4i_array *p_src);
void GDAPI pandemonium_pool_vector4i_array_new_with_array(pandemonium_pool_vector4i_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_vector4i_array_destroy(pandemonium_pool_vector4i_array *p_self);

// color

void GDAPI pandemonium_pool_color_array_new(pandemonium_pool_color_array *r_dest);
void GDAPI pandemonium_pool_color_array_new_copy(pandemonium_pool_color_array *r_dest, const pandemonium_pool_color_array *p_src);
void GDAPI pandemonium_pool_color_array_new_with_array(pandemonium_pool_color_array *r_dest, const pandemonium_array *p_a);

void GDAPI pandemonium_pool_color_array_append(pandemonium_pool_color_array *p_self, const pandemonium_color *p_data);

void GDAPI pandemonium_pool_color_array_append_array(pandemonium_pool_color_array *p_self, const pandemonium_pool_color_array *p_array);

pandemonium_error GDAPI pandemonium_pool_color_array_insert(pandemonium_pool_color_array *p_self, const pandemonium_int p_idx, const pandemonium_color *p_data);

void GDAPI pandemonium_pool_color_array_invert(pandemonium_pool_color_array *p_self);

void GDAPI pandemonium_pool_color_array_push_back(pandemonium_pool_color_array *p_self, const pandemonium_color *p_data);

void GDAPI pandemonium_pool_color_array_remove(pandemonium_pool_color_array *p_self, const pandemonium_int p_idx);

void GDAPI pandemonium_pool_color_array_resize(pandemonium_pool_color_array *p_self, const pandemonium_int p_size);

void GDAPI pandemonium_pool_color_array_sort(pandemonium_pool_color_array *p_self);

pandemonium_pool_color_array_read_access GDAPI *pandemonium_pool_color_array_read(const pandemonium_pool_color_array *p_self);

pandemonium_pool_color_array_write_access GDAPI *pandemonium_pool_color_array_write(pandemonium_pool_color_array *p_self);

void GDAPI pandemonium_pool_color_array_set(pandemonium_pool_color_array *p_self, const pandemonium_int p_idx, const pandemonium_color *p_data);
pandemonium_color GDAPI pandemonium_pool_color_array_get(const pandemonium_pool_color_array *p_self, const pandemonium_int p_idx);

pandemonium_int GDAPI pandemonium_pool_color_array_size(const pandemonium_pool_color_array *p_self);

pandemonium_bool GDAPI pandemonium_pool_color_array_empty(const pandemonium_pool_color_array *p_self);

pandemonium_bool GDAPI pandemonium_pool_color_array_has(const pandemonium_pool_color_array *p_self, const pandemonium_color *p_data);

void GDAPI pandemonium_pool_color_array_destroy(pandemonium_pool_color_array *p_self);

//
// read accessor functions
//

pandemonium_pool_byte_array_read_access GDAPI *pandemonium_pool_byte_array_read_access_copy(const pandemonium_pool_byte_array_read_access *p_other);
const uint8_t GDAPI *pandemonium_pool_byte_array_read_access_ptr(const pandemonium_pool_byte_array_read_access *p_read);
void GDAPI pandemonium_pool_byte_array_read_access_operator_assign(pandemonium_pool_byte_array_read_access *p_read, pandemonium_pool_byte_array_read_access *p_other);
void GDAPI pandemonium_pool_byte_array_read_access_destroy(pandemonium_pool_byte_array_read_access *p_read);

pandemonium_pool_int_array_read_access GDAPI *pandemonium_pool_int_array_read_access_copy(const pandemonium_pool_int_array_read_access *p_other);
const pandemonium_int GDAPI *pandemonium_pool_int_array_read_access_ptr(const pandemonium_pool_int_array_read_access *p_read);
void GDAPI pandemonium_pool_int_array_read_access_operator_assign(pandemonium_pool_int_array_read_access *p_read, pandemonium_pool_int_array_read_access *p_other);
void GDAPI pandemonium_pool_int_array_read_access_destroy(pandemonium_pool_int_array_read_access *p_read);

pandemonium_pool_real_array_read_access GDAPI *pandemonium_pool_real_array_read_access_copy(const pandemonium_pool_real_array_read_access *p_other);
const pandemonium_real GDAPI *pandemonium_pool_real_array_read_access_ptr(const pandemonium_pool_real_array_read_access *p_read);
void GDAPI pandemonium_pool_real_array_read_access_operator_assign(pandemonium_pool_real_array_read_access *p_read, pandemonium_pool_real_array_read_access *p_other);
void GDAPI pandemonium_pool_real_array_read_access_destroy(pandemonium_pool_real_array_read_access *p_read);

pandemonium_pool_string_array_read_access GDAPI *pandemonium_pool_string_array_read_access_copy(const pandemonium_pool_string_array_read_access *p_other);
const pandemonium_string GDAPI *pandemonium_pool_string_array_read_access_ptr(const pandemonium_pool_string_array_read_access *p_read);
void GDAPI pandemonium_pool_string_array_read_access_operator_assign(pandemonium_pool_string_array_read_access *p_read, pandemonium_pool_string_array_read_access *p_other);
void GDAPI pandemonium_pool_string_array_read_access_destroy(pandemonium_pool_string_array_read_access *p_read);

pandemonium_pool_vector2_array_read_access GDAPI *pandemonium_pool_vector2_array_read_access_copy(const pandemonium_pool_vector2_array_read_access *p_other);
const pandemonium_vector2 GDAPI *pandemonium_pool_vector2_array_read_access_ptr(const pandemonium_pool_vector2_array_read_access *p_read);
void GDAPI pandemonium_pool_vector2_array_read_access_operator_assign(pandemonium_pool_vector2_array_read_access *p_read, pandemonium_pool_vector2_array_read_access *p_other);
void GDAPI pandemonium_pool_vector2_array_read_access_destroy(pandemonium_pool_vector2_array_read_access *p_read);

pandemonium_pool_vector2i_array_read_access GDAPI *pandemonium_pool_vector2i_array_read_access_copy(const pandemonium_pool_vector2i_array_read_access *p_other);
const pandemonium_vector2i GDAPI *pandemonium_pool_vector2i_array_read_access_ptr(const pandemonium_pool_vector2i_array_read_access *p_read);
void GDAPI pandemonium_pool_vector2i_array_read_access_operator_assign(pandemonium_pool_vector2i_array_read_access *p_read, pandemonium_pool_vector2i_array_read_access *p_other);
void GDAPI pandemonium_pool_vector2i_array_read_access_destroy(pandemonium_pool_vector2i_array_read_access *p_read);

pandemonium_pool_vector3_array_read_access GDAPI *pandemonium_pool_vector3_array_read_access_copy(const pandemonium_pool_vector3_array_read_access *p_other);
const pandemonium_vector3 GDAPI *pandemonium_pool_vector3_array_read_access_ptr(const pandemonium_pool_vector3_array_read_access *p_read);
void GDAPI pandemonium_pool_vector3_array_read_access_operator_assign(pandemonium_pool_vector3_array_read_access *p_read, pandemonium_pool_vector3_array_read_access *p_other);
void GDAPI pandemonium_pool_vector3_array_read_access_destroy(pandemonium_pool_vector3_array_read_access *p_read);

pandemonium_pool_vector3i_array_read_access GDAPI *pandemonium_pool_vector3i_array_read_access_copy(const pandemonium_pool_vector3i_array_read_access *p_other);
const pandemonium_vector3i GDAPI *pandemonium_pool_vector3i_array_read_access_ptr(const pandemonium_pool_vector3i_array_read_access *p_read);
void GDAPI pandemonium_pool_vector3i_array_read_access_operator_assign(pandemonium_pool_vector3i_array_read_access *p_read, pandemonium_pool_vector3i_array_read_access *p_other);
void GDAPI pandemonium_pool_vector3i_array_read_access_destroy(pandemonium_pool_vector3i_array_read_access *p_read);

pandemonium_pool_vector4_array_read_access GDAPI *pandemonium_pool_vector4_array_read_access_copy(const pandemonium_pool_vector4_array_read_access *p_other);
const pandemonium_vector4 GDAPI *pandemonium_pool_vector4_array_read_access_ptr(const pandemonium_pool_vector4_array_read_access *p_read);
void GDAPI pandemonium_pool_vector4_array_read_access_operator_assign(pandemonium_pool_vector4_array_read_access *p_read, pandemonium_pool_vector4_array_read_access *p_other);
void GDAPI pandemonium_pool_vector4_array_read_access_destroy(pandemonium_pool_vector4_array_read_access *p_read);

pandemonium_pool_vector4i_array_read_access GDAPI *pandemonium_pool_vector4i_array_read_access_copy(const pandemonium_pool_vector4i_array_read_access *p_other);
const pandemonium_vector4i GDAPI *pandemonium_pool_vector4i_array_read_access_ptr(const pandemonium_pool_vector4i_array_read_access *p_read);
void GDAPI pandemonium_pool_vector4i_array_read_access_operator_assign(pandemonium_pool_vector4i_array_read_access *p_read, pandemonium_pool_vector4i_array_read_access *p_other);
void GDAPI pandemonium_pool_vector4i_array_read_access_destroy(pandemonium_pool_vector4i_array_read_access *p_read);

pandemonium_pool_color_array_read_access GDAPI *pandemonium_pool_color_array_read_access_copy(const pandemonium_pool_color_array_read_access *p_other);
const pandemonium_color GDAPI *pandemonium_pool_color_array_read_access_ptr(const pandemonium_pool_color_array_read_access *p_read);
void GDAPI pandemonium_pool_color_array_read_access_operator_assign(pandemonium_pool_color_array_read_access *p_read, pandemonium_pool_color_array_read_access *p_other);
void GDAPI pandemonium_pool_color_array_read_access_destroy(pandemonium_pool_color_array_read_access *p_read);

//
// write accessor functions
//

pandemonium_pool_byte_array_write_access GDAPI *pandemonium_pool_byte_array_write_access_copy(const pandemonium_pool_byte_array_write_access *p_other);
uint8_t GDAPI *pandemonium_pool_byte_array_write_access_ptr(const pandemonium_pool_byte_array_write_access *p_write);
void GDAPI pandemonium_pool_byte_array_write_access_operator_assign(pandemonium_pool_byte_array_write_access *p_write, pandemonium_pool_byte_array_write_access *p_other);
void GDAPI pandemonium_pool_byte_array_write_access_destroy(pandemonium_pool_byte_array_write_access *p_write);

pandemonium_pool_int_array_write_access GDAPI *pandemonium_pool_int_array_write_access_copy(const pandemonium_pool_int_array_write_access *p_other);
pandemonium_int GDAPI *pandemonium_pool_int_array_write_access_ptr(const pandemonium_pool_int_array_write_access *p_write);
void GDAPI pandemonium_pool_int_array_write_access_operator_assign(pandemonium_pool_int_array_write_access *p_write, pandemonium_pool_int_array_write_access *p_other);
void GDAPI pandemonium_pool_int_array_write_access_destroy(pandemonium_pool_int_array_write_access *p_write);

pandemonium_pool_real_array_write_access GDAPI *pandemonium_pool_real_array_write_access_copy(const pandemonium_pool_real_array_write_access *p_other);
pandemonium_real GDAPI *pandemonium_pool_real_array_write_access_ptr(const pandemonium_pool_real_array_write_access *p_write);
void GDAPI pandemonium_pool_real_array_write_access_operator_assign(pandemonium_pool_real_array_write_access *p_write, pandemonium_pool_real_array_write_access *p_other);
void GDAPI pandemonium_pool_real_array_write_access_destroy(pandemonium_pool_real_array_write_access *p_write);

pandemonium_pool_string_array_write_access GDAPI *pandemonium_pool_string_array_write_access_copy(const pandemonium_pool_string_array_write_access *p_other);
pandemonium_string GDAPI *pandemonium_pool_string_array_write_access_ptr(const pandemonium_pool_string_array_write_access *p_write);
void GDAPI pandemonium_pool_string_array_write_access_operator_assign(pandemonium_pool_string_array_write_access *p_write, pandemonium_pool_string_array_write_access *p_other);
void GDAPI pandemonium_pool_string_array_write_access_destroy(pandemonium_pool_string_array_write_access *p_write);

pandemonium_pool_vector2_array_write_access GDAPI *pandemonium_pool_vector2_array_write_access_copy(const pandemonium_pool_vector2_array_write_access *p_other);
pandemonium_vector2 GDAPI *pandemonium_pool_vector2_array_write_access_ptr(const pandemonium_pool_vector2_array_write_access *p_write);
void GDAPI pandemonium_pool_vector2_array_write_access_operator_assign(pandemonium_pool_vector2_array_write_access *p_write, pandemonium_pool_vector2_array_write_access *p_other);
void GDAPI pandemonium_pool_vector2_array_write_access_destroy(pandemonium_pool_vector2_array_write_access *p_write);

pandemonium_pool_vector2i_array_write_access GDAPI *pandemonium_pool_vector2i_array_write_access_copy(const pandemonium_pool_vector2i_array_write_access *p_other);
pandemonium_vector2i GDAPI *pandemonium_pool_vector2i_array_write_access_ptr(const pandemonium_pool_vector2i_array_write_access *p_write);
void GDAPI pandemonium_pool_vector2i_array_write_access_operator_assign(pandemonium_pool_vector2i_array_write_access *p_write, pandemonium_pool_vector2i_array_write_access *p_other);
void GDAPI pandemonium_pool_vector2i_array_write_access_destroy(pandemonium_pool_vector2i_array_write_access *p_write);

pandemonium_pool_vector3_array_write_access GDAPI *pandemonium_pool_vector3_array_write_access_copy(const pandemonium_pool_vector3_array_write_access *p_other);
pandemonium_vector3 GDAPI *pandemonium_pool_vector3_array_write_access_ptr(const pandemonium_pool_vector3_array_write_access *p_write);
void GDAPI pandemonium_pool_vector3_array_write_access_operator_assign(pandemonium_pool_vector3_array_write_access *p_write, pandemonium_pool_vector3_array_write_access *p_other);
void GDAPI pandemonium_pool_vector3_array_write_access_destroy(pandemonium_pool_vector3_array_write_access *p_write);

pandemonium_pool_vector3i_array_write_access GDAPI *pandemonium_pool_vector3i_array_write_access_copy(const pandemonium_pool_vector3i_array_write_access *p_other);
pandemonium_vector3i GDAPI *pandemonium_pool_vector3i_array_write_access_ptr(const pandemonium_pool_vector3i_array_write_access *p_write);
void GDAPI pandemonium_pool_vector3i_array_write_access_operator_assign(pandemonium_pool_vector3i_array_write_access *p_write, pandemonium_pool_vector3i_array_write_access *p_other);
void GDAPI pandemonium_pool_vector3i_array_write_access_destroy(pandemonium_pool_vector3i_array_write_access *p_write);

pandemonium_pool_vector4_array_write_access GDAPI *pandemonium_pool_vector4_array_write_access_copy(const pandemonium_pool_vector4_array_write_access *p_other);
pandemonium_vector4 GDAPI *pandemonium_pool_vector4_array_write_access_ptr(const pandemonium_pool_vector4_array_write_access *p_write);
void GDAPI pandemonium_pool_vector4_array_write_access_operator_assign(pandemonium_pool_vector4_array_write_access *p_write, pandemonium_pool_vector4_array_write_access *p_other);
void GDAPI pandemonium_pool_vector4_array_write_access_destroy(pandemonium_pool_vector4_array_write_access *p_write);

pandemonium_pool_vector4i_array_write_access GDAPI *pandemonium_pool_vector4i_array_write_access_copy(const pandemonium_pool_vector4i_array_write_access *p_other);
pandemonium_vector4i GDAPI *pandemonium_pool_vector4i_array_write_access_ptr(const pandemonium_pool_vector4i_array_write_access *p_write);
void GDAPI pandemonium_pool_vector4i_array_write_access_operator_assign(pandemonium_pool_vector4i_array_write_access *p_write, pandemonium_pool_vector4i_array_write_access *p_other);
void GDAPI pandemonium_pool_vector4i_array_write_access_destroy(pandemonium_pool_vector4i_array_write_access *p_write);

pandemonium_pool_color_array_write_access GDAPI *pandemonium_pool_color_array_write_access_copy(const pandemonium_pool_color_array_write_access *p_other);
pandemonium_color GDAPI *pandemonium_pool_color_array_write_access_ptr(const pandemonium_pool_color_array_write_access *p_write);
void GDAPI pandemonium_pool_color_array_write_access_operator_assign(pandemonium_pool_color_array_write_access *p_write, pandemonium_pool_color_array_write_access *p_other);
void GDAPI pandemonium_pool_color_array_write_access_destroy(pandemonium_pool_color_array_write_access *p_write);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_POOL_ARRAYS_H
