
#include "http_session.h"

String HTTPSession::get_session_id() {
	return session_id;
}
void HTTPSession::set_session_id(const String &val) {
	session_id = val;
}

int HTTPSession::get_id() {
	return id;
}
void HTTPSession::set_id(const int val) {
	id = val;
}

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

Variant HTTPSession::get_value(const String &key) {
	return _data[key];
}
const Variant &HTTPSession::get_const(const String &key) {
	return _data[key];
}

Object *HTTPSession::get_object(const String &key) {
	// don't lock here

	return _data[key];
}

Ref<Reference> HTTPSession::get_reference(const String &key) {
	// don't lock here

	return Ref<Reference>(_data[key]);
}

int HTTPSession::get_int(const String &key) {
	// don't lock here

	return _data[key];
}

void HTTPSession::clear() {
	_data.clear();
}

void HTTPSession::reset() {
	clear();
	session_id = "";
}

HashMap<String, Variant> *HTTPSession::get_data() {
	return &_data;
}

HTTPSession::HTTPSession() {
	id = 0;
}

HTTPSession::~HTTPSession() {
	clear();
}

void HTTPSession::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_session_id"), &HTTPSession::get_session_id);
	ClassDB::bind_method(D_METHOD("set_session_id", "val"), &HTTPSession::set_session_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "session_id"), "set_session_id", "get_session_id");

	ClassDB::bind_method(D_METHOD("get_id"), &HTTPSession::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "val"), &HTTPSession::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ClassDB::bind_method(D_METHOD("add", "key", "value"), &HTTPSession::add);
	ClassDB::bind_method(D_METHOD("remove", "key"), &HTTPSession::remove);
	ClassDB::bind_method(D_METHOD("has", "key"), &HTTPSession::has);
	ClassDB::bind_method(D_METHOD("get_value", "key"), &HTTPSession::get_value);

	ClassDB::bind_method(D_METHOD("clear"), &HTTPSession::clear);
	ClassDB::bind_method(D_METHOD("reset"), &HTTPSession::reset);
}
