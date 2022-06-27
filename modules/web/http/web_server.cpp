#include "web_server.h"

#include "web_node.h"
#include "web_server_request.h"

#include "http_session_manager.h"

NodePath WebServer::get_web_root_path() const {
	return _web_root_path;
}
void WebServer::set_web_root_path(const NodePath &path) {
	_web_root_path = path;
}

WebNode *WebServer::get_web_root() {
	return _web_root;
}

void WebServer::set_web_root(WebNode *root) {
	_web_root = root;
}

Node *WebServer::get_web_root_bind() {
	return _web_root;
}
void WebServer::set_web_root_bind(Node *root) {
	WebNode *web_root = Object::cast_to<WebNode>(root);

	_web_root = web_root;
}

HTTPSessionManager *WebServer::get_session_manager() {
	return _session_manager;
}
void WebServer::set_session_manager(HTTPSessionManager *sess_man) {
	_session_manager = sess_man;
}

Node *WebServer::get_session_manager_bind() {
	return _session_manager;
}
void WebServer::set_session_manager_bind(Node *sess_man) {
	_session_manager = Object::cast_to<HTTPSessionManager>(sess_man);
}

void WebServer::server_handle_request(Ref<WebServerRequest> request) {
	ERR_FAIL_COND(!_web_root);

	request->server = this;
	request->web_root = _web_root;

	_rw_lock.read_lock();
	_web_root->handle_request_main(request);
	_rw_lock.read_unlock();
}

void WebServer::request_write_lock() {
	_write_lock_requested = true;
}

void WebServer::start() {
	call("_start");
}
void WebServer::stop() {
	call("_stop");
}

void WebServer::_start() {
	//look up root node, and sessionmanager, if not set.

	if (!_web_root && _web_root_path != NodePath()) {
		_web_root = Object::cast_to<WebNode>(get_node_or_null(_web_root_path));
	}
}
void WebServer::_stop() {
}

WebServer::WebServer() {
	_web_root = nullptr;

	_write_lock_requested = false;
}

WebServer::~WebServer() {
}

void WebServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_web_root_path"), &WebServer::get_web_root_path);
	ClassDB::bind_method(D_METHOD("set_web_root_path", "val"), &WebServer::set_web_root_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "web_root_path"), "set_web_root_path", "get_web_root_path");

	ClassDB::bind_method(D_METHOD("get_web_root"), &WebServer::get_web_root_bind);
	ClassDB::bind_method(D_METHOD("set_web_root", "val"), &WebServer::set_web_root_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "web_root", PROPERTY_HINT_RESOURCE_TYPE, "WebRoot", 0), "set_web_root", "get_web_root");

	ClassDB::bind_method(D_METHOD("get_session_manager"), &WebServer::get_session_manager_bind);
	ClassDB::bind_method(D_METHOD("set_session_manager", "val"), &WebServer::set_session_manager_bind);

	ClassDB::bind_method(D_METHOD("server_handle_request", "request"), &WebServer::server_handle_request);
	ClassDB::bind_method(D_METHOD("request_write_lock"), &WebServer::request_write_lock);

	BIND_VMETHOD(MethodInfo("_start"));
	BIND_VMETHOD(MethodInfo("_stop"));
	ClassDB::bind_method(D_METHOD("start"), &WebServer::start);
	ClassDB::bind_method(D_METHOD("stop"), &WebServer::stop);
	ClassDB::bind_method(D_METHOD("_start"), &WebServer::_start);
	ClassDB::bind_method(D_METHOD("_stop"), &WebServer::_stop);
}
