#include "simple_web_server_request.h"

#include "../http/web_server.h"
#include "../http/web_server_cookie.h"
#include "core/object.h"

#include "../http/http_session.h"

#include "../http/http_session_manager.h"
#include "../http/web_node.h"

#include "../http/web_permission.h"
#include "http_server_simple.h"

String SimpleWebServerRequest::get_cookie(const String &key) {
	return "";
}

void SimpleWebServerRequest::add_cookie(const Ref<WebServerCookie> &cookie) {
}

void SimpleWebServerRequest::remove_cookie(const String &key) {
}

HTTPServerEnums::HTTPMethod SimpleWebServerRequest::get_method() const {
	return HTTPServerEnums::HTTP_METHOD_GET;
}

void SimpleWebServerRequest::parse_files() {
}
int SimpleWebServerRequest::get_file_count() const {
	return _files.size();
}
String SimpleWebServerRequest::get_file_file_name(const int index) const {
	ERR_FAIL_INDEX_V(index, _files.size(), "");

	return _files[index].file_name;
}
String SimpleWebServerRequest::get_file_key(const int index) const {
	ERR_FAIL_INDEX_V(index, _files.size(), "");

	return _files[index].file_name;
}
int SimpleWebServerRequest::get_file_length(const int index) const {
	ERR_FAIL_INDEX_V(index, _files.size(), 0);

	return _files[index].data.size();
}
PoolByteArray SimpleWebServerRequest::get_file_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _files.size(), PoolByteArray());

	return _files[index].data;
}
String SimpleWebServerRequest::get_file_data_str(const int index) const {
	ERR_FAIL_INDEX_V(index, _files.size(), "");

	PoolByteArray::Read r = _files[index].data.read();

	String ret = reinterpret_cast<const char *>(r.ptr());

	r.release();

	return ret;
}

String SimpleWebServerRequest::get_parameter(const String &key) const {
	if (!_parameters.has(key)) {
		return "";
	}

	return _parameters[key];
}

void SimpleWebServerRequest::send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
	ERR_FAIL_COND(!_server);

	_server->send_redirect(Ref<WebServerRequest>(this), location, status_code);
}

void SimpleWebServerRequest::send() {
	ERR_FAIL_COND(!_server);

	_server->send(Ref<WebServerRequest>(this));

	// if (connection_closed) {
	//	SimpleWebServerRequestPool::return_request(this);
	//	return;
	// }

	// SimpleWebServerRequestPool::return_request(this);
}

void SimpleWebServerRequest::send_file(const String &p_file_path) {
	ERR_FAIL_COND(!_server);

	_server->send_file(Ref<WebServerRequest>(this), p_file_path);

	// SimpleWebServerRequestPool::return_request(this);
}

String SimpleWebServerRequest::parser_get_path() {
	return _parser_path;
}

String SimpleWebServerRequest::get_host() const {
	return _host;
}

void SimpleWebServerRequest::add_parameter(const String &key, const String &value) {
	_parameters[key] = value;
}

void SimpleWebServerRequest::set_parser_path(const String &value) {
	_parser_path = value;
}

void SimpleWebServerRequest::set_host(const String &value) {
	_host = value;
}

void SimpleWebServerRequest::add_file(const String &key, const String &file_name, const PoolByteArray &data) {
	FileEntry e;
	e.key = key;
	e.file_name = file_name;
	e.data = data;

	_files.push_back(e);
}

SimpleWebServerRequest::SimpleWebServerRequest() {
	_server = nullptr;
}

SimpleWebServerRequest::~SimpleWebServerRequest() {
}

void SimpleWebServerRequest::_bind_methods() {
}
