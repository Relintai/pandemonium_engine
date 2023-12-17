/*************************************************************************/
/*  http_session.cpp                                                     */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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

int HTTPSession::get_int(const String &key, const int def) {
	// don't lock here

	Variant *v = _data.getptr(key);

	if (!v) {
		return def;
	}

	return *v;
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
