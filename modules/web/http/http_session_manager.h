#ifndef HTTP_SESSION_MANAGER_H
#define HTTP_SESSION_MANAGER_H

#include "core/containers/hash_map.h"
#include "core/os/mutex.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

#include "core/object/reference.h"
#include "scene/main/node.h"

#include "web_server_middleware.h"

class HTTPSession;
class WebServerRequest;

class HTTPSessionManager : public Node {
	GDCLASS(HTTPSessionManager, Node);

public:
	void add_session(Ref<HTTPSession> session);
	void remove_session(Ref<HTTPSession> session);
	void delete_session(const String &session_id);
	void save_session(Ref<HTTPSession> session);
	Ref<HTTPSession> get_session(const String &session_id);
	Ref<HTTPSession> create_session();

	void load_sessions();

	void clear();

	virtual String generate_session_id(const String &base = "");

	virtual void migrate();
	virtual void create_table();
	virtual void drop_table();

	HTTPSessionManager();
	~HTTPSessionManager();

	HashMap<String, Ref<HTTPSession>> _sessions;
	Vector<Ref<HTTPSession>> _sessions_vec;
	Mutex _mutex;
	String _table_name;
	String _data_table_name;

protected:
	static void _bind_methods();
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
