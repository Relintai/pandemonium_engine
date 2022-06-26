#include "web_server_request.h"

#include "web/http/cookie.h"
#include "web_server.h"

#include "http_session.h"

#include "session_manager.h"
#include "web/http/web_root.h"

#include "web_permission.h"

Ref<HTTPSession> WebServerRequest::get_or_create_session() {
	if (session.is_valid()) {
		return session;
	}

	session = SessionManager::get_singleton()->create_session();

	return session;
}

bool WebServerRequest::can_view() const {
	return (permissions & WebPermission::WEB_PERMISSION_VIEW) != 0;
}
bool WebServerRequest::can_create() const {
	return (permissions & WebPermission::WEB_PERMISSION_CREATE) != 0;
}
bool WebServerRequest::can_edit() const {
	return (permissions & WebPermission::WEB_PERMISSION_EDIT) != 0;
}
bool WebServerRequest::can_delete() const {
	return (permissions & WebPermission::WEB_PERMISSION_DELETE) != 0;
}

bool WebServerRequest::has_csrf_token() {
	if (!session.is_valid()) {
		return false;
	}

	return session->has("csrf_token");
}

String WebServerRequest::get_csrf_token() {
	if (!session.is_valid()) {
		return "";
	}

	const Variant &val = session->get_const("csrf_token");

	if (val.is_simple_type()) {
		return val.to_string();
	}

	return "";
}

void WebServerRequest::set_csrf_token(const String &value) {
	if (session.is_valid()) {
		session->add("csrf_token", value);

		SessionManager::get_singleton()->save_session(session);
	}
}

bool WebServerRequest::validate_csrf_token() {
	String param_token = get_parameter("csrf_token");
	param_token.trim();

	if (param_token == "") {
		return false;
	}

	String token = get_csrf_token();

	if (token == "") {
		return false;
	}

	return param_token == token;
}

const String WebServerRequest::get_cookie(const String &key) {
	static String str(0);
	return str;
}

void WebServerRequest::add_cookie(const ::Cookie &cookie) {
}

void WebServerRequest::remove_cookie(const String &key) {
}

HTTPMethod WebServerRequest::get_method() const {
	return HTTP_METHOD_GET;
}

void WebServerRequest::parse_files() {
}
int WebServerRequest::get_file_count() const {
	return 0;
}
int WebServerRequest::get_file_length(const int index) const {
	return 0;
}
const uint8_t *WebServerRequest::get_file_data(const int index) const {
	return nullptr;
}

const String WebServerRequest::get_parameter(const String &key) const {
	static String str(0);
	return str;
}

HTTPStatusCode WebServerRequest::get_status_code() const {
	return _status_code;
}
void WebServerRequest::set_status_code(const HTTPStatusCode status_code) {
	_status_code = status_code;
}

void WebServerRequest::send_redirect(const String &location, const HTTPStatusCode status_code) {
}

void WebServerRequest::compile_body() {
	compiled_body.ensure_capacity(body.size() + head.size() + 15 + 13 + 14 + 15 + 1);

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

void WebServerRequest::compile_and_send_body() {
	compile_body();
	send();
}

void WebServerRequest::send() {
	// if (connection_closed) {
	//	WebServerRequestPool::return_request(this);
	//	return;
	// }

	// WebServerRequestPool::return_request(this);
}

void WebServerRequest::send_file(const String &p_file_path) {
	// WebServerRequestPool::return_request(this);
}

void WebServerRequest::send_error(int error_code) {
	server->get_web_root()->handle_error_send_request(this, error_code);
}

void WebServerRequest::reset() {
	session = nullptr;
	server = nullptr;
	_path_stack.clear();
	_path_stack_pointer = 0;
	file_size = 0;
	current_file_progress = 0;
	connection_closed = false;
	_full_path = "";
	_status_code = HTTP_STATUS_CODE_200_OK;
	// Maybe set NONE or only VIEW as default?
	permissions = WebPermission::WEB_PERMISSION_ALL;
	active_permission.unref();

	head.clear();
	body.clear();
	footer.clear();
	compiled_body.clear();

	data.clear();
	reference_data.clear();
}

String WebServerRequest::parser_get_path() {
	return "";
}

void WebServerRequest::setup_url_stack() {
	_full_path = parser_get_path();
	String path = parser_get_path();

	size_t pos = 0;
	String st;
	while ((pos = path.find('/')) != -1) {
		st = path.substr(0, pos);

		if (st.size() != 0) {
			_path_stack.push_back(st);
		}

		path.erase(0, pos + 1);
	}

	if (path.size() != 0) {
		_path_stack.push_back(path);
	}
}

String WebServerRequest::get_path(const bool beginning_slash, const bool end_slash) const {
	String path;

	if (beginning_slash) {
		path += '/';
	}

	for (uint32_t i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += '/';
	}

	if (!end_slash && path.size() > 1) {
		path.pop_back();
	}

	return path;
}

const String &WebServerRequest::get_path_full() const {
	return _full_path;
}

const String &WebServerRequest::get_path_segment(const uint32_t i) const {
	return _path_stack[i];
}

const String &WebServerRequest::get_current_path_segment() const {
	if (_path_stack_pointer >= _path_stack.size()) {
		// for convenience
		static const String e_str = "";
		return e_str;
	}

	return _path_stack[_path_stack_pointer];
}

const String &WebServerRequest::get_next_path_segment() const {
	int pst = _path_stack_pointer + 1;

	if (pst >= _path_stack.size()) {
		// for convenience
		static const String e_str = "";
		return e_str;
	}

	return _path_stack[pst];
}

uint32_t WebServerRequest::get_path_segment_count() const {
	return _path_stack.size();
}

uint32_t WebServerRequest::get_current_segment_index() const {
	return _path_stack_pointer;
}

uint32_t WebServerRequest::get_remaining_segment_count() const {
	if (_path_stack_pointer > _path_stack.size()) {
		return 0;
	}

	return _path_stack.size() - _path_stack_pointer;
}

void WebServerRequest::pop_path() {
	_path_stack_pointer -= 1;
}

void WebServerRequest::push_path() {
	_path_stack_pointer += 1;
}

String WebServerRequest::get_url_root_parent(const int parent) const {
	String path = "/";

	for (uint32_t i = 0; i < _path_stack_pointer - parent; ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_root() const {
	int pst = _path_stack_pointer + 1;

	if (pst > _path_stack.size()) {
		pst = _path_stack.size();
	}

	String path = "/";

	for (uint32_t i = 0; i < _path_stack_pointer; ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_root_current() const {
	int pst = _path_stack_pointer + 1;

	if (pst > _path_stack.size()) {
		pst = _path_stack.size();
	}

	String path = "/";

	for (uint32_t i = 0; i < pst; ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_site() const {
	String path = get_host();

	for (uint32_t i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_root_parent(const String &add) const {
	return get_url_root_parent() + add;
}
String WebServerRequest::get_url_root(const String &add) const {
	return get_url_root() + add;
}
String WebServerRequest::get_url_site(const String &add) const {
	return get_url_site() + add;
}

String WebServerRequest::get_host() const {
	return "";
}

void WebServerRequest::update() {
}

void WebServerRequest::pool() {
}

WebServerRequest::WebServerRequest() {
	// This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	// It is a tradeoff on server memory though, as every active download will consume this amount of memory
	// where the file is bigger than this number
	file_chunk_size = 1 << 21; // 2MB
}

WebServerRequest::~WebServerRequest() {
}
