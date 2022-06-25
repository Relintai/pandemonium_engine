#include "html_builder_bind.h"
#include "core/print_string.h"
#include "core/ustring.h"

#include "core/method_bind_ext.gen.inc"

//#include "web/http/request.h"

bool _HTMLTag::get_simple() const {
	return simple;
}
void _HTMLTag::set_simple(const bool val) {
	simple = val;
}

String _HTMLTag::get_result() {
	return result;
}
void _HTMLTag::set_result(const String &str) {
	result = str;
}

Ref<_HTMLTag> _HTMLTag::str(const String &str) {
	result += " " + str;

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::style(const String &val) {
	attrib("style", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::href(const String &val) {
	attrib("href", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::cls(const String &val) {
	attrib("class", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::clsse(const String &val) {
	if (val == "") {
		return Ref<_HTMLTag>(this);
	}

	attrib("class", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::id(const String &val) {
	attrib("id", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::name(const String &val) {
	attrib("name", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::content(const String &val) {
	attrib("content", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::value(const String &val) {
	attrib("value", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::accept(const String &val) {
	attrib("accept", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::src(const String &val) {
	attrib("src", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::alt(const String &val) {
	attrib("alt", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::autocomplete(const String &val) {
	attrib("autocomplete", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::autocomplete_off() {
	attrib("autocomplete", "off");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_on() {
	attrib("autocomplete", "on");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name() {
	attrib("autocomplete", "name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_honorific_prefix() {
	attrib("autocomplete", "honorific-prefix");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_given_name() {
	attrib("autocomplete", "given-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_additional_name() {
	attrib("autocomplete", "additional-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_family_name() {
	attrib("autocomplete", "family-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_honorific_suffix() {
	attrib("autocomplete", "honorific-suffix");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_name_nickname() {
	attrib("autocomplete", "nickname");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_email() {
	attrib("autocomplete", "email");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_username() {
	attrib("autocomplete", "username");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_new_password() {
	attrib("autocomplete", "new-password");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_current_password() {
	attrib("autocomplete", "current-password");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_one_time_code() {
	attrib("autocomplete", "one-time-code");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_organization_title() {
	attrib("autocomplete", "organization-title");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_organization() {
	attrib("autocomplete", "organization");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_street_address() {
	attrib("autocomplete", "street-address");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_line1() {
	attrib("autocomplete", "address-line1");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_line2() {
	attrib("autocomplete", "address-line2");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_line3() {
	attrib("autocomplete", "address-line3");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_level_1() {
	attrib("autocomplete", "address-level1");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_level_2() {
	attrib("autocomplete", "address-level2");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_level_3() {
	attrib("autocomplete", "address-level3");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_address_level_4() {
	attrib("autocomplete", "address-level4");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_country() {
	attrib("autocomplete", "country");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_country_name() {
	attrib("autocomplete", "country-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_postal_code() {
	attrib("autocomplete", "postal-code");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_name() {
	attrib("autocomplete", "cc-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_given_name() {
	attrib("autocomplete", "cc-given-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_additional_name() {
	attrib("autocomplete", "cc-additional-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_family_name() {
	attrib("autocomplete", "cc-family-name");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_number() {
	attrib("autocomplete", "cc-number");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_exp() {
	attrib("autocomplete", "cc-exp");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_exp_month() {
	attrib("autocomplete", "cc-exp-month");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_exp_year() {
	attrib("autocomplete", "cc-exp-year");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_csc() {
	attrib("autocomplete", "cc-csc");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_cc_type() {
	attrib("autocomplete", "cc-type");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_transaction_currency() {
	attrib("autocomplete", "transaction-currency");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_transaction_amount() {
	attrib("autocomplete", "transaction-amount");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_language() {
	attrib("autocomplete", "language");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_bday() {
	attrib("autocomplete", "bday");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_bday_day() {
	attrib("autocomplete", "bday-day");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_bday_month() {
	attrib("autocomplete", "bday-month");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_bday_year() {
	attrib("autocomplete", "bday-year");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_sex() {
	attrib("autocomplete", "sex");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel() {
	attrib("autocomplete", "tel");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel_country_code() {
	attrib("autocomplete", "tel-country-code");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel_national() {
	attrib("autocomplete", "tel-national");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel_area_code() {
	attrib("autocomplete", "tel-area-code");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel_local() {
	attrib("autocomplete", "tel-local");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_tel_extension() {
	attrib("autocomplete", "tel-extension");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_impp() {
	attrib("autocomplete", "impp");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_url() {
	attrib("autocomplete", "url");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::autocomplete_photo() {
	attrib("autocomplete", "photo");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::onclick(const String &val) {
	attrib("onclick", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::inputmode(const String &val) {
	attrib("inputmode", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::list(const String &val) {
	attrib("list", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::checked(const bool val) {
	if (val) {
		result += " checked";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::selected(const bool val) {
	if (val) {
		result += " selected";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::autofocus(const bool val) {
	if (val) {
		result += " autofocus";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::disabled(const bool val) {
	if (val) {
		result += " disabled";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::multiple(const bool val) {
	if (val) {
		result += " multiple";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::required(const bool val) {
	if (val) {
		result += " required";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::spellcheck(const bool val) {
	if (val) {
		attrib("spellcheck", "true");
	} else {
		attrib("spellcheck", "false");
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::max(const String &val) {
	attrib("max", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::min(const String &val) {
	attrib("min", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::step(const String &val) {
	attrib("step", val);

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::step_any() {
	attrib("step", "any");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::minlength(const int val) {
	attrib("minlength", String::num(val));

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::minlengths(const String &val) {
	attrib("minlength", val);

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::maxlength(const int val) {
	attrib("maxlength", String::num(val));

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::maxlengths(const String &val) {
	attrib("maxlength", val);

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::size(const int val) {
	attrib("size", String::num(val));

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::sizes(const String &val) {
	attrib("size", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::width(const int val) {
	attrib("width", String::num(val));

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::widths(const String &val) {
	attrib("width", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::height(const int val) {
	attrib("height", String::num(val));

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::heights(const String &val) {
	attrib("height", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::pattern(const String &val) {
	attrib("pattern", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::method(const String &val) {
	attrib("method", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::method_get() {
	attrib("method", "get");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::method_post() {
	attrib("method", "post");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::action(const String &val) {
	attrib("action", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::type(const String &val) {
	attrib("type", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::placeholder(const String &val) {
	attrib("placeholder", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::fora(const String &val) {
	attrib("for", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel(const String &val) {
	attrib("rel", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_stylesheet() {
	attrib("rel", "stylesheet");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_alternate() {
	attrib("rel", "alternate");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_author() {
	attrib("rel", "author");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_bookmark() {
	attrib("rel", "bookmark");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_external() {
	attrib("rel", "external");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_help() {
	attrib("rel", "help");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_license() {
	attrib("rel", "license");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_next() {
	attrib("rel", "next");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_nofollow() {
	attrib("rel", "nofollow");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_noopener() {
	attrib("rel", "noopener");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_noreferrer() {
	attrib("rel", "noreferrer");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_prev() {
	attrib("rel", "prev");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_search() {
	attrib("rel", "search");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::rel_tag() {
	attrib("rel", "tag");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::charset(const String &val) {
	attrib("charset", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::charset_utf_8() {
	attrib("charset", "utf-8");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::itbutton() {
	attrib("type", "button");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itcheckbox() {
	attrib("type", "checkbox");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itcolor() {
	attrib("type", "color");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itdate() {
	attrib("type", "date");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itdatetime_local() {
	attrib("type", "datetime_local");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itemail() {
	attrib("type", "email");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itfile() {
	attrib("type", "file");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::ithidden() {
	attrib("type", "hidden");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itimage() {
	attrib("type", "image");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itmonth() {
	attrib("type", "month");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itnumber() {
	attrib("type", "number");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itpassword() {
	attrib("type", "password");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itradio() {
	attrib("type", "radio");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itrange() {
	attrib("type", "range");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itreset() {
	attrib("type", "reset");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itsearch() {
	attrib("type", "search");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itsubmit() {
	attrib("type", "submit");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::ittel() {
	attrib("type", "tel");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::ittext() {
	attrib("type", "text");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::ittime() {
	attrib("type", "time");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::iturl() {
	attrib("type", "url");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::itweek() {
	attrib("type", "week");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::inputmode_none() {
	attrib("inputmode", "none");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_text() {
	attrib("inputmode", "text");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_decimal() {
	attrib("inputmode", "decimal");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_numeric() {
	attrib("inputmode", "numeric");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_tel() {
	attrib("inputmode", "tel");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_search() {
	attrib("inputmode", "search");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_email() {
	attrib("inputmode", "email");

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::inputmode_url() {
	attrib("inputmode", "url");

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::attrib(const String &attr, const String &val) {
	result += " " + attr + "=\"" + val + "\"";

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::start(const String &p_tag, const bool p_simple) {
	simple = p_simple;

	result = "<" + p_tag;

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::reset() {
	result.clear();

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::close() {
	if (simple) {
		result += "/>";
	} else {
		result += ">";
	}

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLBuilder> _HTMLTag::f() {
	return Ref<_HTMLBuilder>(owner);
}

bool _HTMLTag::has_data() {
	return result.size() > 0;
}

_HTMLTag::_HTMLTag() {
	simple = true;
}

void _HTMLTag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_simple"), &_HTMLTag::get_simple);
	ClassDB::bind_method(D_METHOD("set_simple", "val"), &_HTMLTag::set_simple);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "simple"), "set_simple", "get_simple");

	ClassDB::bind_method(D_METHOD("get_result"), &_HTMLTag::get_result);
	ClassDB::bind_method(D_METHOD("set_result", "val"), &_HTMLTag::set_result);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "result"), "set_result", "get_result");

	ClassDB::bind_method(D_METHOD("str", "srt"), &_HTMLTag::str);
	ClassDB::bind_method(D_METHOD("style", "val"), &_HTMLTag::style);
	ClassDB::bind_method(D_METHOD("href", "val"), &_HTMLTag::href);
	ClassDB::bind_method(D_METHOD("cls", "val"), &_HTMLTag::cls);
	ClassDB::bind_method(D_METHOD("clsse", "val"), &_HTMLTag::clsse);
	ClassDB::bind_method(D_METHOD("id", "val"), &_HTMLTag::id);
	ClassDB::bind_method(D_METHOD("name", "val"), &_HTMLTag::name);
	ClassDB::bind_method(D_METHOD("content", "val"), &_HTMLTag::content);
	ClassDB::bind_method(D_METHOD("value", "val"), &_HTMLTag::value);
	ClassDB::bind_method(D_METHOD("accept", "val"), &_HTMLTag::accept);
	ClassDB::bind_method(D_METHOD("src", "val"), &_HTMLTag::src);
	ClassDB::bind_method(D_METHOD("alt", "val"), &_HTMLTag::alt);
	ClassDB::bind_method(D_METHOD("inputmode", "val"), &_HTMLTag::inputmode);
	ClassDB::bind_method(D_METHOD("list", "val"), &_HTMLTag::list);

	ClassDB::bind_method(D_METHOD("autocomplete", "val"), &_HTMLTag::autocomplete);

	ClassDB::bind_method(D_METHOD("autocomplete_off"), &_HTMLTag::autocomplete_off);
	ClassDB::bind_method(D_METHOD("autocomplete_on"), &_HTMLTag::autocomplete_on);
	ClassDB::bind_method(D_METHOD("autocomplete_name"), &_HTMLTag::autocomplete_name);
	ClassDB::bind_method(D_METHOD("autocomplete_name_honorific_prefix"), &_HTMLTag::autocomplete_name_honorific_prefix);
	ClassDB::bind_method(D_METHOD("autocomplete_name_given_name"), &_HTMLTag::autocomplete_name_given_name);
	ClassDB::bind_method(D_METHOD("autocomplete_name_additional_name"), &_HTMLTag::autocomplete_name_additional_name);
	ClassDB::bind_method(D_METHOD("autocomplete_name_family_name"), &_HTMLTag::autocomplete_name_family_name);
	ClassDB::bind_method(D_METHOD("autocomplete_name_honorific_suffix"), &_HTMLTag::autocomplete_name_honorific_suffix);
	ClassDB::bind_method(D_METHOD("autocomplete_name_nickname"), &_HTMLTag::autocomplete_name_nickname);
	ClassDB::bind_method(D_METHOD("autocomplete_email"), &_HTMLTag::autocomplete_email);

	ClassDB::bind_method(D_METHOD("autocomplete_username"), &_HTMLTag::autocomplete_username);
	ClassDB::bind_method(D_METHOD("autocomplete_new_password"), &_HTMLTag::autocomplete_new_password);
	ClassDB::bind_method(D_METHOD("autocomplete_current_password"), &_HTMLTag::autocomplete_current_password);
	ClassDB::bind_method(D_METHOD("autocomplete_one_time_code"), &_HTMLTag::autocomplete_one_time_code);
	ClassDB::bind_method(D_METHOD("autocomplete_organization_title"), &_HTMLTag::autocomplete_organization_title);
	ClassDB::bind_method(D_METHOD("autocomplete_organization"), &_HTMLTag::autocomplete_organization);
	ClassDB::bind_method(D_METHOD("autocomplete_street_address"), &_HTMLTag::autocomplete_street_address);
	ClassDB::bind_method(D_METHOD("autocomplete_address_line1"), &_HTMLTag::autocomplete_address_line1);
	ClassDB::bind_method(D_METHOD("autocomplete_address_line2"), &_HTMLTag::autocomplete_address_line2);
	ClassDB::bind_method(D_METHOD("autocomplete_address_line3"), &_HTMLTag::autocomplete_address_line3);
	ClassDB::bind_method(D_METHOD("autocomplete_address_level_1"), &_HTMLTag::autocomplete_address_level_1);
	ClassDB::bind_method(D_METHOD("autocomplete_address_level_2"), &_HTMLTag::autocomplete_address_level_2);
	ClassDB::bind_method(D_METHOD("autocomplete_address_level_3"), &_HTMLTag::autocomplete_address_level_3);
	ClassDB::bind_method(D_METHOD("autocomplete_address_level_4"), &_HTMLTag::autocomplete_address_level_4);

	ClassDB::bind_method(D_METHOD("autocomplete_country"), &_HTMLTag::autocomplete_country);
	ClassDB::bind_method(D_METHOD("autocomplete_country_name"), &_HTMLTag::autocomplete_country_name);
	ClassDB::bind_method(D_METHOD("autocomplete_postal_code"), &_HTMLTag::autocomplete_postal_code);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_name"), &_HTMLTag::autocomplete_cc_name);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_given_name"), &_HTMLTag::autocomplete_cc_given_name);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_additional_name"), &_HTMLTag::autocomplete_cc_additional_name);

	ClassDB::bind_method(D_METHOD("autocomplete_cc_family_name"), &_HTMLTag::autocomplete_cc_family_name);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_number"), &_HTMLTag::autocomplete_cc_number);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_exp"), &_HTMLTag::autocomplete_cc_exp);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_exp_month"), &_HTMLTag::autocomplete_cc_exp_month);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_exp_year"), &_HTMLTag::autocomplete_cc_exp_year);

	ClassDB::bind_method(D_METHOD("autocomplete_cc_csc"), &_HTMLTag::autocomplete_cc_csc);
	ClassDB::bind_method(D_METHOD("autocomplete_cc_type"), &_HTMLTag::autocomplete_cc_type);
	ClassDB::bind_method(D_METHOD("autocomplete_transaction_currency"), &_HTMLTag::autocomplete_transaction_currency);
	ClassDB::bind_method(D_METHOD("autocomplete_transaction_amount"), &_HTMLTag::autocomplete_transaction_amount);

	ClassDB::bind_method(D_METHOD("autocomplete_language"), &_HTMLTag::autocomplete_language);
	ClassDB::bind_method(D_METHOD("autocomplete_bday"), &_HTMLTag::autocomplete_bday);
	ClassDB::bind_method(D_METHOD("autocomplete_bday_day"), &_HTMLTag::autocomplete_bday_day);
	ClassDB::bind_method(D_METHOD("autocomplete_bday_month"), &_HTMLTag::autocomplete_bday_month);

	ClassDB::bind_method(D_METHOD("autocomplete_bday_year"), &_HTMLTag::autocomplete_bday_year);
	ClassDB::bind_method(D_METHOD("autocomplete_sex"), &_HTMLTag::autocomplete_sex);
	ClassDB::bind_method(D_METHOD("autocomplete_tel"), &_HTMLTag::autocomplete_tel);
	ClassDB::bind_method(D_METHOD("autocomplete_tel_country_code"), &_HTMLTag::autocomplete_tel_country_code);
	ClassDB::bind_method(D_METHOD("autocomplete_tel_national"), &_HTMLTag::autocomplete_tel_national);

	ClassDB::bind_method(D_METHOD("autocomplete_tel_area_code"), &_HTMLTag::autocomplete_tel_area_code);
	ClassDB::bind_method(D_METHOD("autocomplete_tel_local"), &_HTMLTag::autocomplete_tel_local);
	ClassDB::bind_method(D_METHOD("autocomplete_tel_extension"), &_HTMLTag::autocomplete_tel_extension);
	ClassDB::bind_method(D_METHOD("autocomplete_impp"), &_HTMLTag::autocomplete_impp);
	ClassDB::bind_method(D_METHOD("autocomplete_url"), &_HTMLTag::autocomplete_url);
	ClassDB::bind_method(D_METHOD("autocomplete_photo"), &_HTMLTag::autocomplete_photo);

	ClassDB::bind_method(D_METHOD("onclick", "val"), &_HTMLTag::onclick);

	ClassDB::bind_method(D_METHOD("checked", "val"), &_HTMLTag::checked, true);
	ClassDB::bind_method(D_METHOD("selected", "val"), &_HTMLTag::selected, true);
	ClassDB::bind_method(D_METHOD("autofocus", "val"), &_HTMLTag::autofocus, true);
	ClassDB::bind_method(D_METHOD("disabled", "val"), &_HTMLTag::disabled, true);
	ClassDB::bind_method(D_METHOD("multiple", "val"), &_HTMLTag::multiple, true);
	ClassDB::bind_method(D_METHOD("required", "val"), &_HTMLTag::required, true);
	ClassDB::bind_method(D_METHOD("spellcheck", "val"), &_HTMLTag::spellcheck, true);

	ClassDB::bind_method(D_METHOD("max", "val"), &_HTMLTag::max);
	ClassDB::bind_method(D_METHOD("min", "val"), &_HTMLTag::min);
	ClassDB::bind_method(D_METHOD("step", "val"), &_HTMLTag::step);

	ClassDB::bind_method(D_METHOD("step_any"), &_HTMLTag::step_any);

	ClassDB::bind_method(D_METHOD("minlength", "val"), &_HTMLTag::minlength);
	ClassDB::bind_method(D_METHOD("minlengths", "val"), &_HTMLTag::minlengths);
	ClassDB::bind_method(D_METHOD("maxlength", "val"), &_HTMLTag::maxlength);
	ClassDB::bind_method(D_METHOD("maxlengths", "val"), &_HTMLTag::maxlengths);
	ClassDB::bind_method(D_METHOD("size", "val"), &_HTMLTag::size);
	ClassDB::bind_method(D_METHOD("sizes", "val"), &_HTMLTag::sizes);

	ClassDB::bind_method(D_METHOD("width", "val"), &_HTMLTag::width);
	ClassDB::bind_method(D_METHOD("widths", "val"), &_HTMLTag::widths);
	ClassDB::bind_method(D_METHOD("height", "val"), &_HTMLTag::height);
	ClassDB::bind_method(D_METHOD("heights", "val"), &_HTMLTag::heights);

	ClassDB::bind_method(D_METHOD("pattern", "val"), &_HTMLTag::pattern);

	ClassDB::bind_method(D_METHOD("method", "val"), &_HTMLTag::method);

	ClassDB::bind_method(D_METHOD("method_get"), &_HTMLTag::method_get);
	ClassDB::bind_method(D_METHOD("method_post"), &_HTMLTag::method_post);

	ClassDB::bind_method(D_METHOD("action", "val"), &_HTMLTag::action);
	ClassDB::bind_method(D_METHOD("type", "val"), &_HTMLTag::type);
	ClassDB::bind_method(D_METHOD("placeholder", "val"), &_HTMLTag::placeholder);
	ClassDB::bind_method(D_METHOD("fora", "val"), &_HTMLTag::fora);

	ClassDB::bind_method(D_METHOD("rel", "val"), &_HTMLTag::rel);

	ClassDB::bind_method(D_METHOD("rel_stylesheet"), &_HTMLTag::rel_stylesheet);
	ClassDB::bind_method(D_METHOD("rel_alternate"), &_HTMLTag::rel_alternate);
	ClassDB::bind_method(D_METHOD("rel_author"), &_HTMLTag::rel_author);
	ClassDB::bind_method(D_METHOD("rel_bookmark"), &_HTMLTag::rel_bookmark);
	ClassDB::bind_method(D_METHOD("rel_external"), &_HTMLTag::rel_external);
	ClassDB::bind_method(D_METHOD("rel_help"), &_HTMLTag::rel_help);
	ClassDB::bind_method(D_METHOD("rel_license"), &_HTMLTag::rel_license);

	ClassDB::bind_method(D_METHOD("rel_next"), &_HTMLTag::rel_next);
	ClassDB::bind_method(D_METHOD("rel_nofollow"), &_HTMLTag::rel_nofollow);
	ClassDB::bind_method(D_METHOD("rel_noopener"), &_HTMLTag::rel_noopener);
	ClassDB::bind_method(D_METHOD("rel_noreferrer"), &_HTMLTag::rel_noreferrer);
	ClassDB::bind_method(D_METHOD("rel_prev"), &_HTMLTag::rel_prev);
	ClassDB::bind_method(D_METHOD("rel_search"), &_HTMLTag::rel_search);
	ClassDB::bind_method(D_METHOD("rel_tag"), &_HTMLTag::rel_tag);

	ClassDB::bind_method(D_METHOD("charset", "val"), &_HTMLTag::charset);
	ClassDB::bind_method(D_METHOD("charset_utf_8"), &_HTMLTag::charset_utf_8);

	ClassDB::bind_method(D_METHOD("itbutton"), &_HTMLTag::itbutton);
	ClassDB::bind_method(D_METHOD("itcheckbox"), &_HTMLTag::itcheckbox);
	ClassDB::bind_method(D_METHOD("itcolor"), &_HTMLTag::itcolor);
	ClassDB::bind_method(D_METHOD("itdate"), &_HTMLTag::itdate);
	ClassDB::bind_method(D_METHOD("itdatetime_local"), &_HTMLTag::itdatetime_local);
	ClassDB::bind_method(D_METHOD("itemail"), &_HTMLTag::itemail);
	ClassDB::bind_method(D_METHOD("itfile"), &_HTMLTag::itfile);
	ClassDB::bind_method(D_METHOD("ithidden"), &_HTMLTag::ithidden);
	ClassDB::bind_method(D_METHOD("itimage"), &_HTMLTag::itimage);

	ClassDB::bind_method(D_METHOD("itmonth"), &_HTMLTag::itmonth);
	ClassDB::bind_method(D_METHOD("itnumber"), &_HTMLTag::itnumber);
	ClassDB::bind_method(D_METHOD("itpassword"), &_HTMLTag::itpassword);
	ClassDB::bind_method(D_METHOD("itradio"), &_HTMLTag::itradio);
	ClassDB::bind_method(D_METHOD("itrange"), &_HTMLTag::itrange);

	ClassDB::bind_method(D_METHOD("itreset"), &_HTMLTag::itreset);
	ClassDB::bind_method(D_METHOD("itsearch"), &_HTMLTag::itsearch);
	ClassDB::bind_method(D_METHOD("itsubmit"), &_HTMLTag::itsubmit);
	ClassDB::bind_method(D_METHOD("ittel"), &_HTMLTag::ittel);
	ClassDB::bind_method(D_METHOD("ittext"), &_HTMLTag::ittext);

	ClassDB::bind_method(D_METHOD("ittime"), &_HTMLTag::ittime);
	ClassDB::bind_method(D_METHOD("iturl"), &_HTMLTag::iturl);
	ClassDB::bind_method(D_METHOD("itweek"), &_HTMLTag::itweek);

	ClassDB::bind_method(D_METHOD("inputmode_none"), &_HTMLTag::inputmode_none);
	ClassDB::bind_method(D_METHOD("inputmode_text"), &_HTMLTag::inputmode_text);
	ClassDB::bind_method(D_METHOD("inputmode_decimal"), &_HTMLTag::inputmode_decimal);
	ClassDB::bind_method(D_METHOD("inputmode_numeric"), &_HTMLTag::inputmode_numeric);
	ClassDB::bind_method(D_METHOD("inputmode_tel"), &_HTMLTag::inputmode_tel);
	ClassDB::bind_method(D_METHOD("inputmode_search"), &_HTMLTag::inputmode_search);
	ClassDB::bind_method(D_METHOD("inputmode_email"), &_HTMLTag::inputmode_email);
	ClassDB::bind_method(D_METHOD("inputmode_url"), &_HTMLTag::inputmode_url);

	ClassDB::bind_method(D_METHOD("attrib", "attr", "val"), &_HTMLTag::attrib);

	ClassDB::bind_method(D_METHOD("start", "new_tag", "simple"), &_HTMLTag::start, false);

	ClassDB::bind_method(D_METHOD("reset"), &_HTMLTag::reset);
	ClassDB::bind_method(D_METHOD("close"), &_HTMLTag::close);

	ClassDB::bind_method(D_METHOD("f"), &_HTMLTag::f);

	ClassDB::bind_method(D_METHOD("has_data"), &_HTMLTag::has_data);
}

String _HTMLBuilder::get_result() {
	return result;
}
void _HTMLBuilder::set_result(const String &str) {
	result = str;
}

Ref<_HTMLBuilder> _HTMLBuilder::comment(const String &val) {
	write_tag();

	result += "<!--" + val + "-->";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::doctype(const String &val) {
	write_tag();

	return tag->start("!DOCTYPE");
}

Ref<_HTMLTag> _HTMLBuilder::a(const String &href, const String &cls, const String &id) {
	write_tag();

	tag->start("a");

	if (href != "") {
		tag->href(href);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLBuilder> _HTMLBuilder::fa(const String &href, const String &body, const String &cls, const String &id) {
	a(href, cls, id);
	w(body);
	ca();

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::abbr() {
	write_tag();

	return tag->start("abbr");
}

Ref<_HTMLTag> _HTMLBuilder::acronym() { // Not supported in HTML5. Use <abbr> instead. Defines an acronym
	write_tag();

	return tag->start("acronym");
}

Ref<_HTMLTag> _HTMLBuilder::address() {
	write_tag();

	return tag->start("address");
}

Ref<_HTMLTag> _HTMLBuilder::applet() { // Not supported in HTML5. Use <embed> or <object> instead. Defines an embedded applet
	write_tag();

	return tag->start("applet");
}

Ref<_HTMLTag> _HTMLBuilder::area() {
	write_tag();

	return tag->start("area");
}

Ref<_HTMLTag> _HTMLBuilder::article() {
	write_tag();

	return tag->start("article");
}

Ref<_HTMLTag> _HTMLBuilder::aside() {
	write_tag();

	return tag->start("aside");
}

Ref<_HTMLTag> _HTMLBuilder::audio() {
	write_tag();

	return tag->start("audio");
}

Ref<_HTMLTag> _HTMLBuilder::b() {
	write_tag();

	return tag->start("b");
}

Ref<_HTMLTag> _HTMLBuilder::basefont() { // Not supported in HTML5. Use CSS instead. Specifies a default color, size, and font for all text in a document
	write_tag();

	return tag->start("basefont");
}

Ref<_HTMLTag> _HTMLBuilder::bdi() {
	write_tag();

	return tag->start("bdi");
}

Ref<_HTMLTag> _HTMLBuilder::bdo() {
	write_tag();

	return tag->start("bdo");
}

Ref<_HTMLTag> _HTMLBuilder::big() { // Not supported in HTML5. Use CSS instead. Defines big text
	write_tag();

	return tag->start("big");
}

Ref<_HTMLTag> _HTMLBuilder::blockquote() {
	write_tag();

	return tag->start("blockquote");
}

Ref<_HTMLTag> _HTMLBuilder::body() {
	write_tag();

	return tag->start("body");
}

Ref<_HTMLTag> _HTMLBuilder::br() {
	write_tag();

	return tag->start("br", true);
}

Ref<_HTMLTag> _HTMLBuilder::button() {
	write_tag();

	return tag->start("button");
}

Ref<_HTMLTag> _HTMLBuilder::canvas() {
	write_tag();

	return tag->start("canvas");
}

Ref<_HTMLTag> _HTMLBuilder::caption() {
	write_tag();

	return tag->start("caption");
}

Ref<_HTMLTag> _HTMLBuilder::center() { // Not supported in HTML5. Use CSS instead. Defines centered text
	write_tag();

	return tag->start("center");
}

Ref<_HTMLTag> _HTMLBuilder::cite() {
	write_tag();

	return tag->start("cite");
}

Ref<_HTMLTag> _HTMLBuilder::code() {
	write_tag();

	return tag->start("code");
}

Ref<_HTMLTag> _HTMLBuilder::col() {
	write_tag();

	return tag->start("col");
}

Ref<_HTMLTag> _HTMLBuilder::colgroup() {
	write_tag();

	return tag->start("colgroup");
}

Ref<_HTMLTag> _HTMLBuilder::data() {
	write_tag();

	return tag->start("cite");
}

Ref<_HTMLTag> _HTMLBuilder::datalist() {
	write_tag();

	return tag->start("datalist");
}

Ref<_HTMLTag> _HTMLBuilder::dd() {
	write_tag();

	return tag->start("dd");
}

Ref<_HTMLTag> _HTMLBuilder::del() {
	write_tag();

	return tag->start("del");
}

Ref<_HTMLTag> _HTMLBuilder::details() {
	write_tag();

	return tag->start("details");
}

Ref<_HTMLTag> _HTMLBuilder::dfn() {
	write_tag();

	return tag->start("dfn");
}

Ref<_HTMLTag> _HTMLBuilder::dialog() {
	write_tag();

	return tag->start("dialog");
}

Ref<_HTMLTag> _HTMLBuilder::dir() { // Not supported in HTML5. Use <ul> instead.
	write_tag();

	return tag->start("dir");
}

Ref<_HTMLTag> _HTMLBuilder::div(const String &cls, const String &id) {
	write_tag();

	tag->start("div");

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLBuilder> _HTMLBuilder::fdiv(const String &body, const String &cls, const String &id) {
	div(cls, id);
	w(body);
	cdiv();

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::dl() {
	write_tag();

	return tag->start("dl");
}

Ref<_HTMLTag> _HTMLBuilder::dt() {
	write_tag();

	return tag->start("dt");
}

Ref<_HTMLTag> _HTMLBuilder::em() {
	write_tag();

	return tag->start("em");
}

Ref<_HTMLTag> _HTMLBuilder::embed() {
	write_tag();

	return tag->start("embed");
}

Ref<_HTMLTag> _HTMLBuilder::fieldset() {
	write_tag();

	return tag->start("fieldset");
}
Ref<_HTMLTag> _HTMLBuilder::figcaption() {
	write_tag();

	return tag->start("figcaption");
}

Ref<_HTMLTag> _HTMLBuilder::figure() {
	write_tag();

	return tag->start("figure");
}

Ref<_HTMLTag> _HTMLBuilder::font() { // Not supported in HTML5.
	write_tag();

	return tag->start("font");
}

Ref<_HTMLTag> _HTMLBuilder::footer() {
	write_tag();

	return tag->start("footer");
}

Ref<_HTMLTag> _HTMLBuilder::form() {
	write_tag();

	return tag->start("form");
}

Ref<_HTMLTag> _HTMLBuilder::frame() { // Not supported in HTML5.
	write_tag();

	return tag->start("frame");
}

Ref<_HTMLTag> _HTMLBuilder::frameset() { // Not supported in HTML5.
	write_tag();

	return tag->start("frameset");
}

Ref<_HTMLTag> _HTMLBuilder::h1() {
	write_tag();

	return tag->start("h1");
}

Ref<_HTMLTag> _HTMLBuilder::h2() {
	write_tag();

	return tag->start("h2");
}

Ref<_HTMLTag> _HTMLBuilder::h3() {
	write_tag();

	return tag->start("h3");
}

Ref<_HTMLTag> _HTMLBuilder::h4() {
	write_tag();

	return tag->start("h4");
}

Ref<_HTMLTag> _HTMLBuilder::h5() {
	write_tag();

	return tag->start("h5");
}

Ref<_HTMLTag> _HTMLBuilder::h6() {
	write_tag();

	return tag->start("h6");
}

Ref<_HTMLTag> _HTMLBuilder::head() {
	write_tag();

	return tag->start("head");
}

Ref<_HTMLTag> _HTMLBuilder::header() {
	write_tag();

	return tag->start("header");
}

Ref<_HTMLTag> _HTMLBuilder::hr() {
	write_tag();

	return tag->start("hr");
}

Ref<_HTMLTag> _HTMLBuilder::html() {
	write_tag();

	return tag->start("html");
}

Ref<_HTMLTag> _HTMLBuilder::i() {
	write_tag();

	return tag->start("i");
}

Ref<_HTMLTag> _HTMLBuilder::iframe() {
	write_tag();

	return tag->start("iframe");
}

Ref<_HTMLTag> _HTMLBuilder::img() {
	write_tag();

	return tag->start("img");
}

Ref<_HTMLTag> _HTMLBuilder::input() {
	write_tag();

	return tag->start("input");
}

Ref<_HTMLTag> _HTMLBuilder::ins() {
	write_tag();

	return tag->start("ins");
}

Ref<_HTMLTag> _HTMLBuilder::kbd() {
	write_tag();

	return tag->start("kbd");
}

Ref<_HTMLTag> _HTMLBuilder::label() {
	write_tag();

	return tag->start("label");
}

Ref<_HTMLTag> _HTMLBuilder::legend() {
	write_tag();

	return tag->start("legend");
}

Ref<_HTMLTag> _HTMLBuilder::li() {
	write_tag();

	return tag->start("li");
}

Ref<_HTMLTag> _HTMLBuilder::link() {
	write_tag();

	return tag->start("link");
}

Ref<_HTMLTag> _HTMLBuilder::main() {
	write_tag();

	return tag->start("main");
}

Ref<_HTMLTag> _HTMLBuilder::map() {
	write_tag();

	return tag->start("map");
}
Ref<_HTMLTag> _HTMLBuilder::mark() {
	write_tag();

	return tag->start("mark");
}

Ref<_HTMLTag> _HTMLBuilder::meta() {
	write_tag();

	return tag->start("meta");
}

Ref<_HTMLTag> _HTMLBuilder::meter() {
	write_tag();

	return tag->start("meter");
}

Ref<_HTMLTag> _HTMLBuilder::nav() {
	write_tag();

	return tag->start("nav");
}

Ref<_HTMLTag> _HTMLBuilder::noframes() { // Not supported in HTML5.
	write_tag();

	return tag->start("noframes");
}

Ref<_HTMLTag> _HTMLBuilder::noscript() {
	write_tag();

	return tag->start("noscript");
}

Ref<_HTMLTag> _HTMLBuilder::object() {
	write_tag();

	return tag->start("object");
}

Ref<_HTMLTag> _HTMLBuilder::ol() {
	write_tag();

	return tag->start("ol");
}

Ref<_HTMLTag> _HTMLBuilder::optgroup() {
	write_tag();

	return tag->start("optgroup");
}

Ref<_HTMLTag> _HTMLBuilder::option(const String &value) {
	write_tag();

	tag->start("option");

	if (value != "") {
		tag->value(value);
	}

	return tag;
}
Ref<_HTMLBuilder> _HTMLBuilder::foption(const String &value, const String &body, const bool selected) {
	option(value)->selected(selected);
	w(body);
	coption();

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::output() {
	write_tag();

	return tag->start("output");
}

Ref<_HTMLTag> _HTMLBuilder::p() {
	write_tag();

	return tag->start("p");
}

Ref<_HTMLTag> _HTMLBuilder::param() {
	write_tag();

	return tag->start("param");
}

Ref<_HTMLTag> _HTMLBuilder::picture() {
	write_tag();

	return tag->start("picture");
}

Ref<_HTMLTag> _HTMLBuilder::pre() {
	write_tag();

	return tag->start("pre");
}

Ref<_HTMLTag> _HTMLBuilder::progress() {
	write_tag();

	return tag->start("progress");
}

Ref<_HTMLTag> _HTMLBuilder::q() {
	write_tag();

	return tag->start("q");
}

Ref<_HTMLTag> _HTMLBuilder::rp() {
	write_tag();

	return tag->start("rp");
}

Ref<_HTMLTag> _HTMLBuilder::rt() {
	write_tag();

	return tag->start("rt");
}

Ref<_HTMLTag> _HTMLBuilder::ruby() {
	write_tag();

	return tag->start("ruby");
}

Ref<_HTMLTag> _HTMLBuilder::s() {
	write_tag();

	return tag->start("s");
}

Ref<_HTMLTag> _HTMLBuilder::samp() {
	write_tag();

	return tag->start("samp");
}

Ref<_HTMLTag> _HTMLBuilder::script() {
	write_tag();

	return tag->start("script");
}

Ref<_HTMLTag> _HTMLBuilder::section() {
	write_tag();

	return tag->start("section");
}

Ref<_HTMLTag> _HTMLBuilder::select(const String &name, const String &cls, const String &id) {
	write_tag();

	tag->start("select");

	if (name != "") {
		tag->name(name);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::small() {
	write_tag();

	return tag->start("small");
}

Ref<_HTMLTag> _HTMLBuilder::source() {
	write_tag();

	return tag->start("source");
}

Ref<_HTMLTag> _HTMLBuilder::span() {
	write_tag();

	return tag->start("span");
}

Ref<_HTMLTag> _HTMLBuilder::strike() { // Not supported in HTML5
	write_tag();

	return tag->start("strike");
}

Ref<_HTMLTag> _HTMLBuilder::strong() {
	write_tag();

	return tag->start("strong");
}

Ref<_HTMLTag> _HTMLBuilder::style() {
	write_tag();

	return tag->start("style");
}

Ref<_HTMLTag> _HTMLBuilder::sub() {
	write_tag();

	return tag->start("sub");
}

Ref<_HTMLTag> _HTMLBuilder::summary() {
	write_tag();

	return tag->start("summary");
}

Ref<_HTMLTag> _HTMLBuilder::sup() {
	write_tag();

	return tag->start("sup");
}

Ref<_HTMLTag> _HTMLBuilder::svg() {
	write_tag();

	return tag->start("svg");
}

Ref<_HTMLTag> _HTMLBuilder::table() {
	write_tag();

	return tag->start("table");
}

Ref<_HTMLTag> _HTMLBuilder::tbody() {
	write_tag();

	return tag->start("tbody");
}

Ref<_HTMLTag> _HTMLBuilder::td() {
	write_tag();

	return tag->start("td");
}

Ref<_HTMLTag> _HTMLBuilder::templateh() {
	write_tag();

	return tag->start("template");
}

Ref<_HTMLTag> _HTMLBuilder::textarea(const String &name, const String &cls, const String &id) {
	write_tag();

	tag->start("textarea");

	if (name != "") {
		tag->name(name);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLBuilder> _HTMLBuilder::ftextarea(const String &name, const String &body, const String &cls, const String &id) {
	textarea(name, cls, id);
	w(body);
	ctextarea();

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::tfoot() {
	write_tag();

	return tag->start("tfoot");
}

Ref<_HTMLTag> _HTMLBuilder::th() {
	write_tag();

	return tag->start("th");
}

Ref<_HTMLTag> _HTMLBuilder::thead() {
	write_tag();

	return tag->start("thead");
}

Ref<_HTMLTag> _HTMLBuilder::time() {
	write_tag();

	return tag->start("time");
}

Ref<_HTMLTag> _HTMLBuilder::title() {
	write_tag();

	return tag->start("title");
}

Ref<_HTMLTag> _HTMLBuilder::tr() {
	write_tag();

	return tag->start("tr");
}

Ref<_HTMLTag> _HTMLBuilder::track() {
	write_tag();

	return tag->start("track");
}

Ref<_HTMLTag> _HTMLBuilder::tt() { // Not supported in HTML5.
	write_tag();

	return tag->start("tt");
}

Ref<_HTMLTag> _HTMLBuilder::u() {
	write_tag();

	return tag->start("u");
}

Ref<_HTMLTag> _HTMLBuilder::ul() {
	write_tag();

	return tag->start("ul");
}

Ref<_HTMLTag> _HTMLBuilder::var() {
	write_tag();

	return tag->start("var");
}

Ref<_HTMLTag> _HTMLBuilder::video() {
	write_tag();

	return tag->start("video");
}

Ref<_HTMLTag> _HTMLBuilder::wbr() {
	write_tag();

	return tag->start("wbr");
}

// Closing tags

Ref<_HTMLBuilder> _HTMLBuilder::ca() {
	write_tag();
	result += "</a>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cabbr() {
	write_tag();
	result += "</abbr>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cacronym() {
	write_tag();
	result += "</acronym>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::caddress() {
	write_tag();
	result += "</address>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::capplet() {
	write_tag();
	result += "</applet>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::carea() {
	write_tag();
	result += "</area>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::carticle() {
	write_tag();
	result += "</article>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::caside() {
	write_tag();
	result += "</aside>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::caudio() {
	write_tag();
	result += "</audio>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cb() {
	write_tag();
	result += "</b>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbasefont() {
	write_tag();
	result += "</basefont>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbdi() {
	write_tag();
	result += "</bdi>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbdo() {
	write_tag();
	result += "</bdo>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbig() {
	write_tag();
	result += "</big>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cblockquote() {
	write_tag();
	result += "</blockquote>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbody() {
	write_tag();
	result += "</body>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cbutton() {
	write_tag();
	result += "</button>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccanvas() {
	write_tag();
	result += "</canvas>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccaption() {
	write_tag();
	result += "</caption>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccenter() {
	write_tag();
	result += "</center>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccite() {
	write_tag();
	result += "</cite>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccode() {
	write_tag();
	result += "</code>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccol() {
	write_tag();
	result += "</col>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ccolgroup() {
	write_tag();
	result += "</colgroup>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdata() {
	write_tag();
	result += "</data>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdatalist() {
	write_tag();
	result += "</datalist>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdd() {
	write_tag();
	result += "</dd>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdel() {
	write_tag();
	result += "</del>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdetails() {
	write_tag();
	result += "</details>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdfn() {
	write_tag();
	result += "</dfn>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdialog() {
	write_tag();
	result += "</dialog>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdir() {
	write_tag();
	result += "</dir>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdiv() {
	write_tag();
	result += "</div>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdl() {
	write_tag();
	result += "</dl>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cdt() {
	write_tag();
	result += "</dt>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cem() {
	write_tag();
	result += "</em>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cembed() {
	write_tag();
	result += "</embed>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cfieldset() {
	write_tag();
	result += "</fieldset>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cfigcaption() {
	write_tag();
	result += "</figcaption>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cfigure() {
	write_tag();
	result += "</figure>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cfont() {
	write_tag();
	result += "</font>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cfooter() {
	write_tag();
	result += "</footer>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cform() {
	write_tag();
	result += "</form>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cframe() {
	write_tag();
	result += "</frame>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cframeset() {
	write_tag();
	result += "</frameset>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch1() {
	write_tag();
	result += "</h1>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch2() {
	write_tag();
	result += "</h2>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch3() {
	write_tag();
	result += "</h3>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch4() {
	write_tag();
	result += "</h4>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch5() {
	write_tag();
	result += "</h5>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ch6() {
	write_tag();
	result += "</h6>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::chead() {
	write_tag();
	result += "</head>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cheader() {
	write_tag();
	result += "</header>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::chr() {
	write_tag();
	result += "</hr>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::chtml() {
	write_tag();
	result += "</html>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ci() {
	write_tag();
	result += "</i>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ciframe() {
	write_tag();
	result += "</iframe>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cimg() {
	write_tag();
	result += "</img>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cinput() {
	write_tag();
	result += "</input>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cins() {
	write_tag();
	result += "</ins>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ckbd() {
	write_tag();
	result += "</kbd>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::clabel() {
	write_tag();
	result += "</label>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::clegend() {
	write_tag();
	result += "</legend>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cli() {
	write_tag();
	result += "</li>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::clink() {
	write_tag();
	result += "</link>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cmain() {
	write_tag();
	result += "</main>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cmap() {
	write_tag();
	result += "</map>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cmark() {
	write_tag();
	result += "</mark>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cmeta() {
	write_tag();
	result += "</meta>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cmeter() {
	write_tag();
	result += "</meter>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cnav() {
	write_tag();
	result += "</nav>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cnoframes() {
	write_tag();
	result += "</noframes>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cnoscript() {
	write_tag();
	result += "</noscript>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cobject() {
	write_tag();
	result += "</object>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::c_ol() {
	write_tag();
	result += "</ol>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::coptgroup() {
	write_tag();
	result += "</optgroup>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::coption() {
	write_tag();
	result += "</option>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::coutput() {
	write_tag();
	result += "</output>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cp() {
	write_tag();
	result += "</p>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cparam() {
	write_tag();
	result += "</param>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cpicture() {
	write_tag();
	result += "</picture>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cpre() {
	write_tag();
	result += "</pre>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cprogress() {
	write_tag();
	result += "</progress>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cq() {
	write_tag();
	result += "</a>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::crp() {
	write_tag();
	result += "</rp>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::crt() {
	write_tag();
	result += "</rt>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cruby() {
	write_tag();
	result += "</ruby>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cs() {
	write_tag();
	result += "</s>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csamp() {
	write_tag();
	result += "</samp>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cscript() {
	write_tag();
	result += "</script>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csection() {
	write_tag();
	result += "</section>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cselect() {
	write_tag();
	result += "</select>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csmall() {
	write_tag();
	result += "</small>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csource() {
	write_tag();
	result += "</source>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cspan() {
	write_tag();
	result += "</span>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cstrike() {
	write_tag();
	result += "</strike>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cstrong() {
	write_tag();
	result += "</strong>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cstyle() {
	write_tag();
	result += "</style>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csub() {
	write_tag();
	result += "</sub>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csummary() {
	write_tag();
	result += "</summary>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csup() {
	write_tag();
	result += "</sup>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::csvg() {
	write_tag();
	result += "</svg>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctable() {
	write_tag();
	result += "</table>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctbody() {
	write_tag();
	result += "</tbody>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctd() {
	write_tag();
	result += "</td>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctemplateh() {
	write_tag();
	result += "</templateh>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctextarea() {
	write_tag();
	result += "</textarea>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctfoot() {
	write_tag();
	result += "</tfoot>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cth() {
	write_tag();
	result += "</th>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cthead() {
	write_tag();
	result += "</thead>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctime() {
	write_tag();
	result += "</time>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctitle() {
	write_tag();
	result += "</title>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctr() {
	write_tag();
	result += "</tr>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctrack() {
	write_tag();
	result += "</track>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::ctt() {
	write_tag();
	result += "</tt>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cu() {
	write_tag();
	result += "</u>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cul() {
	write_tag();
	result += "</ul>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cvar() {
	write_tag();
	result += "</var>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cvideo() {
	write_tag();
	result += "</video>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::cwbr() {
	write_tag();
	result += "</wbr>";

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::form_get(const String &action, const String &cls, const String &id) {
	write_tag();

	tag->start("form")->method_get();

	tag->fora(action);

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::form_post(const String &action, const String &cls, const String &id) {
	write_tag();

	tag->start("form")->method_post();

	tag->action(action);

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

/*
Ref<_HTMLBuilder> _HTMLBuilder::form_post(const String &action, Request *request, const String &cls, const String &id) {
	form_post(action, cls, id);
	csrf_token(request);

	return Ref<_HTMLBuilder>(this);
}
*/

Ref<_HTMLBuilder> _HTMLBuilder::flabel(const String &pfor, const String &plabel, const String &cls, const String &id) {
	Ref<_HTMLTag> t = label();

	t->fora(pfor);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	w(plabel);

	clabel();

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::input_button(const String &name, const String &value, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itbutton();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_checkbox(const String &name, const String &value, const bool checked, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itcheckbox();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	tag->checked(checked);

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_color(const String &name, const String &value, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itcolor();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_date(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	write_tag();

	tag->start("input")->itdate();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (date_min != "") {
		tag->min(date_min);
	}

	if (date_max != "") {
		tag->max(date_max);
	}

	if (date_step != "") {
		tag->step(date_step);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_datetime_local(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	write_tag();

	tag->start("input")->itdatetime_local();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (date_min != "") {
		tag->min(date_min);
	}

	if (date_max != "") {
		tag->max(date_max);
	}

	if (date_step != "") {
		tag->step(date_step);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_email(const String &name, const String &value, const String &placeholder, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itemail();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_file(const String &name, const String &accept, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itfile();

	if (name != "") {
		tag->name(name);
	}

	if (accept != "") {
		tag->accept(accept);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::input_image(const String &name, const String &src, const String &alt, const String &cls, const String &id, const int width, const int height) {
	write_tag();

	tag->start("input")->itimage();

	if (name != "") {
		tag->name(name);
	}

	if (src != "") {
		tag->src(src);
	}

	if (alt != "") {
		tag->alt(alt);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (width != 0) {
		tag->width(width);
	}

	if (height != 0) {
		tag->height(height);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_month(const String &name, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itmonth();

	if (name != "") {
		tag->name(name);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_number(const String &name, const String &vmin, const String &vmax, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itnumber();

	if (name != "") {
		tag->name(name);
	}

	if (vmin != "") {
		tag->min(vmin);
	}

	if (vmax != "") {
		tag->max(vmax);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_password(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	write_tag();

	tag->start("input")->itpassword();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (minlength != "") {
		tag->minlengths(minlength);
	}

	if (maxlength != "") {
		tag->maxlengths(maxlength);
	}

	if (size != "") {
		tag->sizes(size);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_radio(const String &name, const String &value, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itradio();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_range(const String &name, const String &value, const String &vmin, const String &vmax, const String &vstep, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itrange();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (vmin != "") {
		tag->min(vmin);
	}

	if (vmax != "") {
		tag->max(vmax);
	}

	if (vstep != "") {
		tag->step(vstep);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLTag> _HTMLBuilder::input_reset(const String &name, const String &value, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itreset();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_search(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	write_tag();

	tag->start("input")->itsearch();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (minlength != "") {
		tag->minlengths(minlength);
	}

	if (maxlength != "") {
		tag->maxlengths(maxlength);
	}

	if (size != "") {
		tag->sizes(size);
	}

	if (pattern != "") {
		tag->pattern(pattern);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_submit(const String &value, const String &cls, const String &id) {
	write_tag();

	tag->start("input")->itsubmit();

	if (value != "") {
		tag->value(value);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_tel(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	write_tag();

	tag->start("input")->ittel();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (minlength != "") {
		tag->minlengths(minlength);
	}

	if (maxlength != "") {
		tag->maxlengths(maxlength);
	}

	if (size != "") {
		tag->sizes(size);
	}

	if (pattern != "") {
		tag->pattern(pattern);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_text(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	write_tag();

	tag->start("input")->ittext();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (minlength != "") {
		tag->minlengths(minlength);
	}

	if (maxlength != "") {
		tag->maxlengths(maxlength);
	}

	if (size != "") {
		tag->sizes(size);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_time(const String &name, const String &cls, const String &id, const String &vmin, const String &vmax, const String &vstep) {
	write_tag();

	tag->start("input")->ittime();

	if (name != "") {
		tag->name(name);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (vmin != "") {
		tag->min(vmin);
	}

	if (vmax != "") {
		tag->max(vmax);
	}

	if (vstep != "") {
		tag->step(vstep);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_url(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	write_tag();

	tag->start("input")->iturl();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	if (placeholder != "") {
		tag->placeholder(placeholder);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (minlength != "") {
		tag->minlengths(minlength);
	}

	if (maxlength != "") {
		tag->maxlengths(maxlength);
	}

	if (size != "") {
		tag->sizes(size);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_week(const String &name, const String &cls, const String &id, const String &vmin, const String &vmax) {
	write_tag();
	return tag->start("input")->itweek();

	if (name != "") {
		tag->name(name);
	}

	if (cls != "") {
		tag->cls(cls);
	}

	if (id != "") {
		tag->id(id);
	}

	if (vmin != "") {
		tag->min(vmin);
	}

	if (vmax != "") {
		tag->max(vmax);
	}

	return tag;
}

Ref<_HTMLTag> _HTMLBuilder::input_hidden(const String &name, const String &value) {
	write_tag();

	tag->start("input")->ithidden();

	if (name != "") {
		tag->name(name);
	}

	if (value != "") {
		tag->value(value);
	}

	return tag;
}

Ref<_HTMLBuilder> _HTMLBuilder::csrf_token(const String &token) {
	if (token == "") {
		// don't waste html characters if it's an empty string anyway
		return Ref<_HTMLBuilder>(this);
	}

	input_hidden("csrf_token", token);

	return Ref<_HTMLBuilder>(this);
}
/*
Ref<_HTMLBuilder> _HTMLBuilder::csrf_token(Request *request) {
	return csrf_token(request->get_csrf_token());
}
*/

void _HTMLBuilder::f() {
	write_tag();
}

Ref<_HTMLBuilder> _HTMLBuilder::w(const String &val) {
	write_tag();

	result += val;

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::wn(const double val, int p_decimals) {
	write_tag();

	result += String::num(val, p_decimals);

	return Ref<_HTMLBuilder>(this);
}
Ref<_HTMLBuilder> _HTMLBuilder::wns(const double val) {
	write_tag();

	result += String::num_scientific(val);

	return Ref<_HTMLBuilder>(this);
}
Ref<_HTMLBuilder> _HTMLBuilder::wr(const double val, const bool p_trailing) {
	write_tag();

	//TODO
	//result += String::num_real(val, p_trailing);

	return Ref<_HTMLBuilder>(this);
}
Ref<_HTMLBuilder> _HTMLBuilder::wi(const int64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_int64(val, base, capitalize_hex);

	return Ref<_HTMLBuilder>(this);
}
Ref<_HTMLBuilder> _HTMLBuilder::wui(const uint64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_uint64(val, base, capitalize_hex);

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::wbn(const bool val) {
	write_tag();

	//TODO
	//result += String::bool_num(val);

	return Ref<_HTMLBuilder>(this);
}
Ref<_HTMLBuilder> _HTMLBuilder::wbs(const bool val) {
	write_tag();

	//TODO
	//result += String::bool_str(val);

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::we(const String &val) {
	print_error("_HTMLBuilder::write_excaped NYI!");

	write_tag();

	result += val;

	return Ref<_HTMLBuilder>(this);
}

Ref<_HTMLBuilder> _HTMLBuilder::write_tag() {
	if (tag->has_data()) {
		tag->close();
		result += tag->result;
		tag->reset();
	}

	return Ref<_HTMLBuilder>(this);
}

_HTMLBuilder::_HTMLBuilder() {
	tag.instance();
	tag->owner = this;
}

_HTMLBuilder::~_HTMLBuilder() {
	tag->owner = nullptr;
	tag.unref();
}

void _HTMLBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_result"), &_HTMLBuilder::get_result);
	ClassDB::bind_method(D_METHOD("set_result", "val"), &_HTMLBuilder::set_result);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "result"), "set_result", "get_result");

	ClassDB::bind_method(D_METHOD("comment", "val"), &_HTMLBuilder::comment);
	ClassDB::bind_method(D_METHOD("doctype", "val"), &_HTMLBuilder::doctype, "");

	ClassDB::bind_method(D_METHOD("a", "href", "cls", "id"), &_HTMLBuilder::a, "", "", "");
	ClassDB::bind_method(D_METHOD("fa", "href", "body", "cls", "id"), &_HTMLBuilder::fa, "", "");

	ClassDB::bind_method(D_METHOD("abbr"), &_HTMLBuilder::abbr);
	ClassDB::bind_method(D_METHOD("acronym"), &_HTMLBuilder::acronym);
	ClassDB::bind_method(D_METHOD("address"), &_HTMLBuilder::address);
	ClassDB::bind_method(D_METHOD("applet"), &_HTMLBuilder::applet);
	ClassDB::bind_method(D_METHOD("area"), &_HTMLBuilder::area);

	ClassDB::bind_method(D_METHOD("article"), &_HTMLBuilder::article);
	ClassDB::bind_method(D_METHOD("aside"), &_HTMLBuilder::aside);
	ClassDB::bind_method(D_METHOD("audio"), &_HTMLBuilder::audio);
	ClassDB::bind_method(D_METHOD("b"), &_HTMLBuilder::b);
	ClassDB::bind_method(D_METHOD("basefont"), &_HTMLBuilder::basefont);

	ClassDB::bind_method(D_METHOD("bdi"), &_HTMLBuilder::bdi);
	ClassDB::bind_method(D_METHOD("bdo"), &_HTMLBuilder::bdo);
	ClassDB::bind_method(D_METHOD("big"), &_HTMLBuilder::big);
	ClassDB::bind_method(D_METHOD("blockquote"), &_HTMLBuilder::blockquote);
	ClassDB::bind_method(D_METHOD("body"), &_HTMLBuilder::body);

	ClassDB::bind_method(D_METHOD("br"), &_HTMLBuilder::br);
	ClassDB::bind_method(D_METHOD("button"), &_HTMLBuilder::button);
	ClassDB::bind_method(D_METHOD("canvas"), &_HTMLBuilder::canvas);
	ClassDB::bind_method(D_METHOD("caption"), &_HTMLBuilder::caption);
	ClassDB::bind_method(D_METHOD("center"), &_HTMLBuilder::center);
	ClassDB::bind_method(D_METHOD("cite"), &_HTMLBuilder::cite);
	ClassDB::bind_method(D_METHOD("code"), &_HTMLBuilder::code);

	ClassDB::bind_method(D_METHOD("col"), &_HTMLBuilder::col);
	ClassDB::bind_method(D_METHOD("colgroup"), &_HTMLBuilder::colgroup);
	ClassDB::bind_method(D_METHOD("data"), &_HTMLBuilder::data);
	ClassDB::bind_method(D_METHOD("datalist"), &_HTMLBuilder::datalist);
	ClassDB::bind_method(D_METHOD("dd"), &_HTMLBuilder::dd);
	ClassDB::bind_method(D_METHOD("del"), &_HTMLBuilder::del);

	ClassDB::bind_method(D_METHOD("details"), &_HTMLBuilder::details);
	ClassDB::bind_method(D_METHOD("dfn"), &_HTMLBuilder::dfn);
	ClassDB::bind_method(D_METHOD("dialog"), &_HTMLBuilder::dialog);
	ClassDB::bind_method(D_METHOD("dir"), &_HTMLBuilder::dir);

	ClassDB::bind_method(D_METHOD("div", "cls", "id"), &_HTMLBuilder::div, "", "");
	ClassDB::bind_method(D_METHOD("fdiv", "body", "cls", "id"), &_HTMLBuilder::fdiv, "", "");

	ClassDB::bind_method(D_METHOD("dl"), &_HTMLBuilder::dl);
	ClassDB::bind_method(D_METHOD("dt"), &_HTMLBuilder::dt);
	ClassDB::bind_method(D_METHOD("em"), &_HTMLBuilder::em);
	ClassDB::bind_method(D_METHOD("embed"), &_HTMLBuilder::embed);
	ClassDB::bind_method(D_METHOD("fieldset"), &_HTMLBuilder::fieldset);
	ClassDB::bind_method(D_METHOD("figcaption"), &_HTMLBuilder::figcaption);

	ClassDB::bind_method(D_METHOD("figure"), &_HTMLBuilder::figure);
	ClassDB::bind_method(D_METHOD("font"), &_HTMLBuilder::font);
	ClassDB::bind_method(D_METHOD("footer"), &_HTMLBuilder::footer);
	ClassDB::bind_method(D_METHOD("form"), &_HTMLBuilder::form);
	ClassDB::bind_method(D_METHOD("frame"), &_HTMLBuilder::frame);
	ClassDB::bind_method(D_METHOD("frameset"), &_HTMLBuilder::frameset);

	ClassDB::bind_method(D_METHOD("h1"), &_HTMLBuilder::h1);
	ClassDB::bind_method(D_METHOD("h2"), &_HTMLBuilder::h2);
	ClassDB::bind_method(D_METHOD("h3"), &_HTMLBuilder::h3);
	ClassDB::bind_method(D_METHOD("h4"), &_HTMLBuilder::h4);
	ClassDB::bind_method(D_METHOD("h5"), &_HTMLBuilder::h5);
	ClassDB::bind_method(D_METHOD("h6"), &_HTMLBuilder::h6);

	ClassDB::bind_method(D_METHOD("head"), &_HTMLBuilder::head);
	ClassDB::bind_method(D_METHOD("header"), &_HTMLBuilder::header);
	ClassDB::bind_method(D_METHOD("hr"), &_HTMLBuilder::hr);
	ClassDB::bind_method(D_METHOD("html"), &_HTMLBuilder::html);

	ClassDB::bind_method(D_METHOD("i"), &_HTMLBuilder::i);
	ClassDB::bind_method(D_METHOD("iframe"), &_HTMLBuilder::iframe);
	ClassDB::bind_method(D_METHOD("img"), &_HTMLBuilder::img);
	ClassDB::bind_method(D_METHOD("input"), &_HTMLBuilder::input);
	ClassDB::bind_method(D_METHOD("ins"), &_HTMLBuilder::ins);
	ClassDB::bind_method(D_METHOD("kbd"), &_HTMLBuilder::kbd);

	ClassDB::bind_method(D_METHOD("label"), &_HTMLBuilder::label);
	ClassDB::bind_method(D_METHOD("legend"), &_HTMLBuilder::legend);
	ClassDB::bind_method(D_METHOD("li"), &_HTMLBuilder::li);
	ClassDB::bind_method(D_METHOD("link"), &_HTMLBuilder::link);
	ClassDB::bind_method(D_METHOD("main"), &_HTMLBuilder::main);
	ClassDB::bind_method(D_METHOD("map"), &_HTMLBuilder::map);
	ClassDB::bind_method(D_METHOD("mark"), &_HTMLBuilder::mark);
	ClassDB::bind_method(D_METHOD("meta"), &_HTMLBuilder::meta);
	ClassDB::bind_method(D_METHOD("meter"), &_HTMLBuilder::meter);

	ClassDB::bind_method(D_METHOD("nav"), &_HTMLBuilder::nav);
	ClassDB::bind_method(D_METHOD("noframes"), &_HTMLBuilder::noframes);
	ClassDB::bind_method(D_METHOD("noscript"), &_HTMLBuilder::noscript);
	ClassDB::bind_method(D_METHOD("object"), &_HTMLBuilder::object);
	ClassDB::bind_method(D_METHOD("ol"), &_HTMLBuilder::ol);
	ClassDB::bind_method(D_METHOD("optgroup"), &_HTMLBuilder::optgroup);

	ClassDB::bind_method(D_METHOD("option", "val"), &_HTMLBuilder::option, "");
	ClassDB::bind_method(D_METHOD("foption", "value", "body", "selected"), &_HTMLBuilder::foption, false);

	ClassDB::bind_method(D_METHOD("output"), &_HTMLBuilder::output);
	ClassDB::bind_method(D_METHOD("p"), &_HTMLBuilder::p);
	ClassDB::bind_method(D_METHOD("param"), &_HTMLBuilder::param);

	ClassDB::bind_method(D_METHOD("picture"), &_HTMLBuilder::picture);
	ClassDB::bind_method(D_METHOD("pre"), &_HTMLBuilder::pre);
	ClassDB::bind_method(D_METHOD("progress"), &_HTMLBuilder::progress);
	ClassDB::bind_method(D_METHOD("q"), &_HTMLBuilder::q);
	ClassDB::bind_method(D_METHOD("rp"), &_HTMLBuilder::rp);

	ClassDB::bind_method(D_METHOD("rt"), &_HTMLBuilder::rt);
	ClassDB::bind_method(D_METHOD("ruby"), &_HTMLBuilder::ruby);
	ClassDB::bind_method(D_METHOD("s"), &_HTMLBuilder::s);
	ClassDB::bind_method(D_METHOD("samp"), &_HTMLBuilder::samp);
	ClassDB::bind_method(D_METHOD("script"), &_HTMLBuilder::script);
	ClassDB::bind_method(D_METHOD("section"), &_HTMLBuilder::section);

	ClassDB::bind_method(D_METHOD("select", "name", "cls", "id"), &_HTMLBuilder::select, "", "", "");

	ClassDB::bind_method(D_METHOD("small"), &_HTMLBuilder::small);
	ClassDB::bind_method(D_METHOD("source"), &_HTMLBuilder::source);
	ClassDB::bind_method(D_METHOD("span"), &_HTMLBuilder::span);
	ClassDB::bind_method(D_METHOD("strike"), &_HTMLBuilder::strike);

	ClassDB::bind_method(D_METHOD("strong"), &_HTMLBuilder::strong);
	ClassDB::bind_method(D_METHOD("style"), &_HTMLBuilder::style);
	ClassDB::bind_method(D_METHOD("sub"), &_HTMLBuilder::sub);
	ClassDB::bind_method(D_METHOD("summary"), &_HTMLBuilder::summary);
	ClassDB::bind_method(D_METHOD("sup"), &_HTMLBuilder::sup);

	ClassDB::bind_method(D_METHOD("svg"), &_HTMLBuilder::svg);
	ClassDB::bind_method(D_METHOD("table"), &_HTMLBuilder::table);
	ClassDB::bind_method(D_METHOD("tbody"), &_HTMLBuilder::tbody);
	ClassDB::bind_method(D_METHOD("td"), &_HTMLBuilder::td);
	ClassDB::bind_method(D_METHOD("templateh"), &_HTMLBuilder::templateh);

	ClassDB::bind_method(D_METHOD("textarea", "name", "cls", "id"), &_HTMLBuilder::textarea, "", "", "");
	ClassDB::bind_method(D_METHOD("ftextarea", "name", "body", "cls", "id"), &_HTMLBuilder::ftextarea, "", "");

	ClassDB::bind_method(D_METHOD("tfoot"), &_HTMLBuilder::tfoot);
	ClassDB::bind_method(D_METHOD("th"), &_HTMLBuilder::th);
	ClassDB::bind_method(D_METHOD("thead"), &_HTMLBuilder::thead);

	ClassDB::bind_method(D_METHOD("time"), &_HTMLBuilder::time);
	ClassDB::bind_method(D_METHOD("title"), &_HTMLBuilder::title);
	ClassDB::bind_method(D_METHOD("tra"), &_HTMLBuilder::tr);
	ClassDB::bind_method(D_METHOD("track"), &_HTMLBuilder::track);
	ClassDB::bind_method(D_METHOD("tt"), &_HTMLBuilder::tt);

	ClassDB::bind_method(D_METHOD("u"), &_HTMLBuilder::u);
	ClassDB::bind_method(D_METHOD("ul"), &_HTMLBuilder::ul);
	ClassDB::bind_method(D_METHOD("var"), &_HTMLBuilder::var);
	ClassDB::bind_method(D_METHOD("video"), &_HTMLBuilder::video);
	ClassDB::bind_method(D_METHOD("wbr"), &_HTMLBuilder::wbr);

	ClassDB::bind_method(D_METHOD("ca"), &_HTMLBuilder::ca);
	ClassDB::bind_method(D_METHOD("cabbr"), &_HTMLBuilder::cabbr);
	ClassDB::bind_method(D_METHOD("cacronym"), &_HTMLBuilder::cacronym);
	ClassDB::bind_method(D_METHOD("caddress"), &_HTMLBuilder::caddress);
	ClassDB::bind_method(D_METHOD("capplet"), &_HTMLBuilder::capplet);
	ClassDB::bind_method(D_METHOD("carea"), &_HTMLBuilder::carea);
	ClassDB::bind_method(D_METHOD("carticle"), &_HTMLBuilder::carticle);

	ClassDB::bind_method(D_METHOD("caside"), &_HTMLBuilder::caside);
	ClassDB::bind_method(D_METHOD("caudio"), &_HTMLBuilder::caudio);
	ClassDB::bind_method(D_METHOD("cb"), &_HTMLBuilder::cb);
	ClassDB::bind_method(D_METHOD("cbasefont"), &_HTMLBuilder::cbasefont);
	ClassDB::bind_method(D_METHOD("cbdi"), &_HTMLBuilder::cbdi);
	ClassDB::bind_method(D_METHOD("cbdo"), &_HTMLBuilder::cbdo);

	ClassDB::bind_method(D_METHOD("cbig"), &_HTMLBuilder::cbig);
	ClassDB::bind_method(D_METHOD("cblockquote"), &_HTMLBuilder::cblockquote);
	ClassDB::bind_method(D_METHOD("cbody"), &_HTMLBuilder::cbody);
	ClassDB::bind_method(D_METHOD("cbutton"), &_HTMLBuilder::cbutton);
	ClassDB::bind_method(D_METHOD("ccanvas"), &_HTMLBuilder::ccanvas);

	ClassDB::bind_method(D_METHOD("ccaption"), &_HTMLBuilder::ccaption);
	ClassDB::bind_method(D_METHOD("ccenter"), &_HTMLBuilder::ccenter);
	ClassDB::bind_method(D_METHOD("ccite"), &_HTMLBuilder::ccite);
	ClassDB::bind_method(D_METHOD("ccode"), &_HTMLBuilder::ccode);
	ClassDB::bind_method(D_METHOD("ccol"), &_HTMLBuilder::ccol);

	ClassDB::bind_method(D_METHOD("ccolgroup"), &_HTMLBuilder::ccolgroup);
	ClassDB::bind_method(D_METHOD("cdata"), &_HTMLBuilder::cdata);
	ClassDB::bind_method(D_METHOD("cdatalist"), &_HTMLBuilder::cdatalist);
	ClassDB::bind_method(D_METHOD("cdd"), &_HTMLBuilder::cdd);
	ClassDB::bind_method(D_METHOD("cdel"), &_HTMLBuilder::cdel);
	ClassDB::bind_method(D_METHOD("cdetails"), &_HTMLBuilder::cdetails);

	ClassDB::bind_method(D_METHOD("cdfn"), &_HTMLBuilder::cdfn);
	ClassDB::bind_method(D_METHOD("cdialog"), &_HTMLBuilder::cdialog);
	ClassDB::bind_method(D_METHOD("cdir"), &_HTMLBuilder::cdir);
	ClassDB::bind_method(D_METHOD("cdiv"), &_HTMLBuilder::cdiv);
	ClassDB::bind_method(D_METHOD("cdl"), &_HTMLBuilder::cdl);
	ClassDB::bind_method(D_METHOD("cdt"), &_HTMLBuilder::cdt);

	ClassDB::bind_method(D_METHOD("cembed"), &_HTMLBuilder::cembed);
	ClassDB::bind_method(D_METHOD("cfieldset"), &_HTMLBuilder::cfieldset);
	ClassDB::bind_method(D_METHOD("cfigcaption"), &_HTMLBuilder::cfigcaption);
	ClassDB::bind_method(D_METHOD("cfigure"), &_HTMLBuilder::cfigure);
	ClassDB::bind_method(D_METHOD("cfont"), &_HTMLBuilder::cfont);

	ClassDB::bind_method(D_METHOD("cfooter"), &_HTMLBuilder::cfooter);
	ClassDB::bind_method(D_METHOD("cform"), &_HTMLBuilder::cform);
	ClassDB::bind_method(D_METHOD("cframe"), &_HTMLBuilder::cframe);
	ClassDB::bind_method(D_METHOD("cframeset"), &_HTMLBuilder::cframeset);
	ClassDB::bind_method(D_METHOD("ch1"), &_HTMLBuilder::ch1);
	ClassDB::bind_method(D_METHOD("ch2"), &_HTMLBuilder::ch2);

	ClassDB::bind_method(D_METHOD("ch3"), &_HTMLBuilder::ch3);
	ClassDB::bind_method(D_METHOD("ch4"), &_HTMLBuilder::ch4);
	ClassDB::bind_method(D_METHOD("ch5"), &_HTMLBuilder::ch5);
	ClassDB::bind_method(D_METHOD("ch6"), &_HTMLBuilder::ch6);

	ClassDB::bind_method(D_METHOD("chead"), &_HTMLBuilder::chead);
	ClassDB::bind_method(D_METHOD("cheader"), &_HTMLBuilder::cheader);
	ClassDB::bind_method(D_METHOD("chr"), &_HTMLBuilder::chr);
	ClassDB::bind_method(D_METHOD("chtml"), &_HTMLBuilder::chtml);

	ClassDB::bind_method(D_METHOD("ci"), &_HTMLBuilder::ci);
	ClassDB::bind_method(D_METHOD("ciframe"), &_HTMLBuilder::ciframe);
	ClassDB::bind_method(D_METHOD("cimg"), &_HTMLBuilder::cimg);
	ClassDB::bind_method(D_METHOD("cinput"), &_HTMLBuilder::cinput);
	ClassDB::bind_method(D_METHOD("cins"), &_HTMLBuilder::cins);
	ClassDB::bind_method(D_METHOD("ckbd"), &_HTMLBuilder::ckbd);

	ClassDB::bind_method(D_METHOD("clabel"), &_HTMLBuilder::clabel);
	ClassDB::bind_method(D_METHOD("clegend"), &_HTMLBuilder::clegend);
	ClassDB::bind_method(D_METHOD("cli"), &_HTMLBuilder::cli);
	ClassDB::bind_method(D_METHOD("clink"), &_HTMLBuilder::clink);

	ClassDB::bind_method(D_METHOD("cmain"), &_HTMLBuilder::cmain);
	ClassDB::bind_method(D_METHOD("cmap"), &_HTMLBuilder::cmap);
	ClassDB::bind_method(D_METHOD("cmark"), &_HTMLBuilder::cmark);
	ClassDB::bind_method(D_METHOD("cmeta"), &_HTMLBuilder::cmeta);

	ClassDB::bind_method(D_METHOD("cmeter"), &_HTMLBuilder::cmeter);
	ClassDB::bind_method(D_METHOD("cnav"), &_HTMLBuilder::cnav);
	ClassDB::bind_method(D_METHOD("cnoframes"), &_HTMLBuilder::cnoframes);

	ClassDB::bind_method(D_METHOD("cnoscript"), &_HTMLBuilder::cnoscript);
	ClassDB::bind_method(D_METHOD("cobject"), &_HTMLBuilder::cobject);
	ClassDB::bind_method(D_METHOD("c_ol"), &_HTMLBuilder::c_ol);
	ClassDB::bind_method(D_METHOD("coptgroup"), &_HTMLBuilder::coptgroup);
	ClassDB::bind_method(D_METHOD("coption"), &_HTMLBuilder::coption);

	ClassDB::bind_method(D_METHOD("coutput"), &_HTMLBuilder::coutput);
	ClassDB::bind_method(D_METHOD("cp"), &_HTMLBuilder::cp);
	ClassDB::bind_method(D_METHOD("cparam"), &_HTMLBuilder::cparam);
	ClassDB::bind_method(D_METHOD("cpicture"), &_HTMLBuilder::cpicture);
	ClassDB::bind_method(D_METHOD("cpre"), &_HTMLBuilder::cpre);

	ClassDB::bind_method(D_METHOD("cprogress"), &_HTMLBuilder::cprogress);
	ClassDB::bind_method(D_METHOD("cq"), &_HTMLBuilder::cq);
	ClassDB::bind_method(D_METHOD("crp"), &_HTMLBuilder::crp);
	ClassDB::bind_method(D_METHOD("crt"), &_HTMLBuilder::crt);
	ClassDB::bind_method(D_METHOD("cruby"), &_HTMLBuilder::cruby);

	ClassDB::bind_method(D_METHOD("cs"), &_HTMLBuilder::cs);
	ClassDB::bind_method(D_METHOD("csamp"), &_HTMLBuilder::csamp);
	ClassDB::bind_method(D_METHOD("cscript"), &_HTMLBuilder::cscript);
	ClassDB::bind_method(D_METHOD("csection"), &_HTMLBuilder::csection);
	ClassDB::bind_method(D_METHOD("cselect"), &_HTMLBuilder::cselect);
	ClassDB::bind_method(D_METHOD("csmall"), &_HTMLBuilder::csmall);

	ClassDB::bind_method(D_METHOD("csource"), &_HTMLBuilder::csource);
	ClassDB::bind_method(D_METHOD("cspan"), &_HTMLBuilder::cspan);
	ClassDB::bind_method(D_METHOD("cstrike"), &_HTMLBuilder::cstrike);
	ClassDB::bind_method(D_METHOD("cstrong"), &_HTMLBuilder::cstrong);

	ClassDB::bind_method(D_METHOD("cstyle"), &_HTMLBuilder::cstyle);
	ClassDB::bind_method(D_METHOD("csub"), &_HTMLBuilder::csub);
	ClassDB::bind_method(D_METHOD("csummary"), &_HTMLBuilder::csummary);
	ClassDB::bind_method(D_METHOD("csup"), &_HTMLBuilder::csup);

	ClassDB::bind_method(D_METHOD("csvg"), &_HTMLBuilder::csvg);
	ClassDB::bind_method(D_METHOD("ctable"), &_HTMLBuilder::ctable);
	ClassDB::bind_method(D_METHOD("ctbody"), &_HTMLBuilder::ctbody);
	ClassDB::bind_method(D_METHOD("ctd"), &_HTMLBuilder::ctd);

	ClassDB::bind_method(D_METHOD("ctemplateh"), &_HTMLBuilder::ctemplateh);
	ClassDB::bind_method(D_METHOD("ctextarea"), &_HTMLBuilder::ctextarea);
	ClassDB::bind_method(D_METHOD("ctfoot"), &_HTMLBuilder::ctfoot);
	ClassDB::bind_method(D_METHOD("cth"), &_HTMLBuilder::cth);
	ClassDB::bind_method(D_METHOD("cthead"), &_HTMLBuilder::cthead);
	ClassDB::bind_method(D_METHOD("ctime"), &_HTMLBuilder::ctime);
	ClassDB::bind_method(D_METHOD("ctitle"), &_HTMLBuilder::ctitle);

	ClassDB::bind_method(D_METHOD("ctr"), &_HTMLBuilder::ctr);
	ClassDB::bind_method(D_METHOD("ctrack"), &_HTMLBuilder::ctrack);
	ClassDB::bind_method(D_METHOD("ctt"), &_HTMLBuilder::ctt);
	ClassDB::bind_method(D_METHOD("cu"), &_HTMLBuilder::cu);

	ClassDB::bind_method(D_METHOD("cul"), &_HTMLBuilder::cul);
	ClassDB::bind_method(D_METHOD("cvar"), &_HTMLBuilder::cvar);
	ClassDB::bind_method(D_METHOD("cvideo"), &_HTMLBuilder::cvideo);
	ClassDB::bind_method(D_METHOD("cwbr"), &_HTMLBuilder::cwbr);

	ClassDB::bind_method(D_METHOD("form_get", "action", "cls", "id"), &_HTMLBuilder::form_get, "", "", "");
	ClassDB::bind_method(D_METHOD("form_post", "action", "cls", "id"), &_HTMLBuilder::form_post, "", "", "");

	//ClassDB::bind_method(D_METHOD("", "name", "cls", "id"), &_HTMLBuilder::, "", "", "");
	// will add a csrf token from request
	//Ref<_HTMLBuilder> form_post(const String &action, Request *request, const String &cls = "", const String &id = "");

	ClassDB::bind_method(D_METHOD("input_button", "name", "value", "cls", "id"), &_HTMLBuilder::input_button, "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_checkbox", "name", "value", "checked", "cls", "id"), &_HTMLBuilder::input_checkbox, "", "", false, "", "");
	ClassDB::bind_method(D_METHOD("input_color", "name", "value", "cls", "id"), &_HTMLBuilder::input_color, "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_date", "name", "value", "cls", "id", "date_min", "date_max", "date_step"), &_HTMLBuilder::input_date, "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_datetime_local", "name", "value", "cls", "id", "date_min", "date_max", "date_step"), &_HTMLBuilder::input_datetime_local, "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_email", "name", "value", "placeholder ", "cls", "id"), &_HTMLBuilder::input_email, "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_file", "name", "accept ", "cls", "id"), &_HTMLBuilder::input_file, "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_image", "name", "src", "alt", "cls", "id", "width", "height"), &_HTMLBuilder::input_image, "", "", "", "", "", 0, 0);
	ClassDB::bind_method(D_METHOD("input_month", "name", "cls", "id"), &_HTMLBuilder::input_month, "", "", "");
	ClassDB::bind_method(D_METHOD("input_number", "name", "vmin", "vmax", "cls", "id"), &_HTMLBuilder::input_number, "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_password", "name", "value", "placeholder", "cls", "id", "minlength", "maxlength", "size"), &_HTMLBuilder::input_password, "", "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_radio", "name", "value", "cls", "id"), &_HTMLBuilder::input_radio, "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_range", "name", "value", "vmin", "vmax", "vstep", "cls", "id"), &_HTMLBuilder::input_range, "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_reset", "name", "value", "cls", "id"), &_HTMLBuilder::input_reset, "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_search", "name", "value", "placeholder ", "cls", "id", "minlength", "maxlength", "size"), &_HTMLBuilder::input_search, "", "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_submit", "value", "cls", "id"), &_HTMLBuilder::input_submit, "", "", "");
	ClassDB::bind_method(D_METHOD("input_tel", "name", "value", "placeholder", "cls", "id", "minlength", "maxlength", "size"), &_HTMLBuilder::input_tel, "", "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_text", "name", "value", "placeholder", "cls", "id", "minlength", "maxlength", "size"), &_HTMLBuilder::input_text, "", "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_time", "name", "cls", "id", "vmin", "vmax", "vstep"), &_HTMLBuilder::input_time, "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_url", "name", "value", "placeholder ", "cls", "id", "minlength", "maxlength", "size"), &_HTMLBuilder::input_url, "", "", "", "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_week", "name", "cls", "id", "vmin", "vmax"), &_HTMLBuilder::input_week, "", "", "", "", "");
	ClassDB::bind_method(D_METHOD("input_hidden", "name", "value"), &_HTMLBuilder::input_hidden, "", "");

	ClassDB::bind_method(D_METHOD("flabel", "pfor", "plabel", "cls", "id"), &_HTMLBuilder::flabel, "", "");

	ClassDB::bind_method(D_METHOD("csrf_token", "token"), &_HTMLBuilder::csrf_token);
	//ClassDB::bind_method(D_METHOD("csrf_token", "request"), &_HTMLBuilder::csrf_token);

	ClassDB::bind_method(D_METHOD("f"), &_HTMLBuilder::f);

	ClassDB::bind_method(D_METHOD("w", "val"), &_HTMLBuilder::w);
	ClassDB::bind_method(D_METHOD("wn", "val", "decimals "), &_HTMLBuilder::wn, -1);
	ClassDB::bind_method(D_METHOD("wns", "val"), &_HTMLBuilder::wns);
	ClassDB::bind_method(D_METHOD("wr", "val", "trailing "), &_HTMLBuilder::wr, true);
	ClassDB::bind_method(D_METHOD("wi", "val", "base", "capitalize_hex"), &_HTMLBuilder::wi, 10, false);
	ClassDB::bind_method(D_METHOD("wui", "val", "base", "capitalize_hex"), &_HTMLBuilder::wui, 10, false);
	ClassDB::bind_method(D_METHOD("wbn", "val"), &_HTMLBuilder::wbn);
	ClassDB::bind_method(D_METHOD("wbs", "val"), &_HTMLBuilder::wbs);
	ClassDB::bind_method(D_METHOD("we", "val"), &_HTMLBuilder::we);

	ClassDB::bind_method(D_METHOD("write_tag"), &_HTMLBuilder::write_tag);
}
