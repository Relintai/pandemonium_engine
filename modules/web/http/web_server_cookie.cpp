
#include "web_server_cookie.h"

String WebServerCookie::get_domain() {
	return domain;
}
void WebServerCookie::set_domain(const String &val) {
	domain = val;
}

String WebServerCookie::get_path() {
	return path;
}
void WebServerCookie::set_path(const String &val) {
	path = val;
}

String WebServerCookie::get_key() {
	return key;
}
void WebServerCookie::set_key(const String &val) {
	key = val;
}

String WebServerCookie::get_value() {
	return value;
}
void WebServerCookie::set_value(const String &val) {
	value = val;
}

bool WebServerCookie::get_http_only() {
	return http_only;
}
void WebServerCookie::set_http_only(const bool val) {
	http_only = val;
}

bool WebServerCookie::get_secure() {
	return secure;
}
void WebServerCookie::set_secure(const bool val) {
	secure = val;
}

void WebServerCookie::set_data(const String &p_key, const String &p_value) {
	key = p_key;
	value = p_value;
}

WebServerCookie::WebServerCookie() {
	http_only = true;
	secure = false;
}

WebServerCookie::~WebServerCookie() {
}

void WebServerCookie::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_domain"), &WebServerCookie::get_domain);
	ClassDB::bind_method(D_METHOD("set_domain", "val"), &WebServerCookie::set_domain);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "domain"), "set_domain", "get_domain");

	ClassDB::bind_method(D_METHOD("get_path"), &WebServerCookie::get_path);
	ClassDB::bind_method(D_METHOD("set_path", "val"), &WebServerCookie::set_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "path"), "set_path", "get_path");

	ClassDB::bind_method(D_METHOD("get_key"), &WebServerCookie::get_key);
	ClassDB::bind_method(D_METHOD("set_key", "val"), &WebServerCookie::set_key);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "key"), "set_key", "get_key");

	ClassDB::bind_method(D_METHOD("get_value"), &WebServerCookie::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "val"), &WebServerCookie::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_http_only"), &WebServerCookie::get_http_only);
	ClassDB::bind_method(D_METHOD("set_http_only", "val"), &WebServerCookie::set_http_only);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "http_only"), "set_http_only", "get_http_only");

	ClassDB::bind_method(D_METHOD("get_secure"), &WebServerCookie::get_secure);
	ClassDB::bind_method(D_METHOD("set_secure", "val"), &WebServerCookie::set_secure);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "secure"), "set_secure", "get_secure");

	ClassDB::bind_method(D_METHOD("set_data", "key", "value"), &WebServerCookie::set_data);
}
