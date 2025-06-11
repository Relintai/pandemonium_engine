/*************************************************************************/
/*  html_builder.cpp                                                     */
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

#include "html_builder.h"

#include "../http/web_server_request.h"

String HTMLTag::get_result() {
	return result;
}
void HTMLTag::set_result(const String &str) {
	result = str;
}

HTMLTag *HTMLTag::str(const String &str) {
	result += " " + str;

	return this;
}

HTMLTag *HTMLTag::style(const String &val) {
	attrib("style", val);

	return this;
}

HTMLTag *HTMLTag::href(const String &val) {
	attrib("href", val);

	return this;
}

HTMLTag *HTMLTag::cls(const String &val) {
	attrib("class", val);

	return this;
}

HTMLTag *HTMLTag::clsse(const String &val) {
	if (val == "") {
		return this;
	}

	attrib("class", val);

	return this;
}

HTMLTag *HTMLTag::id(const String &val) {
	attrib("id", val);

	return this;
}

HTMLTag *HTMLTag::name(const String &val) {
	attrib("name", val);

	return this;
}

HTMLTag *HTMLTag::content(const String &val) {
	attrib("content", val);

	return this;
}

HTMLTag *HTMLTag::value(const String &val) {
	attrib("value", val);

	return this;
}

HTMLTag *HTMLTag::accept(const String &val) {
	attrib("accept", val);

	return this;
}

HTMLTag *HTMLTag::src(const String &val) {
	attrib("src", val);

	return this;
}

HTMLTag *HTMLTag::alt(const String &val) {
	attrib("alt", val);

	return this;
}

HTMLTag *HTMLTag::rows(const String &val) {
	attrib("rows", val);

	return this;
}

HTMLTag *HTMLTag::cols(const String &val) {
	attrib("cols", val);

	return this;
}

HTMLTag *HTMLTag::enctype(const String &val) {
	attrib("enctype", val);

	return this;
}

HTMLTag *HTMLTag::enctype_multipart_form_data() {
	attrib("enctype", "multipart/form-data");

	return this;
}

HTMLTag *HTMLTag::autocomplete(const String &val) {
	attrib("autocomplete", val);

	return this;
}

HTMLTag *HTMLTag::autocomplete_off() {
	attrib("autocomplete", "off");

	return this;
}
HTMLTag *HTMLTag::autocomplete_on() {
	attrib("autocomplete", "on");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name() {
	attrib("autocomplete", "name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_honorific_prefix() {
	attrib("autocomplete", "honorific-prefix");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_given_name() {
	attrib("autocomplete", "given-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_additional_name() {
	attrib("autocomplete", "additional-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_family_name() {
	attrib("autocomplete", "family-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_honorific_suffix() {
	attrib("autocomplete", "honorific-suffix");

	return this;
}
HTMLTag *HTMLTag::autocomplete_name_nickname() {
	attrib("autocomplete", "nickname");

	return this;
}
HTMLTag *HTMLTag::autocomplete_email() {
	attrib("autocomplete", "email");

	return this;
}
HTMLTag *HTMLTag::autocomplete_username() {
	attrib("autocomplete", "username");

	return this;
}
HTMLTag *HTMLTag::autocomplete_new_password() {
	attrib("autocomplete", "new-password");

	return this;
}
HTMLTag *HTMLTag::autocomplete_current_password() {
	attrib("autocomplete", "current-password");

	return this;
}
HTMLTag *HTMLTag::autocomplete_one_time_code() {
	attrib("autocomplete", "one-time-code");

	return this;
}
HTMLTag *HTMLTag::autocomplete_organization_title() {
	attrib("autocomplete", "organization-title");

	return this;
}
HTMLTag *HTMLTag::autocomplete_organization() {
	attrib("autocomplete", "organization");

	return this;
}
HTMLTag *HTMLTag::autocomplete_street_address() {
	attrib("autocomplete", "street-address");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_line1() {
	attrib("autocomplete", "address-line1");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_line2() {
	attrib("autocomplete", "address-line2");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_line3() {
	attrib("autocomplete", "address-line3");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_level_1() {
	attrib("autocomplete", "address-level1");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_level_2() {
	attrib("autocomplete", "address-level2");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_level_3() {
	attrib("autocomplete", "address-level3");

	return this;
}
HTMLTag *HTMLTag::autocomplete_address_level_4() {
	attrib("autocomplete", "address-level4");

	return this;
}
HTMLTag *HTMLTag::autocomplete_country() {
	attrib("autocomplete", "country");

	return this;
}
HTMLTag *HTMLTag::autocomplete_country_name() {
	attrib("autocomplete", "country-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_postal_code() {
	attrib("autocomplete", "postal-code");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_name() {
	attrib("autocomplete", "cc-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_given_name() {
	attrib("autocomplete", "cc-given-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_additional_name() {
	attrib("autocomplete", "cc-additional-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_family_name() {
	attrib("autocomplete", "cc-family-name");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_number() {
	attrib("autocomplete", "cc-number");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_exp() {
	attrib("autocomplete", "cc-exp");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_exp_month() {
	attrib("autocomplete", "cc-exp-month");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_exp_year() {
	attrib("autocomplete", "cc-exp-year");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_csc() {
	attrib("autocomplete", "cc-csc");

	return this;
}
HTMLTag *HTMLTag::autocomplete_cc_type() {
	attrib("autocomplete", "cc-type");

	return this;
}
HTMLTag *HTMLTag::autocomplete_transaction_currency() {
	attrib("autocomplete", "transaction-currency");

	return this;
}
HTMLTag *HTMLTag::autocomplete_transaction_amount() {
	attrib("autocomplete", "transaction-amount");

	return this;
}
HTMLTag *HTMLTag::autocomplete_language() {
	attrib("autocomplete", "language");

	return this;
}
HTMLTag *HTMLTag::autocomplete_bday() {
	attrib("autocomplete", "bday");

	return this;
}
HTMLTag *HTMLTag::autocomplete_bday_day() {
	attrib("autocomplete", "bday-day");

	return this;
}
HTMLTag *HTMLTag::autocomplete_bday_month() {
	attrib("autocomplete", "bday-month");

	return this;
}
HTMLTag *HTMLTag::autocomplete_bday_year() {
	attrib("autocomplete", "bday-year");

	return this;
}
HTMLTag *HTMLTag::autocomplete_sex() {
	attrib("autocomplete", "sex");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel() {
	attrib("autocomplete", "tel");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel_country_code() {
	attrib("autocomplete", "tel-country-code");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel_national() {
	attrib("autocomplete", "tel-national");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel_area_code() {
	attrib("autocomplete", "tel-area-code");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel_local() {
	attrib("autocomplete", "tel-local");

	return this;
}
HTMLTag *HTMLTag::autocomplete_tel_extension() {
	attrib("autocomplete", "tel-extension");

	return this;
}
HTMLTag *HTMLTag::autocomplete_impp() {
	attrib("autocomplete", "impp");

	return this;
}
HTMLTag *HTMLTag::autocomplete_url() {
	attrib("autocomplete", "url");

	return this;
}
HTMLTag *HTMLTag::autocomplete_photo() {
	attrib("autocomplete", "photo");

	return this;
}

HTMLTag *HTMLTag::onclick(const String &val) {
	attrib("onclick", val);

	return this;
}

HTMLTag *HTMLTag::inputmode(const String &val) {
	attrib("inputmode", val);

	return this;
}

HTMLTag *HTMLTag::list(const String &val) {
	attrib("list", val);

	return this;
}

HTMLTag *HTMLTag::checked(const bool val) {
	if (val) {
		result += " checked";
	}

	return this;
}

HTMLTag *HTMLTag::selected(const bool val) {
	if (val) {
		result += " selected";
	}

	return this;
}

HTMLTag *HTMLTag::autofocus(const bool val) {
	if (val) {
		result += " autofocus";
	}

	return this;
}

HTMLTag *HTMLTag::disabled(const bool val) {
	if (val) {
		result += " disabled";
	}

	return this;
}

HTMLTag *HTMLTag::multiple(const bool val) {
	if (val) {
		result += " multiple";
	}

	return this;
}

HTMLTag *HTMLTag::required(const bool val) {
	if (val) {
		result += " required";
	}

	return this;
}

HTMLTag *HTMLTag::spellcheck(const bool val) {
	if (val) {
		attrib("spellcheck", "true");
	} else {
		attrib("spellcheck", "false");
	}

	return this;
}

HTMLTag *HTMLTag::max(const String &val) {
	attrib("max", val);

	return this;
}

HTMLTag *HTMLTag::min(const String &val) {
	attrib("min", val);

	return this;
}

HTMLTag *HTMLTag::step(const String &val) {
	attrib("step", val);

	return this;
}
HTMLTag *HTMLTag::step_any() {
	attrib("step", "any");

	return this;
}

HTMLTag *HTMLTag::minlength(const int val) {
	attrib("minlength", String::num(val));

	return this;
}
HTMLTag *HTMLTag::minlength(const String &val) {
	attrib("minlength", val);

	return this;
}
HTMLTag *HTMLTag::maxlength(const int val) {
	attrib("maxlength", String::num(val));

	return this;
}
HTMLTag *HTMLTag::maxlength(const String &val) {
	attrib("maxlength", val);

	return this;
}
HTMLTag *HTMLTag::size(const int val) {
	attrib("size", String::num(val));

	return this;
}
HTMLTag *HTMLTag::size(const String &val) {
	attrib("size", val);

	return this;
}

HTMLTag *HTMLTag::width(const int val) {
	attrib("width", String::num(val));

	return this;
}

HTMLTag *HTMLTag::width(const String &val) {
	attrib("width", val);

	return this;
}

HTMLTag *HTMLTag::height(const int val) {
	attrib("height", String::num(val));

	return this;
}

HTMLTag *HTMLTag::height(const String &val) {
	attrib("height", val);

	return this;
}

HTMLTag *HTMLTag::pattern(const String &val) {
	attrib("pattern", val);

	return this;
}

HTMLTag *HTMLTag::method(const String &val) {
	attrib("method", val);

	return this;
}

HTMLTag *HTMLTag::method_get() {
	attrib("method", "get");

	return this;
}
HTMLTag *HTMLTag::method_post() {
	attrib("method", "post");

	return this;
}

HTMLTag *HTMLTag::action(const String &val) {
	attrib("action", val);

	return this;
}

HTMLTag *HTMLTag::type(const String &val) {
	attrib("type", val);

	return this;
}

HTMLTag *HTMLTag::placeholder(const String &val) {
	attrib("placeholder", val);

	return this;
}

HTMLTag *HTMLTag::fora(const String &val) {
	attrib("for", val);

	return this;
}

HTMLTag *HTMLTag::rel(const String &val) {
	attrib("rel", val);

	return this;
}

HTMLTag *HTMLTag::rel_stylesheet() {
	attrib("rel", "stylesheet");

	return this;
}

HTMLTag *HTMLTag::rel_alternate() {
	attrib("rel", "alternate");

	return this;
}

HTMLTag *HTMLTag::rel_author() {
	attrib("rel", "author");

	return this;
}

HTMLTag *HTMLTag::rel_bookmark() {
	attrib("rel", "bookmark");

	return this;
}

HTMLTag *HTMLTag::rel_external() {
	attrib("rel", "external");

	return this;
}

HTMLTag *HTMLTag::rel_help() {
	attrib("rel", "help");

	return this;
}

HTMLTag *HTMLTag::rel_next() {
	attrib("rel", "next");

	return this;
}

HTMLTag *HTMLTag::rel_nofollow() {
	attrib("rel", "nofollow");

	return this;
}

HTMLTag *HTMLTag::rel_noopener() {
	attrib("rel", "noopener");

	return this;
}

HTMLTag *HTMLTag::rel_noreferrer() {
	attrib("rel", "noreferrer");

	return this;
}

HTMLTag *HTMLTag::rel_prev() {
	attrib("rel", "prev");

	return this;
}

HTMLTag *HTMLTag::rel_search() {
	attrib("rel", "search");

	return this;
}

HTMLTag *HTMLTag::rel_tag() {
	attrib("rel", "_tag");

	return this;
}

HTMLTag *HTMLTag::charset(const String &val) {
	attrib("charset", val);

	return this;
}

HTMLTag *HTMLTag::charset_utf_8() {
	attrib("charset", "utf-8");

	return this;
}

HTMLTag *HTMLTag::itbutton() {
	attrib("type", "button");

	return this;
}
HTMLTag *HTMLTag::itcheckbox() {
	attrib("type", "checkbox");

	return this;
}
HTMLTag *HTMLTag::itcolor() {
	attrib("type", "color");

	return this;
}
HTMLTag *HTMLTag::itdate() {
	attrib("type", "date");

	return this;
}
HTMLTag *HTMLTag::itdatetime_local() {
	attrib("type", "datetime_local");

	return this;
}
HTMLTag *HTMLTag::itemail() {
	attrib("type", "email");

	return this;
}
HTMLTag *HTMLTag::itfile() {
	attrib("type", "file");

	return this;
}
HTMLTag *HTMLTag::ithidden() {
	attrib("type", "hidden");

	return this;
}
HTMLTag *HTMLTag::itimage() {
	attrib("type", "image");

	return this;
}
HTMLTag *HTMLTag::itmonth() {
	attrib("type", "month");

	return this;
}
HTMLTag *HTMLTag::itnumber() {
	attrib("type", "number");

	return this;
}
HTMLTag *HTMLTag::itpassword() {
	attrib("type", "password");

	return this;
}
HTMLTag *HTMLTag::itradio() {
	attrib("type", "radio");

	return this;
}
HTMLTag *HTMLTag::itrange() {
	attrib("type", "range");

	return this;
}
HTMLTag *HTMLTag::itreset() {
	attrib("type", "reset");

	return this;
}
HTMLTag *HTMLTag::itsearch() {
	attrib("type", "search");

	return this;
}
HTMLTag *HTMLTag::itsubmit() {
	attrib("type", "submit");

	return this;
}
HTMLTag *HTMLTag::ittel() {
	attrib("type", "tel");

	return this;
}
HTMLTag *HTMLTag::ittext() {
	attrib("type", "text");

	return this;
}
HTMLTag *HTMLTag::ittime() {
	attrib("type", "time");

	return this;
}
HTMLTag *HTMLTag::iturl() {
	attrib("type", "url");

	return this;
}
HTMLTag *HTMLTag::itweek() {
	attrib("type", "week");

	return this;
}

HTMLTag *HTMLTag::inputmode_none() {
	attrib("inputmode", "none");

	return this;
}
HTMLTag *HTMLTag::inputmode_text() {
	attrib("inputmode", "text");

	return this;
}
HTMLTag *HTMLTag::inputmode_decimal() {
	attrib("inputmode", "decimal");

	return this;
}
HTMLTag *HTMLTag::inputmode_numeric() {
	attrib("inputmode", "numeric");

	return this;
}
HTMLTag *HTMLTag::inputmode_tel() {
	attrib("inputmode", "tel");

	return this;
}
HTMLTag *HTMLTag::inputmode_search() {
	attrib("inputmode", "search");

	return this;
}
HTMLTag *HTMLTag::inputmode_email() {
	attrib("inputmode", "email");

	return this;
}
HTMLTag *HTMLTag::inputmode_url() {
	attrib("inputmode", "url");

	return this;
}

HTMLTag *HTMLTag::attrib(const String &attr, const String &val) {
	result += " " + attr + "=\"" + val + "\"";

	return this;
}

HTMLTag *HTMLTag::start(const String &p_tag, const bool p_simple) {
	simple = p_simple;

	result = "<" + p_tag;

	return this;
}

HTMLTag *HTMLTag::reset() {
	result.clear();

	return this;
}

HTMLTag *HTMLTag::close() {
	if (simple) {
		result += "/>";
	} else {
		result += ">";
	}

	return this;
}

HTMLBuilder *HTMLTag::f() {
	return owner;
}

bool HTMLTag::has_data() {
	return result.length() > 0;
}

HTMLTag::HTMLTag() {
	simple = true;
	owner = nullptr;
}



String HTMLBuilder::get_result() {
	return result;
}
void HTMLBuilder::set_result(const String &str) {
	result = str;
}

HTMLBuilder *HTMLBuilder::comment(const String &val) {
	write_tag();

	result += "<!--" + val + "-->";

	return this;
}

HTMLTag *HTMLBuilder::doctype() {
	write_tag();

	return _tag.start("!DOCTYPE");
}

HTMLBuilder *HTMLBuilder::doctype(const String &val) {
	write_tag();

	result += "<!DOCTYPE " + val + ">";

	return this;
}

HTMLTag *HTMLBuilder::a() {
	write_tag();

	return _tag.start("a");
}
HTMLTag *HTMLBuilder::abbr() {
	write_tag();

	return _tag.start("abbr");
}

HTMLTag *HTMLBuilder::acronym() { // Not supported in HTML5. Use <abbr> instead. Defines an acronym
	write_tag();

	return _tag.start("acronym");
}

HTMLTag *HTMLBuilder::address() {
	write_tag();

	return _tag.start("address");
}

HTMLTag *HTMLBuilder::applet() { // Not supported in HTML5. Use <embed> or <object> instead. Defines an embedded applet
	write_tag();

	return _tag.start("applet");
}

HTMLTag *HTMLBuilder::area() {
	write_tag();

	return _tag.start("area");
}

HTMLTag *HTMLBuilder::article() {
	write_tag();

	return _tag.start("article");
}

HTMLTag *HTMLBuilder::aside() {
	write_tag();

	return _tag.start("aside");
}

HTMLTag *HTMLBuilder::audio() {
	write_tag();

	return _tag.start("audio");
}

HTMLTag *HTMLBuilder::b() {
	write_tag();

	return _tag.start("b");
}

HTMLTag *HTMLBuilder::basefont() { // Not supported in HTML5. Use CSS instead. Specifies a default color, size, and font for all text in a document
	write_tag();

	return _tag.start("basefont");
}

HTMLTag *HTMLBuilder::bdi() {
	write_tag();

	return _tag.start("bdi");
}

HTMLTag *HTMLBuilder::bdo() {
	write_tag();

	return _tag.start("bdo");
}

HTMLTag *HTMLBuilder::big() { // Not supported in HTML5. Use CSS instead. Defines big text
	write_tag();

	return _tag.start("big");
}

HTMLTag *HTMLBuilder::blockquote() {
	write_tag();

	return _tag.start("blockquote");
}

HTMLTag *HTMLBuilder::body() {
	write_tag();

	return _tag.start("body");
}

HTMLTag *HTMLBuilder::br() {
	write_tag();

	return _tag.start("br", true);
}

HTMLTag *HTMLBuilder::button() {
	write_tag();

	return _tag.start("button");
}

HTMLTag *HTMLBuilder::canvas() {
	write_tag();

	return _tag.start("canvas");
}

HTMLTag *HTMLBuilder::caption() {
	write_tag();

	return _tag.start("caption");
}

HTMLTag *HTMLBuilder::center() { // Not supported in HTML5. Use CSS instead. Defines centered text
	write_tag();

	return _tag.start("center");
}

HTMLTag *HTMLBuilder::cite() {
	write_tag();

	return _tag.start("cite");
}

HTMLTag *HTMLBuilder::code() {
	write_tag();

	return _tag.start("code");
}

HTMLTag *HTMLBuilder::col() {
	write_tag();

	return _tag.start("col");
}

HTMLTag *HTMLBuilder::colgroup() {
	write_tag();

	return _tag.start("colgroup");
}

HTMLTag *HTMLBuilder::data() {
	write_tag();

	return _tag.start("data");
}

HTMLTag *HTMLBuilder::datalist() {
	write_tag();

	return _tag.start("datalist");
}

HTMLTag *HTMLBuilder::dd() {
	write_tag();

	return _tag.start("dd");
}

HTMLTag *HTMLBuilder::del() {
	write_tag();

	return _tag.start("del");
}

HTMLTag *HTMLBuilder::details() {
	write_tag();

	return _tag.start("details");
}

HTMLTag *HTMLBuilder::dfn() {
	write_tag();

	return _tag.start("dfn");
}

HTMLTag *HTMLBuilder::dialog() {
	write_tag();

	return _tag.start("dialog");
}

HTMLTag *HTMLBuilder::dir() { // Not supported in HTML5. Use <ul> instead.
	write_tag();

	return _tag.start("dir");
}

HTMLTag *HTMLBuilder::div() {
	write_tag();

	return _tag.start("div");
}

HTMLTag *HTMLBuilder::dl() {
	write_tag();

	return _tag.start("dl");
}

HTMLTag *HTMLBuilder::dt() {
	write_tag();

	return _tag.start("dt");
}

HTMLTag *HTMLBuilder::em() {
	write_tag();

	return _tag.start("em");
}

HTMLTag *HTMLBuilder::embed() {
	write_tag();

	return _tag.start("embed");
}

HTMLTag *HTMLBuilder::fieldset() {
	write_tag();

	return _tag.start("fieldset");
}
HTMLTag *HTMLBuilder::figcaption() {
	write_tag();

	return _tag.start("figcaption");
}

HTMLTag *HTMLBuilder::figure() {
	write_tag();

	return _tag.start("figure");
}

HTMLTag *HTMLBuilder::font() { // Not supported in HTML5.
	write_tag();

	return _tag.start("font");
}

HTMLTag *HTMLBuilder::footer() {
	write_tag();

	return _tag.start("footer");
}

HTMLTag *HTMLBuilder::form() {
	write_tag();

	return _tag.start("form");
}

HTMLTag *HTMLBuilder::frame() { // Not supported in HTML5.
	write_tag();

	return _tag.start("frame");
}

HTMLTag *HTMLBuilder::frameset() { // Not supported in HTML5.
	write_tag();

	return _tag.start("frameset");
}

HTMLTag *HTMLBuilder::h1() {
	write_tag();

	return _tag.start("h1");
}

HTMLTag *HTMLBuilder::h2() {
	write_tag();

	return _tag.start("h2");
}

HTMLTag *HTMLBuilder::h3() {
	write_tag();

	return _tag.start("h3");
}

HTMLTag *HTMLBuilder::h4() {
	write_tag();

	return _tag.start("h4");
}

HTMLTag *HTMLBuilder::h5() {
	write_tag();

	return _tag.start("h5");
}

HTMLTag *HTMLBuilder::h6() {
	write_tag();

	return _tag.start("h6");
}

HTMLTag *HTMLBuilder::head() {
	write_tag();

	return _tag.start("head");
}

HTMLTag *HTMLBuilder::header() {
	write_tag();

	return _tag.start("header");
}

HTMLTag *HTMLBuilder::hr() {
	write_tag();

	return _tag.start("hr");
}

HTMLTag *HTMLBuilder::html() {
	write_tag();

	return _tag.start("html");
}

HTMLTag *HTMLBuilder::i() {
	write_tag();

	return _tag.start("i");
}

HTMLTag *HTMLBuilder::iframe() {
	write_tag();

	return _tag.start("iframe");
}

HTMLTag *HTMLBuilder::img() {
	write_tag();

	return _tag.start("img");
}

HTMLTag *HTMLBuilder::input() {
	write_tag();

	return _tag.start("input");
}

HTMLTag *HTMLBuilder::ins() {
	write_tag();

	return _tag.start("ins");
}

HTMLTag *HTMLBuilder::kbd() {
	write_tag();

	return _tag.start("kbd");
}

HTMLTag *HTMLBuilder::label() {
	write_tag();

	return _tag.start("label");
}

HTMLTag *HTMLBuilder::legend() {
	write_tag();

	return _tag.start("legend");
}

HTMLTag *HTMLBuilder::li() {
	write_tag();

	return _tag.start("li");
}

HTMLTag *HTMLBuilder::link() {
	write_tag();

	return _tag.start("link");
}

HTMLTag *HTMLBuilder::main() {
	write_tag();

	return _tag.start("main");
}

HTMLTag *HTMLBuilder::map() {
	write_tag();

	return _tag.start("map");
}
HTMLTag *HTMLBuilder::mark() {
	write_tag();

	return _tag.start("mark");
}

HTMLTag *HTMLBuilder::meta() {
	write_tag();

	return _tag.start("meta");
}

HTMLTag *HTMLBuilder::meter() {
	write_tag();

	return _tag.start("meter");
}

HTMLTag *HTMLBuilder::nav() {
	write_tag();

	return _tag.start("nav");
}

HTMLTag *HTMLBuilder::noframes() { // Not supported in HTML5.
	write_tag();

	return _tag.start("noframes");
}

HTMLTag *HTMLBuilder::noscript() {
	write_tag();

	return _tag.start("noscript");
}

HTMLTag *HTMLBuilder::objectt() {
	write_tag();

	return _tag.start("object");
}

HTMLTag *HTMLBuilder::ol() {
	write_tag();

	return _tag.start("ol");
}

HTMLTag *HTMLBuilder::optgroup() {
	write_tag();

	return _tag.start("optgroup");
}

HTMLTag *HTMLBuilder::option() {
	write_tag();

	return _tag.start("option");
}

HTMLTag *HTMLBuilder::output() {
	write_tag();

	return _tag.start("output");
}

HTMLTag *HTMLBuilder::p() {
	write_tag();

	return _tag.start("p");
}

HTMLTag *HTMLBuilder::param() {
	write_tag();

	return _tag.start("param");
}

HTMLTag *HTMLBuilder::picture() {
	write_tag();

	return _tag.start("picture");
}

HTMLTag *HTMLBuilder::pre() {
	write_tag();

	return _tag.start("pre");
}

HTMLTag *HTMLBuilder::progress() {
	write_tag();

	return _tag.start("progress");
}

HTMLTag *HTMLBuilder::q() {
	write_tag();

	return _tag.start("q");
}

HTMLTag *HTMLBuilder::rp() {
	write_tag();

	return _tag.start("rp");
}

HTMLTag *HTMLBuilder::rt() {
	write_tag();

	return _tag.start("rt");
}

HTMLTag *HTMLBuilder::ruby() {
	write_tag();

	return _tag.start("ruby");
}

HTMLTag *HTMLBuilder::s() {
	write_tag();

	return _tag.start("s");
}

HTMLTag *HTMLBuilder::samp() {
	write_tag();

	return _tag.start("samp");
}

HTMLTag *HTMLBuilder::script() {
	write_tag();

	return _tag.start("script");
}

HTMLTag *HTMLBuilder::section() {
	write_tag();

	return _tag.start("section");
}

HTMLTag *HTMLBuilder::select() {
	write_tag();

	return _tag.start("select");
}

HTMLTag *HTMLBuilder::small() {
	write_tag();

	return _tag.start("small");
}

HTMLTag *HTMLBuilder::source() {
	write_tag();

	return _tag.start("source");
}

HTMLTag *HTMLBuilder::span() {
	write_tag();

	return _tag.start("span");
}

HTMLTag *HTMLBuilder::strike() { // Not supported in HTML5
	write_tag();

	return _tag.start("strike");
}

HTMLTag *HTMLBuilder::strong() {
	write_tag();

	return _tag.start("strong");
}

HTMLTag *HTMLBuilder::style() {
	write_tag();

	return _tag.start("style");
}

HTMLTag *HTMLBuilder::sub() {
	write_tag();

	return _tag.start("sub");
}

HTMLTag *HTMLBuilder::summary() {
	write_tag();

	return _tag.start("summary");
}

HTMLTag *HTMLBuilder::sup() {
	write_tag();

	return _tag.start("sup");
}

HTMLTag *HTMLBuilder::svg() {
	write_tag();

	return _tag.start("svg");
}

HTMLTag *HTMLBuilder::table() {
	write_tag();

	return _tag.start("table");
}

HTMLTag *HTMLBuilder::tbody() {
	write_tag();

	return _tag.start("tbody");
}

HTMLTag *HTMLBuilder::td() {
	write_tag();

	return _tag.start("td");
}

HTMLTag *HTMLBuilder::templateh() {
	write_tag();

	return _tag.start("template");
}

HTMLTag *HTMLBuilder::textarea() {
	write_tag();

	return _tag.start("textarea");
}

HTMLTag *HTMLBuilder::tfoot() {
	write_tag();

	return _tag.start("tfoot");
}

HTMLTag *HTMLBuilder::th() {
	write_tag();

	return _tag.start("th");
}

HTMLTag *HTMLBuilder::thead() {
	write_tag();

	return _tag.start("thead");
}

HTMLTag *HTMLBuilder::time() {
	write_tag();

	return _tag.start("time");
}

HTMLTag *HTMLBuilder::title() {
	write_tag();

	return _tag.start("title");
}

HTMLTag *HTMLBuilder::tr() {
	write_tag();

	return _tag.start("tr");
}

HTMLTag *HTMLBuilder::track() {
	write_tag();

	return _tag.start("track");
}

HTMLTag *HTMLBuilder::tt() { // Not supported in HTML5.
	write_tag();

	return _tag.start("tt");
}

HTMLTag *HTMLBuilder::u() {
	write_tag();

	return _tag.start("u");
}

HTMLTag *HTMLBuilder::ul() {
	write_tag();

	return _tag.start("ul");
}

HTMLTag *HTMLBuilder::var() {
	write_tag();

	return _tag.start("var");
}

HTMLTag *HTMLBuilder::video() {
	write_tag();

	return _tag.start("video");
}

HTMLTag *HTMLBuilder::wbr() {
	write_tag();

	return _tag.start("wbr");
}

HTMLBuilder *HTMLBuilder::a(const String &href, const String &cls, const String &id) {
	HTMLTag *t = a();

	t->href(href);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::fa(const String &href, const String &body, const String &cls, const String &id) {
	a(href, cls, id);
	w(body);
	ca();

	return this;
}

HTMLBuilder *HTMLBuilder::div(const String &cls, const String &id) {
	HTMLTag *t = div();

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::fdiv(const String &body, const String &cls, const String &id) {
	div(cls, id);
	w(body);
	cdiv();

	return this;
}

HTMLBuilder *HTMLBuilder::textarea(const String &name, const String &cls, const String &id) {
	HTMLTag *t = textarea();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}
HTMLBuilder *HTMLBuilder::ftextarea(const String &name, const String &body, const String &cls, const String &id) {
	textarea(name, cls, id);
	w(body);
	ctextarea();

	return this;
}

HTMLBuilder *HTMLBuilder::select(const String &name, const String &cls, const String &id) {
	HTMLTag *t = select();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLTag *HTMLBuilder::option(const String &value) {
	HTMLTag *t = option();

	t->value(value);

	return t;
}
HTMLBuilder *HTMLBuilder::foption(const String &value, const String &body, const bool selected) {
	option(value)->selected(selected);
	w(body);
	coption();

	return this;
}

// Closing _tags

HTMLBuilder *HTMLBuilder::ca() {
	write_tag();
	result += "</a>";

	return this;
}

HTMLBuilder *HTMLBuilder::cabbr() {
	write_tag();
	result += "</abbr>";

	return this;
}

HTMLBuilder *HTMLBuilder::cacronym() {
	write_tag();
	result += "</acronym>";

	return this;
}

HTMLBuilder *HTMLBuilder::caddress() {
	write_tag();
	result += "</address>";

	return this;
}

HTMLBuilder *HTMLBuilder::capplet() {
	write_tag();
	result += "</applet>";

	return this;
}

HTMLBuilder *HTMLBuilder::carea() {
	write_tag();
	result += "</area>";

	return this;
}

HTMLBuilder *HTMLBuilder::carticle() {
	write_tag();
	result += "</article>";

	return this;
}

HTMLBuilder *HTMLBuilder::caside() {
	write_tag();
	result += "</aside>";

	return this;
}

HTMLBuilder *HTMLBuilder::caudio() {
	write_tag();
	result += "</audio>";

	return this;
}

HTMLBuilder *HTMLBuilder::cb() {
	write_tag();
	result += "</b>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbasefont() {
	write_tag();
	result += "</basefont>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbdi() {
	write_tag();
	result += "</bdi>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbdo() {
	write_tag();
	result += "</bdo>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbig() {
	write_tag();
	result += "</big>";

	return this;
}

HTMLBuilder *HTMLBuilder::cblockquote() {
	write_tag();
	result += "</blockquote>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbody() {
	write_tag();
	result += "</body>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbutton() {
	write_tag();
	result += "</button>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccanvas() {
	write_tag();
	result += "</canvas>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccaption() {
	write_tag();
	result += "</caption>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccenter() {
	write_tag();
	result += "</center>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccite() {
	write_tag();
	result += "</cite>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccode() {
	write_tag();
	result += "</code>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccol() {
	write_tag();
	result += "</col>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccolgroup() {
	write_tag();
	result += "</colgroup>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdata() {
	write_tag();
	result += "</data>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdatalist() {
	write_tag();
	result += "</datalist>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdd() {
	write_tag();
	result += "</dd>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdel() {
	write_tag();
	result += "</del>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdetails() {
	write_tag();
	result += "</details>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdfn() {
	write_tag();
	result += "</dfn>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdialog() {
	write_tag();
	result += "</dialog>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdir() {
	write_tag();
	result += "</dir>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdiv() {
	write_tag();
	result += "</div>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdl() {
	write_tag();
	result += "</dl>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdt() {
	write_tag();
	result += "</dt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cem() {
	write_tag();
	result += "</em>";

	return this;
}

HTMLBuilder *HTMLBuilder::cembed() {
	write_tag();
	result += "</embed>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfieldset() {
	write_tag();
	result += "</fieldset>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfigcaption() {
	write_tag();
	result += "</figcaption>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfigure() {
	write_tag();
	result += "</figure>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfont() {
	write_tag();
	result += "</font>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfooter() {
	write_tag();
	result += "</footer>";

	return this;
}

HTMLBuilder *HTMLBuilder::cform() {
	write_tag();
	result += "</form>";

	return this;
}

HTMLBuilder *HTMLBuilder::cframe() {
	write_tag();
	result += "</frame>";

	return this;
}

HTMLBuilder *HTMLBuilder::cframeset() {
	write_tag();
	result += "</frameset>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch1() {
	write_tag();
	result += "</h1>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch2() {
	write_tag();
	result += "</h2>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch3() {
	write_tag();
	result += "</h3>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch4() {
	write_tag();
	result += "</h4>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch5() {
	write_tag();
	result += "</h5>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch6() {
	write_tag();
	result += "</h6>";

	return this;
}

HTMLBuilder *HTMLBuilder::chead() {
	write_tag();
	result += "</head>";

	return this;
}

HTMLBuilder *HTMLBuilder::cheader() {
	write_tag();
	result += "</header>";

	return this;
}

HTMLBuilder *HTMLBuilder::chr() {
	write_tag();
	result += "</hr>";

	return this;
}

HTMLBuilder *HTMLBuilder::chtml() {
	write_tag();
	result += "</html>";

	return this;
}

HTMLBuilder *HTMLBuilder::ci() {
	write_tag();
	result += "</i>";

	return this;
}

HTMLBuilder *HTMLBuilder::ciframe() {
	write_tag();
	result += "</iframe>";

	return this;
}

HTMLBuilder *HTMLBuilder::cimg() {
	write_tag();
	result += "</img>";

	return this;
}

HTMLBuilder *HTMLBuilder::cinput() {
	write_tag();
	result += "</input>";

	return this;
}

HTMLBuilder *HTMLBuilder::cins() {
	write_tag();
	result += "</ins>";

	return this;
}

HTMLBuilder *HTMLBuilder::ckbd() {
	write_tag();
	result += "</kbd>";

	return this;
}

HTMLBuilder *HTMLBuilder::clabel() {
	write_tag();
	result += "</label>";

	return this;
}

HTMLBuilder *HTMLBuilder::clegend() {
	write_tag();
	result += "</legend>";

	return this;
}

HTMLBuilder *HTMLBuilder::cli() {
	write_tag();
	result += "</li>";

	return this;
}

HTMLBuilder *HTMLBuilder::clink() {
	write_tag();
	result += "</link>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmain() {
	write_tag();
	result += "</main>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmap() {
	write_tag();
	result += "</map>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmark() {
	write_tag();
	result += "</mark>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmeta() {
	write_tag();
	result += "</meta>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmeter() {
	write_tag();
	result += "</meter>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnav() {
	write_tag();
	result += "</nav>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnoframes() {
	write_tag();
	result += "</noframes>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnoscript() {
	write_tag();
	result += "</noscript>";

	return this;
}

HTMLBuilder *HTMLBuilder::cobjectt() {
	write_tag();
	result += "</object>";

	return this;
}

HTMLBuilder *HTMLBuilder::c_ol() {
	write_tag();
	result += "</ol>";

	return this;
}

HTMLBuilder *HTMLBuilder::coptgroup() {
	write_tag();
	result += "</optgroup>";

	return this;
}

HTMLBuilder *HTMLBuilder::coption() {
	write_tag();
	result += "</option>";

	return this;
}

HTMLBuilder *HTMLBuilder::coutput() {
	write_tag();
	result += "</output>";

	return this;
}

HTMLBuilder *HTMLBuilder::cp() {
	write_tag();
	result += "</p>";

	return this;
}

HTMLBuilder *HTMLBuilder::cparam() {
	write_tag();
	result += "</param>";

	return this;
}

HTMLBuilder *HTMLBuilder::cpicture() {
	write_tag();
	result += "</picture>";

	return this;
}

HTMLBuilder *HTMLBuilder::cpre() {
	write_tag();
	result += "</pre>";

	return this;
}

HTMLBuilder *HTMLBuilder::cprogress() {
	write_tag();
	result += "</progress>";

	return this;
}

HTMLBuilder *HTMLBuilder::cq() {
	write_tag();
	result += "</a>";

	return this;
}

HTMLBuilder *HTMLBuilder::crp() {
	write_tag();
	result += "</rp>";

	return this;
}

HTMLBuilder *HTMLBuilder::crt() {
	write_tag();
	result += "</rt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cruby() {
	write_tag();
	result += "</ruby>";

	return this;
}

HTMLBuilder *HTMLBuilder::cs() {
	write_tag();
	result += "</s>";

	return this;
}

HTMLBuilder *HTMLBuilder::csamp() {
	write_tag();
	result += "</samp>";

	return this;
}

HTMLBuilder *HTMLBuilder::cscript() {
	write_tag();
	result += "</script>";

	return this;
}

HTMLBuilder *HTMLBuilder::csection() {
	write_tag();
	result += "</section>";

	return this;
}

HTMLBuilder *HTMLBuilder::cselect() {
	write_tag();
	result += "</select>";

	return this;
}

HTMLBuilder *HTMLBuilder::csmall() {
	write_tag();
	result += "</small>";

	return this;
}

HTMLBuilder *HTMLBuilder::csource() {
	write_tag();
	result += "</source>";

	return this;
}

HTMLBuilder *HTMLBuilder::cspan() {
	write_tag();
	result += "</span>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstrike() {
	write_tag();
	result += "</strike>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstrong() {
	write_tag();
	result += "</strong>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstyle() {
	write_tag();
	result += "</style>";

	return this;
}

HTMLBuilder *HTMLBuilder::csub() {
	write_tag();
	result += "</sub>";

	return this;
}

HTMLBuilder *HTMLBuilder::csummary() {
	write_tag();
	result += "</summary>";

	return this;
}

HTMLBuilder *HTMLBuilder::csup() {
	write_tag();
	result += "</sup>";

	return this;
}

HTMLBuilder *HTMLBuilder::csvg() {
	write_tag();
	result += "</svg>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctable() {
	write_tag();
	result += "</table>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctbody() {
	write_tag();
	result += "</tbody>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctd() {
	write_tag();
	result += "</td>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctemplateh() {
	write_tag();
	result += "</templateh>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctextarea() {
	write_tag();
	result += "</textarea>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctfoot() {
	write_tag();
	result += "</tfoot>";

	return this;
}

HTMLBuilder *HTMLBuilder::cth() {
	write_tag();
	result += "</th>";

	return this;
}

HTMLBuilder *HTMLBuilder::cthead() {
	write_tag();
	result += "</thead>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctime() {
	write_tag();
	result += "</time>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctitle() {
	write_tag();
	result += "</title>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctr() {
	write_tag();
	result += "</tr>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctrack() {
	write_tag();
	result += "</track>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctt() {
	write_tag();
	result += "</tt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cu() {
	write_tag();
	result += "</u>";

	return this;
}

HTMLBuilder *HTMLBuilder::cul() {
	write_tag();
	result += "</ul>";

	return this;
}

HTMLBuilder *HTMLBuilder::cvar() {
	write_tag();
	result += "</var>";

	return this;
}

HTMLBuilder *HTMLBuilder::cvideo() {
	write_tag();
	result += "</video>";

	return this;
}

HTMLBuilder *HTMLBuilder::cwbr() {
	write_tag();
	result += "</wbr>";

	return this;
}

HTMLTag *HTMLBuilder::form_get() {
	write_tag();

	return _tag.start("form")->method_get();
}
HTMLTag *HTMLBuilder::form_post() {
	write_tag();

	return _tag.start("form")->method_post();
}
HTMLBuilder *HTMLBuilder::form_get(const String &action, const String &cls, const String &id) {
	HTMLTag *t = form_get();

	t->fora(action);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}
HTMLBuilder *HTMLBuilder::form_post(const String &action, const String &cls, const String &id) {
	HTMLTag *t = form_post();

	t->action(action);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::form_postr(const String &action, Ref<WebServerRequest> request, const String &cls, const String &id) {
	form_post(action, cls, id);
	csrf_tokenr(request);

	return this;
}

HTMLTag *HTMLBuilder::input_button() {
	write_tag();

	return _tag.start("input")->itbutton();
}

HTMLTag *HTMLBuilder::input_checkbox() {
	write_tag();

	return _tag.start("input")->itcheckbox();
}

HTMLTag *HTMLBuilder::input_color() {
	write_tag();

	return _tag.start("input")->itcolor();
}

HTMLTag *HTMLBuilder::input_date() {
	write_tag();

	return _tag.start("input")->itdate();
}

HTMLTag *HTMLBuilder::input_datetime_local() {
	write_tag();

	return _tag.start("input")->itdatetime_local();
}

HTMLTag *HTMLBuilder::input_email() {
	write_tag();

	return _tag.start("input")->itemail();
}

HTMLTag *HTMLBuilder::input_file() {
	write_tag();

	return _tag.start("input")->itfile();
}

HTMLTag *HTMLBuilder::input_hidden() {
	write_tag();

	return _tag.start("input")->ithidden();
}

HTMLTag *HTMLBuilder::input_image() {
	write_tag();

	return _tag.start("input")->itimage();
}

HTMLTag *HTMLBuilder::input_month() {
	write_tag();

	return _tag.start("input")->itmonth();
}

HTMLTag *HTMLBuilder::input_number() {
	write_tag();

	return _tag.start("input")->itnumber();
}

HTMLTag *HTMLBuilder::input_password() {
	write_tag();

	return _tag.start("input")->itpassword();
}

HTMLTag *HTMLBuilder::input_radio() {
	write_tag();

	return _tag.start("input")->itradio();
}

HTMLTag *HTMLBuilder::input_range() {
	write_tag();

	return _tag.start("input")->itrange();
}

HTMLTag *HTMLBuilder::input_reset() {
	write_tag();

	return _tag.start("input")->itreset();
}

HTMLTag *HTMLBuilder::input_search() {
	write_tag();

	return _tag.start("input")->itsearch();
}

HTMLTag *HTMLBuilder::input_submit() {
	write_tag();

	return _tag.start("input")->itsubmit();
}

HTMLTag *HTMLBuilder::input_tel() {
	write_tag();

	return _tag.start("input")->ittel();
}

HTMLTag *HTMLBuilder::input_text() {
	write_tag();

	return _tag.start("input")->ittext();
}

HTMLTag *HTMLBuilder::input_time() {
	write_tag();

	return _tag.start("input")->ittime();
}

HTMLTag *HTMLBuilder::input_url() {
	write_tag();

	return _tag.start("input")->iturl();
}

HTMLTag *HTMLBuilder::input_week() {
	write_tag();

	return _tag.start("input")->itweek();
}

HTMLBuilder *HTMLBuilder::label(const String &pfor, const String &plabel, const String &cls, const String &id) {
	HTMLTag *t = label();

	t->fora(pfor);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	w(plabel);

	clabel();

	return this;
}

HTMLBuilder *HTMLBuilder::input_button(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_button();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_checkbox(const String &name, const String &value, const bool checked, const String &cls, const String &id) {
	HTMLTag *t = input_checkbox();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	t->checked(checked);

	return this;
}

HTMLBuilder *HTMLBuilder::input_color(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_color();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_date(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	HTMLTag *t = input_date();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (date_min != "") {
		t->min(date_min);
	}

	if (date_max != "") {
		t->max(date_max);
	}

	if (date_step != "") {
		t->step(date_step);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_datetime_local(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	HTMLTag *t = input_datetime_local();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (date_min != "") {
		t->min(date_min);
	}

	if (date_max != "") {
		t->max(date_max);
	}

	if (date_step != "") {
		t->step(date_step);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_email(const String &name, const String &value, const String &placeholder, const String &cls, const String &id) {
	HTMLTag *t = input_email();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_file(const String &name, const String &accept, const String &cls, const String &id) {
	HTMLTag *t = input_file();

	t->name(name);

	if (accept != "") {
		t->accept(accept);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_image(const String &name, const String &src, const String &alt, const String &cls, const String &id, const int width, const int height) {
	HTMLTag *t = input_image();

	t->name(name);

	if (src != "") {
		t->src(src);
	}

	if (alt != "") {
		t->alt(alt);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (width != 0) {
		t->width(width);
	}

	if (height != 0) {
		t->height(height);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_month(const String &name, const String &cls, const String &id) {
	HTMLTag *t = input_month();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_number(const String &name, const String &vmin, const String &vmax, const String &cls, const String &id) {
	HTMLTag *t = input_number();

	t->name(name);

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_password(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_password();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_radio(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_password();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_range(const String &name, const String &value, const String &vmin, const String &vmax, const String &vstep, const String &cls, const String &id) {
	HTMLTag *t = input_range();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (vstep != "") {
		t->step(vstep);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_reset(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_reset();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_search(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	HTMLTag *t = input_search();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	if (pattern != "") {
		t->pattern(pattern);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_submit(const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_submit();

	t->value(value);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_tel(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	HTMLTag *t = input_tel();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	if (pattern != "") {
		t->pattern(pattern);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_text(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_text();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_time(const String &name, const String &cls, const String &id, const String &vmin, const String &vmax, const String &vstep) {
	HTMLTag *t = input_time();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (vstep != "") {
		t->step(vstep);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_url(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_url();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_week(const String &name, const String &cls, const String &id, const String &vmin, const String &vmax) {
	HTMLTag *t = input_week();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::input_hidden(const String &name, const String &value) {
	HTMLTag *t = input_hidden();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	return this;
}

HTMLBuilder *HTMLBuilder::csrf_token(const String &token) {
	if (token == "") {
		// don't waste html characters if it's an empty string anyway
		return this;
	}

	input_hidden("csrf_token", token);

	return this;
}

HTMLBuilder *HTMLBuilder::csrf_tokenr(Ref<WebServerRequest> request) {
	return csrf_token(request->get_csrf_token());
}

void HTMLBuilder::f() {
	write_tag();
}

HTMLTag *HTMLBuilder::tag(const String &p_tag, const bool p_simple) {
	write_tag();

	return _tag.start(p_tag, p_simple);
}

HTMLBuilder *HTMLBuilder::ctag(const String &p_tag) {
	write_tag();

	result += "</";
	result += p_tag;
	result += ">";

	return this;
}

HTMLBuilder *HTMLBuilder::w(const String &val) {
	write_tag();

	result += val;

	return this;
}

HTMLBuilder *HTMLBuilder::wn(const double val, int p_decimals) {
	write_tag();

	result += String::num(val, p_decimals);

	return this;
}
HTMLBuilder *HTMLBuilder::wns(const double val) {
	write_tag();

	result += String::num_scientific(val);

	return this;
}
HTMLBuilder *HTMLBuilder::wr(const double val, const bool p_trailing) {
	write_tag();

	result += String::num(val, p_trailing);

	return this;
}
HTMLBuilder *HTMLBuilder::wi(const int64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_int64(val, base, capitalize_hex);

	return this;
}
HTMLBuilder *HTMLBuilder::wui(const uint64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_uint64(val, base, capitalize_hex);

	return this;
}

HTMLBuilder *HTMLBuilder::wbn(const bool val) {
	write_tag();

	result += String::bool_num(val);

	return this;
}
HTMLBuilder *HTMLBuilder::wbs(const bool val) {
	write_tag();

	result += String::bool_str(val);

	return this;
}

HTMLBuilder *HTMLBuilder::we(const String &val) {
	write_tag();

	result += val.http_escape();

	return this;
}

HTMLBuilder *HTMLBuilder::write_tag() {
	if (_tag.has_data()) {
		_tag.close();
		result += _tag.get_result();
		_tag.reset();
	}

	return this;
}

HTMLBuilder::HTMLBuilder() {
	_tag.owner = this;
}

HTMLBuilder::~HTMLBuilder() {
}
