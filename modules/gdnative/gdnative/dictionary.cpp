/*************************************************************************/
/*  dictionary.cpp                                                       */
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

#include "gdn/dictionary.h"

#include "core/variant/variant.h"
// core/variant.h before to avoid compile errors with MSVC
#include "core/io/json.h"
#include "core/variant/dictionary.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_dictionary) == sizeof(Dictionary), "Dictionary size mismatch");

void GDAPI pandemonium_dictionary_new(pandemonium_dictionary *r_dest) {
	Dictionary *dest = (Dictionary *)r_dest;
	memnew_placement(dest, Dictionary);
}

void GDAPI pandemonium_dictionary_new_copy(pandemonium_dictionary *r_dest, const pandemonium_dictionary *p_src) {
	Dictionary *dest = (Dictionary *)r_dest;
	const Dictionary *src = (const Dictionary *)p_src;
	memnew_placement(dest, Dictionary(*src));
}

void GDAPI pandemonium_dictionary_destroy(pandemonium_dictionary *p_self) {
	Dictionary *self = (Dictionary *)p_self;
	self->~Dictionary();
}

pandemonium_dictionary GDAPI pandemonium_dictionary_duplicate(const pandemonium_dictionary *p_self, const pandemonium_bool p_deep) {
	const Dictionary *self = (const Dictionary *)p_self;
	pandemonium_dictionary res;
	Dictionary *val = (Dictionary *)&res;
	memnew_placement(val, Dictionary);
	*val = self->duplicate(p_deep);
	return res;
}

pandemonium_int GDAPI pandemonium_dictionary_size(const pandemonium_dictionary *p_self) {
	const Dictionary *self = (const Dictionary *)p_self;
	return self->size();
}

pandemonium_bool GDAPI pandemonium_dictionary_empty(const pandemonium_dictionary *p_self) {
	const Dictionary *self = (const Dictionary *)p_self;
	return self->empty();
}

void GDAPI pandemonium_dictionary_clear(pandemonium_dictionary *p_self) {
	Dictionary *self = (Dictionary *)p_self;
	self->clear();
}

pandemonium_bool GDAPI pandemonium_dictionary_has(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	return self->has(*key);
}

pandemonium_bool GDAPI pandemonium_dictionary_has_all(const pandemonium_dictionary *p_self, const pandemonium_array *p_keys) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Array *keys = (const Array *)p_keys;
	return self->has_all(*keys);
}

void GDAPI pandemonium_dictionary_erase(pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	Dictionary *self = (Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	self->erase(*key);
}

pandemonium_int GDAPI pandemonium_dictionary_hash(const pandemonium_dictionary *p_self) {
	const Dictionary *self = (const Dictionary *)p_self;
	return self->hash();
}

pandemonium_array GDAPI pandemonium_dictionary_keys(const pandemonium_dictionary *p_self) {
	pandemonium_array dest;
	const Dictionary *self = (const Dictionary *)p_self;
	memnew_placement(&dest, Array(self->keys()));
	return dest;
}

pandemonium_array GDAPI pandemonium_dictionary_values(const pandemonium_dictionary *p_self) {
	pandemonium_array dest;
	const Dictionary *self = (const Dictionary *)p_self;
	memnew_placement(&dest, Array(self->values()));
	return dest;
}

pandemonium_variant GDAPI pandemonium_dictionary_get(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	pandemonium_variant raw_dest;
	Variant *dest = (Variant *)&raw_dest;
	const Dictionary *self = (const Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	memnew_placement(dest, Variant(self->operator[](*key)));
	return raw_dest;
}

void GDAPI pandemonium_dictionary_set(pandemonium_dictionary *p_self, const pandemonium_variant *p_key, const pandemonium_variant *p_value) {
	Dictionary *self = (Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	const Variant *value = (const Variant *)p_value;
	self->operator[](*key) = *value;
}

pandemonium_variant GDAPI *pandemonium_dictionary_operator_index(pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	Dictionary *self = (Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	return (pandemonium_variant *)&self->operator[](*key);
}

const pandemonium_variant GDAPI *pandemonium_dictionary_operator_index_const(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	return (const pandemonium_variant *)&self->operator[](*key);
}

pandemonium_variant GDAPI *pandemonium_dictionary_next(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	Dictionary *self = (Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	return (pandemonium_variant *)self->next(key);
}

pandemonium_bool GDAPI pandemonium_dictionary_deep_equal(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Dictionary *dictionary = (const Dictionary *)p_dictionary;
	return self->deep_equal(*dictionary);
}
pandemonium_bool GDAPI pandemonium_dictionary_deep_equal_recursion_count(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary, pandemonium_int p_recursion_count) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Dictionary *dictionary = (const Dictionary *)p_dictionary;
	return self->deep_equal(*dictionary, p_recursion_count);
}

pandemonium_bool GDAPI pandemonium_dictionary_operator_equal(const pandemonium_dictionary *p_self, const pandemonium_dictionary *p_b) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Dictionary *b = (const Dictionary *)p_b;
	return *self == *b;
}

pandemonium_string GDAPI pandemonium_dictionary_to_json(const pandemonium_dictionary *p_self) {
	pandemonium_string raw_dest;
	String *dest = (String *)&raw_dest;
	const Dictionary *self = (const Dictionary *)p_self;
	memnew_placement(dest, String(JSON::print(Variant(*self))));
	return raw_dest;
}

pandemonium_bool GDAPI pandemonium_dictionary_erase_with_return(pandemonium_dictionary *p_self, const pandemonium_variant *p_key) {
	Dictionary *self = (Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	return self->erase(*key);
}

pandemonium_variant GDAPI pandemonium_dictionary_get_with_default(const pandemonium_dictionary *p_self, const pandemonium_variant *p_key, const pandemonium_variant *p_default) {
	const Dictionary *self = (const Dictionary *)p_self;
	const Variant *key = (const Variant *)p_key;
	const Variant *def = (const Variant *)p_default;

	pandemonium_variant raw_dest;
	Variant *dest = (Variant *)&raw_dest;
	memnew_placement(dest, Variant(self->get(*key, *def)));

	return raw_dest;
}

void GDAPI pandemonium_dictionary_merge(pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary) {
	Dictionary *self = (Dictionary *)p_self;
	const Dictionary *dictionary = (const Dictionary *)p_dictionary;
	self->merge(*dictionary);
}

void GDAPI pandemonium_dictionary_merge_overwrite(pandemonium_dictionary *p_self, const pandemonium_dictionary *p_dictionary, const pandemonium_bool p_overwrite) {
	Dictionary *self = (Dictionary *)p_self;
	const Dictionary *dictionary = (const Dictionary *)p_dictionary;
	self->merge(*dictionary, p_overwrite);
}

#ifdef __cplusplus
}
#endif
