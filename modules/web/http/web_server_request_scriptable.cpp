/*************************************************************************/
/*  web_server_request_scriptable.cpp                                    */
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

#include "web_server_request_scriptable.h"

#include "web_node.h"
#include "web_server.h"

String WebServerRequestScriptable::get_cookie(const String &key) {
	return call("_get_cookie", key);
}

HTTPServerEnums::HTTPMethod WebServerRequestScriptable::get_method() const {
	int m = const_cast<WebServerRequestScriptable *>(this)->call("_get_method");

	return static_cast<HTTPServerEnums::HTTPMethod>(m);
}

int WebServerRequestScriptable::get_file_count() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_count");
}
String WebServerRequestScriptable::get_file_file_name(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_file_name", index);
}
String WebServerRequestScriptable::get_file_key(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_key", index);
}
uint64_t WebServerRequestScriptable::get_file_length(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_length", index);
}
PoolByteArray WebServerRequestScriptable::get_file_data(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_data", index);
}
String WebServerRequestScriptable::get_file_data_str(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_file_data_str", index);
}
Error WebServerRequestScriptable::move_file(const int index, const String &p_dest_file, const bool p_force) {
	return (Error)(int)call("_move_file", index, p_dest_file, p_force);
}
bool WebServerRequestScriptable::is_file_moved(const int index) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_is_file_moved", index);
}

String WebServerRequestScriptable::get_parameter(const String &key) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_parameter", key);
}

String WebServerRequestScriptable::get_post_parameter(const String &key) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_post_parameter", key);
}

String WebServerRequestScriptable::get_get_parameter(const String &key) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_get_parameter", key);
}

void WebServerRequestScriptable::set_post_parameter(const String &key, const String &value) {
	call("_set_post_parameter", key, value);
}
void WebServerRequestScriptable::set_get_parameter(const String &key, const String &value) {
	call("_set_get_parameter", key, value);
}

String WebServerRequestScriptable::get_header_parameter(const String &key) const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_header_parameter", key);
}
void WebServerRequestScriptable::set_header_parameter(const String &key, const String &value) {
	call("_set_header_parameter", key, value);
}

PoolStringArray WebServerRequestScriptable::get_header_parameter_keys() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_header_parameter_keys");
}
PoolStringArray WebServerRequestScriptable::get_parameter_keys() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_parameter_keys");
}
PoolStringArray WebServerRequestScriptable::get_post_parameter_keys() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_post_parameter_keys");
}
PoolStringArray WebServerRequestScriptable::get_get_parameter_keys() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_get_parameter_keys");
}

void WebServerRequestScriptable::send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
	call("_send_redirect", location, status_code);
}

void WebServerRequestScriptable::compile_body() {
	call("_compile_body");
}

void WebServerRequestScriptable::compile_and_send_body() {
	call("_compile_and_send_body");
}

void WebServerRequestScriptable::send() {
	call("_send");
}

void WebServerRequestScriptable::send_file(const String &p_file_path) {
	call("_send_file", p_file_path);
}

void WebServerRequestScriptable::send_error(int error_code) {
	call("_send_error", error_code);
}

String WebServerRequestScriptable::parser_get_path() {
	return call("_parser_get_path");
}

String WebServerRequestScriptable::get_path_full() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_path_full");
}

String WebServerRequestScriptable::get_host() const {
	return const_cast<WebServerRequestScriptable *>(this)->call("_get_host");
}

void WebServerRequestScriptable::update() {
	call("_update");
}

// script virtuals

String WebServerRequestScriptable::_get_cookie(const String &key) {
	return String();
}

HTTPServerEnums::HTTPMethod WebServerRequestScriptable::_get_method() const {
	return HTTPServerEnums::HTTP_METHOD_GET;
}

int WebServerRequestScriptable::_get_file_count() const {
	return 0;
}
String WebServerRequestScriptable::_get_file_file_name(const int index) const {
	return String();
}
String WebServerRequestScriptable::_get_file_key(const int index) const {
	return String();
}
int WebServerRequestScriptable::_get_file_length(const int index) const {
	return 0;
}
PoolByteArray WebServerRequestScriptable::_get_file_data(const int index) const {
	return PoolByteArray();
}
String WebServerRequestScriptable::_get_file_data_str(const int index) const {
	return String();
}
Error WebServerRequestScriptable::_move_file(const int index, const String &p_dest_file, const bool p_force) {
	return ERR_PRINTER_ON_FIRE;
}
bool WebServerRequestScriptable::_is_file_moved(const int index) const {
	return true;
}

String WebServerRequestScriptable::_get_parameter(const String &key) const {
	return String();
}

String WebServerRequestScriptable::_get_post_parameter(const String &key) const {
	return String();
}

String WebServerRequestScriptable::_get_get_parameter(const String &key) const {
	return String();
}

void WebServerRequestScriptable::_set_post_parameter(const String &key, const String &value) {
}
void WebServerRequestScriptable::_set_get_parameter(const String &key, const String &value) {
}

String WebServerRequestScriptable::_get_header_parameter(const String &key) const {
	return String();
}
void WebServerRequestScriptable::_set_header_parameter(const String &key, const String &value) {
}

PoolStringArray WebServerRequestScriptable::_get_header_parameter_keys() const {
	return PoolStringArray();
}
PoolStringArray WebServerRequestScriptable::_get_parameter_keys() const {
	return PoolStringArray();
}
PoolStringArray WebServerRequestScriptable::_get_post_parameter_keys() const {
	return PoolStringArray();
}
PoolStringArray WebServerRequestScriptable::_get_get_parameter_keys() const {
	return PoolStringArray();
}

void WebServerRequestScriptable::_send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
}

void WebServerRequestScriptable::_compile_body() {
	//compiled_body.ensure_capacity(body.size() + head.size() + 15 + 13 + 14 + 15 + 1);

	// 15
	compiled_body += "<!DOCTYPE html>";

	// 13
	compiled_body += "<html>"
					 "<head>";

	compiled_body += head;

	// 14
	compiled_body += "</head>"
					 "<body>";

	compiled_body += body;
	compiled_body += footer;

	// 15
	compiled_body += "</body>"
					 "</html>";

	// response->setBody(compiled_body);
}

void WebServerRequestScriptable::_compile_and_send_body() {
	compile_body();
	send();
}

void WebServerRequestScriptable::_send() {
	// if (connection_closed) {
	//	WebServerRequestScriptablePool::return_request(this);
	//	return;
	// }

	// WebServerRequestScriptablePool::return_request(this);
}

void WebServerRequestScriptable::_send_file(const String &p_file_path) {
	// WebServerRequestScriptablePool::return_request(this);
}

void WebServerRequestScriptable::_send_error(int error_code) {
}

String WebServerRequestScriptable::_parser_get_path() {
	return String();
}

String WebServerRequestScriptable::_get_path_full() const {
	return _full_path;
}

String WebServerRequestScriptable::_get_host() const {
	return String();
}

void WebServerRequestScriptable::_update() {
}

WebServerRequestScriptable::WebServerRequestScriptable() {
}

WebServerRequestScriptable::~WebServerRequestScriptable() {
}

void WebServerRequestScriptable::_set_server_bind(Node *v) {
	_set_server(Object::cast_to<WebServer>(v));
}
void WebServerRequestScriptable::_set_web_root_bind(Node *v) {
	_set_web_root(Object::cast_to<WebNode>(v));
}

void WebServerRequestScriptable::_bind_methods() {
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_cookie", PropertyInfo(Variant::STRING, "key")));

	BIND_VMETHOD(MethodInfo(Variant::INT, "_get_method"));

	BIND_VMETHOD(MethodInfo(Variant::INT, "_get_file_count"));
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_file_file_name", PropertyInfo(Variant::INT, "index")));
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_file_key", PropertyInfo(Variant::INT, "index")));
	BIND_VMETHOD(MethodInfo(Variant::INT, "_get_file_length", PropertyInfo(Variant::INT, "index")));
	BIND_VMETHOD(MethodInfo(Variant::POOL_BYTE_ARRAY, "_get_file_data", PropertyInfo(Variant::INT, "index")));
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_file_data_str", PropertyInfo(Variant::INT, "index")));
	BIND_VMETHOD(MethodInfo(Variant::INT, "_move_file", PropertyInfo(Variant::INT, "index"), PropertyInfo(Variant::STRING, "dest_file"), PropertyInfo(Variant::BOOL, "force")));
	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_is_file_moved", PropertyInfo(Variant::INT, "index")));

	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_parameter", PropertyInfo(Variant::STRING, "key")));
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_post_parameter", PropertyInfo(Variant::STRING, "key")));
	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_get_parameter", PropertyInfo(Variant::STRING, "key")));

	BIND_VMETHOD(MethodInfo("_set_post_parameter", PropertyInfo(Variant::STRING, "key"), PropertyInfo(Variant::STRING, "value")));
	BIND_VMETHOD(MethodInfo("_set_get_parameter", PropertyInfo(Variant::STRING, "key"), PropertyInfo(Variant::STRING, "value")));

	BIND_VMETHOD(MethodInfo("_get_header_parameter", PropertyInfo(Variant::STRING, "key")));
	BIND_VMETHOD(MethodInfo("_set_header_parameter", PropertyInfo(Variant::STRING, "key"), PropertyInfo(Variant::STRING, "value")));

	BIND_VMETHOD(MethodInfo(Variant::POOL_STRING_ARRAY, "_get_header_parameter_keys"));
	BIND_VMETHOD(MethodInfo(Variant::POOL_STRING_ARRAY, "_get_parameter_keys"));
	BIND_VMETHOD(MethodInfo(Variant::POOL_STRING_ARRAY, "_get_post_parameter_keys"));
	BIND_VMETHOD(MethodInfo(Variant::POOL_STRING_ARRAY, "_get_get_parameter_keys"));

	BIND_VMETHOD(MethodInfo("_send_redirect", PropertyInfo(Variant::STRING, "location"), PropertyInfo(Variant::INT, "status_code")));

	BIND_VMETHOD(MethodInfo("_compile_body"));
	BIND_VMETHOD(MethodInfo("_compile_and_send_body"));
	BIND_VMETHOD(MethodInfo("_send"));
	BIND_VMETHOD(MethodInfo("_send_file", PropertyInfo(Variant::STRING, "p_file_path")));
	BIND_VMETHOD(MethodInfo("_send_error", PropertyInfo(Variant::INT, "error_code")));

	BIND_VMETHOD(MethodInfo("_parser_get_path"));
	BIND_VMETHOD(MethodInfo("_get_host"));
	BIND_VMETHOD(MethodInfo("_get_path_full"));

	BIND_VMETHOD(MethodInfo("_update"));

	ClassDB::bind_method(D_METHOD("_get_cookie", "key"), &WebServerRequestScriptable::_get_cookie);

	ClassDB::bind_method(D_METHOD("_get_method"), &WebServerRequestScriptable::_get_method);

	ClassDB::bind_method(D_METHOD("_get_file_count"), &WebServerRequestScriptable::_get_file_count);
	ClassDB::bind_method(D_METHOD("_get_file_file_name", "index"), &WebServerRequestScriptable::_get_file_file_name);
	ClassDB::bind_method(D_METHOD("_get_file_key", "index"), &WebServerRequestScriptable::_get_file_key);
	ClassDB::bind_method(D_METHOD("_get_file_length", "index"), &WebServerRequestScriptable::_get_file_length);
	ClassDB::bind_method(D_METHOD("_get_file_data", "index"), &WebServerRequestScriptable::_get_file_data);
	ClassDB::bind_method(D_METHOD("_get_file_data_str", "index"), &WebServerRequestScriptable::_get_file_data_str);
	ClassDB::bind_method(D_METHOD("_move_file", "index", "dest_file", "force"), &WebServerRequestScriptable::_move_file);
	ClassDB::bind_method(D_METHOD("_is_file_moved", "index"), &WebServerRequestScriptable::_is_file_moved);

	ClassDB::bind_method(D_METHOD("_get_parameter", "key"), &WebServerRequestScriptable::_get_parameter);
	ClassDB::bind_method(D_METHOD("_get_post_parameter", "key"), &WebServerRequestScriptable::_get_post_parameter);
	ClassDB::bind_method(D_METHOD("_get_get_parameter", "key"), &WebServerRequestScriptable::_get_get_parameter);

	ClassDB::bind_method(D_METHOD("_set_post_parameter", "key", "value"), &WebServerRequestScriptable::_set_post_parameter);
	ClassDB::bind_method(D_METHOD("_set_get_parameter", "key", "value"), &WebServerRequestScriptable::_set_get_parameter);

	ClassDB::bind_method(D_METHOD("_get_header_parameter", "key"), &WebServerRequestScriptable::_get_header_parameter);
	ClassDB::bind_method(D_METHOD("_set_header_parameter", "key", "value"), &WebServerRequestScriptable::_set_header_parameter);

	ClassDB::bind_method(D_METHOD("_get_header_parameter_keys"), &WebServerRequestScriptable::_get_header_parameter_keys);
	ClassDB::bind_method(D_METHOD("_get_parameter_keys"), &WebServerRequestScriptable::_get_parameter_keys);
	ClassDB::bind_method(D_METHOD("_get_post_parameter_keys"), &WebServerRequestScriptable::_get_post_parameter_keys);
	ClassDB::bind_method(D_METHOD("_get_get_parameter_keys"), &WebServerRequestScriptable::_get_get_parameter_keys);

	ClassDB::bind_method(D_METHOD("_send_redirect", "location", "status_code"), &WebServerRequestScriptable::_send_redirect);

	ClassDB::bind_method(D_METHOD("_compile_body"), &WebServerRequestScriptable::_compile_body);
	ClassDB::bind_method(D_METHOD("_compile_and_send_body"), &WebServerRequestScriptable::_compile_and_send_body);
	ClassDB::bind_method(D_METHOD("_send"), &WebServerRequestScriptable::_send);

	ClassDB::bind_method(D_METHOD("_send_file", "p_file_path"), &WebServerRequestScriptable::_send_file);
	ClassDB::bind_method(D_METHOD("_send_error", "error_code"), &WebServerRequestScriptable::_send_error);

	ClassDB::bind_method(D_METHOD("_parser_get_path"), &WebServerRequestScriptable::_parser_get_path);
	ClassDB::bind_method(D_METHOD("_get_host"), &WebServerRequestScriptable::_get_host);
	ClassDB::bind_method(D_METHOD("_get_path_full"), &WebServerRequestScriptable::_get_path_full);
	ClassDB::bind_method(D_METHOD("_update"), &WebServerRequestScriptable::_update);

	ClassDB::bind_method(D_METHOD("_set_server"), &WebServerRequestScriptable::_set_server_bind);
	ClassDB::bind_method(D_METHOD("_set_web_root"), &WebServerRequestScriptable::_set_web_root_bind);
}
