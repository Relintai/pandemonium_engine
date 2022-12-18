#include "http_session_manager_db.h"

#include "../http/http_session.h"

#include "modules/database/database.h"
#include "modules/database/database_connection.h"
#include "modules/database/database_manager.h"
#include "modules/database/query_builder.h"
#include "modules/database/query_result.h"
#include "modules/database/table_builder.h"

#include "../http/web_server.h"
#include "../http/web_server_cookie.h"
#include "../http/web_server_request.h"

#include "core/bind/core_bind.h"

String HTTPSessionManagerDB::get_database_table_name() {
	return _database_table_name;
}
void HTTPSessionManagerDB::set_database_table_name(const String &val) {
	_database_table_name = val;
}

String HTTPSessionManagerDB::get_database_data_table_name() {
	return _database_data_table_name;
}
void HTTPSessionManagerDB::set_database_data_table_name(const String &val) {
	_database_data_table_name = val;
}

Ref<Database> HTTPSessionManagerDB::get_database() {
	if (_database.is_valid()) {
		return _database;
	}

	return DatabaseManager::get_singleton()->get_ddb();
}

void HTTPSessionManagerDB::set_database(const Ref<Database> &db) {
	_database = db;

	// todo send event to children when it's implemented?
}

Ref<DatabaseConnection> HTTPSessionManagerDB::get_database_connection() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<DatabaseConnection>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<DatabaseConnection>());

	return conn;
}

Ref<TableBuilder> HTTPSessionManagerDB::get_table_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<TableBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<TableBuilder>());

	return conn->get_table_builder();
}

Ref<QueryBuilder> HTTPSessionManagerDB::get_query_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<QueryBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<QueryBuilder>());

	return conn->get_query_builder();
}

void HTTPSessionManagerDB::add_session(Ref<HTTPSession> session) {
	if (!session.is_valid()) {
		printf("HTTPSessionManagerDB::add_session: ERROR, session is null!\n");
		return;
	}

	_mutex.lock();

	_sessions_vec.push_back(session);
	_sessions[session->session_id] = session;

	_mutex.unlock();
}

void HTTPSessionManagerDB::remove_session(Ref<HTTPSession> session) {
	if (!session.is_valid()) {
		printf("HTTPSessionManagerDB::remove_session: ERROR, session is null!\n");
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

void HTTPSessionManagerDB::delete_session(const String &session_id) {
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

	if (!s.is_valid()) {
		return;
	}

	if (!s->id) {
		return;
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->del(_database_data_table_name)->where()->wpi("session_db_id", s->id)->end_command();
	b->del(_database_table_name)->where()->wpi("id", s->id)->end_command();
	b->run_query();
}

void HTTPSessionManagerDB::save_session(Ref<HTTPSession> session) {
	Ref<QueryBuilder> b = get_query_builder();

	if (!session->id) {
		b->insert(_database_table_name, "session_id");
		b->values();
		b->vals(session->session_id);
		b->cvalues();
		b->end_command();
		b->select_last_insert_id();

		session->id = b->run()->get_last_insert_rowid();

		b->reset();
	}

	b->del(_database_data_table_name)->where()->wpi("session_db_id", session->id)->end_command();
	int id = session->id;

	HashMap<String, Variant> *m = session->get_data();

	const String *k = NULL;

	while ((k = m->next(k))) {
		const Variant &val = m->get(*k);

		Variant::Type t = val.get_type();

		// Maybe it should be allowed?
		// Or maybe when adding stuff to the sessions the method should have a store = true bool, if false skip saving
		if (t == Variant::OBJECT) {
			continue;
		}

		String vb64 = _Marshalls::get_singleton()->variant_to_base64(val);

		b->insert(_database_data_table_name, "session_db_id,key,value")->values()->vali(id)->vals(*k)->vals(vb64)->cvalues()->end_command();
	}

	b->run_query();
}

Ref<HTTPSession> HTTPSessionManagerDB::get_session(const String &session_id) {
	return _sessions[session_id];
}

Ref<HTTPSession> HTTPSessionManagerDB::create_session() {
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

void HTTPSessionManagerDB::load_sessions() {
	clear();

	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, session_id");
	b->from(_database_table_name);
	// b->print();
	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		int id = r->get_cell_int(0);
		String session_id = r->get_cell(1);

		Ref<HTTPSession> s;
		s.instance();
		s->id = id;

		s->session_id = session_id;

		add_session(s);
	}

	b->reset();

	b->select("session_db_id,key,value");
	b->from(_database_data_table_name);
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
			printf("Error: HTTPSessionManagerDB::load_sessions(): %d sid doesn't exists!\n", session_db_id);

			continue;
		}

		String key = r->get_cell(1);
		String vb64 = r->get_cell(2);

		Variant val = _Marshalls::get_singleton()->base64_to_variant(vb64);

		s->add(key, val);
	}
}

void HTTPSessionManagerDB::clear() {
	_mutex.lock();

	_sessions.clear();
	_sessions_vec.clear();

	_mutex.unlock();
}

String HTTPSessionManagerDB::generate_session_id(const String &base) {
	// todo make something simpler / better

	String sid = base;

	sid += itos(Math::rand());

	return sid.sha256_text().substr(0, 20);
}

void HTTPSessionManagerDB::migrate() {
	drop_table();
	create_table();
}

void HTTPSessionManagerDB::create_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->create_table(_database_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("session_id", 100)->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();

	tb->result = "";

	tb->create_table(_database_data_table_name);
	tb->integer("session_db_id")->not_null()->next_row();
	tb->varchar("key", 100)->next_row();
	tb->text("value")->not_null()->next_row();
	tb->foreign_key("session_db_id");
	tb->references(_database_table_name, "id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();
}
void HTTPSessionManagerDB::drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_database_table_name)->run_query();
	tb->drop_table_if_exists(_database_data_table_name)->run_query();
}

HTTPSessionManagerDB::HTTPSessionManagerDB() {
	_database_table_name = "sessions";
	_database_data_table_name = "session_data";
}

HTTPSessionManagerDB::~HTTPSessionManagerDB() {
	clear();
}

void HTTPSessionManagerDB::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE: {
			DatabaseManager::get_singleton()->connect("migration", this, "migrate");
		} break;
		case NOTIFICATION_EXIT_TREE: {
			DatabaseManager::get_singleton()->disconnect("migration", this, "migrate");
		} break;
		default:
			break;
	}
}

void HTTPSessionManagerDB::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_database_table_name"), &HTTPSessionManagerDB::get_database_table_name);
	ClassDB::bind_method(D_METHOD("set_database_table_name", "val"), &HTTPSessionManagerDB::set_database_table_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "database_table_name"), "set_database_table_name", "get_database_table_name");

	ClassDB::bind_method(D_METHOD("get_database_data_table_name"), &HTTPSessionManagerDB::get_database_data_table_name);
	ClassDB::bind_method(D_METHOD("set_database_data_table_name", "val"), &HTTPSessionManagerDB::set_database_data_table_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "database_data_table_name"), "set_database_data_table_name", "get_database_data_table_name");

	ClassDB::bind_method(D_METHOD("get_database"), &HTTPSessionManagerDB::get_database);
	ClassDB::bind_method(D_METHOD("set_database", "val"), &HTTPSessionManagerDB::set_database);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "database", PROPERTY_HINT_RESOURCE_TYPE, "Database", 0), "set_database", "get_database");

	ClassDB::bind_method(D_METHOD("get_database_connection"), &HTTPSessionManagerDB::get_database_connection);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &HTTPSessionManagerDB::get_table_builder);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &HTTPSessionManagerDB::get_query_builder);

	ClassDB::bind_method(D_METHOD("add_session", "session"), &HTTPSessionManagerDB::add_session);
	ClassDB::bind_method(D_METHOD("remove_session", "session"), &HTTPSessionManagerDB::remove_session);
	ClassDB::bind_method(D_METHOD("delete_session", "session_id"), &HTTPSessionManagerDB::delete_session);
	ClassDB::bind_method(D_METHOD("save_session", "session"), &HTTPSessionManagerDB::save_session);
	ClassDB::bind_method(D_METHOD("get_session", "session_id"), &HTTPSessionManagerDB::get_session);
	ClassDB::bind_method(D_METHOD("create_session"), &HTTPSessionManagerDB::create_session);

	ClassDB::bind_method(D_METHOD("load_sessions"), &HTTPSessionManagerDB::load_sessions);
	ClassDB::bind_method(D_METHOD("clear"), &HTTPSessionManagerDB::clear);
	ClassDB::bind_method(D_METHOD("generate_session_id", "base"), &HTTPSessionManagerDB::generate_session_id, "");
}
