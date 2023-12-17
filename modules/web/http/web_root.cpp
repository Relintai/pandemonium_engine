/*************************************************************************/
/*  web_root.cpp                                                         */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "web_root.h"

#include "core/object/object.h"
#include "modules/web/http/http_server_enums.h"
#include "web_server_request.h"

#include "../file_cache.h"

#include "web_permission.h"

String WebRoot::get_www_root_path() {
	return _www_root_path;
}
void WebRoot::set_www_root_path(const String &val) {
	_www_root_path = val;

	if (val == "") {
		_www_root_file_cache->set_wwwroot(val);
		_www_root_file_cache->clear();
	} else {
		_www_root_file_cache->set_wwwroot(val);
		_www_root_file_cache->wwwroot_refresh_cache();
	}
}

Ref<FileCache> WebRoot::get_www_root_file_cache() {
	return _www_root_file_cache;
}

Vector<Variant> WebRoot::get_middlewares() {
	Vector<Variant> r;
	for (int i = 0; i < _middlewares.size(); i++) {
		r.push_back(_middlewares[i].get_ref_ptr());
	}
	return r;
}

void WebRoot::set_middlewares(const Vector<Variant> &data) {
	_middlewares.clear();
	for (int i = 0; i < data.size(); i++) {
		Ref<WebServerMiddleware> e = Ref<WebServerMiddleware>(data[i]);
		_middlewares.push_back(e);
	}
}

//void WebRoot::setup_middleware() {
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
//}

void WebRoot::_handle_request_main(Ref<WebServerRequest> request) {
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
		handle_request(request);
		return;
	}

	if (!try_route_request_to_children(request)) {
		handle_request(request);
	}
}

void WebRoot::_handle_error_send_request(Ref<WebServerRequest> request, const int error_code) {
	if (error_code == HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND) {
		request->compiled_body = "<html><body>404 :(</body></html>";

		request->send();
		return;
	}

	request->compiled_body = "<html><body>Internal server error! :(</body></html>";
	request->send();
}

bool WebRoot::process_middlewares(Ref<WebServerRequest> request) {
	for (int i = 0; i < _middlewares.size(); ++i) {
		Ref<WebServerMiddleware> m = _middlewares[i];

		if (!m.is_valid()) {
			continue;
		}

		if (m->on_before_handle_request_main(request)) {
			// handled
			return true;
		}
	}

	return false;
}

bool WebRoot::try_send_wwwroot_file(Ref<WebServerRequest> request) {
	String path = request->get_path_full();
	path = path.to_lower();

	int file_indx = _www_root_file_cache->wwwroot_get_file_index(path);

	if (file_indx != -1) {
		send_file(_www_root_file_cache->wwwroot_get_file_orig_path(file_indx), request);

		return true;
	}

	return false;
}

void WebRoot::send_file(const String &path, Ref<WebServerRequest> request) {
	String fp = _www_root_file_cache->get_wwwroot_abs() + path;

	request->send_file(fp);
}

void WebRoot::register_request_update(Ref<WebServerRequest> request) {
	_update_registered_requests_mutex.lock();
	_update_registered_requests.push_back(request);
	_update_registered_requests_mutex.unlock();
}
void WebRoot::unregister_request_update(Ref<WebServerRequest> request) {
	_update_registered_requests_mutex.lock();

	std::size_t s = _update_registered_requests.size();
	for (std::size_t i = 0; i < s; ++i) {
		Ref<WebServerRequest> r = _update_registered_requests[i];

		if (r == request) {
			_update_registered_requests.remove(i);
			_update_registered_requests_mutex.unlock();

			return;
		}
	}
	_update_registered_requests_mutex.unlock();
}

WebRoot::WebRoot() {
	_www_root_file_cache.instance();
	set_process_internal(true);
}

WebRoot::~WebRoot() {
}

void WebRoot::_notification(int p_what) {
	if (p_what == NOTIFICATION_INTERNAL_PROCESS) {
		for (int i = 0; i < _update_registered_requests.size(); ++i) {
			Ref<WebServerRequest> r = _update_registered_requests[i];

			r->update();
		}
	} else if (p_what == NOTIFICATION_READY) {
		if (!_www_root_path.empty()) {
			_www_root_file_cache->wwwroot_refresh_cache();
		}
	}
}

void WebRoot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_www_root_path"), &WebRoot::get_www_root_path);
	ClassDB::bind_method(D_METHOD("set_www_root_path", "val"), &WebRoot::set_www_root_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "www_root_path"), "set_www_root_path", "get_www_root_path");

	ClassDB::bind_method(D_METHOD("get_www_root_file_cache"), &WebRoot::get_www_root_file_cache);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "www_root_file_cache", PROPERTY_HINT_RESOURCE_TYPE, "FileCache", 0), "", "get_www_root_file_cache");

	ClassDB::bind_method(D_METHOD("get_middlewares"), &WebRoot::get_middlewares);
	ClassDB::bind_method(D_METHOD("set_middlewares", "data"), &WebRoot::set_middlewares);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "middlewares", PROPERTY_HINT_NONE, "23/20:WebServerMiddleware", PROPERTY_USAGE_DEFAULT, "WebServerMiddleware"), "set_middlewares", "get_middlewares");

	ClassDB::bind_method(D_METHOD("process_middlewares", "request"), &WebRoot::process_middlewares);
	ClassDB::bind_method(D_METHOD("try_send_wwwroot_file", "request"), &WebRoot::try_send_wwwroot_file);
	ClassDB::bind_method(D_METHOD("send_file", "path", "request"), &WebRoot::send_file);

	ClassDB::bind_method(D_METHOD("register_request_update", "request"), &WebRoot::register_request_update);
	ClassDB::bind_method(D_METHOD("unregister_request_update", "request"), &WebRoot::unregister_request_update);
}
