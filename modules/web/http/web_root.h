#ifndef WEB_ROOT_H
#define WEB_ROOT_H

#include "core/containers/vector.h"
#include "core/string.h"
#include "mutex"
#include <functional>
#include <map>

#include "web_node.h"

#include "middleware.h"

class Request;

// TODO FileCache -> set up, for this webroot, don't use singleton

class WebRoot : public WebNode {
	RCPP_OBJECT(WebRoot, WebNode);

public:
	static String default_error_404_body;
	static String default_generic_error_body;

	void handle_request_main(Request *request);
	void handle_error_send_request(Request *request, const int error_code);

	bool process_middlewares(Request *request);
	bool try_send_wwwroot_file(Request *request);
	void send_file(const String &path, Request *request);

	static void default_fallback_error_handler(Request *request, int error_code);
	static void default_404_error_handler(Request *request, int error_code);

	virtual void setup();
	virtual void setup_error_handlers();
	virtual void setup_middleware();

	void default_routing_middleware(Object *instance, Request *request);

	void register_request_update(Request *request);
	void unregister_request_update(Request *request);
	void update();

	WebRoot();
	virtual ~WebRoot();

public:
	Vector<Ref<WebServerMiddleware>> _middlewares;

	std::map<int, std::function<void(Request *, int)>> error_handler_map;
	std::function<void(Request *, int)> default_error_handler_func;

protected:
	std::mutex _update_registered_requests_mutex;
	std::vector<Request *> _update_registered_requests;
};

#endif
