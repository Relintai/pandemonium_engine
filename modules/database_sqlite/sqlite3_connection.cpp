#include "sqlite3_connection.h"

#include "core/string/print_string.h"
#include "core/string/ustring.h"
#include "sqlite3_prepared_statement.h"
#include "sqlite3_query_builder.h"
#include "sqlite3_query_result.h"
#include "sqlite3_table_builder.h"

#include "./sqlite/sqlite3.h"

#include <cstdio>

Ref<QueryBuilder> SQLite3DatabaseConnection::get_query_builder() {
	Ref<SQLite3QueryBuilder> b;
	b.instance();
	b->_connection.reference_ptr(this);

	return b;
}

Ref<TableBuilder> SQLite3DatabaseConnection::get_table_builder() {
	Ref<SQLite3TableBuilder> b;
	b.instance();
	b->_connection.reference_ptr(this);

	return b;
}

Ref<PreparedStatement> SQLite3DatabaseConnection::create_prepared_statement() {
	Ref<SQLite3PreparedStatement> b;
	b.instance();
	b->_connection.reference_ptr(this);

	return b;
}

Error SQLite3DatabaseConnection::database_connect(const String &connection_str) {
	int ret = sqlite3_config(SQLITE_CONFIG_SERIALIZED);

	if (ret != SQLITE_OK) {
		ERR_PRINT(vformat("SQLITE3 sqlite3_config failed! code: %d !", ret));
		return FAILED;
	}

	CharString cstr = connection_str.utf8();

	ret = sqlite3_open(cstr.get_data(), &conn);

	if (ret != SQLITE_OK) {
		ERR_PRINT(vformat("SQLITE3 database_connect failed! code: %d !", ret));
		return FAILED;
	}

	return OK;
}

Ref<QueryResult> SQLite3DatabaseConnection::query(const String &query) {
	Ref<Sqlite3QueryResult> res;
	res.instance();

	res->query(query, conn);

	return res;
}

void SQLite3DatabaseConnection::query_run(const String &query) {
	char *err_msg;

	CharString q = query.utf8();

	if (sqlite3_exec(conn, q.get_data(), NULL, NULL, &err_msg) != SQLITE_OK) {
		ERR_PRINT("SQLite3Database::query_run error:");
		ERR_PRINT("Query: " + query);
		ERR_PRINT("Error: " + String::utf8(err_msg));
		sqlite3_free(err_msg);
	}
}

String SQLite3DatabaseConnection::escape(const String &str) {
	char *ret;

	CharString q = str.utf8();

	ret = sqlite3_mprintf("%q", q.get_data());

	if (ret) {
		String res = String::utf8(ret);

		sqlite3_free(ret);

		return res;
	}

	return "";
}
void SQLite3DatabaseConnection::escape_to(const String &str, String *to) {
	char *ret;

	ret = sqlite3_mprintf("%q", str.utf8().get_data());

	if (ret) {
		to->operator=(String::utf8(ret));

		sqlite3_free(ret);
	}
}

int SQLite3DatabaseConnection::get_table_version(const String &table) {
	ensure_version_table_exists();
	
	Ref<QueryBuilder> qb = get_query_builder();

	qb->select("version");
	qb->from("table_versions");
	qb->where();
	qb->wps("table_name", table);
	qb->end_command();
	Ref<QueryResult> res = qb->run();

	if (!res->next_row()) {
		return 0;
	}

	return res->get_next_cell_int();
}
void SQLite3DatabaseConnection::set_table_version(const String &table, const int version) {
	ensure_version_table_exists();
	
	Ref<QueryBuilder> qb = get_query_builder();

	qb->select("id");
	qb->from("table_versions");
	qb->where();
	qb->wps("table_name", table);
	qb->end_command();
	Ref<QueryResult> res = qb->run();
	
	qb->reset();

	if (!res->next_row()) {
		if (version == -1) {
			return;
		}
	
		qb->insert("table_versions", "table_name,version");
		qb->values();
		qb->vals(table);
		qb->vali(version);
		qb->cvalues();
		qb->end_command();
		qb->run_query();
	} else {
		int id = res->get_next_cell_int();
		
		if (version != -1) {
			qb->update("table_versions");
			qb->sets();
			qb->setpi("version", version);
			qb->cset();
			qb->where()->wpi("id", id);
			qb->run_query();
		} else {
			qb->del("table_versions");
			qb->where()->wpi("id", id);
			qb->run_query();
		}
	}
}
void SQLite3DatabaseConnection::ensure_version_table_exists() {
	// https://www.sqlite.org/fileformat2.html#storage_of_the_sql_database_schema
	Ref<QueryBuilder> qb = get_query_builder();

	qb->select("count(*)");
	qb->from("sqlite_master");
	qb->where();
	qb->wps("type", "table");
	qb->land();
	qb->wps("name", "table_versions");
	qb->end_command();
	Ref<QueryResult> res = qb->run();

	ERR_FAIL_COND(!res->next_row());

	int c = res->get_next_cell_int();
	
	if (c == 0) {
		Ref<TableBuilder> tb = get_table_builder();

		tb->create_table("table_versions");
		tb->integer("id")->auto_increment()->next_row();
		tb->varchar("table_name", 256)->not_null()->next_row();
		tb->integer("version")->next_row();
		tb->primary_key("id");
		tb->ccreate_table();
		tb->run_query();
		// tb->print();
	}
}

SQLite3DatabaseConnection::SQLite3DatabaseConnection() {
	conn = nullptr;
}

SQLite3DatabaseConnection::~SQLite3DatabaseConnection() {
	if (conn) {
		sqlite3_close(conn);
	}
}
