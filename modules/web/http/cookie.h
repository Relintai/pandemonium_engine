#ifndef COOKIE_H
#define COOKIE_H

#include "core/string.h"

class Cookie {
public:
	//todo date
	String domain;
	String path;
	String key;
	String value;
	bool http_only;
	bool secure;

	Cookie();
	Cookie(const String &p_key, const String &p_value);
	~Cookie();
};

#endif