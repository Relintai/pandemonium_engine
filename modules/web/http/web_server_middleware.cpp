
#include "middleware.h"

#include "request.h"

bool Middleware::on_before_handle_request_main(Request *request) {
	return false;
}

Middleware::Middleware() : Reference() {
}

Middleware::~Middleware() {
}