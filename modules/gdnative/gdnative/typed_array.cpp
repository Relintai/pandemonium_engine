/*************************************************************************/
/*  typed_array.cpp                                                      */
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

#include "gdn/typed_array.h"

#include "core/os/memory.h"
#include "core/variant/typed_array.h"

#include "core/containers/pool_vector.h"
#include "core/math/color.h"

#include "core/variant/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_typed_array) == sizeof(TypedArray), "TypedArray size mismatch");

void GDAPI pandemonium_typed_array_new(pandemonium_typed_array *r_dest) {
	TypedArray *dest = (TypedArray *)r_dest;
	memnew_placement(dest, TypedArray);
}

void GDAPI pandemonium_typed_array_new_copy(pandemonium_typed_array *r_dest, const pandemonium_typed_array *p_src) {
	TypedArray *dest = (TypedArray *)r_dest;
	const TypedArray *src = (const TypedArray *)p_src;
	memnew_placement(dest, TypedArray(*src));
}

void GDAPI pandemonium_typed_array_new_pool_byte_array(pandemonium_typed_array *r_dest, const pandemonium_pool_byte_array *p_pba) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<uint8_t> *pca = (PoolVector<uint8_t> *)p_pba;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_int_array(pandemonium_typed_array *r_dest, const pandemonium_pool_int_array *p_pia) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<pandemonium_int> *pca = (PoolVector<pandemonium_int> *)p_pia;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_real_array(pandemonium_typed_array *r_dest, const pandemonium_pool_real_array *p_pra) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<pandemonium_real> *pca = (PoolVector<pandemonium_real> *)p_pra;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_string_array(pandemonium_typed_array *r_dest, const pandemonium_pool_string_array *p_psa) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<String> *pca = (PoolVector<String> *)p_psa;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector2_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector2_array *p_pv2a) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector2> *pca = (PoolVector<Vector2> *)p_pv2a;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector2i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector2i_array *p_pv2ia) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector2i> *pca = (PoolVector<Vector2i> *)p_pv2ia;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector3_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector3_array *p_pv3a) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector3> *pca = (PoolVector<Vector3> *)p_pv3a;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector3i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector3i_array *p_pv3ia) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector3i> *pca = (PoolVector<Vector3i> *)p_pv3ia;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector4_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector4_array *p_pv4a) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector4> *pca = (PoolVector<Vector4> *)p_pv4a;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_vector4i_array(pandemonium_typed_array *r_dest, const pandemonium_pool_vector4i_array *p_pv4ia) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Vector4i> *pca = (PoolVector<Vector4i> *)p_pv4ia;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

void GDAPI pandemonium_typed_array_new_pool_color_array(pandemonium_typed_array *r_dest, const pandemonium_pool_color_array *p_pca) {
	TypedArray *dest = (TypedArray *)r_dest;
	PoolVector<Color> *pca = (PoolVector<Color> *)p_pca;
	memnew_placement(dest, TypedArray);
	dest->resize(pca->size());

	for (int i = 0; i < dest->size(); i++) {
		Variant v = pca->operator[](i);
		dest->set(i, v);
	}
}

pandemonium_bool GDAPI pandemonium_typed_array_set(pandemonium_typed_array *p_self, const pandemonium_int p_idx, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *value = (const Variant *)p_value;
	return self->set(p_idx, *value);
}

pandemonium_variant GDAPI pandemonium_typed_array_get(const pandemonium_typed_array *p_self, const pandemonium_int p_idx) {
	pandemonium_variant raw_dest;
	Variant *dest = (Variant *)&raw_dest;
	const TypedArray *self = (const TypedArray *)p_self;
	memnew_placement(dest, Variant(self->operator[](p_idx)));
	return raw_dest;
}

const pandemonium_variant GDAPI *pandemonium_typed_array_operator_index_const(const pandemonium_typed_array *p_self, const pandemonium_int p_idx) {
	const TypedArray *self = (const TypedArray *)p_self;
	return (const pandemonium_variant *)&self->operator[](p_idx);
}

pandemonium_bool GDAPI pandemonium_typed_array_append_array(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array) {
	TypedArray *self = (TypedArray *)p_self;
	const TypedArray *typed_array = (const TypedArray *)p_typed_array;
	return self->append_array(*typed_array);
}

pandemonium_bool pandemonium_typed_array_append(pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	Variant *val = (Variant *)p_value;
	return self->append(*val);
}

pandemonium_bool GDAPI pandemonium_typed_array_append_from(pandemonium_typed_array *p_self, const pandemonium_variant *p_array) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *array = (const Variant *)p_array;
	return self->append_from(*array);
}

void GDAPI pandemonium_typed_array_clear(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	self->clear();
}

pandemonium_bool GDAPI pandemonium_typed_array_deep_equal(const pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array) {
	const TypedArray *self = (const TypedArray *)p_self;
	const TypedArray *typed_array = (const TypedArray *)p_typed_array;
	return self->deep_equal(*typed_array);
}
pandemonium_bool GDAPI pandemonium_typed_array_deep_equal_recursion_count(const pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array, pandemonium_int p_recursion_count) {
	const TypedArray *self = (const TypedArray *)p_self;
	const TypedArray *typed_array = (const TypedArray *)p_typed_array;
	return self->deep_equal(*typed_array, p_recursion_count);
}
pandemonium_bool GDAPI pandemonium_typed_array_operator_equals(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_typed_array) {
	TypedArray *self = (TypedArray *)p_self;
	const TypedArray *typed_array = (const TypedArray *)p_typed_array;
	return self->operator==(*typed_array);
}

pandemonium_int GDAPI pandemonium_typed_array_count(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->count(*val);
}

pandemonium_bool GDAPI pandemonium_typed_array_empty(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	return self->empty();
}

void GDAPI pandemonium_typed_array_erase(pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	self->erase(*val);
}

pandemonium_variant GDAPI pandemonium_typed_array_front(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->front();
	return v;
}

pandemonium_variant GDAPI pandemonium_typed_array_back(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->back();
	return v;
}

pandemonium_int GDAPI pandemonium_typed_array_find(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find(*val);
}

pandemonium_int GDAPI pandemonium_typed_array_find_from(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find(*val, p_from);
}

pandemonium_int GDAPI pandemonium_typed_array_find_last(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->find_last(*val);
}

pandemonium_bool GDAPI pandemonium_typed_array_has(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->has(*val);
}

pandemonium_int GDAPI pandemonium_typed_array_hash(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	return self->hash();
}

pandemonium_bool GDAPI pandemonium_typed_array_insert(pandemonium_typed_array *p_self, const pandemonium_int p_pos, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->insert(p_pos, *val);
}

void GDAPI pandemonium_typed_array_invert(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	self->invert();
}

pandemonium_variant GDAPI pandemonium_typed_array_pop_back(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->pop_back();
	return v;
}

pandemonium_variant GDAPI pandemonium_typed_array_pop_front(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->pop_front();
	return v;
}

pandemonium_variant GDAPI pandemonium_typed_array_pop_at(pandemonium_typed_array *p_self, pandemonium_int p_pos) {
	pandemonium_variant dest;
	TypedArray *self = (TypedArray *)p_self;
	*((Variant *)&dest) = self->pop_at(p_pos);
	return dest;
}

pandemonium_bool GDAPI pandemonium_typed_array_push_back(pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->push_back(*val);
}

pandemonium_bool GDAPI pandemonium_typed_array_push_front(pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *val = (const Variant *)p_value;
	return self->push_front(*val);
}

void GDAPI pandemonium_typed_array_remove(pandemonium_typed_array *p_self, const pandemonium_int p_idx) {
	TypedArray *self = (TypedArray *)p_self;
	self->remove(p_idx);
}

pandemonium_bool GDAPI pandemonium_typed_array_fill(pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *value = (const Variant *)p_value;
	return self->fill(*value);
}

void GDAPI pandemonium_typed_array_resize(pandemonium_typed_array *p_self, const pandemonium_int p_size) {
	TypedArray *self = (TypedArray *)p_self;
	self->resize(p_size);
}

pandemonium_int GDAPI pandemonium_typed_array_rfind(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->rfind(*val);
}
pandemonium_int GDAPI pandemonium_typed_array_rfind_from(const pandemonium_typed_array *p_self, const pandemonium_variant *p_what, const pandemonium_int p_from) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *val = (const Variant *)p_what;
	return self->rfind(*val, p_from);
}

pandemonium_int GDAPI pandemonium_typed_array_size(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	return self->size();
}

void GDAPI pandemonium_typed_array_sort(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	self->sort();
}

void GDAPI pandemonium_typed_array_sort_custom(pandemonium_typed_array *p_self, pandemonium_object *p_obj, const pandemonium_string *p_func) {
	TypedArray *self = (TypedArray *)p_self;
	const String *func = (const String *)p_func;
	self->sort_custom((Object *)p_obj, *func);
}

pandemonium_int GDAPI pandemonium_typed_array_bsearch(pandemonium_typed_array *p_self, const pandemonium_variant *p_value, const pandemonium_bool p_before) {
	TypedArray *self = (TypedArray *)p_self;
	return self->bsearch(*(const Variant *)p_value, p_before);
}

pandemonium_int GDAPI pandemonium_typed_array_bsearch_custom(pandemonium_typed_array *p_self, const pandemonium_variant *p_value, pandemonium_object *p_obj, const pandemonium_string *p_func, const pandemonium_bool p_before) {
	TypedArray *self = (TypedArray *)p_self;
	const String *func = (const String *)p_func;
	return self->bsearch_custom(*(const Variant *)p_value, (Object *)p_obj, *func, p_before);
}

void GDAPI pandemonium_typed_array_destroy(pandemonium_typed_array *p_self) {
	((TypedArray *)p_self)->~TypedArray();
}

pandemonium_typed_array GDAPI pandemonium_typed_array_duplicate(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_typed_array res;
	TypedArray *val = (TypedArray *)&res;
	memnew_placement(val, TypedArray);
	*val = self->duplicate();
	return res;
}
pandemonium_typed_array GDAPI pandemonium_typed_array_duplicate_deep(const pandemonium_typed_array *p_self, const pandemonium_bool p_deep) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_typed_array res;
	TypedArray *val = (TypedArray *)&res;
	memnew_placement(val, TypedArray);
	*val = self->duplicate(p_deep);
	return res;
}

pandemonium_typed_array GDAPI pandemonium_typed_array_slice(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_typed_array res;
	TypedArray *val = (TypedArray *)&res;
	memnew_placement(val, TypedArray);
	*val = self->slice(p_begin, p_end);
	return res;
}
pandemonium_typed_array GDAPI pandemonium_typed_array_slice_step(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_typed_array res;
	TypedArray *val = (TypedArray *)&res;
	memnew_placement(val, TypedArray);
	*val = self->slice(p_begin, p_end, p_step);
	return res;
}
pandemonium_typed_array GDAPI pandemonium_typed_array_slice_step_deep(const pandemonium_typed_array *p_self, const pandemonium_int p_begin, const pandemonium_int p_end, const pandemonium_int p_step, const pandemonium_bool p_deep) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_typed_array res;
	TypedArray *val = (TypedArray *)&res;
	memnew_placement(val, TypedArray);
	*val = self->slice(p_begin, p_end, p_step, p_deep);
	return res;
}

pandemonium_variant GDAPI pandemonium_typed_array_max(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->max();
	return v;
}

pandemonium_variant GDAPI pandemonium_typed_array_min(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	pandemonium_variant v;
	Variant *val = (Variant *)&v;
	memnew_placement(val, Variant);
	*val = self->min();
	return v;
}

void GDAPI pandemonium_typed_array_shuffle(pandemonium_typed_array *p_self) {
	TypedArray *self = (TypedArray *)p_self;
	self->shuffle();
}

pandemonium_string GDAPI pandemonium_typed_array_get_typename_string(const pandemonium_typed_array *p_self) {
	pandemonium_string raw_dest;
	String *dest = (String *)&raw_dest;
	const TypedArray *self = (const TypedArray *)p_self;
	memnew_placement(dest, String(self->get_typename_string()));
	return raw_dest;
}

pandemonium_int GDAPI pandemonium_typed_array_get_variant_type(const pandemonium_typed_array *p_self) {
	const TypedArray *self = (const TypedArray *)p_self;
	return self->get_variant_type();
}
void GDAPI pandemonium_typed_array_set_variant_type(pandemonium_typed_array *p_self, const pandemonium_int p_variant_type) {
	TypedArray *self = (TypedArray *)p_self;
	self->set_variant_type(p_variant_type);
}

pandemonium_string_name GDAPI pandemonium_typed_array_get_object_class_name(const pandemonium_typed_array *p_self) {
	pandemonium_string_name dest;
	const TypedArray *self = (const TypedArray *)p_self;
	*((StringName *)&dest) = self->get_object_class_name();
	return dest;
}
void GDAPI pandemonium_typed_array_set_object_class_name(pandemonium_typed_array *p_self, const pandemonium_string_name *p_object_type_name) {
	TypedArray *self = (TypedArray *)p_self;
	const StringName *object_type_name = (const StringName *)p_object_type_name;
	self->set_object_class_name(*object_type_name);
}

void GDAPI pandemonium_typed_array_set_type_from_name(pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name) {
	TypedArray *self = (TypedArray *)p_self;
	const StringName *type_name = (const StringName *)p_type_name;
	self->set_type_from_name(*type_name);
}

void GDAPI pandemonium_typed_array_set_type_from_variant(pandemonium_typed_array *p_self, const pandemonium_variant *p_variant) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *variant = (const Variant *)p_variant;
	self->set_type_from_variant(*variant);
}

void GDAPI pandemonium_typed_array_set_type_from_array_element(pandemonium_typed_array *p_self, const pandemonium_variant *p_array) {
	TypedArray *self = (TypedArray *)p_self;
	const Variant *array = (const Variant *)p_array;
	self->set_type_from_array_element(*array);
}

void GDAPI pandemonium_typed_array_set_type_from(pandemonium_typed_array *p_self, const pandemonium_typed_array *p_array) {
	TypedArray *self = (TypedArray *)p_self;
	const TypedArray *array = (const TypedArray *)p_array;
	self->set_type_from(*array);
}

pandemonium_bool GDAPI pandemonium_typed_array_validate_type_name(const pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name) {
	const TypedArray *self = (const TypedArray *)p_self;
	const StringName *type_name = (const StringName *)p_type_name;
	return self->validate_type_name(*type_name);
}
pandemonium_bool GDAPI pandemonium_typed_array_validate_object_type_name(const pandemonium_typed_array *p_self, const pandemonium_string_name *p_type_name) {
	const TypedArray *self = (const TypedArray *)p_self;
	const StringName *type_name = (const StringName *)p_type_name;
	return self->validate_object_type_name(*type_name);
}
pandemonium_bool GDAPI pandemonium_typed_array_can_take_variant(const pandemonium_typed_array *p_self, const pandemonium_variant *p_value) {
	const TypedArray *self = (const TypedArray *)p_self;
	const Variant *value = (const Variant *)p_value;
	return self->can_take_variant(*value);
}

#ifdef __cplusplus
}
#endif
