#include "web_server_request.h"

#include "core/object/object.h"
#include "core/variant/variant.h"
#include "web_server.h"
#include "web_server_cookie.h"

#include "http_session.h"

#include "http_session_manager.h"
#include "web_node.h"

#include "web_permission.h"

String WebServerRequest::get_head() {
	return head;
}
void WebServerRequest::set_head(const String &val) {
	head = val;
}

String WebServerRequest::get_body() {
	return body;
}
void WebServerRequest::set_body(const String &val) {
	body = val;
}

String WebServerRequest::get_footer() {
	return footer;
}
void WebServerRequest::set_footer(const String &val) {
	footer = val;
}

String WebServerRequest::get_compiled_body() {
	return compiled_body;
}
void WebServerRequest::set_compiled_body(const String &val) {
	compiled_body = val;
}

bool WebServerRequest::get_connection_closed() {
	return _connection_closed;
}
void WebServerRequest::set_connection_closed(const bool &val) {
	_connection_closed = val;
}

Ref<HTTPSession> WebServerRequest::get_session() {
	return _session;
}
void WebServerRequest::set_session(const Ref<HTTPSession> &val) {
	_session = val;
}

Ref<WebPermission> WebServerRequest::get_active_permission() {
	return _active_permission;
}
void WebServerRequest::set_active_permission(const Ref<WebPermission> &val) {
	_active_permission = val;
}

int WebServerRequest::get_permissions() {
	return _permissions;
}
void WebServerRequest::set_permissions(const int &val) {
	_permissions = val;
}

Ref<HTTPSession> WebServerRequest::get_or_create_session() {
	if (_session.is_valid()) {
		return _session;
	}

	HTTPSessionManager *sm = _server->get_session_manager();

	ERR_FAIL_COND_V(!sm, _session);

	_session = sm->create_session();

	return _session;
}

bool WebServerRequest::can_view() const {
	return (_permissions & WebPermission::WEB_PERMISSION_VIEW) != 0;
}
bool WebServerRequest::can_create() const {
	return (_permissions & WebPermission::WEB_PERMISSION_CREATE) != 0;
}
bool WebServerRequest::can_edit() const {
	return (_permissions & WebPermission::WEB_PERMISSION_EDIT) != 0;
}
bool WebServerRequest::can_delete() const {
	return (_permissions & WebPermission::WEB_PERMISSION_DELETE) != 0;
}

bool WebServerRequest::has_csrf_token() {
	if (!_session.is_valid()) {
		return false;
	}

	return _session->has("csrf_token");
}

String WebServerRequest::get_csrf_token() {
	if (!_session.is_valid()) {
		return "";
	}

	const Variant &val = _session->get_const("csrf_token");

	return val;
}

void WebServerRequest::set_csrf_token(const String &value) {
	if (_session.is_valid()) {
		_session->add("csrf_token", value);

		HTTPSessionManager *sm = _server->get_session_manager();

		ERR_FAIL_COND(!sm);

		sm->save_session(_session);
	}
}

bool WebServerRequest::validate_csrf_token() {
	String param_token = get_parameter("csrf_token");
	param_token.strip_edges();

	if (param_token == "") {
		return false;
	}

	String token = get_csrf_token();

	if (token == "") {
		return false;
	}

	return param_token == token;
}

String WebServerRequest::get_cookie(const String &key) {
	return "";
}

void WebServerRequest::response_add_cookie(const Ref<WebServerCookie> &cookie) {
	_cookies.push_back(cookie);
}

int WebServerRequest::response_get_cookie_count() {
	return _cookies.size();
}

Ref<WebServerCookie> WebServerRequest::response_get_cookie(const int index) {
	ERR_FAIL_INDEX_V(index, _cookies.size(), Ref<WebServerCookie>());

	return _cookies[index];
}

void WebServerRequest::response_remove_cookie(const int index) {
	ERR_FAIL_INDEX(index, _cookies.size());

	_cookies.remove(index);
}

void WebServerRequest::response_remove_cookie_simple(const String &key) {
	Ref<WebServerCookie> cookie;
	cookie.instance();

	cookie->set_data(key, "");
	cookie->set_delete(true);

	_cookies.push_back(cookie);
}

HTTPServerEnums::HTTPMethod WebServerRequest::get_method() const {
	return HTTPServerEnums::HTTP_METHOD_GET;
}

void WebServerRequest::parse_files() {
}
int WebServerRequest::get_file_count() const {
	return 0;
}
String WebServerRequest::get_file_file_name(const int index) const {
	return "";
}
String WebServerRequest::get_file_key(const int index) const {
	return "";
}
int WebServerRequest::get_file_length(const int index) const {
	return 0;
}
PoolByteArray WebServerRequest::get_file_data(const int index) const {
	return PoolByteArray();
}
String WebServerRequest::get_file_data_str(const int index) const {
	return "";
}

String WebServerRequest::get_parameter(const String &key) const {
	return "";
}

HTTPServerEnums::HTTPStatusCode WebServerRequest::get_status_code() const {
	return _status_code;
}
void WebServerRequest::set_status_code(const HTTPServerEnums::HTTPStatusCode status_code) {
	_status_code = status_code;
}

void WebServerRequest::send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
}

void WebServerRequest::compile_body() {
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
	_server->get_web_root()->handle_error_send_request(this, error_code);
}

String WebServerRequest::parser_get_path() {
	return "";
}

void WebServerRequest::setup_url_stack() {
	_full_path = parser_get_path();
	String path = parser_get_path();

	int pos = 0;
	String st;

	while ((pos = path.find("/")) != -1) {
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

	for (int i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += '/';
	}

	if (!end_slash && path.length() > 1) {
		path.resize(path.size() - 1);
	}

	return path;
}

String WebServerRequest::get_path_full() const {
	return _full_path;
}

String WebServerRequest::get_path_segment(const uint32_t i) const {
	return _path_stack[i];
}

String WebServerRequest::get_current_path_segment() const {
	if (_path_stack_pointer >= _path_stack.size()) {
		// for convenience
		static const String e_str = "";
		return e_str;
	}

	return _path_stack[_path_stack_pointer];
}

String WebServerRequest::get_next_path_segment() const {
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

	for (int i = 0; i < _path_stack_pointer - parent; ++i) {
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

	for (int i = 0; i < _path_stack_pointer; ++i) {
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

	for (int i = 0; i < pst; ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_site() const {
	String path = get_host();

	for (int i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

String WebServerRequest::get_url_root_parent_add(const String &add) const {
	return get_url_root_parent() + add;
}
String WebServerRequest::get_url_root_add(const String &add) const {
	return get_url_root() + add;
}
String WebServerRequest::get_url_site_add(const String &add) const {
	return get_url_site() + add;
}

String WebServerRequest::get_host() const {
	return "";
}

void WebServerRequest::update() {
}

WebServer *WebServerRequest::get_server() {
	return _server;
}

WebNode *WebServerRequest::get_web_root() {
	return _web_root;
}

WebServerRequest::WebServerRequest() {
	//session.unref();
	//server = nullptr;
	//_path_stack.clear();
	_path_stack_pointer = 0;
	_connection_closed = false;
	//_full_path = "";
	_status_code = HTTPServerEnums::HTTP_STATUS_CODE_200_OK;
	// Maybe set NONE or only VIEW as default?
	_permissions = WebPermission::WEB_PERMISSION_ALL;
	//active_permission.unref();

	//head.clear();
	//body.clear();
	//footer.clear();
	//compiled_body.clear();

	//data.clear();
	_server = nullptr;
	_web_root = nullptr;
}

WebServerRequest::~WebServerRequest() {
}

void WebServerRequest::_set_server(WebServer *v) {
	_server = v;
}
void WebServerRequest::_set_web_root(WebNode *v) {
	_web_root = v;
}

void WebServerRequest::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_head"), &WebServerRequest::get_head);
	ClassDB::bind_method(D_METHOD("set_head", "val"), &WebServerRequest::set_head);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "head"), "set_head", "get_head");

	ClassDB::bind_method(D_METHOD("get_body"), &WebServerRequest::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "val"), &WebServerRequest::set_body);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "body"), "set_body", "get_body");

	ClassDB::bind_method(D_METHOD("get_footer"), &WebServerRequest::get_footer);
	ClassDB::bind_method(D_METHOD("set_footer", "val"), &WebServerRequest::set_footer);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "footer"), "set_footer", "get_footer");

	ClassDB::bind_method(D_METHOD("get_compiled_body"), &WebServerRequest::get_compiled_body);
	ClassDB::bind_method(D_METHOD("set_compiled_body", "val"), &WebServerRequest::set_compiled_body);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "compiled_body"), "set_compiled_body", "get_compiled_body");

	ClassDB::bind_method(D_METHOD("get_connection_closed"), &WebServerRequest::get_connection_closed);
	ClassDB::bind_method(D_METHOD("set_connection_closed", "val"), &WebServerRequest::set_connection_closed);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "connection_closed"), "set_connection_closed", "get_connection_closed");

	ClassDB::bind_method(D_METHOD("get_session"), &WebServerRequest::get_session);
	ClassDB::bind_method(D_METHOD("set_session", "val"), &WebServerRequest::set_session);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "session", PROPERTY_HINT_RESOURCE_TYPE, "HTTPSession"), "set_session", "get_session");

	ClassDB::bind_method(D_METHOD("get_active_permission"), &WebServerRequest::get_active_permission);
	ClassDB::bind_method(D_METHOD("set_active_permission", "val"), &WebServerRequest::set_active_permission);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "active_permission", PROPERTY_HINT_RESOURCE_TYPE, "WebPermission"), "set_active_permission", "get_active_permission");

	ClassDB::bind_method(D_METHOD("get_permissions"), &WebServerRequest::get_permissions);
	ClassDB::bind_method(D_METHOD("set_permissions", "val"), &WebServerRequest::set_permissions);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "permissions"), "set_permissions", "get_permissions");

	ClassDB::bind_method(D_METHOD("get_or_create_session"), &WebServerRequest::get_or_create_session);

	ClassDB::bind_method(D_METHOD("can_view"), &WebServerRequest::can_view);
	ClassDB::bind_method(D_METHOD("can_create"), &WebServerRequest::can_create);
	ClassDB::bind_method(D_METHOD("can_edit"), &WebServerRequest::can_edit);
	ClassDB::bind_method(D_METHOD("can_delete"), &WebServerRequest::can_delete);

	ClassDB::bind_method(D_METHOD("has_csrf_token"), &WebServerRequest::has_csrf_token);
	ClassDB::bind_method(D_METHOD("get_csrf_token"), &WebServerRequest::get_csrf_token);
	ClassDB::bind_method(D_METHOD("set_csrf_token", "value"), &WebServerRequest::set_csrf_token);
	ClassDB::bind_method(D_METHOD("validate_csrf_token"), &WebServerRequest::validate_csrf_token);

	ClassDB::bind_method(D_METHOD("get_cookie", "key"), &WebServerRequest::get_cookie);

	ClassDB::bind_method(D_METHOD("response_add_cookie", "cookie"), &WebServerRequest::response_add_cookie);
	ClassDB::bind_method(D_METHOD("response_get_cookie_count"), &WebServerRequest::response_get_cookie_count);
	ClassDB::bind_method(D_METHOD("response_get_cookie", "index"), &WebServerRequest::response_get_cookie);
	ClassDB::bind_method(D_METHOD("response_remove_cookie", "index"), &WebServerRequest::response_remove_cookie);
	ClassDB::bind_method(D_METHOD("response_remove_cookie_simple", "key"), &WebServerRequest::response_remove_cookie_simple);

	ClassDB::bind_method(D_METHOD("get_method"), &WebServerRequest::get_method);

	ClassDB::bind_method(D_METHOD("parse_files"), &WebServerRequest::parse_files);
	ClassDB::bind_method(D_METHOD("get_file_count"), &WebServerRequest::get_file_count);
	ClassDB::bind_method(D_METHOD("get_file_file_name", "index"), &WebServerRequest::get_file_file_name);
	ClassDB::bind_method(D_METHOD("get_file_key", "index"), &WebServerRequest::get_file_key);
	ClassDB::bind_method(D_METHOD("get_file_length", "index"), &WebServerRequest::get_file_length);
	ClassDB::bind_method(D_METHOD("get_file_data"), &WebServerRequest::get_file_data);
	ClassDB::bind_method(D_METHOD("get_file_data_str"), &WebServerRequest::get_file_data_str);

	ClassDB::bind_method(D_METHOD("get_parameter", "key"), &WebServerRequest::get_parameter);

	ClassDB::bind_method(D_METHOD("get_status_code"), &WebServerRequest::get_status_code);
	ClassDB::bind_method(D_METHOD("set_status_code", "val"), &WebServerRequest::set_status_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "status_code"), "set_status_code", "get_status_code");

	ClassDB::bind_method(D_METHOD("send_redirect", "location", "status_code "), &WebServerRequest::send_redirect);
	ClassDB::bind_method(D_METHOD("compile_body"), &WebServerRequest::compile_body);
	ClassDB::bind_method(D_METHOD("compile_and_send_body"), &WebServerRequest::compile_and_send_body);
	ClassDB::bind_method(D_METHOD("send"), &WebServerRequest::send);
	ClassDB::bind_method(D_METHOD("send_file", "file_path"), &WebServerRequest::send_file);
	ClassDB::bind_method(D_METHOD("send_error", "error_code"), &WebServerRequest::send_error);

	ClassDB::bind_method(D_METHOD("parser_get_path"), &WebServerRequest::parser_get_path);
	ClassDB::bind_method(D_METHOD("get_host"), &WebServerRequest::get_host);

	ClassDB::bind_method(D_METHOD("setup_url_stack"), &WebServerRequest::setup_url_stack);
	ClassDB::bind_method(D_METHOD("get_path", "beginning_slash", "end_slash "), &WebServerRequest::get_path, false, true);
	ClassDB::bind_method(D_METHOD("get_path_full"), &WebServerRequest::get_path_full);
	ClassDB::bind_method(D_METHOD("get_path_segment", "i"), &WebServerRequest::get_path_segment);
	ClassDB::bind_method(D_METHOD("get_current_path_segment"), &WebServerRequest::get_current_path_segment);
	ClassDB::bind_method(D_METHOD("get_next_path_segment"), &WebServerRequest::get_next_path_segment);
	ClassDB::bind_method(D_METHOD("get_path_segment_count"), &WebServerRequest::get_path_segment_count);
	ClassDB::bind_method(D_METHOD("get_current_segment_index"), &WebServerRequest::get_current_segment_index);
	ClassDB::bind_method(D_METHOD("get_remaining_segment_count"), &WebServerRequest::get_remaining_segment_count);
	ClassDB::bind_method(D_METHOD("pop_path"), &WebServerRequest::pop_path);
	ClassDB::bind_method(D_METHOD("push_path"), &WebServerRequest::push_path);

	ClassDB::bind_method(D_METHOD("get_url_root_parent", "parent"), &WebServerRequest::get_url_root_parent, 1);
	ClassDB::bind_method(D_METHOD("get_url_root"), &WebServerRequest::get_url_root);
	ClassDB::bind_method(D_METHOD("get_url_root_current"), &WebServerRequest::get_url_root_current);
	ClassDB::bind_method(D_METHOD("get_url_site"), &WebServerRequest::get_url_site);

	ClassDB::bind_method(D_METHOD("get_url_root_parent_add", "add"), &WebServerRequest::get_url_root_parent_add);
	ClassDB::bind_method(D_METHOD("get_url_root_add", "add"), &WebServerRequest::get_url_root_add);
	ClassDB::bind_method(D_METHOD("get_url_site_add", "add"), &WebServerRequest::get_url_site_add);

	ClassDB::bind_method(D_METHOD("get_server"), &WebServerRequest::get_server);
	ClassDB::bind_method(D_METHOD("get_web_root"), &WebServerRequest::get_web_root);
}
