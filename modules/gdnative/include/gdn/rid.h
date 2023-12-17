#ifndef GDNATIVE_RID_H
#define GDNATIVE_RID_H

/*************************************************************************/
/*  rid.h                                                                */
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

#define PANDEMONIUM_RID_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_RID_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_RID_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_RID_SIZE];
} pandemonium_rid;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_int GDAPI pandemonium_rid_get_id(const pandemonium_rid *p_self);

void GDAPI pandemonium_rid_new_with_resource(pandemonium_rid *r_dest, const pandemonium_object *p_from);

pandemonium_bool GDAPI pandemonium_rid_operator_equal(const pandemonium_rid *p_self, const pandemonium_rid *p_b);

pandemonium_bool GDAPI pandemonium_rid_operator_less(const pandemonium_rid *p_self, const pandemonium_rid *p_b);

void GDAPI pandemonium_rid_new(pandemonium_rid *r_dest);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_RID_H
