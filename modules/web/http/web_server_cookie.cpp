
#include "web_server_cookie.h"
#include "core/object/class_db.h"
#include "core/object/object.h"

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

WebServerCookie::SameSiteValues WebServerCookie::get_same_site() {
	return _same_site;
}
void WebServerCookie::set_same_site(const WebServerCookie::SameSiteValues val) {
	_same_site = val;
}

bool WebServerCookie::get_use_max_age() {
	return _use_max_age;
}
void WebServerCookie::set_use_max_age(const bool val) {
	_use_max_age = val;
}

int WebServerCookie::get_max_age() {
	return _max_age;
}
void WebServerCookie::set_max_age(const int val) {
	_max_age = val;
}

bool WebServerCookie::get_use_expiry_date() {
	return _use_expiry_date;
}
void WebServerCookie::set_use_expiry_date(const bool val) {
	_use_expiry_date = val;
}

uint64_t WebServerCookie::get_expiry_date_unix_time() {
	return OS::get_singleton()->get_unix_time_from_datetime(_expiry_date);
}
void WebServerCookie::set_expiry_date_unix_time(const uint64_t unix_time) {
	_expiry_date = OS::get_singleton()->get_datetime_from_unix_time(unix_time);

	_use_expiry_date = true;
}

OS::DateTime WebServerCookie::get_expiry_date() {
	return _expiry_date;
}
void WebServerCookie::set_expiry_date(OS::DateTime date_time) {
	_expiry_date = date_time;

	_use_expiry_date = true;
}
void WebServerCookie::set_expiry_date(OS::Date date, OS::Time time) {
	_expiry_date.date = date;
	_expiry_date.time = time;

	_use_expiry_date = true;
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

	_use_expiry_date = true;
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

	_use_expiry_date = true;
}

void WebServerCookie::set_data(const String &p_key, const String &p_value) {
	_key = p_key;
	_value = p_value;
}

String WebServerCookie::get_response_header_string() {
	if (_delete) {
		String l;

		l += "Set-Cookie: ";
		l += _key;
		l += "=; expires=Thu, 01 Jan 1970 00:00:00 GMT";

		if (_domain != "") {
			l += "; Domain=";
			l += _domain;
		}

		if (_path != "") {
			l += "; Path=";
			l += _path;
		}

		l += "\r\n";

		return l;
	}

	String l;

	l += "Set-Cookie: ";
	l += _key;
	l += "=";
	l += _value;

	if (_use_expiry_date) {
		l += "; Expires=";

		switch (_expiry_date.date.weekday) {
			case OS::DAY_SUNDAY:
				l += "Sun, ";
				break;
			case OS::DAY_MONDAY:
				l += "Mon, ";
				break;
			case OS::DAY_TUESDAY:
				l += "Tue, ";
				break;
			case OS::DAY_WEDNESDAY:
				l += "Wed, ";
				break;
			case OS::DAY_THURSDAY:
				l += "Thu, ";
				break;
			case OS::DAY_FRIDAY:
				l += "Fri, ";
				break;
			case OS::DAY_SATURDAY:
				l += "Sun, ";
				break;
			default:
				break;
		}

		l += itos(_expiry_date.date.day).pad_zeros(2);
		l += " ";

		switch (_expiry_date.date.month) {
			case OS::MONTH_JANUARY:
				l += "Jan ";
				break;
			case OS::MONTH_FEBRUARY:
				l += "Feb ";
				break;
			case OS::MONTH_MARCH:
				l += "Mar ";
				break;
			case OS::MONTH_APRIL:
				l += "Apr ";
				break;
			case OS::MONTH_MAY:
				l += "May ";
				break;
			case OS::MONTH_JUNE:
				l += "Jun ";
				break;
			case OS::MONTH_JULY:
				l += "Jul ";
				break;
			case OS::MONTH_AUGUST:
				l += "Aug ";
				break;
			case OS::MONTH_SEPTEMBER:
				l += "Sep ";
				break;
			case OS::MONTH_OCTOBER:
				l += "Oct ";
				break;
			case OS::MONTH_NOVEMBER:
				l += "Nov ";
				break;
			case OS::MONTH_DECEMBER:
				l += "Dec ";
				break;
			default:
				break;
		}

		l += itos(_expiry_date.date.year);
		l += " ";
		l += itos(_expiry_date.time.hour).pad_zeros(2);
		l += ":";
		l += itos(_expiry_date.time.min).pad_zeros(2);
		l += ":";
		l += itos(_expiry_date.time.sec).pad_zeros(2);

		l += " GMT";
	}

	if (_use_max_age) {
		l += "; Max-Age=";
		l += itos(_max_age);
	}

	if (_secure) {
		l += "; Secure";
	}

	if (_http_only) {
		l += "; HttpOnly";
	}

	if (_domain != "") {
		l += "; Domain=";
		l += _domain;
	}

	if (_path != "") {
		l += "; Path=";
		l += _path;
	}

	switch (_same_site) {
		case SAME_SITE_NONE:
			l += "; SameSite=None";
			break;
		case SAME_SITE_LAX:
			l += "; SameSite=Lax";
			break;
		case SAME_SITE_STRICT:
			l += "; SameSite=Strict";
			break;
		case SAME_SITE_UNSET:
		default:
			break;
	}

	l += "\r\n";

	return l;
}

WebServerCookie::WebServerCookie() {
	_use_max_age = false;
	_max_age = 0;
	_use_expiry_date = false;
	_http_only = true;
	_secure = false;
	_delete = false;
	_same_site = SAME_SITE_UNSET;
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

	ClassDB::bind_method(D_METHOD("get_same_site"), &WebServerCookie::get_same_site);
	ClassDB::bind_method(D_METHOD("set_same_site", "val"), &WebServerCookie::set_same_site);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "same_site", PROPERTY_HINT_ENUM, "Unset,None,Lax,Strict"), "set_same_site", "get_same_site");

	ClassDB::bind_method(D_METHOD("get_use_max_age"), &WebServerCookie::get_use_max_age);
	ClassDB::bind_method(D_METHOD("set_use_max_age", "val"), &WebServerCookie::set_use_max_age);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_max_age"), "set_use_max_age", "get_use_max_age");

	ClassDB::bind_method(D_METHOD("get_max_age"), &WebServerCookie::get_max_age);
	ClassDB::bind_method(D_METHOD("set_max_age", "val"), &WebServerCookie::set_max_age);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_age"), "set_max_age", "get_max_age");

	ClassDB::bind_method(D_METHOD("get_use_expiry_date"), &WebServerCookie::get_use_expiry_date);
	ClassDB::bind_method(D_METHOD("set_use_expiry_date", "val"), &WebServerCookie::set_use_expiry_date);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_expiry_date"), "set_use_expiry_date", "get_use_expiry_date");

	ClassDB::bind_method(D_METHOD("get_expiry_date_unix_time"), &WebServerCookie::get_expiry_date_unix_time);
	ClassDB::bind_method(D_METHOD("set_expiry_date_unix_time", "unix_time"), &WebServerCookie::set_expiry_date_unix_time);

	ClassDB::bind_method(D_METHOD("get_expiry_date"), &WebServerCookie::get_expiry_date_dict);
	ClassDB::bind_method(D_METHOD("set_expiry_date", "date", "time"), &WebServerCookie::set_expiry_date_dict);
	ClassDB::bind_method(D_METHOD("set_expiry_date_dt", "date_time"), &WebServerCookie::set_expiry_date_dt_dict);

	ClassDB::bind_method(D_METHOD("set_data", "key", "value"), &WebServerCookie::set_data);
	ClassDB::bind_method(D_METHOD("get_response_header_string"), &WebServerCookie::get_response_header_string);

	BIND_ENUM_CONSTANT(SAME_SITE_UNSET);
	BIND_ENUM_CONSTANT(SAME_SITE_NONE);
	BIND_ENUM_CONSTANT(SAME_SITE_LAX);
	BIND_ENUM_CONSTANT(SAME_SITE_STRICT);
}
