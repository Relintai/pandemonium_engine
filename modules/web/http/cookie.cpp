
#include "cookie.h"

Cookie::Cookie(const String &p_key, const String &p_value) {
	http_only = true;
	secure = false;

	key = p_key;
	value = p_value;
}

Cookie::Cookie() {
	http_only = true;
	secure = false;
}

Cookie::~Cookie() {
}