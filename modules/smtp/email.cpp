/*************************************************************************/
/*  email.cpp                                                            */
/*************************************************************************/
/*                         This file is part of:                         */
/*                   PANDEMONIUM ENGINE'S SMTP MODULE                    */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2021-2024 Nicolò Santilio                               */
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

#include "email.h"

String EMail::get_sender_address() const {
	return sender_address;
}
void EMail::set_sender_address(const String &p_value) {
	sender_address = p_value;
}

String EMail::get_sender_personal() const {
	return sender_personal;
}
void EMail::set_sender_personal(const String &p_value) {
	sender_personal = p_value;
}

void EMail::set_sender(String p_address, String p_personal) {
	sender_address = p_address;
	sender_personal = p_personal;
}

String EMail::get_recipient_address(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, to.size(), "");

	return to[p_index].address;
}
void EMail::set_recipient_address(const int p_index, const String &p_value) {
	ERR_FAIL_INDEX(p_index, to.size());

	to.write[p_index].address = p_value;
}

String EMail::get_recipient_personal(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, to.size(), "");

	return to[p_index].personal;
}
void EMail::set_recipient_personal(const int p_index, const String &p_value) {
	ERR_FAIL_INDEX(p_index, to.size());

	to.write[p_index].personal = p_value;
}

void EMail::add_recipient(String p_address, String p_personal) {
	Address a;

	a.address = p_address;
	a.personal = p_personal;

	to.push_back(a);
}

int EMail::get_recipient_count() const {
	return to.size();
}

void EMail::remove_recipient(const int p_index) {
	ERR_FAIL_INDEX(p_index, to.size());

	to.remove(p_index);
}

Array EMail::get_recipients() {
	Array r;
	for (int i = 0; i < to.size(); i++) {
		const Address &a = to[i];

		Array ra;
		ra.push_back(a.address);
		ra.push_back(a.personal);
	}
	return r;
}
void EMail::set_recipients(const Array &p_recipients) {
	to.clear();

	for (int i = 0; i < p_recipients.size(); i++) {
		Variant v = p_recipients[i];

		ERR_CONTINUE(v.get_type() != Variant::ARRAY);

		Array varr = v;

		ERR_CONTINUE(varr.size() != 2);

		Address a;
		a.address = varr[0];
		a.personal = varr[1];

		to.push_back(a);
	}
}

String EMail::get_cc_address(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, cc.size(), "");

	return cc[p_index].address;
}
void EMail::set_cc_address(const int p_index, const String &p_value) {
	ERR_FAIL_INDEX(p_index, cc.size());

	cc.write[p_index].address = p_value;
}

String EMail::get_cc_personal(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, cc.size(), "");

	return cc[p_index].personal;
}
void EMail::set_cc_personal(const int p_index, const String &p_value) {
	ERR_FAIL_INDEX(p_index, cc.size());

	cc.write[p_index].personal = p_value;
}

void EMail::add_cc(String p_address, String p_personal) {
	Address a;

	a.address = p_address;
	a.personal = p_personal;

	cc.push_back(a);
}

int EMail::get_cc_count() const {
	return cc.size();
}

void EMail::remove_cc(const int p_index) {
	ERR_FAIL_INDEX(p_index, cc.size());

	cc.remove(p_index);
}

Array EMail::get_ccs() {
	Array r;
	for (int i = 0; i < cc.size(); i++) {
		const Address &a = cc[i];

		Array ra;
		ra.push_back(a.address);
		ra.push_back(a.personal);
	}
	return r;
}
void EMail::set_ccs(const Array &p_recipients) {
	cc.clear();

	for (int i = 0; i < p_recipients.size(); i++) {
		Variant v = p_recipients[i];

		ERR_CONTINUE(v.get_type() != Variant::ARRAY);

		Array varr = v;

		ERR_CONTINUE(varr.size() != 2);

		Address a;
		a.address = varr[0];
		a.personal = varr[1];

		cc.push_back(a);
	}
}

String EMail::get_subject() const {
	return subject;
}

void EMail::set_subject(String p_subject) {
	subject = p_subject;
}

String EMail::get_body() const {
	return body;
}

void EMail::set_body(String p_body) {
	body = p_body;
}

String EMail::get_to_data_string() {
	String ret;

	for (int i = 0; i < to.size(); ++i) {
		Address t = to[i];

		if (ret.size() != 0) {
			ret += ", ";
		}

		ret += t.get_address_data_string();
	}

	return ret;
}

String EMail::get_cc_data_string() {
	String ret;

	for (int i = 0; i < to.size(); ++i) {
		Address t = to[i];

		if (ret.size() != 0) {
			ret += ", ";
		}

		ret += t.get_address_data_string();
	}

	return ret;
}

String EMail::get_email_data_string(String email_default_sender_name, String email_default_sender_email) {
	String from_address;

	if (sender_address.size() > 0) {
		if (sender_personal.size() > 0) {
			from_address = sender_personal + " ";
		} else {
			from_address = sender_address.split("@")[0] + " ";
		}

		from_address += "<" + sender_address + ">";
	} else {
		if (email_default_sender_name.size() > 0) {
			from_address = email_default_sender_name + " ";
		} else {
			from_address = email_default_sender_email.split("@")[0] + " ";
		}

		from_address += "<" + email_default_sender_email + ">";
	}

	String data = "From: " + from_address + "\n";

	String to_data_string = get_to_data_string();
	if (!to_data_string.empty()) {
		data += "To: " + to_data_string + "\n";
	}

	String cc_data_string = get_cc_data_string();
	if (!cc_data_string.empty()) {
		data += "Cc: " + cc_data_string + "\n";
	}

	data += "Subject: " + subject + "\n\n";
	data += body + "\n";

	return data;
}
String EMail::_to_string() {
	String from_address;

	if (sender_personal.size() > 0) {
		from_address = sender_personal + " ";
	} else {
		from_address = sender_address.split("@")[0] + " ";
	}

	from_address += "<" + sender_address + ">";

	String data = "From: " + from_address + "\n";

	String to_data_string = get_to_data_string();
	if (!to_data_string.empty()) {
		data += "To: " + to_data_string + "\n";
	}

	String cc_data_string = get_cc_data_string();
	if (!cc_data_string.empty()) {
		data += "Cc: " + cc_data_string + "\n";
	}

	data += "Subject: " + subject + "\n\n";
	data += body + "\n";

	return data;
}

EMail::EMail() {
}
EMail::~EMail() {
}

void EMail::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_sender_address"), &EMail::get_sender_address);
	ClassDB::bind_method(D_METHOD("set_sender_address", "val"), &EMail::set_sender_address);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "sender_address"), "set_sender_address", "get_sender_address");

	ClassDB::bind_method(D_METHOD("get_sender_personal"), &EMail::get_sender_personal);
	ClassDB::bind_method(D_METHOD("set_sender_personal", "val"), &EMail::set_sender_personal);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "sender_personal"), "set_sender_personal", "get_sender_personal");

	ClassDB::bind_method(D_METHOD("set_sender", "address", "personal"), &EMail::set_sender);

	// Recipients
	ClassDB::bind_method(D_METHOD("get_recipient_address", "index"), &EMail::get_recipient_address);
	ClassDB::bind_method(D_METHOD("set_recipient_address", "index", "val"), &EMail::set_recipient_address);

	ClassDB::bind_method(D_METHOD("get_recipient_personal", "index"), &EMail::get_recipient_personal);
	ClassDB::bind_method(D_METHOD("set_recipient_personal", "index", "val"), &EMail::set_recipient_personal);

	ClassDB::bind_method(D_METHOD("add_recipient", "address", "personal"), &EMail::add_recipient);

	ClassDB::bind_method(D_METHOD("get_recipient_count"), &EMail::get_recipient_count);

	ClassDB::bind_method(D_METHOD("remove_recipient", "index"), &EMail::remove_recipient);

	ClassDB::bind_method(D_METHOD("get_recipients"), &EMail::get_recipients);
	ClassDB::bind_method(D_METHOD("set_recipients", "recipients"), &EMail::set_recipients);

	// CC
	ClassDB::bind_method(D_METHOD("get_cc_address", "index"), &EMail::get_cc_address);
	ClassDB::bind_method(D_METHOD("set_cc_address", "index", "val"), &EMail::set_cc_address);

	ClassDB::bind_method(D_METHOD("get_cc_personal", "index"), &EMail::get_cc_personal);
	ClassDB::bind_method(D_METHOD("set_cc_personal", "index", "val"), &EMail::set_cc_personal);

	ClassDB::bind_method(D_METHOD("add_cc", "address", "personal"), &EMail::add_cc);

	ClassDB::bind_method(D_METHOD("get_cc_count"), &EMail::get_cc_count);

	ClassDB::bind_method(D_METHOD("remove_cc", "index"), &EMail::remove_cc);

	ClassDB::bind_method(D_METHOD("get_ccs"), &EMail::get_ccs);
	ClassDB::bind_method(D_METHOD("set_ccs", "ccs"), &EMail::set_ccs);

	// Other
	ClassDB::bind_method(D_METHOD("get_subject"), &EMail::get_subject);
	ClassDB::bind_method(D_METHOD("set_subject", "val"), &EMail::set_subject);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "subject"), "set_subject", "get_subject");

	ClassDB::bind_method(D_METHOD("get_body"), &EMail::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "val"), &EMail::set_body);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "body"), "set_body", "get_body");

	// Helpers
	ClassDB::bind_method(D_METHOD("get_to_data_string"), &EMail::get_to_data_string);
	ClassDB::bind_method(D_METHOD("get_cc_data_string"), &EMail::get_cc_data_string);

	ClassDB::bind_method(D_METHOD("get_email_data_string", "email_default_sender_name", "email_default_sender_email"), &EMail::get_email_data_string);
}
