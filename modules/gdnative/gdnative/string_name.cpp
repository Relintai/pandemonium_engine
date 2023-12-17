/*************************************************************************/
/*  string_name.cpp                                                      */
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

#include "gdn/string_name.h"

#include "core/string/string_name.h"
#include "core/string/ustring.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_string_name) == sizeof(StringName), "StringName size mismatch");

pandemonium_string GDAPI pandemonium_string_name_get_name(const pandemonium_string_name *p_self) {
	pandemonium_string ret;
	const StringName *self = (const StringName *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

uint32_t GDAPI pandemonium_string_name_get_hash(const pandemonium_string_name *p_self) {
	const StringName *self = (const StringName *)p_self;
	return self->hash();
}

const void GDAPI *pandemonium_string_name_get_data_unique_pointer(const pandemonium_string_name *p_self) {
	const StringName *self = (const StringName *)p_self;
	return self->data_unique_pointer();
}

pandemonium_bool GDAPI pandemonium_string_name_operator_equal(const pandemonium_string_name *p_self, const pandemonium_string_name *p_other) {
	const StringName *self = (const StringName *)p_self;
	const StringName *other = (const StringName *)p_other;
	return *self == *other;
}

pandemonium_bool GDAPI pandemonium_string_name_operator_less(const pandemonium_string_name *p_self, const pandemonium_string_name *p_other) {
	const StringName *self = (const StringName *)p_self;
	const StringName *other = (const StringName *)p_other;
	return *self < *other;
}

void GDAPI pandemonium_string_name_new(pandemonium_string_name *r_dest) {
	StringName *dest = (StringName *)r_dest;
	memnew_placement(dest, StringName);
}

void GDAPI pandemonium_string_name_new_data_string(pandemonium_string_name *r_dest, const pandemonium_string *p_name) {
	StringName *dest = (StringName *)r_dest;
	const String *name = (const String *)p_name;
	memnew_placement(dest, StringName(*name));
}
void GDAPI pandemonium_string_name_new_data_char(pandemonium_string_name *r_dest, const char *p_name) {
	StringName *dest = (StringName *)r_dest;
	memnew_placement(dest, StringName(p_name));
}

void GDAPI pandemonium_string_name_new_datac(pandemonium_string_name *r_dest, const pandemonium_char_type *p_name) {
	StringName *dest = (StringName *)r_dest;
	memnew_placement(dest, StringName(p_name));
}

void GDAPI pandemonium_string_name_destroy(pandemonium_string_name *p_self) {
	StringName *self = (StringName *)p_self;
	self->~StringName();
}

#ifdef __cplusplus
}
#endif
