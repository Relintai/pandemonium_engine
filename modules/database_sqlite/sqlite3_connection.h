#ifndef SQLITE3_DATABASE_CONNECTION_H
#define SQLITE3_DATABASE_CONNECTION_H

#include "../database/database_connection.h"

#include "./sqlite/sqlite3.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;
class Database;
struct sqlite3;

class SQLite3DatabaseConnection : public DatabaseConnection {
public:
	void database_connect(const String &connection_str);
	Ref<QueryResult> query(const String &query);
	void query_run(const String &query);

	Ref<QueryBuilder> get_query_builder();
	Ref<TableBuilder> get_table_builder();

	String escape(const String &str);
	void escape_to(const String &str, String *to);

	SQLite3DatabaseConnection();
	~SQLite3DatabaseConnection();

protected:
	sqlite3 *conn;
};

#endif
