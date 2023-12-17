/*************************************************************************/
/*  form_validator.cpp                                                   */
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

#include "form_validator.h"

#include "../http/web_server_request.h"

#include <ctype.h>

//FormFieldEntry

PoolStringArray FormFieldEntry::validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	return call("_validate", request, field, data);
}

PoolStringArray FormFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	return PoolStringArray();
}

FormFieldEntry::FormFieldEntry() {
}

FormFieldEntry::~FormFieldEntry() {
}

void FormFieldEntry::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_validate", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::OBJECT, "field", PROPERTY_HINT_RESOURCE_TYPE, "FormField"), PropertyInfo(Variant::STRING, "data")));
	ClassDB::bind_method(D_METHOD("validate", "request", "field", "data"), &FormFieldEntry::validate);
	ClassDB::bind_method(D_METHOD("_validate", "request", "field", "data"), &FormFieldEntry::_validate);
}

//FormExistsFieldEntry

String FormExistsFieldEntry::get_not_exists_error() {
	return _not_exists_error;
}
void FormExistsFieldEntry::set_not_exists_error(const String &val) {
	_not_exists_error = val;
}

PoolStringArray FormExistsFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data == "") {
		errors.push_back(field->get_human_name() + _not_exists_error);
	}

	return errors;
}

FormExistsFieldEntry::FormExistsFieldEntry() {
	_not_exists_error = " field need to exists!";
}
FormExistsFieldEntry::~FormExistsFieldEntry() {
}

void FormExistsFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_not_exists_error"), &FormExistsFieldEntry::get_not_exists_error);
	ClassDB::bind_method(D_METHOD("set_not_exists_error", "val"), &FormExistsFieldEntry::set_not_exists_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "not_exists_error"), "set_not_exists_error", "get_not_exists_error");
}

//FormIntFieldEntry

String FormIntFieldEntry::get_not_int_error() {
	return _not_int_error;
}
void FormIntFieldEntry::set_not_int_error(const String &val) {
	_not_int_error = val;
}

PoolStringArray FormIntFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.empty()) {
		return errors;
	}

	if (!data.is_valid_integer()) {
		errors.push_back(field->get_human_name() + _not_int_error);
	}

	return errors;
}

FormIntFieldEntry::FormIntFieldEntry() {
	_not_int_error = " needs to be an integer.";
}

FormIntFieldEntry::~FormIntFieldEntry() {
}

void FormIntFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_not_int_error"), &FormIntFieldEntry::get_not_int_error);
	ClassDB::bind_method(D_METHOD("set_not_int_error", "val"), &FormIntFieldEntry::set_not_int_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "not_int_error"), "set_not_int_error", "get_not_int_error");
}

//FormFloatFieldEntry

String FormFloatFieldEntry::get_not_float_error() {
	return _not_float_error;
}
void FormFloatFieldEntry::set_not_float_error(const String &val) {
	_not_float_error = val;
}

PoolStringArray FormFloatFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.empty()) {
		return errors;
	}

	if (!data.is_valid_float()) {
		errors.push_back(field->get_human_name() + _not_float_error);
	}

	return errors;
}

FormFloatFieldEntry::FormFloatFieldEntry() {
	_not_float_error = " needs to be an floating point number.";
}
FormFloatFieldEntry::~FormFloatFieldEntry() {
}

void FormFloatFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_not_float_error"), &FormFloatFieldEntry::get_not_float_error);
	ClassDB::bind_method(D_METHOD("set_not_float_error", "val"), &FormFloatFieldEntry::set_not_float_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "not_float_error"), "set_not_float_error", "get_not_float_error");
}

//FormAlphaFieldEntry

String FormAlphaFieldEntry::get_not_alpha_error() {
	return _not_alpha_error;
}
void FormAlphaFieldEntry::set_not_alpha_error(const String &val) {
	_not_alpha_error = val;
}

PoolStringArray FormAlphaFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.length(); ++i) {
		if (!isalpha(data[i])) {
			errors.push_back(field->get_human_name() + _not_alpha_error);

			return errors;
		}
	}

	return errors;
}

FormAlphaFieldEntry::FormAlphaFieldEntry() {
	_not_alpha_error = " needs to only contain characters.";
}
FormAlphaFieldEntry::~FormAlphaFieldEntry() {
}

void FormAlphaFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_not_alpha_error"), &FormAlphaFieldEntry::get_not_alpha_error);
	ClassDB::bind_method(D_METHOD("set_not_alpha_error", "val"), &FormAlphaFieldEntry::set_not_alpha_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "not_alpha_error"), "set_not_alpha_error", "get_not_alpha_error");
}

//FormAlphaNumericFieldEntry

String FormAlphaNumericFieldEntry::get_not_alpha_numeric_error() {
	return _not_alpha_numeric_error;
}
void FormAlphaNumericFieldEntry::set_not_alpha_numeric_error(const String &val) {
	_not_alpha_numeric_error = val;
}

PoolStringArray FormAlphaNumericFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.length(); ++i) {
		if (!isalnum(data[i])) {
			errors.push_back(field->get_human_name() + _not_alpha_numeric_error);

			return errors;
		}
	}

	return errors;
}

FormAlphaNumericFieldEntry::FormAlphaNumericFieldEntry() {
	_not_alpha_numeric_error = " needs to only contain chaarcters of numbers.";
}
FormAlphaNumericFieldEntry::~FormAlphaNumericFieldEntry() {
}

void FormAlphaNumericFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_not_alpha_numeric_error"), &FormAlphaNumericFieldEntry::get_not_alpha_numeric_error);
	ClassDB::bind_method(D_METHOD("set_not_alpha_numeric_error", "val"), &FormAlphaNumericFieldEntry::set_not_alpha_numeric_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "not_alpha_numeric_error"), "set_not_alpha_numeric_error", "get_not_alpha_numeric_error");
}

//FormNeedsLowercaseCharacterFieldEntry

String FormNeedsLowercaseCharacterFieldEntry::get_does_not_have_lowercase_error() {
	return _does_not_have_lowercase_error;
}
void FormNeedsLowercaseCharacterFieldEntry::set_does_not_have_lowercase_error(const String &val) {
	_does_not_have_lowercase_error = val;
}

PoolStringArray FormNeedsLowercaseCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.length(); ++i) {
		if (islower(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->get_human_name() + _does_not_have_lowercase_error);

	return errors;
}

FormNeedsLowercaseCharacterFieldEntry::FormNeedsLowercaseCharacterFieldEntry() {
	_does_not_have_lowercase_error = " needs at least one lowercase character!";
}
FormNeedsLowercaseCharacterFieldEntry::~FormNeedsLowercaseCharacterFieldEntry() {
}

void FormNeedsLowercaseCharacterFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_does_not_have_lowercase_error"), &FormNeedsLowercaseCharacterFieldEntry::get_does_not_have_lowercase_error);
	ClassDB::bind_method(D_METHOD("set_does_not_have_lowercase_error", "val"), &FormNeedsLowercaseCharacterFieldEntry::set_does_not_have_lowercase_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_lowercase_error"), "set_does_not_have_lowercase_error", "get_does_not_have_lowercase_error");
}

//FormNeedsUppercaseCharacterFieldEntry

String FormNeedsUppercaseCharacterFieldEntry::get_does_not_have_uppercase_error() {
	return _does_not_have_uppercase_error;
}
void FormNeedsUppercaseCharacterFieldEntry::set_does_not_have_uppercase_error(const String &val) {
	_does_not_have_uppercase_error = val;
}

PoolStringArray FormNeedsUppercaseCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	for (int i = 0; i < data.length(); ++i) {
		if (isupper(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->get_human_name() + _does_not_have_uppercase_error);

	return errors;
}

FormNeedsUppercaseCharacterFieldEntry::FormNeedsUppercaseCharacterFieldEntry() {
	_does_not_have_uppercase_error = " needs at least one uppercase character!";
}
FormNeedsUppercaseCharacterFieldEntry::~FormNeedsUppercaseCharacterFieldEntry() {
}

void FormNeedsUppercaseCharacterFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_does_not_have_uppercase_error"), &FormNeedsUppercaseCharacterFieldEntry::get_does_not_have_uppercase_error);
	ClassDB::bind_method(D_METHOD("set_does_not_have_uppercase_error", "val"), &FormNeedsUppercaseCharacterFieldEntry::set_does_not_have_uppercase_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_uppercase_error"), "set_does_not_have_uppercase_error", "get_does_not_have_uppercase_error");
}

//FormNeedsOtherCharacterFieldEntry

String FormNeedsOtherCharacterFieldEntry::get_does_not_have_other_error() {
	return _does_not_have_other_error;
}
void FormNeedsOtherCharacterFieldEntry::set_does_not_have_other_error(const String &val) {
	_does_not_have_other_error = val;
}

PoolStringArray FormNeedsOtherCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	for (int i = 0; i < data.length(); ++i) {
		if (!isalnum(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->get_human_name() + _does_not_have_other_error);

	return errors;
}

FormNeedsOtherCharacterFieldEntry::FormNeedsOtherCharacterFieldEntry() {
	_does_not_have_other_error = " needs at least one other character!";
}
FormNeedsOtherCharacterFieldEntry::~FormNeedsOtherCharacterFieldEntry() {
}

void FormNeedsOtherCharacterFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_does_not_have_other_error"), &FormNeedsOtherCharacterFieldEntry::get_does_not_have_other_error);
	ClassDB::bind_method(D_METHOD("set_does_not_have_other_error", "val"), &FormNeedsOtherCharacterFieldEntry::set_does_not_have_other_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_other_error"), "set_does_not_have_other_error", "get_does_not_have_other_error");
}

//FormMinimumLengthFieldEntry

int FormMinimumLengthFieldEntry::get_min_length() {
	return _min_length;
}
void FormMinimumLengthFieldEntry::set_min_length(const int &val) {
	_min_length = val;
}

String FormMinimumLengthFieldEntry::get_does_not_have_min_length_errorf() {
	return _does_not_have_min_length_errorf;
}
void FormMinimumLengthFieldEntry::set_does_not_have_min_length_errorf(const String &val) {
	_does_not_have_min_length_errorf = val;
}

String FormMinimumLengthFieldEntry::get_does_not_have_min_length_errors() {
	return _does_not_have_min_length_errors;
}
void FormMinimumLengthFieldEntry::set_does_not_have_min_length_errors(const String &val) {
	_does_not_have_min_length_errors = val;
}

PoolStringArray FormMinimumLengthFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	if (data.length() < _min_length) {
		errors.push_back(field->get_human_name() + _does_not_have_min_length_errorf + itos(_min_length) + _does_not_have_min_length_errors);
	}

	return errors;
}

FormMinimumLengthFieldEntry::FormMinimumLengthFieldEntry() {
	_does_not_have_min_length_errorf = " needs at least ";
	_does_not_have_min_length_errors = " characters!";

	_min_length = 5;
}
FormMinimumLengthFieldEntry::~FormMinimumLengthFieldEntry() {
}

void FormMinimumLengthFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_min_length"), &FormMinimumLengthFieldEntry::get_min_length);
	ClassDB::bind_method(D_METHOD("set_min_length", "val"), &FormMinimumLengthFieldEntry::set_min_length);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_length"), "set_min_length", "get_min_length");

	ClassDB::bind_method(D_METHOD("get_does_not_have_min_length_errorf"), &FormMinimumLengthFieldEntry::get_does_not_have_min_length_errorf);
	ClassDB::bind_method(D_METHOD("set_does_not_have_min_length_errorf", "val"), &FormMinimumLengthFieldEntry::set_does_not_have_min_length_errorf);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_min_length_errorf"), "set_does_not_have_min_length_errorf", "get_does_not_have_min_length_errorf");

	ClassDB::bind_method(D_METHOD("get_does_not_have_min_length_errors"), &FormMinimumLengthFieldEntry::get_does_not_have_min_length_errors);
	ClassDB::bind_method(D_METHOD("set_does_not_have_min_length_errors", "val"), &FormMinimumLengthFieldEntry::set_does_not_have_min_length_errors);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_min_length_errors"), "set_does_not_have_min_length_errors", "get_does_not_have_min_length_errors");
}

//FormMaximumLengthFieldEntry

int FormMaximumLengthFieldEntry::get_max_length() {
	return _max_length;
}
void FormMaximumLengthFieldEntry::set_max_length(const int &val) {
	_max_length = val;
}

String FormMaximumLengthFieldEntry::get_does_not_have_max_length_errorf() {
	return _does_not_have_max_length_errorf;
}
void FormMaximumLengthFieldEntry::set_does_not_have_max_length_errorf(const String &val) {
	_does_not_have_max_length_errorf = val;
}

String FormMaximumLengthFieldEntry::get_does_not_have_max_length_errors() {
	return _does_not_have_max_length_errors;
}
void FormMaximumLengthFieldEntry::set_does_not_have_max_length_errors(const String &val) {
	_does_not_have_max_length_errors = val;
}

PoolStringArray FormMaximumLengthFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.length() > _max_length) {
		errors.push_back(field->get_human_name() + _does_not_have_max_length_errorf + itos(_max_length) + _does_not_have_max_length_errors);
	}

	return errors;
}

FormMaximumLengthFieldEntry::FormMaximumLengthFieldEntry() {
	_does_not_have_max_length_errorf = " needs at most ";
	_does_not_have_max_length_errors = " characters!";

	_max_length = 10;
}
FormMaximumLengthFieldEntry::~FormMaximumLengthFieldEntry() {
}

void FormMaximumLengthFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_max_length"), &FormMaximumLengthFieldEntry::get_max_length);
	ClassDB::bind_method(D_METHOD("set_max_length", "val"), &FormMaximumLengthFieldEntry::set_max_length);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_length"), "set_max_length", "get_max_length");

	ClassDB::bind_method(D_METHOD("get_does_not_have_max_length_errorf"), &FormMaximumLengthFieldEntry::get_does_not_have_max_length_errorf);
	ClassDB::bind_method(D_METHOD("set_does_not_have_max_length_errorf", "val"), &FormMaximumLengthFieldEntry::set_does_not_have_max_length_errorf);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_max_length_errorf"), "set_does_not_have_max_length_errorf", "get_does_not_have_max_length_errorf");

	ClassDB::bind_method(D_METHOD("get_does_not_have_max_length_errors"), &FormMaximumLengthFieldEntry::get_does_not_have_max_length_errors);
	ClassDB::bind_method(D_METHOD("set_does_not_have_max_length_errors", "val"), &FormMaximumLengthFieldEntry::set_does_not_have_max_length_errors);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_have_max_length_errors"), "set_does_not_have_max_length_errors", "get_does_not_have_max_length_errors");
}

//FormEmailFieldEntry

String FormEmailFieldEntry::get_email_format_error() {
	return _email_format_error;
}
void FormEmailFieldEntry::set_email_format_error(const String &val) {
	_email_format_error = val;
}

PoolStringArray FormEmailFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.length() == 0) {
		errors.push_back(field->get_human_name() + _email_format_error);

		return errors;
	}

	if (!isalpha(data[0])) {
		errors.push_back(field->get_human_name() + _email_format_error);

		return errors;
	}

	int dot_pos = -1;
	int at_pos = -1;

	for (int i = 0; i < data.length(); ++i) {
		if (data[i] == '.') {
			if (dot_pos != -1) {
				errors.push_back(field->get_human_name() + _email_format_error);

				return errors;
			}

			dot_pos = i;

			return errors;
		}
	}

	if (dot_pos == -1) {
		errors.push_back(field->get_human_name() + _email_format_error);

		return errors;
	}

	for (int i = 0; i < data.length(); ++i) {
		if (data[i] == '@') {
			if (at_pos != -1) {
				errors.push_back(field->get_human_name() + _email_format_error);

				return errors;
			}

			at_pos = i;

			return errors;
		}
	}

	if (at_pos == -1) {
		errors.push_back(field->get_human_name() + _email_format_error);

		return errors;
	}

	for (int i = 0; i < data.length(); ++i) {
		if (i == at_pos || i == dot_pos) {
			continue;
		}

		if (!isalnum(data[i])) {
			errors.push_back(field->get_human_name() + _email_format_error);

			return errors;
		}
	}

	return errors;
}

FormEmailFieldEntry::FormEmailFieldEntry() {
	_email_format_error = " is invalid!";
}
FormEmailFieldEntry::~FormEmailFieldEntry() {
}

void FormEmailFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_email_format_error"), &FormEmailFieldEntry::get_email_format_error);
	ClassDB::bind_method(D_METHOD("set_email_format_error", "val"), &FormEmailFieldEntry::set_email_format_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "email_format_error"), "set_email_format_error", "get_email_format_error");
}

//FormNeedToMatchOtherFieldEntry

String FormNeedToMatchOtherFieldEntry::get_other_field() {
	return _other_field;
}
void FormNeedToMatchOtherFieldEntry::set_other_field(const String &val) {
	_other_field = val;
}

String FormNeedToMatchOtherFieldEntry::get_does_not_match_error() {
	return _does_not_match_error;
}
void FormNeedToMatchOtherFieldEntry::set_does_not_match_error(const String &val) {
	_does_not_match_error = val;
}

PoolStringArray FormNeedToMatchOtherFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data != request->get_parameter(_other_field)) {
		errors.push_back(field->get_human_name() + _does_not_match_error + field->get_field_name() + ".");
	}

	return errors;
}

FormNeedToMatchOtherFieldEntry::FormNeedToMatchOtherFieldEntry() {
	_does_not_match_error = " does not match ";
}
FormNeedToMatchOtherFieldEntry::~FormNeedToMatchOtherFieldEntry() {
}

void FormNeedToMatchOtherFieldEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_other_field"), &FormNeedToMatchOtherFieldEntry::get_other_field);
	ClassDB::bind_method(D_METHOD("set_other_field", "val"), &FormNeedToMatchOtherFieldEntry::set_other_field);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "other_field"), "set_other_field", "get_other_field");

	ClassDB::bind_method(D_METHOD("get_does_not_match_error"), &FormNeedToMatchOtherFieldEntry::get_does_not_match_error);
	ClassDB::bind_method(D_METHOD("set_does_not_match_error", "val"), &FormNeedToMatchOtherFieldEntry::set_does_not_match_error);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "does_not_match_error"), "set_does_not_match_error", "get_does_not_match_error");
}

//FormField

String FormField::get_field_name() const {
	return _field_name;
}
void FormField::set_field_name(const String &val) {
	_field_name = val;
}

String FormField::get_human_name() const {
	return _human_name;
}
void FormField::set_human_name(const String &val) {
	_human_name = val;
}

bool FormField::get_ignore_if_not_exists() const {
	return _ignore_if_not_exists;
}
void FormField::set_ignore_if_not_exists(const bool &val) {
	_ignore_if_not_exists = val;
}

bool FormField::get_ignore_if_other_field_not_exists() const {
	return _ignore_if_other_field_not_exists;
}
void FormField::set_ignore_if_other_field_not_exists(const bool &val) {
	_ignore_if_other_field_not_exists = val;
}

String FormField::get_ignore_if_other_field_not_exist_field() const {
	return _ignore_if_other_field_not_exist_field;
}
void FormField::set_ignore_if_other_field_not_exist_field(const String &val) {
	_ignore_if_other_field_not_exist_field = val;
}

void FormField::add_entry(const Ref<FormFieldEntry> &field) {
	_entries.push_back(field);
}

Ref<FormFieldEntry> FormField::get_entry(const int index) {
	ERR_FAIL_INDEX_V(index, _entries.size(), Ref<FormFieldEntry>());

	return _entries[index];
}

void FormField::remove_entry(const int index) {
	ERR_FAIL_INDEX(index, _entries.size());

	_entries.remove(index);
}
void FormField::clear_entries() {
	_entries.clear();
}

int FormField::get_entry_count() const {
	return _entries.size();
}

Vector<Variant> FormField::get_entries() {
	Vector<Variant> r;
	for (int i = 0; i < _entries.size(); i++) {
		r.push_back(_entries[i].get_ref_ptr());
	}
	return r;
}
void FormField::set_entries(const Vector<Variant> &p_arrays) {
	_entries.clear();
	for (int i = 0; i < p_arrays.size(); ++i) {
		Ref<FormFieldEntry> f = Ref<FormFieldEntry>(p_arrays[i]);

		_entries.push_back(f);
	}
}

Ref<FormField> FormField::need_to_exist() {
	Ref<FormExistsFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_be_int() {
	Ref<FormIntFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_be_float() {
	Ref<FormFloatFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_be_alpha() {
	Ref<FormAlphaFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_be_alpha_numeric() {
	Ref<FormAlphaNumericFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_have_lowercase_character() {
	Ref<FormNeedsLowercaseCharacterFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_have_uppercase_character() {
	Ref<FormNeedsUppercaseCharacterFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_have_other_character() {
	Ref<FormNeedsOtherCharacterFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_minimum_length(const int min_length) {
	Ref<FormMinimumLengthFieldEntry> f;
	f.instance();
	f->set_min_length(min_length);
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_maximum_length(const int max_length) {
	Ref<FormMaximumLengthFieldEntry> f;
	f.instance();
	f->set_max_length(max_length);
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_to_be_email() {
	Ref<FormEmailFieldEntry> f;
	f.instance();
	add_entry(f);

	return Ref<FormField>(this);
}

Ref<FormField> FormField::need_to_match(const String &other) {
	Ref<FormNeedToMatchOtherFieldEntry> f;
	f.instance();

	f->set_other_field(other);
	add_entry(f);

	return Ref<FormField>(this);
}

Ref<FormField> FormField::ignore_if_not_exists() {
	_ignore_if_not_exists = true;

	return Ref<FormField>(this);
}

Ref<FormField> FormField::ignore_if_other_field_not_exists(const String &other) {
	_ignore_if_other_field_not_exists = true;
	_ignore_if_other_field_not_exist_field = other;

	return Ref<FormField>(this);
}

PoolStringArray FormField::validate(const Ref<WebServerRequest> &request) {
	return call("_validate", request);
}

PoolStringArray FormField::_validate(Ref<WebServerRequest> request) {
	String param = request->get_parameter(get_field_name());

	if (_ignore_if_not_exists && param == "") {
		return PoolStringArray();
	}

	if (_ignore_if_other_field_not_exists) {
		String op = request->get_parameter(_ignore_if_other_field_not_exist_field);

		if (op == "") {
			return PoolStringArray();
		}
	}

	PoolStringArray arr;

	for (int i = 0; i < _entries.size(); ++i) {
		Ref<FormFieldEntry> f = _entries[i];

		if (f.is_valid()) {
			arr.append_array(f->validate(request, Ref<FormField>(this), param));
		}
	}

	return arr;
}

FormField::FormField() {
	_ignore_if_not_exists = false;
	_ignore_if_other_field_not_exists = false;
}
FormField::~FormField() {
	_entries.clear();
}

void FormField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_field_name"), &FormField::get_field_name);
	ClassDB::bind_method(D_METHOD("set_field_name", "val"), &FormField::set_field_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "field_name"), "set_field_name", "get_field_name");

	ClassDB::bind_method(D_METHOD("get_human_name"), &FormField::get_human_name);
	ClassDB::bind_method(D_METHOD("set_human_name", "val"), &FormField::set_human_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "human_name"), "set_human_name", "get_human_name");

	ClassDB::bind_method(D_METHOD("get_ignore_if_not_exists"), &FormField::get_ignore_if_not_exists);
	ClassDB::bind_method(D_METHOD("set_ignore_if_not_exists", "val"), &FormField::set_ignore_if_not_exists);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_ignore_if_not_exists"), "set_ignore_if_not_exists", "get_ignore_if_not_exists");

	ClassDB::bind_method(D_METHOD("get_ignore_if_other_field_not_exists"), &FormField::get_ignore_if_other_field_not_exists);
	ClassDB::bind_method(D_METHOD("set_ignore_if_other_field_not_exists", "val"), &FormField::set_ignore_if_other_field_not_exists);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_ignore_if_other_field_not_exists"), "set_ignore_if_other_field_not_exists", "get_ignore_if_other_field_not_exists");

	ClassDB::bind_method(D_METHOD("get_ignore_if_other_field_not_exist_field"), &FormField::get_ignore_if_other_field_not_exist_field);
	ClassDB::bind_method(D_METHOD("set_ignore_if_other_field_not_exist_field", "val"), &FormField::set_ignore_if_other_field_not_exist_field);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "ignore_if_other_field_not_exist_field"), "set_ignore_if_other_field_not_exist_field", "get_ignore_if_other_field_not_exist_field");

	ClassDB::bind_method(D_METHOD("add_entry", "field"), &FormField::add_entry);
	ClassDB::bind_method(D_METHOD("get_entry", "index"), &FormField::get_entry);
	ClassDB::bind_method(D_METHOD("remove_entry", "index"), &FormField::remove_entry);
	ClassDB::bind_method(D_METHOD("clear_entries"), &FormField::clear_entries);
	ClassDB::bind_method(D_METHOD("get_entry_count"), &FormField::get_entry_count);

	ClassDB::bind_method(D_METHOD("get_entries"), &FormField::get_entries);
	ClassDB::bind_method(D_METHOD("set_entries", "array"), &FormField::set_entries);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "entries", PROPERTY_HINT_NONE, "23/20:FormFieldEntry", PROPERTY_USAGE_DEFAULT, "FormFieldEntry"), "set_entries", "get_entries");

	ClassDB::bind_method(D_METHOD("need_to_exist"), &FormField::need_to_exist);
	ClassDB::bind_method(D_METHOD("need_to_be_int"), &FormField::need_to_be_int);
	ClassDB::bind_method(D_METHOD("need_to_be_float"), &FormField::need_to_be_float);
	ClassDB::bind_method(D_METHOD("need_to_be_alpha"), &FormField::need_to_be_alpha);
	ClassDB::bind_method(D_METHOD("need_to_be_alpha_numeric"), &FormField::need_to_be_alpha_numeric);
	ClassDB::bind_method(D_METHOD("need_to_have_lowercase_character"), &FormField::need_to_have_lowercase_character);
	ClassDB::bind_method(D_METHOD("need_to_have_uppercase_character"), &FormField::need_to_have_uppercase_character);
	ClassDB::bind_method(D_METHOD("need_to_have_other_character"), &FormField::need_to_have_other_character);
	ClassDB::bind_method(D_METHOD("need_minimum_length", "min_length"), &FormField::need_minimum_length);
	ClassDB::bind_method(D_METHOD("need_maximum_length", "max_length"), &FormField::need_maximum_length);
	ClassDB::bind_method(D_METHOD("need_to_be_email"), &FormField::need_to_be_email);
	ClassDB::bind_method(D_METHOD("need_to_match", "other"), &FormField::need_to_match);
	ClassDB::bind_method(D_METHOD("ignore_if_not_exists"), &FormField::ignore_if_not_exists);
	ClassDB::bind_method(D_METHOD("ignore_if_other_field_not_exists", "other"), &FormField::ignore_if_other_field_not_exists);

	BIND_VMETHOD(MethodInfo("_validate", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("validate", "request"), &FormField::validate);
	ClassDB::bind_method(D_METHOD("_validate", "request"), &FormField::_validate);
}

//FormValidator

void FormValidator::add_field(const Ref<FormField> &field) {
	_fields.push_back(field);
}

Ref<FormField> FormValidator::get_field(const int index) {
	ERR_FAIL_INDEX_V(index, _fields.size(), Ref<FormField>());

	return _fields[index];
}

void FormValidator::remove_field(const int index) {
	ERR_FAIL_INDEX(index, _fields.size());

	_fields.remove(index);
}
void FormValidator::clear_fields() {
	_fields.clear();
}

Ref<FormField> FormValidator::new_field(const String &name, const String &human_name) {
	Ref<FormField> f;
	f.instance();
	f->set_field_name(name);
	f->set_human_name(human_name);

	_fields.push_back(f);

	return f;
}

int FormValidator::get_field_count() const {
	return _fields.size();
}

Vector<Variant> FormValidator::get_fields() {
	Vector<Variant> r;
	for (int i = 0; i < _fields.size(); i++) {
		r.push_back(_fields[i].get_ref_ptr());
	}
	return r;
}
void FormValidator::set_fields(const Vector<Variant> &p_arrays) {
	_fields.clear();
	for (int i = 0; i < p_arrays.size(); ++i) {
		Ref<FormField> f = Ref<FormField>(p_arrays[i]);

		_fields.push_back(f);
	}
}

PoolStringArray FormValidator::validate(const Ref<WebServerRequest> &request) {
	return call("_validate", request);
}

PoolStringArray FormValidator::_validate(Ref<WebServerRequest> request) {
	PoolStringArray arr;

	for (int i = 0; i < _fields.size(); ++i) {
		Ref<FormField> f = _fields[i];

		if (f.is_valid()) {
			arr.append_array(f->validate(request));
		}
	}

	return arr;
}

FormValidator::FormValidator() {
}

FormValidator::~FormValidator() {
	_fields.clear();
}

void FormValidator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_field", "field"), &FormValidator::add_field);
	ClassDB::bind_method(D_METHOD("get_field", "index"), &FormValidator::get_field);
	ClassDB::bind_method(D_METHOD("remove_field", "index"), &FormValidator::remove_field);
	ClassDB::bind_method(D_METHOD("clear_fields"), &FormValidator::clear_fields);
	ClassDB::bind_method(D_METHOD("new_field", "name", "human_name"), &FormValidator::new_field);
	ClassDB::bind_method(D_METHOD("get_field_count"), &FormValidator::get_field_count);

	ClassDB::bind_method(D_METHOD("get_fields"), &FormValidator::get_fields);
	ClassDB::bind_method(D_METHOD("set_fields", "array"), &FormValidator::set_fields);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "fields", PROPERTY_HINT_NONE, "23/20:FormField", PROPERTY_USAGE_DEFAULT, "FormField"), "set_fields", "get_fields");

	BIND_VMETHOD(MethodInfo("_validate", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("validate", "request"), &FormValidator::validate);
	ClassDB::bind_method(D_METHOD("_validate", "request"), &FormValidator::_validate);
}
