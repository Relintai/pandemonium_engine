
#include "middleware.h"

#include "request.h"

bool WebServerMiddleware::on_before_handle_request_main(Request *request) {
	return false;
}

WebServerMiddleware::WebServerMiddleware() :
		Reference() {
}

WebServerMiddleware::~WebServerMiddleware() {
}
