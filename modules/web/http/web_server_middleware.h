#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include "core/string.h"

#include "core/reference.h"

class Request;

class WebServerMiddleware : public Reference {
	RCPP_OBJECT(WebServerMiddleware, Reference);

public:
	//returnring true means handled, false, means continue
	virtual bool on_before_handle_request_main(Request *request);

	WebServerMiddleware();
	~WebServerMiddleware();
};

#endif
