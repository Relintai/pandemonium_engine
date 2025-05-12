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
	friend class SQLite3PreparedStatement;

	virtual Error database_connect(const String &connection_str);
	virtual Ref<QueryResult> query(const String &query);
	virtual void query_run(const String &query);

	virtual Ref<QueryBuilder> get_query_builder();
	virtual Ref<TableBuilder> get_table_builder();
	virtual Ref<PreparedStatement> create_prepared_statement();

	virtual String escape(const String &str);
	virtual void escape_to(const String &str, String *to);
	
	virtual int get_table_version(const String &table);
	virtual void set_table_version(const String &table, const int version);
	virtual void ensure_version_table_exists();

	SQLite3DatabaseConnection();
	~SQLite3DatabaseConnection();

protected:
	sqlite3 *conn;
};

#endif
