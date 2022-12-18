#ifndef HTTP_SESSION_MANAGER_H
#define HTTP_SESSION_MANAGER_H

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/os/mutex.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"
#include "scene/main/node.h"

#include "web_server_middleware.h"

class HTTPSession;
class WebServerRequest;

class HTTPSessionManager : public Node {
	GDCLASS(HTTPSessionManager, Node);

public:
	virtual void add_session(Ref<HTTPSession> session);
	virtual void remove_session(Ref<HTTPSession> session);
	virtual Ref<HTTPSession> delete_session(const String &session_id);
	virtual void save_session(Ref<HTTPSession> session);
	virtual Ref<HTTPSession> get_session(const String &session_id);
	virtual Ref<HTTPSession> create_session();

	virtual void load_sessions();

	virtual void clear();

	virtual String generate_session_id(const String &base = "");

	HTTPSessionManager();
	~HTTPSessionManager();

protected:
	static void _bind_methods();

	HashMap<String, Ref<HTTPSession>> _sessions;
	Vector<Ref<HTTPSession>> _sessions_vec;
	Mutex _mutex;
};

class SessionSetupWebServerMiddleware : public WebServerMiddleware {
	GDCLASS(SessionSetupWebServerMiddleware, WebServerMiddleware);

public:
	//returnring true means handled, false means continue
	bool _on_before_handle_request_main(Ref<WebServerRequest> request);

	SessionSetupWebServerMiddleware();
	~SessionSetupWebServerMiddleware();

protected:
	static void _bind_methods();
};

#endif
