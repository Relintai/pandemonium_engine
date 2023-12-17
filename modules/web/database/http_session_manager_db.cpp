/*************************************************************************/
/*  http_session_manager_db.cpp                                          */
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

Ref<HTTPSession> HTTPSessionManagerDB::delete_session(const String &session_id) {
	Ref<HTTPSession> s = HTTPSessionManager::delete_session(session_id);

	if (!s.is_valid()) {
		return s;
	}

	if (!s->id) {
		return s;
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->del(_database_data_table_name)->where()->wpi("session_db_id", s->id)->end_command();
	b->del(_database_table_name)->where()->wpi("id", s->id)->end_command();
	b->run_query();

	return s;
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

		// Maybe it should be allowed?
		// Or maybe when adding stuff to the sessions the method should have a store = true bool, if false skip saving
		if (val.get_type() == Variant::OBJECT) {
			continue;
		}

		String vb64 = _Marshalls::get_singleton()->variant_to_base64(val);

		b->insert(_database_data_table_name, "session_db_id,key,value")->values()->vali(id)->vals(*k)->vals(vb64)->cvalues()->end_command();
	}

	b->run_query();
}

void HTTPSessionManagerDB::load_sessions() {
	clear();

	Ref<QueryBuilder> b = get_query_builder();

	ERR_FAIL_COND(!b.is_valid());

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

		ERR_CONTINUE_MSG(!s.is_valid(), vformat("Error: HTTPSessionManagerDB::load_sessions(): %d sid doesn't exists!", session_db_id));

		String key = r->get_cell(1);
		String vb64 = r->get_cell(2);

		Variant val = _Marshalls::get_singleton()->base64_to_variant(vb64);

		s->add(key, val);
	}
}

void HTTPSessionManagerDB::create_table() {
	call("_create_table");
}
void HTTPSessionManagerDB::drop_table() {
	call("_drop_table");
}
void HTTPSessionManagerDB::update_table(const int p_current_table_version) {
	call("_update_table", p_current_table_version);
}
void HTTPSessionManagerDB::create_default_entries(const int p_seed) {
	call("_create_default_entries", p_seed);
}

void HTTPSessionManagerDB::_create_table() {
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
void HTTPSessionManagerDB::_drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_database_table_name)->run_query();
	tb->drop_table_if_exists(_database_data_table_name)->run_query();
}
void HTTPSessionManagerDB::_update_table(const int p_current_table_version) {
}
void HTTPSessionManagerDB::_create_default_entries(const int p_seed) {
}

void HTTPSessionManagerDB::migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	call("_migrate", p_clear, p_should_seed, p_seed);
}

void HTTPSessionManagerDB::_migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	if (p_clear) {
		drop_table();
		create_table();
	} else {
#ifdef MODULE_DATABASE_ENABLED
		Ref<DatabaseConnection> conn = get_database_connection();
		ERR_FAIL_COND(!conn.is_valid());
		int ver = conn->get_table_version(_database_table_name);
		update_table(ver);
#else
		update_table(0);
#endif
	}

	if (p_should_seed) {
		create_default_entries(p_seed);
	}
}

HTTPSessionManagerDB::HTTPSessionManagerDB() {
	_database_table_name = "http_sessions";
	_database_data_table_name = "http_session_data";
}

HTTPSessionManagerDB::~HTTPSessionManagerDB() {
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

	BIND_VMETHOD(MethodInfo("_create_table"));
	BIND_VMETHOD(MethodInfo("_drop_table"));
	BIND_VMETHOD(MethodInfo("_update_table", PropertyInfo(Variant::INT, "current_table_version")));
	BIND_VMETHOD(MethodInfo("_create_default_entries", PropertyInfo(Variant::INT, "pseed")));

	ClassDB::bind_method(D_METHOD("create_table"), &HTTPSessionManagerDB::create_table);
	ClassDB::bind_method(D_METHOD("drop_table"), &HTTPSessionManagerDB::drop_table);
	ClassDB::bind_method(D_METHOD("update_table", "current_table_version"), &HTTPSessionManagerDB::update_table);
	ClassDB::bind_method(D_METHOD("create_default_entries", "pseed"), &HTTPSessionManagerDB::create_default_entries);

	ClassDB::bind_method(D_METHOD("_create_table"), &HTTPSessionManagerDB::_create_table);
	ClassDB::bind_method(D_METHOD("_drop_table"), &HTTPSessionManagerDB::_drop_table);
	ClassDB::bind_method(D_METHOD("_update_table", "current_table_version"), &HTTPSessionManagerDB::_update_table);
	ClassDB::bind_method(D_METHOD("_create_default_entries", "pseed"), &HTTPSessionManagerDB::_create_default_entries);

	BIND_VMETHOD(MethodInfo("_migrate", PropertyInfo(Variant::BOOL, "clear"), PropertyInfo(Variant::BOOL, "should_seed"), PropertyInfo(Variant::INT, "pseed")));
	ClassDB::bind_method(D_METHOD("migrate", "clear", "pseed"), &HTTPSessionManagerDB::migrate);
	ClassDB::bind_method(D_METHOD("_migrate", "clear", "pseed"), &HTTPSessionManagerDB::_migrate);
}
