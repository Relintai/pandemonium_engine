/*************************************************************************/
/*  node_path.cpp                                                        */
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

#include "gdn/node_path.h"

#include "core/string/node_path.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_node_path) == sizeof(NodePath), "NodePath size mismatch");

pandemonium_string_name GDAPI pandemonium_node_path_get_sname(const pandemonium_node_path *p_self) {
	pandemonium_string_name dest;
	const NodePath *self = (const NodePath *)p_self;
	*((StringName *)&dest) = self->get_sname();
	return dest;
}

void GDAPI pandemonium_node_path_new(pandemonium_node_path *r_dest, const pandemonium_string *p_from) {
	NodePath *dest = (NodePath *)r_dest;
	const String *from = (const String *)p_from;
	memnew_placement(dest, NodePath(*from));
}

void GDAPI pandemonium_node_path_new_copy(pandemonium_node_path *r_dest, const pandemonium_node_path *p_src) {
	NodePath *dest = (NodePath *)r_dest;
	const NodePath *src = (const NodePath *)p_src;
	memnew_placement(dest, NodePath(*src));
}

void GDAPI pandemonium_node_path_destroy(pandemonium_node_path *p_self) {
	NodePath *self = (NodePath *)p_self;
	self->~NodePath();
}

pandemonium_string GDAPI pandemonium_node_path_as_string(const pandemonium_node_path *p_self) {
	pandemonium_string ret;
	const NodePath *self = (const NodePath *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

pandemonium_bool GDAPI pandemonium_node_path_is_absolute(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	return self->is_absolute();
}

pandemonium_int GDAPI pandemonium_node_path_get_name_count(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	return self->get_name_count();
}

pandemonium_string GDAPI pandemonium_node_path_get_name(const pandemonium_node_path *p_self, const pandemonium_int p_idx) {
	pandemonium_string dest;
	const NodePath *self = (const NodePath *)p_self;

	memnew_placement(&dest, String(self->get_name(p_idx)));
	return dest;
}

pandemonium_int GDAPI pandemonium_node_path_get_subname_count(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	return self->get_subname_count();
}

pandemonium_string GDAPI pandemonium_node_path_get_subname(const pandemonium_node_path *p_self, const pandemonium_int p_idx) {
	pandemonium_string dest;
	const NodePath *self = (const NodePath *)p_self;

	memnew_placement(&dest, String(self->get_subname(p_idx)));
	return dest;
}

pandemonium_string GDAPI pandemonium_node_path_get_concatenated_subnames(const pandemonium_node_path *p_self) {
	pandemonium_string dest;
	const NodePath *self = (const NodePath *)p_self;
	memnew_placement(&dest, String(self->get_concatenated_subnames()));
	return dest;
}

pandemonium_bool GDAPI pandemonium_node_path_is_empty(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	return self->is_empty();
}

pandemonium_bool GDAPI pandemonium_node_path_operator_equal(const pandemonium_node_path *p_self, const pandemonium_node_path *p_b) {
	const NodePath *self = (const NodePath *)p_self;
	const NodePath *b = (const NodePath *)p_b;
	return *self == *b;
}

void GDAPI pandemonium_node_path_simplify(pandemonium_node_path *p_self) {
	NodePath *self = (NodePath *)p_self;
	self->simplify();
}
pandemonium_node_path GDAPI pandemonium_node_path_simplified(const pandemonium_node_path *p_self) {
	pandemonium_node_path dest;
	const NodePath *self = (const NodePath *)p_self;
	*((NodePath *)&dest) = self->simplified();
	return dest;
}

pandemonium_node_path GDAPI pandemonium_node_path_rel_path_to(const pandemonium_node_path *p_self, const pandemonium_node_path *p_np) {
	pandemonium_node_path dest;
	const NodePath *self = (const NodePath *)p_self;
	const NodePath *np = (const NodePath *)p_np;
	*((NodePath *)&dest) = self->rel_path_to(*np);
	return dest;
}

pandemonium_node_path pandemonium_node_path_get_as_property_path(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	pandemonium_node_path res;
	NodePath *val = (NodePath *)&res;
	memnew_placement(val, NodePath);
	*val = self->get_as_property_path();
	return res;
}

void GDAPI pandemonium_node_path_prepend_period(pandemonium_node_path *p_self) {
	NodePath *self = (NodePath *)p_self;
	self->prepend_period();
}

pandemonium_int GDAPI pandemonium_node_path_hash(const pandemonium_node_path *p_self) {
	const NodePath *self = (const NodePath *)p_self;
	return self->hash();
}

#ifdef __cplusplus
}
#endif
