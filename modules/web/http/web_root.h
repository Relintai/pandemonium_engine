#ifndef WEB_ROOT_H
#define WEB_ROOT_H

#include "core/hash_map.h"
#include "core/os/mutex.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "web_node.h"

#include "web_server_middleware.h"

class WebServerRequest;

// TODO FileCache -> set up, for this webroot, don't use singleton

class WebRoot : public WebNode {
	GDCLASS(WebRoot, WebNode);

public:
	String get_default_error_404_body();
	void set_default_error_404_body(const String &val);

	String get_default_generic_error_body();
	void set_default_generic_error_body(const String &val);

	void handle_request_main(Ref<WebServerRequest> request);
	void handle_error_send_request(Ref<WebServerRequest> request, const int error_code);

	bool process_middlewares(Ref<WebServerRequest> request);
	bool try_send_wwwroot_file(Ref<WebServerRequest> request);
	void send_file(const String &path, Ref<WebServerRequest> request);

	static void default_fallback_error_handler(Ref<WebServerRequest> request, int error_code);
	static void default_404_error_handler(Ref<WebServerRequest> request, int error_code);

	virtual void setup();
	virtual void setup_error_handlers();
	virtual void setup_middleware();

	void default_routing_middleware(Object *instance, Ref<WebServerRequest> request);

	void register_request_update(Ref<WebServerRequest> request);
	void unregister_request_update(Ref<WebServerRequest> request);
	void update();

	WebRoot();
	~WebRoot();

	static String default_error_404_body;
	static String default_generic_error_body;

public:
	Vector<Ref<WebServerMiddleware>> _middlewares;

	HashMap<int, void (*)(Ref<WebServerRequest>, int)> error_handler_map;
	void (*default_error_handler_func)(Ref<WebServerRequest>, int);

protected:
	Mutex _update_registered_requests_mutex;
	Vector<Ref<WebServerRequest>> _update_registered_requests;
};

#endif
