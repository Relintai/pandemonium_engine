/**************************************************************************/
/*  string.cpp                                                            */
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

#include "gdn/string.h"

#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(pandemonium_char_string) == sizeof(CharString), "CharString size mismatch");
static_assert(sizeof(pandemonium_string) == sizeof(String), "String size mismatch");
static_assert(sizeof(pandemonium_char_type) == sizeof(CharType), "CharType size mismatch");

pandemonium_int GDAPI pandemonium_char_16_string_size(const pandemonium_char_16_string *p_self) {
	const Char16String *self = (const Char16String *)p_self;
	return self->size();
}
pandemonium_error GDAPI pandemonium_char_16_string_resize(pandemonium_char_16_string *p_self, pandemonium_int p_size) {
	Char16String *self = (Char16String *)p_self;
	return (pandemonium_error)self->resize(p_size);
}

char GDAPI pandemonium_char_16_string_get(const pandemonium_char_16_string *p_self, pandemonium_int p_index) {
	char dest;
	const Char16String *self = (const Char16String *)p_self;
	*((char *)&dest) = self->get(p_index);
	return dest;
}
void GDAPI pandemonium_char_16_string_set(pandemonium_char_16_string *p_self, pandemonium_int p_index, const char16_t p_elem) {
	Char16String *self = (Char16String *)p_self;
	self->set(p_index, p_elem);
}

pandemonium_int GDAPI pandemonium_char_16_string_length(const pandemonium_char_16_string *p_cs) {
	const Char16String *cs = (const Char16String *)p_cs;

	return cs->length();
}
const char16_t GDAPI *pandemonium_char_16_string_get_data(const pandemonium_char_16_string *p_cs) {
	const Char16String *cs = (const Char16String *)p_cs;

	return cs->get_data();
}

void GDAPI pandemonium_char_16_string_destroy(pandemonium_char_16_string *p_cs) {
	Char16String *cs = (Char16String *)p_cs;

	cs->~Char16String();
}

pandemonium_int GDAPI pandemonium_char_string_size(const pandemonium_char_string *p_self) {
	const CharString *self = (const CharString *)p_self;
	return self->size();
}
pandemonium_error GDAPI pandemonium_char_string_resize(pandemonium_char_string *p_self, pandemonium_int p_size) {
	CharString *self = (CharString *)p_self;
	return (pandemonium_error)self->resize(p_size);
}

char GDAPI pandemonium_char_string_get(const pandemonium_char_string *p_self, pandemonium_int p_index) {
	char dest;
	const CharString *self = (const CharString *)p_self;
	*((char *)&dest) = self->get(p_index);
	return dest;
}
void GDAPI pandemonium_char_string_set(pandemonium_char_string *p_self, pandemonium_int p_index, const char *p_elem) {
	CharString *self = (CharString *)p_self;
	const char *elem = (const char *)p_elem;
	self->set(p_index, *elem);
}

pandemonium_int GDAPI pandemonium_char_string_length(const pandemonium_char_string *p_cs) {
	const CharString *cs = (const CharString *)p_cs;

	return cs->length();
}

const char GDAPI *pandemonium_char_string_get_data(const pandemonium_char_string *p_cs) {
	const CharString *cs = (const CharString *)p_cs;

	return cs->get_data();
}

void GDAPI pandemonium_char_string_destroy(pandemonium_char_string *p_cs) {
	CharString *cs = (CharString *)p_cs;

	cs->~CharString();
}

void GDAPI pandemonium_string_remove(pandemonium_string *p_self, pandemonium_int p_index) {
	String *self = (String *)p_self;
	self->remove(p_index);
}

void GDAPI pandemonium_string_clear(pandemonium_string *p_self) {
	String *self = (String *)p_self;
	self->clear();
}

pandemonium_char_type GDAPI pandemonium_string_get(const pandemonium_string *p_self, pandemonium_int p_index) {
	pandemonium_char_type dest;
	const String *self = (const String *)p_self;
	*((CharType *)&dest) = self->get(p_index);
	return dest;
}
void GDAPI pandemonium_string_set(pandemonium_string *p_self, pandemonium_int p_index, const pandemonium_char_type p_elem) {
	String *self = (String *)p_self;
	self->set(p_index, p_elem);
}

pandemonium_int GDAPI pandemonium_string_size(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->size();
}
pandemonium_int GDAPI pandemonium_string_resize(pandemonium_string *p_self, pandemonium_int p_size) {
	String *self = (String *)p_self;
	return self->resize(p_size);
}
pandemonium_int GDAPI pandemonium_string_set_length(pandemonium_string *p_self, pandemonium_int p_length) {
	String *self = (String *)p_self;
	return self->set_length(p_length);
}

const char32_t GDAPI *pandemonium_string_operator_index(pandemonium_string *p_self, const pandemonium_int p_idx) {
	String *self = (String *)p_self;
	return &(self->operator[](p_idx));
}

char32_t GDAPI pandemonium_string_operator_index_const(const pandemonium_string *p_self, const pandemonium_int p_idx) {
	const String *self = (const String *)p_self;
	return self->operator[](p_idx);
}

const char32_t GDAPI *pandemonium_string_get_data(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->get_data();
}

pandemonium_bool GDAPI pandemonium_string_operator_equal(const pandemonium_string *p_self, const pandemonium_string *p_b) {
	const String *self = (const String *)p_self;
	const String *b = (const String *)p_b;
	return *self == *b;
}

pandemonium_bool GDAPI pandemonium_string_operator_less(const pandemonium_string *p_self, const pandemonium_string *p_b) {
	const String *self = (const String *)p_self;
	const String *b = (const String *)p_b;
	return *self < *b;
}

pandemonium_string GDAPI pandemonium_string_operator_plus(const pandemonium_string *p_self, const pandemonium_string *p_b) {
	pandemonium_string ret;
	const String *self = (const String *)p_self;
	const String *b = (const String *)p_b;
	memnew_placement(&ret, String(*self + *b));
	return ret;
}

void GDAPI pandemonium_string_destroy(pandemonium_string *p_self) {
	String *self = (String *)p_self;
	self->~String();
}

/* Standard size stuff */

pandemonium_int GDAPI pandemonium_string_length(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->length();
}

pandemonium_bool GDAPI pandemonium_string_is_valid_string(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_valid_string();
}

void GDAPI pandemonium_string_print_unicode_error(const pandemonium_string *p_self, const pandemonium_string *p_message, pandemonium_bool p_critical) {
	const String *self = (const String *)p_self;
	const String *message = (const String *)p_message;
	self->print_unicode_error(*message, p_critical);
}

/* Helpers */

signed char GDAPI pandemonium_string_casecmp_to(const pandemonium_string *p_self, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;

	return self->casecmp_to(*str);
}

signed char GDAPI pandemonium_string_nocasecmp_to(const pandemonium_string *p_self, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;

	return self->nocasecmp_to(*str);
}
signed char GDAPI pandemonium_string_naturalnocasecmp_to(const pandemonium_string *p_self, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;

	return self->naturalnocasecmp_to(*str);
}

pandemonium_bool GDAPI pandemonium_string_begins_with(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;

	return self->begins_with(*string);
}

pandemonium_bool GDAPI pandemonium_string_begins_with_char_array(const pandemonium_string *p_self, const char *p_char_array) {
	const String *self = (const String *)p_self;

	return self->begins_with(p_char_array);
}

pandemonium_array GDAPI pandemonium_string_bigrams(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<String> return_value = self->bigrams();

	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
};

pandemonium_string GDAPI pandemonium_string_chr(wchar_t p_character) {
	pandemonium_string result;
	memnew_placement(&result, String(String::chr(p_character)));

	return result;
}

pandemonium_bool GDAPI pandemonium_string_ends_with(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;

	return self->ends_with(*string);
}

pandemonium_int GDAPI pandemonium_string_count(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from, pandemonium_int p_to) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->count(*what, p_from, p_to);
}

pandemonium_int GDAPI pandemonium_string_countn(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from, pandemonium_int p_to) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->countn(*what, p_from, p_to);
}

pandemonium_int GDAPI pandemonium_string_find(const pandemonium_string *p_self, pandemonium_string p_what) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->find(*what);
}

pandemonium_int GDAPI pandemonium_string_find_from(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->find(*what, p_from);
}

pandemonium_int GDAPI pandemonium_string_findc(const pandemonium_string *p_self, const char *p_str) {
	const String *self = (const String *)p_self;
	return self->find(p_str);
}
pandemonium_int GDAPI pandemonium_string_findc_from(const pandemonium_string *p_self, const char *p_str, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	return self->find(p_str, p_from);
}
pandemonium_int GDAPI pandemonium_string_find_char(const pandemonium_string *p_self, const pandemonium_char_type p_char) {
	const String *self = (const String *)p_self;
	return self->find_char(p_char);
}
pandemonium_int GDAPI pandemonium_string_find_char_from(const pandemonium_string *p_self, const pandemonium_char_type p_char, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	return self->find_char(p_char, p_from);
}

pandemonium_int GDAPI pandemonium_string_findmk(const pandemonium_string *p_self, const pandemonium_array *p_keys) {
	const String *self = (const String *)p_self;

	Vector<String> keys;
	Array *keys_proxy = (Array *)p_keys;
	keys.resize(keys_proxy->size());
	for (int i = 0; i < keys_proxy->size(); i++) {
		keys.write[i] = (*keys_proxy)[i];
	}

	return self->findmk(keys);
}

pandemonium_int GDAPI pandemonium_string_findmk_from(const pandemonium_string *p_self, const pandemonium_array *p_keys, pandemonium_int p_from) {
	const String *self = (const String *)p_self;

	Vector<String> keys;
	Array *keys_proxy = (Array *)p_keys;
	keys.resize(keys_proxy->size());
	for (int i = 0; i < keys_proxy->size(); i++) {
		keys.write[i] = (*keys_proxy)[i];
	}

	return self->findmk(keys, p_from);
}

pandemonium_int GDAPI pandemonium_string_findmk_from_in_place(const pandemonium_string *p_self, const pandemonium_array *p_keys, pandemonium_int p_from, pandemonium_int *r_key) {
	const String *self = (const String *)p_self;

	Vector<String> keys;
	Array *keys_proxy = (Array *)p_keys;
	keys.resize(keys_proxy->size());
	for (int i = 0; i < keys_proxy->size(); i++) {
		keys.write[i] = (*keys_proxy)[i];
	}

	return self->findmk(keys, p_from, r_key);
}

pandemonium_int GDAPI pandemonium_string_find_first_difference_index(const pandemonium_string *p_self, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;
	return self->find_first_difference_index(*str);
}

pandemonium_bool GDAPI pandemonium_string_is_word_atc(const pandemonium_string *p_self, const pandemonium_int p_index, const char *p_str) {
	const String *self = (const String *)p_self;
	return self->is_word_at(p_index, p_str);
}
pandemonium_bool GDAPI pandemonium_string_is_word_at(const pandemonium_string *p_self, const pandemonium_int p_index, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;
	return self->is_word_at(p_index, *str);
}

pandemonium_int GDAPI pandemonium_string_findn(const pandemonium_string *p_self, pandemonium_string p_what) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->findn(*what);
}

pandemonium_int GDAPI pandemonium_string_findn_from(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->findn(*what, p_from);
}

pandemonium_int GDAPI pandemonium_string_find_last(const pandemonium_string *p_self, pandemonium_string p_what) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->rfind(*what);
}

pandemonium_string GDAPI pandemonium_string_format(const pandemonium_string *p_self, const pandemonium_variant *p_values) {
	const String *self = (const String *)p_self;
	const Variant *values = (const Variant *)p_values;
	pandemonium_string result;
	memnew_placement(&result, String(self->format(*values)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_format_with_custom_placeholder(const pandemonium_string *p_self, const pandemonium_variant *p_values, const char *p_placeholder) {
	const String *self = (const String *)p_self;
	const Variant *values = (const Variant *)p_values;
	String placeholder = String(p_placeholder);
	pandemonium_string result;
	memnew_placement(&result, String(self->format(*values, placeholder)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_hex_encode_buffer(const uint8_t *p_buffer, pandemonium_int p_len) {
	pandemonium_string result;
	memnew_placement(&result, String(String::hex_encode_buffer(p_buffer, p_len)));

	return result;
}

pandemonium_int GDAPI pandemonium_string_hex_to_int(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hex_to_int();
}

pandemonium_int GDAPI pandemonium_string_hex_to_int_without_prefix(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hex_to_int(true);
}

pandemonium_string GDAPI pandemonium_string_newline_to_br(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	pandemonium_string dest;
	memnew_placement(&dest, String(self->newline_to_br()));

	return dest;
}

pandemonium_string GDAPI pandemonium_string_repeat(const pandemonium_string *p_self, pandemonium_int p_count) {
	const String *self = (const String *)p_self;

	pandemonium_string dest;
	memnew_placement(&dest, String(self->repeat(p_count)));

	return dest;
}
pandemonium_string GDAPI pandemonium_string_insert(const pandemonium_string *p_self, pandemonium_int p_at_pos, pandemonium_string p_string) {
	const String *self = (const String *)p_self;
	String *content = (String *)&p_string;
	pandemonium_string result;
	memnew_placement(&result, String(self->insert(p_at_pos, *content)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_bool_num(pandemonium_bool p_val) {
	pandemonium_string dest;
	memnew_placement(&dest, String(String::bool_num(p_val)));
	return dest;
}
pandemonium_string GDAPI pandemonium_string_bool_str(pandemonium_bool p_val) {
	pandemonium_string dest;
	memnew_placement(&dest, String(String::bool_str(p_val)));
	return dest;
}

pandemonium_bool GDAPI pandemonium_string_is_numeric(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_numeric();
}

pandemonium_bool GDAPI pandemonium_string_is_zero(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_zero();
}

pandemonium_bool GDAPI pandemonium_string_is_enclosed_in(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;
	return self->is_enclosed_in(*string);
}

pandemonium_bool GDAPI pandemonium_string_is_subsequence_of(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;

	return self->is_subsequence_of(*string);
}

pandemonium_bool GDAPI pandemonium_string_is_subsequence_ofi(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;

	return self->is_subsequence_ofi(*string);
}

pandemonium_bool GDAPI pandemonium_string_is_quoted(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_quoted();
}

pandemonium_string GDAPI pandemonium_string_lpad(const pandemonium_string *p_self, pandemonium_int p_min_length) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->lpad(p_min_length)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_lpad_with_custom_character(const pandemonium_string *p_self, pandemonium_int p_min_length, const pandemonium_string *p_character) {
	const String *self = (const String *)p_self;
	const String *character = (const String *)p_character;
	pandemonium_string result;
	memnew_placement(&result, String(self->lpad(p_min_length, *character)));

	return result;
}

pandemonium_bool GDAPI pandemonium_string_match(const pandemonium_string *p_self, const pandemonium_string *p_wildcard) {
	const String *self = (const String *)p_self;
	const String *wildcard = (const String *)p_wildcard;

	return self->match(*wildcard);
}

pandemonium_bool GDAPI pandemonium_string_matchn(const pandemonium_string *p_self, const pandemonium_string *p_wildcard) {
	const String *self = (const String *)p_self;
	const String *wildcard = (const String *)p_wildcard;

	return self->matchn(*wildcard);
}

pandemonium_string GDAPI pandemonium_string_md5(const uint8_t *p_md5) {
	pandemonium_string result;
	memnew_placement(&result, String(String::md5(p_md5)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num(double p_num) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num(p_num)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_int64(int64_t p_num, pandemonium_int p_base) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_int64(p_num, p_base)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_int64_capitalized(int64_t p_num, pandemonium_int p_base, pandemonium_bool p_capitalize_hex) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_int64(p_num, p_base, p_capitalize_hex)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_uint64(uint64_t p_num, pandemonium_int p_base) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_uint64(p_num, p_base)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_uint64_capitalized(uint64_t p_num, pandemonium_int p_base, pandemonium_bool p_capitalize_hex) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_uint64(p_num, p_base, p_capitalize_hex)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_real(double p_num) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_real(p_num)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_scientific(double p_num) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num_scientific(p_num)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_num_with_decimals(double p_num, pandemonium_int p_decimals) {
	pandemonium_string result;
	memnew_placement(&result, String(String::num(p_num, p_decimals)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_pad_decimals(const pandemonium_string *p_self, pandemonium_int p_digits) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->pad_decimals(p_digits)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_pad_zeros(const pandemonium_string *p_self, pandemonium_int p_digits) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->pad_zeros(p_digits)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_replace(const pandemonium_string *p_self, pandemonium_string p_key, pandemonium_string p_with) {
	const String *self = (const String *)p_self;
	String *key = (String *)&p_key;
	String *with = (String *)&p_with;
	pandemonium_string result;
	memnew_placement(&result, String(self->replace(*key, *with)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_replacec(const pandemonium_string *p_self, const pandemonium_char_type *p_key, const pandemonium_char_type *p_with) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	memnew_placement(&dest, String(self->replace(p_key, p_with)));
	return dest;
}

pandemonium_string GDAPI pandemonium_string_replacen(const pandemonium_string *p_self, pandemonium_string p_key, pandemonium_string p_with) {
	const String *self = (const String *)p_self;
	String *key = (String *)&p_key;
	String *with = (String *)&p_with;
	pandemonium_string result;
	memnew_placement(&result, String(self->replacen(*key, *with)));

	return result;
}

pandemonium_int GDAPI pandemonium_string_rfind(const pandemonium_string *p_self, pandemonium_string p_what) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->rfind(*what);
}

pandemonium_int GDAPI pandemonium_string_rfindn(const pandemonium_string *p_self, pandemonium_string p_what) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->rfindn(*what);
}

pandemonium_int GDAPI pandemonium_string_rfind_from(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->rfind(*what, p_from);
}

pandemonium_int GDAPI pandemonium_string_rfindn_from(const pandemonium_string *p_self, pandemonium_string p_what, pandemonium_int p_from) {
	const String *self = (const String *)p_self;
	String *what = (String *)&p_what;

	return self->rfindn(*what, p_from);
}

pandemonium_string GDAPI pandemonium_string_replace_first(const pandemonium_string *p_self, pandemonium_string p_key, pandemonium_string p_with) {
	const String *self = (const String *)p_self;
	String *key = (String *)&p_key;
	String *with = (String *)&p_with;
	pandemonium_string result;
	memnew_placement(&result, String(self->replace_first(*key, *with)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_rpad(const pandemonium_string *p_self, pandemonium_int p_min_length) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->rpad(p_min_length)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_rpad_with_custom_character(const pandemonium_string *p_self, pandemonium_int p_min_length, const pandemonium_string *p_character) {
	const String *self = (const String *)p_self;
	const String *character = (const String *)p_character;
	pandemonium_string result;
	memnew_placement(&result, String(self->rpad(p_min_length, *character)));

	return result;
}

pandemonium_real GDAPI pandemonium_string_similarity(const pandemonium_string *p_self, const pandemonium_string *p_string) {
	const String *self = (const String *)p_self;
	const String *string = (const String *)p_string;

	return self->similarity(*string);
}

pandemonium_string GDAPI pandemonium_string_sprintf(const pandemonium_string *p_self, const pandemonium_array *p_values, pandemonium_bool *p_error) {
	const String *self = (const String *)p_self;
	const Array *values = (const Array *)p_values;

	pandemonium_string result;
	String return_value = self->sprintf(*values, p_error);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_quote(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->quote();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_quote_quotechar(const pandemonium_string *p_self, pandemonium_string *p_quotechar) {
	const String *self = (const String *)p_self;
	String *quotechar = (String *)p_quotechar;

	pandemonium_string dest;
	memnew_placement(&dest, String(self->quote(*quotechar)));

	return dest;
}
pandemonium_string GDAPI pandemonium_string_unquote(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	pandemonium_string dest;
	memnew_placement(&dest, String(self->unquote()));

	return dest;
}

pandemonium_string GDAPI pandemonium_string_substr(const pandemonium_string *p_self, pandemonium_int p_from, pandemonium_int p_chars) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->substr(p_from, p_chars)));

	return result;
}
pandemonium_string GDAPI pandemonium_string_substr_index(const pandemonium_string *p_self, const pandemonium_int p_start_index, const pandemonium_int p_end_index) {
	const String *self = (const String *)p_self;

	pandemonium_string dest;
	memnew_placement(&dest, String(self->substr_index(p_start_index, p_end_index)));

	return dest;
}

double GDAPI pandemonium_string_to_double(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->to_double();
}

pandemonium_real GDAPI pandemonium_string_to_float(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->to_float();
}

pandemonium_int GDAPI pandemonium_string_to_int(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->to_int();
}

pandemonium_bool GDAPI pandemonium_string_to_bool(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->to_bool();
}
uint32_t GDAPI pandemonium_string_to_uint(const pandemonium_string *p_self) {
	uint32_t dest;
	const String *self = (const String *)p_self;
	*((uint32_t *)&dest) = self->to_uint();
	return dest;
}

pandemonium_string GDAPI pandemonium_string_capitalize(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->capitalize()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_camelcase_to_underscore(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->camelcase_to_underscore(false)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_camelcase_to_underscore_lowercased(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->camelcase_to_underscore()));

	return result;
}

pandemonium_int GDAPI pandemonium_string_char_to_int(const char *p_what) {
	return String::to_int(p_what);
}

int64_t GDAPI pandemonium_string_wchar_to_int(const wchar_t *p_str) {
	return String::to_int(p_str);
}

pandemonium_int GDAPI pandemonium_string_char_to_int_with_len(const char *p_what, pandemonium_int p_len) {
	return String::to_int(p_what, p_len);
}

int64_t GDAPI pandemonium_string_char_to_int64_with_len(const wchar_t *p_str, int p_len) {
	return String::to_int(p_str, p_len);
}

int64_t GDAPI pandemonium_string_to_intc(const pandemonium_char_type *p_str) {
	const CharType *str = (const CharType *)p_str;
	return String::to_int(str);
}
int64_t GDAPI pandemonium_string_to_intc_len(const pandemonium_char_type *p_str, pandemonium_int p_len) {
	const CharType *str = (const CharType *)p_str;
	return String::to_int(str, p_len);
}
int64_t GDAPI pandemonium_string_to_intc_len_clamp(const pandemonium_char_type *p_str, pandemonium_int p_len, pandemonium_bool p_clamp) {
	const CharType *str = (const CharType *)p_str;
	return String::to_int(str, p_len, p_clamp);
}

int64_t GDAPI pandemonium_string_hex_to_int64(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hex_to_int64(false);
}

int64_t GDAPI pandemonium_string_hex_to_int64_with_prefix(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hex_to_int64();
}

int64_t GDAPI pandemonium_string_to_int64(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->to_int64();
}

float GDAPI pandemonium_string_char_to_float(const char *p_what) {
	return String::to_double(p_what);
}

float GDAPI pandemonium_string_wchar_to_float(const wchar_t *p_str, const wchar_t **r_end) {
	return String::to_double(p_str, r_end);
}

float GDAPI pandemonium_string_pandemonium_char_to_float(const pandemonium_char_type *p_str, const pandemonium_char_type **r_end) {
	return String::to_double(p_str, r_end);
}

double GDAPI pandemonium_string_char_to_double(const char *p_what) {
	return String::to_double(p_what);
}

double GDAPI pandemonium_string_wchar_to_double(const wchar_t *p_str, const wchar_t **r_end) {
	return String::to_double(p_str, r_end);
}

double GDAPI pandemonium_string_pandemonium_char_to_double(const pandemonium_char_type *p_str, const pandemonium_char_type **r_end) {
	return String::to_double(p_str, r_end);
}

uint32_t GDAPI pandemonium_string_num_characters(int64_t p_int) {
	return String::num_characters(p_int);
}

pandemonium_string GDAPI pandemonium_string_get_slice(const pandemonium_string *p_self, pandemonium_string p_splitter, pandemonium_int p_slice) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)&p_splitter;
	pandemonium_string result;
	memnew_placement(&result, String(self->get_slice(*splitter, p_slice)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_get_slicec(const pandemonium_string *p_self, wchar_t p_splitter, pandemonium_int p_slice) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->get_slicec(p_splitter, p_slice)));

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_split(const pandemonium_string *p_self, const pandemonium_string *p_splitter) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->split(*splitter);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_split_allow_empty(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->split(*splitter, p_allow_empty);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_split_maxsplit(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty, const pandemonium_int p_maxsplit) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->split(*splitter, p_allow_empty, p_maxsplit);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_rsplit(const pandemonium_string *p_self, const pandemonium_string *p_splitter) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->rsplit(*splitter);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_rsplit_allow_empty(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->rsplit(*splitter, p_allow_empty);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_string_array GDAPI pandemonium_string_rsplit_maxsplit(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty, const pandemonium_int p_maxsplit) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)p_splitter;

	pandemonium_pool_string_array result;
	memnew_placement(&result, PoolStringArray);
	PoolStringArray *proxy = (PoolStringArray *)&result;
	PoolStringArray::Write proxy_writer = proxy->write();
	Vector<String> tmp_result = self->rsplit(*splitter, p_allow_empty, p_maxsplit);
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_spaces(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<String> return_value = self->split_spaces();

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_floats(const pandemonium_string *p_self, const pandemonium_string *p_splitter) {
	const String *self = (const String *)p_self;
	const String *splitter = (const String *)p_splitter;
	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<float> return_value = self->split_floats(*splitter);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_floats_allow_empty(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;
	const String *splitter = (const String *)p_splitter;
	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<float> return_value = self->split_floats(*splitter, p_allow_empty);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_floats_mk(const pandemonium_string *p_self, const pandemonium_array *p_splitters) {
	const String *self = (const String *)p_self;

	Vector<String> splitters;
	Array *splitter_proxy = (Array *)p_splitters;
	splitters.resize(splitter_proxy->size());
	for (int i = 0; i < splitter_proxy->size(); i++) {
		splitters.write[i] = (*splitter_proxy)[i];
	}

	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<float> return_value = self->split_floats_mk(splitters);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_floats_mk_allow_empty(const pandemonium_string *p_self, const pandemonium_array *p_splitters, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;

	Vector<String> splitters;
	Array *splitter_proxy = (Array *)p_splitters;
	splitters.resize(splitter_proxy->size());
	for (int i = 0; i < splitter_proxy->size(); i++) {
		splitters.write[i] = (*splitter_proxy)[i];
	}

	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<float> return_value = self->split_floats_mk(splitters, p_allow_empty);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_ints(const pandemonium_string *p_self, const pandemonium_string *p_splitter) {
	const String *self = (const String *)p_self;
	const String *splitter = (const String *)p_splitter;
	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<int> return_value = self->split_ints(*splitter);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_ints_allow_empty(const pandemonium_string *p_self, const pandemonium_string *p_splitter, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;
	const String *splitter = (const String *)p_splitter;
	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<int> return_value = self->split_ints(*splitter, p_allow_empty);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_ints_mk(const pandemonium_string *p_self, const pandemonium_array *p_splitters) {
	const String *self = (const String *)p_self;

	Vector<String> splitters;
	Array *splitter_proxy = (Array *)p_splitters;
	splitters.resize(splitter_proxy->size());
	for (int i = 0; i < splitter_proxy->size(); i++) {
		splitters.write[i] = (*splitter_proxy)[i];
	}

	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<int> return_value = self->split_ints_mk(splitters);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_array GDAPI pandemonium_string_split_ints_mk_allow_empty(const pandemonium_string *p_self, const pandemonium_array *p_splitters, const pandemonium_bool p_allow_empty) {
	const String *self = (const String *)p_self;

	Vector<String> splitters;
	Array *splitter_proxy = (Array *)p_splitters;
	splitters.resize(splitter_proxy->size());
	for (int i = 0; i < splitter_proxy->size(); i++) {
		splitters.write[i] = (*splitter_proxy)[i];
	}

	pandemonium_array result;
	memnew_placement(&result, Array);
	Array *proxy = (Array *)&result;
	Vector<int> return_value = self->split_ints_mk(splitters, p_allow_empty);

	proxy->resize(return_value.size());
	for (int i = 0; i < return_value.size(); i++) {
		(*proxy)[i] = return_value[i];
	}

	return result;
}

pandemonium_string GDAPI pandemonium_string_join(const pandemonium_string *p_self, const pandemonium_array *p_parts) {
	const String *self = (const String *)p_self;

	const Array *parts_proxy = (const Array *)p_parts;
	Vector<String> parts;
	parts.resize(parts_proxy->size());
	for (int i = 0; i < parts_proxy->size(); i++) {
		parts.write[i] = (*parts_proxy)[i];
	}

	pandemonium_string str;
	String *s = (String *)&str;
	memnew_placement(s, String);
	*s = self->join(parts);
	return str;
}

pandemonium_string GDAPI pandemonium_string_get_with_code_lines(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->get_with_code_lines();
	return dest;
}

pandemonium_int GDAPI pandemonium_string_get_slice_count(const pandemonium_string *p_self, pandemonium_string p_splitter) {
	const String *self = (const String *)p_self;
	String *splitter = (String *)&p_splitter;

	return self->get_slice_count(*splitter);
}

pandemonium_char_type GDAPI pandemonium_string_char_lowercase(pandemonium_char_type p_char) {
	return String::char_lowercase(p_char);
}

pandemonium_char_type GDAPI pandemonium_string_char_uppercase(pandemonium_char_type p_char) {
	return String::char_uppercase(p_char);
}

pandemonium_string GDAPI pandemonium_string_to_lower(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->to_lower()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_to_upper(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->to_upper()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_get_basename(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->get_basename()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_get_extension(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->get_extension()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_left(const pandemonium_string *p_self, pandemonium_int p_pos) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->left(p_pos)));

	return result;
}

pandemonium_char_type GDAPI pandemonium_string_unicode_at(const pandemonium_string *p_self, pandemonium_int p_idx) {
	pandemonium_char_type dest;
	const String *self = (const String *)p_self;
	*((CharType *)&dest) = self->unicode_at(p_idx);
	return dest;
}

pandemonium_char_type GDAPI pandemonium_string_ord_at(const pandemonium_string *p_self, pandemonium_int p_idx) {
	const String *self = (const String *)p_self;

	return self->ord_at(p_idx);
}

pandemonium_string GDAPI pandemonium_string_plus_file(const pandemonium_string *p_self, const pandemonium_string *p_file) {
	const String *self = (const String *)p_self;
	const String *file = (const String *)p_file;
	pandemonium_string result;
	memnew_placement(&result, String(self->plus_file(*file)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_right(const pandemonium_string *p_self, pandemonium_int p_pos) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->right(p_pos)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_indent(const pandemonium_string *p_self, const pandemonium_string *p_prefix) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	const String *prefix = (const String *)p_prefix;
	*((String *)&dest) = self->indent(*prefix);
	return dest;
}

pandemonium_string GDAPI pandemonium_string_strip_edges(const pandemonium_string *p_self, pandemonium_bool p_left, pandemonium_bool p_right) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->strip_edges(p_left, p_right)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_strip_escapes(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->strip_escapes()));

	return result;
}
pandemonium_string GDAPI pandemonium_string_lstrip(const pandemonium_string *p_self, const pandemonium_string *p_chars) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	const String *chars = (const String *)p_chars;
	*((String *)&dest) = self->lstrip(*chars);
	return dest;
}
void GDAPI pandemonium_string_erase(pandemonium_string *p_self, pandemonium_int p_pos, pandemonium_int p_chars) {
	String *self = (String *)p_self;

	return self->erase(p_pos, p_chars);
}

pandemonium_char_string GDAPI pandemonium_string_ascii(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_char_string result;

	memnew_placement(&result, CharString(self->ascii()));

	return result;
}

pandemonium_char_string GDAPI pandemonium_string_ascii_extended(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	pandemonium_char_string result;

	memnew_placement(&result, CharString(self->ascii(true)));

	return result;
}

pandemonium_char_string GDAPI pandemonium_string_utf8(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	pandemonium_char_string result;

	memnew_placement(&result, CharString(self->utf8()));

	return result;
}

pandemonium_error GDAPI pandemonium_string_parse_utf8(pandemonium_string *p_self, const char *p_utf8) {
	String *self = (String *)p_self;

	return (pandemonium_error)self->parse_utf8(p_utf8);
}

pandemonium_error GDAPI pandemonium_string_parse_utf8_with_len(pandemonium_string *p_self, const char *p_utf8, pandemonium_int p_len) {
	String *self = (String *)p_self;

	return (pandemonium_error)self->parse_utf8(p_utf8, p_len);
}

pandemonium_error GDAPI pandemonium_string_parse_utf8_with_len_skip_cr(pandemonium_string *p_self, const char *p_utf8, pandemonium_int p_len, pandemonium_bool p_skip_cr) {
	String *self = (String *)p_self;

	return (pandemonium_error)self->parse_utf8(p_utf8, p_len, p_skip_cr);
}

pandemonium_string GDAPI pandemonium_string_chars_to_utf8(const char *p_utf8) {
	pandemonium_string result;
	memnew_placement(&result, String(String::utf8(p_utf8)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_chars_to_utf8_with_len(const char *p_utf8, pandemonium_int p_len) {
	pandemonium_string result;
	memnew_placement(&result, String(String::utf8(p_utf8, p_len)));

	return result;
}

pandemonium_int GDAPI pandemonium_string_utf8_byte_length(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->utf8_byte_length();
}

pandemonium_char_16_string GDAPI pandemonium_string_utf16(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	pandemonium_char_16_string result;
	memnew_placement(&result, Char16String(self->utf16()));

	return result;
}

pandemonium_error GDAPI pandemonium_string_parse_utf16(pandemonium_string *p_self, const char16_t *p_utf16) {
	String *self = (String *)p_self;

	return (pandemonium_error)self->parse_utf16(p_utf16);
}

pandemonium_error GDAPI pandemonium_string_parse_utf16_with_len(pandemonium_string *p_self, const char16_t *p_utf16, pandemonium_int p_len) {
	String *self = (String *)p_self;

	return (pandemonium_error)self->parse_utf16(p_utf16, p_len);
}

pandemonium_string GDAPI pandemonium_string_chars_to_utf16(const char16_t *p_utf16) {
	pandemonium_string result;
	memnew_placement(&result, String(String::utf16(p_utf16)));

	return result;
}

pandemonium_string GDAPI pandemonium_string_chars_to_utf16_with_len(const char16_t *p_utf16, pandemonium_int p_len) {
	pandemonium_string result;
	memnew_placement(&result, String(String::utf16(p_utf16, p_len)));

	return result;
}

pandemonium_int GDAPI pandemonium_string_utf16_byte_length(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->utf16_byte_length();
}

uint32_t GDAPI pandemonium_string_hash(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hash();
}

uint64_t GDAPI pandemonium_string_hash64(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->hash64();
}

uint32_t GDAPI pandemonium_string_hash_chars(const char *p_cstr) {
	return String::hash(p_cstr);
}

uint32_t GDAPI pandemonium_string_hash_chars_with_len(const char *p_cstr, pandemonium_int p_len) {
	return String::hash(p_cstr, p_len);
}

uint32_t GDAPI pandemonium_string_hash_wchar(const wchar_t *p_str) {
	return String::hash(p_str);
}

uint32_t GDAPI pandemonium_string_hash_wchar_with_len(const wchar_t *p_str, pandemonium_int p_len) {
	return String::hash(p_str, p_len);
}

uint32_t GDAPI pandemonium_string_hashc(const pandemonium_char_type *p_cstr) {
	uint32_t dest;
	*((uint32_t *)&dest) = String::hash(p_cstr);
	return dest;
}
uint32_t GDAPI pandemonium_string_hashc_with_len(const pandemonium_char_type *p_cstr, pandemonium_int p_len) {
	uint32_t dest;
	*((uint32_t *)&dest) = String::hash(p_cstr, p_len);
	return dest;
}

pandemonium_pool_byte_array GDAPI pandemonium_string_md5_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->md5_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_pool_byte_array GDAPI pandemonium_string_sha1_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->sha1_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_string GDAPI pandemonium_string_md5_text(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->md5_text()));

	return result;
}

pandemonium_string GDAPI pandemonium_string_sha1_text(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->sha1_text();
	return dest;
}

pandemonium_pool_byte_array GDAPI pandemonium_string_sha256_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->sha256_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

pandemonium_string GDAPI pandemonium_string_sha256_text(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	memnew_placement(&result, String(self->sha256_text()));

	return result;
}

pandemonium_bool pandemonium_string_empty(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->empty();
}

pandemonium_bool GDAPI pandemonium_string_containsc(const pandemonium_string *p_self, const char *p_str) {
	const String *self = (const String *)p_self;
	return self->contains(p_str);
}
pandemonium_bool GDAPI pandemonium_string_contains(const pandemonium_string *p_self, const pandemonium_string *p_str) {
	const String *self = (const String *)p_self;
	const String *str = (const String *)p_str;
	return self->contains(*str);
}

// path functions
pandemonium_string GDAPI pandemonium_string_get_base_dir(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->get_base_dir();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_get_file(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->get_file();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_humanize_size(uint64_t p_size) {
	pandemonium_string result;
	String return_value = String::humanize_size(p_size);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_bool GDAPI pandemonium_string_is_abs_path(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_abs_path();
}

pandemonium_bool GDAPI pandemonium_string_is_rel_path(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_rel_path();
}

pandemonium_bool GDAPI pandemonium_string_is_resource_file(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_resource_file();
}

pandemonium_string GDAPI pandemonium_string_path_to(const pandemonium_string *p_self, const pandemonium_string *p_path) {
	const String *self = (const String *)p_self;
	String *path = (String *)p_path;
	pandemonium_string result;
	String return_value = self->path_to(*path);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_path_to_file(const pandemonium_string *p_self, const pandemonium_string *p_path) {
	const String *self = (const String *)p_self;
	String *path = (String *)p_path;
	pandemonium_string result;
	String return_value = self->path_to_file(*path);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_simplify_path(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->simplify_path();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_bool GDAPI pandemonium_string_is_network_share_path(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_network_share_path();
}
pandemonium_string GDAPI pandemonium_string_append_pathc(const pandemonium_string *p_self, const char *p_path) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->append_path(p_path);
	return dest;
}
pandemonium_string GDAPI pandemonium_string_append_path(const pandemonium_string *p_self, const pandemonium_string *p_path) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	const String *path = (const String *)p_path;
	*((String *)&dest) = self->append_path(*path);
	return dest;
}
pandemonium_string GDAPI pandemonium_string_path_clean_end_slash(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->path_clean_end_slash();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_path_ensure_end_slash(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->path_ensure_end_slash();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_path_get_prev_dir(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->path_get_prev_dir();
	return dest;
}

pandemonium_string GDAPI pandemonium_string_c_escape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->c_escape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_c_escape_multiline(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->c_escape_multiline();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_c_unescape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->c_unescape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_http_escape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->http_escape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_http_unescape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->http_unescape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_uri_encode(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->uri_encode();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_uri_decode(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->uri_decode();
	return dest;
}

pandemonium_string GDAPI pandemonium_string_json_escape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->json_escape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_word_wrap(const pandemonium_string *p_self, pandemonium_int p_chars_per_line) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->word_wrap(p_chars_per_line);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_error GDAPI pandemonium_string_parse_url(const pandemonium_string *p_self, pandemonium_string *r_scheme, pandemonium_string *r_host, pandemonium_int *r_port, pandemonium_string *r_path) {
	const String *self = (const String *)p_self;
	String *scheme = (String *)r_scheme;
	String *host = (String *)r_host;
	String *path = (String *)r_path;
	return (pandemonium_error)self->parse_url(*scheme, *host, *r_port, *path);
}

pandemonium_string GDAPI pandemonium_string_xml_escape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->xml_escape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_xml_escape_with_quotes(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->xml_escape(true);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_xml_unescape(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->xml_unescape();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_percent_decode(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->percent_decode();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_percent_encode(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->percent_encode();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_property_name_encode(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->property_name_encode();
	return dest;
}

pandemonium_bool GDAPI pandemonium_string_is_valid_float(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_valid_float();
}

pandemonium_bool GDAPI pandemonium_string_is_valid_hex_number(const pandemonium_string *p_self, pandemonium_bool p_with_prefix) {
	const String *self = (const String *)p_self;

	return self->is_valid_hex_number(p_with_prefix);
}

pandemonium_bool GDAPI pandemonium_string_is_valid_html_color(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_valid_html_color();
}

pandemonium_string GDAPI pandemonium_string_get_invalid_node_name_characters() {
	pandemonium_string dest;
	*((String *)&dest) = String::get_invalid_node_name_characters();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_validate_node_name(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->validate_node_name();
	return dest;
}
pandemonium_string GDAPI pandemonium_string_validate_identifier(const pandemonium_string *p_self) {
	pandemonium_string dest;
	const String *self = (const String *)p_self;
	*((String *)&dest) = self->validate_identifier();
	return dest;
}

pandemonium_bool GDAPI pandemonium_string_is_valid_identifier(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_valid_identifier();
}

pandemonium_bool GDAPI pandemonium_string_is_valid_integer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_valid_integer();
}

pandemonium_bool GDAPI pandemonium_string_is_valid_ip_address(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;

	return self->is_valid_ip_address();
}

pandemonium_bool GDAPI pandemonium_string_is_valid_filename(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_valid_filename();
}
pandemonium_bool GDAPI pandemonium_string_is_valid_bool(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_valid_bool();
}
pandemonium_bool GDAPI pandemonium_string_is_valid_unsigned_integer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	return self->is_valid_unsigned_integer();
}

pandemonium_string GDAPI pandemonium_string_dedent(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	pandemonium_string result;
	String return_value = self->dedent();
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_trim_prefix(const pandemonium_string *p_self, const pandemonium_string *p_prefix) {
	const String *self = (const String *)p_self;
	String *prefix = (String *)p_prefix;
	pandemonium_string result;
	String return_value = self->trim_prefix(*prefix);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_trim_suffix(const pandemonium_string *p_self, const pandemonium_string *p_suffix) {
	const String *self = (const String *)p_self;
	String *suffix = (String *)p_suffix;
	pandemonium_string result;
	String return_value = self->trim_suffix(*suffix);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_string GDAPI pandemonium_string_rstrip(const pandemonium_string *p_self, const pandemonium_string *p_chars) {
	const String *self = (const String *)p_self;
	String *chars = (String *)p_chars;
	pandemonium_string result;
	String return_value = self->rstrip(*chars);
	memnew_placement(&result, String(return_value));

	return result;
}

pandemonium_pool_byte_array GDAPI pandemonium_string_to_ascii_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->to_ascii_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}
pandemonium_pool_byte_array GDAPI pandemonium_string_to_utf8_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->to_utf8_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}
pandemonium_pool_byte_array GDAPI pandemonium_string_to_utf16_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->to_utf16_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}
pandemonium_pool_byte_array GDAPI pandemonium_string_to_utf32_buffer(const pandemonium_string *p_self) {
	const String *self = (const String *)p_self;
	Vector<uint8_t> tmp_result = self->to_utf32_buffer();

	pandemonium_pool_byte_array result;
	memnew_placement(&result, PoolByteArray);
	PoolByteArray *proxy = (PoolByteArray *)&result;
	PoolByteArray::Write proxy_writer = proxy->write();
	proxy->resize(tmp_result.size());

	for (int i = 0; i < tmp_result.size(); i++) {
		proxy_writer[i] = tmp_result[i];
	}

	return result;
}

void GDAPI pandemonium_string_new(pandemonium_string *r_dest) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String);
}

void GDAPI pandemonium_string_new_copy(pandemonium_string *r_dest, const pandemonium_string *p_src) {
	String *dest = (String *)r_dest;
	const String *src = (const String *)p_src;
	memnew_placement(dest, String(*src));
}

void GDAPI pandemonium_string_new_char(pandemonium_string *r_dest, const char *p_contents) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents));
}
void GDAPI pandemonium_string_new_char_clip_to_len(pandemonium_string *r_dest, const char *p_contents, const int p_size) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents, p_size));
}
void GDAPI pandemonium_string_new_wchar(pandemonium_string *r_dest, const wchar_t *p_contents) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents));
}
void GDAPI pandemonium_string_new_wchar_clip_to_len(pandemonium_string *r_dest, const wchar_t *p_contents, const int p_size) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents, p_size));
}
void GDAPI pandemonium_string_newc(pandemonium_string *r_dest, const pandemonium_char_type *p_contents) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents));
}
void GDAPI pandemonium_string_newc_clip_to_len(pandemonium_string *r_dest, const pandemonium_char_type *p_contents, const int p_size) {
	String *dest = (String *)r_dest;
	memnew_placement(dest, String(p_contents, p_size));
}

#ifdef __cplusplus
}
#endif
