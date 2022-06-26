#ifndef CSRF_TOKEN_MIDDLEWARE_H
#define CSRF_TOKEN_MIDDLEWARE_H

#include "web_server_middleware.h"

#include "core/ustring.h"
#include "core/vector.h"

class WebServerRequest;

class CSRFTokenWebServerMiddleware : public WebServerMiddleware {
	GDCLASS(CSRFTokenWebServerMiddleware, WebServerMiddleware);

public:
	//returnring true means handled, false means continue
	bool _on_before_handle_request_main(Ref<WebServerRequest> request);

	bool shold_ignore(Ref<WebServerRequest> request);

	virtual String create_token();

	CSRFTokenWebServerMiddleware();
	~CSRFTokenWebServerMiddleware();

	Vector<String> ignored_urls;

protected:
	static void _bind_methods();
};

#endif
