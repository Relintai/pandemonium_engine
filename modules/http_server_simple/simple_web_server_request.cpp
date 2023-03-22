#include "simple_web_server_request.h"

#include "core/object/object.h"
#include "modules/web/http/web_server.h"
#include "modules/web/http/web_server_cookie.h"

#include "modules/web/http/http_session.h"

#include "modules/web/http/http_session_manager.h"
#include "modules/web/http/web_node.h"

#include "http_server_simple.h"
#include "modules/web/http/web_permission.h"

String SimpleWebServerRequest::get_cookie(const String &key) {
	for (int i = 0; i < _cookies.size(); ++i) {
		const CookieData &d = _cookies[i];
		if (d.key == key) {
			return d.value;
		}
	}

	return "";
}

HTTPServerEnums::HTTPMethod SimpleWebServerRequest::get_method() const {
	return _method;
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
	if (!_post_parameters.has(key)) {
		return get_get_parameter(key);
	}

	return get_post_parameter(key);
}
String SimpleWebServerRequest::get_post_parameter(const String &key) const {
	if (!_post_parameters.has(key)) {
		return "";
	}

	return _post_parameters[key];
}
String SimpleWebServerRequest::get_get_parameter(const String &key) const {
	if (!_get_parameters.has(key)) {
		return "";
	}

	return _get_parameters[key];
}

void SimpleWebServerRequest::set_post_parameter(const String &key, const String &value) {
	_post_parameters[key] = value;
}
void SimpleWebServerRequest::set_get_parameter(const String &key, const String &value) {
	_get_parameters[key] = value;
}

void SimpleWebServerRequest::send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
	ERR_FAIL_COND(!_server);

	_connection->send_redirect(Ref<WebServerRequest>(this), location, status_code);
}

void SimpleWebServerRequest::send() {
	ERR_FAIL_COND(!_server);

	_connection->send(Ref<WebServerRequest>(this));

	// if (connection_closed) {
	//	SimpleWebServerRequestPool::return_request(this);
	//	return;
	// }

	// SimpleWebServerRequestPool::return_request(this);
}

void SimpleWebServerRequest::send_file(const String &p_file_path) {
	ERR_FAIL_COND(!_connection.is_valid());

	_connection->send_file(Ref<WebServerRequest>(this), p_file_path);

	// SimpleWebServerRequestPool::return_request(this);
}

String SimpleWebServerRequest::parser_get_path() {
	return _parser_path;
}

String SimpleWebServerRequest::get_host() const {
	return _host;
}

void SimpleWebServerRequest::add_post_parameter(const String &key, const String &value) {
	_post_parameters[key] = value;
}

void SimpleWebServerRequest::add_get_parameter(const String &key, const String &value) {
	_get_parameters[key] = value;
}

void SimpleWebServerRequest::set_parser_path(const String &value) {
	//https://www.rfc-editor.org/rfc/rfc3986.txt
	//3.4.  Query
	//The query component is indicated by the first question mark ("?") ...
	int qpos = value.find_char('?');

	if (qpos == -1) {
		_parser_path = value;
		return;
	}

	_parser_path = value.substr_index(0, qpos);

	//... and terminated by a number sign ("#") character or by the end of the URI.
	int get_query_end = value.find_char('#', qpos);

	// ...The characters slash ("/") and question mark ("?") may represent data within the query component.
	//  "key=value" pairs and one frequently used value is a reference to another URI, it is sometimes
	//  better for usability to avoid percent-encoding those characters.

	if (get_query_end == -1) {
		get_query_end = value.size() - 1;
	}

	String get_req_str = value.substr_index(qpos + 1, get_query_end);

	Vector<String> get_req_params = get_req_str.split("&");

	for (int i = 0; i < get_req_params.size(); ++i) {
		String cp = get_req_params[i];

		int eq = cp.find_char('=');

		if (eq == -1) {
			// skip if &param&
			continue;
		}

		String key = cp.substr_index(0, eq);
		String val = cp.substr_index(eq + 1, cp.size());

		if (key.empty() || val.empty()) {
			// &=v&  or  &p=&
			continue;
		}

		add_get_parameter(key.percent_decode(), val.percent_decode());
	}
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

void SimpleWebServerRequest::add_cookie_data(const String &key, const String &value) {
	CookieData d;
	d.key = key;
	d.value = value;

	_cookies.push_back(d);
}

void SimpleWebServerRequest::set_method(const HTTPServerEnums::HTTPMethod method) {
	_method = method;
}

SimpleWebServerRequest::SimpleWebServerRequest() {
	_server = nullptr;
	_method = HTTPServerEnums::HTTP_METHOD_GET;
}

SimpleWebServerRequest::~SimpleWebServerRequest() {
}

void SimpleWebServerRequest::_bind_methods() {
}
