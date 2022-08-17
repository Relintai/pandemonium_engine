#ifndef WEB_ROOT_H
#define WEB_ROOT_H

#include "core/containers/hash_map.h"
#include "core/os/mutex.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "web_node.h"

#include "web_server_middleware.h"

class WebServerRequest;
class FileCache;

class WebRoot : public WebNode {
	GDCLASS(WebRoot, WebNode);

public:
	String get_www_root_path();
	void set_www_root_path(const String &val);

	Ref<FileCache> get_www_root_file_cache();

	Vector<Variant> get_middlewares();
	void set_middlewares(const Vector<Variant> &data);

	void _handle_request_main(Ref<WebServerRequest> request);

	void _handle_error_send_request(Ref<WebServerRequest> request, const int error_code);

	bool process_middlewares(Ref<WebServerRequest> request);
	bool try_send_wwwroot_file(Ref<WebServerRequest> request);
	void send_file(const String &path, Ref<WebServerRequest> request);

	void register_request_update(Ref<WebServerRequest> request);
	void unregister_request_update(Ref<WebServerRequest> request);

	WebRoot();
	~WebRoot();

	Vector<Ref<WebServerMiddleware>> _middlewares;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Mutex _update_registered_requests_mutex;
	Vector<Ref<WebServerRequest>> _update_registered_requests;

	String _www_root_path;
	Ref<FileCache> _www_root_file_cache;
};

#endif
