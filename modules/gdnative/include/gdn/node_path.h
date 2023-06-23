/**************************************************************************/
/*  node_path.h                                                           */
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

#ifndef GDNATIVE_NODE_PATH_H
#define GDNATIVE_NODE_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PANDEMONIUM_NODE_PATH_SIZE sizeof(void *)

#ifndef PANDEMONIUM_CORE_API_PANDEMONIUM_NODE_PATH_TYPE_DEFINED
#define PANDEMONIUM_CORE_API_PANDEMONIUM_NODE_PATH_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[PANDEMONIUM_NODE_PATH_SIZE];
} pandemonium_node_path;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdn/gdnative.h>
#include <gdn/string.h>
#include <gdn/string_name.h>

#ifdef __cplusplus
extern "C" {
#endif

pandemonium_string_name GDAPI pandemonium_node_path_get_sname(const pandemonium_node_path *p_self);

pandemonium_bool GDAPI pandemonium_node_path_is_absolute(const pandemonium_node_path *p_self);
pandemonium_int GDAPI pandemonium_node_path_get_name_count(const pandemonium_node_path *p_self);
pandemonium_string GDAPI pandemonium_node_path_get_name(const pandemonium_node_path *p_self, const pandemonium_int p_idx);
pandemonium_int GDAPI pandemonium_node_path_get_subname_count(const pandemonium_node_path *p_self);
pandemonium_string GDAPI pandemonium_node_path_get_subname(const pandemonium_node_path *p_self, const pandemonium_int p_idx);
//Vector<StringName> get_names() const;
//Vector<StringName> get_subnames() const;
pandemonium_string GDAPI pandemonium_node_path_get_concatenated_subnames(const pandemonium_node_path *p_self);

pandemonium_node_path GDAPI pandemonium_node_path_rel_path_to(const pandemonium_node_path *p_self, const pandemonium_node_path *p_np);
pandemonium_node_path pandemonium_node_path_get_as_property_path(const pandemonium_node_path *p_self);

void GDAPI pandemonium_node_path_prepend_period(pandemonium_node_path *p_self);

pandemonium_int GDAPI pandemonium_node_path_hash(const pandemonium_node_path *p_self);

pandemonium_string GDAPI pandemonium_node_path_as_string(const pandemonium_node_path *p_self);
pandemonium_bool GDAPI pandemonium_node_path_is_empty(const pandemonium_node_path *p_self);

pandemonium_bool GDAPI pandemonium_node_path_operator_equal(const pandemonium_node_path *p_self, const pandemonium_node_path *p_b);

void GDAPI pandemonium_node_path_simplify(pandemonium_node_path *p_self);
pandemonium_node_path GDAPI pandemonium_node_path_simplified(const pandemonium_node_path *p_self);

//NodePath(const Vector<StringName> &p_path, bool p_absolute);
//NodePath(const Vector<StringName> &p_path, const Vector<StringName> &p_subpath, bool p_absolute);
void GDAPI pandemonium_node_path_new(pandemonium_node_path *r_dest, const pandemonium_string *p_from);
void GDAPI pandemonium_node_path_new_copy(pandemonium_node_path *r_dest, const pandemonium_node_path *p_src);
void GDAPI pandemonium_node_path_destroy(pandemonium_node_path *p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_NODE_PATH_H
