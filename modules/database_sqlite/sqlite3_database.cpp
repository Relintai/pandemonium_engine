#include "sqlite3_database.h"

#include "database/database_manager.h"

#include "sqlite3_query_builder.h"
#include "sqlite3_query_result.h"
#include "sqlite3_table_builder.h"

Database *SQLite3Database::_creation_func() {
	return new SQLite3Database();
}

void SQLite3Database::_register() {
	DatabaseManager::_register_db_creation_func("sqlite", SQLite3Database::_creation_func);
}

void SQLite3Database::_unregister() {
	DatabaseManager::_unregister_db_creation_func("sqlite");
}

Ref<QueryBuilder> SQLite3Database::get_query_builder() {
	SQLite3QueryBuilder *b = new SQLite3QueryBuilder();
	b->_db = this;

	return b;
}

Ref<TableBuilder> SQLite3Database::get_table_builder() {
	SQLite3TableBuilder *b = new SQLite3TableBuilder();
	b->_db = this;

	return b;
}

void SQLite3Database::connect(const String &connection_str) {
	int ret = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
	if (ret != SQLITE_OK) {
		printf("SQLITE3 multithreading is not supported!\n");
	}

	ret = sqlite3_open(connection_str.c_str(), &conn);
}

Ref<QueryResult> SQLite3Database::query(const String &query) {
	Sqlite3QueryResult *res = new Sqlite3QueryResult();

	res->query(query, conn);

	return res;
}

void SQLite3Database::query_run(const String &query) {
	char *err_msg;

	if (sqlite3_exec(conn, query.c_str(), NULL, NULL, &err_msg) != SQLITE_OK) {
		printf("SQLite3Database::query_run error: \nQuery: %s \n Error:\n %s\n", query.c_str(), err_msg);
		sqlite3_free(err_msg);
	}
}

String SQLite3Database::escape(const String str) {
	char *ret;

	ret = sqlite3_mprintf("%q", str.c_str());

	if (ret) {
		String res(ret);

		sqlite3_free(ret);

		return res;
	}

	return "";
}
void SQLite3Database::escape(const String str, String *to) {
	char *ret;

	ret = sqlite3_mprintf("%q", str.c_str());

	if (ret) {
		to->operator=(ret);

		sqlite3_free(ret);
	}
}

SQLite3Database::SQLite3Database() :
		Database() {
}

SQLite3Database::~SQLite3Database() {
	if (conn)
		sqlite3_close(conn);
}
