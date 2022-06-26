#include "web_root.h"

#include <functional>
#include <string>

#include "request.h"

#include "web/file_cache.h"

#include <iostream>

#include "web_permission.h"
#include <stdio.h>
#include <stdlib.h>

void WebRoot::setup() {
	setup_error_handlers();
	setup_middleware();
}

void WebRoot::setup_error_handlers() {
	default_error_handler_func = WebRoot::default_fallback_error_handler;
	error_handler_map[404] = WebRoot::default_404_error_handler;
}

void WebRoot::setup_middleware() {
	// WebServerMiddlewares get processed in the order they are in the _middlewares array

	// -------        Built in middleware selection        -------

	// ---   SessionSetupWebServerMiddleware   ---
	// If you want sessions add this to your inherited class.
	// (#include "web/http/session_manager.h")

	// _middlewares.push_back(Ref<SessionSetupWebServerMiddleware>(new SessionSetupWebServerMiddleware()));

	// ---   UserSessionSetupWebServerMiddleware   ---
	// This one looks up users based on sessions
	// (#include "web_modules/users/user_controller.h")

	// _middlewares.push_back(Ref<UserSessionSetupWebServerMiddleware>(new UserSessionSetupWebServerMiddleware()));

	// ---   RBACUserSessionSetupWebServerMiddleware / RBACDefaultUserSessionSetupWebServerMiddleware   ---
	// Same as the previous, but if you want the RBAC system to work use one of these
	// UserSessionSetupWebServerMiddleware is not needed if you need these
	// (#include "web_modules/rbac_users/rbac_user_controller.h")

	// _middlewares.push_back(Ref<RBACUserSessionSetupWebServerMiddleware>(new RBACUserSessionSetupWebServerMiddleware()));
	// _middlewares.push_back(Ref<RBACDefaultUserSessionSetupWebServerMiddleware>(new RBACDefaultUserSessionSetupWebServerMiddleware()));

	// ---   CSRF Token Middlweare    ---
	// (#include "web/http/csrf_token.h")

	// Ref<CSRFTokenWebServerMiddleware> csrf_middleware;
	// csrf_middleware.instance();
	// csrf_middleware->ignored_urls.push_back("/user/login");
	// csrf_middleware->ignored_urls.push_back("/user/register");
	// _middlewares.push_back(csrf_middleware);
}

void WebRoot::default_fallback_error_handler(Request *request, int error_code) {
	request->compiled_body = default_generic_error_body;
	request->send();
}

void WebRoot::default_404_error_handler(Request *request, int error_code) {
	request->compiled_body = default_error_404_body;
	request->send();
}

void WebRoot::handle_request_main(Request *request) {
	if (process_middlewares(request)) {
		return;
	}

	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	// handle files first
	if (try_send_wwwroot_file(request)) {
		return;
	}

	// normal routing
	if (!_routing_enabled) {
		_handle_request_main(request);
		return;
	}

	if (!try_route_request_to_children(request)) {
		_handle_request_main(request);
	}
}

void WebRoot::handle_error_send_request(Request *request, const int error_code) {
	std::function<void(Request *, int)> func = error_handler_map[error_code];

	if (!func) {
		if (!default_error_handler_func) {
			WebNode::handle_error_send_request(request, error_code);
			return;
		}

		default_error_handler_func(request, error_code);
		return;
	}

	func(request, error_code);
}

bool WebRoot::process_middlewares(Request *request) {
	for (int i = 0; i < _middlewares.size(); ++i) {
		if (_middlewares[i]->on_before_handle_request_main(request)) {
			// handled
			return true;
		}
	}

	return false;
}

bool WebRoot::try_send_wwwroot_file(Request *request) {
	const String &path = request->get_path_full();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return true;
	}

	return false;
}

void WebRoot::send_file(const String &path, Request *request) {
	String fp = FileCache::get_singleton()->wwwroot + path;

	request->send_file(fp);
}

void WebRoot::register_request_update(Request *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	_update_registered_requests.push_back(request);
}
void WebRoot::unregister_request_update(Request *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	std::size_t s = _update_registered_requests.size();
	for (std::size_t i = 0; i < s; ++i) {
		Request *r = _update_registered_requests[i];

		if (r == request) {
			_update_registered_requests[i] = _update_registered_requests[s - 1];

			_update_registered_requests.pop_back();

			return;
		}
	}
}

void WebRoot::update() {
	for (std::size_t i = 0; i < _update_registered_requests.size(); ++i) {
		Request *r = _update_registered_requests[i];

		r->update();
	}
}

WebRoot::WebRoot() :
		WebNode() {
}

WebRoot::~WebRoot() {
	error_handler_map.clear();
	_middlewares.clear();
}

String WebRoot::default_error_404_body = "<html><body>404 :(</body></html>";
String WebRoot::default_generic_error_body = "<html><body>Internal server error! :(</body></html>";
