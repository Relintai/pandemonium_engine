#include "html_builder_bind.h"
#include "core/print_string.h"
#include "core/ustring.h"

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
Ref<_HTMLTag> _HTMLTag::minlength(const String &val) {
	attrib("minlength", val);

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::maxlength(const int val) {
	attrib("maxlength", String::num(val));

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::maxlength(const String &val) {
	attrib("maxlength", val);

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::size(const int val) {
	attrib("size", String::num(val));

	return Ref<_HTMLTag>(this);
}
Ref<_HTMLTag> _HTMLTag::size(const String &val) {
	attrib("size", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::width(const int val) {
	attrib("width", String::num(val));

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::width(const String &val) {
	attrib("width", val);

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::height(const int val) {
	attrib("height", String::num(val));

	return Ref<_HTMLTag>(this);
}

Ref<_HTMLTag> _HTMLTag::height(const String &val) {
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
	return owner;
}

bool _HTMLTag::has_data() {
	return result.size() > 0;
}

_HTMLTag::_HTMLTag() {
	simple = true;
}

void _HTMLTag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("style", "val"), &_HTMLTag::style);
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

Ref<_HTMLTag> _HTMLBuilder::doctype() {
	write_tag();

	return tag->start("!DOCTYPE");
}

Ref<_HTMLBuilder> _HTMLBuilder::doctype(const String &val) {
	write_tag();

	result += "<!DOCTYPE " + val + ">";

	return Ref<_HTMLBuilder>(this);
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
		tag->minlength(minlength);
	}

	if (maxlength != "") {
		tag->maxlength(maxlength);
	}

	if (size != "") {
		tag->size(size);
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
		tag->minlength(minlength);
	}

	if (maxlength != "") {
		tag->maxlength(maxlength);
	}

	if (size != "") {
		tag->size(size);
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
		tag->minlength(minlength);
	}

	if (maxlength != "") {
		tag->maxlength(maxlength);
	}

	if (size != "") {
		tag->size(size);
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
		tag->minlength(minlength);
	}

	if (maxlength != "") {
		tag->maxlength(maxlength);
	}

	if (size != "") {
		tag->size(size);
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
		tag->minlength(minlength);
	}

	if (maxlength != "") {
		tag->maxlength(maxlength);
	}

	if (size != "") {
		tag->size(size);
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
	tag->owner.reference_ptr(this);
}

_HTMLBuilder::~_HTMLBuilder() {
}

void _HTMLBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("a", "href", "cls", "id"), &_HTMLBuilder::a, "", "", "");
}
