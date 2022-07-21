#ifndef WEB_SERVER_COOKIE_H
#define WEB_SERVER_COOKIE_H

#include "core/os/os.h"
#include "core/ustring.h"

#include "core/reference.h"

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
