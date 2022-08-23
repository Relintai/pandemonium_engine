#ifndef WEB_NODE_EDITOR_WEB_SERVER_H
#define WEB_NODE_EDITOR_WEB_SERVER_H

#include "core/object/reference.h"

#include "../http/web_server.h"

class WebNode;
class WebNodeEditorWebServerRequest;

class WebNodeEditorWebServer : public WebServer {
	GDCLASS(WebNodeEditorWebServer, WebServer);

public:
	void web_editor_request(WebNode *node, Ref<WebNodeEditorWebServerRequest> request);

	WebNodeEditorWebServer();
	~WebNodeEditorWebServer();

	Ref<WebNodeEditorWebServerRequest> _last_request;

protected:
	static void _bind_methods();
};

#endif
