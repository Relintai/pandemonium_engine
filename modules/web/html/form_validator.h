#ifndef HTTP_FORM_VALIDATOR_H
#define HTTP_FORM_VALIDATOR_H

#include "core/ustring.h"
#include "core/containers/vector.h"

#include "core/object/resource.h"

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

protected:
	static void _bind_methods();
};

class FormExistsFieldEntry : public FormFieldEntry {
	GDCLASS(FormExistsFieldEntry, FormFieldEntry);

public:
	String get_not_exists_error();
	void set_not_exists_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormExistsFieldEntry();
	~FormExistsFieldEntry();

protected:
	static void _bind_methods();

	String _not_exists_error;
};

class FormIntFieldEntry : public FormFieldEntry {
	GDCLASS(FormIntFieldEntry, FormFieldEntry);

public:
	String get_not_int_error();
	void set_not_int_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormIntFieldEntry();
	~FormIntFieldEntry();

protected:
	static void _bind_methods();

	String _not_int_error;
};

class FormFloatFieldEntry : public FormFieldEntry {
	GDCLASS(FormFloatFieldEntry, FormFieldEntry);

public:
	String get_not_float_error();
	void set_not_float_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormFloatFieldEntry();
	~FormFloatFieldEntry();

protected:
	static void _bind_methods();

	String _not_float_error;
};

class FormAlphaFieldEntry : public FormFieldEntry {
	GDCLASS(FormAlphaFieldEntry, FormFieldEntry);

public:
	String get_not_alpha_error();
	void set_not_alpha_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormAlphaFieldEntry();
	~FormAlphaFieldEntry();

protected:
	static void _bind_methods();

	String _not_alpha_error;
};

class FormAlphaNumericFieldEntry : public FormFieldEntry {
	GDCLASS(FormAlphaNumericFieldEntry, FormFieldEntry);

public:
	String get_not_alpha_numeric_error();
	void set_not_alpha_numeric_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormAlphaNumericFieldEntry();
	~FormAlphaNumericFieldEntry();

protected:
	static void _bind_methods();

	String _not_alpha_numeric_error;
};

class FormNeedsLowercaseCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsLowercaseCharacterFieldEntry, FormFieldEntry);

public:
	String get_does_not_have_lowercase_error();
	void set_does_not_have_lowercase_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsLowercaseCharacterFieldEntry();
	~FormNeedsLowercaseCharacterFieldEntry();

protected:
	static void _bind_methods();

	String _does_not_have_lowercase_error;
};

class FormNeedsUppercaseCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsUppercaseCharacterFieldEntry, FormFieldEntry);

public:
	String get_does_not_have_uppercase_error();
	void set_does_not_have_uppercase_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsUppercaseCharacterFieldEntry();
	~FormNeedsUppercaseCharacterFieldEntry();

protected:
	static void _bind_methods();

	String _does_not_have_uppercase_error;
};

class FormNeedsOtherCharacterFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedsOtherCharacterFieldEntry, FormFieldEntry);

public:
	String get_does_not_have_other_error();
	void set_does_not_have_other_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedsOtherCharacterFieldEntry();
	~FormNeedsOtherCharacterFieldEntry();

protected:
	static void _bind_methods();

	String _does_not_have_other_error;
};

class FormMinimumLengthFieldEntry : public FormFieldEntry {
	GDCLASS(FormMinimumLengthFieldEntry, FormFieldEntry);

public:
	int get_min_length();
	void set_min_length(const int &val);

	String get_does_not_have_min_length_errorf();
	void set_does_not_have_min_length_errorf(const String &val);

	String get_does_not_have_min_length_errors();
	void set_does_not_have_min_length_errors(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormMinimumLengthFieldEntry();
	~FormMinimumLengthFieldEntry();

protected:
	static void _bind_methods();

	int _min_length;

	String _does_not_have_min_length_errorf;
	String _does_not_have_min_length_errors;
};

class FormMaximumLengthFieldEntry : public FormFieldEntry {
	GDCLASS(FormMaximumLengthFieldEntry, FormFieldEntry);

public:
	int get_max_length();
	void set_max_length(const int &val);

	String get_does_not_have_max_length_errorf();
	void set_does_not_have_max_length_errorf(const String &val);

	String get_does_not_have_max_length_errors();
	void set_does_not_have_max_length_errors(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormMaximumLengthFieldEntry();
	~FormMaximumLengthFieldEntry();

protected:
	static void _bind_methods();

	int _max_length;

	String _does_not_have_max_length_errorf;
	String _does_not_have_max_length_errors;
};

class FormEmailFieldEntry : public FormFieldEntry {
	GDCLASS(FormEmailFieldEntry, FormFieldEntry);

public:
	String get_email_format_error();
	void set_email_format_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormEmailFieldEntry();
	~FormEmailFieldEntry();

protected:
	static void _bind_methods();

	String _email_format_error;
};

class FormNeedToMatchOtherFieldEntry : public FormFieldEntry {
	GDCLASS(FormNeedToMatchOtherFieldEntry, FormFieldEntry);

public:
	String get_other_field();
	void set_other_field(const String &val);

	String get_does_not_match_error();
	void set_does_not_match_error(const String &val);

	PoolStringArray _validate(Ref<WebServerRequest> request, const Ref<FormField> &field, const String &data);

	FormNeedToMatchOtherFieldEntry();
	~FormNeedToMatchOtherFieldEntry();

protected:
	static void _bind_methods();

	String _other_field;
	String _does_not_match_error;
};

//FormField

class FormField : public Resource {
	GDCLASS(FormField, Resource);

public:
	String get_field_name() const;
	void set_field_name(const String &val);

	String get_human_name() const;
	void set_human_name(const String &val);

	bool get_ignore_if_not_exists() const;
	void set_ignore_if_not_exists(const bool &val);

	bool get_ignore_if_other_field_not_exists() const;
	void set_ignore_if_other_field_not_exists(const bool &val);

	String get_ignore_if_other_field_not_exist_field() const;
	void set_ignore_if_other_field_not_exist_field(const String &val);

	void add_entry(const Ref<FormFieldEntry> &field);
	Ref<FormFieldEntry> get_entry(const int index);
	void remove_entry(const int index);
	void clear_entries();
	int get_entry_count() const;

	Vector<Variant> get_entries();
	void set_entries(const Vector<Variant> &p_arrays);

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

	PoolStringArray validate(const Ref<WebServerRequest> &request);
	virtual PoolStringArray _validate(Ref<WebServerRequest> request);

	FormField();
	~FormField();

protected:
	static void _bind_methods();

	String _field_name;
	String _human_name;

	bool _ignore_if_not_exists;

	bool _ignore_if_other_field_not_exists;
	String _ignore_if_other_field_not_exist_field;

	Vector<Ref<FormFieldEntry>> _entries;
};

//FormValidator

class FormValidator : public Resource {
	GDCLASS(FormValidator, Resource);

public:
	void add_field(const Ref<FormField> &field);
	Ref<FormField> get_field(const int index);
	void remove_field(const int index);
	void clear_fields();
	Ref<FormField> new_field(const String &name, const String &human_name);
	int get_field_count() const;

	Vector<Variant> get_fields();
	void set_fields(const Vector<Variant> &p_arrays);

	PoolStringArray validate(const Ref<WebServerRequest> &request);
	virtual PoolStringArray _validate(Ref<WebServerRequest> request);

	FormValidator();
	~FormValidator();

protected:
	static void _bind_methods();

	Vector<Ref<FormField>> _fields;
};

#endif
