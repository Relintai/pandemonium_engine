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
			_sessions_vec.write[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.resize(_sessions_vec.size() - 1);
			_mutex.unlock();
			return;
		}
	}

	_mutex.unlock();
}

void HTTPSessionManager::delete_session(const String &session_id) {
	_mutex.lock();

	Ref<HTTPSession> s = _sessions[session_id];

	_sessions.erase(session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		Ref<HTTPSession> sess = _sessions_vec[i];

		if (sess->session_id == session_id) {
			_sessions_vec.write[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.resize(_sessions_vec.size() - 1);

			break;
		}
	}

	_mutex.unlock();

	if (!s.is_valid()) {
		return;
	}

	if (!s->id) {
		return;
	}

#if DATABASES_ENABLED
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->del(_data_table_name)->where()->wp("session_db_id", s->id)->end_command();
	b->del(_table_name)->where()->wp("id", s->id)->end_command();
	b->run_query();
#endif
}

void HTTPSessionManager::save_session(Ref<HTTPSession> session) {
#if DATABASES_ENABLED
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (!session->id) {
		b->insert(_table_name, "session_id");
		b->values();
		b->val(session->session_id);
		b->cvalues();
		b->end_command();
		b->select_last_insert_id();

		session->id = b->run()->get_last_insert_rowid();

		b->reset();
	}

	b->del(_data_table_name)->where()->wp("session_db_id", session->id)->end_command();
	int id = session->id;

	std::map<String, Variant> *m = session->get_data();
	for (std::map<String, Variant>::iterator it = m->begin(); it != m->end(); it++) {
		const Variant &val = it->second;

		if (val.is_simple_type()) {
			b->insert(_data_table_name, "session_db_id,key,value")->values()->val(id)->val(it->first)->val(val.to_string())->cvalues()->end_command();
		}
	}

	b->run_query();
#endif
}

Ref<HTTPSession> HTTPSessionManager::get_session(const String &session_id) {
	return _sessions[session_id];
}

Ref<HTTPSession> HTTPSessionManager::create_session() {
	Ref<HTTPSession> session = new HTTPSession();

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

#if DATABASES_ENABLED
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id, session_id");
	b->from(_table_name);
	// b->print();
	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		int id = r->get_cell_int(0);
		String session_id = r->get_cell(1);

		Ref<HTTPSession> s = new HTTPSession();
		s->id = id;

		s->session_id = session_id;

		add_session(s);
	}

	b->reset();

	b->select("session_db_id,key,value");
	b->from(_data_table_name);
	// b->print();
	r = b->run();

	while (r->next_row()) {
		int session_db_id = r->get_cell_int(0);

		Ref<HTTPSession> s;

		for (int i = 0; i < _sessions_vec.size(); ++i) {
			Ref<HTTPSession> ss = _sessions_vec[i];

			if (ss.is_valid() && session_db_id == ss->id) {
				s = ss;
				break;
			}
		}

		if (!s.is_valid()) {
			printf("Error: HTTPSessionManager::load_sessions(): %d sid doesn't exists!\n", session_db_id);

			continue;
		}

		String key = r->get_cell(1);
		String value = r->get_cell(2);

		s->add(key, Variant::parse_string(value));
	}
#endif
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

void HTTPSessionManager::migrate() {
	drop_table();
	create_table();
}

void HTTPSessionManager::create_table() {
#if DATABASES_ENABLED
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("session_id", 100)->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();

	tb->result = "";

	tb->create_table(_data_table_name);
	tb->integer("session_db_id")->not_null()->next_row();
	tb->varchar("key", 100)->next_row();
	tb->text("value")->not_null()->next_row();
	tb->foreign_key("session_db_id");
	tb->references(_table_name, "id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();
#endif
}
void HTTPSessionManager::drop_table() {
#if DATABASES_ENABLED
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
	tb->drop_table_if_exists(_data_table_name)->run_query();
#endif
}

HTTPSessionManager::HTTPSessionManager() {
	_table_name = "sessions";
	_data_table_name = "session_data";
}

HTTPSessionManager::~HTTPSessionManager() {
	clear();
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

	HTTPSessionManager *sm = request->server->get_session_manager();
	ERR_FAIL_COND_V(!sm, false);
	request->session = sm->get_session(sid);

	return false;
}

SessionSetupWebServerMiddleware::SessionSetupWebServerMiddleware() {
}
SessionSetupWebServerMiddleware::~SessionSetupWebServerMiddleware() {
}

void SessionSetupWebServerMiddleware::_bind_methods() {
}
