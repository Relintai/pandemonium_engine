#ifndef USER_SESSION_SETUP_WEB_SERVER_MIDDLEWARE_H
#define USER_SESSION_SETUP_WEB_SERVER_MIDDLEWARE_H

#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "modules/web/http/web_server_middleware.h"

// just session setup
class UserSessionSetupWebServerMiddleware : public WebServerMiddleware {
	GDCLASS(UserSessionSetupWebServerMiddleware, WebServerMiddleware);

public:
	//returning true means handled, false means continue
	bool _on_before_handle_request_main(Ref<WebServerRequest> request);

	UserSessionSetupWebServerMiddleware();
	~UserSessionSetupWebServerMiddleware();
};

#endif
