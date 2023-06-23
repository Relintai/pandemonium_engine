/**************************************************************************/
/*  array.cpp                                                             */
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

#include "gdn/array.h"

#include "core/os/memory.h"
#include "core/variant/array.h"

#include "core/containers/pool_vector.h"
#include "core/math/color.h"

#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_array) == sizeof(Array), "Array size mismatch");

void GDAPI pandemonium_array_new(pandemonium_array *r_dest) {
	Array *dest = (Array *)r_dest;
	memnew_placement(dest, Array);
}

void GDAPI pandemonium_array_new_copy(pandemonium_array *r_dest, const pandemonium_array *p_src) {
	Array *dest = (Array *)r_dest;
	const Array *src = (const Array *)p_src;
	memnew_placement(dest, Array(*src));
}

void GDAPI pandemonium_array_new_pool_byte_array(pandemonium_array *r_dest, const pandemonium_pool_byte_array *p_pba) {
	Array *dest = (Array *)r_dest;
	PoolVector<uint8_t> *pca = (PoolVector<uint8_t> *)p_pba;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_int_array(pandemonium_array *r_dest, const pandemonium_pool_int_array *p_pia) {
	Array *dest = (Array *)r_dest;
	PoolVector<pandemonium_int> *pca = (PoolVector<pandemonium_int> *)p_pia;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_real_array(pandemonium_array *r_dest, const pandemonium_pool_real_array *p_pra) {
	Array *dest = (Array *)r_dest;
	PoolVector<pandemonium_real> *pca = (PoolVector<pandemonium_real> *)p_pra;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_string_array(pandemonium_array *r_dest, const pandemonium_pool_string_array *p_psa) {
	Array *dest = (Array *)r_dest;
	PoolVector<String> *pca = (PoolVector<String> *)p_psa;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector2_array(pandemonium_array *r_dest, const pandemonium_pool_vector2_array *p_pv2a) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector2> *pca = (PoolVector<Vector2> *)p_pv2a;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector2i_array(pandemonium_array *r_dest, const pandemonium_pool_vector2i_array *p_pv2ia) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector2i> *pca = (PoolVector<Vector2i> *)p_pv2ia;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector3_array(pandemonium_array *r_dest, const pandemonium_pool_vector3_array *p_pv3a) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector3> *pca = (PoolVector<Vector3> *)p_pv3a;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector3i_array(pandemonium_array *r_dest, const pandemonium_pool_vector3i_array *p_pv3ia) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector3i> *pca = (PoolVector<Vector3i> *)p_pv3ia;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector4_array(pandemonium_array *r_dest, const pandemonium_pool_vector4_array *p_pv4a) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector4> *pca = (PoolVector<Vector4> *)p_pv4a;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_vector4i_array(pandemonium_array *r_dest, const pandemonium_pool_vector4i_array *p_pv4ia) {
	Array *dest = (Array *)r_dest;
	PoolVector<Vector4i> *pca = (PoolVector<Vector4i> *)p_pv4ia;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_new_pool_color_array(pandemonium_array *r_dest, const pandemonium_pool_color_array *p_pca) {
	Array *dest = (Array *)r_dest;
	PoolVector<Color> *pca = (PoolVector<Color> *)p_pca;
	memnew_placement(dest, Array);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->operator[](i) = v;
	}
}

void GDAPI pandemonium_array_set(pandemonium_array *p_self, const pandemonium_int p_idx, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	Variant *val = (Variant *)p_value;
	self->operator[](p_idx) = *val;
}

pandemonium_variant GDAPI pandemonium_array_get(const pandemonium_array *p_self, const pandemonium_int p_idx) {
	pandemonium_variant raw_dest;
	Variant *dest = (Variant *)&raw_dest;
	const Array *self = (const Array *)p_self;
	memnew_placement(dest, Variant(self->operator[](p_idx)));
	return raw_dest;
}

pandemonium_variant GDAPI *pandemonium_array_operator_index(pandemonium_array *p_self, const pandemonium_int p_idx) {
	Array *self = (Array *)p_self;
	return (pandemonium_variant *)&self->operator[](p_idx);
}

const pandemonium_variant GDAPI *pandemonium_array_operator_index_const(const pandemonium_array *p_self, const pandemonium_int p_idx) {
	const Array *self = (const Array *)p_self;
	return (const pandemonium_variant *)&self->operator[](p_idx);
}

void GDAPI pandemonium_array_append_array(pandemonium_array *p_self, const pandemonium_array *p_array) {
	Array *self = (Array *)p_self;
	const Array *array = (const Array *)p_array;
	self->append_array(*array);
}

void GDAPI pandemonium_array_append(pandemonium_array *p_self, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	Variant *val = (Variant *)p_value;
	self->append(*val);
}

void GDAPI pandemonium_array_clear(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	self->clear();
}

pandemonium_bool GDAPI pandemonium_array_deep_equal(const pandemonium_array *p_self, const pandemonium_array *p_array) {
	const Array *self = (const Array *)p_self;
	const Array *array = (const Array *)p_array;
	return self->deep_equal(*array);
}
pandemonium_bool GDAPI pandemonium_array_deep_equal_recursion_count(const pandemonium_array *p_self, const pandemonium_array *p_array, pandemonium_int p_recursion_count) {
	const Array *self = (const Array *)p_self;
	const Array *array = (const Array *)p_array;
	return self->deep_equal(*array, p_recursion_count);
}
pandemonium_bool GDAPI pandemonium_array_operator_equals(pandemonium_array *p_self, const pandemonium_array *p_array) {
	Array *self = (Array *)p_self;
	const Array *array = (const Array *)p_array;
	return self->operator==(*array);
}

pandemonium_int GDAPI pandemonium_array_count(const pandemonium_array *p_self, const pandemonium_variant *p_value) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->count(*val);
}

pandemonium_bool GDAPI pandemonium_array_empty(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	return self->empty();
}

void GDAPI pandemonium_array_erase(pandemonium_array *p_self, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	self->erase(*val);
}

pandemonium_variant GDAPI pandemonium_array_front(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->front();
	return v;
}

pandemonium_variant GDAPI pandemonium_array_back(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->back();
	return v;
}

pandemonium_int GDAPI pandemonium_array_find(const pandemonium_array *p_self, const pandemonium_variant *p_what) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find(*val);
}

pandemonium_int GDAPI pandemonium_array_find_from(const pandemonium_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find(*val, p_from);
}

pandemonium_int GDAPI pandemonium_array_find_last(const pandemonium_array *p_self, const pandemonium_variant *p_what) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find_last(*val);
}

pandemonium_bool GDAPI pandemonium_array_has(const pandemonium_array *p_self, const pandemonium_variant *p_value) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->has(*val);
}

pandemonium_int GDAPI pandemonium_array_hash(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	return self->hash();
}

void GDAPI pandemonium_array_insert(pandemonium_array *p_self, const pandemonium_int p_pos, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	self->insert(p_pos, *val);
}

void GDAPI pandemonium_array_invert(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	self->invert();
}

pandemonium_variant GDAPI pandemonium_array_pop_back(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->pop_back();
	return v;
}

pandemonium_variant GDAPI pandemonium_array_pop_front(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->pop_front();
	return v;
}

pandemonium_variant GDAPI pandemonium_array_pop_at(pandemonium_array *p_self, pandemonium_int p_pos) {
	pandemonium_variant dest;
	Array *self = (Array *)p_self;
	*((Variant *)&dest) = self->pop_at(p_pos);
	return dest;
}

void GDAPI pandemonium_array_push_back(pandemonium_array *p_self, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	self->push_back(*val);
}

void GDAPI pandemonium_array_push_front(pandemonium_array *p_self, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	const Variant *val = (const Variant *)p_value;
	self->push_front(*val);
}

void GDAPI pandemonium_array_remove(pandemonium_array *p_self, const pandemonium_int p_idx) {
	Array *self = (Array *)p_self;
	self->remove(p_idx);
}

void GDAPI pandemonium_array_fill(pandemonium_array *p_self, const pandemonium_variant *p_value) {
	Array *self = (Array *)p_self;
	const Variant *value = (const Variant *)p_value;
	self->fill(*value);
}

void GDAPI pandemonium_array_resize(pandemonium_array *p_self, const pandemonium_int p_size) {
	Array *self = (Array *)p_self;
	self->resize(p_size);
}

pandemonium_int GDAPI pandemonium_array_rfind(const pandemonium_array *p_self, const pandemonium_variant *p_what) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->rfind(*val);
}
pandemonium_int GDAPI pandemonium_array_rfind_from(const pandemonium_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from) {
	const Array *self = (const Array *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->rfind(*val, p_from);
}

pandemonium_int GDAPI pandemonium_array_size(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	return self->size();
}

void GDAPI pandemonium_array_sort(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	self->sort();
}

void GDAPI pandemonium_array_sort_custom(pandemonium_array *p_self, pandemonium_object *p_obj, const pandemonium_string *p_func) {
	Array *self = (Array *)p_self;
	const String *func = (const String *)p_func;
	self->sort_custom((Object *)p_obj, *func);
}

pandemonium_int GDAPI pandemonium_array_bsearch(pandemonium_array *p_self, const pandemonium_variant *p_value, const pandemonium_bool p_before) {
	Array *self = (Array *)p_self;
	return self->bsearch(*(const Variant *)p_value, p_before);
}

pandemonium_int GDAPI pandemonium_array_bsearch_custom(pandemonium_array *p_self, const pandemonium_variant *p_value, pandemonium_object *p_obj, const pandemonium_string *p_func, const pandemonium_bool p_before) {
	Array *self = (Array *)p_self;
	const String *func = (const String *)p_func;
	return self->bsearch_custom(*(const Variant *)p_value, (Object *)p_obj, *func, p_before);
}

void GDAPI pandemonium_array_destroy(pandemonium_array *p_self) {
	((Array *)p_self)->~Array();
}

pandemonium_array GDAPI pandemonium_array_duplicate(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	pandemonium_array res;
	Array *val = (Array *)&res;
	memnew_placement(val, Array);
	*val = self->duplicate();
	return res;
}
pandemonium_array GDAPI pandemonium_array_duplicate_deep(const pandemonium_array *p_self, const pandemonium_bool p_deep) {
	const Array *self = (const Array *)p_self;
	pandemonium_array res;
	Array *val = (Array *)&res;
	memnew_placement(val, Array);
	*val = self->duplicate(p_deep);
	return res;
}

pandemonium_array GDAPI pandemonium_array_slice(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end) {
	const Array *self = (const Array *)p_self;
	pandemonium_array res;
	Array *val = (Array *)&res;
	memnew_placement(val, Array);
	*val = self->slice(p_begin, p_end);
	return res;
}
pandemonium_array GDAPI pandemonium_array_slice_step(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step) {
	const Array *self = (const Array *)p_self;
	pandemonium_array res;
	Array *val = (Array *)&res;
	memnew_placement(val, Array);
	*val = self->slice(p_begin, p_end, p_step);
	return res;
}
pandemonium_array GDAPI pandemonium_array_slice_step_deep(const pandemonium_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step, const pandemonium_bool p_deep) {
	const Array *self = (const Array *)p_self;
	pandemonium_array res;
	Array *val = (Array *)&res;
	memnew_placement(val, Array);
	*val = self->slice(p_begin, p_end, p_step, p_deep);
	return res;
}

pandemonium_variant GDAPI pandemonium_array_max(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->max();
	return v;
}

pandemonium_variant GDAPI pandemonium_array_min(const pandemonium_array *p_self) {
	const Array *self = (const Array *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->min();
	return v;
}

void GDAPI pandemonium_array_shuffle(pandemonium_array *p_self) {
	Array *self = (Array *)p_self;
	self->shuffle();
}

#ifdef __cplusplus
}
#endif
