#include "web_editor_web_server.h"

#include "../http/web_node.h"
#include "web_editor_web_server_request.h"


void WebEditorWebServer::web_editor_request(WebNode *node, Ref<WebEditorWebServerRequest> request) {
	_web_root = node;
	_last_request = request;

	//server_handle_request(request);

	request->_set_server(this);
	request->_set_web_root(_web_root);

	node->handle_request(request);
}

WebEditorWebServer::WebEditorWebServer() {
	_is_running = true;
}

WebEditorWebServer::~WebEditorWebServer() {
}

void WebEditorWebServer::_bind_methods() {
}
