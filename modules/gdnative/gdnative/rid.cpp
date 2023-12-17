/*************************************************************************/
/*  rid.cpp                                                              */
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

#include "gdn/rid.h"

#include "core/containers/rid.h"
#include "core/object/resource.h"
#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_rid) == sizeof(RID), "RID size mismatch");

pandemonium_int GDAPI pandemonium_rid_get_id(const pandemonium_rid *p_self) {
	const RID *self = (const RID *)p_self;
	return self->get_id();
}

void GDAPI pandemonium_rid_new_with_resource(pandemonium_rid *r_dest, const pandemonium_object *p_from) {
	const Resource *res_from = Object::cast_to<Resource>((Object *)p_from);
	pandemonium_rid_new(r_dest);
	if (res_from) {
		RID *dest = (RID *)r_dest;
		*dest = RID(res_from->get_rid());
	}
}

pandemonium_bool GDAPI pandemonium_rid_operator_equal(const pandemonium_rid *p_self, const pandemonium_rid *p_b) {
	const RID *self = (const RID *)p_self;
	const RID *b = (const RID *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_rid_operator_less(const pandemonium_rid *p_self, const pandemonium_rid *p_b) {
	const RID *self = (const RID *)p_self;
	const RID *b = (const RID *)p_b;
	return *self < *b;
}

void GDAPI pandemonium_rid_new(pandemonium_rid *r_dest) {
	RID *dest = (RID *)r_dest;
	memnew_placement(dest, RID);
}

#ifdef __cplusplus
}
#endif
