#ifndef EMAIL_H
#define EMAIL_H

/*************************************************************************/
/*  email.h                                                              */
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

#include "core/containers/vector.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"

class EMail : public Reference {
	GDCLASS(EMail, Reference);

public:
	struct Address {
		String address;
		String personal;

		String get_address_data_string() {
			if (personal.size() == 0) {
				return address;
			}

			return personal + " <" + address + ">";
		}

		String get_address_data_list_string() {
			if (personal.size() > 0) {
				return personal + ":" + address;
			}

			return address;
		}
	};

	// Sender
	String get_sender_address() const;
	void set_sender_address(const String &p_value);

	String get_sender_personal() const;
	void set_sender_personal(const String &p_value);

	void set_sender(String p_address, String p_personal);

	// Recipients
	String get_recipient_address(const int p_index) const;
	void set_recipient_address(const int p_index, const String &p_value);

	String get_recipient_personal(const int p_index) const;
	void set_recipient_personal(const int p_index, const String &p_value);

	void add_recipient(String p_address, String p_personal);

	int get_recipient_count() const;

	void remove_recipient(const int p_index);

	Array get_recipients();
	void set_recipients(const Array &p_recipients);

	// CC
	String get_cc_address(const int p_index) const;
	void set_cc_address(const int p_index, const String &p_value);

	String get_cc_personal(const int p_index) const;
	void set_cc_personal(const int p_index, const String &p_value);

	void add_cc(String p_address, String p_personal);

	int get_cc_count() const;

	void remove_cc(const int p_index);

	Array get_ccs();
	void set_ccs(const Array &p_ccs);

	// Other
	String get_subject() const;
	void set_subject(String p_subject);

	String get_body() const;
	void set_body(String p_body);

	// Helpers
	String get_to_data_string();
	String get_cc_data_string();

	String get_email_data_string(String email_default_sender_name, String email_default_sender_email);

	String _to_string();

	EMail();
	~EMail();

protected:
	static void _bind_methods();

	String sender_address;
	String sender_personal;

	Vector<Address> to;
	Vector<Address> cc;

	String subject;
	String body;
};

#endif
