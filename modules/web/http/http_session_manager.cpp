#include "http_session_manager.h"

#include "http_session.h"

#if DATABASES_ENABLED
#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"
#endif

#include "web_server.h"
#include "web_server_cookie.h"
#include "web_server_request.h"

void HTTPSessionManager::add_session(Ref<HTTPSession> session) {
	if (!session.is_valid()) {
		printf("HTTPSessionManager::add_session: ERROR, session is null!\n");
		return;
	}

	_mutex.lock();

	_sessions_vec.push_back(session);
	_sessions[session->session_id] = session;

	_mutex.unlock();
}

void HTTPSessionManager::remove_session(Ref<HTTPSession> session) {
	if (!session.is_valid()) {
		printf("HTTPSessionManager::remove_session: ERROR, session is null!\n");
		return;
	}

	_mutex.lock();

	_sessions.erase(session->session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		if (_sessions_vec[i] == session) {
			_sessions_vec.remove(i);
			_mutex.unlock();
			return;
		}
	}

	_mutex.unlock();
}

Ref<HTTPSession> HTTPSessionManager::delete_session(const String &session_id) {
	_mutex.lock();

	Ref<HTTPSession> s = _sessions[session_id];

	_sessions.erase(session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		Ref<HTTPSession> sess = _sessions_vec[i];

		if (sess->session_id == session_id) {
			_sessions_vec.remove(i);

			break;
		}
	}

	_mutex.unlock();

	return s;
}

void HTTPSessionManager::save_session(Ref<HTTPSession> session) {
}

Ref<HTTPSession> HTTPSessionManager::get_session(const String &session_id) {
	return _sessions[session_id];
}

Ref<HTTPSession> HTTPSessionManager::create_session() {
	Ref<HTTPSession> session;
	session.instance();

	while (true) {
		session->session_id = generate_session_id(session->session_id);

		_mutex.lock();

		if (_sessions[session->session_id] == nullptr) {
			_sessions_vec.push_back(session);
			_sessions[session->session_id] = session;

			_mutex.unlock();

			return session;
		}

		_mutex.unlock();
	}

	save_session(session);

	return session;
}

void HTTPSessionManager::load_sessions() {
	clear();
}

void HTTPSessionManager::clear() {
	_mutex.lock();

	_sessions.clear();
	_sessions_vec.clear();

	_mutex.unlock();
}

String HTTPSessionManager::generate_session_id(const String &base) {
	// todo make something simpler / better

	String sid = base;

	sid += itos(Math::rand());

	return sid.sha256_text().substr(0, 20);
}

HTTPSessionManager::HTTPSessionManager() {
}

HTTPSessionManager::~HTTPSessionManager() {
}

void HTTPSessionManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_session", "session"), &HTTPSessionManager::add_session);
	ClassDB::bind_method(D_METHOD("remove_session", "session"), &HTTPSessionManager::remove_session);
	ClassDB::bind_method(D_METHOD("delete_session", "session_id"), &HTTPSessionManager::delete_session);
	ClassDB::bind_method(D_METHOD("save_session", "session"), &HTTPSessionManager::save_session);
	ClassDB::bind_method(D_METHOD("get_session", "session_id"), &HTTPSessionManager::get_session);
	ClassDB::bind_method(D_METHOD("create_session"), &HTTPSessionManager::create_session);

	ClassDB::bind_method(D_METHOD("load_sessions"), &HTTPSessionManager::load_sessions);
	ClassDB::bind_method(D_METHOD("clear"), &HTTPSessionManager::clear);
	ClassDB::bind_method(D_METHOD("generate_session_id", "base"), &HTTPSessionManager::generate_session_id, "");
}

bool SessionSetupWebServerMiddleware::_on_before_handle_request_main(Ref<WebServerRequest> request) {
	const String sid = request->get_cookie("session_id");

	if (sid == "") {
		// You could create a session here if you want to always assign sessions to visitors.
		// Example code:
		// HTTPSessionManager *sm = request->server->get_session_manager();
		// ERR_FAIL_COND_V(!sm, false);
		// Ref<HTTPSession> session = sm->create_session();
		// request->session = session;
		// request->add_cookie(::Cookie("session_id", session->session_id));

		return false;
	}

	HTTPSessionManager *sm = request->get_server()->get_session_manager();
	ERR_FAIL_COND_V(!sm, false);
	request->set_session(sm->get_session(sid));

	return false;
}

SessionSetupWebServerMiddleware::SessionSetupWebServerMiddleware() {
}
SessionSetupWebServerMiddleware::~SessionSetupWebServerMiddleware() {
}

void SessionSetupWebServerMiddleware::_bind_methods() {
}
