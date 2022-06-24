#ifndef FORM_H
#define FORM_H

#include "core/string.h"
#include "core/containers/vector.h"

#include <ctype.h>

class Request;
class FormField;

class FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormFieldEntry();
	virtual ~FormFieldEntry();
};

class FormExistsFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormExistsFieldEntry();
	~FormExistsFieldEntry();

	String not_exists_error;
};

class FormIntFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormIntFieldEntry();
	~FormIntFieldEntry();

	String not_int_error;
};

class FormFloatFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormFloatFieldEntry();
	~FormFloatFieldEntry();

	String not_float_error;
};

class FormAlphaFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormAlphaFieldEntry();
	~FormAlphaFieldEntry();

	String not_alpha_error;
};

class FormAlphaNumericFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormAlphaNumericFieldEntry();
	~FormAlphaNumericFieldEntry();

	String not_alpha_numeric_error;
};

class FormNeedsLowercaseCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormNeedsLowercaseCharacterFieldEntry();
	~FormNeedsLowercaseCharacterFieldEntry();

	String does_not_have_lowercase_error;
};

class FormNeedsUppercaseCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormNeedsUppercaseCharacterFieldEntry();
	~FormNeedsUppercaseCharacterFieldEntry();

	String does_not_have_uppercase_error;
};

class FormNeedsOtherCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormNeedsOtherCharacterFieldEntry();
	~FormNeedsOtherCharacterFieldEntry();

	String does_not_have_other_error;
};

class FormMinimumLengthFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormMinimumLengthFieldEntry();
	~FormMinimumLengthFieldEntry();

	int min_length;

	String does_not_have_min_length_errorf;
	String does_not_have_min_length_errors;
};

class FormMaximumLengthFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormMaximumLengthFieldEntry();
	~FormMaximumLengthFieldEntry();

	int max_length;

	String does_not_have_max_length_errorf;
	String does_not_have_max_length_errors;
};

class FormEmailFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormEmailFieldEntry();
	~FormEmailFieldEntry();

	String email_format_error;
};

class FormNeedToMatchOtherFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const String &data, Vector<String> *errors);

	FormNeedToMatchOtherFieldEntry();
	~FormNeedToMatchOtherFieldEntry();

	String other_field;

	String does_not_match_error;
};

//FormField

class FormField {
public:
	String name;
    String human_name;

	bool _ignore_if_not_exists;

	bool _ignore_if_other_field_not_exists;
	String _ignore_if_other_field_not_exist_field;

	FormField *need_to_exist();
	FormField *need_to_be_int();
	FormField *need_to_be_float();
	FormField *need_to_be_alpha();
	FormField *need_to_be_alpha_numeric();
	FormField *need_to_have_lowercase_character();
	FormField *need_to_have_uppercase_character();
	FormField *need_to_have_other_character();
	FormField *need_minimum_length(const int min_length);
	FormField *need_maximum_length(const int max_length);
	FormField *need_to_be_email();
    FormField *need_to_match(const String &other);
	FormField *ignore_if_not_exists();
	FormField *ignore_if_other_field_not_exists(const String &other);

	void add_entry(FormFieldEntry *field);

	bool validate(Request *request, Vector<String> *errors);

	FormField();
	virtual ~FormField();

	Vector<FormFieldEntry *> fields;
};

//FormValidator

class FormValidator {
public:
	bool validate(Request *request, Vector<String> *errors = nullptr);

	void add_field(FormField *field);
	FormField *new_field(const String &name, const String &human_name);

	FormValidator();
	virtual ~FormValidator();

	Vector<FormField *> fields;
};

#endif