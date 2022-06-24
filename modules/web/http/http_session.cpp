
#include "http_session.h"

void HTTPSession::add(const String &key, const Variant &value) {
	_mutex.lock();

	_data[key] = value;

	_mutex.unlock();
}
void HTTPSession::remove(const String &key) {
	_mutex.lock();

	_data[key] = Variant();

	_mutex.unlock();
}
bool HTTPSession::has(const String &key) {
	return !_data[key].is_null();
}

Variant HTTPSession::get(const String &key) {
	return _data[key];
}
const Variant &HTTPSession::get_const(const String &key) {
	return _data[key];
}

Object *HTTPSession::get_object(const String &key) {
	// don't lock here

	return _data[key].to_object();
}

Ref<Reference> HTTPSession::get_reference(const String &key) {
	// don't lock here

	return _data[key].to_reference();
}

int HTTPSession::get_int(const String &key) {
	// don't lock here

	return _data[key].to_int();
}

void HTTPSession::clear() {
	_data.clear();
}

void HTTPSession::reset() {
	clear();
	session_id = "";
}

std::map<String, Variant> *HTTPSession::get_data() {
	return &_data;
}

HTTPSession::HTTPSession() {
	id = 0;
}

HTTPSession::~HTTPSession() {
	clear();
}