#ifndef CSRF_TOKEN_H
#define CSRF_TOKEN_H

#include "middleware.h"

#include "core/containers/vector.h"
#include "core/string.h"

class Request;

class CSRFTokenWebServerMiddleware : public WebServerMiddleware {
	RCPP_OBJECT(CSRFTokenWebServerMiddleware, WebServerMiddleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	bool shold_ignore(Request *request);

	virtual String create_token();

	CSRFTokenWebServerMiddleware();
	~CSRFTokenWebServerMiddleware();

	Vector<String> ignored_urls;
};

#endif
