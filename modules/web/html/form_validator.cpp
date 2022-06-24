#include "form_validator.h"

#include "web/http/request.h"

//FormFieldEntry

bool FormFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	return true;
}

FormFieldEntry::FormFieldEntry() {
}

FormFieldEntry::~FormFieldEntry() {
}

//FormExistsFieldEntry

bool FormExistsFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data == "") {
		if (errors) {
			errors->push_back(field->human_name + not_exists_error);
		}

		return false;
	}

	return true;
}

FormExistsFieldEntry::FormExistsFieldEntry() {
	not_exists_error = " field need to exists!";
}
FormExistsFieldEntry::~FormExistsFieldEntry() {
}

//FormIntFieldEntry

bool FormIntFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	//https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int

	if (data.empty()) {
		return true;
	}

	if (((!isdigit(data[0])) && (data[0] != '-') && (data[0] != '+'))) {
		if (errors) {
			errors->push_back(field->human_name + not_int_error);
		}

		return false;
	}

	char *p;
	strtol(data.c_str(), &p, 10);

	bool is_int = (*p == 0);

	if (!is_int) {
		if (errors) {
			errors->push_back(field->human_name + not_int_error);
		}
	}

	return is_int;
}

FormIntFieldEntry::FormIntFieldEntry() {
	not_int_error = " needs to be an integer.";
}

FormIntFieldEntry::~FormIntFieldEntry() {
}

//FormFloatFieldEntry

bool FormFloatFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data.empty()) {
		return true;
	}

	//from https://stackoverflow.com/questions/447206/c-isfloat-function
	char *ptr;
	strtof(data.c_str(), &ptr);
	bool is_float = (*ptr) == '\0';

	if (!is_float) {
		if (errors) {
			errors->push_back(field->human_name + not_float_error);
		}
	}

	return is_float;
}

FormFloatFieldEntry::FormFloatFieldEntry() {
	not_float_error = " needs to be an floating point number.";
}
FormFloatFieldEntry::~FormFloatFieldEntry() {
}

//FormAlphaFieldEntry

bool FormAlphaFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	for (int i = 0; i < data.size(); ++i) {
		if (!isalpha(data[i])) {
			if (errors) {
				errors->push_back(field->human_name + not_alpha_error);
			}

			return false;
		}
	}

	return true;
}

FormAlphaFieldEntry::FormAlphaFieldEntry() {
	not_alpha_error = " needs to only contain caharcters.";
}
FormAlphaFieldEntry::~FormAlphaFieldEntry() {
}

//FormAlphaNumericFieldEntry

bool FormAlphaNumericFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	for (int i = 0; i < data.size(); ++i) {
		if (!isalnum(data[i])) {
			if (errors) {
				errors->push_back(field->human_name + not_alpha_numeric_error);
			}

			return false;
		}
	}

	return true;
}

FormAlphaNumericFieldEntry::FormAlphaNumericFieldEntry() {
	not_alpha_numeric_error = " needs to only contain caharcters of numbers.";
}
FormAlphaNumericFieldEntry::~FormAlphaNumericFieldEntry() {
}

//FormNeedsLowercaseCharacterFieldEntry

bool FormNeedsLowercaseCharacterFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	for (int i = 0; i < data.size(); ++i) {
		if (islower(data[i])) {

			return true;
		}
	}

	if (errors) {
		errors->push_back(field->human_name + does_not_have_lowercase_error);
	}

	return false;
}

FormNeedsLowercaseCharacterFieldEntry::FormNeedsLowercaseCharacterFieldEntry() {
	does_not_have_lowercase_error = " needs at least one lowercase character!";
}
FormNeedsLowercaseCharacterFieldEntry::~FormNeedsLowercaseCharacterFieldEntry() {
}

//FormNeedsUppercaseCharacterFieldEntry

bool FormNeedsUppercaseCharacterFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	for (int i = 0; i < data.size(); ++i) {
		if (isupper(data[i])) {
			return true;
		}
	}

	if (errors) {
		errors->push_back(field->human_name + does_not_have_uppercase_error);
	}

	return false;
}

FormNeedsUppercaseCharacterFieldEntry::FormNeedsUppercaseCharacterFieldEntry() {
	does_not_have_uppercase_error = " needs at least one uppercase character!";
}
FormNeedsUppercaseCharacterFieldEntry::~FormNeedsUppercaseCharacterFieldEntry() {
}

//FormNeedsOtherCharacterFieldEntry

bool FormNeedsOtherCharacterFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	for (int i = 0; i < data.size(); ++i) {
		if (!isalnum(data[i])) {
			return true;
		}
	}

	if (errors) {
		errors->push_back(field->human_name + does_not_have_other_error);
	}

	return false;
}

FormNeedsOtherCharacterFieldEntry::FormNeedsOtherCharacterFieldEntry() {
	does_not_have_other_error = " needs at least one other character!";
}
FormNeedsOtherCharacterFieldEntry::~FormNeedsOtherCharacterFieldEntry() {
}

//FormMinimumLengthFieldEntry

bool FormMinimumLengthFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data.size() < min_length) {
		if (errors) {
			errors->push_back(field->human_name + does_not_have_min_length_errorf + min_length + does_not_have_min_length_errors);
		}

		return false;
	}

	return true;
}

FormMinimumLengthFieldEntry::FormMinimumLengthFieldEntry() {
	does_not_have_min_length_errorf = " needs at least ";
	does_not_have_min_length_errors = " characters!";

	min_length = 5;
}
FormMinimumLengthFieldEntry::~FormMinimumLengthFieldEntry() {
}

//FormMaximumLengthFieldEntry

bool FormMaximumLengthFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data.size() > max_length) {
		if (errors) {
			errors->push_back(field->human_name + does_not_have_max_length_errorf + max_length + does_not_have_max_length_errors);
		}

		return false;
	}

	return true;
}

FormMaximumLengthFieldEntry::FormMaximumLengthFieldEntry() {
	does_not_have_max_length_errorf = " needs at most ";
	does_not_have_max_length_errors = " characters!";

	max_length = 10;
}
FormMaximumLengthFieldEntry::~FormMaximumLengthFieldEntry() {
}

//FormEmailFieldEntry

bool FormEmailFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data.size() == 0) {
		if (errors) {
			errors->push_back(field->human_name + email_format_error);
		}

		return false;
	}

	if (!isalpha(data[0])) {
		if (errors) {
			errors->push_back(field->human_name + email_format_error);
		}

		return false;
	}

	int dot_pos = -1;
	int at_pos = -1;

	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '.') {
			if (dot_pos != -1) {
				if (errors) {
					errors->push_back(field->human_name + email_format_error);
				}

				return false;
			}

			dot_pos = i;

			return true;
		}
	}

	if (dot_pos == -1) {
		if (errors) {
			errors->push_back(field->human_name + email_format_error);
		}

		return false;
	}

	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '@') {
			if (at_pos != -1) {
				if (errors) {
					errors->push_back(field->human_name + email_format_error);
				}

				return false;
			}

			at_pos = i;

			return true;
		}
	}

	if (at_pos == -1) {
		if (errors) {
			errors->push_back(field->human_name + email_format_error);
		}

		return false;
	}

	for (int i = 0; i < data.size(); ++i) {
		if (i == at_pos || i == dot_pos) {
			continue;
		}

		if (!isalnum(data[i])) {
			if (errors) {
				errors->push_back(field->human_name + email_format_error);
			}

			return false;
		}
	}

	return true;
}

FormEmailFieldEntry::FormEmailFieldEntry() {
	email_format_error = " is invalid!";
}
FormEmailFieldEntry::~FormEmailFieldEntry() {
}

//FormNeedToMatchOtherFieldEntry

bool FormNeedToMatchOtherFieldEntry::validate(Request *request, const FormField *field, const String &data, Vector<String> *errors) {
	if (data != request->get_parameter(other_field)) {
		if (errors) {
			errors->push_back(field->human_name + does_not_match_error + field->name + ".");
		}

		return false;
	}

	return true;
}

FormNeedToMatchOtherFieldEntry::FormNeedToMatchOtherFieldEntry() {
	does_not_match_error = " does not match ";
}
FormNeedToMatchOtherFieldEntry::~FormNeedToMatchOtherFieldEntry() {
}

//FormField

FormField *FormField::need_to_exist() {
	add_entry(new FormExistsFieldEntry());

	return this;
}
FormField *FormField::need_to_be_int() {
	add_entry(new FormIntFieldEntry());

	return this;
}
FormField *FormField::need_to_be_float() {
	add_entry(new FormFloatFieldEntry());

	return this;
}
FormField *FormField::need_to_be_alpha() {
	add_entry(new FormAlphaFieldEntry());

	return this;
}
FormField *FormField::need_to_be_alpha_numeric() {
	add_entry(new FormAlphaNumericFieldEntry());

	return this;
}
FormField *FormField::need_to_have_lowercase_character() {
	add_entry(new FormNeedsLowercaseCharacterFieldEntry());

	return this;
}
FormField *FormField::need_to_have_uppercase_character() {
	add_entry(new FormNeedsUppercaseCharacterFieldEntry());

	return this;
}
FormField *FormField::need_to_have_other_character() {
	add_entry(new FormNeedsOtherCharacterFieldEntry());

	return this;
}
FormField *FormField::need_minimum_length(const int min_length) {
	FormMinimumLengthFieldEntry *f = new FormMinimumLengthFieldEntry();
	f->min_length = min_length;
	add_entry(f);

	return this;
}
FormField *FormField::need_maximum_length(const int max_length) {
	FormMaximumLengthFieldEntry *f = new FormMaximumLengthFieldEntry();
	f->max_length = max_length;
	add_entry(f);

	return this;
}
FormField *FormField::need_to_be_email() {
	add_entry(new FormEmailFieldEntry());

	return this;
}

FormField *FormField::need_to_match(const String &other) {
	FormNeedToMatchOtherFieldEntry *f = new FormNeedToMatchOtherFieldEntry();
	f->other_field = other;
	add_entry(f);

	return this;
}

FormField *FormField::ignore_if_not_exists() {
	_ignore_if_not_exists = true;

	return this;
}

FormField *FormField::ignore_if_other_field_not_exists(const String &other) {
	_ignore_if_other_field_not_exists = true;
	_ignore_if_other_field_not_exist_field = other;

	return this;
}

void FormField::add_entry(FormFieldEntry *field) {
	fields.push_back(field);
}

bool FormField::validate(Request *request, Vector<String> *errors) {
	String param = request->get_parameter(name);

	if (_ignore_if_not_exists && param == "") {
		return true;
	}

	if (_ignore_if_other_field_not_exists) {
		String op = request->get_parameter(_ignore_if_other_field_not_exist_field);

		if (op == "") {
			return true;
		}
	}

	bool valid = true;

	for (int i = 0; i < fields.size(); ++i) {
		if (!fields[i]->validate(request, this, param, errors)) {
			valid = false;
		}
	}

	return valid;
}

FormField::FormField() {
	_ignore_if_not_exists = false;
	_ignore_if_other_field_not_exists = false;
}
FormField::~FormField() {
	for (int i = 0; i < fields.size(); ++i) {
		delete fields[i];
	}

	fields.clear();
}

//FormValidator

bool FormValidator::validate(Request *request, Vector<String> *errors) {
	bool valid = true;

	for (int i = 0; i < fields.size(); ++i) {

		if (!fields[i]->validate(request, errors)) {
			valid = false;
		}
	}

	return valid;
}

void FormValidator::add_field(FormField *field) {
	fields.push_back(field);
}

FormField *FormValidator::new_field(const String &name, const String &human_name) {
	FormField *f = new FormField();
	f->name = name;
	f->human_name = human_name;

	fields.push_back(f);

	return f;
}

FormValidator::FormValidator() {
}

FormValidator::~FormValidator() {
	for (int i = 0; i < fields.size(); ++i) {
		delete fields[i];
	}

	fields.clear();
}
