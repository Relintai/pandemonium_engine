#ifndef WEB_SERVRER_MIDDLEWARE_H
#define WEB_SERVRER_MIDDLEWARE_H

#include "core/ustring.h"

#include "core/reference.h"

class WebServerRequest;

class WebServerMiddleware : public Reference {
	GDCLASS(WebServerMiddleware, Reference);

public:
	//returnring true means handled, false, means continue
	bool on_before_handle_request_main(Ref<WebServerRequest> request);

	virtual bool _on_before_handle_request_main(Ref<WebServerRequest> request);

	WebServerMiddleware();
	~WebServerMiddleware();

protected:
	static void _bind_methods();
};

#endif
