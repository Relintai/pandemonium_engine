/*************************************************************************/
/*  web_node_editor_web_server_request.cpp                               */
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

#include "web_node_editor_web_server_request.h"

#include "core/object/object.h"
#include "core/variant/variant.h"

#include "../http/web_node.h"
#include "../http/web_server.h"

String WebNodeEditorWebServerRequest::get_cookie(const String &key) {
	return "";
}

HTTPServerEnums::HTTPMethod WebNodeEditorWebServerRequest::get_method() const {
	return HTTPServerEnums::HTTP_METHOD_GET;
}

void WebNodeEditorWebServerRequest::parse_files() {
}
int WebNodeEditorWebServerRequest::get_file_count() const {
	return 0;
}
String WebNodeEditorWebServerRequest::get_file_file_name(const int index) const {
	return "";
}
String WebNodeEditorWebServerRequest::get_file_key(const int index) const {
	return "";
}
int WebNodeEditorWebServerRequest::get_file_length(const int index) const {
	return 0;
}
PoolByteArray WebNodeEditorWebServerRequest::get_file_data(const int index) const {
	return PoolByteArray();
}
String WebNodeEditorWebServerRequest::get_file_data_str(const int index) const {
	return "";
}

String WebNodeEditorWebServerRequest::get_parameter(const String &key) const {
	return "";
}

void WebNodeEditorWebServerRequest::send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
	_response_type = RESPONSE_TYPE_REDIRECT;
	_status_code = status_code;
	_sent_message = location;
}

void WebNodeEditorWebServerRequest::send() {
	_response_type = RESPONSE_TYPE_NORMAL;
	_sent_message = get_compiled_body();
}

void WebNodeEditorWebServerRequest::send_file(const String &p_file_path) {
	_response_type = RESPONSE_TYPE_FILE;
	_sent_message = p_file_path;
}

void WebNodeEditorWebServerRequest::send_error(int error_code) {
	_error_handler_called = true;

	_server->get_web_root()->handle_error_send_request(this, error_code);
}

String WebNodeEditorWebServerRequest::parser_get_path() {
	return "";
}

String WebNodeEditorWebServerRequest::get_host() const {
	return "";
}

void WebNodeEditorWebServerRequest::update() {
}

WebNodeEditorWebServerRequest::WebNodeEditorWebServerRequest() {
	_response_type = RESPONSE_TYPE_NONE;
	_error_handler_called = false;
}

WebNodeEditorWebServerRequest::~WebNodeEditorWebServerRequest() {
}

void WebNodeEditorWebServerRequest::_bind_methods() {
}
