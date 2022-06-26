#ifndef COOKIE_H
#define COOKIE_H

#include "core/string.h"

class WebServerCookie {
public:
	//todo date
	String domain;
	String path;
	String key;
	String value;
	bool http_only;
	bool secure;

	WebServerCookie();
	WebServerCookie(const String &p_key, const String &p_value);
	~WebServerCookie();
};

#endif
