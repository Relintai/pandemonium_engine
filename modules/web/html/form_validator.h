#ifndef HTTP_FORM_VALIDATOR_H
#define HTTP_FORM_VALIDATOR_H

#include "core/ustring.h"
#include "core/vector.h"

#include "core/resource.h"

class Request;
class FormField;
class WebServerRequest;

class FormFieldEntry : public Resource {
	GDCLASS(FormFieldEntry, Resource);

public:
	PoolStringArray validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);
	virtual PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormFieldEntry();
	~FormFieldEntry();
};

class FormExistsFieldEntry : public FormFieldEntry {
	GDCLASS(FormExistsFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormExistsFieldEntry();
	~FormExistsFieldEntry();

	String not_exists_error;
};

class FormIntFieldEntry : public FormFieldEntry {
	GDCLASS(FormIntFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormIntFieldEntry();
	~FormIntFieldEntry();

	String not_int_error;
};

class FormFloatFieldEntry : public FormFieldEntry {
	GDCLASS(FormFloatFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormFloatFieldEntry();
	~FormFloatFieldEntry();

	String not_float_error;
};

class FormAlphaFieldEntry : public FormFieldEntry {
	GDCLASS(FormAlphaFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormAlphaFieldEntry();
	~FormAlphaFieldEntry();

	String not_alpha_error;
};

class FormAlphaNumericFieldEntry : public FormFieldEntry {
	GDCLASS(FormAlphaNumericFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormAlphaNumericFieldEntry();
	~FormAlphaNumericFieldEntry();

	String not_alpha_numeric_error;
};

class FormNeedsLowercaseCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsLowercaseCharacterFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsLowercaseCharacterFieldEntry();
	~FormNeedsLowercaseCharacterFieldEntry();

	String does_not_have_lowercase_error;
};

class FormNeedsUppercaseCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsUppercaseCharacterFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsUppercaseCharacterFieldEntry();
	~FormNeedsUppercaseCharacterFieldEntry();

	String does_not_have_uppercase_error;
};

class FormNeedsOtherCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsOtherCharacterFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsOtherCharacterFieldEntry();
	~FormNeedsOtherCharacterFieldEntry();

	String does_not_have_other_error;
};

class FormMinimumLengthFieldEntry : public FormFieldEntry {
	GDCLASS(FormMinimumLengthFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormMinimumLengthFieldEntry();
	~FormMinimumLengthFieldEntry();

	int min_length;

	String does_not_have_min_length_errorf;
	String does_not_have_min_length_errors;
};

class FormMaximumLengthFieldEntry : public FormFieldEntry {
	GDCLASS(FormMaximumLengthFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormMaximumLengthFieldEntry();
	~FormMaximumLengthFieldEntry();

	int max_length;

	String does_not_have_max_length_errorf;
	String does_not_have_max_length_errors;
};

class FormEmailFieldEntry : public FormFieldEntry {
	GDCLASS(FormEmailFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormEmailFieldEntry();
	~FormEmailFieldEntry();

	String email_format_error;
};

class FormNeedToMatchOtherFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedToMatchOtherFieldEntry, FormFieldEntry);

public:
	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedToMatchOtherFieldEntry();
	~FormNeedToMatchOtherFieldEntry();

	String other_field;

	String does_not_match_error;
};

//FormField

class FormField : public Resource {
	GDCLASS(FormField, Resource);

public:
	String name;
	String human_name;

	bool _ignore_if_not_exists;

	bool _ignore_if_other_field_not_exists;
	String _ignore_if_other_field_not_exist_field;

	Ref<FormField> need_to_exist();
	Ref<FormField> need_to_be_int();
	Ref<FormField> need_to_be_float();
	Ref<FormField> need_to_be_alpha();
	Ref<FormField> need_to_be_alpha_numeric();
	Ref<FormField> need_to_have_lowercase_character();
	Ref<FormField> need_to_have_uppercase_character();
	Ref<FormField> need_to_have_other_character();
	Ref<FormField> need_minimum_length(const int min_length);
	Ref<FormField> need_maximum_length(const int max_length);
	Ref<FormField> need_to_be_email();
	Ref<FormField> need_to_match(const String &other);
	Ref<FormField> ignore_if_not_exists();
	Ref<FormField> ignore_if_other_field_not_exists(const String &other);

	void add_entry(const Ref<FormFieldEntry> &field);

	PoolStringArray validate(const Ref<WebServerRequest> &request);
	virtual PoolStringArray _validate(Ref<WebServerRequest> request);

	FormField();
	~FormField();

	Vector<Ref<FormFieldEntry>> fields;
};

//FormValidator

class FormValidator : public Resource {
	GDCLASS(FormValidator, Resource);

public:
	PoolStringArray validate(const Ref<WebServerRequest> &request);
	virtual PoolStringArray _validate(Ref<WebServerRequest> request);

	void add_field(const Ref<FormField> &field);
	Ref<FormField> new_field(const String &name, const String &human_name);

	FormValidator();
	~FormValidator();

protected:
	Vector<Ref<FormField>> fields;
};

#endif
