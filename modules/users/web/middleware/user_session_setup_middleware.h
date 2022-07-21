#ifndef USER_SESSION_SETUP_MIDDLEWARE_H
#define USER_SESSION_SETUP_MIDDLEWARE_H

#include "core/reference.h"
#include "core/ustring.h"

#include "modules/web/http/web_server_middleware.h"

// just session setup
class UserSessionSetupMiddleware : public WebServerMiddleware {
	GDCLASS(UserSessionSetupMiddleware, WebServerMiddleware);

public:
	//returning true means handled, false means continue
	bool on_before_handle_request_main(Ref<WebServerRequest> request);

	UserSessionSetupMiddleware();
	~UserSessionSetupMiddleware();
};

#endif
