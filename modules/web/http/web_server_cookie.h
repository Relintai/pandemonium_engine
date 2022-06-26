#ifndef WEB_SERVER_COOKIE_H
#define WEB_SERVER_COOKIE_H

#include "core/ustring.h"

#include "core/reference.h"

class WebServerCookie : public Reference {
	GDCLASS(WebServerCookie, Reference);

public:
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

	void set_data(const String &p_key, const String &p_value);

	WebServerCookie();
	~WebServerCookie();

	//todo date
	String domain;
	String path;
	String key;
	String value;
	bool http_only;
	bool secure;

protected:
	static void _bind_methods();
};

#endif
