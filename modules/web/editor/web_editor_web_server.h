#ifndef WEB_EDITOR_WEB_SERVER_H
#define WEB_EDITOR_WEB_SERVER_H

#include "core/object/reference.h"

#include "../http/web_server.h"

class WebNode;
class WebEditorWebServerRequest;

class WebEditorWebServer : public WebServer {
	GDCLASS(WebEditorWebServer, WebServer);

public:
	void web_editor_request(WebNode *node, Ref<WebEditorWebServerRequest> request);

	WebEditorWebServer();
	~WebEditorWebServer();

	Ref<WebEditorWebServerRequest> _last_request;

protected:
	static void _bind_methods();
};

#endif
