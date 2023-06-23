/**************************************************************************/
/*  string_name.h                                                         */
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

#ifndef GDNATIVE_STRING_NAME_H
#define GDNATIVE_STRING_NAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

#define PANDEMONIUM_STRING_NAME_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_STRING_NAME_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_STRING_NAME_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_STRING_NAME_SIZE];
} pandemonium_string_name;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_string GDAPI pandemonium_string_name_get_name(const pandemonium_string_name *p_self);

uint32_t GDAPI pandemonium_string_name_get_hash(const pandemonium_string_name *p_self);
const void GDAPI *pandemonium_string_name_get_data_unique_pointer(const pandemonium_string_name *p_self);

pandemonium_bool GDAPI pandemonium_string_name_operator_equal(const pandemonium_string_name *p_self, const pandemonium_string_name *p_other);
pandemonium_bool GDAPI pandemonium_string_name_operator_less(const pandemonium_string_name *p_self, const pandemonium_string_name *p_other);

void GDAPI pandemonium_string_name_new(pandemonium_string_name *r_dest);
void GDAPI pandemonium_string_name_new_data_string(pandemonium_string_name *r_dest, const pandemonium_string *p_name);
void GDAPI pandemonium_string_name_new_data_char(pandemonium_string_name *r_dest, const char *p_name);
void GDAPI pandemonium_string_name_new_datac(pandemonium_string_name *r_dest, const pandemonium_char_type *p_name);

void GDAPI pandemonium_string_name_destroy(pandemonium_string_name *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_STRING_NAME_H
