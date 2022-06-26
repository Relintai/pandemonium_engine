
#include "cookie.h"

WebServerCookie::WebServerCookie(const String &p_key, const String &p_value) {
	http_only = true;
	secure = false;

	key = p_key;
	value = p_value;
}

WebServerCookie::WebServerCookie() {
	http_only = true;
	secure = false;
}

WebServerCookie::~WebServerCookie() {
}
