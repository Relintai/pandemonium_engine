
#include "web_server_cookie.h"

String WebServerCookie::get_domain() {
	return _domain;
}
void WebServerCookie::set_domain(const String &val) {
	_domain = val;
}

String WebServerCookie::get_path() {
	return _path;
}
void WebServerCookie::set_path(const String &val) {
	_path = val;
}

String WebServerCookie::get_key() {
	return _key;
}
void WebServerCookie::set_key(const String &val) {
	_key = val;
}

String WebServerCookie::get_value() {
	return _value;
}
void WebServerCookie::set_value(const String &val) {
	_value = val;
}

bool WebServerCookie::get_http_only() {
	return _http_only;
}
void WebServerCookie::set_http_only(const bool val) {
	_http_only = val;
}

bool WebServerCookie::get_secure() {
	return _secure;
}
void WebServerCookie::set_secure(const bool val) {
	_secure = val;
}

bool WebServerCookie::get_delete() {
	return _delete;
}
void WebServerCookie::set_delete(const bool val) {
	_delete = val;
}

bool WebServerCookie::get_should_expire() {
	return _should_expire;
}
void WebServerCookie::set_should_expire(const bool val) {
	_should_expire = val;
}

uint64_t WebServerCookie::get_expiry_date_unix_time() {
	return OS::get_singleton()->get_unix_time_from_datetime(_expiry_date);
}
void WebServerCookie::set_expiry_date_unix_time(const uint64_t unix_time) {
	_expiry_date = OS::get_singleton()->get_datetime_from_unix_time(unix_time);

	_should_expire = true;
}

OS::DateTime WebServerCookie::get_expiry_date() {
	return _expiry_date;
}
void WebServerCookie::set_expiry_date(OS::DateTime date_time) {
	_expiry_date = date_time;

	_should_expire = true;
}
void WebServerCookie::set_expiry_date(OS::Date date, OS::Time time) {
	_expiry_date.date = date;
	_expiry_date.time = time;

	_should_expire = true;
}

Dictionary WebServerCookie::get_expiry_date_dict() {
	Dictionary date_time;
	date_time["year"] = _expiry_date.date.year;
	date_time["month"] = _expiry_date.date.month;
	date_time["day"] = _expiry_date.date.day;
	date_time["weekday"] = _expiry_date.date.weekday;
	date_time["dst"] = _expiry_date.date.dst;
	date_time["hour"] = _expiry_date.time.hour;
	date_time["minute"] = _expiry_date.time.min;
	date_time["second"] = _expiry_date.time.sec;

	return date_time;
}

void WebServerCookie::set_expiry_date_dict(const Dictionary &date, const Dictionary &time) {
	int month = date["month"];
	int weekday = date["weekday"];

	_expiry_date.date.year = date["year"];
	_expiry_date.date.month = static_cast<OS::Month>(month);
	_expiry_date.date.day = date["day"];
	_expiry_date.date.weekday = static_cast<OS::Weekday>(weekday);
	_expiry_date.date.dst = date["dst"];
	_expiry_date.time.hour = time["hour"];
	_expiry_date.time.min = time["minute"];
	_expiry_date.time.sec = time["second"];

	_should_expire = true;
}

void WebServerCookie::set_expiry_date_dt_dict(const Dictionary &date_time) {
	int month = date_time["month"];
	int weekday = date_time["weekday"];

	_expiry_date.date.year = date_time["year"];
	_expiry_date.date.month = static_cast<OS::Month>(month);
	_expiry_date.date.day = date_time["day"];
	_expiry_date.date.weekday = static_cast<OS::Weekday>(weekday);
	_expiry_date.date.dst = date_time["dst"];
	_expiry_date.time.hour = date_time["hour"];
	_expiry_date.time.min = date_time["minute"];
	_expiry_date.time.sec = date_time["second"];

	_should_expire = true;
}

void WebServerCookie::set_data(const String &p_key, const String &p_value) {
	_key = p_key;
	_value = p_value;
}

String WebServerCookie::get_response_header_string() {
	return "";
}

WebServerCookie::WebServerCookie() {
	_should_expire = false;
	_http_only = true;
	_secure = false;
	_delete = false;
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

	ClassDB::bind_method(D_METHOD("get_delete"), &WebServerCookie::get_delete);
	ClassDB::bind_method(D_METHOD("set_delete", "val"), &WebServerCookie::set_delete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "delete"), "set_delete", "get_delete");

	ClassDB::bind_method(D_METHOD("get_should_expire"), &WebServerCookie::get_should_expire);
	ClassDB::bind_method(D_METHOD("set_should_expire", "val"), &WebServerCookie::set_should_expire);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_expire"), "set_should_expire", "get_should_expire");

	ClassDB::bind_method(D_METHOD("get_expiry_date_unix_time"), &WebServerCookie::get_expiry_date_unix_time);
	ClassDB::bind_method(D_METHOD("set_expiry_date_unix_time", "unix_time"), &WebServerCookie::set_expiry_date_unix_time);

	ClassDB::bind_method(D_METHOD("get_expiry_date"), &WebServerCookie::get_expiry_date_dict);
	ClassDB::bind_method(D_METHOD("set_expiry_date", "date", "time"), &WebServerCookie::set_expiry_date_dict);
	ClassDB::bind_method(D_METHOD("set_expiry_date_dt", "date_time"), &WebServerCookie::set_expiry_date_dt_dict);

	ClassDB::bind_method(D_METHOD("set_data", "key", "value"), &WebServerCookie::set_data);
	ClassDB::bind_method(D_METHOD("get_response_header_string"), &WebServerCookie::get_response_header_string);
}
