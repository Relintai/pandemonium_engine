#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/string.h"
#include "core/containers/vector.h"
#include "core/threading/mutex.h"
#include <map>

#include "core/object.h"

#include "middleware.h"

class HTTPSession;
class Request;

class SessionManager : public Object {
	RCPP_OBJECT(SessionManager, Object);

public:
	void add_session(Ref<HTTPSession> &session);
	void remove_session(Ref<HTTPSession> &session);
	void delete_session(const String &session_id);
	void save_session(Ref<HTTPSession> &session);
	Ref<HTTPSession> get_session(const String &session_id);
	Ref<HTTPSession> create_session();

	void load_sessions();

	void clear();

	virtual String generate_session_id(const String &base = "");

	virtual void migrate();
	virtual void create_table();
	virtual void drop_table();

	static SessionManager *get_singleton();

	SessionManager();
	~SessionManager();

	std::map<String, Ref<HTTPSession>> _sessions;
	Vector<Ref<HTTPSession>> _sessions_vec;
	Mutex _mutex;

protected:
	static SessionManager *_self;

	static String _table_name;
	static String _data_table_name;
};

class SessionSetupMiddleware : public Middleware {
	RCPP_OBJECT(SessionSetupMiddleware, Middleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	SessionSetupMiddleware();
	~SessionSetupMiddleware();
};

#endif