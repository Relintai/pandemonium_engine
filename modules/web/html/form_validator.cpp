#include "form_validator.h"

#include "../http/web_server_request.h"

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

//FormExistsFieldEntry

PoolStringArray FormExistsFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data == "") {
		errors.push_back(field->human_name + not_exists_error);
	}

	return errors;
}

FormExistsFieldEntry::FormExistsFieldEntry() {
	not_exists_error = " field need to exists!";
}
FormExistsFieldEntry::~FormExistsFieldEntry() {
}

//FormIntFieldEntry

PoolStringArray FormIntFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.empty()) {
		return errors;
	}

	if (!data.is_valid_integer()) {
		errors.push_back(field->human_name + not_int_error);
	}

	return errors;
}

FormIntFieldEntry::FormIntFieldEntry() {
	not_int_error = " needs to be an integer.";
}

FormIntFieldEntry::~FormIntFieldEntry() {
}

//FormFloatFieldEntry

PoolStringArray FormFloatFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.empty()) {
		return errors;
	}

	if (!data.is_valid_float()) {
		errors.push_back(field->human_name + not_float_error);
	}

	return errors;
}

FormFloatFieldEntry::FormFloatFieldEntry() {
	not_float_error = " needs to be an floating point number.";
}
FormFloatFieldEntry::~FormFloatFieldEntry() {
}

//FormAlphaFieldEntry

PoolStringArray FormAlphaFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.size(); ++i) {
		if (!isalpha(data[i])) {
			errors.push_back(field->human_name + not_alpha_error);

			return errors;
		}
	}

	return errors;
}

FormAlphaFieldEntry::FormAlphaFieldEntry() {
	not_alpha_error = " needs to only contain caharcters.";
}
FormAlphaFieldEntry::~FormAlphaFieldEntry() {
}

//FormAlphaNumericFieldEntry

PoolStringArray FormAlphaNumericFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.size(); ++i) {
		if (!isalnum(data[i])) {
			errors.push_back(field->human_name + not_alpha_numeric_error);

			return errors;
		}
	}

	return errors;
}

FormAlphaNumericFieldEntry::FormAlphaNumericFieldEntry() {
	not_alpha_numeric_error = " needs to only contain caharcters of numbers.";
}
FormAlphaNumericFieldEntry::~FormAlphaNumericFieldEntry() {
}

//FormNeedsLowercaseCharacterFieldEntry

PoolStringArray FormNeedsLowercaseCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	for (int i = 0; i < data.size(); ++i) {
		if (islower(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->human_name + does_not_have_lowercase_error);

	return errors;
}

FormNeedsLowercaseCharacterFieldEntry::FormNeedsLowercaseCharacterFieldEntry() {
	does_not_have_lowercase_error = " needs at least one lowercase character!";
}
FormNeedsLowercaseCharacterFieldEntry::~FormNeedsLowercaseCharacterFieldEntry() {
}

//FormNeedsUppercaseCharacterFieldEntry

PoolStringArray FormNeedsUppercaseCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	for (int i = 0; i < data.size(); ++i) {
		if (isupper(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->human_name + does_not_have_uppercase_error);

	return errors;
}

FormNeedsUppercaseCharacterFieldEntry::FormNeedsUppercaseCharacterFieldEntry() {
	does_not_have_uppercase_error = " needs at least one uppercase character!";
}
FormNeedsUppercaseCharacterFieldEntry::~FormNeedsUppercaseCharacterFieldEntry() {
}

//FormNeedsOtherCharacterFieldEntry

PoolStringArray FormNeedsOtherCharacterFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	for (int i = 0; i < data.size(); ++i) {
		if (!isalnum(data[i])) {
			return errors;
		}
	}

	errors.push_back(field->human_name + does_not_have_other_error);

	return errors;
}

FormNeedsOtherCharacterFieldEntry::FormNeedsOtherCharacterFieldEntry() {
	does_not_have_other_error = " needs at least one other character!";
}
FormNeedsOtherCharacterFieldEntry::~FormNeedsOtherCharacterFieldEntry() {
}

//FormMinimumLengthFieldEntry

PoolStringArray FormMinimumLengthFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;
	if (data.size() < min_length) {
		errors.push_back(field->human_name + does_not_have_min_length_errorf + itos(min_length) + does_not_have_min_length_errors);
	}

	return errors;
}

FormMinimumLengthFieldEntry::FormMinimumLengthFieldEntry() {
	does_not_have_min_length_errorf = " needs at least ";
	does_not_have_min_length_errors = " characters!";

	min_length = 5;
}
FormMinimumLengthFieldEntry::~FormMinimumLengthFieldEntry() {
}

//FormMaximumLengthFieldEntry

PoolStringArray FormMaximumLengthFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.size() > max_length) {
		errors.push_back(field->human_name + does_not_have_max_length_errorf + itos(max_length) + does_not_have_max_length_errors);
	}

	return errors;
}

FormMaximumLengthFieldEntry::FormMaximumLengthFieldEntry() {
	does_not_have_max_length_errorf = " needs at most ";
	does_not_have_max_length_errors = " characters!";

	max_length = 10;
}
FormMaximumLengthFieldEntry::~FormMaximumLengthFieldEntry() {
}

//FormEmailFieldEntry

PoolStringArray FormEmailFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data.size() == 0) {
		errors.push_back(field->human_name + email_format_error);

		return errors;
	}

	if (!isalpha(data[0])) {
		errors.push_back(field->human_name + email_format_error);

		return errors;
	}

	int dot_pos = -1;
	int at_pos = -1;

	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '.') {
			if (dot_pos != -1) {
				errors.push_back(field->human_name + email_format_error);

				return errors;
			}

			dot_pos = i;

			return errors;
		}
	}

	if (dot_pos == -1) {
		errors.push_back(field->human_name + email_format_error);

		return errors;
	}

	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '@') {
			if (at_pos != -1) {
				errors.push_back(field->human_name + email_format_error);

				return errors;
			}

			at_pos = i;

			return errors;
		}
	}

	if (at_pos == -1) {
		errors.push_back(field->human_name + email_format_error);

		return errors;
	}

	for (int i = 0; i < data.size(); ++i) {
		if (i == at_pos || i == dot_pos) {
			continue;
		}

		if (!isalnum(data[i])) {
			errors.push_back(field->human_name + email_format_error);

			return errors;
		}
	}

	return errors;
}

FormEmailFieldEntry::FormEmailFieldEntry() {
	email_format_error = " is invalid!";
}
FormEmailFieldEntry::~FormEmailFieldEntry() {
}

//FormNeedToMatchOtherFieldEntry

PoolStringArray FormNeedToMatchOtherFieldEntry::_validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data) {
	PoolStringArray errors;

	if (data != request->get_parameter(other_field)) {
		errors.push_back(field->human_name + does_not_match_error + field->name + ".");
	}

	return errors;
}

FormNeedToMatchOtherFieldEntry::FormNeedToMatchOtherFieldEntry() {
	does_not_match_error = " does not match ";
}
FormNeedToMatchOtherFieldEntry::~FormNeedToMatchOtherFieldEntry() {
}

//FormField

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
	f->min_length = min_length;
	add_entry(f);

	return Ref<FormField>(this);
}
Ref<FormField> FormField::need_maximum_length(const int max_length) {
	Ref<FormMaximumLengthFieldEntry> f;
	f.instance();
	f->max_length = max_length;
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

	f->other_field = other;
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

void FormField::add_entry(const Ref<FormFieldEntry> &field) {
	fields.push_back(field);
}

PoolStringArray FormField::validate(const Ref<WebServerRequest> &request) {
	return call("_validate", request);
}

PoolStringArray FormField::_validate(Ref<WebServerRequest> request) {
	String param = request->get_parameter(name);

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

	for (int i = 0; i < fields.size(); ++i) {
		arr.append_array(fields.write[i]->validate(request, Ref<FormField>(this), param));
	}

	return arr;
}

FormField::FormField() {
	_ignore_if_not_exists = false;
	_ignore_if_other_field_not_exists = false;
}
FormField::~FormField() {
	fields.clear();
}

//FormValidator

PoolStringArray FormValidator::validate(const Ref<WebServerRequest> &request) {
	return call("_validate", request);
}

PoolStringArray FormValidator::_validate(Ref<WebServerRequest> request) {
	PoolStringArray arr;

	for (int i = 0; i < fields.size(); ++i) {
		arr.append_array(fields.write[i]->validate(request));
	}

	return arr;
}

void FormValidator::add_field(const Ref<FormField> &field) {
	fields.push_back(field);
}

Ref<FormField> FormValidator::new_field(const String &name, const String &human_name) {
	Ref<FormField> f;
	f.instance();
	f->name = name;
	f->human_name = human_name;

	fields.push_back(f);

	return f;
}

FormValidator::FormValidator() {
}

FormValidator::~FormValidator() {
	fields.clear();
}
