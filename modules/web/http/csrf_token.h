#ifndef CSRF_TOKEN_H
#define CSRF_TOKEN_H

#include "middleware.h"

#include "core/containers/vector.h"
#include "core/string.h"

class Request;

class CSRFTokenMiddleware : public Middleware {
	RCPP_OBJECT(CSRFTokenMiddleware, Middleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	bool shold_ignore(Request *request);

	virtual String create_token();

	CSRFTokenMiddleware();
	~CSRFTokenMiddleware();

	Vector<String> ignored_urls;
};

#endif