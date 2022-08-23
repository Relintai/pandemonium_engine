#include "web_node_editor_web_server.h"

#include "../http/web_node.h"
#include "web_node_editor_web_server_request.h"


void WebNodeEditorWebServer::web_editor_request(WebNode *node, Ref<WebNodeEditorWebServerRequest> request) {
	_web_root = node;
	_last_request = request;

	//server_handle_request(request);

	request->_set_server(this);
	request->_set_web_root(_web_root);

	node->handle_request(request);
}

WebNodeEditorWebServer::WebNodeEditorWebServer() {
	_is_running = true;
}

WebNodeEditorWebServer::~WebNodeEditorWebServer() {
}

void WebNodeEditorWebServer::_bind_methods() {
}
