#ifndef WEB_SERVER_COOKIE_H
#define WEB_SERVER_COOKIE_H

/*************************************************************************/
/*  web_server_cookie.h                                                  */
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

#include "core/os/os.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"

class WebServerCookie : public Reference {
	GDCLASS(WebServerCookie, Reference);

public:
	enum SameSiteValues {
		SAME_SITE_UNSET = 0,
		SAME_SITE_NONE,
		SAME_SITE_LAX,
		SAME_SITE_STRICT,
	};

	String get_domain();
	void set_domain(const String &val);

	String get_path();
	void set_path(const String &val);

	String get_key();
	void set_key(const String &val);

	String get_value();
	void set_value(const String &val);

	bool get_http_only();
	void set_http_only(const bool val);

	bool get_secure();
	void set_secure(const bool val);

	bool get_delete();
	void set_delete(const bool val);

	SameSiteValues get_same_site();
	void set_same_site(const SameSiteValues val);

	bool get_use_max_age();
	void set_use_max_age(const bool val);

	int get_max_age();
	void set_max_age(const int val);

	bool get_use_expiry_date();
	void set_use_expiry_date(const bool val);

	uint64_t get_expiry_date_unix_time();
	void set_expiry_date_unix_time(const uint64_t unix_time);

	OS::DateTime get_expiry_date();
	void set_expiry_date(OS::DateTime date_time);
	void set_expiry_date(OS::Date date, OS::Time time);

	Dictionary get_expiry_date_dict();
	void set_expiry_date_dict(const Dictionary &date, const Dictionary &time);
	void set_expiry_date_dt_dict(const Dictionary &date_time);

	void set_data(const String &p_key, const String &p_value);

	String get_response_header_string();

	WebServerCookie();
	~WebServerCookie();

protected:
	static void _bind_methods();

	bool _use_max_age;
	int _max_age;
	bool _use_expiry_date;
	OS::DateTime _expiry_date;
	String _domain;
	String _path;
	String _key;
	String _value;
	bool _http_only;
	bool _secure;
	bool _delete;
	SameSiteValues _same_site;
};

VARIANT_ENUM_CAST(WebServerCookie::SameSiteValues);

#endif
