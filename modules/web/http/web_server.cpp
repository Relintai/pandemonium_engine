#include "web_server.h"

#include "web_node.h"
#include "web_server_request.h"

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

void WebServer::handle_request(Ref<WebServerRequest> request) {
	ERR_FAIL_COND(!_web_root);

	_rw_lock.read_lock();
	_web_root->handle_request_main(request);
	_rw_lock.read_unlock();
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

	ClassDB::bind_method(D_METHOD("get_web_root_bind"), &WebServer::get_web_root_bind);
	ClassDB::bind_method(D_METHOD("set_web_root_bind", "val"), &WebServer::set_web_root_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "web_root", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "set_web_root_bind", "get_web_root_bind");

	ClassDB::bind_method(D_METHOD("handle_request", "request"), &WebServer::handle_request);
	ClassDB::bind_method(D_METHOD("request_write_lock"), &WebServer::request_write_lock);
}
