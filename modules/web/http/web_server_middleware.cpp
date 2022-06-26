
#include "web_server_middleware.h"

#include "web_server_request.h"

bool WebServerMiddleware::on_before_handle_request_main(Ref<WebServerRequest> request) {
	return call("_on_before_handle_request_main", request);
}

bool WebServerMiddleware::_on_before_handle_request_main(Ref<WebServerRequest> request) {
	return false;
}

WebServerMiddleware::WebServerMiddleware() {
}

WebServerMiddleware::~WebServerMiddleware() {
}

void WebServerMiddleware::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_on_before_handle_request_main", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));

	ClassDB::bind_method(D_METHOD("on_before_handle_request_main", "request"), &WebServerMiddleware::on_before_handle_request_main);

	ClassDB::bind_method(D_METHOD("_on_before_handle_request_main", "request"), &WebServerMiddleware::_on_before_handle_request_main);
}
