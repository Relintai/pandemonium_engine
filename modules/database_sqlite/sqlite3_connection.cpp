#include "sqlite3_connection.h"

#include "core/string/print_string.h"
#include "core/string/ustring.h"
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

void SQLite3DatabaseConnection::database_connect(const String &connection_str) {
	int ret = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
	//if (ret != SQLITE_OK) {
		//ERR_PRINT("SQLITE3 multithreading is not supported!\n");
	//}

	//CharString cstr = connection_str.ascii();
	CharString cstr = connection_str.utf8();

	ret = sqlite3_open(cstr.get_data(), &conn);

	if (ret != SQLITE_OK) {
		ERR_PRINT(vformat("SQLITE3 database_connect failed! code: %d !", ret));
	}
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
		ERR_PRINT("Error: " + String(err_msg));
		sqlite3_free(err_msg);
	}
}

String SQLite3DatabaseConnection::escape(const String &str) {
	char *ret;

	CharString q = str.utf8();

	ret = sqlite3_mprintf("%q", q.get_data());

	if (ret) {
		String res(ret);

		sqlite3_free(ret);

		return res;
	}

	return "";
}
void SQLite3DatabaseConnection::escape_to(const String &str, String *to) {
	char *ret;

	ret = sqlite3_mprintf("%q", str.utf8().get_data());

	if (ret) {
		to->operator=(ret);

		sqlite3_free(ret);
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
